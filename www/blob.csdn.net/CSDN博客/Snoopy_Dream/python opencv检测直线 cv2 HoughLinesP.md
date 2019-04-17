# python opencv检测直线 cv2.HoughLinesP - Snoopy_Dream - CSDN博客





2018年09月17日 22:10:22[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1010








opencv的HoughLinesP函数是统计概率霍夫线变换函数，该函数能输出检测到的直线的端点 ![(x_{0}, y_{0}, x_{1}, y_{1})](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/20eb30037d99342a5887bea2a086e1b39e78904a.png)，其函数原型为：HoughLinesP(image, rho, theta, threshold[, lines[, minLineLength[, maxLineGap]]]) -> lines

**image**参数表示边缘检测的输出图像，该图像为单通道8位二进制图像。

**rho**参数表示参数极径 ![r](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/b55ca7a0aa88ab7d58f4fc035317fdac39b17861.png) 以像素值为单位的分辨率，这里一般使用 1 像素。

**theta**参数表示参数极角 ![\theta](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/52e8ed7a3ba22130ad3984eb2cd413406475a689.png) 以弧度为单位的分辨率，这里使用 1度。

**threshold**参数表示**检测一条直线所需最少的曲线交点**。

**lines**参数表示储存着检测到的直线的参数对 ![(x_{start}, y_{start}, x_{end}, y_{end})](http://www.opencv.org.cn/opencvdoc/2.3.2/html/_images/math/baee84de274f174bbcc7d67e86f0dea7b49a0af9.png) 的容器，也就是线段两个端点的坐标。

**minLineLength**参数表示能组成一条直线的最少点的数量，点数量不足的直线将被抛弃。

**maxLineGap**参数表示能**被认为在一条直线上的亮点的最大距离**。







lines = cv2.HoughLinesP(edges,1,np.pi/360,**10**,minLineLength=30,maxLineGap=5)

**10和**maxLineGap控制斜线检测的程度



![](https://img-blog.csdn.net/20180917220735920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

lines = cv2.HoughLinesP(edges,1,np.pi/360**,30,**minLineLength=30,maxLineGap=5)

![](https://img-blog.csdn.net/20180917220809229?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

lines = cv2.HoughLinesP(edges,1,np.pi/360**,30,**minLineLength=30,maxLineGap=**50**)

![](https://img-blog.csdn.net/20180917221132389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

lines = cv2.HoughLinesP(edges,***0.01***,np.pi/360**,30,**minLineLength=30,maxLineGap=**50**)

![](https://img-blog.csdn.net/20180917221228374?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

lines = cv2.HoughLinesP(edges,***1***,np.pi/***3600*,30,**minLineLength=30,maxLineGap=**50**)

![](https://img-blog.csdn.net/20180917221342628?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

lines = cv2.HoughLinesP(edges,1,np.pi/360**,10,**minLineLength=30,maxLineGap=**50**)

![](https://img-blog.csdn.net/2018091722105818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)















