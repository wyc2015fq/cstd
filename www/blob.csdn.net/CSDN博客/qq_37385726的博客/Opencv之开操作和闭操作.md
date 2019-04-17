# Opencv之开操作和闭操作 - qq_37385726的博客 - CSDN博客





2018年08月24日 17:14:37[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：879
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、开操作、闭操作](#%E4%B8%80%E3%80%81%E5%BC%80%E6%93%8D%E4%BD%9C%E3%80%81%E9%97%AD%E6%93%8D%E4%BD%9C)

[二、morphologyEX](#%E4%BA%8C%E3%80%81morphologyEX)

[三、代码](#%E4%B8%89%E3%80%81%E4%BB%A3%E7%A0%81)



# 一、开操作、闭操作

**用于对二值化后的图像进行处理，属于形态学操作（morphology）**

开操作：消除白色的小点，去除小的干扰块

![](https://img-blog.csdn.net/20180824170853935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

闭操作：消除黑色的小块，填充闭合区域

![](https://img-blog.csdn.net/20180824170940908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 二、morphologyEX

cv2.morphologyEX(
- img  输入图像
- cv2.MORPH_CLOSE，cv2.MORPH_OPEN  形态学操作
- kernel  

）

其中kernel，用

cv2.getStructuringElement(cv2.MORPH_RECT,(5,5))
获得



# 三、代码

```python
import cv2 as cv
import numpy as np

img = cv.imread("tooth.png")
cv.imshow('img', img)
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
cv.imshow('gray', gray)
ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
cv.imshow('bin', binary)
# 核的大小和形状
kernel = cv.getStructuringElement(cv.MORPH_RECT, (5, 5))
# 开操作
ret1 = cv.morphologyEx(binary, cv.MORPH_OPEN, kernel, iterations=5)
# 闭操作
ret2 = cv.morphologyEx(binary, cv.MORPH_CLOSE, kernel, iterations=5)
cv.imshow('ret1', ret1)
cv.imshow('ret2', ret2)
cv.waitKey(0)
```





