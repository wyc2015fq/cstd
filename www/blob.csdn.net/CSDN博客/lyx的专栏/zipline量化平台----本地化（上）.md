# zipline量化平台----本地化（上） - lyx的专栏 - CSDN博客





2017年06月18日 19:58:30[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：15483








        这么多python开源的量化平台中，zipline应该是应用最广泛的一个了，而且在quantopian的体系下，可以和pyfolio和alphalen无缝衔接。但是相比于之前笔者使用的backtrader量化回测平台，zipline在本地的实用化更加复杂。当然，如果用joinquant和ricequant下，其实很easy，但是云端运行策略，很多方面都会有限制。

        所以，zipline想真正用起来，第一步就是本地化。之前了解过本地化的过程，浅尝辄止了，而近来发现，zipline的受众太多了，但是国内的资料几乎是空白。所以笔者尝试进行一下本地化。

        这篇教程，并没有成功的本地化，碰到了一些问题，但是想必已经很接近了，仅此作为一个记录。

# 1.zipline安装

        这里，笔者建议安装zipline之前先安装最新的anaconda，然后用

conda install -c Quantopian zipline

        如果不是最新的anaconda，可能会有各种各样的问题。笔者用的是python2.7的版本，似乎3.X也是可以的。

# 2.zipline教程中的运行方法

        在zipline的官方教程中，重点讲了命令行的运行方法，如下：

zipline run -f ../../zipline/examples/buyapple.py --start 2000-1-1 --end 2014-1-1 -o buyapple_out.pickle

        zipline的ipython格式的教程中，还介绍了ipython中的运行方式，唯独没有介绍如何在pycharm这样的ide中运行的方式。

# 3.zipline在ide中运行的方式

        首先，在zipline中，我们需要两个关键函数来完成一个策略。教程中有如下这样段代码。



```python
from zipline.api import order, record, symbol


def initialize(context):
    pass


def handle_data(context, data):
    order(symbol('AAPL'), 10)
    record(AAPL=data.current(symbol('AAPL'), 'price'))
```
        如果不知道这两个函数分别是干嘛的，大家可以先去joinquant上了解一下基本教程。



        那么，我们怎么在ide中运行这样的一个策略呢？

        方法就是我们构建一个TradingAlgorithm类，然后run这个类。



```python
algor_obj = TradingAlgorithm(initialize=initialize, handle_data=handle_data)
```
        然后，我们run一下

```python
perf_manual = algor_obj.run(data_c)
```
这样我们的策略就完成了。



一个完整的例子如下：



```python
#-*- coding: utf-8 -*-
from datetime import datetime
from zipline.algorithm import TradingAlgorithm
from zipline.finance.trading import TradingEnvironment
from zipline.api import order, record, symbol, history
from zipline.finance import trading
from zipline.utils.factory import create_simulation_parameters
import pandas as pd
import numpy as np

n = 0
# Define algorithm
def initialize(context):
    context.asset = symbol('AAPL')
    print "initialization"
    pass

def handle_data(context, data):
    global n
    print "handle", n
    print data.history(context.asset, 'price', 1, '1d')#close price
    # print history(1, '1d', 'price').mean()
    n += 1
    order(symbol('AAPL'), 10)
    record(AAPL=data.current(symbol('AAPL'), 'price'))

def analyze(context=None, results=None):
    import matplotlib.pyplot as plt
    # Plot the portfolio and asset data.
    ax1 = plt.subplot(211)
    results.portfolio_value.plot(ax=ax1)
    ax1.set_ylabel('Portfolio value (USD)')
    ax2 = plt.subplot(212, sharex=ax1)
    results.AAPL.plot(ax=ax2)
    ax2.set_ylabel('AAPL price (USD)')

    # Show the plot.
    plt.gcf().set_size_inches(18, 8)
    plt.show()

# 本地化工作开始
def load_t(trading_day, trading_days, bm_symbol):
    # dates = pd.date_range('2001-01-01 00:00:00', periods=365, tz="Asia/Shanghai")
    bm = pd.Series(data=np.random.random_sample(len(trading_days)), index=trading_days)
    tr = pd.DataFrame(data=np.random.random_sample((len(trading_days), 7)), index=trading_days,
                      columns=['1month', '3month', '6month', '1year', '2year', '3year', '10year'])
    return bm, tr
trading.environment = TradingEnvironment(load=load_t, bm_symbol='^HSI', exchange_tz='Asia/Shanghai')

# 回测参数设置
sim_params = create_simulation_parameters(year=2014,
    start=pd.to_datetime("2001-01-01 00:00:00").tz_localize("Asia/Shanghai"),
    end=pd.to_datetime("2001-09-21 00:00:00").tz_localize("Asia/Shanghai"),
    data_frequency="daily", emission_rate="daily")  # 原始版本是上面这样的，代码里面是交易日历，然而，如何产生交易日历呢？


# setting the algo parameters
algor_obj = TradingAlgorithm(initialize=initialize, handle_data=handle_data,
                             sim_params=sim_params, env=trading.environment, analyze=analyze
                             )
# data format definition
parse = lambda x: datetime.date(datetime.strptime(x, '%Y/%m/%d'))

# data generator
data_s = pd.read_csv('AAPL.csv', parse_dates=['Date'], index_col=0, date_parser=parse)
print data_s
data_c = pd.Panel({'AAPL': data_s})

perf_manual = algor_obj.run(data_c)
# Print
perf_manual.to_csv('myoutput.csv')
```
        我们可以看到这样的输出，以及一张图片。

initialization

handle 0

2001-01-02 00:00:00+00:00    11.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64

handle 1

2001-01-03 00:00:00+00:00    12.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64

handle 2

2001-01-04 00:00:00+00:00    13.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64

handle 3

2001-01-05 00:00:00+00:00    14.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64

handle 4

2001-01-08 00:00:00+00:00    17.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64

handle 5

2001-01-09 00:00:00+00:00    18.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64

handle 6

2001-01-10 00:00:00+00:00    19.1

Freq: C, Name: Equity(0 [AAPL]), dtype: float64
![](https://img-blog.csdn.net/20170618192837488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        上面的代码中，比笔者之前介绍的多了一些东西。



        首先是我们定义了一个analyze()函数，这个函数在整个回测结束之后将会被调用。大家可以自己定义一些回测的结果计算指标。

        然后，我们在TradingAlgorithm类中，还有sim_params和env参数。而这两个正是我们本地化工作的核心。其中，env更加重要，因为这个参数决定了你的回测环境，也就是说你的市场环境，其中有一个calender的设置，特别重要。然而，笔者并没有完全研究透彻这两个参数，还有很多问题存在。

        这个例子当中的数据跑起来没有问题，因为交易数据是虚假的，其部分数据如下：

Date,open,high,low,close,volume,adj close

2001/1/1,0.1,11.1,9.1,10.1,500,10.1

2001/1/2,0.1,12.1,10.1,11.1,501,11.1

2001/1/3,0.1,13.1,11.1,12.1,502,12.1

2001/1/4,0.1,14.1,12.1,13.1,503,13.1

2001/1/5,14.1,15.1,13.1,14.1,504,14.1

2001/1/6,15.1,16.1,14.1,15.1,505,15.1

2001/1/7,16.1,17.1,15.1,16.1,506,16.1

2001/1/8,17.1,18.1,16.1,17.1,507,17.1

2001/1/9,18.1,19.1,17.1,18.1,508,18.1

2001/1/10,19.1,20.1,18.1,19.1,509,19.1

2001/1/11,20.1,21.1,19.1,20.1,510,20.1

2001/1/12,21.1,22.1,20.1,21.1,511,21.1

2001/1/13,22.1,23.1,21.1,22.1,512,22.1

2001/1/14,23.1,24.1,22.1,23.1,513,23.1

2001/1/15,24.1,25.1,23.1,24.1,514,24.1

2001/1/16,25.1,26.1,24.1,25.1,515,25.1

2001/1/17,26.1,27.1,25.1,26.1,516,26.1

2001/1/18,27.1,28.1,26.1,27.1,517,27.1

2001/1/19,28.1,29.1,27.1,28.1,518,28.1

2001/1/20,29.1,30.1,28.1,29.1,519,29.1

2001/1/21,30.1,31.1,29.1,30.1,520,30.1

2001/1/22,31.1,32.1,30.1,31.1,521,31.1

2001/1/23,32.1,33.1,31.1,32.1,522,32.1

2001/1/24,33.1,34.1,32.1,33.1,523,33.1

2001/1/25,34.1,35.1,33.1,34.1,524,34.1

2001/1/26,35.1,36.1,34.1,35.1,525,35.1

2001/1/27,36.1,37.1,35.1,36.1,526,36.1

2001/1/28,37.1,38.1,36.1,37.1,527,37.1

2001/1/29,38.1,39.1,37.1,38.1,528,38.1

2001/1/30,39.1,40.1,38.1,39.1,529,39.1

2001/1/31,40.1,41.1,39.1,40.1,530,40.1

2001/2/1,41.1,42.1,40.1,41.1,531,41.1

2001/2/2,42.1,43.1,41.1,42.1,532,42.1

2001/2/3,43.1,44.1,42.1,43.1,533,43.1

2001/2/4,44.1,45.1,43.1,44.1,534,44.1

2001/2/5,45.1,46.1,44.1,45.1,535,45.1

2001/2/6,46.1,47.1,45.1,46.1,536,46.1

2001/2/7,47.1,48.1,46.1,47.1,537,47.1

2001/2/8,48.1,49.1,47.1,48.1,538,48.1

2001/2/9,49.1,50.1,48.1,49.1,539,49.1

2001/2/10,50.1,51.1,49.1,50.1,540,50.1


        可以看出来，最大的特点是，日期是连续的。而其实交易日是有放假的。

# 4.真实数据的例子



```python
#! Users\Administrator\Documents\Python Scripts
#-*- coding: utf-8 -*-

import pytz
from datetime import datetime

import zipline
from zipline.algorithm import TradingAlgorithm
from zipline.finance.trading import TradingEnvironment
from zipline.api import order, record, symbol, history
from zipline.finance import trading
from zipline.utils.factory import create_simulation_parameters
from zipline.assets.synthetic import make_simple_equity_info
from zipline.utils.calendars import exchange_calendar_sh
from zipline.utils.calendars import exchange_calendar_lse
from zipline.utils.calendars import exchange_calendar_nyse
import pandas as pd
import numpy as np

n = 0
# Define algorithm
def initialize(context):
    context.asset = symbol('dfqc')
    print "initialization"
    pass


def handle_data(context, data):
    global n
    print "handle", n
    print data.history(context.asset, 'price', 1, '1d')#close price
    # print history(1, '1d', 'price').mean()
    n += 1
    order(symbol('dfqc'), 10)
    record(AAPL=data.current(symbol('dfqc'), 'price'))

def analyze(context=None, results=None):
    import matplotlib.pyplot as plt
    # Plot the portfolio and asset data.
    ax1 = plt.subplot(211)
    results.portfolio_value.plot(ax=ax1)
    ax1.set_ylabel('Portfolio value (USD)')
    ax2 = plt.subplot(212, sharex=ax1)
    results.AAPL.plot(ax=ax2)
    ax2.set_ylabel('dfqc price (USD)')

    # Show the plot.
    plt.gcf().set_size_inches(18, 8)
    plt.show()

# This is needed to handle the correct calendar. Assume that market data has the right index for trade able days.
# Passing in env_trading_calendar=trading calendar_lse doesn't appear to work, as it doesn't implement open_and_closes

def load_t(trading_day, trading_days, bm_symbol):
    # dates = pd.date_range('2001-01-01 00:00:00', periods=365, tz="Asia/Shanghai")
    bm = pd.Series(data=np.random.random_sample(len(trading_days)), index=trading_days)
    tr = pd.DataFrame(data=np.random.random_sample((len(trading_days), 7)), index=trading_days,
                      columns=['1month', '3month', '6month', '1year', '2year', '3year', '10year'])
    return bm, tr
cal = exchange_calendar_sh.SHExchangeCalendar()
trading.environment = TradingEnvironment(load=load_t, bm_symbol='^HSI', exchange_tz='Asia/Shanghai', trading_calendar=cal)



# 回测参数设置
sim_params = create_simulation_parameters(year=2014,
    start=pd.to_datetime("2014-01-01 00:00:00").tz_localize("Asia/Shanghai"),
    end=pd.to_datetime("2014-03-07 00:00:00").tz_localize("Asia/Shanghai"),
    data_frequency="daily", emission_rate="daily")  # 原始版本是上面这样的，代码里面是交易日历，然而，如何产生交易日历呢？


# setting the algo parameters
algor_obj = TradingAlgorithm(initialize=initialize, handle_data=handle_data,
                             sim_params=sim_params, env=trading.environment, analyze=analyze,
                             trading_calendar=cal)

# data format definition
parse = lambda x: datetime.date(datetime.strptime(x, '%Y-%m-%d %H:%M:%S.%f'))

# parse = lambda x: datetime.date(pytz.utc.localize(datetime.strptime(x, '%Y-%m-%d %H:%M:%S.%f')))
# data generator
data_s = pd.read_csv('dfqc.csv', parse_dates=['date'], index_col=0, date_parser=parse)
print data_s
data_c = pd.Panel({'dfqc': data_s})


perf_manual = algor_obj.run(data_c)
# Print
perf_manual.to_csv('myoutput.csv')
```
        这是一个使用真实数据来实现本地化的例子，但是没有成功，笔者认为问题在于交易日期的设置。



        注意到，在设置TradingEnvironment和TradingAlgorithm中，我们多了一个trading_calendar的参数，参数都是cal。而cal是SHExchangeCalendar()的一个实例。



```python
cal = exchange_calendar_sh.SHExchangeCalendar()
```
        而，exchange_calendar_sh是笔者参照别的calender文件写的，如下：





```python
from datetime import time
from itertools import chain

from pandas.tseries.holiday import (
    GoodFriday,
    USPresidentsDay,
    USLaborDay,
    USThanksgivingDay
)
from pandas.tslib import Timestamp
from pytz import timezone
from pandas.tseries.holiday import (
    Holiday,
    DateOffset,
    MO,
    weekend_to_monday,
    GoodFriday,
    EasterMonday,
)

from zipline.utils.calendars import TradingCalendar
from zipline.utils.calendars.trading_calendar import HolidayCalendar
from zipline.utils.calendars.us_holidays import (
    USNewYearsDay,
    Christmas,
    USMartinLutherKingJrAfter1998,
    USMemorialDay,
    USIndependenceDay,
    USNationalDaysofMourning)

CHINANewYearsDay = Holiday(
    "New Year's Day",
    month=1,
    day=31,
)
class SHExchangeCalendar(TradingCalendar):
    @property
    def name(self):
        return "SH"

    @property
    def tz(self):
        return timezone("Asia/Shanghai")

    @property
    def open_time(self):
        return time(9, 30)

    @property
    def close_time(self):
        return time(15)

    @property
    def regular_holidays(self):
        return HolidayCalendar([
            CHINANewYearsDay
        ])
```
        如果，我们不设置trading_calendar，我们运行的结果如下：



![](https://img-blog.csdn.net/20170618194315202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这个错误的原因应该如下：

        当我们不设置trading_calendar的时候，zipline默认使用纳斯达克的交易时间，2014-1-31在美国是正常的交易日，但是在中国一天是春节，所以我们的数据中没有这一天的数据。当我们随意补上一条虚拟的2014-1-31的交易记录后，错误提示变成如下：

![](https://img-blog.csdn.net/20170618195007265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        由于2月1号2号是周末，所以zipline自动跳过了，但是2月3号在国内还是春节，所以错误提示就变成这样了。

        既然如此我们把1月31号设置成一个假日之后来看一下，然后用我们自己的trading_calendar，但是结果却不是笔者想象的那样，而是

![](https://img-blog.csdn.net/20170618195518971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


        错误变成很奇怪的东西了。

于是笔者迷茫了。希望下次能够解决吧。











