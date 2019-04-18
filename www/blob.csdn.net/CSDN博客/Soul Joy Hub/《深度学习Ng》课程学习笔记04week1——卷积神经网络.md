# 《深度学习Ng》课程学习笔记04week1——卷积神经网络 - Soul Joy Hub - CSDN博客

2018年01月14日 15:48:55[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：235标签：[cnn](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)
个人分类：[深度学习																[计算机视觉](https://blog.csdn.net/u011239443/article/category/8257568)](https://blog.csdn.net/u011239443/article/category/6990149)

所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)


[http://blog.csdn.net/u011239443/article/details/79057016](http://blog.csdn.net/u011239443/article/details/79057016)

# 1.1 计算机视觉

## 计算机视觉领域的问题
- 图片分类
- 目标检测
- 图片风格转化

![](http://upload-images.jianshu.io/upload_images/1621805-82f467449c07d389.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 深度学习在图像中的应用

![](http://upload-images.jianshu.io/upload_images/1621805-f29704cf5b804be0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

过多的权重参数矩阵让计算、内存消耗使得传统神经网络不能接受。

# 1.2 边缘检测示例

## 过滤器示例

![](http://upload-images.jianshu.io/upload_images/1621805-4215d9e80c66a657.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-f666935c39ad03ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-439777f33b1eebd3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

该过滤器为竖直边缘过滤器。为什么叫做**竖直边缘**呢？我们再用该过滤器举个例子：

![](http://upload-images.jianshu.io/upload_images/1621805-58406f074f4b9d96.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

通过过滤器，能过滤检测出竖直边缘。

# 1.3 更多边缘检测内容

## 水平检测过滤器

![](http://upload-images.jianshu.io/upload_images/1621805-a95be1f5fd44e29d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 其他过滤器

![](http://upload-images.jianshu.io/upload_images/1621805-312255e5a8eba030.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这些过滤器的中间权值比较大，使得中心点附近的像素更具有代表性，这样会有更好的鲁棒性。

## 深度学习中的过滤器

![](http://upload-images.jianshu.io/upload_images/1621805-ec7421474f0370e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在CNN中，我们会把过滤器中的各个权值作为参数来训练。这样使得过滤器不仅仅能检测竖直或者水平的边缘特征，而且可以检测到角度旋转后的复杂边缘。

#1.4 Padding

过滤器有个缺点就是会把矩阵的大小减少，使用  Padding，将图像周边都填充0，就可以解决这个问题：
![](http://upload-images.jianshu.io/upload_images/1621805-8998715c44310634.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

#1.5 卷积步长

过滤器每次移动可以不一定的一步，我们来看下步长为2的情况：
![](http://upload-images.jianshu.io/upload_images/1621805-43d348a017181408.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

过滤后矩阵边长的公式为：
![](http://upload-images.jianshu.io/upload_images/1621805-0b3d71409296afe8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- n：原矩阵边长
- p：padding 填充边长
- f：过滤器边长
- s :过滤器移动步长

# 1.6 卷积中“卷”的体现之处 & 1.7 单层卷积网络

结合例子整体的讲解下一次卷积的过程。如我们有 6 *  6 * 3（3是RGB，这里称作信道） 的图片，那么我们的过滤器会是 f * f * 3，过滤器的信道数要和原来图片相同。过滤器的每个信道在原图与之对应的信道上进行过滤，这里就会生成 4 * 4 * 3 的图（图中没有画出来），生成图各个信道的图的对应位置相加，然后加上偏置项b，讲累加的结果传入激活函数，就得到了的 4 * 4 的图。不同过滤器得到的图作为某个信道的图堆积成最后的结果，也就是说 有 n 个过滤器，那么过滤得到的图就有 n 个信道。

![](http://upload-images.jianshu.io/upload_images/1621805-1628545eff866305.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

可以看到，我们训练的是过滤器的权重矩阵W，无论原图变得再大，我们需要训练的矩阵W大小是不变的。

# 1.8 简单卷积网络示例

![](http://upload-images.jianshu.io/upload_images/1621805-506776e5c8b6d17e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.9 池化层

![](http://upload-images.jianshu.io/upload_images/1621805-12fd7dc708950e48.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-cf8722ab95ea7121.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.10 卷积神经网络示例（LeNet-5）

![](http://upload-images.jianshu.io/upload_images/1621805-54a5b84545eacd08.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-23b66cb6df60d6db.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-e7dd694099f756b7?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

