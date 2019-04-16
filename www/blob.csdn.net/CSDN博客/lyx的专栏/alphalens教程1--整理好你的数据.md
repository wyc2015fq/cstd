# alphalens教程1--整理好你的数据 - lyx的专栏 - CSDN博客





2017年06月25日 17:48:03[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：6547








很久以前研究过这个，周末下大雨，整理一下子IDE里面的工程文件，发现了当时的测试demo，于是决定再来感受一下。

alphalens是quantopian下的三大quant利器这里，剩下两个是大名鼎鼎的zipline和pyfolio。alphalens是用于因子回测的，使用很方便，但是，最大的一个特点就是，函数的名称真是长啊！

安装就不说了，似乎pip就可以了。

万事开头难，中间也难，结尾更难。很多事确实是这样。

alphalens第一个难点就是把要测试的因子相关的数据整理成alphalens需要的那样。我们从alphalens的一个数据标准化函数说起。alphalens.get_clean_factor_and_forward_returns

函数定义如下：


```python
def get_clean_factor_and_forward_returns(factor,
                                         prices,
                                         groupby=None,
                                         by_group=False,
                                         quantiles=5,
                                         bins=None,
                                         periods=(1, 5, 10),
                                         filter_zscore=20,
                                         groupby_labels=None):
```

我们来解释一下参数：

**factor : pd.Series - MultiIndex**

一个MultiIndex Series类型的数据，index分别是日期与资产名称，值是当天的alpha值。

![](https://img-blog.csdn.net/20170624175900201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**prices : pd.DataFrame**

通常是一个列数很多的dataframe的数据结构，如下图所示，列名是股票代码，index是日期。
![](https://img-blog.csdn.net/20170624175925086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**groupby : pd.Series - MultiIndex or dict**

groupby是一个MultiIndex 的series或者一个dict，通常用dict更加直观，而且这两者也是可以相互转换的。dict的key是股票的代码，值是所属行业的分类名词，不一定是数字也可以是别的。
![](https://img-blog.csdn.net/20170624175946639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


还有一点，如果传入的是dict则默认测试周期中，行业的属性不变，如果使用Series则可以改变。

**by_group : bool**

如果是True，那么就按行业分类计算.

**quantiles : int or sequence[float]**

因子分层的设置，可以直接设置一个整数，把标的池均分成几个等分，也可以用一个列表，用来分不同的分层比例，例如 [0, .10, .5, .90, 1.] or [.05, .5, .95]

**bins : int or sequence[float]**

与quantiles一样，用于分层设置，不过是value策略。所以，两者直接只能选一个，另外一个必须设置为None

**periods : sequence[int]**

调仓周期设置

**filter_zscore : int or float**

设置收益率异常值的阈值为多少倍标准差

**groupby_labels : dict**

与groupby相互联系，例如之前是按照行业分类，而每个行业用数字表示，这里则可以在把数字映射到具体的行业。
![](https://img-blog.csdn.net/20170624180008055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



所以，我们需要准备好上面这些输入的数据。




最后，我们来看一下这个函数的返回值。



返回值是pd.DataFrame - MultiIndex，包含每个调仓周期的收益率，因子值，所属group（这里是行业），以及分层的次序。
![](https://img-blog.csdn.net/20170624180033567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170624180048785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果，我们整好数据，然后使用了这个函数获得返回值，那么，技术性难题已经解决了80%了。









