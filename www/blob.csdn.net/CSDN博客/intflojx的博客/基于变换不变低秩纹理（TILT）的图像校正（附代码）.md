
# 基于变换不变低秩纹理（TILT）的图像校正（附代码） - intflojx的博客 - CSDN博客


2019年01月02日 15:11:59[paulpanwang](https://me.csdn.net/intflojx)阅读数：161


## 原理简介
事实上，对于未加旋转的图像，由于图像的对称性与自相似性，我们可以将其看作是一个带噪声的低秩矩阵。当图像由端正发生旋转时，图像的对称性和规律性就会被破坏，也就是说各行像素间的线性相关性被破坏，因此矩阵的秩就会增加。
**低秩纹理映射算法(TransformInvariant Low-rank Textures，TILT)**是一种用低秩性与噪声的稀疏性进行低秩纹理恢复的算法。它的思想是通过几何变换把D所代表的图像区域校正成正则的区域，如具有横平竖直、对称特性，这些特性可以通过低秩性来进行刻画。通过计算图像透视变换各种角度之后的矩阵对应的秩，找到矩阵的秩最小的情况，利用透视变换的角度对原图像进行处理，即可将图片变为正则。当图像由端正发生旋转时，规律性被破坏矩阵的秩相应的就会增加。本文提出了变化不变纹理，用低秩性与噪声的稀疏性进行低秩恢复，通过几何变换把不规则的区域校正成端正的，如具有横平竖直、对称等特性，进而通过低秩来刻画这些特性。如下面的例子：
![](https://img-blog.csdn.net/20160719151057993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
红色窗口代表着原始的输入，绿色窗口则是通过我们模型校正过的纹理。可以这样理解，在绿色窗口中的图像一般都变成对称的了，因此图像具有低秩性。可以发现，校正过图像的秩要明显的低一些了。
### 低秩纹理的定义
现在开始进入核心部分。在这篇文章中，我们将二维纹理表示为一个定义在实数二维空间上的一个函数I0(x,y)，我们称I0为低秩纹理当且仅当一维函数![](https://img-blog.csdn.net/20160719154300786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)张成一个有限维的线性空间，也就是有下式成立
![](https://img-blog.csdn.net/20160719154441268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果r是一个有限数，我们就称I0为秩为r的纹理。容易发现，如果函数I0(x,y)的秩为1，那么函数一定是g(x).h(y)的形式。（这里我不知道该怎么解释，原文是这样说的，但是下面紧跟着给出的式子可以从分块矩阵相乘的角度进行类似理解）推广一下，一个秩为r的函数I0(x,y)可以显式地分解为r个秩为1函数的组合，即
![](https://img-blog.csdn.net/20160719155420850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面几幅图像具有理想的低秩纹理
![](https://img-blog.csdn.net/20160719155606087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上面可以看出，一个水平或者竖直的边(e)可以看成秩为1的纹理，一个角(f)可以看成秩为2的纹理。可以这样说，低秩纹理整合了很多传统的局部特征（边、角）。通过上述定义可以发现，规则对称的样式(pattern)更容易形成低秩纹理。所以，低秩纹理这一概念包含了一更为宽泛的“特征”或者区域，而不仅仅局限于边角之类的。然而，需要指出的是，现实场景中并非所有的对称纹理或形状具有低秩性，我们仅考虑那些产生低秩矩阵的对称纹理。
给定一低秩纹理（对应一函数），显然将函数扩大多少倍或者对坐标进行线性变换其秩并不会发生改变，也就是说按照我们的定义，通过![](https://img-blog.csdn.net/20160719161046153?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)变换得到的新的低秩纹理（对应一个新的函数）的与最初的低秩纹理具有相同的秩。在现实应用中，观测到的往往是变换得到的I(x,y)而希望复原原始的I0(x,y)。在本文中，无特别声明，我们认为可通过scale和transform得到彼此的两个低秩纹理等价，即![](https://img-blog.csdn.net/20160719161603943?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)成立,这个等价群众的元素具有如下形式
![](https://img-blog.csdn.net/20160719161819673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
公式3可理解为人工地将(x,y)拓展为(x,y,1)，1有单位截距的意味。
现实中，二维纹理图像函数并非连续的，我们通常都是离散采样将连续函数I0(x,y)表示为一个m*n的矩阵。对低秩纹理，我们总是假设采样窗口的大小显著大于纹理固有的秩，即![](https://img-blog.csdn.net/20160719162334187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)成立。
容易得知，只要采样率不致使由公式（2）定义的函数失真，那么得到的矩阵的秩同连续函数的秩一样。方便起见，我们将采样得到的矩阵同样记作I0(x,y)，相对其自身的维度，i0(x,y)有较低的秩。
### 通过迭代凸优化算法求解模型
通过适当的松弛模型可转化为
![](https://img-blog.csdn.net/20160719200550463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意到尽管上面的目标函数是凸的，但是其约束是非线性的，因此整个额外难题依然是非凸的。解决该问题的一个通用的技术是在当前估计和迭代中线性化其约束条件，其线性版本为
![](https://img-blog.csdn.net/20160719200956751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
至此变为线性问题且易于求解。
### 不断更新
用低秩纹理这个性质来校正图像，直接从图像中的不变特征出发，在原始的输入图像上选取变换区域进行处理，并且在该算法的模型构建过程中，加入对噪声图像的估计和约束，使得模型对图像中少量的噪声具有鲁棒性，其校正效果也比传统的方法有很大的改善，但在出现如下两种情况时，该方法的校正结果依然是误差较大或者完全失败。一个是平面问题，当输入图像的变换区域不在同一平面时，如侧面与侧面相接、侧面和地面相接，则TILT校正失败。如下图
### 使用方式
选择一张图片，点击选择左上角和右下角
![](https://img-blog.csdnimg.cn/20190102150811590.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
对图片进行处理
![](https://img-blog.csdnimg.cn/20190102150910713.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
计算透视变换后各种情况的矩阵的秩：
![](https://img-blog.csdnimg.cn/20190102150921572.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
找出矩阵的秩最小的旋转角度
![](https://img-blog.csdnimg.cn/20190102150946893.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
得到最后结果：
![](https://img-blog.csdnimg.cn/20190102151000393.png)
通过TILT可以做一些场景下的图像校正。如车牌校正、身份证照片校正、交通标志牌校正、人脸校正等。从而提高识别的准确率。
作者提供了代码可以下载，链接如下：[http://perception.csl.illinois.edu/matrix-rank/tilt.html](http://perception.csl.illinois.edu/matrix-rank/tilt.html)

