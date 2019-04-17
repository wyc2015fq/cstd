# Opencv之图像固定阈值二值化处理threshold - qq_37385726的博客 - CSDN博客





2018年08月24日 12:02:58[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1682
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、图像二值化](#%E4%B8%80%E3%80%81%E5%9B%BE%E5%83%8F%E4%BA%8C%E5%80%BC%E5%8C%96)

[二、threshold](#%E4%BA%8C%E3%80%81threshold)

[三、代码](#%E4%B8%89%E3%80%81%E4%BB%A3%E7%A0%81)

# 一、图像二值化

定义：图像的二值化，就是将图像上的像素点的灰度值设置为0或255，也就是将整个图像呈现出明显的只有黑和白的视觉效果。

灰度值0：黑，灰度值255：白

   一幅图像包括目标物体、背景还有噪声，要想从多值的数字图像中直接提取出目标物体，常用的方法就是设定一个阈值T，用T将图像的数据分成两部分：大于T的像素群和小于T的像素群。这是研究灰度变换的最特殊的方法，称为图像的二值化（Binarization）。



# 二、threshold

**cv2.threshold(img, threshold, maxval,type)**

**其中：**
- threshold是设定的阈值
- maxval是当灰度值大于（或小于）阈值时将该灰度值赋成的值
- type规定的是当前二值化的方式
- **破折线为将被阈值化的值；虚线为阈值    **

![](https://img-blog.csdn.net/20180824115455484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
**cv2.THRESH_BINARY    大于阈值的部分被置为255，小于部分被置为0  **


![](https://img-blog.csdn.net/20180824115507259?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
**cv2.THRESH_BINARY_INV    大于阈值部分被置为0，小于部分被置为255    **


![](https://img-blog.csdn.net/20180824115535130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
**cv2.THRESH_TRUNC     大于阈值部分被置为threshold，小于部分保持原样 **


![](https://img-blog.csdn.net/20180824115556754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
**cv2.THRESH_TOZERO   小于阈值部分被置为0，大于部分保持不变**


![](https://img-blog.csdn.net/20180824115612458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 
**cv2.THRESH_TOZERO_INV    大于阈值部分被置为0，小于部分保持不变**


![](https://img-blog.csdn.net/20180824115624180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 其实还有很重要的cv2.THRESH_OTSU   作为图像自适应二值化的一个很优的算法Otsu大津算法的参数：

### 使用为cv2.threshold(img, 0, 255, cv2.THRESH_OTSU )







# 三、代码

```python
import cv2

img1 = cv2.imread('./Image/cv.jpg',cv2.IMREAD_GRAYSCALE)

img1 = cv2.resize(img1,(300,300),interpolation=cv2.INTER_AREA)
cv2.imshow('img1',img1)
#
#


ret,binary = cv2.threshold(img1,175,255,cv2.THRESH_BINARY)
ret,binaryinv = cv2.threshold(img1,175,255,cv2.THRESH_BINARY_INV)
ret,trunc = cv2.threshold(img1,175,255,cv2.THRESH_TRUNC)
ret,tozero = cv2.threshold(img1,175,255,cv2.THRESH_TOZERO)
ret,tozeroinv = cv2.threshold(img1,175,255,cv2.THRESH_TOZERO_INV)


cv2.imshow('binary',binary)
cv2.imshow('binaryinv',binaryinv)
cv2.imshow('trunc',trunc)
cv2.imshow('tozero',tozero)
cv2.imshow('tozeroinv',tozeroinv)
cv2.waitKey(0)
```

原图

![](https://img-blog.csdn.net/2018082412023492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- cv2.THRESH_BINARY

![](https://img-blog.csdn.net/20180824120119933?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- cv2.THRESH_BINARY_INV

![](https://img-blog.csdn.net/20180824120139926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- cv2.THRESH_TRUNC

![](https://img-blog.csdn.net/20180824120151458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- cv2.THRESH_TOZERO

![](https://img-blog.csdn.net/2018082412020251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- cv2.THRESH_TOZERO_INV

![](https://img-blog.csdn.net/20180824120214330?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







