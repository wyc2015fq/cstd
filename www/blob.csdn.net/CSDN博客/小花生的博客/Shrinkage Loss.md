
# Shrinkage Loss - 小花生的博客 - CSDN博客


2019年03月18日 20:47:46[Peanut_范](https://me.csdn.net/u013841196)阅读数：58



## Shrinkage Loss：
**论文：**《Deep Regression Tracking with Shrinkage Loss》
**问题：**Data Imbalance：常用的解决数据不平衡问题的方法是数据重采样和损失敏感函数。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318203805647.png)
观察上图可以看出，对于输入的一个patch，输出的响应图P和软标签Y中，背景信息的差别其实并不大，但是由于这种简单的背景信息（负样本）较多，导致对于整张图的损失函数来说，简单负样本的贡献较大，这就是文中所说的数据不平衡问题。
[
](https://img-blog.csdnimg.cn/20190318203805647.png)1.传统的回归问题可视为下式所示的优化问题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318203832663.png)
其中*代表卷积操作，W代表卷积的权重，Y是使用高斯函数生成的样本的软标签。将其写成损失函数的形式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318203859270.png)
2.本文使用P表示每次迭代得到的响应图，p代表响应图中的一个位置的值（区间[0,1]中的一个值），y代表该位置的标签（1或者0）。
让l为估计可能性p与其对应的软标签y之间的绝对差：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318203920394.png)
平方损失可以表示为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318203940223.png)
文献Focal Loss中指出，在损失函数中增加一个调节项可以减轻数据不平衡的问题。构造出如下所示的损失函数：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318203957956.png)
为了简便，文中使用γ=1的情况举例。focal loss就变成了L3 loss。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318204020168.png)
为啥上式可以减轻数据不平衡问题？
比如现在有两组样例，分别为简单样本（数量多）和困难样本（数量少），使用损失时，前者的损失是0.09，后者的损失是0.64。若使用L3损失，前者的损失是0.027，后者的损失是0.512。可以算出0.64/0.09大约为7.1，0.512/0.027约为19。这样，从比例上看，使用损失时，简单负样本的贡献程度变得较小。
**作者的意思是，使用**
$$
L^{_{F}}
$$
**损失，既导致简单样本的损失变小了，但同时也使困难样本的损失变小了，这样不好。于是，作者提出了一个函数来代替**
$$
l^{\gamma }
$$
，形如下式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318204230139.png)
式中a和c分别是超参数，而这个函数绘制出来如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318204251131.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
上图可以看出，当$l&gt;0.5$的时候，函数的输出接近1，而$l&lt;0.5$的时候，函数的输出小于1。这样就可以只对简单样本添加惩罚，使其损失变小，而对困难样本不造成影响。于是，损失函数就变成了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019031820440853.png)
使用其重写损失函数形式为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190318204426542.png)
文中设置a=10, c=0.2。
作者说，这个损失函数可以提高模型效果，加快收敛速度。
参考：
[https://blog.csdn.net/ms961516792/article/details/82656542](https://blog.csdn.net/ms961516792/article/details/82656542)
[https://blog.csdn.net/donkey_1993/article/details/83151618](https://blog.csdn.net/donkey_1993/article/details/83151618)
---

###### 注：博众家之所长，集群英之荟萃。

