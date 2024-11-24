#ifndef DETECT_H
#define DETECT_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>  // 包含 sort 函数

using namespace std;
using namespace cv;

//struct默认公有
//存储数据
struct Information{
public:  
    double thresh = 180.0;  //亮度阈值
    double minRatio = 0.001;   //最小宽高比  
    double maxRatio = 10.0;   //最大宽高比
    double light_armor_ratio = 2.55;   //装甲板宽与灯条高之比
    float threshold = 5.0f;    //筛选平行角度的阈值
    double maxHeightDiff = 10.0;    //两灯条质心的最大高度差
    double maxDistance = 150.0;    //两灯条之间的最大距离
};

//图像的阈处理
class imageDispose{
public:
    Mat imageGaussion(const Mat& m_frame);   //使用高斯函数平滑图像，减少噪声
    Mat stressBlue(const Mat& m_frame);   //筛选出蓝色
    Mat stressRed(const Mat& m_frame);    //筛选出红色
    Mat imageThreshold(const Mat& m_frame, const double& thresh);    //对彩色图像进行二值化处理
    Mat imageDilate(const Mat& m_binaryImage);    //对二值化图像进行膨胀
};

//检测灯条
class findLightBar {
public:
    //按宽高比筛选轮廓
    vector<RotatedRect> ScreenAspect(const vector<vector<Point>>& m_contours, const float& minRatio, const float& maxRatio, vector<RotatedRect>& m_rightAspectRect);
};

//匹配灯条
class matchingLightBar{
public:
    // //筛选出面积差不多相等的轮廓
    // //两灯条中心点 x 坐标差距不大
    // pair<RotatedRect, RotatedRect> matchingCentroid(const vector<RotatedRect>& m_rightAspectRect, pair<RotatedRect, RotatedRect>& m_lights);

    //根据平行角度匹配灯条
    vector<pair<RotatedRect, RotatedRect>> matchRotatedRects(const vector<RotatedRect>& rects,
    const float& threshold, const double& m_maxHeightDiff, const double& m_maxDistance);
    };

//绘制并检测装甲板
class Armor{
public:
    //绘制装甲板
    Mat Armors(const vector<pair<RotatedRect, RotatedRect>>& m_lights, Mat& m_frame);
};

#endif

