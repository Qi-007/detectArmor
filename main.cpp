#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "detect.h"

using namespace std;
using namespace cv;

//绘制最小外接矩形(灯条类)
void drawLight(Mat& image, const LightDescriptor& light, const Scalar& color, int thickness) {
    // 将四个顶点连线绘制出旋转矩形
    for (int i = 0; i < 4; ++i) {
        line(image, light.point[i], light.point[(i + 1) % 4], color, thickness);
    }
    return;
}

int main(){
    //读取视频文件
    VideoCapture cap("近景.avi");
    if(!cap.isOpened()){
        cout << "视频加载失败" << endl;
    }

    Mat frame,  //原图像
        blurred,    //高斯函数去噪
        red_minus_blue,   //红蓝通道相减后的图像，强调红色
        blue_minus_red,   //蓝红通道相减后的图像，强调蓝色
        binaryImage,      //二值化后的图像
        dst;     //膨胀后的图像

    vector<vector<Point>> all_contours;     //未经筛选的轮廓
    vector<LightDescriptor> lights;       //筛选过宽高比的矩形
    vector<pair<LightDescriptor, LightDescriptor>> matching_lights;    //根据倾斜角等筛选出的配对灯条
    vector<pair<LightDescriptor, LightDescriptor>> foundArmor;      // 识别后的装甲板

    while(true){
    //读取每一帧
    cap >> frame;
    if(frame.empty()){
        break;
    }

    // 图像的预处理
    imageDispose frame_dispose;

    // 使用高斯函数平滑图像，减少噪声
    blurred = frame_dispose.imageGaussion(frame);

    // 红蓝通道相减，强调红色区域
    red_minus_blue = frame_dispose.stressRed(blurred);

    // // 蓝红通道相减，强调蓝色区域
    // blue_minus_red = frame_dispose.stressBlue(frame);

    // 对彩色图像进行二值化处理
    binaryImage = frame_dispose.imageThreshold(red_minus_blue);

    // 对二值化图像进行膨胀
    dst = frame_dispose.imageDilate(binaryImage);    

    // 寻找轮廓
    findContours(dst, all_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //识别灯条
    findLightBar all_lightBar;
    lights = all_lightBar.Lights(all_contours);
    
    // // 尝试绘制识别出的所有灯条
    // for(size_t i = 0; i < lights.size(); i++){
    //     drawLight(frame, lights[i], Scalar(255, 255, 255), 2);
    // }

    //匹配灯条
    matchingLightBar right_lightBar;

    //两两匹配灯条
    matching_lights = right_lightBar.matchLight(lights);

    // // 遍历vector并输出每对RotatedRect
    // for (const auto& lights : matching_lights){
    //     const auto& leftLight = lights.first;
    //     const auto& rightLight = lights.second;
    //     drawLight(frame, leftLight, Scalar(255, 255, 255), 2);
    //     drawLight(frame, rightLight, Scalar(255, 255, 255), 2);
    // }

    // 识别装甲板
    findArmor armors;
    foundArmor = armors.find_Armor(matching_lights);

    // 匹配装甲板
    matchingArmor all_armors;
    frame = all_armors.matchingArmors(foundArmor, frame);

    imshow("前哨站", frame);

    foundArmor.clear();     // 清空上一帧的装甲板
    matching_lights.clear();    // 清空上一帧的配对灯条
    lights.clear();    // 清空上一帧筛选的矩形
    all_contours.clear();       // 清空上一帧的轮廓

    waitKey(1000/cap.get(CAP_PROP_FPS));

    }
    cap.release();
    destroyAllWindows();
    return 0;
}