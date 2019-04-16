# [Python图像处理] 六.图像缩放、图像旋转、图像翻转与图像平移 - 杨秀璋的专栏 - CSDN博客





2018年09月06日 13:24:30[Eastmount](https://me.csdn.net/Eastmount)阅读数：1215
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
[[Python图像处理] 四.图像平滑之均值滤波、方框滤波、高斯滤波及中值滤波](https://blog.csdn.net/Eastmount/article/details/82216380)
[[Python图像处理] 五.图像融合、加法运算及图像类型转换](https://blog.csdn.net/Eastmount/article/details/82347501)
本篇文章主要讲解Python调用OpenCV实现图像位移操作、旋转和翻转效果，包括四部分知识：图像缩放、图像旋转、图像翻转、图像平移。全文均是基础知识，希望对您有所帮助。
**1.图像缩放**
**2.图像旋转**
**3.图像翻转**
**4.图像平移**
PS：文章参考自己以前系列图像处理文章及OpenCV库函数，同时部分参考网易云视频，推荐大家去学习。同时，本篇文章涉及到《计算机图形学》基础知识，请大家下来补充。

推荐原理文章：[OpenCV2:图像的几何变换,平移、镜像、缩放、旋转](https://www.cnblogs.com/wangguchangqing/p/4045150.html)

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一.图像缩放

图像缩放主要调用resize()函数实现，具体如下：
**result = cv2.resize(src, dsize[, result[. fx[, fy[, interpolation]]]])**

其中src表示原始图像，dsize表示缩放大小，fx和fy也可以表示缩放大小倍数，他们两个（dsize或fx\fy）设置一个即可实现图像缩放。例如：- result = cv2.resize(src, (160,160))
- result = cv2.resize(src, None, fx=0.5, fy=0.5)

图像缩放：设（x0, y0）是缩放后的坐标，（x, y）是缩放前的坐标，sx、sy为缩放因子，则公式如下：
$$(1) \left[  \begin{matrix}   x_0 &amp; y_0 &amp; 1  \end{matrix}\right] =\left[  \begin{matrix}   x &amp; y &amp; 1  \end{matrix}\right] \left[ \begin{matrix}   sx &amp; 0 &amp; 0 \\   0 &amp; sy &amp; 0 \\   0 &amp; 0 &amp; 1  \end{matrix}  \right] \tag{1}$$

代码示例如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  
 
#读取图片
src = cv2.imread('test.jpg')

#图像缩放
result = cv2.resize(src, (200,100))
print result.shape

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", result)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示，图像缩小为（200，100）像素。
![](https://img-blog.csdn.net/2018090610474782?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
需要注意的是，代码中 cv2.resize(src, (200,100)) 设置的dsize是列数为200，行数为100。

同样，可以获取原始图像像素再乘以缩放系数进行图像变换，代码如下所示。

```python
#encoding:utf-8
import cv2  
import numpy as np  
 
#读取图片
src = cv2.imread('test.jpg')
rows, cols = src.shape[:2]
print rows, cols

#图像缩放 dsize(列,行)
result = cv2.resize(src, (int(cols*0.6), int(rows*1.2)))

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", result)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180906105933385?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后讲解(fx,fy)缩放倍数的方法对图像进行放大或缩小。

```python
#encoding:utf-8
import cv2  
import numpy as np  
 
#读取图片
src = cv2.imread('test.jpg')
rows, cols = src.shape[:2]
print rows, cols

#图像缩放
result = cv2.resize(src, None, fx=0.3, fy=0.3)

#显示图像
cv2.imshow("src", src)
cv2.imshow("result", result)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

最后输出的结果如下图所示，这是按例比0.3*0.3缩小的。
![](https://img-blog.csdn.net/20180906110345882?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 二、图像旋转

图像旋转主要调用getRotationMatrix2D()函数和warpAffine()函数实现，绕图像的中心旋转，具体如下：
- **M = cv2.getRotationMatrix2D((cols/2, rows/2), 30, 1)**

参数分别为：旋转中心、旋转度数、scale
- **rotated = cv2.warpAffine(src, M, (cols, rows))**

参数分别为：原始图像、旋转参数、原始图像宽高

图像旋转：设（x0, y0）是旋转后的坐标，（x, y）是旋转前的坐标，(m,n)是旋转中心，a是旋转的角度，(left,top)是旋转后图像的左上角坐标，则公式如下：
$$(1) \left[  \begin{matrix}   x_0 &amp; y_0 &amp; 1  \end{matrix}\right] = \left[  \begin{matrix}   x &amp; y &amp; 1  \end{matrix}\right] \left[ \begin{matrix}   1 &amp; 0 &amp; 0 \\   0 &amp; -1 &amp; 0 \\   -m &amp; n &amp; 1  \end{matrix}  \right]   \left[ \begin{matrix}   cosa &amp; -sina &amp; 0 \\   sina &amp; cosa &amp; 0 \\   0 &amp; 0 &amp; 1  \end{matrix}  \right]   \left[ \begin{matrix}   1 &amp; 0 &amp; 0 \\   0 &amp; -1 &amp; 0 \\   left &amp; top &amp; 1  \end{matrix}  \right] \tag{1}$$

代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np  
 
#读取图片
src = cv2.imread('test.jpg')

#原图的高、宽 以及通道数
rows, cols, channel = src.shape

#绕图像的中心旋转
#参数：旋转中心 旋转度数 scale
M = cv2.getRotationMatrix2D((cols/2, rows/2), 30, 1)
#参数：原始图像 旋转参数 元素图像宽高
rotated = cv2.warpAffine(src, M, (cols, rows))

#显示图像
cv2.imshow("src", src)
cv2.imshow("rotated", rotated)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180906125143474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果设置-90度，则核心代码和图像如下所示。

M = cv2.getRotationMatrix2D((cols/2, rows/2), -90, 1)

rotated = cv2.warpAffine(src, M, (cols, rows))![](https://img-blog.csdn.net/20180906125332386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 三、图像翻转

图像翻转在OpenCV中调用函数flip()实现，原型如下：
**dst = cv2.flip(src, flipCode)**

其中src表示原始图像，flipCode表示翻转方向，如果flipCode为0，则以X轴为对称轴翻转，如果fliipCode>0则以Y轴为对称轴翻转，如果flipCode<0则在X轴、Y轴方向同时翻转。
代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test.jpg')
src = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

#图像翻转
#0以X轴为对称轴翻转 >0以Y轴为对称轴翻转 <0X轴Y轴翻转
img1 = cv2.flip(src, 0)
img2 = cv2.flip(src, 1)
img3 = cv2.flip(src, -1)

#显示图形
titles = ['Source', 'Image1', 'Image2', 'Image3']  
images = [src, img1, img2, img3]  
for i in xrange(4):  
   plt.subplot(2,2,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180906130642638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 四、图像平移

图像平移：设（x0, y0）是缩放后的坐标，（x, y）是缩放前的坐标，dx、dy为偏移量，则公式如下：
$$(3) \left[  \begin{matrix}   x_0 &amp; y_0 &amp; 1  \end{matrix}\right] =\left[  \begin{matrix}   x &amp; y &amp; 1  \end{matrix}\right] \left[ \begin{matrix}   1 &amp; 0 &amp; 0 \\   0 &amp; 1 &amp; 0 \\   dx &amp; dy &amp; 1  \end{matrix}  \right] \tag{3}$$

图像平移首先定义平移矩阵M，再调用warpAffine()函数实现平移，核心函数如下：
**M = np.float32([[1, 0, x], [0, 1, y]])**
**shifted = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))**
完整代码如下所示：

```python
#encoding:utf-8
import cv2  
import numpy as np
import matplotlib.pyplot as plt
 
#读取图片
img = cv2.imread('test.jpg')
image = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)

#图像平移 下、上、右、左平移
M = np.float32([[1, 0, 0], [0, 1, 100]])
img1 = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))

M = np.float32([[1, 0, 0], [0, 1, -100]])
img2 = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))

M = np.float32([[1, 0, 100], [0, 1, 0]])
img3 = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))

M = np.float32([[1, 0, -100], [0, 1, 0]])
img4 = cv2.warpAffine(image, M, (image.shape[1], image.shape[0]))

#显示图形
titles = [ 'Image1', 'Image2', 'Image3', 'Image4']  
images = [img1, img2, img3, img4]  
for i in xrange(4):  
   plt.subplot(2,2,i+1), plt.imshow(images[i], 'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180906132244706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
三尺讲台，三寸舌，

三千桃李，三杆笔。

再累再苦，站在讲台前就是最美的自己，几个月的烦恼和忧愁都已消失，真的好享受这种状态，仿佛散着光芒，终于给低年级的同学上课了越早培养编程兴趣越好，恨不能倾囊相授。

即使当一辈子的教书匠，平平淡淡也喜欢，而且总感觉给学生讲课远不是课酬和职称所能比拟，这就是所谓的事业，所谓的爱好。

希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。 准备出去休回家了，好好享受最美时光。

（By：Eastmount 2018-09-06 早10点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)










