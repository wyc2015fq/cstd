# 【opencv】Opencv（Python） 教程-轮廓（1）查找与绘制轮廓 - zj360202的专栏 - CSDN博客





2018年01月26日 11:18:41[zj360202](https://me.csdn.net/zj360202)阅读数：427








## 目标：


1.理解什么是轮廓


2.学习找轮廓，绘制轮廓等


3.学习函数：cv2.findContours()，cv2.drawContours()

### **什么是轮廓**


轮廓可以简单认为成将连续的点（连着边界）连在一起的曲线，具有相同的颜色或者灰度。轮廓在形状分析和物体的检测和识别中很有用。 为了更加准确，要使用二值化图像。在寻找轮廓之前，要进行阈值化处理或者 Canny 边界检测。 查找轮廓的函数会修改原始图像。如果你在找到轮廓之后还想使用原始图像的话，你应该将原始图像存储到其他变量中。在 OpenCV 中，查找轮廓就像在黑色背景中找白色物体。你应该记住，要找的物体应该是白色而背景应该是黑色。

### 查找轮廓：cv2.findContours() 


函数 cv2.findContours() 有三个参数，第一个是输入图像，第二个是轮廓检索模式，第三个是轮廓近似方法。返回值有三个，第一个是图像，第二个是轮廓，第三个是（轮廓的）层析结构。轮廓（第二个返回值）是一个 Python列表，其中存储这图像中的所有轮廓。每一个轮廓都是一个 Numpy 数组，包含对象边界点（x，y）的坐标。

### 绘制轮廓：cv2.drawContours() 


函数 cv2.drawContours() 可以被用来绘制轮廓。它可以根据你提供的边界点绘制任何形状。它的第一个参数是原始图像，第二个参数是轮廓，一个 Python 列表。第三个参数是轮廓的索引（在绘制独立轮廓是很有用，当设置为 -1 时绘制所有轮廓）。接下来的参数是轮廓的颜色和厚度等。


### 实例 （环境为Python）


图片材料：

![](https://img-blog.csdn.net/20170620092107055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



实例代码：






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480016#)[copy](http://blog.csdn.net/jjddss/article/details/73480016#)



- import numpy as np  
- import cv2  
- img = cv2.imread('test.jpg')  
- imgray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)   #彩色转灰度
- ret,thresh = cv2.threshold(imgray,127,255,0)   #进行二值化
- image, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE) # 检索模式为树形cv2.RETR_TREE，
- #轮廓存储模式为简单模式cv2.CHAIN_APPROX_SIMPLE，如果设置为 cv2.CHAIN_APPROX_NONE，所有的边界点都会被存储。
- img = cv2.drawContour(img, contours, -1, (0,255,0), 3)  #此时是将轮廓绘制到了原始图像上
- #第三个参数是轮廓的索引（在绘制独立轮廓是很有用，当设置为 -1 时绘制所有轮廓）。接下来的参数是轮廓的颜色和厚度等
- cv2.imshow('img',img)  #显示原始图像
- cv2.waitKey() #窗口等待按键，无此代码，窗口闪一下就消失
- 

效果：
![](https://img-blog.csdn.net/20170620092913930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






