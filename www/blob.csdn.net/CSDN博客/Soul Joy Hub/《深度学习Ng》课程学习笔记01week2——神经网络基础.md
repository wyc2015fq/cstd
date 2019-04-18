# 《深度学习Ng》课程学习笔记01week2——神经网络基础 - Soul Joy Hub - CSDN博客

2017年09月05日 16:10:13[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：672
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://write.blog.csdn.net/mdeditor#!postId=77852727](http://write.blog.csdn.net/mdeditor#!postId=77852727)

# 2.1 二分分类

二分类例子： 

判断图片中是否有猫： 

将图片RGB矩阵拉伸为向量： 
![](http://upload-images.jianshu.io/upload_images/1621805-864eb636423e420b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-c00fe8bd9c4afa55.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

使用上面的特征向量来判断图片中是否有猫。

# 2.2 logistic 回归

![](http://upload-images.jianshu.io/upload_images/1621805-3abc7ded39504be7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-dfeefadeaf9f8c77.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-8efc601a2ef617d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.3 logistic 回归损失函数

这里给出的是交叉熵损失函数：

![](http://upload-images.jianshu.io/upload_images/1621805-acd9368ebf61dd1e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.4 梯度下降法

为了最小化代价函数，找到 w,b 的最优解 
![](http://upload-images.jianshu.io/upload_images/1621805-be3e5263f08b6cc2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

对 代价函数对 w，b 求导，再根据学习率，更新 w 和 b ：

![](http://upload-images.jianshu.io/upload_images/1621805-4d11fafb61f968e5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

2.5 到 2.8 是非常基础的导数讲解，这里就不做笔记了。

# 2.9 logistic 回归中的梯度下降法

![](http://upload-images.jianshu.io/upload_images/1621805-b32e6dde4ae20797.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.10 m 个样本的梯度下降

![](http://upload-images.jianshu.io/upload_images/1621805-f2bcd2e7227993a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

计算步骤为：

![](http://upload-images.jianshu.io/upload_images/1621805-fecfd4e23a4a0115.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.11 向量化

我们讲上述的计算步骤向量化：

![](http://upload-images.jianshu.io/upload_images/1621805-6ac540ead8971dd4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

向量化 和 循环的代码对比：

![](http://upload-images.jianshu.io/upload_images/1621805-77793cb825ba227e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们可以看到向量化比循环的代码快了300倍左右。

# 2.12 向量化的更多例子

![](http://upload-images.jianshu.io/upload_images/1621805-0911a0602aa89cf0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-f224c35b1954c8b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

回到我们梯度下降的步骤，将其改为向量化：

![](http://upload-images.jianshu.io/upload_images/1621805-99676b19531a886b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.13 向量化 logistic 回归

![](http://upload-images.jianshu.io/upload_images/1621805-227512361cb72b58.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.14 向量化 logistic 回归的梯度输出

进一步向量化 logistic 回归：

![](http://upload-images.jianshu.io/upload_images/1621805-e476b78a45b483c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

进一步向量化 梯度下降： 
![](http://upload-images.jianshu.io/upload_images/1621805-baffda42d4727bb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.15 Python 中的广播

例子： 

计算事物的能量成分比例：

![](http://upload-images.jianshu.io/upload_images/1621805-6f3837b9f9bee94f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

python 代码如下：

![](http://upload-images.jianshu.io/upload_images/1621805-15ad978a05a458fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

axis = 0，列相加；axis = 1，行相加。

其他例子：

![](http://upload-images.jianshu.io/upload_images/1621805-8bd75d08030388ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

总结：

![](http://upload-images.jianshu.io/upload_images/1621805-bd87874b3b4a6709.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.16 关于 python / numpy 向量的说明

![](http://upload-images.jianshu.io/upload_images/1621805-906acde7683f050b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-1fedd3bfe3784e1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![这里写图片描述](https://img-blog.csdn.net/20170905160939597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

