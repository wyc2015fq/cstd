# 人脸检测（十六）--Cascade CNN - 工作笔记 - CSDN博客





2018年06月01日 09:56:53[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7000
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









转自：https://blog.csdn.net/xzzppp/article/details/51692907

论文：A Convolutional Neural Network Cascade for FaceDetection

1、简介

    CascadeCNN是对经典的Violajones方法的深度卷积网络实现，是一种检测速度较快的人脸检测方法。使用VGA图片，在CPU上达到14FPS，GPU上达到100FPS，在FDDB上达到85.1%的召回率和87%的准确率。

2、总体框架







    下图是该方法的整个流程示意图，可以看出是3阶级联。

![](https://img-blog.csdn.net/20180601100045320)



          级联结构中有6个CNN，3个CNN用于人脸非人脸二分类，另外3个CNN用于人脸区域的边框校正。给定一幅图像，12-net密集扫描整幅图片，拒绝90%以上的窗口。剩余的窗口输入到12-calibration-net中调整大小和位置，以接近真实目标。接着输入到NMS中，消除高度重叠窗口。下面网络与上面类似。

![](https://img-blog.csdn.net/20180601100052690)

12-net, 24-net and48-net的CNN结构图

2.1  net网络



         12-net：使用12×12的检测窗口，以步长为4，在W×H的图片上滑动，得到检测窗口。实际应用中，将图像尺度变换为金字塔结构，每层都作为12-net输入。12-net可以拒绝90%以上的窗口，大大减小了下级网络的负担。

         24-net：在12-net的基础上，将24×24的图片缩放到12×12，输入到12-net全连接层，与12-net并联，最后一起输出到24-net全连接层。

2.2  calibration-net网络

![](https://img-blog.csdn.net/20180601100059948)



         该网络用于窗口校正，使用三个偏移变量：Xn:水平平移量，Yn:垂直平移量，Sn:宽高比缩放。候选框口(x,y,w,h)中，(x,y)表示左上点坐标，(w,h)表示宽和高。

我们要将窗口的控制坐标调整为：

![](https://img-blog.csdn.net/2018060110010795)



          这项工作中，我们有N=5×3×3=45种模式。偏移向量三个参数包括以下值：

          Sn：(0.83,0.91,1.0,1.10,1.21)

          Xn：(-0.17,0,0.17)

          Yn：(-0.17,0,0.17)

          同时对偏移向量三个参数进行校正。



![](https://img-blog.csdn.net/20180601100114201)

校正网络的结构如下图所示：

![](https://img-blog.csdn.net/20180601100318168)

Cascade CNN一定程度上解决了传统方法在开放场景中对光照、角度等敏感的问题，但是该框架的第一级还是基于密集滑动窗口的方式进行窗口过滤，在高分辨率存在大量小人脸（tiny face）的图片上限制了算法的性能上限。

3、检测结果



     在FDDB数据集上进行测试，每阶段的检测窗口个数和召回率如图所示：

![](https://img-blog.csdn.net/20180601100121141)







