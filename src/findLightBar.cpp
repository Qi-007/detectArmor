#include "detect.h"

// //宽高比
// //按轮廓的宽高比筛选轮廓
// vector<RotatedRect> findLightBar:: ScreenAspect(const vector<vector<Point>>& m_contours, const float& minRatio, 
// const float& maxRatio, const float& minArea, vector<RotatedRect>& m_rightAspectRect){
//     // //绘制轮廓
//     // vector<vector<Point>> m_contours;
//     // findContours(m_binaryImage, m_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
//     for(int i = 0; i < m_contours.size(); i++){
//         //使用最小外接矩形框选轮廓
//         RotatedRect roi = minAreaRect(m_contours[i]);
//         //计算长宽比
//         float aspectRatio = static_cast<float> (roi.size.width) / (roi.size.height);
//         if(aspectRatio > minRatio && aspectRatio < maxRatio && roi.size.area() > minArea){
//             // cout << roi.size.area() << endl;
//             m_rightAspectRect.push_back(roi);
//         }
//     }
//     return m_rightAspectRect;
// }

//按轮廓的宽高比.面积大小筛选轮廓
vector<LightDescriptor> findLightBar:: Lights(const vector<vector<Point>>& contours){
    float minArea = 200.0f;   //最小面积
    float maxArea = 5000.0f;   // 最大面积
    double minRatio = 0.10;   //最小宽高比  
    double maxRatio = 0.20;   //最大宽高比
    //存储筛选过的灯条
    vector<LightDescriptor> m_light;

    // 遍历轮廓，拟合椭圆并计算外接矩形
    // 因为灯条是椭圆型的，所以用椭圆去拟合轮廓，再直接获取旋转外接矩形
    for (size_t i = 0; i < contours.size(); ++i) {
        if (contours[i].size() < 5) continue; // 拟合椭圆需要至少5个点
        // 获取椭圆的外接矩形
        RotatedRect Light_Rec = fitEllipse(contours[i]);
          //计算宽高比
        float aspectRatio = static_cast<float> (Light_Rec.size.width) / (Light_Rec.size.height); 
        if(aspectRatio > minRatio && aspectRatio < maxRatio && Light_Rec.size.area() > minArea && Light_Rec.size.area() < maxArea){
            m_light.push_back(Light_Rec);
        }
   }
    return m_light;
}