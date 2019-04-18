# VC数字图像处理编程讲座之七 - 深之JohnChen的专栏 - CSDN博客

2005年12月28日 19:19:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3808标签：[图像处理																[编程																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


图像分割

图像分割是指通过某种方法，使得画面场景中的目标物被分为不同的类别。通常图像分割的实现方法是，将图像分为“黑”、“白”两类，这两类分别代表了两个不同的对象。因为结果图像为二值图像，所以通常又称图像分割为图像的二值化处理。

7．1二值图像法

图像分割原理上的计算公式如下：

![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangfenge1.jpg)

其中，f(i,j)为原始图像，g(i,j)为结果图像（二值），

Th为阈值。

7．2 p-参数法

•p-参数法对于实现已知目标物在画面中所占比例的情况下使用比较有效。

•p-参数法的基本设计思想是：对固定分辨率下的目标物，根据目标物在画面中所占的比例来选择阈值，进行二值化处理。

p-参数法原理

•如下图所示，假设目标物为暗，背景为亮；

•先试探性地给出一个阈值（黄色），统计目标物的像素点数在整幅图中所占的比例是否满足要求，是则阈值合适；否则，阈值则偏大（右）或者偏小（左），再进行调整，直到满足要求（白色）。

![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangfenge2.jpg)

p-参数法算法


1.计算得到原图的灰度直方图h；

2.输入目标物所占画面的比例p；

3.尝试性地给定一个阈值Th=Th0；

4.计算在Th下判定的目标物的像素点数N;
![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangchuli3.jpg)

5. 设图像的大小为m*n，判断ps=N/(m*n)是否接近p?

是，则输出结果，否则，Th=Th+dT; (if ps<p, 则dT>0;else dT<0)，转4，直到满足条件。

7.3均匀性度量法

![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangchuli4.jpg)

![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangchuli5.jpg)

![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangchuli6.jpg)

4。基于熵的阈值分割法。

![](http://blog.csdn.net/images/blog_csdn_net/byxdaz/167850/r_tuxiangchuli7.jpg)

这种方法的缺点是仅仅考虑了像素点的灰度信息，没有考虑到像素点的空间信息，所以当图像的信噪比降低时分割效果不理想。毫无疑问，像素点的灰度是最基本的特征，但它对噪声比较敏感，为此，在分割图像时可以再考虑图像的区域信息，区域灰度特征包含了图像的部分空间信息，且对噪声的敏感程度要低于点灰度特征。综合利用图像的这两个特征就产生了二维最大熵阈值分割方法。


