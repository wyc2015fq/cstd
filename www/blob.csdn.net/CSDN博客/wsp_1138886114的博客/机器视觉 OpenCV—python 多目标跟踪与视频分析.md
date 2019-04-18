# 机器视觉 OpenCV—python 多目标跟踪与视频分析 - wsp_1138886114的博客 - CSDN博客





2019年01月01日 11:15:13[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：515








在上一篇中，我们了解[目标跟踪（光流）](https://blog.csdn.net/wsp_1138886114/article/details/84400392)，这次我们在视频监控与分析中，视频前后景分析、多目标检测、目标跟踪等算法进行协同工作。该算法实现了匈牙利算法与卡尔曼滤波的多目标跟踪。

贡献者：俄罗斯的Andrey Smorodov

github下载：[https://github.com/Smorodov/Multitarget-tracker.git](https://github.com/Smorodov/Multitarget-tracker.git)- 该库包含的内容有：

1.背景减: 内置来自OpenCV的 Vibe, SuBSENSE， LOBSTER，MOG2算法 ; 来自opencv_contrib的MOG, GMG 和 CNT算法;

2.前景分割: contours;

3.匹配算法: 基于加权二分图的算法或者匈牙利算法;

4.跟踪算法: 卡尔曼滤波跟踪目标中心或者目标的坐标与尺度;

5.基于LK optical flow的轨迹平滑;

6.KCF, MIL, MedianFlow, GOTURN, MOSSE or CSRT 跟踪丢失的目标和碰撞解决；

7.OpenCV的Haar人脸检测；

8.基于HOG 与 C4算法的行人检测；

9.来自 chuanqi305/MobileNet-SSD 的SSD目标检测；

10.来自 [https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/) 的YOLO和Tiny YOLO目标检测

11.遗留物检测；

12.车辆计数。
待续





