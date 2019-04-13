
# 雅可比（Jacobian）矩阵 - 郭云飞的专栏 - CSDN博客


2017年10月13日 18:47:09[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：6640


在向量分析中, 雅可比矩阵是**一阶偏导数**以**一定方式排列**成的**矩阵**。其行列式称为雅可比行列式。还有, 在代数几何中, 代数曲线的雅可比量表示雅可比簇：伴随该曲线的一个代数群, 曲线可以嵌入其中. 它们全部都以数学家卡尔·雅可比(Carl Jacob, 1804年10月4日－1851年2月18日)命名。
**一、Jacobian矩阵**
雅可比矩阵的重要性在于它体现了一个可微方程与给出点的最优线性逼近. 因此, 雅可比矩阵类似于多元函数的导数。
假设*F: Rn→Rm*是一个从欧式n维空间转换到欧式m维空间的函数。这个函数**F**由m个实函数组成:*y1(x1,…,xn), …, ym(x1,…,xn)*。这些函数的偏导数(如果存在)可以组成一个m行n列的矩阵, 这就是所谓的雅可比矩阵：
![](https://img-blog.csdn.net/20171013185145287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
表示为:
![](https://img-blog.csdn.net/20171013185256237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果p是Rn中的一点, F在p点可微分, 那么在这一点的导数由*J**F(p)*给出(这是求该点导数最简便的方法). 在此情况下, 由F(p)描述的线性算子即接近点p的F的最优线性逼近, x逼近于p:
![](https://img-blog.csdn.net/20171013185546656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**二、举例**
由球坐标系（Spherical coordinate system）到直角坐标系的转化由F函数给出︰
![](https://img-blog.csdn.net/20171013190057706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此坐标变换的雅可比矩阵是：
![](https://img-blog.csdn.net/20171013190211961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



