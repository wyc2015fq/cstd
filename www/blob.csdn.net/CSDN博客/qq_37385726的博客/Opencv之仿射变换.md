# Opencv之仿射变换 - qq_37385726的博客 - CSDN博客





2018年08月23日 11:25:57[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：83
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、仿射矩阵](#%E4%B8%80%E3%80%81%E4%BB%BF%E5%B0%84%E7%9F%A9%E9%98%B5)

[二、warpAffine()](#%E4%BA%8C%E3%80%81warpAffine())

[三、代码](#%E4%B8%89%E3%80%81%E4%BB%A3%E7%A0%81)



# 一、仿射矩阵

图像的旋转加上拉升就是图像仿射变换。

opencv中通过定义两个array（pos1,pos2），结合cv2.getAffineTransform(pos1,pos2)来实现仿射变换。

pos1和pos2是3X2的矩阵，每一行都是一个点的位置，其中pos1定义的是原图中三个点的位置，pos2定义的是仿射后的图中三个点的位置。

![](https://img-blog.csdn.net/20180823112447941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

M=cv2.getAffineTransform(pos1,pos2),其中两个位置就是变换前后的对应位置关系。输出的就是仿射矩阵M。



# 二、warpAffine()

```python
res = cv2.warpAffine(img1,aff,(2*x,2*y))
```



# 三、代码

```python
import cv2
import numpy as np


img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)


x,y = img1.shape[:2]

b = np.float32([[50,50],[200,50],[50,200]])
p = np.float32([[10,100],[200,50],[100,250]])

aff = cv2.getAffineTransform(b, p)

res = cv2.warpAffine(img1,aff,(2*x,2*y))

cv2.imshow('img',img1)
cv2.imshow('res',res)

cv2.waitKey(0)
```



![](https://img-blog.csdn.net/20180823112102555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)











