# 【opencv】opencv-OpenCV中的图像处理 [2]-OpenCV中的轮廓 - zj360202的专栏 - CSDN博客





2018年01月26日 11:34:55[zj360202](https://me.csdn.net/zj360202)阅读数：261








参考：

1、[http://docs.opencv.org/3.3.0/](http://docs.opencv.org/3.2.0/)官方文档api

2、[http://docs.opencv.org/3.3.0/d6/d00/tutorial_py_root.html](http://docs.opencv.org/3.2.0/d6/d00/tutorial_py_root.html)官方英文教程

3、https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_tutorials.html


4、https://github.com/makelove/OpenCV-Python-Tutorial# 进阶教程


5、https://docs.opencv.org/3.3.0/index.html  官方英文教程

6、[https://github.com/abidrahmank/OpenCV2-Python-Tutorials](http://docs.opencv.org/3.2.0/d6/d00/tutorial_py_root.html)

7、[https://www.learnopencv.com/](http://docs.opencv.org/3.2.0/d6/d00/tutorial_py_root.html)

8、[http://answers.opencv.org/questions/](http://docs.opencv.org/3.2.0/d6/d00/tutorial_py_root.html)OpenCV论坛




注：安装的版本 opencv_python-3.3.0-cp36-cp36m-win_amd64.whl







参考：https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_tutorials.html












# OpenCV中的轮廓

# 轮廓：入门


## 目标

> - Understand what contours are.
- Learn to find contours, draw contours etc
- You will see these functions : **cv2.findContours()**, **cv2.drawContours()**



## 什么是轮廓?
轮廓可以简单地解释为连接所有连续点（沿着边界），具有相同颜色或强度的曲线。 轮廓是形状分析和物体检测和识别的有用工具。



- 为了更好的准确性，使用二进制图像 所以在找到轮廓之前，应用阈值或canny边缘检测。
- findContours函数修改源图像。 所以如果你想要源图像，即使在找到轮廓之后，已经存储到一些其他的变量。
- 在OpenCV中，查找轮廓就像从黑色背景中找到白色物体。 所以记住，要找到的对象应该是白色的，背景应该是黑色的。

让我们看看如何找到二进制图像的轮廓：


**import **numpy **as **np
**import **cv2

im = cv2.imread('Lenna.png')
imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
ret,thresh = cv2.threshold(imgray,127,255,0)
image, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
cv2.imshow('img',image)
cv2.waitKey(0)
cv2.destroyAllWindows()参见cv2.findContours（）函数有三个参数，第一个是源图像，第二个是轮廓检索模式，第三个是轮廓逼近方法。 并输出图像，轮廓和层次结构。 轮廓是图像中所有轮廓的Python列表。 每个单独的轮廓是对象的边界点的（x，y）坐标的Numpy数组。



稍后我们将详细讨论第二和第三个参数和关于层次结构。 在此之前，代码示例中赋予它们的值对所有图像都可以正常工作。







## 如何绘制轮廓?




要绘制轮廓，使用cv2.drawContours函数。 它也可以用来绘制任何形状，只要你有它的边界点。 它的第一个参数是源图像，第二个参数是应该作为Python列表传递的轮廓，第三个参数是轮廓的索引（在绘制单个轮廓时有用）绘制所有轮廓，传递-1），剩余的参数是颜色，厚度 等等


绘制图像中的所有轮廓：

img=cv2.drawContours(img,contours,-,(,,),)绘制第4个轮廓



img=cv2.drawContours(img,contours,,(,,),)





但大多数时候，下面的方法将是有用的：


cnt=contours[]img=cv2.drawContours(img,[cnt],,(,,),)



最后两个方法是一样的，但是当你向前看时，你会看到最后一个更有用。





## 轮廓近似法
这是cv2.findContours函数的第三个参数。它实际上代表什么？

以上，我们告诉轮廓是具有相同强度的形状的边界。它存储形状边界的（x，y）坐标。但是它是否存储所有坐标？这是通过这种轮廓逼近法来规定的。

如果您通过cv2.CHAIN_APPROX_NONE，则会存储所有边界点。但实际上我们需要所有的要点吗？例如，您发现一条直线的轮廓。你需要在线上的所有点来表示这一行吗？不，我们只需要这条线的两个终点。这是cv2.CHAIN_APPROX_SIMPLE所做的。它删除所有冗余点并压缩轮廓，从而节省内存。

在矩形下面的图像展示了这种技术。在轮廓数组的所有坐标上绘制一个圆（用蓝色绘制）。第一张图片显示了我用cv2.CHAIN_APPROX_NONE（734分）得到的积分，第二张图像显示了cv2.CHAIN_APPROX_SIMPLE（只有4分）的图像。看，它节省多少内存！

![Contour Retrieval Method](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/none.jpg)


**import **numpy **as **np
**import **cv2

im = cv2.imread('messi5.jpg')
imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)

cv2.imshow('img0',imgray)

ret,thresh = cv2.threshold(imgray,127,255,0)
image, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

img = cv2.drawContours(imgray, contours, -1, (0,255,0), 3) *# -1 **表示绘制所有轮廓*cv2.imshow('img1',img)

img = cv2.drawContours(imgray, contours, 3, (0,255,0), 3) *# **绘制第**4**个轮廓*cv2.imshow('img2',img)
*# **但大多数时候，下面的方法将是有用的：*cnt = contours[4]
img = cv2.drawContours(imgray, [cnt], 0, (0,255,0), 3)
cv2.imshow('img3',img)
cv2.imshow('img',image)
cv2.waitKey(0)
cv2.destroyAllWindows()








# 轮廓特征


## 目标


In this article, we will learn

> - To find the different features of contours, like area, perimeter, centroid, bounding box etc
- You will see plenty of functions related to contours.




## 1. Moments

Image moments 可以帮助您计算一些特征，如物体的质心，物体的面积等。查看[Image
 Moments](http://en.wikipedia.org/wiki/Image_moment)的维基百科页面

函数cv2.moments（）给出了计算的所有moment 值的字典。
 见下文：



**import **cv2
**import **numpy **as **np

img = cv2.imread('lenna.png',0)
ret,thresh = cv2.threshold(img,127,255,0)
image,contours,hierarchy = cv2.findContours(thresh, 1, 2)

cnt = contours[0]
M = cv2.moments(cnt)
print(M)从这moments，您可以提取有用的数据，如面积，质心等。质心由关系给出，![C_x = \frac{M_{10}}{M_{00}}](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/math/41d9674b5828dd601e9517557fbdef3f2ee81c7b.png) and ![C_y = \frac{M_{01}}{M_{00}}](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/math/33e173a81f373aea8d354af006ddd31a82ac4e10.png)




cx=int(M['m10']/M['m00'])cy=int(M['m01']/M['m00'])





## 2. 轮廓区域
Contour area is given by the function cv2.contourArea() or
 from moments, M[‘m00’].



area=cv2.contourArea(cnt)





## 3. 轮廓周长
也称为弧长。 可以使用cv2.arcLength（）函数找到它。 第二个参数指定shape是封闭轮廓（如果通过True）或仅仅是曲线。



perimeter=cv2.arcLength(cnt,True)
## 4. 轮廓近似




它根据我们指定的精度将轮廓形状近似为具有较少数量顶点的另一形状。 这是Douglas-Peucker算法的实现。 检查维基百科页面的算法和演示。


要理解这一点，假设您正在尝试在图像中找到一个正方形，但是由于图像中存在一些问题，您没有获得一个完美的方形，而是一个“坏的形状”（如下图所示）。 现在您可以使用此功能近似形状。 在这里，第二个参数称为epsilon，它是从轮廓到近似轮廓的最大距离。 这是一个精度参数。 需要一个明智的选择来获得正确的输出。

epsilon=0.1*cv2.arcLength(cnt,True)approx=cv2.approxPolyDP(cnt,epsilon,True)


以下，在第二幅图像中，绿线表示ε= 10％弧长的近似曲线。 epsilon = 1％的弧长显示相同的图像。 第三个参数指定曲线是否闭合。![Contour Approximation](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/approx.jpg)



## 5. 凸 Hull




Convex Hull将看起来类似于轮廓近似，但它不是（两者可能在某些情况下提供相同的结果）。 这里，cv2.convexHull（）函数检查一个曲线的凸性缺陷并进行修正。 一般来说，凸曲线是总是凸出的或至少平坦的曲线。 如果内部膨胀，则称为凸面缺陷。 例如，检查下面的图像。 红线显示手的凸包。 双面箭头标记显示凸度缺陷，这是船体与轮廓的局部最大偏差。

![Convex Hull](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/convexitydefects.jpg)hull=cv2.convexHull(points[,hull[,clockwise[,returnPoints]]参数细节：

points是我们传入的轮廓。
hull是输出，通常我们避免它。
clockwise顺时针方向标志。 如果为真，输出凸包为顺时针方向。 否则，它是逆时针方向。
returnPoints：默认情况下为True。 然后它返回船体点的坐标。 如果为False，则返回与船体点对应的轮廓点的索引。
所以为了得到如上图所示的凸包，以下是足够的：


hull=cv2.convexHull(cnt)但是如果要查找凸性缺陷，则需要传递returnPoints = False。 要理解它，我们将把上面的矩形图像。 首先我发现它的轮廓为cnt。 现在我发现它的凸包具有returnPoints = True，我得到以下值：[[[234 202]]，[[51 202]]，[[51 79]]，[[234
 79]]] 矩形点。 现在如果对returnPoints = False做同样的，我得到以下结果：[[129]，[67]，[0]，[142]]。 这些是轮廓中相应点的指标。 例如，检查与第一个结果相同的第一个值：cnt [129] = [[234,202]]（对于其他结果，等等）。

当我们讨论凸性缺陷时，您将再次看到它。







## 6. 检查凸度
有一个函数来检查曲线是否是凸的，cv2.isContourConvex（）。 它只返回是True还是False。 没什么大不了的。



k=cv2.isContourConvex(cnt)
## 7. 边界矩形




有两种类型的边界矩形。



### 7.a. 直边矩形
它是一个直的矩形，它不考虑对象的旋转。 所以边界矩形的区域不会是最小的。 它由函数cv2.boundingRect（）发现。

令（x，y）为矩形的左上角坐标，（w，h）为其宽度和高度。


x,y,w,h=cv2.boundingRect(cnt)img=cv2.rectangle(img,(x,y),(x+w,y+h),(,,),)
### 7.b. 旋转矩形
这里，以最小面积绘制边界矩形，因此也考虑旋转。 使用的功能是cv2.minAreaRect（）。 它返回一个Box2D结构，其包含以下detals - （左上角（x，y），（宽度，高度），旋转角度）。 但是要绘制这个矩形，我们需要矩形的四个角。 它是通过函数cv2.boxPoints（）获得的




rect=cv2.minAreaRect(cnt)box=cv2.boxPoints(rect)box=np.int0(box)im=cv2.drawContours(im,[box],,(,,),)两个矩形都显示在单个图像中。 绿色矩形显示正常的边界。 红色矩形是旋转的矩形。


![Bounding Rectangle](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/boundingrect.png)



## 8. 最小封闭圈
接下来，我们使用函数cv2.minEnclosingCircle（）找到对象的外接圆。 它是一个圆形，完全覆盖最小面积的物体。





(x,y),radius=cv2.minEnclosingCircle(cnt)center=(int(x),int(y))radius=int(radius)img=cv2.circle(img,center,radius,(,,),)

![Minimum Enclosing Circle](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/circumcircle.png)





## 9. Fitting一个椭圆
下一个是将一个椭圆适合于一个对象。 它返回旋转的矩形，其中刻有椭圆。





ellipse=cv2.fitEllipse(cnt)im=cv2.ellipse(im,ellipse,(,,),)

![Fitting an Ellipse](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/fitellipse.png)
## 10. Fitting一条线
类似地，我们可以将一条线适合一组点。 图像下方包含一组白点。 我们可以近似一条直线。





rows,cols=img.shape[:][vx,vy,x,y]=cv2.fitLine(cnt,cv2.DIST_L2,,0.01,0.01)lefty=int((-x*vy/vx)+y)righty=int(((cols-x)*vy/vx)+y)img=cv2.line(img,(cols-,righty),(,lefty),(,,),)

![Fitting a Line](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/fitline.jpg)




**import **cv2
**import **numpy **as **np

img = cv2.imread('j.png',0)
ret,thresh = cv2.threshold(img,127,255,0)
image,contours,hierarchy = cv2.findContours(thresh, 1, 2)

cnt = contours[0]

*# x,y,w,h = cv2.boundingRect(cnt)**# img = cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)**# rect = cv2.minAreaRect(cnt)**# box = cv2.boxPoints(rect)**# box = np.int0(box)**# img = cv2.drawContours(img,[box],0,(0,0,255),2)**#**# cv2.imshow('img',img)**# (x,y),radius = cv2.minEnclosingCircle(cnt)**# center = (int(x),int(y))**# radius = int(radius)**# img = cv2.circle(img,center,radius,(0,255,0),2)*ellipse = cv2.fitEllipse(cnt)
img = cv2.ellipse(img,ellipse,(0,255,0),2)

cv2.imshow('img',img)
cv2.waitKey(0)
cv2.destroyAllWindows()








# 轮廓属性
在这里我们将学习提取一些常用的属性，如Solidity，Equivalent Diameter，Mask image，Mean Intensity等。更多的功能可以在Matlab的regionprops文档中找到。

（注意：质心，面积，周长等也属于这一类，但我们已经在上一章看到）








## 1. 长宽比
它是对象边界的宽度与高度之比。






![Aspect \; Ratio = \frac{Width}{Height}](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/math/f9b59fc75e892c6f0b6b5cefb322c048f41d1e81.png)



x,y,w,h=cv2.boundingRect(cnt)aspect_ratio=float(w)/h







## 2. 程度
范围是轮廓面积与边界矩形区域的比率。





![Extent = \frac{Object \; Area}{Bounding \; Rectangle \; Area}](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/math/22a73ff9d27701129a1bfed7259db29e62892729.png)



area=cv2.contourArea(cnt)x,y,w,h=cv2.boundingRect(cnt)rect_area=w*hextent=float(area)/rect_area


## 3. 密实度
solidity 是轮廓面积与其凸包面积的比值。






![Solidity = \frac{Contour \; Area}{Convex \; Hull \; Area}](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/math/686e09ed3af5ffc8d58648dba226e47992d749e6.png)



area=cv2.contourArea(cnt)hull=cv2.convexHull(cnt)hull_area=cv2.contourArea(hull)solidity=float(area)/hull_area


## 4. 等效直径
等效直径是其面积与轮廓面积相同的圆的直径。





![Equivalent \; Diameter = \sqrt{\frac{4 \times Contour \; Area}{\pi}}](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/math/dd455145891023674d52ad7f48d9ff5a2613e49b.png)



area=cv2.contourArea(cnt)equi_diameter=np.sqrt(*area/np.pi)


## 5. 方向
方向是物体指向的角度。 以下方法还给出了主轴和次轴长度。



(x,y),(MA,ma),angle=cv2.fitEllipse(cnt)





## 6.  掩膜和像素点
在某些情况下，我们可能需要包含该对象的所有点。 可以做到如下：




mask=np.zeros(imgray.shape,np.uint8)cv2.drawContours(mask,[cnt],,,-)pixelpoints=np.transpose(np.nonzero(mask))#pixelpoints = cv2.findNonZero(mask)这里，使用两个方法，一个使用Numpy函数，下一个使用OpenCV函数（最后一个注释行）也是这样做的。 结果也是一样的，但略有不同。 Numpy以（行，列）格式给出坐标，而OpenCV以（x，y）格式给出坐标。 所以基本上答案将互换。 请注意，row = x和column = y。








## 7. 最大值，最小值及其位置
我们可以使用掩模图像找到这些参数。




min_val,max_val,min_loc,max_loc=cv2.minMaxLoc(imgray,mask=mask)





## 8.平均颜色或平均强度
在这里，我们可以找到一个对象的平均颜色。 或者它可以是灰度模式下对象的平均强度。 我们再次使用相同的面具来做到这一点。




mean_val=cv2.mean(im,mask=mask)





## 9. 极点
极点表示物体的最高，最下，最右和最左点。




leftmost=tuple(cnt[cnt[:,:,].argmin()][])rightmost=tuple(cnt[cnt[:,:,].argmax()][])topmost=tuple(cnt[cnt[:,:,].argmin()][])bottommost=tuple(cnt[cnt[:,:,].argmax()][])例如，如果我将其应用于印度地图，我会得到以下结果：


![Extreme Points](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/extremepoints.jpg)






# 轮廓：更多功能


## 目标
- In this chapter, we will learn about
- Convexity defects and how to find them.
- Finding shortest distance from a point to a polygon
- Matching different shapes



## 理论与代码


### 1. Convexity缺陷


我们在第二章中看到了什么是凸包，关于轮廓。 物体与hull的任何偏差都可以认为是凸面缺陷。

OpenCV带有一个现成的函数来找到这个，cv2.convexityDefects（）。 基本的函数调用如下所示：



hull=cv2.convexHull(cnt,returnPoints=False)defects=cv2.convexityDefects(cnt,hull)记住我们必须在找到凸包时传递returnPoints = False，才能找到凸性缺陷。






它返回一个数组，其中每行包含这些值 - [起始点，终点，最远点，到最远点的近似距离]。 我们可以使用图像进行可视化。 我们画一条加入起点和终点的线，然后在最远点画一个圆。 记住返回的前三个值是cnt的索引。 所以我们必须把这些值从cnt。



**import **cv2
**import **numpy **as **np

img = cv2.imread('contour.jpg')
img_gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
ret, thresh = cv2.threshold(img_gray, 127, 255,0)
image,contours,hierarchy = cv2.findContours(thresh,2,1)
cnt = contours[0]

hull = cv2.convexHull(cnt,returnPoints = **False**)
defects = cv2.convexityDefects(cnt,hull)

**for **i **in **range(defects.shape[0]):
    s,e,f,d = defects[i,0]
    start = tuple(cnt[s][0])
    end = tuple(cnt[e][0])
    far = tuple(cnt[f][0])
    cv2.line(img,start,end,[0,255,0],2)
    cv2.circle(img,far,5,[0,0,255],-1)

cv2.imshow('img',img)
cv2.waitKey(0)
cv2.destroyAllWindows()![Convexity Defects](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/defects.jpg)








### 2.点多边形测试




该功能可以找到图像中某个点与轮廓之间的最短距离。 当点在轮廓外部时，返回的距离为负，当点在内部时为正，如果点在轮廓上，则为零。


例如，我们可以检查点（50,50）如下：

dist=cv2.pointPolygonTest(cnt,(,),True)



在函数中，第三个参数是measureDist。 如果为True，则找到已签名的距离。 如果为False，则会发现该点是在内部还是外部还是在轮廓上（分别返回+1，-1，0）。





如果您不想找距离，请确保第三个参数为False，因为这是一个耗时的过程。 所以，使它成为假提供2-3倍加速。






### 3. 匹配形状
OpenCV带有一个函数cv2.matchShapes（），使我们能够比较两个形状或两个轮廓，并返回一个显示相似度的度量。 结果越低，匹配越好。 它是根据hu-moment值计算的。 文档中介绍了不同的测量方法。


**import **cv2
**import **numpy **as **np

img1 = cv2.imread('star.jpg',0)
img2 = cv2.imread('star2.jpg',0)

ret, thresh = cv2.threshold(img1, 127, 255,0)
ret, thresh2 = cv2.threshold(img2, 127, 255,0)
contours,hierarchy = cv2.findContours(thresh,2,1)
cnt1 = contours[0]
contours,hierarchy = cv2.findContours(thresh2,2,1)
cnt2 = contours[0]

ret = cv2.matchShapes(cnt1,cnt2,1,0.0)
print ret![Match Shapes](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/matchshapes.jpg)






# 轮廓等级


## 目标


This time, we learn about the hierarchy of contours, i.e. the parent-child relationship in Contours.



## 理论

在关于轮廓的最后几篇文章中，我们已经处理了与OpenCV提供的轮廓相关的几个功能。 但是当我们使用cv2.findContours（）函数找到图像中的轮廓时，我们已经传递了一个参数Contour Retrieval Mode。 我们通常通过cv2.RETR_LIST或cv2.RETR_TREE，它工作不错。
 但它实际上是什么意思？

另外，在输出中，我们得到三个数组，第一个是图像，第二个是我们的轮廓，另一个是我们命名为层次结构的输出（请先查看以前的文章中的代码）。 但是我们从来没有使用这个层次结构 那么这个层次结构是什么呢？ 它与前面提到的函数参数有什么关系？

这就是我们在本文中要处理的。





### 什么是层次结构?
通常我们使用cv2.findContours（）函数来检测图像中的对象，对吧？ 有时对象位于不同的位置。 但在某些情况下，某些形状在其他形状之内。 就像嵌套的数字。 在这种情况下，我们称外部为父，而内为1为子。 这样，图像中的轮廓与彼此有一些关系。 我们可以指定一个轮廓如何相互连接，就像是一些其他轮廓的小孩，还是一个轮廓，或者这是一个父类。这种关系的表示被称为层次结构。






![Hierarchy Representation](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/hierarchy.png)

在这个图像中，有几个形状，我从0-5编号。 2和2a表示最外面的盒子的外部和内部轮廓。


这里，轮廓0,1,2是外部的或最外面的。 我们可以说，它们处于层次结构0，或者只是它们处于相同的层次结构。


接下来是轮廓2a。 它可以被认为是轮廓-2（或相反的方式，轮廓-2是轮廓2a的父母）的小孩。 所以让它在层次结构1中。 类似的轮廓-3是轮廓-2的小孩，它进入下一层。 最后轮廓4,5是轮廓3a的孩子，他们进入最后一层次。 从我编号框的方式，我会说轮廓-4是轮廓3a的第一个孩子（它也可以是轮廓-5）。


我提到这些东西要了解同样的层次结构，外部轮廓，子轮廓，父轮廓，第一个孩子等等。现在让我们进入OpenCV。






### OpenCV中的层次表示
所以每个轮廓都有自己的信息，它是什么层次结构，谁是它的孩子，是谁的父类。OpenCV将它表示为四个值的数组：[Next，Previous，First_Child，Parent]

“下一个表示下一轮廓在相同的层次。”

例如，在我们的图片中取轮廓0。谁在同一级别的下一轮廓？它是轮廓-1。所以简单地把Next = 1.类似地，对于轮廓-1，接下来是轮廓-2。所以Next = 2。

轮廓2怎么样？同一级别没有下一个轮廓。所以简单地说，把Next = -1。轮廓-4怎么样？它与轮廓-5在同一水平。所以下一个轮廓是轮廓-5，所以Next = 5。

“上一个表示同一层次上的先前轮廓”。

与上述相同。轮廓-1的先前轮廓是同一水平的轮廓0。轮廓-2类似，它是轮廓-1。对于轮廓0，没有以前的，所以把它作为-1。

“First_Child表示其第一个子轮廓。”

没有任何解释。对于轮廓2，小孩是轮廓2a。因此得到轮廓2a的相应索引值。轮廓3a如何？它有两个孩子。但是我们只拿一个孩子。它是轮廓-4。对于contour-3a，First_Child = 4。

“父代表其父轮廓的索引”。

它正好与First_Child相反。轮廓4和轮廓-5均为轮廓3a。对于轮廓3a，它是轮廓-3等等。






如果没有孩子或父母，那么该字段被取为-1


所以现在我们知道OpenCV中使用的层次结构样式，我们可以借助上面给出的相同图像来查看OpenCV中的轮廓检索模式。 即，像cv2.RETR_LIST，cv2.RETR_TREE，cv2.RETR_CCOMP，cv2.RETR_EXTERNAL等标志是什么意思？







## 轮廓检索模式

### 1. RETR_LIST
这是四个标志中最简单的（从解释的角度来看）。 它只是检索所有轮廓，但不创建任何父子关系。 父母和孩子在这个规则下是平等的，只是轮廓。 即它们都属于同一层级。

所以这里，层次结构数组中的第3和第4项总是-1。 但是显然，下一个和前一个术语将具有相应的值。 只需检查一下它并验证。

下面是我得到的结果，每行是相应轮廓的层次结构细节。 例如，第一行对应于轮廓0.下一轮廓是轮廓1.所以下一个= 1。没有先前的轮廓，所以上一个= 0，而剩下的两个，如前所述，它是-1。







>>> hierarchyarray([[[ 1, -1, -1, -1],        [ 2,  0, -1, -1],        [ 3,  1, -1, -1],        [ 4,  2, -1, -1],        [ 5,  3, -1, -1],        [ 6,  4, -1, -1],        [ 7,  5, -1, -1],        [-1,  6, -1, -1]]])如果您不使用任何层次结构功能，这是您的代码中使用的好选择。








### 2. RETR_EXTERNAL
如果你使用这个标志，它只返回极端的外部标志。 所有子轮廓都留下。 我们可以说，根据这项法律，只有每个家庭中的长者才能得到照顾。 它不在乎家庭的其他成员:)。

那么，在我们的形象中，有多少极端的外轮廓呢？ 即在层次0级？ 只有3，即轮廓0,1,2，对吗？ 现在尝试使用此标志找到轮廓。 在这里，赋予每个元素的值与上述相同。 将其与上述结果进行比较。 以下是我得到的：




>>> hierarchyarray([[[ 1, -1, -1, -1],        [ 2,  0, -1, -1],        [-1,  1, -1, -1]]])如果要仅提取外部轮廓，则可以使用此标志。
 在某些情况下可能会有用。







### 3. RETR_CCOMP
该标志检索所有轮廓并将其排列到2级层次结构。 即对象（即其边界）的外部轮廓被放置在层次结构-1中。 并且对象（如果有的话）中的孔的轮廓被放置在层次结构-2中。 如果其中的任何对象，其轮廓再次放置在层次结构1中。 而它的洞在等级2等等。

只要在黑色背景上考虑“大白零”的图像。 零圈零属于第一层次，内圈零属于第二层次。

我们可以用简单的图像来解释它。 在这里，我已经以绿色（1或2）标记了红色和他们所属的层次结构的轮廓顺序。 该顺序与OpenCV检测轮廓的顺序相同。



![CCOMP Hierarchy](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/ccomp_hierarchy.png)

所以考虑第一个轮廓，即轮廓0。它是层次结构1。它有两个孔，轮廓1和2，它们属于层次2。因此对于轮廓0，相同层级的下一轮廓是轮廓-3。而没有以前的。而第一个是小孩是等级-2的轮廓1。它没有父级，因为它在层次结构1中。所以它的层次数组是[3，-1,1，-1]


现在轮廓-1。它在层次结构2中。下一个层次相同（轮廓-1的父母身份）是轮廓2。没有前一个。没有孩子，但父母是轮廓0。所以数组是[2，-1，-1,0]。


类似的轮廓-2：它在层次2。轮廓0下的相同层次结构中没有下一个轮廓。所以没有下一个以前是轮廓-1。没有孩子，父母是轮廓0。所以数组是[-1,1，-1,0]。


轮廓-3：层次结构-1中的下一个是轮廓-5。以前是轮廓0。孩子是轮廓4，没有父母。所以数组是[5,0,4，-1]。


轮廓-4：轮廓3下的等级2，没有兄弟。所以没有下一个，没有以前，没有孩子，父母是轮廓-3。所以数组是[-1，-1，-1,3]。


剩下你可以填写这是我得到的最后答案：


>>> hierarchyarray([[[ 3, -1,  1, -1],        [ 2, -1, -1,  0],        [-1,  1, -1,  0],        [ 5,  0,  4, -1],        [-1, -1, -1,  3],        [ 7,  3,  6, -1],        [-1, -1, -1,  5],        [ 8,  5, -1, -1],        [-1,  7, -1, -1]]])





### 4. RETR_TREE
而这是最后一个人，先生。 它检索所有轮廓并创建一个完整的家族层次结构列表。 甚至告诉你，谁是爷爷，父亲，儿子，孙子甚至超越... :)。

例如，我拍摄了上面的图像，重写cv2.RETR_TREE的代码，根据OpenCV给出的结果重新排列轮廓，并对其进行分析。 再次，红色字母给出轮廓数字，绿色字母给出层次顺序。



![CCOMP Hierarchy](http://opencv-python-tutroals.readthedocs.io/en/latest/_images/tree_hierarchy.png)

取轮廓0：处于层次结构0。 相同层次结构中的下一个轮廓是轮廓7。 没有以前的轮廓。 孩子是轮廓-1。 没有父母 所以数组是[7，-1,1，-1]。


轮廓2：它在层次结构1。 没有轮廓在同一水平。 没有前一个。 孩子是轮廓2。 父轮廓为0。 所以数组是[-1，-1,2,0]。


剩下的，尝试一下自己。 以下是完整的答案：

>>> hierarchyarray([[[ 7, -1,  1, -1],        [-1, -1,  2,  0],        [-1, -1,  3,  1],        [-1, -1,  4,  2],        [-1, -1,  5,  3],        [ 6, -1, -1,  4],        [-1,  5, -1,  4],        [ 8,  0, -1, -1],        [-1,  7, -1, -1]]])














