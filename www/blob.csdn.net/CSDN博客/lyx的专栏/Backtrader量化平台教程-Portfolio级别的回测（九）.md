# Backtrader量化平台教程-Portfolio级别的回测（九） - lyx的专栏 - CSDN博客





2017年07月29日 19:50:22[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4078
所属专栏：[量化回测平台Backtrader教程](https://blog.csdn.net/column/details/15582.html)









AD：（本人录制的backtrader视频课程，大家多多支持哦~ [https://edu.csdn.net/course/detail/9040](https://edu.csdn.net/course/detail/9040)）   

    我们知道，有时候我们要测试的可不仅仅是一个标的，但是之前backtrader的教程中，我们都是针对的是一个标的的回测。如果接触过优矿、ricequant这样的平台的同学，可能觉得backtrader不适合做这样的portfolio层面的回测。确实，似乎backtrader整个官方教程里面，没有任何讲到这种全市场、组合的回测demo，但是backtrader其实也是可以胜任这样的任务的。

       前段时间，笔者就做了这样的一个事情，让backtrader能够完成我们想要的组合层面的回测。

# 1.最终的效果

       和一般的portfolio层面的回测平台一样，我们希望，最后我们实现一个策略只要进行一些设置就可以了。笔者利用backtrader封装了一个函数，实现了几乎和优矿一样的功能。

        使用的时候，笔者的函数只需要如下的设置：



```python
start_date = "2017-04-01"
end_date = "2017-06-20"
trading_csv_name = 'trading_data_two_year.csv'
portfolio_csv_name = 'port_two_year.csv'
benchmark_csv_name = None
```

         然后我们就可以回测了。笔者把回测的类封装了起来，只要调用笔者的回测类就可以了。

```python
begin = datetime.datetime.now()
result_dict = bt_backtest.backtrader_backtest(start_date=start_date, end_date=end_date, trading_csv_name=trading_csv_name, \
                                              portfolio_csv_name=portfolio_csv_name, bechmark_csv_name=benchmark_csv_name)
```

        然后，回测结束后输出评价指标。

```python
end = datetime.datetime.now()
print "time elapse:", (end - begin)
print 'Start Portfolio Value: %.2f' % result_dict['start_cash']
print 'Final Portfolio Value: %.2f' % result_dict['final_value']
print 'Total Return:', result_dict['total_return']
print 'Sharpe Ratio :', result_dict['sharpe_ratio'] #* 2 # todo there should be consider!
print 'Max Drowdown:', result_dict['max_drowdown'] * 2
print 'Max Drowdown Money:', result_dict['max_drowdown_money']
print "Trade Information", result_dict['trade_info']

result = pd.read_csv('result.csv', index_col=0)
result.plot()
plt.show()
position_info = pd.read_csv('position_info.csv', index_col=0)
```

        接下来说一下我们要输入的这些csv文件吧。

```python
trading_csv_name = 'trading_data_two_year.csv'
portfolio_csv_name = 'port_two_year.csv'
benchmark_csv_name = None
```

        第一个是交易行情的数据，数据格式如下：



tradingdate,ticker,_open,_high,_low,_close,_volume

20070104,000001,14.65,15.32,13.83,14.11,69207082.0

20070104,000002,15.7,16.56,15.28,15.48,75119519.0

20070104,000004,4.12,4.12,3.99,4.06,1262915.0

20070104,000005,2.51,2.53,2.46,2.47,14123749.0

20070104,000006,13.5,14.07,13.39,13.7,15026054.0

20070104,000007,2.44,2.44,2.35,2.36,3014956.0

20070104,000008,4.16,4.24,4.15,4.16,818282.0

20070104,000009,0.0,0.0,0.0,4.23,0.0

20070104,000010,0.0,0.0,0.0,5.37,0.0

20070104,000011,5.78,5.85,5.42,5.45,4292318.0

20070104,000012,11.15,11.3,10.66,10.95,6934903.0

        所以，这一部分的数据会相当的大，一天就会有3000条左右的记录，毕竟，A股的股票数目就是这样。如果是用的更小级别的数据，那么数据量必然更大了。



```python
portfolio_csv_name = 'port_two_year.csv'
```



        这是我们调仓日的目标仓位，只需要sigdate,secucode,weight三个字段就行。

        benchmark_csv_name自然就是benchmark的daily return数据文件了。这里可以不设置。

# 2.回测函数

         接下来就是核心的回测的函数了。

```python
# -*- coding: utf-8 -*-

from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import datetime
import backtrader as bt
from backtrader import Order
import pandas as pd
import matplotlib.pyplot as plt


#  Create a Stratey
class AlphaPortfolioStrategy(bt.Strategy):

    def log(self, txt, dt=None):
        dt = dt or self.datas[0].datetime.date(0)
        print('%s, %s' % (dt.isoformat(), txt))

    def __init__(self, dataId_to_secId_dict, secId_to_dataId_dict, adj_df, end_date, backtesting_length=None, benchmark=None, result_csv_name='result'):
        # 1.get target portfolio weight
        self.adj_df = adj_df
        self.backtesting_length = backtesting_length
        self.end_date = end_date
        # 2.backtrader data_id and secId transfer diction
        self.dataId_to_secId_dict = dataId_to_secId_dict
        self.secId_to_dataId_dict = secId_to_dataId_dict
        self.benchmark = benchmark
        # 3.store the untradable day due to the up and down floor, empty it in a new adjustment day
        self.order_line_dict = {}
        self.pre_position_data_id = list()
        self.value_for_plot = {}
        # 4.keep track of pending orders and buy price/commission
        self.order = None
        self.buyprice = None
        self.buycomm = None
        self.value = 10000000.0
        self.cash = 10000000.0

        self.positions_info = open('position_info.csv', 'wb')
        self.positions_info.write('date, bt_id,sec_code, size, last price\n')

    def start(self):
        print("the world call me!")

    def notify_fund(self, cash, value, fundvalue, shares):
        # update the market value every day
        # print("actual value:", value)
        self.value = value - 10000000.0  # we give the broker more 10 million for the purpose of illiquidity
        # self.value = value  # we give the broker more 10 million for the purpose of illiquidity

        self.value_for_plot[self.datetime.datetime()] = self.value / 10000000.0
        self.cash = cash
        # print("cash:", cash)

    def notify_order(self, order):
        if order.status in [order.Submitted, order.Accepted]:
            # Buy/Sell order submitted/accepted to/by broker - Nothing to do
            return
        # Check if an order has been completed
        # Attention: broker could reject order if not enougth cash
        if order.status in [order.Completed]:
            if order.isbuy():
                # order.
                self.log(
                    'BUY EXECUTED, Price: %.2f, Cost: %.2f, Comm %.2f' %
                    (order.executed.price,
                     order.executed.value,
                     order.executed.comm))
                self.buyprice = order.executed.price
                self.buycomm = order.executed.comm
            else:  # Sell
                self.log('SELL EXECUTED, Price: %.2f, Cost: %.2f, Comm %.2f' %
                         (order.executed.price,
                          order.executed.value,
                          order.executed.comm))
            self.bar_executed = len(self)
        elif order.status in [order.Canceled, order.Rejected]:
            self.log('Order Canceled/Rejected')
        elif order.status == order.Margin:
            self.log('Order Margin')
        self.order = None

    def notify_trade(self, trade):
        if not trade.isclosed:
            return
        self.log('OPERATION PROFIT, GROSS %.2f, NET %.2f' %
                 (trade.pnl, trade.pnlcomm))

    def next(self):
        # 0.Check if an order is pending ... if yes, we cannot send a 2nd one
        # if self.order:
        #     return
        bar_time = self.datetime.datetime()
        bar_time_str = bar_time.strftime('%Y-%m-%d')
        trading_date = bar_time + datetime.timedelta(days=1)
        # bar_time_str = (self.datetime.datetime() + datetime.timedelta(1)).strftime('%Y-%m-%d')
        print(bar_time_str, self.value)
        print("bar day===:", bar_time_str, "===============")
        for (k, v) in self.dataId_to_secId_dict.items():
            size = self.positions[self.datas[k]].size
            price = self.datas[k].close[-1]
            self.positions_info.write('%s ,%s ,%s, %s, %s' % (bar_time_str, k, v, size, price))
            self.positions_info.write('\n')

        # 1. no matter the adjustment day, up/down floor blocked order should deal with each bar
        for (k, v) in self.order_line_dict.items():
            bar = self.datas[k]
            buyable = False if bar.low[1] / bar.close[0] > 1.0950 else True
            sellable = False if bar.high[1] / bar.close[0] < 0.910 else True

            # buyable = False if (bar.open[1] / bar.close[0] > 1.0950) and (bar.close[1] / bar.close[0] > 1.0950) else True
            # sellable = False if (bar.open[1] / bar.close[0] < 0.910 and (bar.close[1] / bar.close[0] < 0.91)) else True

            if v > 0:
                if buyable:
                    del self.order_line_dict[k]
                    self.log('%s BUY CREATE, %.2f, vlo is %s' % (self.dataId_to_secId_dict[k], bar.open[1], v))
                    self.order = self.buy(data=bar, size=v, exectype=Order.Market)
                else:
                    print("############:")
                    print("unbuyable:", self.dataId_to_secId_dict[k])
            elif v < 0:
                if sellable:
                    del self.order_line_dict[k]
                    self.log('%s SELL CREATE, %.2f, vol is %s' % (self.dataId_to_secId_dict[k], bar.open[1], v))
                    self.order = self.sell(data=bar, size=abs(v), exectype=Order.Market)
                else:
                    print("############:")
                    print("unsellable:", self.dataId_to_secId_dict[k])

        # 2. ensure the adjustment day
        # 2.1 get the current bar time
        adj_sig = self.adj_df[self.adj_df['sigdate'] == trading_date.strftime('%Y-%m-%d')][['secucode', 'hl_weight']]
        # 2.2 check the adjustment day
        if len(adj_sig) == 0 or bar_time_str == self.end_date:
            return
        # 3. adjust the portfolio
        # 3.1 set two dicts to store the buy order and sell order spearately
        buy_dict = {}
        sell_dict = {}
        self.order_line_dict = {}
        current_position_data_id = list()
        # 3.2 iterate the portfolio instruments and divide into buy group and sell group
        for index in adj_sig.index:
            # get current instrument code and transfer to the data_id
            sec_id = adj_sig.loc[index]['secucode']
            data_id = self.secId_to_dataId_dict[sec_id]
            if self.backtesting_length and data_id >= self.backtesting_length:
                continue
            bar = self.datas[data_id]
            current_position_data_id.append(data_id)
            # get the  target weight value
            target_weight = adj_sig.loc[index]['hl_weight']
            # calculate the current weight value
            current_position = self.positions[self.datas[data_id]]
            current_mv = current_position.size * bar.close[0]
            current_weight = current_mv / float(self.value)
            diff_weight = (target_weight - current_weight)
            if bar.open[1] == 0:
                continue
            diff_volume = int(diff_weight * self.value / bar.open[1] / 100) * 100
            print("the weight difference", diff_weight)
            if diff_volume > 0:
                buy_dict[data_id] = diff_volume
            elif diff_volume < 0:
                sell_dict[data_id] = diff_volume

        # 3.3 make order work
        for (k, v) in sell_dict.items():
            bar = self.datas[k]
            # sellable = False if (bar.high[1] == bar.low[1]) and (bar.open[1] / bar.close[0] < 0.920) else True
            sellable = False if (bar.open[1] / bar.close[0] < 0.920) else True
            # sellable = False if (bar.open[1] / bar.close[0] < 0.910 and (bar.close[1] / bar.close[0] < 0.91)) else True

            if sellable:
                self.log('%s SELL CREATE, %.2f, vol is %s' % (self.dataId_to_secId_dict[k], bar.open[1], v))
                self.order = self.sell(data=bar, size=abs(v), exectype=Order.Market)
            else:
                print("############:")
                print("unsellable:", self.dataId_to_secId_dict[k])
                self.order_line_dict[k] = v

        for (k, v) in buy_dict.items():
            bar = self.datas[k]
            # buyable = False if (bar.low[1] == bar.high[1]) and (bar.open[1] / bar.close[0]) > 1.0950 else True
            buyable = False if (bar.open[1] / bar.close[0]) > 1.0950 else True
            # buyable = False if (bar.open[1] / bar.close[0] > 1.0950) and (bar.close[1] / bar.close[0] > 1.0950) else True

            if buyable:
                self.log('%s BUY CREATE, %.2f, vlo is %s' % (self.dataId_to_secId_dict[k], bar.open[1], v))
                self.order = self.buy(data=bar, size=v, exectype=Order.Market)
            else:
                print("############:")
                print("unbuyable:", self.dataId_to_secId_dict[k])
                self.order_line_dict[k] = v

        # 3.4 close position, when the data_id is not in current portfolio and in last portfolio, we close the position
        if self.pre_position_data_id:
            clost_data_id = [di for di in self.pre_position_data_id if di not in current_position_data_id]
            for di in clost_data_id:
                bar = self.datas[di]
                print('CLOSE POSITION:',  self.dataId_to_secId_dict[di])
                self.order = self.close(data=bar)
        # 3.5 update the position data id for next checking
        self.pre_position_data_id = current_position_data_id

    def stop(self):
        # plot the net value and the benchmark curves
        plot_df = pd.concat([pd.Series(self.value_for_plot, name="net value").to_frame(), self.benchmark], axis=1, join='inner')
        plot_df.to_csv('result.csv')
        self.positions_info.close()
        print("death")

def backtrader_backtest(start_date, end_date, trading_csv_name, portfolio_csv_name, bechmark_csv_name):
    # 1.backtest parameters setting:start time, end time, the assets number(backtest_length) and the benckmark data and new a cerebro
    start_date, end_date = datetime.datetime.strptime(start_date, "%Y-%m-%d"), datetime.datetime.strptime(end_date, "%Y-%m-%d")
    backtest_length = None
    # benchmark series
    if bechmark_csv_name:
        benchmark = pd.read_csv(bechmark_csv_name, date_parser=True, dtype={'date': str})
        benchmark['date'] = benchmark['date'].apply(lambda x: datetime.datetime.strptime(x, "%Y-%m-%d"))
        benchmark = benchmark.set_index('date')
    else:
        benchmark = None

    # result_df = pd.DataFrame()
    cerebro = bt.Cerebro()

    # 2.get required trading data
    # 2.1 get trading data(total trading data)
    trading_data_df = pd.read_csv(trading_csv_name, dtype={'sigdate': str, 'secucode': str})
    trading_data_df.rename(columns={'sigdate': 'tradingdate', 'secucode': 'ticker'}, inplace=True)
    transer = lambda x: datetime.datetime.strptime(x, "%Y-%m-%d")
    trading_data_df['tradingdate'] = trading_data_df['tradingdate'].apply(transer)
    trading_data_df = trading_data_df[(trading_data_df['tradingdate'] > start_date) & (trading_data_df['tradingdate'] < end_date)]
    trading_data_df['openinterest'] = 0
    trading_data_df = trading_data_df.set_index('tradingdate')
    # 2.2 get target portfolio data for the target assets filter
    adj_df = pd.read_csv(portfolio_csv_name, dtype={'secucode': str})
    adj_df = adj_df[['sigdate', 'secucode', 'hl_weight']]

    parser1 = lambda x: datetime.datetime.strptime(x, "%Y/%m/%d")
    parser2 = lambda x: x.strftime("%Y-%m-%d")
    adj_df['sigdate'] = adj_df['sigdate'].apply(parser1)
    adj_df = adj_df[(adj_df['sigdate'] > start_date) & (adj_df['sigdate'] < end_date)]

    adj_df['sigdate'] = adj_df['sigdate'].apply(parser2)
    # 2.3 generate two diction to trans between secId and backtrader id
    sec_id_list = adj_df['secucode'].drop_duplicates().tolist()
    data_id_list = [i for i in range(len(sec_id_list))]

    dataId_to_secId_dict = dict(zip(data_id_list, sec_id_list))
    secId_to_dataId_dict = dict(zip(sec_id_list, data_id_list))

    print("total stocks' number", len(sec_id_list), '.', 'data feeding......')
    # 2.4 feed required datafeed and add them to the cerebro
    for index, sec_id in enumerate(sec_id_list[0:backtest_length]):
        sec_df = trading_data_df[trading_data_df['ticker'] == sec_id]
        # sec_df = sec_df.set_index('tradingdate')
        sec_raw_start = sec_df.index[0]
        if sec_raw_start != start_date.strftime("%Y-%m-%d"):
            na_fill_value = sec_df.head(1)['open'].values[0]
            df_temp = pd.DataFrame(index=pd.date_range(start=start_date, end=sec_raw_start, freq='D')[:-1],
                                    columns=['open', 'high', 'low', 'close', 'volume', 'openinterest']
                                   ).fillna(na_fill_value)
            frames = [df_temp, sec_df]
            sec_df = pd.concat(frames)
        data_feed = bt.feeds.PandasData(dataname=sec_df,
                                        fromdate=start_date,
                                        todate=end_date
                                        )
        cerebro.adddata(data_feed, name=sec_id)
    print('data feed finish!')
    # 3.cerebero config
    cerebro.addstrategy(AlphaPortfolioStrategy,
                        dataId_to_secId_dict, secId_to_dataId_dict, adj_df, end_date.strftime("%Y-%m-%d"), backtest_length, benchmark)
    cerebro.broker.setcash(20000000.0)
    # cerebro.broker.setcash(10000000.0)

    cerebro.broker.setcommission(commission=0.0008)
    cerebro.broker.set_slippage_fixed(0.02)
    cerebro.addanalyzer(bt.analyzers.Returns, _name="Returns")
    cerebro.addanalyzer(bt.analyzers.SharpeRatio, _name='SharpeRatio', riskfreerate=0.00, stddev_sample=True, annualize=True)
    cerebro.addanalyzer(bt.analyzers.AnnualReturn, _name='AnnualReturn')
    cerebro.addanalyzer(bt.analyzers.DrawDown, _name='DW')
    cerebro.addanalyzer(bt.analyzers.TradeAnalyzer, _name='TradeAnalyzer')
    ### information print
    start_cash = (cerebro.broker.getvalue() - 10000000.0)
    # start_cash = (cerebro.broker.getvalue())

    # print('Starting Portfolio Value: %.2f' % (cerebro.broker.getvalue() - 10000000.0))
    print("start cerebro.run()")
    results = cerebro.run()  # run the cerebro
    # 4. show the result
    strat = results[0]
    final_value = (cerebro.broker.getvalue() - 10000000.0)
    total_return = ((cerebro.broker.getvalue() - 10000000.0) / 10000000.0 - 1)
    # final_value = (cerebro.broker.getvalue())
    # total_return = ((cerebro.broker.getvalue()) / 10000000.0 - 1)

    sharpe_ratio = strat.analyzers.SharpeRatio.get_analysis()['sharperatio']
    max_drowdown = strat.analyzers.DW.get_analysis()['max']['drawdown']
    max_drowdown_money = strat.analyzers.DW.get_analysis()['max']['moneydown']
    trade_info = strat.analyzers.TradeAnalyzer.get_analysis()
    return_dict = {'start_cash': start_cash, 'final_value': final_value, 'total_return': total_return,\
                   'sharpe_ratio': sharpe_ratio, 'max_drowdown': max_drowdown, 'max_drowdown_money': max_drowdown_money,\
                   'trade_info': trade_info}
    return return_dict
```

        基本上，代码的注释很全面了，基本实现了涨跌停不能买入，进入排队等待，当日开盘价买入，滑点设置等等这些功能。Backtrader有一点，可能是为了加快速度，特别不方便，就是datafeed不能按照名字来实现查找，而是用index来寻找，所以我们需要建立一个全局的dict，能够实现data id到股票代码以及反过来的一一对应的dict。别的，其实实现起来还是比较方便的，但是性能有待提高，等后续需要继续启动这个项目的时候，笔者继续努力吧。譬如如何修改一个查找的逻辑和数据类型的使用。


PS.文章中提到的port_two_year.csv文件其实就是目标组合文件，格式大概是：

sigdate,secucode,weight

2019-01-07,000001,0.5

2019-01-07,000003,0.2

.......



