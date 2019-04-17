# Opencv之边缘检测Sobel滤波 - qq_37385726的博客 - CSDN博客





2018年08月24日 16:25:46[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：336
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、Sobel函数](#%E4%B8%80%E3%80%81Sobel%E5%87%BD%E6%95%B0)

[二、对垂直/水平方向边缘检测](#%E4%BA%8C%E3%80%81%E5%AF%B9%E5%9E%82%E7%9B%B4%2F%E6%B0%B4%E5%B9%B3%E6%96%B9%E5%90%91%E8%BE%B9%E7%BC%98%E6%A3%80%E6%B5%8B)

[三、对垂直和水平方向边缘检测](#%E4%B8%89%E3%80%81%E5%AF%B9%E5%9E%82%E7%9B%B4%E5%92%8C%E6%B0%B4%E5%B9%B3%E6%96%B9%E5%90%91%E8%BE%B9%E7%BC%98%E6%A3%80%E6%B5%8B)



# 一、Sobel函数

**# 利用Sobel方法可以进行sobel边缘检测 # **

sobelx = cv2.Sobel(img,cv2.CV_64F, 1, 0, ksize=3)
- img表示源图像，即进行边缘检测的图像
- cv2.CV_64F表示64位浮点数即64float。这里不使用numpy.float64，因为可能会发生溢出现象
- 第三和第四个参数分别是对X和Y方向的导数（即dx,dy），这里1表示对X求偏导，0表示不对Y求导。其中，X还可以求2次导。
- 注意：对X求导就是检测垂直方向的边缘，对Y求导是检测水平方向的边缘
- 第五个参数ksize是指核的大小。
- 这里说明一下，这个参数的前四个参数都没有给谁赋值，而ksize则是被赋值的对象。实际上，这时可省略的参数，而前四个是不可省的参数。注意其中的不同点



# 二、对垂直/水平方向边缘检测

```python
import cv2


# Step1  读入灰度图
initial_car = cv2.imread(r'F:\ml_summer\Opencv\Image\car.jpg',cv2.IMREAD_GRAYSCALE)

# Step2  高斯模糊处理
blur_car = cv2.GaussianBlur(initial_car,(5,5),0)

#Step3  Sobel计算水平导数
sobel_car = cv2.Sobel(blur_car,cv2.CV_16S,1,0)
sobel_car = cv2.convertScaleAbs(sobel_car)  #转回uint8



cv2.imshow('now',sobel_car)
cv2.waitKey(0)
```

**上述代码对X求导，即检测了垂直方向的边缘**

![](https://img-blog.csdn.net/20180824161948781?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
**使用cv2.Sobel(img,cv2.CV_16S,0,1)可以实现对Y求一阶导，即对水平方向进行检测**






# **三、对垂直和水平方向边缘检测**

我们不是使用
`cv2.Sobel(blur_car,cv2.CV_16S,1,1)`
而是通过，**先对水平方向边缘检测，在对垂直方向边缘检测，最后利用加权相加的方式实现对垂直和水平方向进行边缘检测**

```python
import cv2


# Step1  读入灰度图
initial_car = cv2.imread(r'F:\ml_summer\Opencv\Image\car.jpg',cv2.IMREAD_GRAYSCALE)

# Step2  高斯模糊处理
blur_car = cv2.GaussianBlur(initial_car,(5,5),0)

#Step3  Sobel计算水平导数
sobel_car1 = cv2.Sobel(blur_car,cv2.CV_16S,1,0)
sobel_car2 = cv2.Sobel(blur_car,cv2.CV_16S,0,1)
sobel_car1 = cv2.convertScaleAbs(sobel_car1)  #转回uint8
sobel_car2 = cv2.convertScaleAbs(sobel_car2)

sobel_car = cv2.addWeighted(sobel_car1,0.5,sobel_car2,0.5,0)

cv2.imshow('now',sobel_car)
cv2.waitKey(0)
```

![](https://img-blog.csdn.net/20180824162525476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)









