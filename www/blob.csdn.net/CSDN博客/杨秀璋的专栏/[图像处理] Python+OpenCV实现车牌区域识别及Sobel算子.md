
# [图像处理] Python+OpenCV实现车牌区域识别及Sobel算子 - 杨秀璋的专栏 - CSDN博客

2018年08月06日 22:17:16[Eastmount](https://me.csdn.net/Eastmount)阅读数：2449所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)[Python图像处理及图像识别](https://blog.csdn.net/column/details/26274.html)



由于最近太忙，这篇文章只给出相关代码供大家学习，过一段时间会详细的写一些列Python图像处理的文章，包括各种算法原理、图像识别、图像增强、图像分类、深度学习等。本篇文章主要调用OpenCV库（cv2）进行车牌区域识别，具体步骤包括：
1.灰度转换：将彩色图片转换为灰度图像，常见的R=G=B=像素平均值。
2.高斯平滑和中值滤波：去除噪声。
3.Sobel算子：提取图像边缘轮廓，X方向和Y方向平方和开跟。
4.二值化处理：图像转换为黑白两色，通常像素大于127设置为255，小于设置为0。
5.膨胀和细化：放大图像轮廓，转换为一个个区域，这些区域内包含车牌。
6.通过算法选择合适的车牌位置，通常将较小的区域过滤掉或寻找蓝色底的区域。
7.标注车牌位置，如果是花儿、人脸、牛角，可能需要特征提取和训练。

![](https://img-blog.csdn.net/20180806222455978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

本篇文章为基础性文章，希望对你有所帮助，主要提供些思路，也是自己教学的内容。如果文章中存在错误或不足之处，还请海涵。同时，推荐大家阅读我以前的文章了解其他知识。
推荐我C++图像系列基础知识：
[https://blog.csdn.net/column/details/eastmount-mfc.html](https://blog.csdn.net/column/details/eastmount-mfc.html)
PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)
五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C\#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。


## 一、读取图像及灰度转换
代码如下：
```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
#读取图片
imagePath = '10.jpg'
img = cv2.imread(imagePath)
#opencv默认的imread是以BGR的方式进行存储的
#而matplotlib的imshow默认则是以RGB格式展示
#所以此处我们必须对图片的通道进行转换
lenna_img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
#灰度图像处理
GrayImage = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
print(u"读入lenna图的shape为", GrayImage.shape)
#显示图形
titles = ['Source Image','Gray Image']  
images = [lenna_img, GrayImage]  
for i in xrange(2):  
   plt.subplot(1,2,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()  
cv2.imshow('Gray.bmp', GrayImage)
cv2.waitKey(0)
```
输出结果如下图所示：
![](https://img-blog.csdn.net/20180806213756450?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 二、高斯平滑和中值滤波去噪
这里原理推荐我以前C++图像处理的文章，如下：[https://blog.csdn.net/column/details/eastmount-mfc.html](https://blog.csdn.net/column/details/eastmount-mfc.html)
![](https://img-blog.csdn.net/20180806214142308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180806215639183?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
完整代码如下所示：
```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
#读取图片
imagePath = '10.jpg'
img = cv2.imread(imagePath)
#opencv默认的imread是以BGR的方式进行存储的
#而matplotlib的imshow默认则是以RGB格式展示
#所以此处我们必须对图片的通道进行转换
lenna_img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
#灰度图像处理
GrayImage = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
print(u"读入lenna图的shape为", GrayImage.shape)
#直方图均衡化
#equ = cv2.equalizeHist(gray)
#高斯平滑
Gaussian = cv2.GaussianBlur(GrayImage, (3, 3), 0, 0, cv2.BORDER_DEFAULT)
#Gaussian = cv2.GaussianBlur(GrayImage, (9, 9),0)
#中值滤波
Median = cv2.medianBlur(Gaussian, 5)
#显示图形
titles = ['Source Image','Gray Image', 'Gaussian Image', 'Median Image']  
images = [lenna_img, GrayImage, Gaussian, Median]  
for i in xrange(4):  
   plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```
输出结果如下图所示，分别是原图、灰度图像、高斯处理和中值滤波处理。
![](https://img-blog.csdn.net/20180806214938992?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 三、Sobel算子提取轮廓和二值化处理
有时还需要加强图像中景物的边缘和轮廓，边缘和轮廓通常位于图像中灰度突出的地方，因而可以直观的想到用灰度的差分对边缘和轮廓进行提取，通常可以通过梯度算子进行提取。图像锐化的目的是提高图像的对比度，从而使图像更清晰，通过提高邻域内像素的灰度差来提高图像的对比度。本文采用Sobel算子提取边缘轮廓。
![](https://img-blog.csdn.net/20180806215608911?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
阈值又称为临界值，它的目的是确定出一个范围，然后这个范围内的部分使用同一种方法处理，而阈值之外的部分则使用另一种处理方法或保持原样。常用的包括产生二值图：当x<T时y=0，当x>=T时y=255（其中T是阈值）。阈值变换在生物学上的应用比较广泛，常用语细胞图像分割等。本文采用二值化处理将大于等于170像素的转换为255，而下于的转换为0，使得图像更加清晰。
完整代码如下所示：
```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
#读取图片
imagePath = '10.jpg'
img = cv2.imread(imagePath)
#opencv默认的imread是以BGR的方式进行存储的
lenna_img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
#灰度图像处理
GrayImage = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
print(u"读入lenna图的shape为", GrayImage.shape)
#直方图均衡化
#equ = cv2.equalizeHist(gray)
# 高斯平滑
Gaussian = cv2.GaussianBlur(GrayImage, (3, 3), 0, 0, cv2.BORDER_DEFAULT)
# 中值滤波
Median = cv2.medianBlur(Gaussian, 5)
# Sobel算子 XY方向求梯度
x = cv2.Sobel(Median, cv2.CV_8U, 1, 0, ksize = 3) #X方向
y = cv2.Sobel(Median, cv2.CV_8U, 0, 1, ksize = 3) #Y方向
absX = cv2.convertScaleAbs(x)   # 转回uint8    
absY = cv2.convertScaleAbs(y)    
Sobel = cv2.addWeighted(absX, 0.5, absY, 0.5,0)
cv2.imshow('dilation2', Sobel)
cv2.waitKey(0)
# 二值化处理 周围像素影响
ret, Binary = cv2.threshold(Sobel, 170, 255, cv2.THRESH_BINARY)
cv2.imshow('dilation2',Binary)
cv2.waitKey(0)
#显示图形
titles = ['Source Image','Gray Image', 'Gaussian Image', 'Median Image',
          'Sobel Image', 'Binary Image']  
images = [lenna_img, GrayImage, Gaussian, Median, Sobel, Binary]  
for i in xrange(6):  
   plt.subplot(2,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```
输出结果如下所示：
![](https://img-blog.csdn.net/20180806215833104?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 四、膨胀和腐蚀处理
接下来进行膨胀和腐蚀处理，其中膨胀让轮廓突出，腐蚀去掉细节。
```python
#encoding:utf-8
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
#读取图片
imagePath = '10.jpg'
img = cv2.imread(imagePath)
#opencv默认的imread是以BGR的方式进行存储的
lenna_img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
#灰度图像处理
GrayImage = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
print(u"读入lenna图的shape为", GrayImage.shape)
#直方图均衡化
#equ = cv2.equalizeHist(gray)
#高斯平滑 去噪
Gaussian = cv2.GaussianBlur(GrayImage, (3, 3), 0, 0, cv2.BORDER_DEFAULT)
#Gaussian = cv2.GaussianBlur(GrayImage, (9, 9),0)
#中值滤波
Median = cv2.medianBlur(Gaussian, 5)
#Sobel算子 XY方向求梯度 cv2.CV_8U
x = cv2.Sobel(Median, cv2.CV_32F, 1, 0, ksize = 3) #X方向
y = cv2.Sobel(Median, cv2.CV_32F, 0, 1, ksize = 3) #Y方向
#absX = cv2.convertScaleAbs(x)   # 转回uint8    
#absY = cv2.convertScaleAbs(y)
#Sobel = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)
gradient = cv2.subtract(x, y)
Sobel = cv2.convertScaleAbs(gradient)
cv2.imshow('dilation2', Sobel)
cv2.waitKey(0)
#二值化处理 周围像素影响
blurred = cv2.GaussianBlur(Sobel, (9, 9),0) #再进行高斯去噪
#注意170可以替换的
ret, Binary = cv2.threshold(blurred , 90, 255, cv2.THRESH_BINARY)
cv2.imshow('dilation2', Binary)
cv2.waitKey(0)
#膨胀和腐蚀操作的核函数
element1 = cv2.getStructuringElement(cv2.MORPH_RECT, (9, 1))
element2 = cv2.getStructuringElement(cv2.MORPH_RECT, (9, 7))
# 膨胀让轮廓突出
Dilation = cv2.dilate(Binary, element2, iterations = 1)
# 腐蚀去掉细节
Erosion = cv2.erode(Dilation, element1, iterations = 1)
# 再次膨胀
Dilation2 = cv2.dilate(Erosion, element2,iterations = 3)
cv2.imshow('Dilation2 ', Dilation2)
cv2.waitKey(0)
#显示图形
titles = ['Source Image','Gray Image', 'Gaussian Image', 'Median Image',
          'Sobel Image', 'Binary Image', 'Dilation Image', 'Erosion Image', 'Dilation2 Image']  
images = [lenna_img, GrayImage, Gaussian,
          Median, Sobel, Binary,
          Dilation, Erosion, Dilation2]  
for i in xrange(9):  
   plt.subplot(3,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()
```
输出结果如下图所示，可以看到轮廓区域已经被提取出来，接下来开始有选择的进行获取。
![](https://img-blog.csdn.net/20180806220335502?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 五、指定算法选择车牌区域
该部分代码膨胀和腐蚀略有区别，采用closed变量实现。同时获取最理想的区域，完整代码如下所示：
```python
#encoding:utf-8
#BY：Eastmount CSDN 2018-08-06
import cv2  
import numpy as np  
import matplotlib.pyplot as plt
#读取图片
imagePath = '10.jpg'
img = cv2.imread(imagePath)
#opencv默认的imread是以BGR的方式进行存储的
lenna_img = cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
#灰度图像处理
GrayImage = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
print(u"读入lenna图的shape为", GrayImage.shape)
#直方图均衡化
#equ = cv2.equalizeHist(gray)
#高斯平滑 去噪
Gaussian = cv2.GaussianBlur(GrayImage, (3, 3), 0, 0, cv2.BORDER_DEFAULT)
#Gaussian = cv2.GaussianBlur(GrayImage, (9, 9),0)
#中值滤波
Median = cv2.medianBlur(Gaussian, 5)
#Sobel算子 XY方向求梯度 cv2.CV_8U
x = cv2.Sobel(Median, cv2.CV_32F, 1, 0, ksize = 3) #X方向
y = cv2.Sobel(Median, cv2.CV_32F, 0, 1, ksize = 3) #Y方向
#absX = cv2.convertScaleAbs(x)   # 转回uint8    
#absY = cv2.convertScaleAbs(y)
#Sobel = cv2.addWeighted(absX, 0.5, absY, 0.5, 0)
gradient = cv2.subtract(x, y)
Sobel = cv2.convertScaleAbs(gradient)
cv2.imshow('dilation2', Sobel)
cv2.waitKey(0)
#二值化处理 周围像素影响
blurred = cv2.GaussianBlur(Sobel, (9, 9),0) #再进行一次高斯去噪
#注意170可以替换的
ret, Binary = cv2.threshold(blurred , 170, 255, cv2.THRESH_BINARY)
cv2.imshow('dilation2', Binary)
cv2.waitKey(0)
# 膨胀和腐蚀操作的核函数
element1 = cv2.getStructuringElement(cv2.MORPH_RECT, (9, 1))
element2 = cv2.getStructuringElement(cv2.MORPH_RECT, (9, 7))
# 膨胀一次，让轮廓突出
Dilation = cv2.dilate(Binary, element2, iterations = 1)
# 腐蚀一次，去掉细节
Erosion = cv2.erode(Dilation, element1, iterations = 1)
# 再次膨胀，让轮廓明显一些
Dilation2 = cv2.dilate(Erosion, element2,iterations = 3)
cv2.imshow('Dilation2 ', Dilation2)
cv2.waitKey(0)

##########################################
#建立一个椭圆核函数
kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (25, 25))
#执行图像形态学, 细节直接查文档，很简单
closed = cv2.morphologyEx(Binary, cv2.MORPH_CLOSE, kernel)
closed = cv2.erode(closed, None, iterations=4)
closed = cv2.dilate(closed, None, iterations=4)
cv2.imshow('erode dilate', closed)
cv2.waitKey(0)
##########################################

#显示图形
titles = ['Source Image','Gray Image', 'Gaussian Image', 'Median Image',
          'Sobel Image', 'Binary Image', 'Dilation Image', 'Erosion Image', 'Dilation2 Image']  
images = [lenna_img, GrayImage, Gaussian,
          Median, Sobel, Binary,
          Dilation, Erosion, closed]  
for i in xrange(9):  
   plt.subplot(3,3,i+1),plt.imshow(images[i],'gray')  
   plt.title(titles[i])  
   plt.xticks([]),plt.yticks([])  
plt.show()  
cv2.imshow('Gray', GrayImage)
cv2.waitKey(0)
"""
接下来使用Dilation2图片确定车牌的轮廓
这里opencv3返回的是三个参数
  参数一：二值化图像
  参数二：轮廓类型 检测的轮廓不建立等级关系
  参数三：处理近似方法  例如一个矩形轮廓只需4个点来保存轮廓信息
"""
(_, cnts, _) = cv2.findContours(closed.copy(), 
                                cv2.RETR_LIST,               #RETR_TREE
                                cv2.CHAIN_APPROX_SIMPLE)
#画出轮廓
c = sorted(cnts, key=cv2.contourArea, reverse=True)[0]
print c
#compute the rotated bounding box of the largest contour
rect = cv2.minAreaRect(c)
print 'rectt', rect
Box = np.int0(cv2.boxPoints(rect))
print 'Box', Box
#draw a bounding box arounded the detected barcode and display the image
Final_img = cv2.drawContours(img.copy(), [Box], -1, (0, 0, 255), 3)
cv2.imshow('Final_img', Final_img)
cv2.waitKey(0)
```
输出结果如下图所示，可以看到车牌被选中了。
![](https://img-blog.csdn.net/20180806221322521?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018080622134289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考文献推荐大家学习三位大神的作品：
[https://www.jianshu.com/p/fcfbd3131b84](https://www.jianshu.com/p/fcfbd3131b84)
[https://blog.csdn.net/sinat_36458870/article/details/7882557](https://blog.csdn.net/sinat_36458870/article/details/78825571)1
[https://blog.csdn.net/sumkee911/article/details/79435983](https://blog.csdn.net/sumkee911/article/details/79435983)
当然还有很多优化和提升的地方，作者自己也还在不断学习中，希望大家对这篇文章感兴趣，同时最近的Python新书也宣传下，大家一起进步。基础性文章，如果文章中有错误或不足之处还请海涵。

![](https://img-blog.csdn.net/20180611095441156)

## 本书主要包括上下两册：
《Python网络数据爬取及分析从入门到精通（爬取篇）》
《Python网络数据爬取及分析从入门到精通（分析篇）》
(By:Eastmount 2018-08-06 深夜11点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


