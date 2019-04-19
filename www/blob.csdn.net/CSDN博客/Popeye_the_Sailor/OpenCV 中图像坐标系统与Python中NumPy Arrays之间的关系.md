# OpenCV 中图像坐标系统与Python中NumPy Arrays之间的关系 - Popeye_the_Sailor - CSDN博客
2018年07月09日 22:38:49[_Sailor_](https://me.csdn.net/lz0499)阅读数：2068
## **OpenCV 中图像坐标系统与Python中NumPyArrays之间的关系**
#### **OpenCV中的图像坐标系**
一般而言，图像可以用像素网格表示。想象一下一张网格的纸张，网格纸张中的那一个个网格就可以看作是一个一个像素。在这张网格纸中，最左上角代表的就是图像的（0,0）坐标点。当我们向下并向右移动时，x和y的坐标值都随之增大。
我们以如下图片进行说明。
![](https://img-blog.csdn.net/20180709222335724?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x6MDQ5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如上图，在一张网格纸上，呈现一个字母I。我们能够观察到这是一个8*8的网格，总计64个像素。点（0,0）代表的是最左上角的像素，相当于该坐标系的原点。点（7,7）对应于最右下角。点（3,4）表示的是从原点起，向下数四行，向右数三列。即图像坐标系中，任何像素的坐标位置是相对于原点而言，可以使用（w,h）表示。其中w表示的是图像的横向坐标，是图像的列相对位置；h表示的是图像的纵坐标，是图像中行的相对位置。相对位置都是以0为索引开始计算，即图像坐标系先按宽，再按高进行索引，即（W,H）
#### **Python中Numpy数组所表示的图像**
Python中使用Numpy 数组代表的图像时，表示的是一个二维数组。用shape方法得到的数组属性为（height,width，depth）。当我们第一次用Numpy数组处理图像的时候，常常感到疑惑的是：为什么height会在width前面了？
这个是因为矩阵的定义。二维数组我们一般也可以用矩阵表示。当我们定义一个矩阵大小时，我们常常说是多少行多少列的矩阵。矩阵中的行在图像坐标系中恰好对应的是图像的高度，而矩阵中的列恰好对应图像的宽度，而图像的深度的depth未改变其含义。
假设想获取图像坐标系中某一点（W,H）的像素灰度值，在Numpy数组表示的图像中，应该通img.item(H,W)的方式获得。如下代码段所示：
```python
img = cv2.imread('LenaRGB.bmp')
h,w = img.shape[:2]
cv2.circle(img,(w//2,128), 10, (0,0,255), -1)
#first print
print(img.item(128,256,0))
print(img.item(128,256,1))
print(img.item(128,256,2))
#second print
print(img.item(256,128,0))
print(img.item(256,128,1))
print(img.item(256,128,2))
```
LenaRGB图像大小为512*512，我在距离图像原点横坐标256，距离原点纵坐标128的位置画出一个半径为10的红色圆。
![](https://img-blog.csdn.net/20180715205255418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x6MDQ5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第一次打印和第二次打印出来的结果为：
```python
#first print
0
0
255
//second print
98
118
224
```
可以看出来，第一次打印出来的结果才是圆心坐标的像素灰度值。也即表明用Numpy数组对图像像素进行访问时，应该先写图像高度所对应的坐标，再写图像宽度对应的坐标。
是否调用cv2中OpenCV自带的函数时，所使用的坐标是图像坐标系，使用Numpy数组时使用的是类似访问矩阵的方法呢？
#### **Python Numpy**数组所表示的图像与OpenCV图像坐标系中的转换
```python
import numpy as np
import argparse
import imutils
import cv2
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required = True,
help = "Path to the image")
args = vars(ap.parse_args())
image = cv2.imread(args["image"])
cv2.imshow("Original", image)
(h, w) = image.shape[:2]
center = (w // 2, h // 2)
M = cv2.getRotationMatrix2D(center, 45, 1.0)
rotated = cv2.warpAffine(image, M, (w, h))
cv2.imshow("Rotated by 45 Degrees", rotated)
M = cv2.getRotationMatrix2D(center, -90, 1.0)
rotated = cv2.warpAffine(image, M, (w, h))
cv2.imshow("Rotated by -90 Degrees", rotated)
```
上述代码段表示的对图像进行45度的旋转。我们选择的旋转中心是图像的中心像素。图像的宽和高我们通过Numpy中的shape方法获得：
```python
(h, w) = image.shape[:2] #通过shape方法获得矩阵的行*列，即图像坐标系中的高和宽
```
之后，我们通过图像的宽和高获得图像的中心点坐标：
```python
center = (w // 2, h // 2)
```
再使用仿射变换对图像进行旋转操作。仿射变换中的SIZE即为图像的w*h
#### 参考资料：
Deep Learning for Computer Vision with Python  Starter Bundle
Practical Python and OpenCV: An Introductory, Example Driven Guide to Image Processing and Computer Vision
3rd Edition Dr. Adrian Rosebrock

