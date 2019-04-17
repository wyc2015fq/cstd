# Opencv之图像自适应阈值二值化处理adaptiveThreshold - qq_37385726的博客 - CSDN博客





2018年08月24日 14:10:26[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1707
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、自适应阈值](#%E4%B8%80%E3%80%81%E8%87%AA%E9%80%82%E5%BA%94%E9%98%88%E5%80%BC)

[二、adaptiveThreshold](#%E4%BA%8C%E3%80%81adaptiveThreshold)

[三、代码](#%E4%B8%89%E3%80%81%E4%BB%A3%E7%A0%81)



# 一、自适应阈值

相比于固定阈值的二值化处理【[https://blog.csdn.net/qq_37385726/article/details/82015545](https://blog.csdn.net/qq_37385726/article/details/82015545)】

自适应阈值不需要确定一个固定的阈值，而是可以根据对应的自适应方法，通过图像的局部特征自适应的设定阈值，做出二值化处理。



# 二、adaptiveThreshold

adaptiveThreshold(
- img  输入图像. 
- double max_value,
- int adaptive_method=cv2.ADAPTIVE_THRESH_MEAN_C，cv2.ADAPTIVE_THRESH_MEAN_C
- int threshold_type=CV_THRESH_BINARY，cv2.THRESH_BINARY_INV
- int block_size=3,
- double param=5

)

 CV_ADAPTIVE_THRESH_MEAN_C 和 CV_ADAPTIVE_THRESH_GAUSSIAN_C， 它是一个从均值或加权均值(区域中（x，y）周围的像素根据高斯函数按照他们离中心点的距离进行加权计算)提取的常数

参数6，param是在 adaptive_method计算出结果后，需要再减去param





# 三、代码

```python
import cv2

img1 = cv2.imread('./Image/letter.png',cv2.IMREAD_GRAYSCALE)


img1 = cv2.resize(img1,(300,300),interpolation=cv2.INTER_AREA)
cv2.imshow('img1',img1)


res1 = cv2.adaptiveThreshold(img1,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)
res2 = cv2.adaptiveThreshold(img1,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,25,5)

cv2.imshow('res1',res1)
cv2.imshow('res2',res2)
```

![](https://img-blog.csdn.net/20180824140932586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180824140944256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180824140955314?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





