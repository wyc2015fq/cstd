# 量化策略系列教程：12Boll指标策略 - 知乎
# 



**小哥每天一唠叨~用掘金、用掘金、用掘金**[证经社量化社区 - 证经社](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php)**~~有没有觉得小哥的策略跑的收益率很高呀！可以把你们的回测图给小哥发过来，看着都觉得自己牛×！哈哈**

**1.策略原理及逻辑**

**1.1策略原理                                                                                                                                                           **

布林线指标，即BOLL指标，其英文全称是“Bollinger Bands”，布林线(BOLL)由约翰·布林先生创造，其利用统计原理，求出[股价](https://link.zhihu.com/?target=https%3A//baike.baidu.com/item/%25E8%2582%25A1%25E4%25BB%25B7)的标准差及其信赖区间，从而确定股价的波动范围及未来走势，利用波带显示股价的安全高低价位，因而也被称为布林带。其上下限范围不固定，随股价的滚动而变化。布林指标和麦克指标MIKE一样同属路径指标，股价波动在上限和下限的区间之内，这条带状区的宽窄，随着股价波动幅度的大小而变化，股价[涨跌幅度](https://link.zhihu.com/?target=https%3A//baike.baidu.com/item/%25E6%25B6%25A8%25E8%25B7%258C%25E5%25B9%2585%25E5%25BA%25A6)加大时，带状区变宽，涨跌幅度狭小盘整时，带状区则变窄。



![](https://pic1.zhimg.com/v2-29c8bc20c6590304d9416a3204e11384_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2064' height='1056'></svg>)
**计算公式**

中轨线=N日的移动平均线

上轨线=中轨线+两倍的标准差

下轨线=中轨线－两倍的标准差

（1）计算MA

MA=N日内的收盘价之和÷N

（2）计算标准差MD

MD=平方根（N-1）日的（C－MA）的两次方之和除以N

（3）计算MB、UP、DN线

MB=（N－1）日的MA

UP=MB+k×MD

DN=MB－k×MD

（K为参数，可根据股票的特性来做相应的调整，一般默认为2）

**指标表示**

在股市分析软件中，BOLL指标一共由四条线组成，即上轨线UP 、中轨线MB、下轨线DN和价格线。其中上轨线UP是UP数值的连线；中轨线MB是MB数值的连线；下轨线DN是DN数值的连线；在实战中，投资者不需要进行BOLL指标的计算，主要是了解BOLL的计算方法和过程，以便更加深入地掌握BOLL指标的实质，为运用指标打下基础。

**布林线有四个主要功能**

　　（1）布林线可以指示支撑和压力位置；

　　（2）布林线可以显示超买、超卖；

　　（3）布林线可以指示趋势；

　　（4）布林线具备通道功能。

　　布林线的理论使用原则是：当股价穿越最外面的压力线（支撑线）时,表示卖点（买点）出现。当股价延着压力线（支撑线）上升（下降）运行,虽然股价并未穿越,但若回头突破第二条线即是卖点或买点。

**1.2策略逻辑**
- 当布林线的上、中、下轨线同时向上运行时，且当前价格高于中轨的价格，则买入。
- 当布林线的上、中、下轨线同时向下运行时，或者当前价格低于中轨的价格，则卖出。 

**2. 代码解读：**

**   2.1配置文件【boll_stock.ini】（提示ini配置文件，需要保存成UTF8格式）**


```
[strategy]
username=
password=
;回测模式
mode=4
td_addr=localhost:8001
strategy_id=
;订阅代码注意及时更新
subscribe_symbols=

[backtest]
start_time=2016-03-01 09:00:00
end_time=2016-12-18 15:00:00

;策略初始资金
initial_cash=1000000

;委托量成交比率，默认=1（每个委托100%成交）
transaction_ratio=1

;手续费率，默认=0（不计算手续费）
commission_ratio=0.0003

;滑点比率，默认=0（无滑点）
slippage_ratio=0.00246

;行情复权模式,0=不复权,1=前复权
price_type=1

;基准
bench_symbol=SHSE.000903

[para]
;数据订阅周期
bar_type=86400

;boll指标参数

;boll周期
boll_period=22

;上下轨偏差系数
nbdev_up=2
nbdev_down=2

ma_type=0

#止盈止损
;是否固定止盈止损
is_fixation_stop=0
;是否移动止盈
is_movement_stop=1

;移动盈利开始比率及固定盈利比率
stop_fixation_profit=0.2
;亏损比率
stop_fixation_loss=0.068

;移动止盈比率
stop_movement_profit=0.068

;累计开仓距离当前的最大交易日
;若开仓距今超过这个日期，则认为未开过仓
open_max_days=22

;历史数据长度
hist_size=30

;开仓量
open_vol=2000
```


**2.2策略文件【boll_stock.py】**


```
#!/usr/bin/env python
# encoding: utf-8

import sys
import logging
import logging.config
import configparser
import csv
import numpy as np
import datetime
import talib
import arrow
from gmsdk import *

EPS = 1e-6
INIT_CLOSE_PRICE = 0


class BOLL_STOCK(StrategyBase):
    cls_config = None
    cls_user_name = None
    cls_password = None
    cls_mode = None
    cls_td_addr = None
    cls_strategy_id = None
    cls_subscribe_symbols = None
    cls_stock_pool = []

    cls_backtest_start = None
    cls_backtest_end = None
    cls_initial_cash = 1000000
    cls_transaction_ratio = 1
    cls_commission_ratio = 0.0
    cls_slippage_ratio = 0.0
    cls_price_type = 1
    cls_bench_symbol = None

    def __init__(self, *args, **kwargs):
        super(BOLL_STOCK, self).__init__(*args, **kwargs)
        self.cur_date = None
        self.dict_close = {}
        self.dict_open_close_signal = {}
        self.dict_entry_high_low = {}
        self.dict_last_factor = {}
        self.dict_open_cum_days = {}

    @classmethod
    def read_ini(cls, ini_name):
        """
        功能：读取策略配置文件
        """
        cls.cls_config = configparser.ConfigParser()
        cls.cls_config.read(ini_name)

    @classmethod
    def get_strategy_conf(cls):
        """
        功能：读取策略配置文件strategy段落的值
        """
        if cls.cls_config is None:
            return

        cls.cls_user_name = cls.cls_config.get('strategy', 'username')
        cls.cls_password = cls.cls_config.get('strategy', 'password')
        cls.cls_strategy_id = cls.cls_config.get('strategy', 'strategy_id')
        cls.cls_subscribe_symbols = cls.cls_config.get('strategy', 'subscribe_symbols')
        cls.cls_mode = cls.cls_config.getint('strategy', 'mode')
        cls.cls_td_addr = cls.cls_config.get('strategy', 'td_addr')
        if len(cls.cls_subscribe_symbols) <= 0:
            cls.get_subscribe_stock()
        else:
            subscribe_ls = cls.cls_subscribe_symbols.split(',')
            for data in subscribe_ls:
                index1 = data.find('.')
                index2 = data.find('.', index1 + 1, -1)
                cls.cls_stock_pool.append(data[:index2])

        return

    @classmethod
    def get_backtest_conf(cls):
        """
        功能：读取策略配置文件backtest段落的值
        """
        if cls.cls_config is None:
            return

        cls.cls_backtest_start = cls.cls_config.get('backtest', 'start_time')
        cls.cls_backtest_end = cls.cls_config.get('backtest', 'end_time')
        cls.cls_initial_cash = cls.cls_config.getfloat('backtest', 'initial_cash')
        cls.cls_transaction_ratio = cls.cls_config.getfloat('backtest', 'transaction_ratio')
        cls.cls_commission_ratio = cls.cls_config.getfloat('backtest', 'commission_ratio')
        cls.cls_slippage_ratio = cls.cls_config.getfloat('backtest', 'slippage_ratio')
        cls.cls_price_type = cls.cls_config.getint('backtest', 'price_type')
        cls.cls_bench_symbol = cls.cls_config.get('backtest', 'bench_symbol')

        return

    @classmethod
    def get_stock_pool(cls, csv_file):
        """
        功能：获取股票池中的代码
        """
        csvfile = open(csv_file, 'r')
        reader = csv.reader(csvfile)
        for line in reader:
            cls.cls_stock_pool.append(line[0])

        return

    @classmethod
    def get_subscribe_stock(cls):
        """
        功能：获取订阅代码
        """
        cls.get_stock_pool('stock_pool.csv')
        bar_type = cls.cls_config.getint('para', 'bar_type')
        if 86400 == bar_type:
            bar_type_str = '.bar.' + 'daily'
        else:
            bar_type_str = '.bar.' + '%d' % cls.cls_config.getint('para', 'bar_type')

        cls.cls_subscribe_symbols = ','.join(data + bar_type_str for data in cls.cls_stock_pool)
        return

    def utc_strtime(self, utc_time):
        """
        功能：utc转字符串时间
        """
        str_time = '%s' % arrow.get(utc_time).to('local')
        str_time.replace('T', ' ')
        str_time = str_time.replace('T', ' ')
        return str_time[:19]

    def get_para_conf(self):
        """
        功能：读取策略配置文件para(自定义参数)段落的值
        """
        if self.cls_config is None:
            return

        self.boll_period = self.cls_config.getint('para', 'boll_period')
        self.nbdev_up = self.cls_config.getfloat('para', 'nbdev_up')
        self.nbdev_down = self.cls_config.getfloat('para', 'nbdev_down')
        self.ma_type = self.cls_config.getint('para', 'ma_type')

        self.hist_size = self.cls_config.getint('para', 'hist_size')
        self.open_vol = self.cls_config.getint('para', 'open_vol')
        self.open_max_days = self.cls_config.getint('para', 'open_max_days')

        self.is_fixation_stop = self.cls_config.getint('para', 'is_fixation_stop')
        self.is_movement_stop = self.cls_config.getint('para', 'is_movement_stop')

        self.stop_fixation_profit = self.cls_config.getfloat('para', 'stop_fixation_profit')
        self.stop_fixation_loss = self.cls_config.getfloat('para', 'stop_fixation_loss')

        self.stop_movement_profit = self.cls_config.getfloat('para', 'stop_movement_profit')

        return

    def init_strategy(self):
        """
        功能：策略启动初始化操作
        """
        if self.cls_mode == gm.MD_MODE_PLAYBACK:
            self.cur_date = self.cls_backtest_start
            self.end_date = self.cls_backtest_end
        else:
            self.cur_date = datetime.date.today().strftime('%Y-%m-%d') + ' 08:00:00'
            self.end_date = datetime.date.today().strftime('%Y-%m-%d') + ' 16:00:00'

        self.dict_open_close_signal = {}
        self.dict_entry_high_low = {}
        self.get_last_factor()
        self.init_data()
        self.init_entry_high_low()
        return

    def init_data(self):
        """
        功能：获取订阅代码的初始化数据
        """
        for ticker in self.cls_stock_pool:
            # 初始化仓位操作信号字典
            self.dict_open_close_signal.setdefault(ticker, False)

            daily_bars = self.get_last_n_dailybars(ticker, self.hist_size - 1, self.cur_date)
            if len(daily_bars) <= 0:
                continue

            end_daily_bars = self.get_last_n_dailybars(ticker, 1, self.end_date)
            if len(end_daily_bars) <= 0:
                continue

            if ticker not in self.dict_last_factor:
                continue

            end_adj_factor = self.dict_last_factor[ticker]
            cp_ls = [data.close * data.adj_factor / end_adj_factor for data in daily_bars]
            cp_ls.reverse()

            # 留出一个空位存储当天的一笔数据
            cp_ls.append(INIT_CLOSE_PRICE)
            close = np.asarray(cp_ls, dtype=np.float)

            # 存储历史的close
            self.dict_close.setdefault(ticker, close)

            # end = time.clock()
            # logging.info('init_data cost time: %f s' % (end - start))

    def init_data_newday(self):
        """
        功能：新的一天初始化数据
        """
        # 新的一天，去掉第一笔数据,并留出一个空位存储当天的一笔数据
        for key in self.dict_close:
            if len(self.dict_close[key]) >= self.hist_size and abs(self.dict_close[key][-1] - INIT_CLOSE_PRICE) > EPS:
                self.dict_close[key] = np.append(self.dict_close[key][1:], INIT_CLOSE_PRICE)
            elif len(self.dict_close[key]) < self.hist_size and abs(self.dict_close[key][-1] - INIT_CLOSE_PRICE) > EPS:
                self.dict_close[key] = np.append(self.dict_close[key][:], INIT_CLOSE_PRICE)

        # 初始化仓位操作信号字典
        for key in self.dict_open_close_signal:
            self.dict_open_close_signal[key] = False

            # 开仓后到当前的交易日天数
        keys = list(self.dict_open_cum_days.keys())
        for key in keys:
            if self.dict_open_cum_days[key] >= self.open_max_days:
                del self.dict_open_cum_days[key]
            else:
                self.dict_open_cum_days[key] += 1

    def get_last_factor(self):
        """
        功能：获取指定日期最新的复权因子
        """
        for ticker in self.cls_stock_pool:
            daily_bars = self.get_last_n_dailybars(ticker, 1, self.end_date)
            if daily_bars is not None and len(daily_bars) > 0:
                self.dict_last_factor.setdefault(ticker, daily_bars[0].adj_factor)

    def init_entry_high_low(self):
        """
        功能：获取进场后的最高价和最低价,仿真或实盘交易启动时加载
        """
        pos_list = self.get_positions()
        high_list = []
        low_list = []
        for pos in pos_list:
            symbol = pos.exchange + '.' + pos.sec_id
            init_time = self.utc_strtime(pos.init_time)

            cur_time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')

            daily_bars = self.get_dailybars(symbol, init_time, cur_time)

            high_list = [bar.high for bar in daily_bars]
            low_list = [bar.low for bar in daily_bars]

            if len(high_list) > 0:
                highest = np.max(high_list)
            else:
                highest = pos.vwap

            if len(low_list) > 0:
                lowest = np.min(low_list)
            else:
                lowest = pos.vwap

            self.dict_entry_high_low.setdefault(symbol, [highest, lowest])

    def on_bar(self, bar):
        if self.cls_mode == gm.MD_MODE_PLAYBACK:
            if bar.strtime[0:10] != self.cur_date[0:10]:
                self.cur_date = bar.strtime[0:10] + ' 08:00:00'
                # 新的交易日
                self.init_data_newday()

        symbol = bar.exchange + '.' + bar.sec_id

        self.movement_stop_profit_loss(bar)
        self.fixation_stop_profit_loss(bar)

        # 填充价格
        if symbol in self.dict_close:
            self.dict_close[symbol][-1] = bar.close

        pos = self.get_position(bar.exchange, bar.sec_id, OrderSide_Bid)

        if self.dict_open_close_signal[symbol] is False:
            # 当天未有对该代码开、平仓
            if symbol in self.dict_close:
                upper, middle, lower = talib.BBANDS(self.dict_close[symbol], timeperiod=self.boll_period,
                                                    nbdevup=self.nbdev_up, nbdevdn=self.nbdev_down, matype=self.ma_type)

                if pos is None and symbol not in self.dict_open_cum_days \
                        and (upper[-1] > upper[-2] and upper[-2] > upper[-3] \
                                     and middle[-1] > middle[-2] and middle[-2] > middle[-3] \
                                     and lower[-1] > lower[-2] and lower[-2] > lower[-3] \
                                     and bar.close > middle[-1]):
                    # 有开仓机会则设置已开仓的交易天数
                    self.dict_open_cum_days[symbol] = 0

                    cash = self.get_cash()
                    cur_open_vol = self.open_vol
                    if cash.available / bar.close > self.open_vol:
                        cur_open_vol = self.open_vol
                    else:
                        cur_open_vol = int(cash.available / bar.close / 100) * 100

                    if cur_open_vol == 0:
                        print('no available cash to buy, available cash: %.2f' % cash.available)
                    else:
                        # 上、中、下轨同时向上运行
                        self.open_long(bar.exchange, bar.sec_id, bar.close, cur_open_vol)
                        self.dict_open_close_signal[symbol] = True
                        logging.info('open long, symbol:%s, time:%s, price:%.2f' % (symbol, bar.strtime, bar.close))
                elif pos is not None and (upper[-1] < upper[-2] and middle[-1] < middle[-2] and lower[-1] < lower[-2] \
                                                  or bar.close < middle[-1]):
                    # 上、中、下轨线同时向下运行时
                    vol = pos.volume - pos.volume_today
                    if vol > 0:
                        self.close_long(bar.exchange, bar.sec_id, bar.close, vol)
                        self.dict_open_close_signal[symbol] = True
                        logging.info('close long, symbol:%s, time:%s, price:%.2f' % (symbol, bar.strtime, bar.close))
                        # print 'close long, symbol:%s, time:%s '%(symbol, bar.strtime)

    def on_order_filled(self, order):
        symbol = order.exchange + '.' + order.sec_id
        if order.position_effect == PositionEffect_CloseYesterday \
                and order.side == OrderSide_Bid:
            pos = self.get_position(order.exchange, order.sec_id, order.side)
            if pos is None and self.is_movement_stop == 1:
                self.dict_entry_high_low.pop(symbol)

    def fixation_stop_profit_loss(self, bar):
        """
        功能：固定止盈、止损,盈利或亏损超过了设置的比率则执行止盈、止损
        """
        if self.is_fixation_stop == 0:
            return

        symbol = bar.exchange + '.' + bar.sec_id
        pos = self.get_position(bar.exchange, bar.sec_id, OrderSide_Bid)
        if pos is not None:
            if pos.fpnl > 0 and pos.fpnl / pos.cost >= self.stop_fixation_profit:
                self.close_long(bar.exchange, bar.sec_id, 0, pos.volume - pos.volume_today)
                self.dict_open_close_signal[symbol] = True
                logging.info(
                    'fixnation stop profit: close long, symbol:%s, time:%s, price:%.2f, vwap: %s, volume:%s' % (symbol,
                                                                                                                bar.strtime,
                                                                                                                bar.close,
                                                                                                                pos.vwap,
                                                                                                                pos.volume))
            elif pos.fpnl < 0 and pos.fpnl / pos.cost <= -1 * self.stop_fixation_loss:
                self.close_long(bar.exchange, bar.sec_id, 0, pos.volume - pos.volume_today)
                self.dict_open_close_signal[symbol] = True
                logging.info(
                    'fixnation stop loss: close long, symbol:%s, time:%s, price:%.2f, vwap:%s, volume:%s' % (symbol,
                                                                                                             bar.strtime,
                                                                                                             bar.close,
                                                                                                             pos.vwap,
                                                                                                             pos.volume))

    def movement_stop_profit_loss(self, bar):
        """
        功能：移动止盈, 移动止盈止损按进场后的最高价乘以设置的比率与当前价格相比，
              并且盈利比率达到设定的盈亏比率时，执行止盈
        """
        if self.is_movement_stop == 0:
            return

        entry_high = None
        entry_low = None
        pos = self.get_position(bar.exchange, bar.sec_id, OrderSide_Bid)
        symbol = bar.exchange + '.' + bar.sec_id

        is_stop_profit = True

        if pos is not None and pos.volume > 0:
            if symbol in self.dict_entry_high_low:
                if self.dict_entry_high_low[symbol][0] < bar.close:
                    self.dict_entry_high_low[symbol][0] = bar.close
                    is_stop_profit = False
                if self.dict_entry_high_low[symbol][1] > bar.close:
                    self.dict_entry_high_low[symbol][1] = bar.close
                [entry_high, entry_low] = self.dict_entry_high_low[symbol]

            else:
                self.dict_entry_high_low.setdefault(symbol, [bar.close, bar.close])
                [entry_high, entry_low] = self.dict_entry_high_low[symbol]
                is_stop_profit = False

            if is_stop_profit:
                # 移动止盈
                if bar.close <= (
                    1 - self.stop_movement_profit) * entry_high and pos.fpnl / pos.cost >= self.stop_fixation_profit:
                    if pos.volume - pos.volume_today > 0:
                        self.close_long(bar.exchange, bar.sec_id, 0, pos.volume - pos.volume_today)
                        self.dict_open_close_signal[symbol] = True
                        logging.info(
                            'movement stop profit: close long, symbol:%s, time:%s, price:%.2f, vwap:%.2f, volume:%s' % (
                            symbol,
                            bar.strtime, bar.close, pos.vwap, pos.volume))

                        # 止损
            if pos.fpnl < 0 and pos.fpnl / pos.cost <= -1 * self.stop_fixation_loss:
                self.close_long(bar.exchange, bar.sec_id, 0, pos.volume - pos.volume_today)
                self.dict_open_close_signal[symbol] = True
                logging.info(
                    'movement stop loss: close long, symbol:%s, time:%s, price:%.2f, vwap:%.2f, volume:%s' % (symbol,
                                                                                                              bar.strtime,
                                                                                                              bar.close,
                                                                                                              pos.vwap,
                                                                                                              pos.volume))


if __name__ == '__main__':
    print(get_version())
    logging.config.fileConfig('logging.ini')
    BOLL_STOCK.read_ini('boll_stock.ini')
    BOLL_STOCK.get_strategy_conf()

    boll_stock = BOLL_STOCK(username=BOLL_STOCK.cls_user_name,
                            password=BOLL_STOCK.cls_password,
                            strategy_id=BOLL_STOCK.cls_strategy_id,
                            subscribe_symbols=BOLL_STOCK.cls_subscribe_symbols,
                            mode=BOLL_STOCK.cls_mode,
                            td_addr=BOLL_STOCK.cls_td_addr)

    if BOLL_STOCK.cls_mode == gm.MD_MODE_PLAYBACK:
        BOLL_STOCK.get_backtest_conf()
        ret = boll_stock.backtest_config(start_time=BOLL_STOCK.cls_backtest_start,
                                         end_time=BOLL_STOCK.cls_backtest_end,
                                         initial_cash=BOLL_STOCK.cls_initial_cash,
                                         transaction_ratio=BOLL_STOCK.cls_transaction_ratio,
                                         commission_ratio=BOLL_STOCK.cls_commission_ratio,
                                         slippage_ratio=BOLL_STOCK.cls_slippage_ratio,
                                         price_type=BOLL_STOCK.cls_price_type,
                                         bench_symbol=BOLL_STOCK.cls_bench_symbol)

    boll_stock.get_para_conf()
    boll_stock.init_strategy()
    ret = boll_stock.run()

print('run result %s' % ret)
```


**如果想了解相关的python函数和掘金接口函数，走下方通道：**

[http://zjshe.cn/q/forum.php?mod=viewthread&tid=60&extra=page%3D1](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php%3Fmod%3Dviewthread%26tid%3D60%26extra%3Dpage%253D1)

**有不了解的可以给小编留言哦，小编会细心为大家解答~**


