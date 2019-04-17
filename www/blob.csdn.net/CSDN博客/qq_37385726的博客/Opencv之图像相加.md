# Opencv之图像相加 - qq_37385726的博客 - CSDN博客





2018年08月24日 11:48:57[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：299
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、图像相加](#%E4%B8%80%E3%80%81%E5%9B%BE%E5%83%8F%E7%9B%B8%E5%8A%A0)

[二、图像加权相加](#%E4%BA%8C%E3%80%81%E5%9B%BE%E5%83%8F%E5%8A%A0%E6%9D%83%E7%9B%B8%E5%8A%A0)



# 一、图像相加

> 
**cv2.add(img1,img2)**

要求img1.shape与img2.shape一致


```python
import cv2



img1 = cv2.imread('./Image/img1.png')
img2 = cv2.imread('./Image/img2.png')


img1 = cv2.resize(img1,(300,300),interpolation=cv2.INTER_AREA)
img2 = cv2.resize(img2,(300,300),interpolation=cv2.INTER_AREA)


res1 = cv2.add(img1,img2)



cv2.imshow('img1',img1)
cv2.imshow('img2',img2)

cv2.imshow('res1',res1)
```

![](https://img-blog.csdn.net/20180824110958514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 二、图像加权相加

> 
**cv2.addweighted(img1,w1,img2,gamma)**
- dst = img1 * w1+img2 * w2 + gamma;
- 要求img1.shape与img2.shape一致


```python
import cv2

img1 = cv2.imread('./Image/img1.png')
img2 = cv2.imread('./Image/img2.png')


img1 = cv2.resize(img1,(300,300),interpolation=cv2.INTER_AREA)
img2 = cv2.resize(img2,(300,300),interpolation=cv2.INTER_AREA)


res2 = cv2.addWeighted(img1,0.7,img2,0.3,0)


cv2.imshow('img1',img1)
cv2.imshow('img2',img2)

cv2.imshow('res2',res2)
```



![](https://img-blog.csdn.net/20180824111422509?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

















