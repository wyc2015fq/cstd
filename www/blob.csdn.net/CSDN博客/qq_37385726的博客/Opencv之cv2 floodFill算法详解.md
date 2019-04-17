# Opencv之cv2.floodFill算法详解 - qq_37385726的博客 - CSDN博客





2018年09月02日 10:43:09[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1714
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)









# **目录**

[一、 泛洪算法——floodFill函数原型](#%E4%B8%80%E3%80%81%20%E6%B3%9B%E6%B4%AA%E7%AE%97%E6%B3%95%E2%80%94%E2%80%94floodFill%E5%87%BD%E6%95%B0%E5%8E%9F%E5%9E%8B)

[二、简单应用](#%E4%BA%8C%E3%80%81%E7%AE%80%E5%8D%95%E5%BA%94%E7%94%A8)

[三、应用，结合minareaRect](#%E4%B8%89%E3%80%81%E5%BA%94%E7%94%A8%EF%BC%8C%E7%BB%93%E5%90%88minareaRect)





## 一、 泛洪算法——floodFill函数原型

cv2.floodFill(img,mask,seed,newvalue(BGR),(loDiff1,loDiff2,loDiff3),(upDiff1,upDiff2,upDiff3),flag)
- img：为待使用泛洪算法的图像
- mask：为掩码层，使用掩码可以规定是在哪个区域使用该算法，如果是对于完整图像都要使用，则掩码层大小为原图行数+2，列数+2.是一个二维的0矩阵，边缘一圈会在使用算法是置为1。而只有对于掩码层上对应为0的位置才能泛洪，所以掩码层初始化为0矩阵。【**dtype:np.uint8**】
- seed：为泛洪算法的种子点，也是根据该点的像素判断决定和其相近颜色的像素点，是否被泛洪处理。

        【类似于![](https://img-blog.csdn.net/20180902095601314?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)】
- newvalue：是对于泛洪区域新赋的值（B,G,R）
- (loDiff1,loDiff2,loDiff3)：是相对于seed种子点像素可以往下的像素值，即seed(B0,G0,R0)，泛洪区域下界为（B0-loDiff1,G0-loDiff2,R0-loDiff3）
- (upDiff1,upDiff2,upDiff3)：是相对于seed种子点像素可以往上的像素值，即seed(B0,G0,R0)，泛洪区域上界为（B0+upDiff1,G0+upDiff2,R0+upDiff3）
- flag：为泛洪算法的处理模式。

![](https://images2017.cnblogs.com/blog/1199089/201709/1199089-20170927161402715-1385689536.png)
- **低八位 **控制算法的连通性，是以seed点为中心，接着判断周围的几个像素点，再将泛洪区域像素点周围的几个像素点进行考虑。 一般为4，8；默认为4
- **中间八位** 与掩码层赋值密切相关，一般使用（255<<8）使中间8位全位1，则值为255，也就是掩码层对应原图的泛洪区域的部分被由原来的初值0赋值成255，如果中间8位为0，则赋值为1.
- **高八位 **由opencv宏参数指定
-   cv2.FLOODFILL_FIXED_RANGE：改变图像，填充newvalue
-   cv2.FLOODFILL_MASK_ONLY：不改变原图像，也就是newvalue参数失去作用，而是改变对应区域的掩码，设为中间八位的值



![](https://images2017.cnblogs.com/blog/1199089/201709/1199089-20170927160313184-1997523552.png)

![](https://images2017.cnblogs.com/blog/1199089/201709/1199089-20170927160813950-993749996.png)



## 二、简单应用

```python
#泛洪填充(彩色图像填充)
import cv2
import numpy as np
def fill_color_demo(image):
    copyImg = image.copy()
    h, w = image.shape[:2]
    mask = np.zeros([h+2, w+2],np.uint8)   #mask必须行和列都加2，且必须为uint8单通道阵列
    #为什么要加2可以这么理解：当从0行0列开始泛洪填充扫描时，mask多出来的2可以保证扫描的边界上的像素都会被处理
    cv.floodFill(copyImg, mask, (220, 250), (0, 255, 255), (100, 100, 100), (50, 50 ,50), cv.FLOODFILL_FIXED_RANGE)
    cv.imshow("fill_color_demo", copyImg)

src = cv.imread('E:/imageload/baboon.jpg')
cv.namedWindow('input_image', cv.WINDOW_AUTOSIZE)
cv.imshow('input_image', src)
fill_color_demo(src)
cv.waitKey(0)
cv.destroyAllWindows()
```

![](https://images2018.cnblogs.com/blog/1327126/201805/1327126-20180515132932003-536213723.png)

![](https://images2018.cnblogs.com/blog/1327126/201805/1327126-20180515132953751-698221371.png)





## 三、应用，结合minareaRect

```python
cv2.floodFill(initial_car,mask,(seed_x,seed_y),(255,0,0),(loDiff,loDiff,loDiff),(upDiff,upDiff,upDiff),flag)

points = []
row,column = mask.shape

for i in range(row):
    for j in range(column):
        if mask[i][j]==255:
           points.append((j,i))   #点应该输入点坐标（列，行）
points = np.asarray(points)
new_rect = cv2.minAreaRect(points)
```













