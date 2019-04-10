# Lucas–Kanade光流算法分析



作者：gnuhpc@gmail.com 



简介：在计算机视觉中，Lucas–Kanade光流算法是一种两帧差分的光流估计算法。它由Bruce D. Lucas 和 Takeo Kanade提出。

光流的概念:(Optical flow or optic flow)它是一种运动模式，这种运动模式指的是一个物体、表面、边缘在一个视角下由一个观察者（比如眼睛、摄像头等）和背景之间形成的明显移动。光流技术，如运动检测和图像分割，时间碰撞，运动补偿编码，三维立体视差，都是利用了这种边缘或表面运动的技术。

二维图像的移动相对于观察者而言是三维物体移动的在图像平面的投影。有序的图像可以估计出二维图像的瞬时图像速率或离散图像转移。

光流算法：它评估了两幅图像的之间的变形，它的基本假设是体素和图像像素守恒。它假设一个物体的颜色在前后两帧没有巨大而明显的变化。基于这个思路，我们可以得到图像约束方程。不同的光流算法解决了假定了不同附加条件的光流问题。

Lucas–Kanade算法：

这个算法是最常见，最流行的。它计算两帧在时间t 到t + δt之间每个每个像素点位置的移动。 				由于它是基于图像信号的泰勒级数，这种方法称为差分，这就是对于空间和时间坐标使用偏导数。图像约束方程可以写为I (x ,y ,z ,t ) = I (x + δx ,y + δy ,z + δz ,t + δt )I(x, y,z, t) 

为在（x,y,z）位置的体素。我们假设移动足够的小，那么对图像约束方程使用泰勒公式，我们可以得到：![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209368274.png) 				

H.O.T. 指更高阶，在移动足够小的情况下可以忽略。从这个方程中我们可以得到：
![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209366290.png)
或者
![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209374272.png)
我们得到：

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209394356.png)
*V* x ,*V* *y*,*V* *z*分别是I(x,y,z,t)的光流向量中x，y，z的组成。

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209462791.png), ![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209468298.png), ![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209461645.png)和 ![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209468265.png)则是图像在(*x* ,*y* ,*z* ,*t* )这一点向相应方向的差分。
所以 
$$
I_xV_x+I_yV_y+I_zV_z=−I_t
$$


写做： 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209471612.png) 

[这个方程有三个未知量，尚不能被解决，这也就是所谓光流算法的光圈问题。那么要找到光流向量则需要另一套解决的方案。而Lucas-Kanade算法是一个非迭代的算法： 

[假设流(Vx,Vy,Vz)在一个大小为m*m*m(m>1)的小窗中是一个常数，那么从像素1...*n* , *n* = *m* 				3 			中可以得到下列一组方程： 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209476039.png) 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/20121204220947989.png) 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209485101.png) 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209485624.png) 

[三个未知数但是有多于三个的方程，这个方程组自然是个超定方程，也就是说方程组内有冗余，方程组可以表示为： 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209482003.png) 

记作：![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209498000.png) 

为了解决这个超定问题，我们采用最小二乘法： 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209496888.png)or  

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209501772.png) 

得到： 

![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209519721.png) 

其中的求和是从1到n。 

这也就是说寻找光流可以通过在四维上图像导数的分别累加得出。我们还需要一个权重函数*W(i, j,k)* 			， 				![img](https://images.cnblogs.com/cnblogs_com/gnuhpc/201212/201212042209512753.png)来突出窗口中心点的坐标。高斯函数做这项工作是非常合适的， 

这个算法的不足在于它不能产生一个密度很高的流向量，例如在运动的边缘和黑大的同质区域中的微小移动方面流信息会很快的褪去。它的优点在于有噪声存在的鲁棒性还是可以的。 

补充：opencv里实现的看上去蛮复杂，现在还不是太明白。其中LK经典算法也是迭代法，是由高斯迭代法解线性方程组进行迭代的。 

参考文献：Lucas  B and Kanade T. An Iterative Image Registration Technique with an  Application to Stereo Vision. Proc. Of 7th International Joint  Conference on Artificial Intelligence (IJCAI), pp.674-679.是81年发表的。 

opencv中实现的是84年发表的算法， 

参考文献：[Bruce D. Lucas  , "Generalized Image Matching by the Method of Differences," doctoral  dissertation, tech. report , Robotics Institute, Carnegie Mellon  University, July, 1984  ](http://www.ri.cmu.edu/person.html?person_id=1802)

这篇论文是收费的，一直都没下到还。