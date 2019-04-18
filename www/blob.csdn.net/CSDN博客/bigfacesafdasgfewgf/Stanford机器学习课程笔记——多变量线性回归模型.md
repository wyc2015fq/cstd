# Stanford机器学习课程笔记——多变量线性回归模型 - bigfacesafdasgfewgf - CSDN博客





2015年01月23日 14:32:17[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2837








**Stanford机器学习课程笔记——多变量线性回归模型**



# 1. 多变量线性回归模型引入




    前面一篇中学习了单变量的线性回归模型，那么自然就会想到多变量线性回归模型，以及非线性模型。这篇我们就学习。

    其实，前面的单变量，顾名思义就是样本的特征只有1个，那么多变量就是样本的特征有多个。同样是前面的房屋价格的例子，吴恩达大叔给出了多变量的例子，如下：

![](https://img-blog.csdn.net/20150123152204555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这个新例子中，每个样本的特征有4个（房屋面积，卧室个数，楼层，建筑年代），需要许褚的依旧是房屋的价格。

    多变量线性回归模型的假设和前面是类似的，只不过theta的个数随着变量个数增加而增加，为：![](https://img-blog.csdn.net/20150123152527293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。然后我们可以给theta_0配对一个x_0，这样整个形式就一样了。也就是：![](https://img-blog.csdn.net/20150123152428933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，其中我们可以令x_0=1，这样可以转换成常见的向量乘矩阵的形式。也就是：![](https://img-blog.csdn.net/20150123152654248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。其中的theta是行向量，里面都是线性回归模型中的参数，X是样本矩阵，每一列为一个样本（注意，这里和sklearn中的每一行为一个样本是不一样的）。




    有了假设，后面就是代价函数。多变量线性回归模型中的代价函数和单变量的相似，都是样本的误差平方和：

![](https://img-blog.csdn.net/20150123152835859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


只不过其中h_0_x_i中x_i的个数增多了。

    同样，我们也可以使用梯度下降法来求解上述目标函数。其粗略的算法步骤如下：

![](https://img-blog.csdn.net/20150123153027500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





其中关键指出在于假设h对所有表示系数theta_i的求导。其实也简单，因为h关于theta的表达式是线性的，那么求导自然就很方便啦，这里我们就直接贴出来吴恩达大叔的原稿：

![](https://img-blog.csdn.net/20150123153246184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这样就得到了更新所有theta时候的下降方向，不过要注意：所有theta_i的更新一定是同时的！




# 2. 特征缩放




    这个特征缩放，我理解类似与特征的归一化normalization. 这是样本多变量情况所特有的。因为样本有多个特征，而每个特征的量纲不完全一样，最大值最小值的取值范围也不同，所以我们需要把它们统一到-1到1之间。

    具体而言，对于其中一个特征，我们把所有样本在该特征维度上的值都提取出来，计算均值和标准差。然后利用下面公式归一化：

![](https://img-blog.csdn.net/20150123153812312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中mu_n是均值，S_n是标准差。这样所有样本在该特征维度上的值均在-1到1之间了。




# 3. 学习率alpha




    这是梯度下降法中的学习率，它决定了在找到下降最快的方向之后，到底下降多少的程度。这是一个度的关系。可以说，梯度下降法这样子万能的求解最优化的选手收到影响的因素就两个，除了初始点的设置之外，还有一个就是它了。


梯度下降法的求解效果收到了学习率大小的影响，如果alpha过小，那么收敛的速度会很慢，求解需要迭代的次数很多；如果alpha很大，那么可能使得更新的时候越过了局部最小值，导致无法收敛，求解失败。（不过这样子看来，还是设置小一些比较稳妥。。）




    画出代价函数值关于迭代次数的变化曲线，可以看出alpha设置的是否合理：

![](https://img-blog.csdn.net/20150123154434703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





而如果alpha设置的不合理，那么代价函数的值可能会逐渐增大，或者在动荡：

![](https://img-blog.csdn.net/20150123154718444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





    上面右图看的比较明显。那么这个学习率到底怎么设置呢？

     还是吴恩达大叔给出了建议：

![](https://img-blog.csdn.net/20150123154922121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





看出来了吧，先试一试小的alpha，0.001,如果感觉它太小了，那么再试一试0.01，如果太大了，返回来试一试0.003，这样子一次试下去。。。




# 4. 非线性回归——多项式回归




    开头我们提到了不是所有的数据都满足线性回归的，那么非线性回归又如何呢？这里我们学习一个简单的非线性回归模型，多项式回归，而且它还是可以转化成线性回归的。转换的思路如下：

![](https://img-blog.csdn.net/20150123155414107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





也就是说，我们可以把后面的高次项重新用一个新的变量来表示，新的变量只是一次的，这样我们整个模型都是一次的。接下来我们继续可以使用线性回归模型的方法求解。梯度下降法啊，代价函数啊，都是适用的。




# 5. 正规方程——解析解




    课程中说这个是正规方程，刚开始看还不是很理解，后面知道了，其实这里就是解析解。因为我们前面的梯度下降法求得的都是局部最优，不是全局的。这个是解析解，全局的。

    说是解析解，是因为回归模型中的代价函数都是可以求导，令导数为零，求得最优解的。那么回到我们刚才说的代价函数形式，我们把假设h的向量乘矩阵的形式![](https://img-blog.csdn.net/20150123152654248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)带入代价函数中，再求导，即可得：

![](https://img-blog.csdn.net/20150123160342899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





其中矩阵X是样本特征矩阵，这个是我们直接就有的，y也是直接有的。也就是说，我们可以把X和y带入上式，就可以一下子求得最优的theta向量了。

    但是，我们要注意计算时间复杂度。因为这个求解公司中有矩阵的逆运算，它的时间复杂度是O(n^3)，而且有的时候不是满秩，不可以求逆运算。所以这两个方法（解析解和梯度下降法）各有利弊，总结如下：

![](https://img-blog.csdn.net/20150123160622644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





其实，遇到不可求逆的情况，分析原因可知：这个因为矩阵不满秩，也就是说代表矩阵X行数的样本特征维数大于代表列数的样本个数，这就是传统的小样本问题。这个时候，我们可以先用PCA降维，把样本的特征维数降低，小于样本的个数，这样就可以了。










已经写到一半的博文突然就没有了，CSDN上面也没有自动保存，晕。。。吐槽一下这个编辑器。。




参考:

[http://blog.csdn.net/abcjennifer/article/details/7700772](http://blog.csdn.net/abcjennifer/article/details/7700772)





《Coursera机器学习笔记》



