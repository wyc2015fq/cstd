# Opencv之图像切割ROI选取 - qq_37385726的博客 - CSDN博客





2018年08月24日 10:56:36[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：403
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、简介](#%E4%B8%80%E3%80%81%E7%AE%80%E4%BB%8B)

[二、代码](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81)



# 一、简介

> 
由于OpenCv中,imread()函数读进来的图片,其本质上就是一个三维的数组,这个NumPy中的三维数组是一致的,所以设置图片的
ROI区域的问题,就转换成数组的切片问题,在Python中,数组就是一个列表序列,所以使用列表的切片就可以完成ROI区域的设置

利用img[y:y_to,x:x_to]来返回img图片的切割后图片，其中y，y_to，x，x_to是圈定的范围



# 二、代码

```python
import cv2

img1 = cv2.imread('./Image/reba_color.jpg')

img_roi_y = 50  # [1]设置ROI区域的左上角x
img_roi_x = 100
img_roi_height = 100  # [2]设置ROI区域的高度
img_roi_width = 300  # [3]设置ROI区域的宽度

img_roi = img1[img_roi_y:(img_roi_y + img_roi_height), img_roi_x:(img_roi_x + img_roi_width)]


cv2.imshow("[ROI_Img]", img_roi)
cv2.waitKey(0)
cv2.destroyWindow("[ROI_Img]")  # [5]python中,创建的窗口,需要手动的销毁
```

![](https://img-blog.csdn.net/20180824105619666?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



