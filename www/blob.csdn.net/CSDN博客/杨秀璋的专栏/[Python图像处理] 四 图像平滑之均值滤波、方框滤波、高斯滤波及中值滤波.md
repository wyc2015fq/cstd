# [Python图像处理] 四.图像平滑之均值滤波、方框滤波、高斯滤波及中值滤波 - 杨秀璋的专栏 - CSDN博客





2018年09月02日 23:27:36[Eastmount](https://me.csdn.net/Eastmount)阅读数：4418
所属专栏：[Python图像处理及图像识别](https://blog.csdn.net/column/details/26274.html)[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)









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
本篇文章主要讲解Python调用OpenCV实现图像平滑，包括四个算法：均值滤波、方框滤波、高斯滤波和中值滤波。全文均是基础知识，希望对您有所帮助。知识点如下：
**1.图像平滑**
**2.均值滤波**
**3.方框滤波**
**4.高斯滤波**
**5.中值滤波**
PS：本文介绍图像平滑，想让大家先看看图像处理的效果，后面还会补充一些基础知识供大家学习。文章参考自己的博客及网易云课堂李大洋老师的讲解，强烈推荐大家学习。

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一.图像平滑

**1.图像增强**

图像增强是对图像进行处理，使其比原始图像更适合于特定的应用，它需要与实际应用相结合。对于图像的某些特征如边缘、轮廓、对比度等，图像增强是进行强调或锐化，以便于显示、观察或进一步分析与处理。图像增强的方法是因应用不同而不同的，研究内容包括：(参考课件和左飞的《数字图像处理》)
![](https://img-blog.csdn.net/20150605170110605)
**2.图像平滑**

图像平滑是一种区域增强的算法，平滑算法有邻域平均法、中指滤波、边界保持类滤波等。在图像产生、传输和复制过程中，常常会因为多方面原因而被噪声干扰或出现数据丢失，降低了图像的质量（某一像素，如果它与周围像素点相比有明显的不同，则该点被噪声所感染）。这就需要对图像进行一定的增强处理以减小这些缺陷带来的影响。

简单平滑-邻域平均法
**3.邻域平均法**

图像简单平滑是指通过邻域简单平均对图像进行平滑处理的方法，用这种方法在一定程度上消除原始图像中的噪声、降低原始图像对比度的作用。它利用卷积运算对图像邻域的像素灰度进行平均，从而达到减小图像中噪声影响、降低图像对比度的目的。

但邻域平均值主要缺点是在降低噪声的同时使图像变得模糊，特别在边缘和细节处，而且邻域越大，在去噪能力增强的同时模糊程度越严重。![](https://img-blog.csdn.net/20150605184726788)
首先给出为图像增加噪声的代码。

```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)
rows, cols, chn = img.shape

#加噪声
for i in range(5000):    
    x = np.random.randint(0, rows) 
    y = np.random.randint(0, cols)    
    img[x,y,:] = 255

cv2.imshow("noise", img)
           
#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下所示：
![](https://img-blog.csdn.net/2018083010311261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 二.均值滤波

**1.原理**

均值滤波是指任意一点的像素值，都是周围N*M个像素值的均值。例如下图中，红色点的像素值为蓝色背景区域像素值之和除25。
![](https://img-blog.csdn.net/20180830100734448?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中红色区域的像素值均值滤波处理过程为：
((197+25+106+156+159)+
(149+40+107+5+71)+
(163+198+**226**+223+156)+
(222+37+68+193+157)+
(42+72+250+41+75)) / 25

其中5*5的矩阵称为核，针对原始图像内的像素点，采用核进行处理，得到结果图像。
![](https://img-blog.csdn.net/20180830101206352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
提取1/25可以将核转换为如下形式：
![](https://img-blog.csdn.net/2018083010130757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2.代码**

Python调用OpenCV实现均值滤波的核心函数如下：
**result = cv2.blur(原始图像,核大小)**

其中，核大小是以（宽度，高度）表示的元祖形式。常见的形式包括：核大小（3，3）和（5，5）。
$$(1) K=\frac{1}{9}\left[ \begin{matrix}   1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1  \end{matrix}  \right] \tag{1}$$
$$(2) K=\frac{1}{25}\left[ \begin{matrix}   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\  \end{matrix}  \right] \tag{2}$$
代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test01.png')
source = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
 
#均值滤波
result = cv2.blur(source, (5,5))
 
#显示图形
titles = ['Source Image', 'Blur Image']  
images = [source, result]  
for i in xrange(2):  
   plt.subplot(1,2,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180830105133276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
核设置为（10，10）和（20，20）会让图像变得更加模糊。
![](https://img-blog.csdn.net/20180830110035805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果设置为（1，1）处理结果就是原图，核中每个权重值相同，称为均值。


# 三.方框滤波

方框滤波和均值滤波核基本一致，区别是需不需要均一化处理。OpenCV调用boxFilter()函数实现方框滤波。函数如下：
**result = cv2.boxFilter(原始图像, 目标图像深度, 核大小, normalize属性)**

其中，目标图像深度是int类型，通常用“-1”表示与原始图像一直；核大小主要包括（3，3）和（5，5），如下所示。
$$(3) K=\frac{1}{9}\left[ \begin{matrix}   1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1  \end{matrix}  \right] \tag{3}$$
$$(4) K=\frac{1}{25}\left[ \begin{matrix}   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\   1 &amp; 1 &amp; 1 &amp; 1 &amp; 1 \\  \end{matrix}  \right] \tag{4}$$

normalize属性表示是否对目标图像进行归一化处理。当normalize为true时需要执行均值化处理，当normalize为false时，不进行均值化处理，实际上为求周围各像素的和，很容易发生溢出，溢出时均为白色，对应像素值为255。
![](https://img-blog.csdn.net/20180830111849922?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在图像简单平滑中，算法利用卷积模板逐一处理图像中每个像素，这一过程可以形象地比作对原始图像的像素一一进行过滤整理，在图像处理中把邻域像素逐一处理的算法过程称为滤波器。平滑线性滤波器的工作原理是利用模板对邻域内像素灰度进行加权平均，也称为均值滤波器。
![](https://img-blog.csdn.net/2018083011221891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test01.png')
source = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
 
#方框滤波
result = cv2.boxFilter(source, -1, (5,5), normalize=1)
 
#显示图形
titles = ['Source Image', 'BoxFilter Image']  
images = [source, result]  
for i in xrange(2):  
   plt.subplot(1,2,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

代码中使用5*5的核，normalize=1表示进行归一化处理，此时与均值滤波相同，输出结果如下图所示：
![](https://img-blog.csdn.net/20180830112732820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面是图像左上角处理前后的像素结果：

```python
print(source[0:3, 0:3, 0])
#[[115 180 106]
# [ 83 152  72]
# [ 55  58  55]]
print(result[0:3, 0:3, 0])
#[[92 90 78]
# [92 89 77]
# [82 80 72]]
```

如果省略参数normalize，则默认是进行归一化处理。如果normalize=0则不进行归一化处理，像素值为周围像素之和，图像更多为白色。

```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test01.png')
source = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
 
#方框滤波
result = cv2.boxFilter(source, -1, (5,5), normalize=0)
 
#显示图形
titles = ['Source Image', 'BoxFilter Image']  
images = [source, result]  
for i in xrange(2):  
   plt.subplot(1,2,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180830141037784?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图很多像素为白色，因为图像求和结果几乎都是255。如果设置的是2*2矩阵，只取四个像素结果要好些。

result = cv2.boxFilter(source, -1, (2,2), normalize=0)
![](https://img-blog.csdn.net/20180830141420541?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 四.高斯滤波

为了克服简单局部平均法的弊端(图像模糊)，目前已提出许多保持边缘、细节的局部平滑算法。它们的出发点都集中在如何选择邻域的大小、形状和方向、参数加平均及邻域各店的权重系数等。

图像高斯平滑也是邻域平均的思想对图像进行平滑的一种方法，在图像高斯平滑中，对图像进行平均时，不同位置的像素被赋予了不同的权重。高斯平滑与简单平滑不同，它在对邻域内像素进行平均时，给予不同位置的像素不同的权值，下图的所示的 3 * 3 和 5 * 5 领域的高斯模板。
![](https://img-blog.csdn.net/20150606173732592)
高斯滤波让临近的像素具有更高的重要度，对周围像素计算加权平均值，较近的像素具有较大的权重值。如下图所示，中心位置权重最高为0.4。
![](https://img-blog.csdn.net/20180830142053775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180830142452485?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Python中OpenCV主要调用GaussianBlur函数，如下：
**dst = cv2.GaussianBlur(src, ksize, sigmaX)**

其中，src表示原始图像，ksize表示核大小，sigmaX表示X方向方差。注意，核大小（N, N）必须是奇数，X方向方差主要控制权重。
$$(5) K(3,3)=\left[ \begin{matrix}   0.05 &amp; 0.1 &amp; 0.05 \\   0.1 &amp; 0.4 &amp; 0.1 \\   0.05 &amp; 0.1 &amp; 0.05  \end{matrix}  \right] \tag{5}$$
$$(4) K(5,5)=\left[ \begin{matrix}   1 &amp; 1 &amp; 2 &amp; 1 &amp; 1 \\   1 &amp; 3 &amp; 4 &amp; 3 &amp; 1 \\   2 &amp; 4 &amp; 8 &amp; 4 &amp; 2 \\   1 &amp; 3 &amp; 4 &amp; 3 &amp; 1 \\   1 &amp; 1 &amp; 2 &amp; 1 &amp; 1 \\  \end{matrix}  \right] \tag{4}$$

代码如下：

```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test01.png')
source = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
 
#高斯滤波
result = cv2.GaussianBlur(source, (3,3), 0)

#显示图形
titles = ['Source Image', 'GaussianBlur Image']  
images = [source, result]  
for i in xrange(2):  
   plt.subplot(1,2,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

输出结果如下所示：
![](https://img-blog.csdn.net/20180830143424533?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果使用15*15的核，则图形将更加模糊。
![](https://img-blog.csdn.net/20180830144251408?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 五.中值滤波

**1.概念**

在使用邻域平均法去噪的同时也使得边界变得模糊。而中值滤波是非线性的图像处理方法，在去噪的同时可以兼顾到边界信息的保留。选一个含有奇数点的窗口W，将这个窗口在图像上扫描，把窗口中所含的像素点按灰度级的升或降序排列，取位于中间的灰度值来代替该点的灰度值。 例如选择滤波的窗口如下图，是一个一维的窗口，待处理像素的灰度取这个模板中灰度的中值，滤波过程如下：
![](https://img-blog.csdn.net/20150606180316627)
如下图所示，将临近像素按照大小排列，取排序像素中位于中间位置的值作为中值滤波的像素值。

**2.代码**

OpenCV主要调用medianBlur()函数实现中值滤波。图像平滑里中值滤波的效果最好。
**dst = cv2.medianBlur(src, ksize)**

其中，src表示源文件，ksize表示核大小。核必须是大于1的奇数，如3、5、7等。![](https://img-blog.csdn.net/20180830145106209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test01.png')
 
#高斯滤波
result = cv2.medianBlur(img, 3)

#显示图像
cv2.imshow("source img", img)
cv2.imshow("medianBlur", result)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180830145509134?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
常用的窗口还有方形、十字形、圆形和环形。不同形状的窗口产生不同的滤波效果，方形和圆形窗口适合外轮廓线较长的物体图像，而十字形窗口对有尖顶角状的图像效果好。中值滤波对于消除孤立点和线段的干扰十分有用，尤其是对于二进噪声，但对消除高斯噪声的影响效果不佳。对于一些细节较多的复杂图像，可以多次使用不同的中值滤波。
![](https://img-blog.csdn.net/20150606190457141)
希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。

（By：Eastmount 2018-09-01 早8点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)












