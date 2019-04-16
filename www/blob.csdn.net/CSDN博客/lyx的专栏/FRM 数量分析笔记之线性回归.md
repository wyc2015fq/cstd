# FRM 数量分析笔记之线性回归 - lyx的专栏 - CSDN博客





2016年08月19日 10:40:40[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1747








        线性回归可能大家都会觉得很熟悉了，玩过机器学习的人还会觉得这个low low的，其实，线性回归在数理统计的角度下，还是有很多值得考察的地方的。

## 1、一元线性回归

        线性回归，自然就想到最小二乘法了。线性回归说白了就是确定斜率和截距，他们的计算方法如下所示。

![](https://img-blog.csdn.net/20160818173502895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        斜率的确定很有意思，协方差除以独立变量的方差。所以，协方差、独立变量方差、斜率，知道两个就可以获得第三个；结合之前的相关系数，那么，可以互推的情况就很多了。

        评价一个线性回归拟合的好不好，直观上有残差平方和，SSR。在数理统计的框架下，评价的指标更加丰富。

        TSS是不考虑回归，数据自身与均值比较的平方和，ESS则是回归方程预测后的数据与均值的平方和，也叫解释平方和。含义就是，原始数据中的波动性被解释的部分。SSR则是没有没解释的部分。那么显然，没有被解释的部分越小越好，所以有了R^2指标，这个指标在SSR越小的情况下变大。


![](https://img-blog.csdn.net/20160818175931045?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




        R^2指标的实际含义就是你的回归曲线能够解释多少的原始波动，而且神奇的事情是，他是X与Y相关系数的平方。

       然后我们引入一个叫做自由度的东西。n个抽样出来的样本，我们认为他是n-1个自由度，这就是为什么我们看到对总体无偏估计计算方差的时候，分母不是n，而是n-1。我们可以这么理解，在已知期望的基础上，我们实际是只有n-1个自由度的，因为根据期望和n-1个样本就可以知道第n个样本的值。在线性回归中也是这样，ESS，被解释部分平方和的自由度是independent变量的个数，在单元线性回归中，就是1个自由度，剩下的n-2个自由度就是属于SSR残差平方和的。而这一平方和除以自由度，就是SER，回归的标准误差。这又是一个衡量线性回归曲线的拟合度的指标。

        完成线性回归的计算后，我们又有一个问题了，我们选择的这个X真的和Y有关吗？有关的话，置信度如何呢？

        根据之前说的假设检验的方法，我们可以有如下null假设和备择假设：
![](https://img-blog.csdn.net/20160819102341340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        接下来就是假设检验的那一套了。这里用到的方差就是斜率的方差，frm考试中往往会直接告知。

        同样的，也可以对截距项做假设检验。

## 2、多元线性回归

        上面说的都是一元回归，如果是多元回归呢?其实是一样的。可以一个一个分开检验斜率项，也可以联合检验，那么这个时候假设检验的假设就是下面这样的了:

![](https://img-blog.csdn.net/20160819102840262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


         当然，这时候的分布就不再是正态分布或者t-分布了，而是F-分布，相应的也使用F-统计量。

        所谓统计量，就是指一个分布中假设检验的假设值距离均值标准差的倍数。

![](https://img-blog.csdn.net/20160819103110312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        这就是联合检验时候计算F-统计量的公式。

        在多元性性回归中，还有一个重要的改变，就是R^2的变化。在理论上，只要加入的变量越多，R^2的数值就会越大，毕竟加入的变量越多，能解释的部分就越多，但是显然这是不合适的。引入的变量过多会有过拟合、运算量大、多重共线性等等的问题，所以我们在计算R^2这个指标的时候，要进行改进：

![](https://img-blog.csdn.net/20160819103556565?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        这就是调整后指标的计算方法，其实还是很好理解的，就是吧SSR和TSS用各自的自由度均值化了，这样做的目的就是可以对K的数量做出惩罚，k越大，那么SSR被平均的越少。






