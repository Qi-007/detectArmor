#include "detect.h"

//绘制最小外接矩形
void drawRotatedRect1(Mat& image, const RotatedRect& rotatedRect, const Scalar& color, int thickness) {
    // 获取 RotatedRect 的四个顶点
    Point2f vertices[4];
    rotatedRect.points(vertices);

    // 将四个顶点连线绘制出旋转矩形
    for (int i = 0; i < 4; ++i) {
        line(image, vertices[i], vertices[(i + 1) % 4], color, thickness);
    }
}

//计算两点之间的欧几里得距离
float calculateDistance2(const Point2f& p1, const Point2f& p2){
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

//计算最小外接矩形质心
Point2f getCentroid(const Rect& rect){
    return Point2f(rect.x + rect.width / 2.0, rect.y + rect.height / 2.0);
}

Mat Armor:: Armors(const vector<pair<LightDescriptor, LightDescriptor>>& m_lights, Mat& m_frame){
    // 如果没有灯条，则直接返回原始图像
    if (m_lights.empty()) {
        return m_frame;
    }

    //绘制装甲板区域
    for(const auto& m_light : m_lights){
        const auto& leftLight = m_light.first;
        const auto& rightLight = m_light.second;

        // 将点存入容器
        vector<Point> points = {leftLight.center, rightLight.center};

        // 按 x 值从小到大排序
        sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x; // 根据 x 值升序排序
        });

        Point left_light = points[0];
        Point right_light = points[1];

        Point2f m_center; // 矩形中心点
        Size2f m_size;      // 矩形宽高
        float m_angle = (leftLight.angle + rightLight.angle) / 2; ;   // 旋转角度（顺时针）

        //寻找两个灯条质心并寻找中点
        m_center.x = (left_light.x + right_light.x) / 2;
        m_center.y = (left_light.y + right_light.y) / 2;

        m_size.height = leftLight.length + rightLight.length;
        m_size.width = calculateDistance2(left_light, right_light);

        if (m_size.width > m_size.height) {
            swap(m_size.width, m_size.height);
            m_angle += 90; // 角度调整，确保旋转后的定义正确
        }

        // 提取两个旋转矩形的顶点
        // 顶点的顺序是按矩形的边排列，依次为左上、右上、右下、左下（以逆时针顺序为准）。
        Point2f vertices1[4], vertices2[4];
        for(int i = 0; i < 4; i++){
            vertices1[i] = leftLight.point[i];
        }
        for(int i = 0; i < 4; i++){
            vertices2[i] = rightLight.point[i];
        }
 
        // 合并所有顶点
        vector<Point2f> allPoints(vertices1, vertices1 + 4);
        allPoints.insert(allPoints.end(), vertices2, vertices2 + 4);

        // 转换为整型点
        vector<Point> intPoints;
        for (const auto& pt : allPoints) {
            intPoints.emplace_back(Point(cvRound(pt.x), cvRound(pt.y)));
        }

        // 使用cv::convexHull计算凸包
        std::vector<cv::Point> hull;
        cv::convexHull(intPoints, hull);

        // 绘制凸包
        for (size_t i = 0; i < hull.size(); i++) {
            cv::line(m_frame, hull[i], hull[(i+1) % hull.size()], cv::Scalar(255, 255, 255), 2);
        }


        points.clear();
    }
    return m_frame;
}
