# detectArmor
识别装甲板
有问题，但暂时得不到解决，能力不够
匹配灯条后直接绘制装甲板，没有匹配，但是装甲板的width出现问题

写于2024.11.23



问题得到解决
使用凸包绘制装甲板
没有匹配装甲板是因为在匹配灯条的时候已经将一对灯条分离出来
所以直接绘制装甲板即可
不太严谨
有待改进

写于2024.11.30



可以识别部分测试视频，狙击点视角还不行
可能是因为使用膨胀后灯条面积过小造成了黏连
等待学长指令

写于2024.12.5


因为不能很好识别装甲板侧面，进行了修改
其他视频没问题，还是狙击点视角，外加/home/zjq/Desktop/detect_video/前哨站/红方前哨站公路视角半速.mp4,
该视频装甲板转第二圈经过右边时有一帧识别不上，不知道是什么问题，
有待改进

写于2024.12.7



