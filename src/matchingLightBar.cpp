#include "detect.h"

// 角度差值并进行标准化
float angle_normalization(float& angle){
    if(angle >= 90){
        angle = fabs(angle - 180);
    }
    return angle;
}

//两两匹配灯条
vector<pair<LightDescriptor, LightDescriptor>> matchingLightBar:: matchLight(const vector<LightDescriptor>& lights){
    float max_angle_diff = 5.0f;    //筛选平行角度的阈值
    float max_len_diff_ratio = 0.2;     // 灯条长度差的最大比率

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

            // 角差
            float angleDiff = fabs(leftLight.angle - rightLight.angle);

            // 长度差比率
            float lenDiff_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);
            
            // 跳过不符合条件的灯条
            if(
                angleDiff > max_angle_diff ||
                lenDiff_ratio > max_len_diff_ratio
            ){
                continue;
            }
                matched.emplace_back(leftLight, rightLight);
            }
        }
    return matched;
}