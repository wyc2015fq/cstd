# OpenCV曲线拟合与圆拟合-流浪的鱼-51CTO博客
## OpenCV曲线拟合与圆拟合
使用OpenCV做图像处理与分析的时候，经常会遇到需要进行曲线拟合与圆拟合的场景，很多OpenCV开发者对此却是一筹莫展，其实OpenCV中是有现成的函数来实现圆拟合与直线拟合的，而且还会告诉你拟合的圆的半径是多少，简直是超级方便，另外一个常用到的场景就是曲线拟合，常见的是基于多项式拟合，可以根据设定的多项式幂次生成多项式方程，然后根据方程进行一系列的点生成，形成完整的曲线，这个车道线检测，轮廓曲线拟合等场景下特别有用。下面就通过两个简单的例子来分别学习一下曲线拟合与圆拟合的应用。
### 一：曲线拟合与应用
基于Numpy包的polyfit函数实现，其支持的三个参数分别是x点集合、y点集合，以及多项式的幂次。得到多项式方程以后，就可以完整拟合曲线，图中有如下四个点：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/f813a96821fabc34feba9bcb487461b8.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
调用polyfit生成的二阶多项式如下：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/d94a9558d47f502ab016fc46336629e6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
拟合结果如下：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/b02b8b693620f8a52ccd112eacbc6d17.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
使用三阶多项式拟合，调用polyfit生成的多项式方程如下：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/d132b4a21ea8d8e472ad1086798a5e41.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
生成的拟合曲线如下：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/4dd35bd5b00ac0a80984402903b76bf4.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
使用polyfit进行曲线拟合时候需要注意的是，多项式的幂次最大是数据点数目N - 1幂次多项式，比如有4个点，最多生成3阶多项式拟合。上述演示的完整代码实现如下：
```
def circle_fitness_demo():
    image = np.zeros((400, 400, 3), dtype=np.uint8)
    x = np.array([30, 50, 100, 120])
    y = np.array([100, 150, 240, 200])
    for i in range(len(x)):
        cv.circle(image, (x[i], y[i]), 3, (255, 0, 0), -1, 8, 0)
    cv.imwrite("D:/curve.png", image)
    poly = np.poly1d(np.polyfit(x, y, 3))
    print(poly)
    for t in range(30, 250, 1):
        y_ = np.int(poly(t))
        cv.circle(image, (t, y_), 1, (0, 0, 255), 1, 8, 0)
    cv.imshow("fit curve", image)
    cv.imwrite("D:/fitcurve.png", image)
```
### 二：圆拟合与应用
圆的拟合是基于轮廓发现的结果，对发现的近似圆的轮廓，通过圆拟合可以得到比较好的显示效果，轮廓发现与拟合的API分别为findContours与fitEllipse，
有图像如下：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/eea0225c5b6184d0f1890c631a18f5ef.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
使用轮廓发现与圆拟合处理结果如下：
![OpenCV曲线拟合与圆拟合](https://s1.51cto.com/images/blog/201806/12/a6f47fdb46a982058022b0e3f768d31d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
红色表示拟合的圆，蓝色是圆的中心位置
上述完整的演示代码如下：
```
def circle_fitness_demo():
    src = cv.imread("D:/javaopencv/c2.png")
    cv.imshow("input", src)
    src = cv.GaussianBlur(src, (3, 3), 0)
    gray = cv.cvtColor(src, cv.COLOR_BGR2GRAY)
    ret, binary = cv.threshold(gray, 0, 255, cv.THRESH_BINARY | cv.THRESH_OTSU)
    cv.imshow("binary", binary)
    image, contours, hierachy = cv.findContours(binary, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    for i in range(len(contours)):
        rrt = cv.fitEllipse(contours[i])
        cv.ellipse(src, rrt, (0, 0, 255), 2, cv.LINE_AA)
        x, y = rrt[0]
        cv.circle(src, (np.int(x), np.int(y)), 4, (255, 0, 0), -1, 8, 0)
    cv.imshow("fit circle", src)
    cv.imwrite("D:/fitcircle.png", src)
```
学习OpenCV Python 系列课程
[OpenCV Python零基础入门视频教程](http://edu.51cto.com/course/13680.html)
[OpenCV Python图像处理进阶视频教程](http://edu.51cto.com/course/13789.html)
**吾心信其可行，则移山填海之难，终有成功之日；吾心信其不可行，则反掌折枝之易，亦无收效之期也**
