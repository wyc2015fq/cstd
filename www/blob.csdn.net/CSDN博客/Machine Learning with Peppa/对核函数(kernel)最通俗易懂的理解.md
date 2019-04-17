# 对核函数(kernel)最通俗易懂的理解 - Machine Learning with Peppa - CSDN博客





2018年06月07日 10:23:25[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：6611








机器学习算法中，不论是感知机还是支持向量机，在面对非线性问题时，往往都会用到一个名为“核函数”的技巧。那么到底什么是核函数呢？是否真的如听起来这样难以理解呢？

![](https://images2015.cnblogs.com/blog/1085343/201704/1085343-20170426122634069-471713021.png)




核函数：是映射关系![](https://images2015.cnblogs.com/blog/1085343/201704/1085343-20170426122751256-1859948756.png) 的内积，映射函数本身仅仅是一种映射关系，并没有增加维度的特性，不过可以利用核函数的特性，构造可以增加维度的核函数，这通常是我们希望的。

二维映射到三维，区分就更容易了，这是聚类、分类常用核函数的原因。为什么PCA这样一个降维算法也用核函数呢？

![](https://images2015.cnblogs.com/blog/1085343/201704/1085343-20170426124308412-900490941.png)


左图为原数据，右图为映射到三维的数据，可以看出：同样是降到1维，先通过Kernel映射到（Kernel是映射的内积，不要弄乱了）三维，再投影到1维，就容易分离开，这就是Kernel在PCA降维中的应用，本质还是对原有数据增加维度。

下面是李航的《统计学习方法》中对于核函数的定义：

![](https://img-blog.csdn.net/20180607102131391?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


要注意，**核函数和映射没有关系。**核函数只是用来计算映射到高维空间之后的内积的一种简便方法。



一般英文文献对Kernel有两种提法，一是Kernel Function，二是Kernel Trick。从Trick一词中就可以看出，这只是一种运算技巧而已，不涉及什么高深莫测的东西。

具体巧在哪里呢？我们如果想进行原本就线性不可分的数据集进行分割，那么选项一是容忍错误分类，即引入Soft Margin；选项二是我们可以对Input Space做Feature Expansion，把数据集映射到高维中去，形成了Feature Space。我们**几乎可以认为**（引用Caltech的课堂用语“**We are safe but not certain**”）原本在低维中线性不可分的数据集在足够高的维度中存在线性可分的超平面。








举一个知乎上霍华德的例子。假设我们的任务是要预测那些微博可以上微博热搜榜。有两个离散特征，一个代表某个微博里有“鹿晗”，一个代表某个微博里有“关晓彤”。

两个特征单独看热度都一般，此时我们用二阶多项式核方法：

![K(鹿晗,关晓彤) = <鹿晗,关晓彤>^2](https://www.zhihu.com/equation?tex=K%28%E9%B9%BF%E6%99%97%2C%E5%85%B3%E6%99%93%E5%BD%A4%29+%3D+%3C%E9%B9%BF%E6%99%97%2C%E5%85%B3%E6%99%93%E5%BD%A4%3E%5E2)

这个核函数可以把二维空间投射到三维空间，展开之后是：


![K(鹿晗,关晓彤) =(鹿晗^2,\sqrt{2}鹿晗\times 关晓彤,关晓彤^2)](https://www.zhihu.com/equation?tex=K%28%E9%B9%BF%E6%99%97%2C%E5%85%B3%E6%99%93%E5%BD%A4%29+%3D%28%E9%B9%BF%E6%99%97%5E2%2C%5Csqrt%7B2%7D%E9%B9%BF%E6%99%97%5Ctimes+%E5%85%B3%E6%99%93%E5%BD%A4%2C%E5%85%B3%E6%99%93%E5%BD%A4%5E2%29)

这样就把二维特征变成了三维，多了一维 ![鹿晗\times 关晓彤](https://www.zhihu.com/equation?tex=%E9%B9%BF%E6%99%97%5Ctimes+%E5%85%B3%E6%99%93%E5%BD%A4) ，代表着某条微博里鹿晗和关晓彤同时出现。

结果大家都知道了，鹿晗关晓彤同时出现的那条微博超级火，把新浪服务器都挤爆了。

![](https://pic1.zhimg.com/50/v2-1fcc114a7b178b34ea00c0e4a83655f3_hd.jpg)![](https://pic1.zhimg.com/80/v2-1fcc114a7b178b34ea00c0e4a83655f3_hd.jpg)![](https://pic1.zhimg.com/50/v2-e908c0f6e9bcaad3ae3d0b6a23a6696c_hd.jpg)![](https://pic1.zhimg.com/80/v2-e908c0f6e9bcaad3ae3d0b6a23a6696c_hd.jpg)

现实生活中有很多非线性非常强的特征 而核方法能够捕捉它们。核技巧(kernel trick)的作用，一句话概括的话，就是降低计算的复杂度，甚至把不可能的计算变为可能。



在机器学习中常用的核函数，一般有这么几类，也就是LibSVM中自带的这几类：





1) 线性：![K(v_1,v_2)=<v_1,v_2>](https://www.zhihu.com/equation?tex=K%28v_1%2Cv_2%29%3D%3Cv_1%2Cv_2%3E)
2) 多项式：![K(v_1,v_2)=(\gamma<v_1,v_2>+c)^n](https://www.zhihu.com/equation?tex=K%28v_1%2Cv_2%29%3D%28%5Cgamma%3Cv_1%2Cv_2%3E%2Bc%29%5En)
3) Radial basis function：![K(v_1,v_2)=\exp(-\gamma||v_1-v_2||^2)](https://www.zhihu.com/equation?tex=K%28v_1%2Cv_2%29%3D%5Cexp%28-%5Cgamma%7C%7Cv_1-v_2%7C%7C%5E2%29)
4) Sigmoid：![K(v_1,v_2)=\tanh(\gamma<v_1,v_2>+c)](https://www.zhihu.com/equation?tex=K%28v_1%2Cv_2%29%3D%5Ctanh%28%5Cgamma%3Cv_1%2Cv_2%3E%2Bc%29)

我举的例子是多项式核函数中![\gamma=1, c=0, n=2](https://www.zhihu.com/equation?tex=%5Cgamma%3D1%2C+c%3D0%2C+n%3D2)的情况。

在实用中，很多使用者都是盲目地试验各种核函数，并扫描其中的参数，选择效果最好的。至于什么样的核函数适用于什么样的问题，大多数人都不懂。很不幸，我也属于这大多数人，所以如果有人对这个问题有理论性的理解，还请指教。




参考文章：

[知乎：核函数的定义与作用](https://www.zhihu.com/question/24627666)






