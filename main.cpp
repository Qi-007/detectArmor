#include <iostream>
#include <thread>
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

// 通道相减图像的预处理
void minus_dispose(const Mat& m_frame, Mat& m_dst){
    imageDispose minus_dispose;

    // 使用高斯函数平滑图像，减少噪声
    Mat blurred, red_minus_blue, blue_minus_red, binary, open, close;
    blurred = minus_dispose.imageGaussion(m_frame);
    // 红蓝通道相减，强调红色区域
    red_minus_blue = minus_dispose.stressRed(blurred);

    // // 蓝红通道相减，强调蓝色区域
    // blue_minus_red = minus_dispose.stressBlue(blurred);

    // 对通道相减图像进行二值化处理
    binary = minus_dispose.imageThreshold(red_minus_blue, 100);

    // imshow("binary", binary);

    // 对二值化后的图像进行开运算处理
    open = minus_dispose.imageOpen(binary);

    // 进行闭运算处理
    close = minus_dispose.imageClose(open);

    // 对闭运算后的图像进行膨胀处理
    m_dst = minus_dispose.imageDilate(close);
}

// 原图像的预处理
void image_dispose(const Mat& m_frame, Mat& m_binaryImage){
    imageDispose frame_dispose;

    // 使用高斯函数平滑图像，减少噪声
    Mat blurred;
    blurred = frame_dispose.imageGaussion(m_frame);

    // 对原图像进行二值化处理
    m_binaryImage = frame_dispose.imageThreshold(blurred, 140);
}

int main(){
    //读取视频文件
    VideoCapture cap("/home/zjq/Desktop/detect_video/前哨站/红方前哨站公路视角半速.mp4");
    if(!cap.isOpened()){
        cout << "视频加载失败" << endl;
    }

    // Mat frame,  //原图像
    //     blurred,    //高斯函数去噪
    //     red_minus_blue,   //红蓝通道相减后的图像，强调红色
    //     blue_minus_red,   //蓝红通道相减后的图像，强调蓝色
    //     binaryImage,      //通道相减图像二值化后的图像
    //     binary,     // 原图像二值化后的图像
    //     dst;     //膨胀后的图像

    Mat frame,      // 原图像
        binaryImage,    // 原图处理后的图像
        dst;     // 通道相减处理后的图像

    vector<vector<Point>> all_contours;     //未经筛选的轮廓
    vector<LightDescriptor> lights;       //筛选过宽高比的矩形
    vector<pair<LightDescriptor, LightDescriptor>> matching_lights;    //根据倾斜角等筛选出的配对灯条
    vector<pair<LightDescriptor, LightDescriptor>> foundArmor;      // 识别后的装甲板

    // // 用户选择 ROI 区域
    // cap >> frame;
    // Rect roi = selectROI("Select ROI", frame, false, false);
    // if (roi.width == 0 || roi.height == 0) {
    //     std::cerr << "未选择有效的 ROI！" << std::endl;
    //     return -1;
    // }

    while(true){
    //读取每一帧
    cap >> frame;
    if(frame.empty()){
        break;
    }

    // // 图像的预处理
    // imageDispose frame_dispose;

    // // 使用高斯函数平滑图像，减少噪声
    // blurred = frame_dispose.imageGaussion(frame);

    // // 红蓝通道相减，强调红色区域
    // red_minus_blue = frame_dispose.stressRed(blurred);

    // // // 蓝红通道相减，强调蓝色区域
    // // blue_minus_red = frame_dispose.stressBlue(blurred);

    // // 对通道相减图像进行二值化处理
    // binaryImage = frame_dispose.imageThreshold(red_minus_blue, 120);

    // 通道相减图像的二值化处理
    thread minusDispose(minus_dispose, ref(frame), ref(dst));

    // 原图像的二值化处理
    thread frameDispose(image_dispose, ref(frame), ref(binaryImage));

    // 等待线程执行完毕
    minusDispose.join();
    frameDispose.join();

    // imshow("dst", dst);
    // imshow("binaryImage", binaryImage);

    Mat image;
    bitwise_and(dst, binaryImage, image);
    imshow("image", image);

    // // 对二值化图像进行膨胀
    // dst = frame_dispose.imageDilate(binaryImage);   

    // 寻找轮廓
    findContours(image, all_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //识别灯条
    findLightBar all_lightBar;
    lights = all_lightBar.Lights(all_contours);

    //    // 尝试绘制识别出的所有灯条
    // for(size_t i = 0; i < lights.size(); i++){
    //     drawLight(frame, lights[i], Scalar(0, 255, 0), 2);
    //     // // 打印每个轮廓的宽高比
    //     // cout << lights[i].length / lights[i].width << endl;
    // }
    
    // // 尝试绘制识别出的所有位于ROI区域的灯条
    // rectangle(frame, roi, Scalar(0, 255, 0), 2);
    // for(size_t i = 0; i < lights.size(); i++){
    //     // 检查是否有顶点在ROI内
    //     bool section = false;
    //     for(int j = 0; j < 4; j++){
    //         if(roi.contains(lights[i].point[j])){
    //             section = true;
    //             break;
    //         }
    //     }
    //     if(section){
    //         // 两者有交集
    //         drawLight(frame, lights[i], Scalar(255, 255, 255), 2);
    //         // // 打印每个轮廓的角度
    //         // cout << "angle:" << lights[i].angle << endl; 
    //         // 打印每个轮廓的宽高比
    //         // cout << lights[i].length / lights[i].width << endl;
    //     }else{
    //         continue;
    //     }
    // }

    // // // imshow("frame", frame);

    //匹配灯条
    matchingLightBar right_lightBar;

    //两两匹配灯条
    matching_lights = right_lightBar.matchLight(lights);

    // // 尝试绘制识别出的所有位于ROI区域的灯条
    // rectangle(frame, roi, Scalar(0, 255, 0), 2);
    // for(size_t i = 0; i < lights.size(); i++){
    //     // 检查是否有顶点在ROI内
    //     bool section = false;
    //     for(int j = 0; j < 4; j++){
    //         if(roi.contains(lights[i].point[j])){
    //             section = true;
    //             break;
    //         }
    //     }
    //     if(section){
    //     //两两匹配灯条
    //     matching_lights = right_lightBar.matchLight(lights);
    //     }else{
    //         continue;
    //     }
    // }

    // // 遍历vector并输出每对RotatedRect
    // for (const auto& lights : matching_lights){
    //     const auto& leftLight = lights.first;
    //     const auto& rightLight = lights.second;
    //     drawLight(frame, leftLight, Scalar(255, 255, 255), 2);      // 左灯条为绿色
    //     drawLight(frame, rightLight, Scalar(255, 255, 255), 2);     // 右灯条为蓝色
    // }

    // 识别装甲板
    findArmor armors;
    foundArmor = armors.find_Armor(matching_lights);

    // 匹配装甲板
    matchingArmor all_armors;
    frame = all_armors.matchingArmors(foundArmor, frame);

    // // 尝试绘制识别出的所有位于ROI区域的灯条
    // rectangle(frame, roi, Scalar(0, 255, 0), 2);
    // for(size_t i = 0; i < lights.size(); i++){
    //     // 检查是否有顶点在ROI内
    //     bool section = false;
    //     for(int j = 0; j < 4; j++){
    //         if(roi.contains(lights[i].point[j])){
    //             section = true;
    //             break;
    //         }
    //     }
    //     if(section){
    //         // 识别装甲板
    //         findArmor armors;
    //         foundArmor = armors.find_Armor(matching_lights);

    //         // 匹配装甲板
    //         matchingArmor all_armors;
    //         frame = all_armors.matchingArmors(foundArmor, frame);
    //     }else{
    //         continue;
    //     }
    // }


    imshow("前哨站", frame);

    foundArmor.clear();     // 清空上一帧的装甲板
    matching_lights.clear();    // 清空上一帧的配对灯条
    lights.clear();    // 清空上一帧筛选的矩形
    all_contours.clear();       // 清空上一帧的轮廓

    char c = waitKey(0);    // cap.get(CAP_PROP_FPS)
    if(c == 'q' || c == 27){
        break;
    }
    }
    cap.release();
    destroyAllWindows();
    return 0;
}