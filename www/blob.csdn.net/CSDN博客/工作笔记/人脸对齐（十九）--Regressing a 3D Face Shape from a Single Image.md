# 人脸对齐（十九）--Regressing a 3D Face Shape from a Single Image - 工作笔记 - CSDN博客





2018年08月21日 14:49:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5484
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### ****Regressing a 3D Face Shape from a Single Image****

主要提出了对一张单独的图片进行3D人脸特征点定位的方法。该方法基于一个级联回归框架直接在3D中对人脸特征点进行定位。本文还提出了一个简单的头部姿态估计方法。

单步3D人脸形状估计：之前的一些方法都是采用两步骤，首先检测出2D人脸特征点，然后通过将3D模型拟合到检测出的2D人脸特征点上，然后进行3D人脸特征点的估计。

真实人脸特征点的定位：之前的一些工作针对被遮挡的人来拿特征点采用附近的可见特征点代替。而本文针对自身遮挡导致的特征点不可见问题采用3D真实人脸特征点来解决。该方法不仅在训练和测试当中保留了人脸形状还准确的评估出了人脸姿态的方向。

4.1 级联回归框架

![](https://img-blog.csdn.net/20180821144637693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4.2 从世界坐标系到3D人脸特征点

![](https://img-blog.csdn.net/20180821144713131?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过针孔相机模型来得到特征点的定位。Xw世界坐标系点，Xc相机坐标系的点，A相机内参，R,T相机外参

![](https://img-blog.csdn.net/20180821144855309?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

总结：

本文提出了一个新的、准确、快速的单步骤方法从一张2D图片来评估3D人脸形状。结果显示当是一幅人脸图片作为3D目标时，2D特征点的整个识别误差都会降低。增加的人脸识别领域辅助提高方法和允许对头部姿态进行简单的评估都显示了本文算法的优势。



