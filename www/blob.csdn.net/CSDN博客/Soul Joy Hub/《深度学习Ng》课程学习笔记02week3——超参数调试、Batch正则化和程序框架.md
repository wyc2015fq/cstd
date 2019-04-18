# 《深度学习Ng》课程学习笔记02week3——超参数调试、Batch正则化和程序框架 - Soul Joy Hub - CSDN博客

2017年09月25日 19:33:24[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：790
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/78088602](http://blog.csdn.net/u011239443/article/details/78088602)

# 3.1 调试处理
- 参数：

![](http://upload-images.jianshu.io/upload_images/1621805-b28b14401845f55d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 不要使用格子点调参，而应该使用随机点调参。因为格子点对于单一粒度 
![](http://upload-images.jianshu.io/upload_images/1621805-18611a9ca353b95c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 调参时，我们可以先调参选到几个结果较优的点（如图被蓝色圈住的） 
![](http://upload-images.jianshu.io/upload_images/1621805-d9a0c66937b0e78d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.2 为超参数选择合适的范围

对于 alpha 的取值：

![](http://upload-images.jianshu.io/upload_images/1621805-f4c8d887a4208418.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们不应该如上述的方法取值，而应先划分（如下），再取值：

![](http://upload-images.jianshu.io/upload_images/1621805-e944171d17a7d7b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-aadcc074b51472a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.3 超参数训练的实践：Pandas VS Caviar

![](http://upload-images.jianshu.io/upload_images/1621805-17f7a1adbde24ac1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.4 正则化网络的激活函数

输入的0均值标准化：

![](http://upload-images.jianshu.io/upload_images/1621805-d34baf75b0f8ea77.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

隐藏层的0均值标准化：

![](http://upload-images.jianshu.io/upload_images/1621805-f8a5d8e0e478a89b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-6ae167436930c15c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.5 将 Batch Norm 拟合进神经网络

![](http://upload-images.jianshu.io/upload_images/1621805-74e9e14b7a540c03.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 使用微批：

![](http://upload-images.jianshu.io/upload_images/1621805-7ff1ee4ee728e29d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 梯度下降：

![](http://upload-images.jianshu.io/upload_images/1621805-df142716735d5240.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.6 Batch Norm 为什么奏效？

当训练集中X发布改变的时候，需要重新训练模型： 
![](http://upload-images.jianshu.io/upload_images/1621805-de47b6e9bc9bc308.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们吧某隐藏层之后的神经网络看成一个模型，可知就算是同分布的数据X从整个网络正向传播，由于W、b的变化，到了该隐藏层，输出分布也都会不同。所以Batch Norm 奏效就是由于避免的这种分布的不同而造成的问题： 
![](http://upload-images.jianshu.io/upload_images/1621805-2dfa7635d61d6766.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.7 测试时的 Batch Norm

![](http://upload-images.jianshu.io/upload_images/1621805-421dbcb6f99ffabf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.8 Softmax 回归

最终预测的各个类别的概率之和不一定等于1： 
![](http://upload-images.jianshu.io/upload_images/1621805-fb05be8cdee4cc75.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-b6a887484ee4c4a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.9 训练一个 Softmax 分类器

![](http://upload-images.jianshu.io/upload_images/1621805-4909b74e636b7085.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 损失函数： 
![](http://upload-images.jianshu.io/upload_images/1621805-fcb3bcad6f66164c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 梯度下降：

![](http://upload-images.jianshu.io/upload_images/1621805-aa57b8cff38b8893.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.10 深度学习框架

![](http://upload-images.jianshu.io/upload_images/1621805-8b4b5db1ed85b088.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.11 TensorFlow

参阅：[TensorFlow实战——入门](http://blog.csdn.net/u011239443/article/details/70224863)
![](https://img-blog.csdn.net/20170925193350430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

