#include "detect.h"

//合并两个最小外接矩形
RotatedRect add_rect(const RotatedRect& rotatedRect1, const RotatedRect& rotatedRect2){
    // 提取两个旋转矩形的顶点
    // 顶点的顺序是按矩形的边排列，依次为左上、右上、右下、左下（以逆时针顺序为准）。
    Point2f vertices1[4], vertices2[4];
    rotatedRect1.points(vertices1);
    rotatedRect2.points(vertices2);

    // 合并所有顶点
    vector<Point2f> allPoints(vertices1, vertices1 + 4);
    allPoints.insert(allPoints.end(), vertices2, vertices2 + 4);

    // 计算最小外接旋转矩形
    RotatedRect boundingRotatedRect = minAreaRect(allPoints);

    // // 获取最小外接旋转矩形的顶点
    // Point2f boundingVertices[4];
    // boundingRotatedRect.points(boundingVertices);

    // // 创建图像并绘制矩形
    // cv::Mat image = cv::Mat::zeros(400, 400, CV_8UC3);

    // // 绘制原始旋转矩形
    // for (int i = 0; i < 4; i++) {
    //     cv::line(image, vertices1[i], vertices1[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);
    //     cv::line(image, vertices2[i], vertices2[(i + 1) % 4], cv::Scalar(255, 0, 0), 2);
    // }

    // // 绘制最小外接旋转矩形
    // for (int i = 0; i < 4; i++) {
    //     cv::line(image, boundingVertices[i], boundingVertices[(i + 1) % 4], cv::Scalar(0, 0, 255), 2);
    // }

    // // 显示图像
    // cv::imshow("Rotated Bounding Rectangle", image);
    // cv::waitKey(0);

    return boundingRotatedRect;
}

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

// Mat Armor:: Armors(const vector<pair<RotatedRect, RotatedRect>>& m_lights, Mat& m_frame){
//     // 如果没有灯条，则直接返回原始图像
//     if (m_lights.empty()) {
//         return m_frame;
//     }

//     //绘制装甲板区域
//     for(const auto& m_light : m_lights){
//         const auto& rect1 = m_light.first;
//         const auto& rect2 = m_light.second;

//         // 将点存入容器
//         vector<Point> points = {rect1.center, rect2.center};

//         // 按 x 值从小到大排序
//         sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
//             return a.x < b.x; // 根据 x 值升序排序
//         });

//         Point left_light = points[0];
//         Point right_light = points[1];

//         Point2f m_center; // 矩形中心点
//         Size2f m_size;      // 矩形宽高
//         float m_angle = (rect1.angle + rect2.angle) / 2; ;   // 旋转角度（顺时针）

//     //     //测试
//     //     int x1 = left_light.center.x;
//     //     int x2 = right_light.center.x;

//     //    if(abs(x1) > 1000 || abs(x2) > 1000){
//     //         continue;
//     //    }

//         //寻找两个灯条质心并寻找中点
//         m_center.x = (left_light.x + right_light.x) / 2;
//         m_center.y = (left_light.y + right_light.y) / 2;

//         m_size.height = rect1.size.height + rect2.size.height;
//         m_size.width = calculateDistance2(left_light, right_light);

//         if (m_size.width > m_size.height) {
//             swap(m_size.width, m_size.height);
//             m_angle += 90; // 角度调整，确保旋转后的定义正确
//         }


//         RotatedRect m_armor(m_center, m_size, m_angle);

//         drawRotatedRect1(m_frame, m_armor, Scalar(255, 255, 255), 2);

//         int radius = 10;           // 圆的半径
//         Scalar color1(0, 255, 0); // 绿色 (B, G, R)
//         Scalar color2(255, 0, 0); // 蓝色
//         // Scalar color3(0, 0, 255); // 红色
//         int thickness = -1;    // 线宽 (设置为 -1 填充圆)

//         // 绘制圆
//         circle(m_frame, left_light, radius, color1, thickness);
//         circle(m_frame, right_light, radius, color2, thickness);
//         // circle(m_frame, m_armor.center, radius, color3, thickness);

//         // cout << "m_armor的高: " << m_armor.size.height << endl;
//         // cout << "m_armor的宽: " << m_armor.size.width << endl;

//         points.clear();
//     }
//     return m_frame;
// }

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



// Mat Armor:: Armors(const vector<pair<RotatedRect, RotatedRect>>& m_lights, Mat& m_frame){
//     // 如果没有灯条，则直接返回原始图像
//     if (m_lights.empty()) {
//         return m_frame;
//     }

//     //绘制装甲板区域
//     for(const auto& m_light : m_lights){
//         const auto& rect1 = m_light.first;
//         const auto& rect2 = m_light.second;

//         // 将点存入容器
//         vector<Point> points = {rect1.center, rect2.center};

//         // 按 x 值从小到大排序
//         sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
//             return a.x < b.x; // 根据 x 值升序排序
//         });

//         Point left_light = points[0];
//         Point right_light = points[1];

//         Point2f m_center; // 矩形中心点
//         Size2f m_size;      // 矩形宽高
//         float m_angle = (rect1.angle + rect2.angle) / 2; ;   // 旋转角度（顺时针）

//     //     //测试
//     //     int x1 = left_light.center.x;
//     //     int x2 = right_light.center.x;

//     //    if(abs(x1) > 1000 || abs(x2) > 1000){
//     //         continue;
//     //    }

//         //寻找两个灯条质心并寻找中点
//         m_center.x = (left_light.x + right_light.x) / 2;
//         m_center.y = (left_light.y + right_light.y) / 2;

//         m_size.height = rect1.size.height + rect2.size.height;
//         m_size.width = calculateDistance2(left_light, right_light);

//         if (m_size.width > m_size.height) {
//             swap(m_size.width, m_size.height);
//             m_angle += 90; // 角度调整，确保旋转后的定义正确
//         }

//         RotatedRect m_armor = add_rect(rect1, rect2);


//         // RotatedRect m_armor(m_center, m_size, m_angle);

//         drawRotatedRect1(m_frame, m_armor, Scalar(255, 255, 255), 2);

//         points.clear();
//     }
//     return m_frame;
// }



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
        float m_angle = (rect1.angle + rect2.angle) / 2; ;   // 旋转角度（顺时针）

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

        // 提取两个旋转矩形的顶点
        // 顶点的顺序是按矩形的边排列，依次为左上、右上、右下、左下（以逆时针顺序为准）。
        Point2f vertices1[4], vertices2[4];
        rect1.points(vertices1);
        rect2.points(vertices2);

        vector<Point2f> armor;
        armor.push_back(vertices1[0]);
        armor.push_back(vertices2[1]);
        armor.push_back(vertices1[2]);
        armor.push_back(vertices2[3]);

        // 转换为整型点
        vector<Point> intPoints;
        for (const auto& pt : armor) {
            intPoints.emplace_back(Point(cvRound(pt.x), cvRound(pt.y)));
        }
    
        // 绘制矩形（闭合多边形）
        const std::vector<std::vector<cv::Point>> polygons = {intPoints}; // 多边形容器
        cv::polylines(m_frame, polygons, true, cv::Scalar(255, 255, 255), 2);   


        // // 矩形的左上角和右下角坐标
        // cv::Point topLeft = vertices1[0];   // 左上角点
        // cv::Point bottomRight = vertices2[2]; // 右下角点

        // // 设置矩形的颜色和线条粗细
        // cv::Scalar rectangleColor(0, 255, 0); // 绿色
        // int thickness = 2; // 线条厚度（正数为线宽，负数为填充矩形）

        // // 绘制矩形
        // cv::rectangle(m_frame, topLeft, bottomRight, rectangleColor, thickness);

        // // 合并所有顶点
        // vector<Point2f> allPoints(vertices1, vertices1 + 4);
        // allPoints.insert(allPoints.end(), vertices2, vertices2 + 4);

    

        points.clear();
    }
    return m_frame;
}
