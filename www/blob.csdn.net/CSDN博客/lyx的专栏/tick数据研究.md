# tick数据研究 - lyx的专栏 - CSDN博客





2019年03月15日 23:24:52[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：93








      经常听见tick数据，回测的时候也用过，但是还真的没有自己去处理过tick数据，据说tick数据有很多坑，所以打算自己研究一下。首先的第一步就是先拿正常的tick数据来生成bar，从而能够理解一些细节，然后就是自己用ctp去接收tick数据，看看ctp有没有坑。

      这里，完美的tick数据是wind上的。

![](https://img-blog.csdnimg.cn/20190315231113508.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      这是wind上面导出来的，看起来还是比较正常的，反正一秒两个数据嘛。毕竟我们知道，咱们交易所给我们的数据不是真正的tick，而是snapshot，说白了就是500毫秒一次切片。一切的行情软件，其实都是根据tick数据来实现的。

      tick数据当然还有别的东西，比如ask、bid但是，最重要的还是last_price和volume。last price当然可以理解，切片时候的成交价格嘛，至于volume，我们来看一下曲线:

![](https://img-blog.csdnimg.cn/20190315231516164.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

       所以，tick数据的volume是累计成交量，而一天的开始是九点的夜盘开始。当然没有夜盘的品种当然就是第二天早上九点了。

      那么怎么变成分钟数据呢？也就是tick变成bar。

```python
#encoding=utf-8
import pandas as pd
from matplotlib import pyplot as plt
import matplotlib.finance as mpf
from matplotlib.pylab import date2num
tick_df = pd.read_hdf('rb_tick.h5')


class mBar(object):
    def __init__(self):
        """Constructor"""
        self.open = None
        self.close = None
        self.high = None
        self.low = None
        self.datetime = None

bar = None
m_bar_list = list()

for datetime, last in tick_df[['last']].iterrows():
    new_minute_flag = False

    if not bar:  # 第一次进循环
        bar = mBar()
        new_minute_flag = True
    elif bar.datetime.minute != datetime.minute:
        bar.datetime = bar.datetime.replace(second=0, microsecond=0)  # 将秒和微秒设为0
        m_bar_list.append(bar)
        # 开启新的一个分钟bar线
        bar = mBar()
        new_minute_flag = True


    if new_minute_flag:
        bar.open, bar.high, bar.low = last['last'], last['last'], last['last']
    else:
        bar.high, bar.low = max(bar.high, last['last']), min(bar.low, last['last'])

    bar.close = last['last']
    bar.datetime = datetime

pk_df = pd.DataFrame(data=[[bar.datetime for bar in m_bar_list], 
                           [bar.close for bar in m_bar_list], 
                           [bar.open for bar in m_bar_list],
                           [bar.high for bar in m_bar_list],
                           [bar.low for bar in m_bar_list]],
             index=['datetime', 'close', 'open','high', 'low']
                     ).T[['datetime', 'open', 'high', 'low', 'close']]

pk_df['datetime'] = pk_df['datetime'].apply(lambda x: date2num(x)*1440) # 为了显示分钟而不叠起来
fig, ax = plt.subplots(facecolor=(0, 0.3, 0.5),figsize=(12,8))

mpf.candlestick_ohlc(ax,pk_df.iloc[:100].as_matrix(),width=0.7,colorup='r',colordown='green') # 上涨为红色K线，下跌为绿色，K线宽度为0.7
plt.grid(True)
```

我们看一下我们生成的bar和wind给出的bar。

![](https://img-blog.csdnimg.cn/20190315232211911.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

来自wind的：

![](https://img-blog.csdnimg.cn/20190315232258730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

      笔者核对过，完全一样。但是这里要注意一个问题，就是分钟Bar的时间戳。我们上面的程序中，分钟bar的时间戳是bar的时间开始，也就是说，14:31分钟的bar线是14点31分0秒开始到14点31分59秒。而有的软件是用bar的时间的结束作为时间戳的。

      理论上，解决这个问题之后，我们就可以把精力聚焦于如何获得质量较高的tick数据。实际过程中，我们的tick数据都是实时的，所以，tick数据的质量往往由两个因素决定，一个是我们处理tick的回调数据的速度，如果响应和处理都很慢的话，显然就会有很大的问题；另外一个影响实时的tick数据的因素就是ctp前置的实时负载，如果服务器压力大的话，很容易就会丢失数据。

      然后笔者花了几天，利用vnpy封装好的ctp接口接收了几天的数据。大致看起来，可能是网络比较好，又地处上海金融中心陆家嘴，所以没有丢包的情况，检查了一下，一直是一秒两个切片。

2019-03-15 14:59:49.500000,3763.0,3709870,3764.0,0.0,0.0,0.0,0.0,40,0,0,0,0,3763.0,0.0,0.0,0.0,0.0,371,0,0,0,0
2019-03-15 14:59:50.500000,3764.0,3710314,3764.0,0.0,0.0,0.0,0.0,58,0,0,0,0,3763.0,0.0,0.0,0.0,0.0,284,0,0,0,0
2019-03-15 14:59:51.500000,3763.0,3711114,3765.0,0.0,0.0,0.0,0.0,738,0,0,0,0,3764.0,0.0,0.0,0.0,0.0,3,0,0,0,0
2019-03-15 14:59:52,3764.0,3711880,3763.0,0.0,0.0,0.0,0.0,1,0,0,0,0,3762.0,0.0,0.0,0.0,0.0,50,0,0,0,0
2019-03-15 14:59:52.500000,3764.0,3712004,3764.0,0.0,0.0,0.0,0.0,12,0,0,0,0,3763.0,0.0,0.0,0.0,0.0,2,0,0,0,0
2019-03-15 14:59:53.500000,3762.0,3712846,3763.0,0.0,0.0,0.0,0.0,20,0,0,0,0,3762.0,0.0,0.0,0.0,0.0,6,0,0,0,0
2019-03-15 14:59:54.500000,3764.0,3713058,3765.0,0.0,0.0,0.0,0.0,739,0,0,0,0,3764.0,0.0,0.0,0.0,0.0,55,0,0,0,0
2019-03-15 14:59:55.500000,3761.0,3713670,3762.0,0.0,0.0,0.0,0.0,2,0,0,0,0,3761.0,0.0,0.0,0.0,0.0,356,0,0,0,0
2019-03-15 14:59:56.500000,3763.0,3713982,3763.0,0.0,0.0,0.0,0.0,55,0,0,0,0,3762.0,0.0,0.0,0.0,0.0,94,0,0,0,0
2019-03-15 14:59:57.500000,3761.0,3714472,3762.0,0.0,0.0,0.0,0.0,4,0,0,0,0,3761.0,0.0,0.0,0.0,0.0,327,0,0,0,0
2019-03-15 14:59:58.500000,3763.0,3714668,3763.0,0.0,0.0,0.0,0.0,6,0,0,0,0,3762.0,0.0,0.0,0.0,0.0,5,0,0,0,0
2019-03-15 14:59:59,3762.0,3714990,3763.0,0.0,0.0,0.0,0.0,1,0,0,0,0,3762.0,0.0,0.0,0.0,0.0,1,0,0,0,0

但是，中间也发生了比较奇怪的现象，比如：

2019-03-15 14:59:59,3762.0,3714990,3763.0,0.0,0.0,0.0,0.0,1,0,0,0,0,3762.0,0.0,0.0,0.0,0.0,1,0,0,0,0
2019-03-15 15:00:00.500000,3763.0,3715090,3764.0,0.0,0.0,0.0,0.0,140,0,0,0,0,3763.0,0.0,0.0,0.0,0.0,12,0,0,0,0
2019-03-15 15:00:01.500000,3763.0,3715090,3764.0,0.0,0.0,0.0,0.0,140,0,0,0,0,3763.0,0.0,0.0,0.0,0.0,12,0,0,0,0
2019-03-15 15:18:12,3763.0,3715090,3764.0,0.0,0.0,0.0,0.0,140,0,0,0,0,3763.0,0.0,0.0,0.0,0.0,12,0,0,0,0
2019-03-15 18:11:30.500000,3763.0,0,1.7976931348623157e+308,0.0,0.0,0.0,0.0,0,0,0,0,0,1.7976931348623157e+308,0.0,0.0,0.0,0.0,0,0,0,0,0
2019-03-15 19:48:09,3763.0,0,1.7976931348623157e+308,0.0,0.0,0.0,0.0,0,0,0,0,0,1.7976931348623157e+308,0.0,0.0,0.0,0.0,0,0,0,0,0
2019-03-15 20:59:01,3758.0,4626,3759.0,0.0,0.0,0.0,0.0,129,0,0,0,0,3758.0,0.0,0.0,0.0,0.0,32,0,0,0,0
2019-03-15 21:00:02,3760.0,8248,3760.0,0.0,0.0,0.0,0.0,255,0,0,0,0,3759.0,0.0,0.0,0.0,0.0,601,0,0,0,0
注意到，15点18分、16点11分等等，在非交易时间也出现了tick数据，而且，有一个8点59分01秒。

这些数据很奇怪，在实盘过程中都是要被剔除的。通常，我们可以设置ctp接受的开始和结束的时间，但是像8点59分这样的记录，其实很难去分离，所以大概还要叠加首个tick是否符合时间要求吧。







