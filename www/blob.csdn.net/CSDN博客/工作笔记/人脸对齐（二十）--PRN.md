# 人脸对齐（二十）--PRN - 工作笔记 - CSDN博客





2018年08月21日 14:55:19[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5793
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









Joint3D Face Reconstruction and Dense Alignment with Position Map Regression（PRN2018）

我们从之前的论文可以看出，基本的3D人脸对齐，稠密人脸对齐，人脸重建，主要分两个方向，一是3DMM+特征的方式训练人脸模型和投影矩阵（PIFAs,3DDFA等），另外一个方向就是训练像素级点对点映射（3D空间人脸和2D图像人脸）。

本文同时预测稠密人脸特征点和3D人脸重建，即3Dmesh的顶点坐标x，y，z。作者号称比之前所有的人脸重建和稠密人脸对齐方法在各种数据集上都好。

主要贡献：

1.首次同时解决端到端的人脸对齐和3D人脸重建

2.提出UV位置图，记录3D人脸位置信息

3.不同位置有不同loss权重

4.9.8ms,100FPS,在个别数据集上，有25%的性能提升。（实测1050Ti，约1s，尴尬）

### 3D人脸表示

     通常一个简单的方式是，将3D脸上的所有点，连接成一个向量，用一个网络预测，但是这种方式在训练时会增加困难，因为3D点要经过投射到2D图像，再到1维向量，丢失了点与点之间的空间信息。其他一些3D人脸表示方法（看论文）在表达能力，复杂度，性价比方面都有不足。

本文通过将mesh的顶点用uv图表示，从而达到了将mesh用一个固定大小的张量表示（256×256×3）。一个3D人脸的纹理信息可以用uv信息找到。而作者则利用了这一点，将40K的顶点映射到了256×256（60K+个顶点）的图片过去，每个点的内容为归一化后的顶点坐标x,y,z。作者这个uv图做的很好，每个顶点映射到这张uv图上都没有重叠的。如下：

![这里写图片描述](https://img-blog.csdn.net/20180322191332932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbm1pbmdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图左边是真实图片和相应的3D人脸。右边上面那一行分别是：真实图片，uv纹理图，mesh映射到uv图上；下一行分别是mesh映射到uv图上的x，y，z。

为了保持点的语义意义，UV坐标是基于3DMM计算的。数据300W-LP,模型BFM，作者将UV坐标与BFM模型对应。

有了上面的表示方法，就可以用CNN网络像预测一张图片那样直接预测顶点坐标了，下图：
![这里写图片描述](https://img-blog.csdn.net/20180322192027637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbm1pbmdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

网络表达RGB图像编码，解码的结构，前面通过10个残差块，编码256*256*3的RGB图像为8*8*512的特征图，解码部分包含17个卷积层，生成256*256*3的位置图。

损失定义为真值位置图与网络输出位置图的差异，作者采用位置图上所有点不同加权掩码的MSE损失。 如下图：

![这里写图片描述](https://img-blog.csdn.net/20180322192104873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbm1pbmdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上图分别是纹理uv图，mesh顶点uv图，不同区域的加权（4个大区域，特征点，鼻子眼睛嘴，人脸其他部分，脖子），加权比例图（16:4:3:0）。加权是在损失函数上进行的：
![这里写图片描述](https://img-blog.csdn.net/201803221924351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbm1pbmdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中P和P~ 是预测的坐标点和真是的坐标点，W就是加权图。每个W与每个P或者P~对应。

作者给出了代码：[https://github.com/YadiraF/PRNet](https://github.com/YadiraF/PRNet)

结果显示纹理有锯齿状
![这里写图片描述](https://img-blog.csdn.net/20180322192911931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbm1pbmdhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



