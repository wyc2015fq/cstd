# FRM 数量分析笔记之概率分布 - lyx的专栏 - CSDN博客





2016年08月18日 16:19:39[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1948








      对于一个随机变量最完整的描述就是概率分布函数了。

## 1、切比雪夫不等式

      首先我们提出一个切比雪夫不等式：

![](https://img-blog.csdn.net/20160818154733129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      这是什么意思的？对于任何一个概率分布，注意，是任何一个哦，某一个数值落在K倍标准差的概率大于1-1/k^2。是不是很神奇，因为它对一切概率分布都满足。那么特殊的概率分布有什么特点呢。这里我们就来讨论几个常用的概率分布。

## 2、伯努利与二项分布

      首先是最简单的抛硬币，也就是伯努利和二项分布，太easy了，就不说了。

特点大致如下：





![](https://img-blog.csdn.net/20160818155057489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 3、泊松分布

      接下来，由二项分布引入一个泊松分布。泊松分布就是当n很大，p很小的时候，来估计二项分布的数值的一个分布。

![](https://img-blog.csdn.net/20160818155317187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      n很大，p很小，那么n乘以p往往就会是一个比较好的数字了，这个数字就是泊松分布中的参数。而k的含义与二项分布中k的含义一样，就是出现多少次的概率。对于泊松分布这个形式，有一个特别有趣的记忆方法。兰姆达背着k，走在大街上，突然，兰姆达额（e）的一声，拿刀（-）自杀了，k掉了下来，很惊讶（！）。

      泊松分布有一个很优美的性质，就是他的期望是兰姆达，方差也是兰姆达，也就是说，他的方差和期望都是那个根据np算出来的参数。

## 4、均匀分布

      接下来是很简单的均匀分布，就不多讲了，均值和方差如下：





![](https://img-blog.csdn.net/20160818155804383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 5、正态分布

      接下来登场的是主角，正态分布了。这个分布的重要性就不要多说了，大多数分布，取个极限什么的，最后都会变成正态分布。正态分布由两个参数决定，一个是期望，也就是均值，一个是方差。


![](https://img-blog.csdn.net/20160818155903695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      正态分布的偏度是0，很显然，正态分布式左右对称的，峰度是3，这在之前有说过。所以如果某一个分布的峰度是4，那么超过3的那部分，也就是1，我们成为超额峰度。

![](https://img-blog.csdn.net/20160818160104479?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      最后，在FRM考试中，我们要记住正态分布很重要的三个分位数，分别对应90%，95%，99%的正态分布取值概率。

![](https://img-blog.csdn.net/20160818160341041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        当然，这里都是双尾的，如果是单尾，那么就有1.65对应95%，1.96对应97.5%

## 6、t-分布

      有一个和正态分布很像的分布，t-分布。为什么要有这个分布，这就得在后面说了。我们只要把他当做正态分布的扁平修正体就可以了，用法和正态分布一模一样，形状也差不多，只是没有那么尖，而且尾肥。

## 7、lognormal分布

      接下来是一个最容易弄混的lognormal分布。这个分布式正态分布其指数获得的，换句话说，lognormal分布的变量取对数之后就是正态分布。我们可以这么认识lognormal分布这个名字：**

*                   取log之后就normal*


![](https://img-blog.csdn.net/20160818161100075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 8、卡方分布和F-分布

      接下来是卡方分布和F分布，都把他们当做正态分布一样用就可以了，而且只要他们一趋向于无穷，本身就是正态分布了。

      卡方分布是正态分布的平方，F-分布式两个正态分布的平方相除，也就是两个卡方分布相除。如此而已，no big deal，具体的在应用的时候再介绍。







