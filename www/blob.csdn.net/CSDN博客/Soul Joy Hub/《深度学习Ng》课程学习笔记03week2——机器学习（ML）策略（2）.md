# 《深度学习Ng》课程学习笔记03week2——机器学习（ML）策略（2） - Soul Joy Hub - CSDN博客

2017年09月29日 12:33:45[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：467
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/78132697](http://blog.csdn.net/u011239443/article/details/78132697)

# 2.1 进行误差分析

标注错误： 
![](http://upload-images.jianshu.io/upload_images/1621805-666df32a109ea16e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.2 清楚标注错误的数据

![](http://upload-images.jianshu.io/upload_images/1621805-64ebd2f0f087f247.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

纠正错误 dev / test 数据集的方法： 
![](http://upload-images.jianshu.io/upload_images/1621805-8817d43597937978.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.3 快速搭建你的第一个系统，并进行迭代

尽快的搭建你的第一个系统。 
![](http://upload-images.jianshu.io/upload_images/1621805-718944a64d84a356.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.4 在不同的划分上进行训练并测试

对于不同来源的数据，最佳方案可能并不是将其混合。如，我们最终需要预测的数据来自于app，那么 dev / test 数据集应该也要来自于 app。option 2 会是更好的选择： 
![](http://upload-images.jianshu.io/upload_images/1621805-1de52e26af525de8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.5 不匹配数据划分的偏差和方差

对于三个不同的问题（不匹配数据或者说数据不是相同分布，偏差和方差），我们有不同的策略。为了判断是那种问题，我将 train 和 dev 的合并称为 training-dev，根据人类误差和不同数据集下的误差来判断是什么问题： 
![](http://upload-images.jianshu.io/upload_images/1621805-fc8030be9b1d96c4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

总结来说：

![](http://upload-images.jianshu.io/upload_images/1621805-6ce16eb15033df5b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-0e16d1e71f651ab5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.6 定位数据不匹配

![image.png](http://upload-images.jianshu.io/upload_images/1621805-5136a588a4fdb8fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

如：

![](http://upload-images.jianshu.io/upload_images/1621805-80c264b3114d6dcf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.7 迁移学习

![](http://upload-images.jianshu.io/upload_images/1621805-6882dd2bb10b618b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

总结： 
![](http://upload-images.jianshu.io/upload_images/1621805-021221667490c379.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.8 多任务学习

![](http://upload-images.jianshu.io/upload_images/1621805-79f782cea7dc45bc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

当label向量中的某项y不存在，则不计算该项的交叉熵损失。

多任务学习的适用情况：

![](http://upload-images.jianshu.io/upload_images/1621805-43c0eaeac27441b1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

左边的是迁移学习的数据情况，右边的多任务学习的。

![](http://upload-images.jianshu.io/upload_images/1621805-1075fa2bcc17e4b1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.9 什么是端到端的深度学习

例如： 
![](http://upload-images.jianshu.io/upload_images/1621805-77612f47aff61ba4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.10 是否要使用端到端的深度学习

端到端的深度学习的优缺点： 
![](http://upload-images.jianshu.io/upload_images/1621805-d30b96ec8f0a09ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

使用端到端的深度学习需要注意的地方： 
![](http://upload-images.jianshu.io/upload_images/1621805-d9ba0c35b42c2817.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-e7dd694099f756b7?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

