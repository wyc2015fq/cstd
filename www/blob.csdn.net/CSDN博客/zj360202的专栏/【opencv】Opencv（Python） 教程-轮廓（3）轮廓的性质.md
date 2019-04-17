# 【opencv】Opencv（Python） 教程-轮廓（3）轮廓的性质 - zj360202的专栏 - CSDN博客





2018年01月26日 11:20:55[zj360202](https://me.csdn.net/zj360202)阅读数：416








#### 边界矩形的宽高比


在上一小节中，我们提到利用下面函数能够得到轮廓的直边界矩形

x,y,w,h = cv2.boundingRect(cnt) 

对于某一轮廓要求取其边界矩形的宽高比，则可通过下面代码实现

```python
x,y,w,h = cv2.boundingRect(cnt)
aspect_ratio = float(w)/h #注意要转化成浮点型
```
- 1
- 2

#### 轮廓面积与边界矩形面积的比

```python
area = cv2.contourArea(cnt) 
x,y,w,h = cv2.boundingRect(cnt) #计算边界矩形相关参数
rect_area = w*h
extent = float(area)/rect_area
```
- 1
- 2
- 3
- 4

#### 轮廓面积与凸包面积的比

```python
area = cv2.contourArea(cnt)  #轮廓面积
hull = cv2.convexHull(cnt) #计算出凸包形状
hull_area = cv2.contourArea(hull) #计算凸包面积
solidity = float(area)/hull_area
```
- 1
- 2
- 3
- 4

#### 与轮廓面积相等的圆形的直径

```python
area = cv2.contourArea(cnt)
equi_diameter = np.sqrt(4*area/np.pi)
```
- 1
- 2

#### 对象方向（计算长轴及短轴）

```python
(x,y),(MA,ma),angle = cv2.fitEllipse(cnt) #MA,ma 分别为长轴短轴长度
```
- 1

#### 掩模和像素点


有时我们需要构成对象的所有像素点，我们可以这样做：

```python
import numpy as np
import cv2
img = cv2.imread('test.jpg')
imgray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #彩色图像转灰度
mask = np.zeros(imgray.shape,np.uint8) #依据图像的大小建立一个全黑的模板
ret,thresh = cv2.threshold(imgray,127,255,0) #灰度图像二值化处理
thresh,cnt,hierarchy = cv2.findContours(thresh, 1, 2) #
cv2.drawContours(mask,[cnt[num]],0,255,-1) #num为希望进行掩模显示的轮廓， # 这里一定要使用参数-1, 绘制填充的的轮廓 #[cnt[num]] 这个中括号一定要加上
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

#### 轮廓最大值和最小值及它们的位置


使用掩模图像得到这些参数

```python
min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(imgray,mask = mask)
```
- 1

#### 轮廓平均颜色及平均灰度

```python
mean_valCol = cv2.mean(img,mask = mask) #彩色图像的平均颜色
mean_valGray=cv2.mean(imgray,mask = mask) #平均灰度
```
- 1
- 2

#### 轮廓极点

```python
cnt=cnts[num] ##首先从所有的轮廓中选择其中的一个轮廓
leftmost = tuple(cnt[cnt[:,:,0].argmin()][0])
rightmost = tuple(cnt[cnt[:,:,0].argmax()][0])
topmost = tuple(cnt[cnt[:,:,1].argmin()][0])
bottommost = tuple(cnt[cnt[:,:,1].argmax()][0])
cv2.circle(img,left,10,[0,0,255],-1)##将最左边点用圆绘制在img图像上，直径为10，颜色红色
```



