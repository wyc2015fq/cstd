# 【opencv】Opencv（Python） 教程-轮廓（2）轮廓特征求取 - zj360202的专栏 - CSDN博客





2018年01月26日 11:19:49[zj360202](https://me.csdn.net/zj360202)阅读数：1406








### 目标


查找轮廓的不同特征，例如面积，周长，重心，边界框等，这些特征在未来的图像识别中，会大量的用到。



### 矩的概念
图像识别的一个核心问题是图像的特征提取，简单描述即为用一组简单的数据（图像描述量）来描述整个图像，这组数据越简单越有代表性越好。良好的特征不受光线、噪点、几何形变的干扰。图像识别发展几十年，不断有新的特征提出，而图像不变矩就是其中一个。



矩是概率与统计中的一个概念，是随机变量的一种数字特征。设为随机变量，为常数，为正整数。则量称为关于点的阶矩




比较重要的有两种情况：


1. 。这时称为的阶原点矩


2. 。这时称为的阶中心矩。


一阶原点矩就是期望。一阶中心矩μ1=0，二阶中心矩μ2就是X的方差Var(X)。在统计学上，高于4阶的矩极少使用。μ3可以去衡量分布是否有偏。μ4可以去衡量分布（密度）在均值附近的陡峭程度如何。


针对于一幅图像，我们把像素的坐标看成是一个二维随机变量(X,Y)，那么一幅灰度图像可以用二维灰度密度函数来表示，因此可以用矩来描述灰度图像的特征。






不变矩(Invariant Moments)是一处高度浓缩的图像特征，具有平移、灰度、尺度、旋转不变性。M.K.Hu在1961年首先提出了不变矩的概念。1979年M.R.Teague根据正交多项式理论提出了Zernike矩。


对于矩的相关具体计算方法及概念，参考链接http://blog.csdn.net/qq_18343569/article/details/46913501





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- import cv2  
- import numpy as np  
- img = cv2.imread('star.jpg',0)  
- img=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #彩色转灰度
- ret,thresh = cv2.threshold(img,127,255,0)   #二值化
- image,contours,hierarchy = cv2.findContours(thresh, 1, 2)  
- cnt = contours[0]   #选取其中的第一个轮廓
- M = cv2.moments(cnt)  #对第一个轮廓
- print (M)             #打印出所有计算的M的参数，其各个数值的计算公式参考http://blog.csdn.net/qq_18343569/article/details/46913501
- cx = int(M['m10']/M['m00'])  #X方向的重心，其中M['m10']表示的是x方向的一阶空间矩，M['m00']表示面积，M['m00']也可以通过cv2.contourArea() 计算得到
- cy = int(M['m01']/M['m00']) #Y方向的重心
- 


另外，关于轮廓的函数还包括如下
### 轮廓周长


cv2.arcLength()





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- perimeter = cv2.arcLength(cnt,True)  

这个函数的第二参数可以用来指定对象的形状是闭合的（True），还是打开的（一条曲线）。假如是一条闭合的曲线，那种方法计算结果一致，如果是开曲线，则两者计算结果不同，其中闭合的方法，会在最后将起始点和终止点连一起的长度加进去。
### 轮廓近似


将轮廓形状近似到另外一种由更少点组成的轮廓形状，新轮廓的点的数目由我们设定的准确度来决定。使用的Douglas-Peucker算法，你可以到维基百科获得更多此算法的细节。为了帮助理解，假设我们要在一幅图像中查找一个矩形，但是由于图像的种种原因，我们不能得到一个完美的矩形，而是一个“坏形状”（如下图所示）。现在你就可以使用这个函数来近似这个形状了。这个函数的第二个参数叫epsilon，它是从原始轮廓到近似轮廓的最大距离。它是一个准确度参数。选择一个好的 epsilon 对于得到满意结果非常重要。





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- epsilon = 0.1*cv2.arcLength(cnt,True)  
- approx = cv2.approxPolyDP(cnt,epsilon,True)  

下边，第二幅图中的绿线是当 epsilon = 10% 时得到的近似轮廓，第三幅图是当 epsilon = 1% 时得到的近似轮廓。第三个参数设定弧线是否闭合。

![](https://img-blog.csdn.net/20170620143613955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 凸包


凸包与轮廓近似相似，但不同，虽然有些情况下它们给出的结果是一样的。函数 cv2.convexHull() 可以用来检测一个曲线是否具有凸性缺陷，并能纠正缺陷。一般来说，凸性曲线总是凸出来的，至少是平的。如果有地方凹进去了就被叫做凸性缺陷。例如下图中的手。红色曲线显示了手的凸包，凸性缺陷被双箭头标出来了。

![](https://img-blog.csdn.net/20170620143958522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


参考代码为：






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- hull = cv2.convexHull(points, hull, clockwise, returnPoints)  

points 我们要传入的轮廓• hull 输出，通常不需要• clockwise 方向标志。如果设置为 True，输出的凸包是顺时针方向的。否则为逆时针方向。• returnPoints 默认值为 True。它会返回凸包上点的坐标。如果设置为 False，就会返回与凸包点对应的轮廓上的点。

### 凸性检测


函数 cv2.isContourConvex() 可以可以用来检测一个曲线是不是凸的。它只能返回 True 或 False。






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- k = cv2.isContourConvex(cnt)  


### 边界矩形

**直边界矩形** 一个直矩形（就是没有旋转的矩形）。它不会考虑对象是否旋转。所以边界矩形的面积不是最小的。可以使用函数 cv2.boundingRect() 查找得到。（x，y）为矩形左上角的坐标，（w，h）是矩形的宽和高。





**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- x,y,w,h = cv2.boundingRect(cnt)  
- img = cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)  

旋转的边界矩形 这个边界矩形是面积最小的，因为它考虑了对象的旋转。用到的函数为 cv2.minAreaRect()。返回的是一个 Box2D 结构，其中包含矩形左上角角点的坐标（x，y），矩形的宽和高（w，h），以及旋转角度。但是要绘制这个矩形需要矩形的
 4 个角点，可以通过函数 cv2.boxPoints() 获得。



**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- 


```python
rect = cv2.minAreaRect(cnt)
box = cv2.cv.BoxPoints(rect)
box = np.int0(box)
cv2.drawContours(img, [box], 0, (0, 0, 255), 2)
```
把这两中边界矩形显示在下图中，其中绿色的为直矩形，红的为旋转矩形。完整代码如下：






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- # 用绿色(0, 255, 0)来画出最小的矩形框架
- x, y, w, h = cv2.boundingRect(cnt)  
- cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)  
- 
- # 用红色表示有旋转角度的矩形框架
- rect = cv2.minAreaRect(cnt)  
- box = cv2.cv.BoxPoints(rect)  
- box = np.int0(box)  
- cv2.drawContours(img, [box], 0, (0, 0, 255), 2)  
- cv2.imwrite('contours.png', img)  


![](https://img-blog.csdn.net/20170620151146892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





### 最小外接圆


函数 cv2.minEnclosingCircle() 可以帮我们找到一个对象的外切圆。它是所有能够包括对象的圆中面积最小的一个。






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- (x,y),radius = cv2.minEnclosingCircle(cnt)  
- center = (int(x),int(y))  
- radius = int(radius)  
- img = cv2.circle(img,center,radius,(0,255,0),2)  


![](https://img-blog.csdn.net/20170620152102974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





### 椭圆拟合


使用的函数为 cv2.ellipse()，返回值其实就是旋转边界矩形的内切圆






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- ellipse = cv2.fitEllipse(cnt)  
- im = cv2.ellipse(im,ellipse,(0,255,0),2)  



### ![](https://img-blog.csdn.net/20170620152315957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 直线拟合


我们可以根据一组点拟合出一条直线，同样我们也可以为图像中的白色点拟合出一条直线。






**[python]**[view
 plain](http://blog.csdn.net/jjddss/article/details/73480497#)[copy](http://blog.csdn.net/jjddss/article/details/73480497#)



- rows,cols = img.shape[:2]  
- [vx,vy,x,y] = cv2.fitLine(cnt, cv2.DIST_L2,0,0.01,0.01)  
- lefty = int((-x*vy/vx) + y)  
- righty = int(((cols-x)*vy/vx)+y)  
- img = cv2.line(img,(cols-1,righty),(0,lefty),(0,255,0),2)  


![](https://img-blog.csdn.net/20170620152534618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvampkZHNz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)














