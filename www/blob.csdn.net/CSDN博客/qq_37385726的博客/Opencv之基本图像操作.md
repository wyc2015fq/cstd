# Opencv之基本图像操作 - qq_37385726的博客 - CSDN博客





2018年08月23日 09:35:10[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：55
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**



[一、主要函数](#%E4%B8%80%E3%80%81%E4%B8%BB%E8%A6%81%E5%87%BD%E6%95%B0)

[二、代码（保存黑白图片）](#%E4%BA%8C%E3%80%81%E4%BB%A3%E7%A0%81%EF%BC%88%E4%BF%9D%E5%AD%98%E9%BB%91%E7%99%BD%E5%9B%BE%E7%89%87%EF%BC%89)



# 一、主要函数
-  cv2.imread()：读入图片，共两个参数，第一个参数为要读入的图片文件名，第二个参数为如何读取图片，包括cv2.IMREAD_COLOR：读入一副彩色图片；cv2.IMREAD_GRAYSCALE：以灰度模式读入图片；cv2.IMREAD_UNCHANGED：读入一幅图片，并包括其alpha通道。
- cv2.imshow()：创建一个窗口显示图片，共两个参数，第一个参数表示窗口名字，可以创建多个窗口中，但是每个窗口不能重名；第二个参数是读入的图片。
- cv2.waitKey()：键盘绑定函数，共一个参数，表示等待毫秒数，将等待特定的几毫秒，看键盘是否有输入，返回值为ASCII值。如果其参数为0，则表示无限期的等待键盘输入。
- cv2.destroyAllWindows()：删除建立的全部窗口。
- cv2.destroyWindows()：删除指定的窗口。
- cv2.imwrite()：保存图片，共两个参数，第一个为保存文件名，第二个为读入图片。



# 二、代码（保存黑白图片）

```python
import cv2
img1 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_COLOR)
img2 = cv2.imread('./Image/reba_color.jpg',cv2.IMREAD_GRAYSCALE)

cv2.imshow('image1',img1)
cv2.imshow('image2',img2)

receive = cv2.waitKey(0)

if receive == ord('e'):
    cv2.destroyAllWindows()
elif receive == ord('s'):
    cv2.imwrite('./Image/reba_heibai.jpg',img2)
    cv2.destroyAllWindows()
```

![](https://img-blog.csdn.net/20180823093348917?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180823093419696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)









