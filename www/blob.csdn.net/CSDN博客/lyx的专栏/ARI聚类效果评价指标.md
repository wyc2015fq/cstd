# ARI聚类效果评价指标 - lyx的专栏 - CSDN博客





2016年09月27日 15:03:09[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：8677标签：[数据挖掘																[聚类																[ARI](https://so.csdn.net/so/search/s.do?q=ARI&t=blog)
个人分类：[机器学习](https://blog.csdn.net/qtlyx/article/category/5925605)





聚类效果有一个评价指标，ARI（Adjusted Rand Index）。这个指标不考虑你使用的聚类方法，把你的方法当做一个黑箱，只注重结果。可以说，是一个十分“功利”的指标。

1.Rand index

在讲ARI之前呢，先讲述一下RI，也就是rand index，从两者的名字也可以看出来，这是ARI的祖宗版。

![](https://img-blog.csdn.net/20160927110355759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里，我们解释一下a,b,c,d分别代表什么。a呢就是说应该在一类，你最后聚类到一类的数量，b呢就是不应该在一类 ，你最后聚类结果也没把他们聚类在一起的数量。c和d那么就是应该在一起而被分开的和不应该在一起而被迫住在一起的。毕竟强扭的瓜不甜，c和d固然是错误的。所以从R的表达式中可以看出，我们只认为a和b是对的，这样能够保证R在0到1之间，而且，聚类越准确，指标越接近于1.

这里有一个关键性的问题，就是什么叫数量？你怎么去计算？准确的说，是配对的数量。比如说a是应该在一起而真的幸福的在一起了的数量，这显然就应该像人类一样按照小夫妻数量计算，但是我们的样本可不管一夫一妻制，任意选两个就是一个配对，所以，就是n(n-1)/2这样来计算，也就是组合数，n个当中选两个的选法。同时我们看到，分母其实是所有配对的总和，所以，我们最后可以写成这样：

![](https://img-blog.csdn.net/20160927111050418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2.ARI

有了先前RI的感性理解之后，我们再来讲一下ARI。

RI有一个缺点，就是惩罚力度不够，换句话说，大家普遍得分比较高，没什么区分度，遍地80分。这样的话，往往是考试的制度不合适，于是就诞生出了ARI，这个指标相对于RI就很有区分度了。

![](https://img-blog.csdn.net/20160927111555030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

具体的公式就是下面这样的。

![](https://img-blog.csdn.net/20160927111657562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们来分析一下，![](https://img-blog.csdn.net/20160927111641437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) nij代表的是聚类之后再i类，应该在j类的样本数量，很显然，这一求和，就是RI中的a,应该在一起而真的在一起的数量。

![](https://img-blog.csdn.net/20160927111957900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)是如果你的聚类是完全对的，那么就应该是这个数目，所以在表达式里面叫做max index。

后面这一个部分![](https://img-blog.csdn.net/20160927112339921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)就比较难理解了，他是a的期望，也就是

![](https://img-blog.csdn.net/20160927112533176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这一个部分最难理解。

![](https://img-blog.csdn.net/20160927112924741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

假设配对矩阵是这样的，显然，我们共有n(n-1)/2个配对方法。我们在行方向计算出可能取到的配对书，在列方向计算可能取到的配对数，相乘以后，除以总的配对数。这就是a的期望了。

这里呢，这个期望就像是一个benchmark，用来衡量距离用的。](https://so.csdn.net/so/search/s.do?q=聚类&t=blog)](https://so.csdn.net/so/search/s.do?q=数据挖掘&t=blog)




