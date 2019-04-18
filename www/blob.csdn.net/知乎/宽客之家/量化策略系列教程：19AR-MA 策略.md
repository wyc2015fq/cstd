# 量化策略系列教程：19AR-MA 策略 - 知乎
# 



**今天上个新策略~[证经社量化社区 - 证经社](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php)**




**1.策略原理及逻辑**

** 1.1策略原理**

人气指标（AR）和意愿指标（BR）都是以分析历史股价为手段的技术指标，其中人气指标较重视开盘价格，从而反映市场买卖的人气；意愿指标则重视收盘价格，反映的是市场买卖意愿的程度，两项指标分别从不同角度股价波动进行分析，达到追踪股价未来动向的共同目的。

　　人气指标是以当天开市价为基础，即以当天市价分别比较当天最高，最低价，通过一定时期内开市价在股价中的地位，反映市场买卖人气。

**计算公式：**




　　其中：H＝当日最高价；L＝当日最低价；O＝当日开市价

　　N为公式中的设定参数，一般设定为26日。

**人气指标的基本应用法则：**

（1）AR值以100为中心地带，其±20之间，即AR值在80－120之间波动时，属盘整行情，股价走势比较平稳，不会出现剧烈波动。

（2）AR值走高时表示行情活跃，人气旺盛，过高则表示股价进入高价，应选择时机退出，AR值的高度没有具体标准，一般情况下，AR值上升至150以上时，股价随时可能回档下跌。

（3）AR值走低时表示人气衰退，需要充实，过低则暗示股价可能跌入低谷，可考虑伺机介入，一般AR值跌至70以下时，股价有可能随时反弹上升。

（4）从AR曲线可以看出一段时期的买卖气势，并具有先于股价到达峰或跌入谷底的功能，观图时主要凭借经验，此策略配合MA一同使用。

（5）MA:在上升行情进入稳定期，短周期、中周期、长周期移动平均线从上而下依次顺序排列，向右上方移动

在下跌行情中，短周期、中周期、长周期移动平均线自下而上依次顺序排列，向右下方移动，称为空头                        排列，预示股价将大幅下跌。

** 1.2策略逻辑**
- AR值<75,且MA(5)>MA(10)>MA(30)，买入
- AR值>130,且MA(5)<MA(10)<MA(30)，卖出 




**2.策略代码**

**2.1配置文件【ar_ma_stock.ini】（提示ini配置文件，需要保存成UTF8格式）**


```
[strategy]
username= 
password= 
;回测模式
mode=4
td_addr=localhost:8001
strategy_id= 
subscribe_symbols=

[backtest]
start_time=2015-03-01 09:00:00
end_time=2017-08-07 15:00:00

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
bench_symbol=SHSE.000300

[para]
;数据订阅周期
bar_type=86400

;AR周期
ar_period=26

;AR人气指标
ar_upr=130
ar_dwn=75


;MA周期
short_period=5
mid_period=10
long_period=30

#止盈止损
;是否固定止盈止损
is_fixation_stop=0
;是否移动止盈
is_movement_stop=1

;移动盈利开始比率及固定盈利比率
stop_fixation_profit=0.25
;亏损比率
stop_fixation_loss=0.068

;移动止盈比率
stop_movement_profit=0.068


;累计开仓距离当前的最大交易日
;若开仓距今超过这个日期，则认为未开过仓
open_max_days=22

;历史数据长度
hist_size=60

;开仓量
open_vol=2000

##############################################################
# logger settings
##############################################################
[loggers]
keys=root

[logger_root]
level=INFO
handlers=file

[handlers]
keys=file

[handler_file]
class=handlers.RotatingFileHandler
args=('ar_ma_stock.log','a',1000,5)
formatter=simple

[handler_console]
class=StreamHandler
args = (sys.stdout,)
formatter=simple

[formatters]
keys = simple

[formatter_simple]
format=%(asctime)s - %(name)s - %(levelname)s - %(message)s
datefmt=
```





**2.2策略文件【ar_ma_stock.py】**


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
'''
##人气指标（AR）介绍

人气指标是以当天开市价为基础，即以当天市价分别比较当天最高、最低价，通过一定时期内开市价在股价中的地位，反映市场买卖人气。

其计算公式如下：
AR=N日内（当日最高价—当日开市价）之和 / N日内（当日开市价—当日最低价）之和
N为公式中的设定参数，一般设定为26日。
使用法则
（1）AR值以100为中心地带，其±20之间，即AR值在80－120之间波动时，属于盘整行情，股价走势比较平稳，不会出现剧烈波动。
（2）AR值走高时表示行情活跃，人气旺盛，过高则表示股价进入高价，应选择时机退出，AR值的高度没有具体标准，一般情况下，AR值上升至150以上时，股价随时可能回档下跌。
（3）AR值走低时表示人气衰退，需要充实，过低则暗示股价可能跌入低谷，可考虑伺机介入，一般AR值跌至70以下时，股价有可能随时反弹上升。
（4）从AR曲线可以看出一段时期的买卖气势，并具有先于股价到达峰或跌入谷底的功能， 结合MA指标一起使用。
MA:在上升行情进入稳定期，短周期、中周期、长周期移动平均线从上而下依次顺序排列，向右上方移动
   在下跌行情中，短周期、中周期、长周期移动平均线自下而上依次顺序排列，向右下方移动，称为空头排列，预示股价将大幅下跌。
'''
EPS = 1e-6
INIT_LOW_PRICE = 10000000.0
INIT_HIGH_PRICE = -1.0
INIT_CLOSE_PRICE = 0.0
INIT_OPEN_PRICE = 0.0


class AR_MA_STOCK(StrategyBase):
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
        super(AR_MA_STOCK, self).__init__(*args, **kwargs)
        self.cur_date = None
        self.dict_price = {}
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

        self.ar_period = self.cls_config.getint('para', 'ar_period')
        self.ar_upr = self.cls_config.getint('para', 'ar_upr')
        self.ar_dwn = self.cls_config.getint('para', 'ar_dwn')

        self.short_period = self.cls_config.getint('para', 'short_period')
        self.mid_period = self.cls_config.getint('para', 'mid_period')
        self.long_period = self.cls_config.getint('para', 'long_period')

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
            # 初始化开仓操作信号字典
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
            open_ls = [data.open * data.adj_factor / end_adj_factor for data in daily_bars]
            open_ls.reverse()
            high_ls = [data.high * data.adj_factor / end_adj_factor for data in daily_bars]
            high_ls.reverse()
            low_ls = [data.low * data.adj_factor / end_adj_factor for data in daily_bars]
            low_ls.reverse()
            cp_ls = [data.close * data.adj_factor / end_adj_factor for data in daily_bars]
            cp_ls.reverse()

            # 留出一个空位存储当天的一笔数据
            open_ls.append(INIT_OPEN_PRICE)
            open = np.asarray(open_ls, dtype=np.float)
            high_ls.append(INIT_HIGH_PRICE)
            high = np.asarray(high_ls, dtype=np.float)
            low_ls.append(INIT_LOW_PRICE)
            low = np.asarray(low_ls, dtype=np.float)
            cp_ls.append(INIT_CLOSE_PRICE)
            close = np.asarray(cp_ls, dtype=np.float)

            # 存储历史的open high low close
            self.dict_price.setdefault(ticker, [open, high, low, close])

    def init_data_newday(self):
        """
        功能：新的一天初始化数据
        """
        # 新的一天，去掉第一笔数据,并留出一个空位存储当天的一笔数据
        for key in self.dict_price:
            if len(self.dict_price[key][0]) >= self.hist_size and self.dict_price[key][0][-1] - INIT_OPEN_PRICE > EPS:
                self.dict_price[key][0] = np.append(self.dict_price[key][0][1:], INIT_OPEN_PRICE)
            elif len(self.dict_price[key][0]) < self.hist_size and self.dict_price[key][0][-1] - INIT_OPEN_PRICE > EPS:
                # 未取足指标所需全部历史数据时回测过程中补充数据
                self.dict_price[key][0] = np.append(self.dict_price[key][0][:], INIT_HIGH_PRICE)

            if len(self.dict_price[key][1]) >= self.hist_size and abs(
                            self.dict_price[key][1][-1] - INIT_HIGH_PRICE) > EPS:
                self.dict_price[key][1] = np.append(self.dict_price[key][1][1:], INIT_HIGH_PRICE)
            elif len(self.dict_price[key][1]) < self.hist_size and abs(
                            self.dict_price[key][1][-1] - INIT_HIGH_PRICE) > EPS:
                self.dict_price[key][1] = np.append(self.dict_price[key][1][:], INIT_HIGH_PRICE)

            if len(self.dict_price[key][2]) >= self.hist_size and abs(
                            self.dict_price[key][2][-1] - INIT_LOW_PRICE) > EPS:
                self.dict_price[key][2] = np.append(self.dict_price[key][2][1:], INIT_LOW_PRICE)
            elif len(self.dict_price[key][2]) < self.hist_size and abs(
                            self.dict_price[key][2][-1] - INIT_LOW_PRICE) > EPS:
                self.dict_price[key][2] = np.append(self.dict_price[key][2][:], INIT_LOW_PRICE)

            if len(self.dict_price[key][3]) >= self.hist_size and abs(
                            self.dict_price[key][3][-1] - INIT_CLOSE_PRICE) > EPS:
                self.dict_price[key][3] = np.append(self.dict_price[key][3][1:], INIT_CLOSE_PRICE)
            elif len(self.dict_price[key][3]) < self.hist_size and abs(
                            self.dict_price[key][3][-1] - INIT_CLOSE_PRICE) > EPS:
                self.dict_price[key][3] = np.append(self.dict_price[key][3][:], INIT_CLOSE_PRICE)

        # 初始化开仓操作信号字典
        for key in self.dict_open_close_signal:
            self.dict_open_close_signal[key] = False

        #  开仓后到当前的交易日天数
        keys = list(self.dict_open_cum_days.keys())
        for key in keys:
            if self.dict_open_cum_days[key] >= self.open_max_days:
                del self.dict_open_cum_days[key]
            else:
                self.dict_open_cum_days[key] += 1

    def cal_ar_index(self, ticker):
        """
        功能：计算ar指标
        """
        ar_index = None

        if (len(self.dict_price[ticker][0]) < self.ar_period or abs(
                    self.dict_price[ticker][0][-1] - INIT_OPEN_PRICE) < EPS) \
                or (len(self.dict_price[ticker][1]) < self.ar_period or abs(
                        self.dict_price[ticker][1][-1] - INIT_HIGH_PRICE) < EPS) \
                or (len(self.dict_price[ticker][2]) < self.ar_period or abs(
                        self.dict_price[ticker][2][-1] - INIT_LOW_PRICE) < EPS) \
                or (len(self.dict_price[ticker][3]) < self.ar_period or abs(
                        self.dict_price[ticker][3][-1] - INIT_CLOSE_PRICE) < EPS):
            # 历史数据不足
            return ar_index

        open_ls = self.dict_price[ticker][0][len(self.dict_price[ticker][0]) - self.ar_period:]
        high_ls = self.dict_price[ticker][1][len(self.dict_price[ticker][1]) - self.ar_period:]
        low_ls = self.dict_price[ticker][2][len(self.dict_price[ticker][2]) - self.ar_period:]

        high_minus_ls = [a_b[0] - a_b[1] for a_b in zip(high_ls, open_ls)]
        low_minus_ls = [a_b1[0] - a_b1[1] for a_b1 in zip(open_ls, low_ls)]

        ar_index = 0.0
        for pos in range(len(high_minus_ls)):
            if low_minus_ls[pos] > EPS:
                ar_index += high_minus_ls[pos] / low_minus_ls[pos]

        return ar_index

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

        pos = self.get_position(bar.exchange, bar.sec_id, OrderSide_Bid)

        # 补充当天价格
        if symbol in self.dict_price:
            self.dict_price[symbol][0][-1] = bar.open

            if self.dict_price[symbol][1][-1] < bar.high:
                self.dict_price[symbol][1][-1] = bar.high

            if self.dict_price[symbol][2][-1] > bar.low:
                self.dict_price[symbol][2][-1] = bar.low

            self.dict_price[symbol][3][-1] = bar.close

        if self.dict_open_close_signal[symbol] is False:
            # 当天未有对该代码开、平仓
            if symbol in self.dict_price:

                ma_short = talib.MA(self.dict_price[symbol][3], self.short_period)
                ma_mid = talib.MA(self.dict_price[symbol][3], self.mid_period)
                ma_long = talib.MA(self.dict_price[symbol][3], self.long_period)

                ar_index = self.cal_ar_index(symbol)
                if ar_index is not None:
                    if pos is None and symbol not in self.dict_open_cum_days \
                            and (ar_index < self.ar_dwn and (ma_short[-1] > ma_mid[-1] and ma_mid[-1] > ma_long[-1])):
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
                            self.open_long(bar.exchange, bar.sec_id, bar.close, cur_open_vol)
                            self.dict_open_close_signal[symbol] = True
                            logging.info('open long, symbol:%s, time:%s, price:%.2f' % (symbol, bar.strtime, bar.close))
                    elif pos is not None and (
                            ar_index > self.ar_upr and (ma_short[-1] < ma_mid[-1] and ma_mid[-1] < ma_long[-1])):
                        vol = pos.volume - pos.volume_today
                        if vol > 0:
                            self.close_long(bar.exchange, bar.sec_id, bar.close, vol)
                            self.dict_open_close_signal[symbol] = True
                            logging.info(
                                'close long, symbol:%s, time:%s, price:%.2f' % (symbol, bar.strtime, bar.close))
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
    logging.config.fileConfig('ar_ma_stock.ini')
    AR_MA_STOCK.read_ini('ar_ma_stock.ini')
    AR_MA_STOCK.get_strategy_conf()

    ar_ma_stock = AR_MA_STOCK(username=AR_MA_STOCK.cls_user_name,
                              password=AR_MA_STOCK.cls_password,
                              strategy_id=AR_MA_STOCK.cls_strategy_id,
                              subscribe_symbols=AR_MA_STOCK.cls_subscribe_symbols,
                              mode=AR_MA_STOCK.cls_mode,
                              td_addr=AR_MA_STOCK.cls_td_addr)

    if AR_MA_STOCK.cls_mode == gm.MD_MODE_PLAYBACK:
        AR_MA_STOCK.get_backtest_conf()
        ret = ar_ma_stock.backtest_config(start_time=AR_MA_STOCK.cls_backtest_start,
                                          end_time=AR_MA_STOCK.cls_backtest_end,
                                          initial_cash=AR_MA_STOCK.cls_initial_cash,
                                          transaction_ratio=AR_MA_STOCK.cls_transaction_ratio,
                                          commission_ratio=AR_MA_STOCK.cls_commission_ratio,
                                          slippage_ratio=AR_MA_STOCK.cls_slippage_ratio,
                                          price_type=AR_MA_STOCK.cls_price_type,
                                          bench_symbol=AR_MA_STOCK.cls_bench_symbol)

    ar_ma_stock.get_para_conf()
    ar_ma_stock.init_strategy()
    ret = ar_ma_stock.run()


print('run result %s' % ret)
```





** 2.3股票池文件【stock_pool.csv】**


```
SHSE.600000
SHSE.600010
SHSE.600011
SHSE.600015
SHSE.600016
SHSE.600018
SHSE.600019
SHSE.600023
SHSE.600028
SHSE.600030
SHSE.600031
SHSE.600036
SHSE.600048
SHSE.600050
SHSE.600104
SHSE.600111
SHSE.600115
SHSE.600150
SHSE.600276
SHSE.600340
SHSE.600372
SHSE.600398
SHSE.600485
SHSE.600518
SHSE.600519
SHSE.600585
SHSE.600637
SHSE.600690
SHSE.600705
SHSE.600795
SHSE.600837
SHSE.600886
SHSE.600887
SHSE.600893
SHSE.600900
SHSE.600958
SHSE.600959
SHSE.600999
SHSE.601006
SHSE.601018
SHSE.601088
SHSE.601111
SHSE.601166
SHSE.601169
SHSE.601186
SHSE.601211
SHSE.601225
SHSE.601288
SHSE.601318
SHSE.601328
SHSE.601336
SHSE.601377
SHSE.601390
SHSE.601398
SHSE.601600
SHSE.601601
SHSE.601618
SHSE.601628
SHSE.601633
SHSE.601668
SHSE.601669
SHSE.601688
SHSE.601727
SHSE.601766
SHSE.601788
SHSE.601800
SHSE.601808
SHSE.601818
SHSE.601857
SHSE.601898
SHSE.601899
SHSE.601901
SHSE.601939
SHSE.601985
SHSE.601988
SHSE.601989
SHSE.601998
SHSE.603288
SZSE.000001
SZSE.000002
SZSE.000063
SZSE.000069
SZSE.000166
SZSE.000333
SZSE.000538
SZSE.000625
SZSE.000651
SZSE.000725
SZSE.000776
SZSE.000858
SZSE.000895
SZSE.002024
SZSE.002252
SZSE.002304
SZSE.002415
SZSE.002594
SZSE.002736
SZSE.002739
SZSE.300059
SZSE.300104
```


**如果想了解相关的python函数和掘金接口函数，走下方通道：**

[http://zjshe.cn/q/forum.php?mod=viewthread&tid=107&extra=page%3D1](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php%3Fmod%3Dviewthread%26tid%3D107%26extra%3Dpage%253D1)

**有不了解的可以给小编留言哦，小编会细心为大家解答~**


