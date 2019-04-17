# 人脸检测（八）--HOG特征原理及实现 - 工作笔记 - CSDN博客





2016年05月30日 08:46:11[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8267
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

原文：

[http://blog.csdn.net/icvpr/article/details/8454527](http://blog.csdn.net/icvpr/article/details/8454527)



1）HOG特征的原始文献

"Histograms of Oriented Gradients for Human Detection" 

"Finding People in Images and Videos" (PhD Thesis) （较为详细）



2） HOG特征算子的网络参考资料

[http://www.cnblogs.com/tornadomeet/archive/2012/08/15/2640754.html](http://www.cnblogs.com/tornadomeet/archive/2012/08/15/2640754.html)

[http://blog.csdn.net/carson2005/article/details/7841443](http://blog.csdn.net/carson2005/article/details/7841443)#

[http://blog.csdn.net/abcjennifer/article/details/7365651](http://blog.csdn.net/abcjennifer/article/details/7365651)

[http://blog.csdn.net/zouxy09/article/details/7929348](http://blog.csdn.net/zouxy09/article/details/7929348)#







HOG特征描述算子——原理、思路、步骤

-----------------------------------------------------------------------------------

1. HOG特征描述子的定义

HOG Descriptor: locally normalised histogram of gradient orientation in dense overlapping grids，即局部归一化的梯度方向直方图。




2. HOG特征的基本思想

Histogram of Oriented Gradient descriptors provide a dense overlapping description of image regions，即统计图像局部区域的梯度方向信息来作为该局部图像区域的表征。

HOG有点类似于SIFT特征描述子，区别： 


1）HOG没有选取主方向，也没有旋转梯度方向直方图，因而本身不具有旋转不变性（较大的方向变化），其rotate不变性是通过采用不同旋转方向的训练样本来实现的； 


2）HOG本身不具有scale不变性，其scale不变性是通过改变检测图像的size来实现的；


3）HOG是在dense采样的图像块中求取的，在计算得到的HOG特征向量中隐含了该块与检测窗口之间的空间位子关系，而SIFT特征向量是在一些独立并离散分布的特征点上提取的（dense SIFT除外）。




3. HOG特征的假设和出发点

The hypothesis is that local object appearance and shape can often be characterised rather well by the distribution of local intensity gradient or edge directions, even without precise knowledge

of the corresponding gradient or edge positions. 即，在图像中，物体的局部外观和形状能够通过其局部梯度或边缘信息很好地表征和描述。





3. HOG特征描述向量的提取与计算步骤



HOG特征提取的流程图如下图所示：

![](https://img-my.csdn.net/uploads/201212/31/1356924004_7530.jpg)



1）全局图像归一化

目的：减少光照的影响

方法：gamma compression 

a） 对每个颜色通道分别，计算平方根，或者 

b） 对每个颜色通道分别，求log



2）计算图像梯度

目的：通过梯度信息来描述图像中物体的边缘、轮廓、形状等纹理信息

方法：对每个颜色通道分别计算梯度。梯度算子：水平边缘算子： [-1, 0, 1] ；垂直边缘算子： [-1, 0, 1]T 。

最后选择三个通道中具有最大模值norm的梯度vector作为该像素的gradient vector。



3）统计局部图像梯度信息

目的：统计局部图像梯度信息并进行量化（或称为编码），得到局部图像区域的特征描述向量。该特征描述算向量既能够描述局部图像的内容，又能够对该图像区域内的pose或外观的小变化具有不变性。

方法：求取梯度方向直方图

a）将image window划分为多个区域“cell”；

b）为每个“cell”计算一个1-D的加权梯度方向直方图；

其中，直方图包含9个bin，划分区间：0°-180°或0°-360°。

其中，加权采用三线性插值方法，即将当前像素的梯度方向大小、像素在cell中的x坐标与y坐标这三个值来作为插值权重，而被用来插入的值为像素的梯度幅值。

采用三线性插值的好处在于：避免了梯度方向直方图在cell边界和梯度方向量化的bin边界处的突然变化。



4）归一化

目的：对每个block得到的histogram进行归一化后，能够够对光照、阴影、边缘对比度等具有更好的不变性、

方法：

1）将多个临近的cell组合成一个block块，然后求其梯度方向直方图向量；

2）采用L2-Norm with Hysteresis threshold方式进行归一化，即将直方图向量中bin值的最大值限制为0.2以下，然后再重新归一化一次；

注意：block之间的是“共享”的，也即是说，一个cell会被多个block“共享”。另外，每个“cell”在被归一化时都是“block”independent的，也就是说每个cell在其所属的block中都会被归一化一次，得到一个vector。



5）生成特征描述向量

即将所有“block”的HOG descriptors组合在一起，形成最终的feature vector，该feature vector就描述了detect window的图像内容。





4. HOG描述算子的优点：



1）orientation histogram

能够有效地描述图像区域的local shape的特征信息

2）采用“cell”方式进行梯度方向量化，使得特征描述算子具有一些（a small amount of）平移或旋转不变性

通过改变histogram的bin个数，以及“cell”的size，能够控制捕获图像局部区域特征信息的精度和保持特征具有不变性

3）具有光照不变性

Gamma normalisation and local contrast normalisation(局部对比度归一化) contribute another key component: illumination invariance.

4）overlapping blocks

The use of overlapping of blocks provides alternative normalisations so that the classifier can choose the most relevant one.



5. 影响HOG性能的几个因素：

finescale gradients, fine orientation binning, relatively coarse spatial binning, and high-quality local contrast normalisation in overlapping descriptor blocks are all important for good performance.





6.  解释

1）为什么使用orientation histogram？

capture local shape information

2）为什么使用“cell”？

achieve a small amount of spatial invariance

3）为什么使用“overlapping blocks”？

在众多的local contrast normalisation方法中，采用overlapping blocks得到的效果最好。







7. HOG代码

OpenCV中包含了HOG特征的提取和描述类cv::HOGDescriptor。通过该类可以提取指定图像区域的HOG特征。



