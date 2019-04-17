# Opencv之图像旋转 - qq_37385726的博客 - CSDN博客





2018年08月23日 11:02:57[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：148
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、旋转矩阵 cv2.getRotationMatrix2D()](#%E4%B8%80%E3%80%81%E6%97%8B%E8%BD%AC%E7%9F%A9%E9%98%B5%20cv2.getRotationMatrix2D())

[二、warpAffine()](#%E4%BA%8C%E3%80%81warpAffine())

[三、代码](#%E4%B8%89%E3%80%81%E4%BB%A3%E7%A0%81)

# 一、旋转矩阵 cv2.getRotationMatrix2D()

这个函数需要三个参数
- 旋转中心
- 旋转角度
- 旋转后图像的缩放比例

```python
rotate = cv2.getRotationMatrix2D((x/2,y/2),45,0.5)
```



# 二、warpAffine()

```python
res = cv2.warpAffine(img1,rotate,(0.5*x,0.5*y))
```



# 三、代码

```python
import cv2

img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)

x,y = img1.shape[:2]

rotate = cv2.getRotationMatrix2D((x/2,y/2),45,0.5)

res = cv2.warpAffine(img1,rotate,(0.5*x,0.5*y))

cv2.imshow('img',img1)
cv2.imshow('res',res)
cv2.waitKey(0)
```



![](https://img-blog.csdn.net/20180823110156363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)











