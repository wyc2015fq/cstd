# alphalens教程3--Information Analysis - lyx的专栏 - CSDN博客





2017年07月01日 14:27:30[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2201








information anslysis适用于分析评价在不考虑交易成本下，一个factor的预测能力的一种方法。主要的方法就是通过因子的IC来分析。

# 1.ic的生成
首先介绍一下一个通过上次的factor_data生成ic的函数：

def factor_information_coefficient(factor_data,

                                   group_adjust=False,

                                   by_group=False):

这个是计算Spearman Rank Correlation IC的函数，参数也很好理解。 based Information Coefficient (IC)。group_adjust就是是否做组内中心，譬如我们的group是行业，那么，选择为true的时候，就是行业中性的算法。

ic = alphalens.performance.fiactor_nformation_coefficient(factor_data)

![](https://img-blog.csdn.net/20170701141756129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        有了这样的数据，和return analysis的顺序一样，接下来就是画图了。


# 2.IC的时间序列        



第一个是time series的图。

def plot_ic_ts(ic, ax=None):

alphalens.plotting.plot_ic_ts(ic)


![](https://img-blog.csdn.net/20170701141813087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




# 3.IC的分布直方图  

然后是绘制ic的分布直方图

def plot_ic_hist(ic, ax=None):

alphalens.plotting.plot_ic_hist(ic)

这个函数会把ic值的分布直方图绘制出来，然后可以看到ic值的分布，我们希望是ic值均值越高越好，方差越小越好。

![](https://img-blog.csdn.net/20170701141834301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



# 4.IC的QQ图        

有了分布，我们要看一下是否正态分布，或者是否和某个分布很吻合。所以，我们还可以绘制qq plot，用于查看我们的ic值是否是一个正态分布，或者多大程度与正态分布相似。

def plot_ic_qq(ic, theoretical_dist=stats.norm, ax=None):

alphalens.plotting.plot_ic_qq(ic)


![](https://img-blog.csdn.net/20170701141944567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5.IC不同维度的比较

def mean_information_coefficient(factor_data,

                                 group_adjust=False,

                                 by_group=False,

                                 by_time=None):

mean_monthly_ic = alphalens.performance.mean_information_coefficient(factor_data, by_time='M')

这个函数给出ic各种形式下的均值，例如，特定的group，每个月，每个group，每个月。 


![](https://img-blog.csdn.net/20170701142006761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       有了分类数据之后，我们可以绘制热力图，热力图可以让我们查看不同市场，不同时间下的因子的一致性。

alphalens.plotting.plot_monthly_ic_heatmap(mean_monthly_ic)


![](https://img-blog.csdn.net/20170701142029597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


和return analysis一样，也有一个所以一起生成的语句：

alphalens.tears.create_information_tear_sheet(factor_data)







