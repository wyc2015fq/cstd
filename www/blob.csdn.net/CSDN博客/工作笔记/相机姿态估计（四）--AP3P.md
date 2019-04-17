# 相机姿态估计（四）--AP3P - 工作笔记 - CSDN博客





2018年08月28日 10:54:29[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5603
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









AP3P

论文：An Efficient Algebraic Solution to the Perspective-Three-Point Problem

从数学的角度，提出了经典P3P算法的更快，更鲁棒，更准确的求解算法。根据已知3对点，可以计算相机位姿和参数。

问题定义：

给定特征fi i = 1, 2, 3在参考坐标系中的位置![\textup{G}pi](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctextup%7BG%7Dpi),以及特征点在相机坐标系中的方向测量向量![\textup{C}bi](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctextup%7BC%7Dbi)，目标是估计相机的旋转矩阵和位置![\boldsymbol{G}pc](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Cboldsymbol%7BG%7Dpc)。其中C表示相机坐标系，G表示 参考坐标系。

![](https://img-blog.csdn.net/20180828094013597?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

根据几何关系，我们得出：

![](https://img-blog.csdn.net/20180828094442137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中di表示参考坐标系中相机到特征点的欧式距离，两两相减，并投影到对应面的法向量：

![](https://img-blog.csdn.net/20180828095039896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了计算旋转矩阵中的角度，我们定义如下因子分解：

![](https://img-blog.csdn.net/20180828095457622?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中： 

![](https://img-blog.csdn.net/20180828095513611?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

将式子5代替到公式2中，得到标量方程：（![\theta 2](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctheta%202)未知）

![](https://img-blog.csdn.net/20180828095527189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后利用罗德里格斯变换:

![](https://img-blog.csdn.net/20180828100247364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了求其他两个角度，定义如下因子分解：

![](https://img-blog.csdn.net/20180828102616471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

利用旋转矩阵特性：

![](https://img-blog.csdn.net/20180828102641255?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

化简式10：

![](https://img-blog.csdn.net/20180828102701200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

式13表述的几何关系下图所示：

![](https://img-blog.csdn.net/2018082810280310?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接着，再次采用罗德里格斯变换得到：

![](https://img-blog.csdn.net/20180828102947746?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

展开并重排上式：

![](https://img-blog.csdn.net/20180828103120984?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 其中：

![](https://img-blog.csdn.net/20180828103206931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

出现3次，由此，我们重写式12：

![](https://img-blog.csdn.net/20180828103256940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



 为了化简等价于16式的18式，我们寻找一个![\phi](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Cphi)，使得下式成立：

![](https://img-blog.csdn.net/20180828103831665?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

推出：

![](https://img-blog.csdn.net/20180828103900374?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后，根据19、8式得到：

![](https://img-blog.csdn.net/20180828103932854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接着，利用14式，展开18式，得到一个16式的等价结果：

![](https://img-blog.csdn.net/20180828104032999?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 在24式中，代替17式为0，并重排结果：

![](https://img-blog.csdn.net/20180828104248952?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

带入具体的i = 1，2到式25，得到：

![](https://img-blog.csdn.net/20180828104517720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因为：

![](https://img-blog.csdn.net/20180828104534236?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

继续化简式26，同时 引入新参数![\theta 3{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctheta%203%7B%7D%27)：

![](https://img-blog.csdn.net/20180828104630722?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后用![\theta 3{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctheta%203%7B%7D%27)代替![\theta 3](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctheta%203)：

![](https://img-blog.csdn.net/20180828104718278?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180828104828452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 根据28式，得到：

![](https://img-blog.csdn.net/20180828104900577?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

计算两边的归一化结果：

![](https://img-blog.csdn.net/2018082810495047?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

得到一个关于![\theta 1{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B80%7D%20%5Ctheta%201%7B%7D%27)的4阶多项式.更加简洁的表达如下：

![](https://img-blog.csdn.net/20180828105120767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

剩下的就是如何计算![\theta 1{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Ctheta%201%7B%7D%27)的问题了，作者采用，逼近的方式找到式38的近似解，一旦找到式38的4个解，便采用牛顿方法进行优化，以最小代价提高精度。

另外，要注意的是：

对于每个cos，可能会得到2个sin值，如下：

![](https://img-blog.csdn.net/20180828110739420?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这会导致计算出2个旋转矩阵， 因此，借用di>0,可以排除一个无效的。下一步，根据每对（cos![\theta 1{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Ctheta%201%7B%7D%27) ，sin![\theta 1{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Ctheta%201%7B%7D%27) ），计算（cos![\theta 3{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Ctheta%203%7B%7D%27) ，sin![\theta 3{}'](https://private.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Ctheta%203%7B%7D%27) ），利用式37：

![](https://img-blog.csdn.net/20180828111135646?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后，根据19、27，5，我们找到相机位置，也可以利用5，12，18式，更快速得到：

![](https://img-blog.csdn.net/20180828111323618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 我们定义如下旋转矩阵：

![](https://img-blog.csdn.net/20180828111733380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因此：

![](https://img-blog.csdn.net/20180828111754847?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 将式54带入53，得：

![](https://img-blog.csdn.net/20180828111824249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接着，计算相机位置：

![](https://img-blog.csdn.net/20180828111917657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里，为了速度，只计算了一个点i=3,如果更在意精度，可以用最小二乘拟合i=1,2,3点的结果。

最后，AP3P已经在OpenCV3中实现了。 





