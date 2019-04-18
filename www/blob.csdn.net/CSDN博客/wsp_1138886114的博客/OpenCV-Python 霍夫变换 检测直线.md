# OpenCV-Python 霍夫变换 检测直线 - wsp_1138886114的博客 - CSDN博客





2018年10月04日 17:00:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：853








### 一、直线表示

一条直线在图像二维空间可由两个变量表示
- 在 笛卡尔坐标系: 可由参数: ($m,b$) 斜率和截距表示.
- 在 极坐标系: 可由参数: ($r,\theta$) 极径和极角表示
- 对于霍夫变换, 我们将用 极坐标系 来表示直线. 因此, 直线的表达式可为下图：
![在这里插入图片描述](https://img-blog.csdn.net/20181004164432723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 一般来说对于点 ($x_{0}, y_{0}$), 我们可以将通过这个点的一族直线统一定义为:
$$r_{\theta} = x_0 ·cos\theta + y_0 ·cos\theta$$

这就意味着每一对 ($r_{\theta},\theta$) 代表一条通过点 ($x_{0}, y_{0}$) 的直线.
### 二、霍夫变换

Hough变换是经典的检测直线的算法。

用来检测图像中的直线，也可以用来检测图像中简单的结构。

OpenCV的中用函数 `HoughLines`(标准) 和 `HoughLinesP`(基于统计) 来检测图像中的直线.
- 基本的版本是cv2.HoughLines。其输入一幅含有点集的二值图（由非0像素表示），其中一些点互相联系组成直线。通常这是通过如Canny算子获得的一幅边缘图像。
- `cv2.HoughLines()`输出的是[float, float]形式的ndarray，

检测到的线`(ρ , θ)`中浮点点值的参数。
- 下面的例子首先使用Canny算子获得图像边缘，然后使用Hough变换检测直线。其中HoughLines函数的参数3和4对应直线搜索的步长。

在本例中：函数将通过步长为1的半径和步长为π/180的角来搜索所有可能的直线。最后一个参数是经过某一点曲线的数量的阈值，超过这个阈值，就表示这个交点所代表的参数对`(ρ , θ)`在原图像中为一条直线。

```python
```python
"""
cv2.HoughLines()
	dst:   输出图像. 它应该是个灰度图 (但事实上是个二值化图)
	lines: 储存着检测到的直线的参数对 (r,\theta) 的容器 
	rho : 参数极径 r 以像素值为单位的分辨率. 我们使用 1 像素.
	theta: 参数极角 \theta 以弧度为单位的分辨率. 我们使用 1度 (即CV_PI/180)
	threshold:    设置阈值： 一条直线所需最少的的曲线交点
	srn and stn:  参数默认为0

cv2.HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 )
	dst:    输出图像. 它应该是个灰度图 (但事实上是个二值化图) 
	lines:  储存着检测到的直线的参数对 (x_{start}, y_{start}, x_{end}, y_{end}) 的容器
	rho :   参数极径 r 以像素值为单位的分辨率. 我们使用 1 像素.
	theta:  参数极角 \theta 以弧度为单位的分辨率. 我们使用 1度 (即CV_PI/180)
	threshold:    设置阈值： 一条直线所需最少的的曲线交点
	minLinLength: 能组成一条直线的最少点的数量. 点数量不足的直线将被抛弃.
	maxLineGap:   能被认为在一条直线上的亮点的最大距离.
"""
import cv2
import numpy as np  
 

original_img= cv2.imread("jianzhu.png", 0)
img = cv2.resize(original_img,None,fx=0.8, fy=0.8, 
                 interpolation = cv2.INTER_CUBIC)
 
img = cv2.GaussianBlur(img,(3,3),0)
edges = cv2.Canny(img, 50, 150, apertureSize = 3)
lines = cv2.HoughLines(edges,1,np.pi/180,118) #这里对最后一个参数使用了经验型的值
result = img.copy()
for line in lines:
	rho = line[0][0]  #第一个元素是距离rho
	theta= line[0][1] #第二个元素是角度theta
	print (rho)
	print (theta)
	if  (theta < (np.pi/4. )) or (theta > (3.*np.pi/4.0)): #垂直直线
		pt1 = (int(rho/np.cos(theta)),0)               #该直线与第一行的交点
		#该直线与最后一行的焦点
		pt2 = (int((rho-result.shape[0]*np.sin(theta))/np.cos(theta)),result.shape[0])
		cv2.line( result, pt1, pt2, (255))             # 绘制一条白线
	else:                                                  #水平直线
		pt1 = (0,int(rho/np.sin(theta)))               # 该直线与第一列的交点
		#该直线与最后一列的交点
		pt2 = (result.shape[1], int((rho-result.shape[1]*np.cos(theta))/np.sin(theta)))
		cv2.line(result, pt1, pt2, (255), 1)           # 绘制一条直线

cv2.imshow('Canny', edges )
cv2.imshow('Result', result)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20181004110452539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
```python
import cv2
import numpy as np  
 
img = cv2.imread("jianzhu.png")
 
img = cv2.GaussianBlur(img,(3,3),0)
edges = cv2.Canny(img, 50, 150, apertureSize = 3)
lines = cv2.HoughLines(edges,1,np.pi/180,118)
result = img.copy()
 
#经验参数
minLineLength = 200
maxLineGap = 15
lines = cv2.HoughLinesP(edges,1,np.pi/180,80,minLineLength,maxLineGap)
for x1,y1,x2,y2 in lines[0]:
	cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)

cv2.imshow('Result', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
```
```

![在这里插入图片描述](https://img-blog.csdn.net/20181004163220145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

鸣谢
[https://blog.csdn.net/sunny2038/article/details/9253823](https://blog.csdn.net/sunny2038/article/details/9253823)





