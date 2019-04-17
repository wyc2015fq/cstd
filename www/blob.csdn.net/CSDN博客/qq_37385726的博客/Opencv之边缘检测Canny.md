# Opencv之边缘检测Canny - qq_37385726的博客 - CSDN博客





2018年08月24日 10:34:52[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：49
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、Canny](#%E4%B8%80%E3%80%81Canny)

[二、代码](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81)

# 一、Canny

> 
**cv2.Canny(img,x,y)**


Canny边缘检测算法非常复杂，但也很有趣。

它有**5个步骤**，即：
- 使用高斯滤波器对图像进行去噪
- 计算梯度
- 在边缘上使用非最大抑制（NMS）
- 在检测到的边缘上使用双阈值去除假阳性
- 最后还会分析所有的边缘及其之间的连接，以保留真正的边缘并消除不明显的边缘。



# 二、代码

```python
import cv2

img1 = cv2.imread('./Image/reba_color.jpg')

x,y = img1.shape[:2]
edge = cv2.Canny(img1,x,y)
cv2.imshow('edge',edge)
```

![](https://img-blog.csdn.net/20180824103425394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





