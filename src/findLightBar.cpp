#include "detect.h"

//宽高比
//按轮廓的宽高比筛选轮廓
vector<RotatedRect> findLightBar:: ScreenAspect(const vector<vector<Point>>& m_contours, const float& minRatio, const float& maxRatio, vector<RotatedRect>& m_rightAspectRect){
    // //绘制轮廓
    // vector<vector<Point>> m_contours;
    // findContours(m_binaryImage, m_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for(int i = 0; i < m_contours.size(); i++){
        //使用最小外接矩形框选轮廓
        RotatedRect roi = minAreaRect(m_contours[i]);
        //计算长宽比
        float aspectRatio = static_cast<float> (roi.size.width) / (roi.size.height);
        if(aspectRatio > minRatio && aspectRatio < maxRatio){
            m_rightAspectRect.push_back(roi);
        }
    }
    return m_rightAspectRect;
}
