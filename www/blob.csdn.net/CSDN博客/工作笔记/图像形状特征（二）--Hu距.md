# 图像形状特征（二）--Hu距 - 工作笔记 - CSDN博客





2016年11月18日 11:59:18[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6936
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文转自：http://blog.csdn.net/wrj19860202/article/details/6327094

1. 矩的概念

图像识别的一个核心问题是图像的特征提取，简单描述即为用一组简单的数据（图像描述量）来描述整个图像，这组数据越简单越有代表性越好。良好的特征不受光线、噪点、几何形变的干扰。图像识别发展几十年，不断有新的特征提出，而图像不变矩就是其中一个。

矩是概率与统计中的一个概念，是随机变量的一种数字特征。设*X*为随机变量，*c*为常数，*k*为正整数。则量*E*[(*x*−*c*)*k*]称为*X*关于*c*点的*k*阶矩。

比较重要的有两种情况：

1. *c*=0。这时*a**k*=*E*(*X**k*)称为*X*的*k*阶原点矩

2. *c*=*E*(*X*)。这时*μ**k*=*E*[(*X*−*E**X*)*k*]称为*X*的k阶中心矩。

一阶原点矩就是期望。一阶中心矩μ1=0，二阶中心矩*μ*2就是*X*X的方差*V**a**r*(*X*)。在统计学上，高于4阶的矩极少使用。*μ*3可以去衡量分布是否有偏。*μ*4可以去衡量分布（密度）在均值附近的陡峭程度如何。

**针对于一幅图像，我们把像素的坐标看成是一个二维随机变量(*X*,*Y*)(X,Y)，那么一幅灰度图像可以用二维灰度密度函数来表示，因此可以用矩来描述灰度图像的特征。**

不变矩(Invariant Moments)是一处高度浓缩的图像特征，具有平移、灰度、尺度、旋转不变性。M.K.Hu在1961年首先提出了不变矩的概念。1979年M.R.Teague根据正交多项式理论提出了Zernike矩。

在连续情况下，图像函数为![](http://hi.csdn.net/attachment/201104/16/0_1302917316zoT3.gif) ，那么图像的p+q阶几何矩（标准矩）定义为：

![](http://hi.csdn.net/attachment/201104/16/0_1302917330iGAG.gif)![](http://hi.csdn.net/attachment/201104/16/0_1302917334ee4g.gif)

p+q阶中心距定义为：

![](http://hi.csdn.net/attachment/201104/16/0_13029173360cTG.gif)![](http://hi.csdn.net/attachment/201104/16/0_1302917336494z.gif)

其中![](http://hi.csdn.net/attachment/201104/16/0_1302917337E3NK.gif) 和![](http://hi.csdn.net/attachment/201104/16/0_1302917337HtLV.gif) 代表图像的重心，

![](http://hi.csdn.net/attachment/201104/16/0_13029173384scj.gif)

![](http://hi.csdn.net/attachment/201104/16/0_13029173395686.gif)

对于离散的数字图像，采用求和号代替积分：

![](http://hi.csdn.net/attachment/201104/16/0_1302917340L9lY.gif)![](http://hi.csdn.net/attachment/201104/16/0_1302917340880Q.gif)

![](http://hi.csdn.net/attachment/201104/16/0_1302917340DZ75.gif)![](http://hi.csdn.net/attachment/201104/16/0_1302917340qKxG.gif)

若将*m*00看作是图像的灰度质量，则(*i*¯,*j*¯)为图像的质心坐标，那么中心矩*μ**p**a*反映的是图像灰度相对于其灰度质心的分布情况。

![](http://hi.csdn.net/attachment/201104/16/0_1302917341ew3e.gif) 和![](http://hi.csdn.net/attachment/201104/16/0_1302917341wx47.gif) 分别是图像的高度和宽度；

归一化的中心距定义为：

![](http://hi.csdn.net/attachment/201104/16/0_1302917341l3tT.gif) ；其中![](http://hi.csdn.net/attachment/201104/16/0_1302917341JPYP.gif)

利用二阶和三阶归一化中心矩构造了7个不变矩![](http://hi.csdn.net/attachment/201104/16/0_1302917341MTz1.gif) ：

![](http://hi.csdn.net/attachment/201104/16/0_1302917342ex7T.gif)

![](http://hi.csdn.net/attachment/201104/16/0_1302917342482c.gif)

![](http://hi.csdn.net/attachment/201104/16/0_1302917342EE05.gif)

![](http://hi.csdn.net/attachment/201104/16/0_1302917342sB5b.gif)

![](http://hi.csdn.net/attachment/201104/16/0_13029173428s8V.gif)



![](http://hi.csdn.net/attachment/201104/16/0_13029173439uJ7.gif)



这7个不变矩构成一组特征量，Hu.M.K在1962年证明了他们具有旋转，缩放和平移不变性。

实际上，在对图片中物体的识别过程中，只有![](http://hi.csdn.net/attachment/201104/16/0_1302917343nr99.gif) 和![](http://hi.csdn.net/attachment/201104/16/0_1302917343rnX6.gif) 不变性保持的比较好，其他的几个不变矩带来的误差比较大，有学者认为只有基于二阶矩的不变矩对二维物体的描述才是真正的具有旋转、缩放和平移不变性（![](http://hi.csdn.net/attachment/201104/16/0_1302917343roSB.gif) 和![](http://hi.csdn.net/attachment/201104/16/0_130291734464ts.gif) 刚好都是由二阶矩组成的）。不过我没有证明是否是真的事这样的。

由Hu矩组成的特征量对图片进行识别，优点就是速度很快，缺点是识别率比较低，我做过手势识别，对于已经分割好的手势轮廓图，识别率也就30%左右，对于纹理比较丰富的图片，识别率更是不堪入眼，只有10%左右。这一部分原因是由于Hu不变矩只用到低阶矩（最多也就用到三阶矩），对于图像的细节未能很好的描述出来，导致对图像的描述不够完整。

Hu不变矩一般用来识别图像中大的物体，对于物体的形状描述得比较好，图像的纹理特征不能太复杂，像识别水果的形状，或者对于车牌中的简单字符的识别效果会相对好一些。



