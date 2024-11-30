#include "detect.h"

//使用高斯函数平滑图像，减少噪声
Mat imageDispose:: imageGaussion(const Mat& m_frame){
    Mat m_blurred;
    GaussianBlur(m_frame, m_blurred, Size(5, 5), 10, 20);
    return m_blurred;
}

// 通道相减，强调蓝色
Mat imageDispose:: stressBlue(const Mat& m_frame){
    //分离通道
    vector<Mat> channels(3);
    split(m_frame, channels);   //分理出B,G,R通道

    //蓝色通道减去红色通道
    Mat m_blue_minus_red;
    subtract(channels[0], channels[2], m_blue_minus_red);     //B-R

    //归一化到可视范围
    normalize(m_blue_minus_red, m_blue_minus_red, 0, 255, NORM_MINMAX);

    return m_blue_minus_red;
}

// 通道相减，强调红色
Mat imageDispose:: stressRed(const Mat& m_frame){
    //分离通道
    vector<Mat> channels(3);
    split(m_frame, channels);   //分理出B,G,R通道

    //红色通道减去蓝色通道
    Mat m_red_minus_blue;
    subtract(channels[2], channels[0], m_red_minus_blue);     //R-B

    //归一化到可视范围
    normalize(m_red_minus_blue, m_red_minus_blue, 0, 255, NORM_MINMAX);

    return m_red_minus_blue;
}

//对彩色/灰色图像进行二值化处理
Mat imageDispose:: imageThreshold(Mat& m_gary){
    double thresh = 120.0;  //亮度阈值
    if(m_gary.channels() != 1){
        cvtColor(m_gary, m_gary, COLOR_BGR2GRAY);
    }
    // cout << m_gary.channels() << endl;
    // cvtColor(m_gary, m_gary, COLOR_BGR2GRAY);
    Mat m_binaryImage;
    // 对灰度图进行阈值化处理
    threshold(m_gary, m_binaryImage, thresh, 255, THRESH_BINARY);

    return m_binaryImage;
}

//对二值化图像进行膨胀处理
Mat imageDispose:: imageDilate(const Mat& m_binaryImage){
    // 创建结构元素（7*7矩形核）
    Mat m_kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    // 存储膨胀结果
    Mat m_dst;
    dilate(m_binaryImage, m_dst, m_kernel);

    return m_dst;
}