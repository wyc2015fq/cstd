# 人脸对齐（十四）--LPFA - 工作笔记 - CSDN博客





2018年08月21日 14:16:40[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5390
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### Large-Pose Face Alignment via CNN-Based Dense 3D Model Fitting（LPFA）

作者试图使用3D人脸建模解决大姿态下面部特征点定位问题。其实跟PIFA2017差不多，双回归架构，3DMM模型，唯一不同的是级联CNN的细节上，以及设计的特征上。

2D的人脸形状U可以看成是3D人脸形状A通过投影变化m得到，如下图所示： 3D人脸形状模型可以表示为平均3D人脸形状 A 0 与若干表征身份、表情的基向量 A id 和 A exp 通过p参数组合而成

![](https://img-blog.csdn.net/20180821142901187?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里参数的定义，跟PIFA2017类似，都是3DMM模型。另外，级联双回归架构也类似。每级CNN的设计如下：

![](https://img-blog.csdn.net/20180821143050780?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

完整的级联架构如下：

![](https://img-blog.csdn.net/20180821143113200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



面部特征点定位问题（预测U）可以转变为同时预测投影矩阵m和3D人脸形状模型参数p

算法的整体框架通过级联6个卷积神经网络来完成这一任务：

(1) 首先以整张人脸图像作为输入，来预测投影矩阵的更新

(2) 使用更新后的投影矩阵计算当前的2D人脸形状，基于当前的2D人脸形状抽取块特征作为下一级卷积神经网络的输入，下一级卷积神经网络用于更新3D人脸形状

(3) 基于更新后的3D人脸形状，计算可得当前2D人脸形状的预测

(4) 根据新的2D人脸形状预测，抽取块特征输入到卷积神经网络中来更新投影矩阵，交替迭代优化求解投影矩阵m和3D人脸形状模型参数p，直到在训练集收敛

值得一提的是，该方法在预测3D人脸形状和投影矩阵的同时也考虑到计算每一个特征点是否可见。如果特征点不可见，则不使用该特征点上的块特征作为输入，这是普通2D人脸对齐方法难以实现的

此外，作者提出两种pose-invariant的特征Piecewise Affine-Warpped Feature (PAWF)和Direct 3D Projected Feature (D3PF)，可以进一步提升特征点定位的精度。

后面的文章里提到，LPFA在Titan X GPU上，只有0.6FPS，看看就行了。



