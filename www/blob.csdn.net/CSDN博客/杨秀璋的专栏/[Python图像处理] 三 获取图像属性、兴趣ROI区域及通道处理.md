# [Python图像处理] 三.获取图像属性、兴趣ROI区域及通道处理 - 杨秀璋的专栏 - CSDN博客





2018年08月29日 10:05:07[Eastmount](https://me.csdn.net/Eastmount)阅读数：2963
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
本篇文章主要讲解Python调用OpenCV获取图像属性，截取感兴趣ROI区域，处理图像通道。全文均是基础知识，希望对您有所帮助。知识点如下：
**1.获取图像属性**
**2.获取感兴趣ROI区域**
**3.图像通道处理**
PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


# 一.获取图像属性

**1.形状-shape**

通过shape关键字获取图像的形状，返回包含行数、列数、通道数的元祖。其中灰度图像返回行数和列数，彩色图像返回行数、列数和通道数。如下图所示：
![](https://img-blog.csdn.net/20180829083056203?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
# -*- coding:utf-8 -*-
import cv2
import numpy

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#获取图像形状
print(img.shape)

#显示图像
cv2.imshow("Demo", img)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：(445L, 670L, 3L)，该图共445行、670列像素，3个通道。
![](https://img-blog.csdn.net/20180829083439350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2.像素数目-size**

通过size关键字获取图像的像素数目，其中灰度图像返回行数 * 列数，彩色图像返回行数 * 列数 * 通道数。代码如下：

```python
# -*- coding:utf-8 -*-
import cv2
import numpy

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#获取图像形状
print(img.shape)

#获取像素数目
print(img.size)
```

输出结果：

(445L, 670L, 3L)

894450
**3.图像类型-dtype**

通过dtype关键字获取图像的数据类型，通常返回uint8。代码如下：

```python
# -*- coding:utf-8 -*-
import cv2
import numpy

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#获取图像形状
print(img.shape)

#获取像素数目
print(img.size)

#获取图像类型
print(img.dtype)
```

输出结果：

(445L, 670L, 3L)

894450

uint8

# 二.获取感兴趣ROI区域

ROI（Region of Interest）表示感兴趣区域。它是指从被处理图像以方框、圆形、椭圆、不规则多边形等方式勾勒出需要处理的区域。可以通过各种算子（Operator）和函数求得感兴趣ROI区域，并进行图像的下一步处理，被广泛应用于热点地图、人脸识别、图像分割等领域。
![](https://img-blog.csdn.net/20180829085411723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过像素矩阵可以直接获取ROI区域，如img[200:400, 200:400]。
![](https://img-blog.csdn.net/20180829085634484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码如下：

```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#定义200*100矩阵 3对应BGR
face = np.ones((200, 100, 3))

#显示原始图像
cv2.imshow("Demo", img)

#显示ROI区域
face = img[200:400, 200:300]
cv2.imshow("face", face)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180829090652805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下面将提取的ROI图像进行融合实验，代码如下：

```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#定义300*100矩阵 3对应BGR
face = np.ones((200, 200, 3))

#显示原始图像
cv2.imshow("Demo", img)

#显示ROI区域
face = img[100:300, 150:350]
img[0:200,0:200] = face
cv2.imshow("face", img)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

将提取的头部融合至图像左上角部分，如下图所示：
![](https://img-blog.csdn.net/2018082909131996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果想将两张图像进行融合，只需再读取一张图像即可，方法原理类似。 实现代码如下：

```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)
test = cv2.imread("test3.jpg", cv2.IMREAD_UNCHANGED)

#定义300*100矩阵 3对应BGR
face = np.ones((200, 200, 3))

#显示原始图像
cv2.imshow("Demo", img)

#显示ROI区域
face = img[100:300, 150:350]
test[400:600,400:600] = face
cv2.imshow("Pic", test)

#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180829093420612?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 三.图像通道处理

**1.通道拆分**

OpenCV读取的彩色图像由B、G、R三原色组成，可以通过下面代码获取不同的通道。

b = img[:, :, 0]

g = img[:, :, 1]

r = img[:, :, 2]![](https://img-blog.csdn.net/20180829094259100?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
也可以使用split()函数拆分通道，下面是拆分不同通道再显示的代码。

```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#拆分通道
b, g, r = cv2.split(img)

#显示原始图像
cv2.imshow("B", b)
cv2.imshow("G", g)
cv2.imshow("R", r)
           
#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下图所示：
![](https://img-blog.csdn.net/20180829094912890?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
也可以获取不同的通道，核心代码如下所示：
b = cv2.split(a)[0]
g = cv2.split(a)[1]
r = cv2.split(a)[2]

**2.通道合并**

图像通道合并主要调用merge()函数实现，核心代码如下：

m = cv2.merge([b, g, r])
```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)

#拆分通道
b, g, r = cv2.split(img)

#合并通道
m = cv2.merge([b, g, r])
cv2.imshow("Merge", m)
           
#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

输出结果如下：
![](https://img-blog.csdn.net/20180829095454364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意，如果是合并[r,g,b]三通道，则显示如下所示，因OpenCV是按照BGR进行读取的。

b, g, r = cv2.split(img)

m = cv2.merge([r, g, b])

cv2.imshow(“Merge”, m)![](https://img-blog.csdn.net/20180829095754877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同时，可以提取图像的不同颜色，提取B颜色通道，G、B通道设置为0，则显示蓝色。代码如下所示：

```python
# -*- coding:utf-8 -*-
import cv2
import numpy as np

#读取图片
img = cv2.imread("test.jpg", cv2.IMREAD_UNCHANGED)
rows, cols, chn = img.shape

#拆分通道
b = cv2.split(img)[0]
g = np.zeros((rows,cols),dtype=img.dtype)
r = np.zeros((rows,cols),dtype=img.dtype)

#合并通道
m = cv2.merge([b, g, r])
cv2.imshow("Merge", m)
           
#等待显示
cv2.waitKey(0)
cv2.destroyAllWindows()
```

蓝色通道输出结果如下所示：
![](https://img-blog.csdn.net/20180829100141804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
绿色通道核心代码及输出结果如下所示：

rows, cols, chn = img.shape

b = np.zeros((rows,cols),dtype=img.dtype)

g = cv2.split(img)[1]

r = np.zeros((rows,cols),dtype=img.dtype)

m = cv2.merge([b, g, r])![](https://img-blog.csdn.net/20180829100304817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
红色通道修改方法与上面类似。

希望文章对大家有所帮助，如果有错误或不足之处，还请海涵。

（By：Eastmount 2018-08-29 早10点 [https://blog.csdn.net/Eastmount/）](https://blog.csdn.net/Eastmount/%EF%BC%89)












