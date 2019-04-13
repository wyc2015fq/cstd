
# HOG特征 - 郭云飞的专栏 - CSDN博客


2017年09月22日 11:49:06[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：114个人分类：[机器学习																](https://blog.csdn.net/guoyunfei20/article/category/7178819)


一、介绍
HOG（*Histogram of Oriented Gridients*），是目前计算机视觉、模式识别领域很常用的一种描述图像局部纹理的特征。
HOG特征描述子的定义：
locally normalised histogram of gradient orientation in dense overlapping grids，即局部归一化的梯度方向直方图，是一种对图像局部重叠区域的密集型描述符, 它通过计算局部区域的梯度方向直方图来构成特征。
HOG的本质：
Histogram of Oriented Gradient descriptors provide a dense overlapping description of image regions，即统计图像局部区域的梯度方向信息来作为该局部图像区域的表征。
OpenCV中的HOG算法来源：
[Histograms of Oriented Gradients for Human Detection](http://lear.inrialpes.fr/pubs/2005/DT05/hog_cvpr2005.pdf)，CVPR 2005。
**二、 图像分块（patch）**
简单来说就是将图像分割成若干小块
![](https://img-blog.csdn.net/20170922115539993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**三、 计算每个块的方向梯度直方图**
利用任意一种梯度算子，例如：sobel，laplacian等，对该patch进行卷积，计算得到每个像素点处的梯度方向和幅值。想看公式的如下：
![](https://img-blog.csdn.net/20170922115819024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将360度（2*PI）根据需要分割成若干个bin，例如：分割成12个bin，每个bin包含30度，整个直方图包含12维，即12个bin。然后根据每个像素点的梯度方向，利用双线性内插法将其幅值累加到直方图中。
**四、组成特征**
将从每个patch中提取出的“小”HOG特征首尾相连，组合成一个大的一维向量，这就是最终的图像特征。可以将这个特征送到分类器中训练了。例如：有4*4=16个patch，每个patch提取12维的小HOG，那么最终特征的长度就是：16*12=192维。






















