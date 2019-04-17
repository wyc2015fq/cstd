# Opencv之图像平移 - qq_37385726的博客 - CSDN博客





2018年08月23日 10:46:37[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：473
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、仿射函数 cv2.warpAffine().](#%E4%B8%80%E3%80%81%E4%BB%BF%E5%B0%84%E5%87%BD%E6%95%B0%20cv2.warpAffine().)

[二、代码](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81)

# 一、仿射函数 cv2.warpAffine().

图像的平移，沿着x方向tx距离，y方向ty距离，那么需要构造移动矩阵： 

![](https://img-blog.csdn.net/2018082310363120?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过numpy来产生这个矩阵**【要求是float类型的】**，并将其赋值给仿射函数cv2.warpAffine(). 

仿射函数cv2.warpAffine()接受三个参数
- 需要变换的原始图像
- 移动矩阵M
- 变换的图像大小（这个大小如果不和原始图像大小相同，那么函数会自 动通过插值来调整像素间的关系）。 



# 二、代码

```python
import cv2
import numpy as np


img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)


tx = 20
ty = 20

affine_arr = np.float32([[1,0,tx],[0,1,ty]])

res = cv2.warpAffine(img1,affine_arr,(img1.shape[0],img1.shape[1]))

cv2.imshow('img',img1)
cv2.imshow('res',res)

cv2.waitkey(0)
```



![](https://img-blog.csdn.net/20180823104341216?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



