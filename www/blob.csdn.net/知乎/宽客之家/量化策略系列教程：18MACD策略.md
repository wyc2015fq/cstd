# 量化策略系列教程：18MACD策略 - 知乎
# 



**最近小哥有点忙，一直顾不上更新，今天会给大家更新点新东西的~[证经社量化社区 - 证经社](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php)**




**1.策略原理及逻辑**

**1.1指标定义**

**指数平滑异同移动平均线**（Moving Average Convergence / Divergence, MACD）是股票交易中一种常见的技术分析工具，由Gerald Appel于1970年代提出，用于研判股票价格变化的强度、方向、能量，以及趋势周期，以便把握股票买进和卖出的时机。MACD指标由一组曲线与图形组成，通过收盘时股价或指数的快变及慢变的指数移动平均值（EMA）之间的[差](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/%25E5%2587%258F%25E6%25B3%2595)计算出来。“快”指更短时段的EMA，而“慢”则指较长时段的EMA，最常用的是12及26日EMA。

**1.2指标计算公式**

**1).差离值（DIF值）：**

先利用收盘价的指数移动平均值（12日／26日）计算出差离值。




**2).讯号线（DEM值，又称MACD值）：**
- 计算出DIF后，会再画一条“讯号线”，通常是DIF的9日指数移动平均值。
- **〖公式〗**




**3).柱形图或棒形图（histogram / bar graph）：**
- 接着，将DIF与DEM的差画成“柱形图”（MACD bar / OSC）。
- **〖公式〗**




简写为




示例图：



![](https://pic1.zhimg.com/v2-f0916653b1d4dc7eccc26bbbbea2a3f0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='744' height='598'></svg>)
上表为收市价图表（OHLC chart），

下表的绿线是差离值（DIF），红线是讯号线（DEM），白色区块柱形图（MACD bar / OSC）是两者的差（D-M）。

**  1.3指标解读**

MACD其实就是两条指数移动平均线——EMA(12)和EMA(26)——的背离和交叉，EMA(26)可视为MACD的零轴，但是MACD呈现的讯息噪声较均线少。

MACD是一种趋势分析指标，不宜同时分析不同的市场环境。以下为三种交易讯号：
- **差离值**（**DIF值**）与**讯号线**（**DEM值**，又称**MACD值**）相交；
- 差离值与零轴相交；
- 股价与差离值的背离。 

差离值（DIF）形成“快线”，讯号线（DEM）形成“慢线”。

当差离值（DIF）从下而上穿过讯号线（DEM），为买进讯号；相反若从上而下穿越，为卖出讯号。买卖讯号可能出现频繁，需要配合其他指标（如：RSI、KD）一同分析。

**棒形图**（**MACD bar** / **Oscillator，OSC**）的作用是显示出“差离值”与“讯号线”的差，同时将两条线的走势具体化，以利判断差离值和讯号线交叉形成的买卖讯号，例如正在下降的棒形图代表两线的差值朝负的方向走，趋势向下；靠近零轴时，差离值和讯号线将相交出现买卖讯号。

棒形图会根据正负值分布在零轴（X轴）的上下。棒形图在零轴上方时表示走势较强，反之则是走势较弱。

差离值由下而上穿过零轴代表市场气氛利好股价，相反由上而下则代表利淡股价。差离值与讯号线均在零轴上方时，被称为多头市场，反之，则被称为空头市场。

当股价创新低，但MACD并没有相应创新低（牛市背离），视为利好（利多）讯息，股价跌势或将完结。相反，若股价创新高，但MACD并没有相应创新高（熊市背离），视为利淡（利空）讯息。同样地，若股价与棒形图不配合，也可作类似结论。

MACD是一种中长线的研判指标。当股市强烈震荡或股价变化巨大（如送配股拆细等）时，可能会给出错误的信号。所以在决定股票操作时，应该谨慎参考其他指标，以及市场状况，不能完全信任差离值的单一研判，避免造成损失。

**1.4策略逻辑**
- DIF、DEA均为正，DIF向上突破DEA，买入
- DIF、DEA均为负，DIF向下跌破DEA，卖出 







**2.策略代码**

** 2.1配置文件【macd_stock.ini】（提示ini配置文件，需要保存成UTF8格式）**


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
start_time=2014-03-01 09:00:00
end_time=2016-03-18 16:00:00

initial_cash=1000000

transaction_ratio=1

commission_ratio=0.0003

;滑点比率，默认=0（无滑点）
slippage_ratio=0.00246


price_type=1

bench_symbol=SHSE.000016

[para]
bar_type=86400

long_term=26
short_term=12
macd_term=9

#止盈止损
;是否固定止盈止损
is_fixation_stop=0
;是否移动止盈
is_movement_stop=1

;移动盈利开始比率及固定盈利比率
stop_fixation_profit=0.20
;亏损比率
stop_fixation_loss=0.068

;移动止盈比率
stop_movement_profit=0.068

hist_size=60

openlong_signal=2

open_vol=2000

##############################################################
# logger settings
##############################################################
[loggers]
keys=root

[logger_root]
level=INFO
handlers=console,file

[handlers]
keys=console,file

[handler_file]
class=handlers.RotatingFileHandler
args=('mack_stock.log','a',1000,5)
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


**2.2策略文件【macd_stock.py】**


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
from gmsdk import *

EPS = 1e-6
INIT_CLOSE_PRICE = 0


class MACD_STOCK(StrategyBase):
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
        super(MACD_STOCK, self).__init__(*args, **kwargs)
        self.cur_date = None
        self.dict_close = {}
        self.dict_openlong_signal = {}
        self.dict_entry_high_low = {}
        self.dict_last_factor = {}

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

        self.long_term = self.cls_config.getint('para', 'long_term')
        self.short_term = self.cls_config.getint('para', 'short_term')
        self.macd_term = self.cls_config.getint('para', 'macd_term')
        self.hist_size = self.cls_config.getint('para', 'hist_size')
        self.openlong_signal = self.cls_config.getint('para', 'openlong_signal')

        self.open_vol = self.cls_config.getint('para', 'open_vol')

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

        self.dict_openlong_signal = {}
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
            # 初始化买多信号字典
            self.dict_openlong_signal.setdefault(ticker, 0)

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

    def init_data_newday(self):
        """
        功能：新的一天初始化数据
        """
        # 新的一天，去掉第一笔数据,并留出一个空位存储当天的一笔数据
        for key in self.dict_close:
            if len(self.dict_close[key]) >= self.hist_size and abs(self.dict_close[key][-1] - INIT_CLOSE_PRICE) > EPS:
                self.dict_close[key] = np.append(self.dict_close[key][1:], INIT_CLOSE_PRICE)
            elif len(self.dict_close[key]) < self.hist_size and abs(self.dict_close[key][-1] - INIT_CLOSE_PRICE) > EPS:
                # 未取足指标所需全部历史数据时回测过程中补充数据
                self.dict_close[key] = np.append(self.dict_close[key][:], INIT_CLOSE_PRICE)

                # 初始化买多信号字典
                # for key in self.dict_openlong_signal:
                # self.dict_openlong_signal.setdefault(key, 0)

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

        if symbol in self.dict_close:
            close = self.dict_close[symbol]
            dif, dea, macd = talib.MACD(close,
                                        fastperiod=self.short_term,
                                        slowperiod=self.long_term,
                                        signalperiod=self.macd_term)
            if pos is None and (dif[-1] > EPS and dea[-1] > EPS and dif[-1] > dif[-2] and dif[-1] > dea[-1]):
                cash = self.get_cash()
                cur_open_vol = self.open_vol
                if cash.available / bar.close > self.open_vol:
                    cur_open_vol = self.open_vol
                else:
                    cur_open_vol = int(cash.available / bar.close / 100) * 100

                if cur_open_vol == 0:
                    print('no available cash to buy, available cash: %.2f' % cash.available)
                else:
                    self.dict_openlong_signal[symbol] += 1
                    if self.dict_openlong_signal[symbol] == self.openlong_signal:
                        self.open_long(bar.exchange, bar.sec_id, 0, self.open_vol)
                        pos = self.get_position(bar.exchange, bar.sec_id, OrderSide_Bid)
                        self.dict_openlong_signal[symbol] = 0
                        logging.info('open long, symbol:%s, time:%s, price:%.2f ' % (symbol, bar.strtime, bar.close))
            elif pos is not None and (dif[-1] < EPS and dea[-1] < EPS and dif[-1] < dif[-2] and dif[-1] < dea[-1]):
                vol = pos.volume - pos.volume_today
                if vol > 0:
                    self.close_long(bar.exchange, bar.sec_id, 0, vol)
                    logging.info('close long, symbol:%s, time:%s, price:%.2f ' % (symbol, bar.strtime, bar.close))

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
                logging.info(
                    'fixnation stop profit: close long, symbol:%s, time:%s, price:%.2f, vwap: %s, volume:%s' % (symbol,
                                                                                                                bar.strtime,
                                                                                                                bar.close,
                                                                                                                pos.vwap,
                                                                                                                pos.volume))
            elif pos.fpnl < 0 and pos.fpnl / pos.cost <= -1 * self.stop_fixation_loss:
                self.close_long(bar.exchange, bar.sec_id, 0, pos.volume - pos.volume_today)
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
                        logging.info(
                            'movement stop profit: close long, symbol:%s, time:%s, price:%.2f, vwap:%.2f, volume:%s' % (
                            symbol,
                            bar.strtime, bar.close, pos.vwap, pos.volume))

                        # 止损
            if pos.fpnl < 0 and pos.fpnl / pos.cost <= -1 * self.stop_fixation_loss:
                self.close_long(bar.exchange, bar.sec_id, 0, pos.volume - pos.volume_today)
                logging.info(
                    'movement stop loss: close long, symbol:%s, time:%s, price:%.2f, vwap:%.2f, volume:%s' % (symbol,
                                                                                                              bar.strtime,
                                                                                                              bar.close,
                                                                                                              pos.vwap,
                                                                                                              pos.volume))


if __name__ == '__main__':
    print(get_version())
    cur_date = datetime.date.today().strftime('%Y%m%d')
    log_file = 'macd_stock' + cur_date + '.log'
    logging.config.fileConfig('macd_stock.ini')
    MACD_STOCK.read_ini('macd_stock.ini')
    MACD_STOCK.get_strategy_conf()

    macd_stock = MACD_STOCK(username=MACD_STOCK.cls_user_name,
                            password=MACD_STOCK.cls_password,
                            strategy_id=MACD_STOCK.cls_strategy_id,
                            subscribe_symbols=MACD_STOCK.cls_subscribe_symbols,
                            mode=MACD_STOCK.cls_mode,
                            td_addr=MACD_STOCK.cls_td_addr)

    if MACD_STOCK.cls_mode == gm.MD_MODE_PLAYBACK:
        MACD_STOCK.get_backtest_conf()
        ret = macd_stock.backtest_config(start_time=MACD_STOCK.cls_backtest_start,
                                         end_time=MACD_STOCK.cls_backtest_end,
                                         initial_cash=MACD_STOCK.cls_initial_cash,
                                         transaction_ratio=MACD_STOCK.cls_transaction_ratio,
                                         commission_ratio=MACD_STOCK.cls_commission_ratio,
                                         slippage_ratio=MACD_STOCK.cls_slippage_ratio,
                                         price_type=MACD_STOCK.cls_price_type,
                                         bench_symbol=MACD_STOCK.cls_bench_symbol)

    macd_stock.get_para_conf()
    macd_stock.init_strategy()
    ret = macd_stock.run()
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

[http://zjshe.cn/q/forum.php?mod=viewthread&tid=96&extra=page%3D1](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php%3Fmod%3Dviewthread%26tid%3D96%26extra%3Dpage%253D1)

**有不了解的可以给小编留言哦，小编会细心为大家解答~**


