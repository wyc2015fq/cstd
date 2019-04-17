# Opencv之cv2.minAreaRect - qq_37385726的博客 - CSDN博客





2018年09月02日 10:57:44[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：768
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、cv2.minAreaRect函数原型](#%E4%B8%80%E3%80%81cv2.minAreaRect%E5%87%BD%E6%95%B0%E5%8E%9F%E5%9E%8B)

[二、minAreaRect函数返回rect对象](#%E4%BA%8C%E3%80%81minAreaRect%E5%87%BD%E6%95%B0%E8%BF%94%E5%9B%9Erect%E5%AF%B9%E8%B1%A1)



## 一、cv2.minAreaRect函数原型

**cv2.minAreaRect(Points)**

其中points是点集，数据类型为ndarray，array((x1,y1),(x2,y2),....,(xn,yn))

而minAreaRect就是求出在上述点集下的最小面积矩形

eg.

![](https://img-blog.csdn.net/20151019233202145?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







## 二、minAreaRect函数返回rect对象



① rect[0]返回矩形的中心**点**，（x,y），实际上为y行x列的像素点

利用：**cv2.boxPoints(rect)**可以返回四个点的值，其中cv2.boxPoints(rect)[0]为point[0]，cv2.boxPoints(rect)[1]为point[1]......

![](https://img-blog.csdn.net/20180902105543201?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② rect[1]返回矩形的长和宽，顺序一定不要弄错了，在旋转角度上有很重要的作用

③ rect[2]返回矩形的旋转角度

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20180718102436813?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3ZvbGE5NTI3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

angel是由x轴逆时针转至W(宽)的角度。

![](https://img-blog.csdn.net/20180319132528485)

 角度范围是[-90,0) 







