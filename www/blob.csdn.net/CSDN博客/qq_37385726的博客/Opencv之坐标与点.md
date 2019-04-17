# Opencv之坐标与点 - qq_37385726的博客 - CSDN博客





2018年09月02日 09:36:41[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：517
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、坐标](#%E4%B8%80%E3%80%81%E5%9D%90%E6%A0%87)

[二、点](#%E4%BA%8C%E3%80%81%E7%82%B9)

[三、关于minareaRect对象rect](#%E4%B8%89%E3%80%81%E5%85%B3%E4%BA%8EminareaRect%E5%AF%B9%E8%B1%A1rect)

### 一、坐标

img的坐标以行列确定，img[row][column]可以获取到row行，column列的像素。

图片宽度*高度是300*100，三通道,用opencv的img.shape返回的是(100,300,3)，shape返回的是图像的**行数，列数，色彩通道数**。



### 二、点

点是有x轴与y轴确定的，而x轴是横轴，y轴是纵轴。P（x,y）对应的坐标实际为（y,x）是，y行x列的像素。



### 三、关于minareaRect对象rect

① rect[0]返回矩形的中心**点**，（x,y），实际上为y行x列的像素点

② rect[1]返回矩形的长和宽，顺序一定不要弄错了

③ rect[2]返回矩形的旋转角度



