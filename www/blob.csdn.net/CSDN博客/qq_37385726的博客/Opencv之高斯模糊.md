# Opencv之高斯模糊 - qq_37385726的博客 - CSDN博客





2018年08月24日 15:54:52[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：762
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、 高斯模糊](#2-%E9%AB%98%E6%96%AF%E6%A8%A1%E7%B3%8A)

[二、实验代码](#%E4%BA%8C%E3%80%81%E5%AE%9E%E9%AA%8C%E4%BB%A3%E7%A0%81)

转自[这里](https://blog.csdn.net/solidcorr/article/details/50625307)



# 一、 高斯模糊

**高斯模糊本质上是低通滤波器，输出图像的每个像素点是原图像上对应像素点与周围像素点的加权和**，原理并不复杂。做久了卷积神经网络看这个分外亲切**，就是用高斯分布权值矩阵与原始图像矩阵做卷积运算而已**。摘录wiki上两段原文：

> 
Mathematically, applying a Gaussian blur to an image is the same as convolving the image with a Gaussian function.

数学上讲，对图像做高斯模糊等同于将图像与高斯函数卷积。

Since the Fourier transform of a Gaussian is another Gaussian, applying a Gaussian blur has the effect of reducing the image’s high-frequency components; a Gaussian blur is thus a low pass filter.

由于高斯分布的傅里叶变换仍然是高斯分布，使用高斯模糊就减少了图像的高频分量，因此高斯模糊是低通滤波器。


至于高斯分布权重矩阵，就是对二维正态分布的密度函数（也就是高斯函数）采样再做归一化的产物。

需要对高斯模糊算法的详细解释，可以参考： 
[http://www.ruanyifeng.com/blog/2012/11/gaussian_blur.html](http://www.ruanyifeng.com/blog/2012/11/gaussian_blur.html)

**使用cv2做高斯模糊，只要一行代码调用GaussianBlur函数，给出高斯矩阵的尺寸和标准差就可以**：
`blur = cv2.GaussianBlur(img,(5,5),0)`


这里(5, 5)表示高斯矩阵的长与宽都是5，**标准差取0时OpenCV会根据高斯矩阵的尺寸自己计算**。概括地讲，**高斯矩阵的尺寸越大，标准差越大，处理过的图像模糊程度越大。也可以自己构造高斯核，相关函数：cv2.GaussianKernel().**



> 
**效果**

可以来退出由照相机或其他环境产生的噪声，减少在边缘提取时的其余边缘的数目


## 二、实验代码

直接看代码：

```python
#!/usr/bin/env python

import cv2

imgName = "1.jpg";
kernel_size = (5, 5);
sigma = 1.5;

img = cv2.imread(imgName);
img = cv2.GaussianBlur(img, kernel_size, sigma);
new_imgName = "New_" + str(kernel_size[0]) + "_" + str(sigma) + "_" + imgName;
cv2.imwrite(new_imgName, img);
```





内容非常简单，通过imread读入图片，用kernel_size和sigma定义的高斯矩阵模糊图片，用imwrite保存为一张新图片。

这里使用一只欠雷做实验，原图： 
![原始图片](http://upload-images.jianshu.io/upload_images/1472481-8939542f6dd669d5.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

经过不同高斯矩阵模糊后的图像：

![kernel_size=(5,5), sigma=0.5](http://upload-images.jianshu.io/upload_images/1472481-3b99da145134d171.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![kernel_size=(5,5), sigma = 1.5](http://upload-images.jianshu.io/upload_images/1472481-5421bec7666eb505.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![kernel_size=(9,9), sigma = 1.5](http://upload-images.jianshu.io/upload_images/1472481-5fdfd5bafe1b182c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

可以看出，趋势的确是高斯矩阵尺寸和标准差越大，处理后的图片越模糊。最后，让我们糊个痛快：

![给红包看原图 :)](http://upload-images.jianshu.io/upload_images/1472481-553bff9ac363813a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



