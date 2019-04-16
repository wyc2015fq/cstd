# 多因子模型之因子（信号）测试平台----alphalens（一） - lyx的专栏 - CSDN博客





2017年08月27日 22:56:43[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4008








广告：本人的单因子测试视频教程 [https://study.163.com/course/introduction/1005568012.htm](https://study.163.com/course/introduction/1005568012.htm)



之前，我们计算了revs10这个因子，并且对其进行了去极值、标准化、和行业中性。

计算因子，并合理处理是单因子测试的第一步，我们必须有正确、合理的因子值，才会有有意义的结果，否则就是garbage in， garbage out。

        对于单因子测试，quantopian有一个很好的开源的python工具，就是alphalens，当然，如果我们想做一个让自己满意的单因子回测平台的话，必然是需要对原生的alphalens做一些修改的。

        这一部分，我们先将我们的数据处理成alphalens可以接受的数据格式，然后做一个简单的测试，看看一切是否都正常。

# 1.数据处理

        如果我们想使用alphalens，那么就要遵守alphalens的规矩。而alphalens的规矩，就是我们传入的数据要符合alphalens的要求。

```python
import pandas as pd
import alphalens
import matplotlib.pyplot as plt
ind_dict = { u'000156.SZ': u'6122010000000000',
             u'000157.SZ': u'6107000000000000',
             u'000166.SZ': u'1000012613000000',
             u'000333.SZ': u'6111000000000000',
             u'000338.SZ': u'1000012588000000',
             u'000402.SZ': u'6118000000000000',
             u'000413.SZ': u'6108000000000000',
             u'000415.SZ': u'1000012613000000',
             u'000423.SZ': u'6115000000000000',
             u'000425.SZ': u'6107000000000000',
             u'000503.SZ': u'6115000000000000',
             u'000538.SZ': u'6115000000000000',
             u'000540.SZ': u'6118000000000000',
             u'000555.SZ': u'1000012601000000',
             u'000559.SZ': u'1000012588000000',
             u'000568.SZ': u'6112000000000000',
             u'000623.SZ': u'6115000000000000',
             u'000625.SZ': u'1000012588000000',
             u'000627.SZ': u'1000012613000000',
             u'000630.SZ': u'6105000000000000',
             u'000651.SZ': u'6111000000000000',
             u'000671.SZ': u'6118000000000000',
             u'000686.SZ': u'1000012613000000',
             u'000709.SZ': u'6104000000000000',
             u'000712.SZ': u'1000012613000000',
             u'000718.SZ': u'6118000000000000',
             u'000725.SZ': u'6108000000000000',
             u'000728.SZ': u'1000012613000000',
             u'000738.SZ': u'1000012579000000',
             u'000750.SZ': u'1000012613000000',
             u'000768.SZ': u'1000012579000000',
             u'000776.SZ': u'1000012613000000',
             u'000778.SZ': u'6104000000000000',
             u'000783.SZ': u'1000012613000000',
             u'000792.SZ': u'6103000000000000',
             u'000793.SZ': u'6122010000000000',
             u'000800.SZ': u'1000012588000000',
             u'000826.SZ': u'6116000000000000',
             u'000839.SZ': u'6123000000000000',
             u'000858.SZ': u'6112000000000000',
             u'000876.SZ': u'6101000000000000',
             u'000895.SZ': u'6112000000000000',
             u'000917.SZ': u'6122010000000000',
             u'000938.SZ': u'1000012601000000',
             u'000963.SZ': u'6115000000000000',
             u'000977.SZ': u'1000012601000000',
             u'000983.SZ': u'6102000000000000',
             u'001979.SZ': u'6118000000000000',
             u'002007.SZ': u'6115000000000000',
             u'002008.SZ': u'6108000000000000'}

data = pd.read_csv('total_data.csv', index_col=0)
data['date'] = pd.to_datetime(data['date'])
factor_df = data[['date', 'sec_id', 'neuted_score']]
factor_df = factor_df.set_index(['date', 'sec_id']) # 因子值的数据
price_df = data[['date', 'sec_id', 'close']]
price_df = price_df.pivot(index='date', columns='sec_id', values='close') #股票价格的数据

factor_data = alphalens.utils.get_clean_factor_and_forward_returns( \
    factor_df, price_df, quantiles=5, groupby=ind_dict
    ) # 利用alphalens自带的工具，生成alphalens可以接受的数据。
```

        具体，每一个部分，factor_df，price_df的具体结构是怎么样的，可以查看之前笔者关于alphalens的教程。

[http://blog.csdn.net/qtlyx/article/details/73693129](http://blog.csdn.net/qtlyx/article/details/73693129)



# 2. 测试一下我们的数据

        我可以尝试使用一下上面的数据：

```python
alphalens.tears.create_summary_tear_sheet(factor_data)
# price_df = price_df.pivot(index='tradeDate', columns='secID', values='closePrice')
plt.show()
```

        然后大家就可以看到相应的效果了。

       具体每一个部分意义，包括图片的含义，是我们接下来的重点，也是单因子测试的核心点。







