# 《深度学习Ng》课程学习笔记03week1——机器学习（ML）策略（1） - Soul Joy Hub - CSDN博客

2017年09月27日 20:58:36[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：502标签：[深度学习																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
个人分类：[机器学习																[深度学习](https://blog.csdn.net/u011239443/article/category/6990149)](https://blog.csdn.net/u011239443/article/category/6268728)

所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)


[http://blog.csdn.net/u011239443/article/details/78117155](http://blog.csdn.net/u011239443/article/details/78117155)

# 1.1 为什么是 ML 策略

各种各样的机器学习策略。如何选择、使用？ 
![](http://upload-images.jianshu.io/upload_images/1621805-c3f9d9a982c97e92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.2 正交化

调参时，应将两个参数进行正交，降低操作的复杂： 
![](http://upload-images.jianshu.io/upload_images/1621805-80711cb1b66086a4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-d91d801f759a7a9e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.3 单一数字评估指标

![](http://upload-images.jianshu.io/upload_images/1621805-80c7cc0465fb3a8f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

用平均值来代替多个值： 
![](http://upload-images.jianshu.io/upload_images/1621805-5e1362d761031e62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.4 满足和优化指标

准确率和运行时间之间的协调： 
![](http://upload-images.jianshu.io/upload_images/1621805-b3499494abf37010.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.5 训练 / 开发 / 测试集划分

![](http://upload-images.jianshu.io/upload_images/1621805-4a309a5b266f2855.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.6 开发集合测试集的大小

传统开发集合测试集的大小： 
![](http://upload-images.jianshu.io/upload_images/1621805-529e5c0195ff682a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

深度学习下，数据量很大，1%的开发集 和 1%的测试集数据已足够。而且，深度学习来说，训练集规模需求比较大：

![](http://upload-images.jianshu.io/upload_images/1621805-70b17db81de7895d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.7 什么时候该改变开发 / 测试集和指标

对不同的数据记录设置不同的代价权重： 
![](http://upload-images.jianshu.io/upload_images/1621805-57c95def9d5b884d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-858a448dfcb9c9a6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.8 为什么是人的表现

![](http://upload-images.jianshu.io/upload_images/1621805-67db2bab0c901668.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-84bf05e044774f18.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.9 可避免偏差

![](http://upload-images.jianshu.io/upload_images/1621805-df30a79e3c22c1ba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.10 理解人的表现

我们用人类变现的误差来估算贝叶斯误差，但是我们的训练误差越接近人类误差，用人类误差越无法准确的用于估计贝叶斯误差： 
![](http://upload-images.jianshu.io/upload_images/1621805-8c295676f17d5ef8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.11 超过人的表现

当训练误差小于人类误差，就很难去判断是误差还是偏差的问题： 
![](http://upload-images.jianshu.io/upload_images/1621805-302e33d9b50d55d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.12 改善你的模型的表现

![](http://upload-images.jianshu.io/upload_images/1621805-fb954d7d28cb45fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a423ade3d96be3ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-e7dd694099f756b7?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

