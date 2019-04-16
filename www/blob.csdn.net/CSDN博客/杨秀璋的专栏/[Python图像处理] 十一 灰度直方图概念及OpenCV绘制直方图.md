# [Python图像处理] 十一.灰度直方图概念及OpenCV绘制直方图 - 杨秀璋的专栏 - CSDN博客





2018年11月06日 09:59:12[Eastmount](https://me.csdn.net/Eastmount)阅读数：1810
所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)[Python图像处理及图像识别](https://blog.csdn.net/column/details/26274.html)









该系列文章是讲解Python OpenCV图像处理知识，前期主要讲解图像入门、OpenCV基础用法，中期讲解图像处理的各种算法，包括图像锐化算子、图像增强技术、图像分割等，后期结合深度学习研究图像识别、图像分类应用。希望文章对您有所帮助，如果有不足之处，还请海涵~

同时推荐作者的C++图像系列知识：
[[数字图像处理] 一.MFC详解显示BMP格式图片](https://blog.csdn.net/eastmount/article/details/18238863)
[[数字图像处理] 二.MFC单文档分割窗口显示图片](https://blog.csdn.net/eastmount/article/details/18987539)
[[数字图像处理] 三.MFC实现图像灰度、采样和量化功能详解](https://blog.csdn.net/eastmount/article/details/46010637)
[[数字图像处理] 四.MFC对话框绘制灰度直方图](https://blog.csdn.net/eastmount/article/details/46237463)
[[数字图像处理] 五.MFC图像点运算之灰度线性变化、灰度非线性变化、阈值化和均衡化处理详解](https://blog.csdn.net/eastmount/article/details/46312145)
[[数字图像处理] 六.MFC空间几何变换之图像平移、镜像、旋转、缩放详解](https://blog.csdn.net/eastmount/article/details/46345299)
[[数字图像处理] 七.MFC图像增强之图像普通平滑、高斯平滑、Laplacian、Sobel、Prewitt锐化详解](https://blog.csdn.net/eastmount/article/details/46378783)
前文参考：
[[Python图像处理] 一.图像处理基础知识及OpenCV入门函数](https://blog.csdn.net/Eastmount/article/details/81748802)
[[Python图像处理] 二.OpenCV+Numpy库读取与修改像素](https://blog.csdn.net/eastmount/article/details/82120114)
[[Python图像处理] 三.获取图像属性、兴趣ROI区域及通道处理](https://blog.csdn.net/eastmount/article/details/82177300)
[[Python图像处理] 四.图像平滑之均值滤波、方框滤波、高斯滤波及中值滤波](https://blog.csdn.net/Eastmount/article/details/82216380)
[[Python图像处理] 五.图像融合、加法运算及图像类型转换](https://blog.csdn.net/Eastmount/article/details/82347501)
[[Python图像处理] 六.图像缩放、图像旋转、图像翻转与图像平移](https://blog.csdn.net/Eastmount/article/details/82454335)
[[Python图像处理] 七.图像阈值化处理及算法对比](https://blog.csdn.net/Eastmount/article/details/83548652)
[[Python图像处理] 八.图像腐蚀与图像膨胀](https://blog.csdn.net/Eastmount/article/details/83581277)
[[Python图像处理] 九.形态学之图像开运算、闭运算、梯度运算](https://blog.csdn.net/Eastmount/article/details/83651172)
[[Python图像处理] 十.形态学之图像顶帽运算和黑帽运算](https://blog.csdn.net/Eastmount/article/details/83692456)
本篇文章主要讲解灰度直方图的基本概念，Python调用OpenCV实现绘制图像直方图，基础性知识希望对您有所帮助。
**1.灰度直方图基本概率**
**2.绘制直方图**
**3.使用OpenCV统计绘制直方图**
PS：文章参考自己以前系列图像处理文章及OpenCV库函数，同时部分参考网易云李大羊老师的视频，推荐大家去学习。同时，本篇文章涉及到《计算机图形学》基础知识，请大家下来补充。

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一. 灰度直方图基本概念

**什么是灰度直方图？**

灰度直方图（histogram）是灰度级的函数，描述的是图像中每种灰度级像素的个数，反映图像中每种灰度出现的频率。横坐标是灰度级，纵坐标是灰度级出现的频率。
![](https://img-blog.csdnimg.cn/20181106083333146.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
对于连续图像，平滑地从中心的高灰度级变化到边缘的低灰度级。直方图定义为：
![](https://img-blog.csdnimg.cn/20181106083440165.png)
其中A(D)为阈值面积函数：为一幅连续图像中被具有灰度级D的所有轮廓线所包围的面积。对于离散函数，固定ΔD为1，则：H(D)=A(D)-A(D+1)。

色彩直方图是高维直方图的特例，它统计色彩的出现频率，即色彩概率分布信息。通常这需要一定的量化过程，将色彩分成若干互不重叠的种类。一般不直接在RGB色彩空间中统计，而是在将亮度分离出来后，对代表色彩部分的信息进行统计，如在HSI空间的HS子空间、YUV空间的UV子空间，以及其它反映人类视觉特点的彩色空间表示中进行。

直方图的计算方法如下：

依据定义，若图像具有L（通常L=256,即8位灰度级）级灰度，则大小为MxN的灰度图像f(x,y)的灰度直方图hist[0…L-1]可用如下计算获得。

1、初始化 hist[k]=0; k=0,…,L-1

2、统计 hist[f(x,y)]++; x=0,…,M-1, y =0,…,N-1

3、归一化 hist[f(x,y)]/=M*N
那么说了这么多，直方图究竟有什么作用呢？

在使用轮廓线确定物体边界时，通过直方图更好的选择边界阈值，进行阈值化处理；对物体与背景有较强对比的景物的分割特别有用；简单物体的面积和综合光密度IOD可以通过图像的直方图求得。


# 二. 绘制直方图

**1.基础概念**

在直方图中，横坐标表示图像中各个像素点的灰度级，纵坐标表示具有该灰度级的像素个数。
![](https://img-blog.csdnimg.cn/20181106084458107.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
假设存在一个3*3的图像，如下图所示，x数组统计的是像素点的灰度级，y数组统计的是具有该灰度级的像素个数。其中，灰度为1的像素共3个，灰度为2的像素共1个，灰度为3的像素共2个，灰度为4的像素共1个，灰度为5的像素共2个。

x = [1, 2, 3, 4, 5]

y = [3, 1, 2, 1, 2]
绘制的折线图如下所示：
![](https://img-blog.csdnimg.cn/20181106085518191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
绘制的直方图如下所示：
![](https://img-blog.csdnimg.cn/20181106085748801.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
如果灰度级为0-255（最小值0黑色，最大值255白色），同样可以绘制对应的直方图，下图是三张图片拼接而成及其对应的直方图。
![](https://img-blog.csdnimg.cn/20181106085935555.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.归一化直方图**

该直方图的横坐标表示图像中各个像素点的灰度级，纵坐标表示出现这个灰度级的概率。其计算方法如下：

(1) 先计算灰度级及对应像素的个数

x = [1, 2, 3, 4, 5]

t = [3, 1, 2, 1, 2]

(2) 统计总的像素个数

n  = (3 + 1 + 2 + 1 +2) = 9

(3) 统计各个灰度级的出现概率

y = t / n = [3/9, 1/9, 2/9, 1/9, 2/9]![](https://img-blog.csdnimg.cn/20181106090519459.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**3.绘制直方图**

主要调用matplotlib的子库pyplot实现，它提供了类似于Matlab的绘图框架，matplotlib是非常强大基础的一个Python绘图包。Provides a Matlab-like plotting framework.  导入代码如下：

```python
import matplotlib.pyplot as plt
```

其中绘制直方图主要调用hist函数实现，它根据数据源和像素级绘制直方图。函数原型如下：

```python
hist(数据源, 像素级)
参数：
数据源必须是一维数组，通常需要通过函数ravel()拉直图像
像素级一般是256，表示[0, 255]
```
![](https://img-blog.csdnimg.cn/20181106091317932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
函数ravel()将多维数组降为一维数组，格式为：

一维数组 = 多维数组.ravel()
![](https://img-blog.csdnimg.cn/20181106091452220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**4.代码实现**

```python
#encoding:utf-8
import cv2  
import numpy as np
import matplotlib.pyplot as plt

src = cv2.imread('test01.jpg')
cv2.imshow("src", src)
cv2.waitKey(0)
cv2.destroyAllWindows()

plt.hist(src.ravel(), 256)
plt.show()
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/201811060922303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181106092146784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)

# 三. 使用OpenCV统计绘制直方图

**1.函数原型**

前面讲解调用matplotlib库绘制直方图，接下来讲解使用OpenCV统计绘制直方图的例子。

直方图横坐标：图像中各个像素点的灰度级

直方图纵坐标：具有该灰度级的像素个数![](https://img-blog.csdnimg.cn/20181106092649370.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
主要调用函数calcHist()实现：
**hist = cv2.calcHist(images, channels, mask, histSize, ranges, accumulate)**

参数：- hist表示直方图，返回的是一个二维数组
- images表示原始图像
- channels表示指定通道，通道编号需要用中括号括起，输入图像是灰度图像时，它的值为[0]，彩色图像则为[0]、[1]、[2]，分别表示B、G、R
- mask表示掩码图像，统计整副图像的直方图，设为None，统计图像的某一部分直方图时，需要掩码图像
- histSize表示BINS的数量，参数子集的数目，如下图当bins=3表示三个灰度级
![](https://img-blog.csdnimg.cn/20181106093829655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181106094135235.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)- ranges表示像素值范围，例如[0, 255]
- accumulate表示累计叠加标识，默认为false，如果被设置为true，则直方图在开始分配时不会被清零，该参数允许从多个对象中计算单个直方图，或者用于实时更新直方图；多个直方图的累积结果用于对一组图像的直方图计算
![](https://img-blog.csdnimg.cn/20181106092940834.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
**2.代码实现**

首先计算图像灰度级的基本大小、形状及内容。

```python
#encoding:utf-8
import cv2  
import numpy as np
import matplotlib.pyplot as plt

src = cv2.imread('test01.jpg')
#参数:原图像 通道[0]-B 掩码 BINS为256 像素范围0-255 
hist = cv2.calcHist([src], [0], None, [256], [0,255])
print(type(hist))
print(hist.size)
print(hist.shape)
print(hist)
```

输出结果如下所示：
![](https://img-blog.csdnimg.cn/20181106094555486.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
下面是绘制图像的代码，首先补充一些matplotlib库绘制图像代码，也推荐我的文章。
[[Python数据挖掘课程] 六.Numpy、Pandas和Matplotlib包基础知识](https://blog.csdn.net/eastmount/article/details/53144633)

```python
#encoding:utf-8
import cv2  
import numpy as np
import matplotlib.pyplot as plt

#绘制sin函数曲线
x1 = np.arange(0, 6, 0.1)
y1 = np.sin(x1)
plt.plot(x1, y1)

#绘制坐标点折现
x2 = [0, 1, 2, 3, 4, 5, 6]
y2 = [0.3, 0.4, 2.5, 3.4, 4, 5.8, 7.2]
plt.plot(x2, y2)

#省略有规则递增的x2参数 
y3 = [0, 0.5, 1.5, 2.4, 4.6, 8]
plt.plot(y3, color="r")

plt.show()
```

输出结果有三条线，如下所示：
![](https://img-blog.csdnimg.cn/20181106095307122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
最后给出调用calcHist()计算B、G、R灰度级并绘制图形的代码。

```python
#encoding:utf-8
import cv2  
import numpy as np
import matplotlib.pyplot as plt

src = cv2.imread('test01.jpg')

histb = cv2.calcHist([src], [0], None, [256], [0,255])
histg = cv2.calcHist([src], [1], None, [256], [0,255])
histr = cv2.calcHist([src], [2], None, [256], [0,255])

cv2.imshow("src", src)
cv2.waitKey(0)
cv2.destroyAllWindows()

plt.plot(histb, color='b')
plt.plot(histg, color='g')
plt.plot(histr, color='r')
plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdnimg.cn/20181106095556636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/201811060922303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。最近经历的事情太多，有喜有悲，关闭了朋友圈，希望通过不断学习和写文章来忘记烦劳，将忧郁转换为动力，每周学习都记录下来。

（By：Eastmount 2018-11-06 早上12点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)











