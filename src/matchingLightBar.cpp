#include "detect.h"
//计算两点之间的欧几里得距离
float calculateDistance1(const Point2f& p1, const Point2f& p2){
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// 角度差值并进行标准化
float angle_normalization(float& angle){
    if(angle >= 90){
        angle = fabs(angle - 180);
    }
    return angle;
}


//如果两个角度之间的差值小于或等于给定的阈值 threshold，则返回 true，表示这两个角度相似；
//否则，返回 false，表示角度差异太大。
vector<pair<RotatedRect, RotatedRect>> matchingLightBar:: matchRotatedRects(const vector<RotatedRect>& rects,
const float& threshold, const double& m_maxHeightDiff, const double& m_maxDistance){
    vector<pair<RotatedRect, RotatedRect>> matches;
        // 双重循环检查每对矩形
        for (size_t i = 0; i < rects.size(); ++i) {
            for (size_t j = i + 1; j < rects.size(); ++j) {
                //如果两个矩形的角度相似，则认为是匹配的

                //两两匹配灯条
                //两灯条的中心高度相差不大
                //两灯条的中心之间的距离相差不大
            if (
                abs(rects[i].center.y - rects[j].center.y) < m_maxHeightDiff &&
                calculateDistance1(rects[i].center, rects[j].center) < m_maxDistance) {
            }
                matches.emplace_back(rects[i], rects[j]);

            }
        }
    return matches;
}



//两两匹配灯条
vector<pair<LightDescriptor, LightDescriptor>> matchingLightBar:: matchLight(const vector<LightDescriptor>& lights,
const float& threshold, const double& m_maxHeightDiff, const double& m_maxDistance){
    vector<pair<LightDescriptor, LightDescriptor>> matched;
        // 双重循环检查每对矩形
        for (size_t i = 0; i < lights.size(); ++i) {
            for (size_t j = i + 1; j < lights.size(); ++j) {
                LightDescriptor leftLight = lights[i];
                LightDescriptor rightLight = lights[j];

                //角度值归一化
                angle_normalization(rightLight.angle);
                angle_normalization(leftLight.angle);

                // // cout << fabs(leftLight.angle - rightLight.angle) << endl;
                // if(fabs(leftLight.angle - rightLight.angle) > 150){
                //     cout<< leftLight.angle << "   " << rightLight.angle << endl;
                // }

            // 跳过不符合条件的灯条
            if(
                abs(leftLight.center.y - rightLight.center.y) > m_maxHeightDiff ||      // 两灯条的中心高度相差不大
                calculateDistance1(leftLight.center, rightLight.center) > m_maxDistance ||    // 两灯条的中心之间的距离相差不大
                abs(leftLight.angle - rightLight.angle) > 5 ||     // 两灯条的角度偏差不大
                fabs(leftLight.center.y - rightLight.center.y) > fabs(leftLight.center.x - rightLight.center.x) 
            ){
                continue;
            }
                matched.emplace_back(leftLight, rightLight);
            }
        }
    return matched;
}