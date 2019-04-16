# alphalens教程4--Turnover Analysis - lyx的专栏 - CSDN博客





2017年07月01日 21:58:59[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1804








      衡量一个因子的好坏还有一个指标，就是稳定性。因子的稳定性直接决定了你的调仓频率。

# 1.基本数据获取
        每天每个股票的因子分层位置。

        我们先来看第一个计算换手率的函数。

def quantile_turnover(quantile_factor, quantile, period=1):

        我们来看一下这个计算出来的是什么：某一层因子中，某个股票之前不在这个层，现在在了的比例是多少。

参数解释：

quantile_factor : pd.Series

日期、股票名为index，因子层序号为value的series，也就是factor_data['factor_quantile']，所以，我们先把quantile_factor = factor_data['factor_quantile']定义好。

quantile : int

计算哪一层的换手率

period: int, optional

计算哪一个周期调仓的换手率

返回一个 pd.Series，value是换手率，例如：

alphalens.performance.quantile_turnover(quantile_factor, 1, 1)，返回的就是日调仓，因子层第一层的换手率。

![](https://img-blog.csdn.net/20170701215603833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        当然，我们希望获得的是每个层的换手率，所以，写成下面这样的列表表达式，然后concat一下。

quantile_turnover = pd.concat([alphalens.performance.quantile_turnover(quantile_factor, q, turnover_period)


# 2.绘图
        接下来，我们利用上面生成的数据来绘制图形。

def plot_top_bottom_quantile_turnover(quantile_turnover, period=1, ax=None):

top和bottom层的换手率图。

alphalens.plotting.plot_top_bottom_quantile_turnover(quantile_turnover, turnover_period)



![](https://img-blog.csdn.net/20170701215704167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        除了换手率查看因子的稳定性外，因子的自回归程度也是一个很重要的衡量因子稳定的指标。

def factor_rank_autocorrelation(factor_data, period=1):

        计算自回归系数的函数很简单。

factor_autocorrelation = alphalens.performance.factor_rank_autocorrelation(factor_data, period=1)


![](https://img-blog.csdn.net/20170701215728973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


alphalens.plotting.plot_factor_rank_auto_correlation(factor_autocorrelation)


![](https://img-blog.csdn.net/20170701215751801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当然，同样的，也有一个整个的函数。

alphalens.tears.create_turnover_tear_sheet(factor_data)



















