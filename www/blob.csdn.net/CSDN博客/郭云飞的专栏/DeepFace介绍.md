
# DeepFace介绍 - 郭云飞的专栏 - CSDN博客


2017年10月10日 19:20:15[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：5748


DeepFace是FaceBook提出来的，后续有DeepID和FaceNet出现。而且在DeepID和FaceNet中都能体现DeepFace的身影，所以DeepFace可以谓之CNN在人脸识别的奠基之作，目前深度学习在人脸识别中也取得了非常好的效果。下边介绍DeepFace的基本框架：
**一、人脸检测（face****detection****）**
DeepFace采用了基于检测点的人脸检测方法（fiducial Point Detector）
先选择6个基准点，2只眼睛中心、 1个鼻子点、3个嘴上的点。
通过LBP特征用SVR来学习得到基准点。关于LBP的介绍见论文：[Face description with local binary pattern](http://http//citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.456.1094&rep=rep1&type=pdf)
**二、人脸对齐（face****alignment****）**
2D alignment：
对Detection后的图片进行二维裁剪， scale, rotate and translate the image into six anchor locations，将人脸部分裁剪出来。
3D alignment：
找到一个3D 模型，用这个3D模型把二维人脸crop成3D人脸。67个基点，然后Delaunay三角化，在轮廓处添加三角形来避免不连续。
将三角化后的人脸转换成3D形状
三角化后的人脸变为有深度的3D三角网
将三角网做偏转，使人脸的正面朝前
最后放正的人脸
![](https://img-blog.csdn.net/20171010191958935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图的2D alignment对应(b)图，3D alignment依次对应(c) ~ (h)。

**三、人脸表示（face****verification****）**
经过3D对齐以后，形成的图像都是152×152的图像，送入CNN，其结果如下：
![](https://img-blog.csdn.net/20171012175813422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
C1：卷积层，卷积核尺寸11*11，共32个卷积核
M2：池化层，最大池化3*3，即stride = 2
C3：卷积层，卷积核尺寸9*9   ，共16个卷积核
L4： 卷积层，卷积核尺寸9*9   ，共16个卷积核。L表示local，意思是卷积核的参数不共享
L5： 卷积层，卷积核尺寸7*7   ，共16个卷积核。
L6： 卷积层，卷积核尺寸5*5   ，共16个卷积核。
F7： 全连接，4096个神经元
F8： 全连接，4030个神经元
前三层的目的在于提取低层次的特征，比如简单的边和纹理。其中*Max-pooling*层使得卷积的输出对微小的偏移情况更加鲁棒。但没有用太多的Max-pooling层，因为太多的Max-pooling层会使得网络*损失图像信息*。
紧接着的三层都是使用参数不共享的卷积核，之所以使用参数不共享，有如下原因：
对齐的人脸图片中，不同的区域会有不同的统计特征，卷积的局部稳定性假设并不存在，所以使用相同的卷积核会导致信息的丢失
不共享的卷积核并不增加抽取特征时的计算量，而会增加训练时的计算量
使用不共享的卷积核，需要训练的参数量大大增加，因而需要很大的数据量，然而这个条件本文刚好满足。
全连接层将上一层的每个单元和本层的所有单元相连，用来捕捉人脸图像不同位置的特征之间的相关性。其中，第7层（4096-d）被用来表示人脸。全连接层的输出可以用于Softmax的输入，Softmax层用于分类。
**四、人脸表示的归一化**
对于输出的4096维向量：
先每一维进行归一化，即对于结果向量中的每一维，都要除以该维度在整个训练集上的最大值。
每个向量进行L2归一化
**五、分类**
得到表示后，使用了多种方法进行分类：
直接算内积
加权的卡方距离
使用Siamese网络结构







