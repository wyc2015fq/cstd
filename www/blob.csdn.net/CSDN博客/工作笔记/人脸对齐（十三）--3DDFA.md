# 人脸对齐（十三）--3DDFA - 工作笔记 - CSDN博客





2018年08月21日 14:13:00[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6444
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### ****Face Alignment Across Large Poses: A 3D Solution(3DDFA2015)****

本篇文章针对的问题：

1).一般的人脸特征点定位模型都是基于可见的特征点训练得到的所以不适合侧脸的特征点定位。

2).从正脸到侧脸的大姿态变化导致人脸外观也产生巨大的变化

3).在大姿态下标注不可见的人脸特征点存在巨大的挑战。

解决方案：

提出了一个新的人脸对齐框架3DDFA（3D Dense Face Alignment），该框架主要通过CNN将一个稠密3D人脸模型来拟合人脸图片。还通过在侧脸上合成大尺度训练样本来解决第三个问题。

1).为了解决大姿态下人脸特征点不可见的问题，本文提出了将3D稠密人脸模型而不是稀疏的特征点形状拟合到人脸图片上。通过加入3D信息，可以解决由3D变换导致的外观变化和自遮挡问题。

2).为了解决3DDFA拟合过程，本文使用一个基于回归的级联卷积神经网络，CNN可以从大姿态变化的图片中抽取有用的信息。另外本文为了CNN更好的拟合3D人脸模型，专门设计了PNCC特征（Projected Normalized Coordinate Code）和WPDC(Weighted Parameter Distance Cost)代价函数。

3).为了更好的训练3DDFA，文章会构建了一个包含成对的2D人脸形状和3D人脸模型。

本文的示意图如下所示：

![](https://img-blog.csdn.net/20180821104159798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

****1 3D形变模型****

![](https://img-blog.csdn.net/20180821104232723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

S是通过PCA描述的人脸空间。

![](https://img-blog.csdn.net/20180821104326402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中p=[f,pitch,yaw,roll,t2d,αid，αexp]，通过弱透视投影（Weak Perspective Projectio ）将3D人脸映射到人脸图片平面。 

****2 网络结构****

![](https://img-blog.csdn.net/20180821104312652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里k表示迭代次数，也就是迭代估计方式，训练参数P，PNCC特征的计算与当前形状相关，可以反映当前形状的信息；算法的输出为3D人脸形状模型参数 

****3 归一化投影坐标编码（PNCC）****

![](https://img-blog.csdn.net/20180821104658981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180821104719787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

NCC（Normalized Coordinate Code ）

![](https://img-blog.csdn.net/20180821104747754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

PNCC（Projected Normalized Coordinate Code ）

****4 损失函数****

1.参数距离代价函数Parameter Distance Cost (PDC)

![](https://img-blog.csdn.net/20180821104920953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

采用CNN预测参数增量![\Delta p](https://private.codecogs.com/gif.latex?%5CDelta%20p),PDC代价函数主要用来最小化真实参数与预测参数之间的误差。

2.顶点距离代价函数Vertex Distance Cost (VDC )

![](https://img-blog.csdn.net/20180821105143319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了使3DMM更好的形变到3D人脸图片上，通过最小化拟合的和真实的3D人脸之间的顶距离误差来优化![\Delta p](https://private.codecogs.com/gif.latex?%5CDelta%20p).

3.加权参数距离代价函数 

![](https://img-blog.csdn.net/20180821105240237?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

****5 生成人脸侧脸来构造大姿态人脸库****

![](https://img-blog.csdn.net/20180821105305973?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018082110531836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过对人脸图片采用MFF（Multi-Features Framework ）拟合得到3D人脸图片，对人脸区域外的部分采用3D meshing方法进行三角剖分，然后在3D空间中对人脸图片进行旋转得到不同姿态下的人脸图片从而扩大训练样本。

****6 总结****

本篇文章提出了一个新的方法-3DDFA来解决大姿态的人脸对齐，通过级联CNN拟合一个稠密3D形变模型来解决在拟合过程中由于大姿态导致的自遮挡和高度非线性问题。文章还提出了一个侧脸算法来合成侧脸的外观特征扩大训练样本。

增加一份Pytorch版本复现的3DDFA：[https://github.com/cleardusk/3DDFA](https://github.com/cleardusk/3DDFA)



