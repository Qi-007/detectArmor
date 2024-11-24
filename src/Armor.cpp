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

Mat Armor:: Armors(const vector<pair<RotatedRect, RotatedRect>>& m_lights, Mat& m_frame){
    // 如果没有灯条，则直接返回原始图像
    if (m_lights.empty()) {
        return m_frame;
    }

    //绘制装甲板区域
    for(const auto& m_light : m_lights){
        const auto& rect1 = m_light.first;
        const auto& rect2 = m_light.second;

        // 将点存入容器
        vector<Point> points = {rect1.center, rect2.center};

        // 按 x 值从小到大排序
        sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x; // 根据 x 值升序排序
        });

        Point left_light = points[0];
        Point right_light = points[1];

        Point2f m_center; // 矩形中心点
        Size2f m_size;      // 矩形宽高
        float m_angle = (rect1.angle + rect2.angle) / 2;    // 旋转角度（顺时针）

    //     //测试
    //     int x1 = left_light.center.x;
    //     int x2 = right_light.center.x;

    //    if(abs(x1) > 1000 || abs(x2) > 1000){
    //         continue;
    //    }

        //寻找两个灯条质心并寻找中点
        m_center.x = (left_light.x + right_light.x) / 2;
        m_center.y = (left_light.y + right_light.y) / 2;

        m_size.height = rect1.size.height + rect2.size.height;
        m_size.width = calculateDistance2(left_light, right_light);

        if (m_size.width > m_size.height) {
            swap(m_size.width, m_size.height);
            m_angle += 90; // 角度调整，确保旋转后的定义正确
        }


        RotatedRect m_armor(m_center, m_size, m_angle);

        drawRotatedRect1(m_frame, m_armor, Scalar(255, 255, 255), 2);

        int radius = 10;           // 圆的半径
        Scalar color1(0, 255, 0); // 绿色 (B, G, R)
        Scalar color2(255, 0, 0); // 蓝色
        Scalar color3(0, 0, 255); // 红色
        int thickness = -1;    // 线宽 (设置为 -1 填充圆)

        // 绘制圆
        circle(m_frame, left_light, radius, color1, thickness);
        circle(m_frame, right_light, radius, color2, thickness);
        circle(m_frame, m_armor.center, radius, color3, thickness);

        // cout << "m_armor的高: " << m_armor.size.height << endl;
        // cout << "m_armor的宽: " << m_armor.size.width << endl;

        points.clear();
    }
    return m_frame;
}

// // 计算旋转矩形的宽度，考虑旋转角度
// float calculateWidth(const RotatedRect& rect1, const RotatedRect& rect2, const Point& left_light, const Point& right_light) {
//     // 获取旋转矩形的角度（顺时针角度）
//     float angle = (rect1.angle + rect2.angle) / 2.0f;

//     // 计算灯条之间的水平距离
//     float deltaX = right_light.x - left_light.x;
//     float deltaY = right_light.y - left_light.y;

//     // 将其转换到旋转矩形的坐标系
//     float angleRad = angle * CV_PI / 180.0f;  // 角度转弧度
//     float cosAngle = cos(angleRad);
//     float sinAngle = sin(angleRad);

//     // 计算水平距离（将两点之间的距离投影到旋转矩形的水平轴）
//     float width = fabs(deltaX * cosAngle + deltaY * sinAngle);

//     return width;
// }
