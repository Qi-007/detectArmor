#include "detect.h"
//计算两点之间的欧几里得距离
float calculateDistance1(const Point2f& p1, const Point2f& p2){
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// 计算角度差值并进行标准化
//如果两个角度之间的差值小于或等于给定的阈值 threshold，则返回 true，表示这两个角度相似；
//否则，返回 false，表示角度差异太大。
bool isAngleSimilar(float angle1, float angle2, float threshold) {
    return fabs(angle1 - angle2) <= threshold;
}

//两两匹配灯条
vector<pair<RotatedRect, RotatedRect>> matchingLightBar:: matchRotatedRects(const vector<RotatedRect>& rects,
const float& threshold, const double& m_maxHeightDiff, const double& m_maxDistance){
    vector<pair<RotatedRect, RotatedRect>> matches;
        // 双重循环检查每对矩形
        for (size_t i = 0; i < rects.size(); ++i) {
            for (size_t j = i + 1; j < rects.size(); ++j) {
                //如果两个矩形的角度相似，则认为是匹配的
                //两灯条的中心高度相差不大
                //两灯条的中心之间的距离相差不大
            if (
                abs(rects[i].center.y - rects[j].center.y) < m_maxHeightDiff &&
                calculateDistance1(rects[i].center, rects[j].center) < m_maxDistance) {
                    matches.emplace_back(rects[i], rects[j]);
            }

            }
        }
    return matches;
}