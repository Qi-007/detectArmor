#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "detect.h"

using namespace std;
using namespace cv;

//绘制最小外接矩形
void drawRotatedRect(Mat& image, const RotatedRect& rotatedRect, const Scalar& color, int thickness) {
    // 获取 RotatedRect 的四个顶点
    Point2f vertices[4];
    rotatedRect.points(vertices);

    // 将四个顶点连线绘制出旋转矩形
    for (int i = 0; i < 4; ++i) {
        line(image, vertices[i], vertices[(i + 1) % 4], color, thickness);
    }
}

int main(){
    //读取视频文件
    VideoCapture cap("前哨站.avi");
    if(!cap.isOpened()){
        cout << "视频加载失败" << endl;
    }
    
    //读取阈值
    Information all_information;

    double thresh = all_information.thresh;
    double minRatio = all_information.minRatio; 
    double maxRatio = all_information.maxRatio;
    double light_armor_ratio = all_information.light_armor_ratio;
    float threshold = all_information.threshold;
    double maxHeightDiff = all_information.maxHeightDiff;
    double maxDistance = all_information.maxDistance;

    Mat frame,  //原图像
        blurred,
        red_minus_blue,   //红蓝通道相减后的图像，强调红色
        blue_minus_red,   //蓝红通道相减后的图像，强调蓝色
        binaryImage,      //二值化后的图像
        dst;     //膨胀后的图像

    vector<vector<Point>> all_contours;     //未经筛选的轮廓
    vector<vector<Point>> rightAreaContours;    // 筛选过面积的轮廓
    vector<RotatedRect> rightAspectRect;       //筛选过宽高比的矩形
    // vector<RotatedRect> filteredRectangles;    // 筛选过倾斜角的矩形
    vector<pair<RotatedRect, RotatedRect>> lights;    //根据倾斜角筛选
    pair<RotatedRect, RotatedRect> light;    //匹配过后的灯条 

    namedWindow("前哨站", WINDOW_AUTOSIZE);
    // namedWindow("binaryImage", WINDOW_AUTOSIZE);

    while(true){
    //读取每一帧
    cap >> frame;
    if(frame.empty()){
        break;
    }

    //图像的阈处理
    imageDispose frame_dispose;

    //使用高斯函数平滑图像，减少噪声
    blurred = frame_dispose.imageGaussion(frame);

    //红蓝通道相减，强调蓝色区域
    red_minus_blue = frame_dispose.stressRed(blurred);
    // //蓝红通道相减，强调红色区域
    // blue_minus_red = frame_dispose.stressBlue(blurred);

    //对彩色图像进行二值化处理
    binaryImage = frame_dispose.imageThreshold(red_minus_blue, thresh);

    //对二值化图像进行膨胀
    dst = frame_dispose.imageDilate(binaryImage);    

    //识别灯条
    findLightBar all_lightBar;
    findContours(dst, all_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //筛选灯条
    //按轮廓的宽高比筛选轮廓
    rightAspectRect = all_lightBar.ScreenAspect(all_contours, minRatio, maxRatio, rightAspectRect);
    
    //匹配灯条
    matchingLightBar right_lightBar;

    //根据矩形的倾斜角度两两匹配灯条
    lights = right_lightBar.matchRotatedRects(rightAspectRect, threshold, maxHeightDiff, maxDistance);

    // // 遍历vector并输出每对RotatedRect
    // for (const auto& RotatedRect : lights){
    //     const auto& rect1 = RotatedRect.first;
    //     const auto& rect2 = RotatedRect.second;
    //     drawRotatedRect(frame, rect1, Scalar(255, 255, 255), 2);
    //     drawRotatedRect(frame, rect2, Scalar(255, 255, 255), 2);
    // }

    //识别装甲板
    Armor all_armors;
    frame = all_armors.Armors(lights, frame);

    imshow("前哨站", frame);

    rightAspectRect.clear();    // 清空上一帧筛选的矩形
    all_contours.clear();       // 清空上一帧的轮廓

    waitKey(1000/cap.get(CAP_PROP_FPS));

    }
    cap.release();
    destroyAllWindows();
    return 0;
}