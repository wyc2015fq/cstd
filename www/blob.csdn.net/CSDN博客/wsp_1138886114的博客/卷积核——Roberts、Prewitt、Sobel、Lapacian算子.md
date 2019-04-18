# 卷积核——Roberts、Prewitt、Sobel、Lapacian算子 - wsp_1138886114的博客 - CSDN博客





2018年08月03日 11:48:58[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：898
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)










- - - [一、算子推导过程](#一算子推导过程)- - [1.1 梯度和Roberts算子：](#11-梯度和roberts算子)
- [1.2  Prewitt：](#12-prewitt)
- [1.3 Sobel算子](#13-sobel算子)
- [1.4 Lapacian算子：](#14-lapacian算子)








**图像处理卷积核——算子**


在对图像的操作，我们采用模板对原图像进行卷积运算，从而达到我们想要的效果。而获取一幅图像的梯度就转化为：模板（Roberts、Prewitt、Sobel、Lapacian算子）对原图像进行卷积。 

### 一、算子推导过程

知识引入：  

在一维连续数集上有函数f(x),我们可以通过求导获得该函数在任一点的斜率，根据导数的定义有：  


$f'(x) = f(x+\Delta x) - f(x)$

在二维连续数集上有函数f(x,y),我们也可以通过求导获得该函数在x和y分量的偏导数，根据定义有： 


$\frac{\partial f(x,y)}{\partial x}   f(x+\Delta x,y) - f(x,y) $


$\frac{\partial f(x,y)}{\partial y}   f(x,y+\Delta y) - f(x,y)$

##### 1.1 梯度和Roberts算子：

对于图像来说，是一个二维的离散型数集，通过推广二维连续型求函数偏导的方法，来求得图像的偏导数，即在(x,y)处的最大变化率，也就是这里的梯度： 


$g_x = \frac{\partial f(x,y)}{\partial x}   f(x+1,y) - f(x,y) $



$g_y = \frac{\partial f(x,y)}{\partial y}   f(x,y+1) - f(x,y)$

梯度是一个矢量，则(x,y)处的梯度表示为：$ ∇f \equiv grad(f) \equiv[g_x,g_y]^T = \begin{bmatrix}  \frac{\partial f}{\partial x},\frac{\partial f}{\partial y}  \end{bmatrix}^T$

其大小为：$M(x,y) = mag(∇f)=\sqrt{g_x^2+g_y^2}$
![这里写图片描述](https://img-blog.csdn.net/20180802222843479?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2*2大小的模板在概念上很简单， 但是他们对于用关于中心点对称的模板来计算边缘方向不是很有用，  

其最小模板大小为3*3。3*3模板考虑了中心点对段数据的性质，并携带有关于边缘方向的更多信息。  
##### 1.2  Prewitt：

![这里写图片描述](https://img-blog.csdn.net/20180802223629332?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.3 Sobel算子

Sobel算子是在Prewitt算子的基础上改进的，在中心系数上使用一个权值2，相比较Prewitt算子，Sobel模板能够较好的抑制（平滑）噪声。  
![这里写图片描述](https://img-blog.csdn.net/20180802224006931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上述所有算子都是通过求一阶导数来计算梯度的，用于线的检测，在图像处理中，通常用于边缘检测。在图像处理过程中，除了检测线，有时候也需要检测特殊点，这就需要用二阶导数进行检测。 
##### 1.4 Lapacian算子：

![这里写图片描述](https://img-blog.csdn.net/20180802224619264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

模板中心位置的数字是-8而不是-4，是因为要使这些系数之和为0，当遇到恒定湖对区域时，模板响应应将0。(如下图1) 

用lapacian算子图像进行卷积运算时，当响应的绝对值超过指定阈值时，那么该点就是被检测出来的孤立点，具体输出如下： (如下图2) 
![这里写图片描述](https://img-blog.csdn.net/20180802224914363?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






