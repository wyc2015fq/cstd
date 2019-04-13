
# DECOUPLED WEIGHT DECAY REGULARIZATION - lpty的博客 - CSDN博客

2019年04月09日 17:20:59[lpty](https://me.csdn.net/sinat_33741547)阅读数：12标签：[Adam																](https://so.csdn.net/so/search/s.do?q=Adam&t=blog)[AdamW																](https://so.csdn.net/so/search/s.do?q=AdamW&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Adam&t=blog)个人分类：[论文阅读																](https://blog.csdn.net/sinat_33741547/article/category/7727913)



# 引言
Adam作为一个常用的深度学习优化方法，提出来的时候论文里的数据表现都非常好，但实际在使用中发现了不少问题，在许多数据集上表现都不如SGDM这类方法。
后续有许多工作针对Adam做了研究，之前整理过关于优化算法的发展历程：[从Stochastic Gradient Descent到Adaptive Moment Estimation](https://blog.csdn.net/sinat_33741547/article/details/87367996)，里面也介绍了一些对于Adam效果不好的研究工作。
这篇论文依旧以此作为研究对象，原文参考：[DECOUPLED WEIGHT DECAY REGULARIZATION](https://arxiv.org/pdf/1711.05101.pdf)。作者提出了一个简单, 但是很少有人注意的事实, 那就是现行的所有深度学习框架在处理Weight Decay的时候，都采用了L2正则的方法来做，但实际上，L2正则在自适应梯度方法中，如Adam，与Weight Decay并不等价。
# 算法
Weight Decay在优化算法中的意义，在于限制值比较大的权重，让整体模型的权重更加接近于零，这也是符合奥卡姆剃刀原理的，权重更小的模型被认为是更简单的模型，从而拥有更好的泛化性能。
对于SGD来说，L2正则与Weight Decay都可以用以下形式表达：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409164943813.PNG)
区别在于正则化操作在目标函数加上权重的L2范数，而Weight Decay则是在梯度更新位置直接减去部分权重，但是最终的表达形式是一致的，但是在Adam中存在自适应的学习率调整，假如依旧使用L2正则的方式实现Weight Decay，那么两个同样大小的权重，其中梯度较大的权重会下降得比另一个更小：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409165500845.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
由上，二阶动量V会随着更新量的增加而自适应调整，导致相应的学习率下降，从而导致权重衰减变小。学习率和权重衰减之间存在很大的相关性，作者针对解耦两者关系提出了解决方法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409165815508.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
方法也很简单，作者按照原始Weight Decay的定义，直接将权重衰减加到优化器之后，将其与学习率解耦，真正发挥了Weight Decay的作用。
[
](https://img-blog.csdnimg.cn/20190409165815508.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
# 结果
[
](https://img-blog.csdnimg.cn/20190409165815508.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)作者实验了多种形式的模型，效果均比原始版本要好，具体参考原论文：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
上面是对SGD和Adam的比较：
[
](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
> Figure 2 we compare the performance of L2 regularization vs decoupled weight decay in SGD (SGD vs. SGDW, top row) and in Adam(Adam vs. AdamW, bottom row).
[
](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)左边为原始的实现方法，可以看到，学习率和权重衰减存在极大的相关性，而右边则为论文提出的方法，最优化的区域明显变大了，在这种情况下能够更加简单的进行参数调整从而搜索到最优点。
[
](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
# 结论
[
](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)提出了新的优化算法AdamW，解耦了学习率和Weight Decay实际上对所有自适应的优化算法都适用。
[
](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
# 引用
[
](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)1、DECOUPLED WEIGHT DECAY REGULARIZATION
2、[https://blog.csdn.net/sinat_33741547/article/details/87367996](https://blog.csdn.net/sinat_33741547/article/details/87367996)
[
						](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
[
	](https://img-blog.csdnimg.cn/20190409171231518.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
