# 量化策略系列教程：11Arbitrage 套利策略 - 知乎
# 



套利策略感觉是个老生常谈的话题了，那今天小编就给大家来一个老策略新代码，大家跑跑看，[证经社量化社区 - 证经社](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php) ，记住，一定要在掘金的环境下跑哈~

**1.策略介绍及逻辑**

  策略在过去经验的统计验证基础上，认为两个股票或期货的价格比值符合统计稳定规律，如果价差超出某一阀值后，存在套利机会。本示例中，使用IF1703，IF1704当做标的。a：IF1703，b:IF1704。

代码中用lna - lnb 来表示的价格比。对价格取ln可以降低数据出现异值的可能性，提升数据的可用性。

交易规则：

监测lna - lnb

- 如果大于0:

   - 如果价格比值超出设定阀值，且低于止损阀值，空a多b；

   - 如果超出止损阀值，平空a,平多b;

- 如果小于0:

   - 如果小于负的设定阀值，且高于负的止损阀值，多a空b；

   - 如果小于负的止损阀值，平多a,平空b；

- 在价格比接近于1时，认为回归，平掉套利仓位

- 防止单腿成交：

 在check_positions中，判断如果4个tick数据（每只代码各2个tick更新）后，仍然只有单腿成交，则平掉单腿仓位。

注：只是一个示例套利的程序框架，实际应用中需要按照具体情况修改。

用同类品种跨期的价格差，可以直接用两者之间的价格相减。

**2.策略代码**

**  2.1配置文件【strategy_sa.ini】（提示ini配置文件，需要保存成UTF8格式）**


```
[strategy]
username= 
password= 
;回测模式
mode=4
td_addr=localhost:8001
strategy_id= 
;订阅代码注意及时更新
subscribe_symbols=CFFEX.IF1703.tick,CFFEX.IF1704.tick,CFFEX.IF1703.bar.15,CFFEX.IF1704.bar.15

[backtest]
start_time=2017-03-01 09:00:00
end_time=2017-03-08 16:00:00
;策略初始资金
initial_cash=10000000

;委托量成交比率，默认=1（每个委托100%成交）
transaction_ratio=1

;手续费率，默认=0（不计算手续费）
commission_ratio=0

;滑点比率，默认=0（无滑点）
slippage_ratio=0

[ss]
bar_type=15
window_size=20
trade_exchange_a=CFFEX
trade_secid_a=IF1703
trade_unit_a=1
trade_exchange_b=CFFEX
trade_secid_b=IF1704
trade_unit_b=1
tick_size=0.2

sigma=2.34

##############################################################
# logger settings
##############################################################
[loggers]
keys=root

[logger_root]
level=DEBUG
handlers=console,file

[handlers]
keys=console,file

[handler_file]
class=handlers.RotatingFileHandler
args=('strategy_sa.log','a',1000,5)
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


** 2.2策略文件【strategy_sa.py】**


```
#!/usr/bin/env python
# encoding: utf-8

import logging
import time
import numpy as np
from collections import deque
from gmsdk import *
from math import log
eps = 1e-6

class StatArb(StrategyBase):
    '''
        statistics arbitrage demo
    '''
    def __init__(self, *args, **kwargs):
        super(StatArb, self).__init__(*args, **kwargs)
        logging.basicConfig(format='%(asctime)s - %(levelname)s: %(message)s')
        self.tick_size = self.config.getfloat('ss', 'tick_size') or 0.2

        self.threshold = self.config.getfloat('ss', 'sigma') or 2.34
        self.significant_diff = self.threshold * 0.0015   ## 3/4 sigma
        self.stop_lose_threshold = self.threshold * 0.002  ## 2 * sigma

        self.trade_exchange_a = self.config.get('ss', 'trade_exchange_a') or 'CFFEX'
        self.trade_secid_a = self.config.get('ss', 'trade_secid_a')
        self.trade_unit_a = self.config.get('ss', 'trade_unit_a') or 1
        self.last_price_a = 0.0

        self.trade_exchange_b = self.config.get('ss', 'trade_exchange_b') or 'CFFEX'
        self.trade_secid_b = self.config.get('ss', 'trade_secid_b')
        self.trade_unit_b = self.config.get('ss', 'trade_unit_b') or 1
        self.last_price_b = 0.0

        self.pos_side_up = False
        self.pos_side_down = False

        self.window_size = 20

        self.close_buffer_symbol_a = deque(maxlen=self.window_size)
               self.close_buffer_symbol_b = deque(maxlen=self.window_size)
        self.at_risk = 0
        self.bar_type = self.config.get('ss', 'bar_type')

    def on_tick(self, tick):
        if tick.sec_id == self.trade_secid_a:
            self.last_price_a = tick.last_price
        elif tick.sec_id == self.trade_secid_b:
            self.last_price_b = tick.last_price

        self.check_position()

    def on_bar(self, bar):

        if bar.bar_type == 15:
            #print (bar.sec_id == 'IF1704')

            if bar.sec_id == 'IF1703':
                a = 1
                #print ('bar')

                self.close_buffer_symbol_a.append(bar.close)

            elif bar.sec_id == 'IF1704': #self.trade_secid_b:
                b = 1
                a = 1
                #print (bar.close)

                #print (bar.sec_id == self.trade_secid_a)
                self.close_buffer_symbol_b.append(bar.close)
                if a == 1 and b == 1:
                    self.algo_action()
            #print ('action')

    def open_side_up(self):
        self.open_short(self.trade_exchange_a, self.trade_secid_a, self.last_price_a, self.trade_unit_a)
        self.open_long(self.trade_exchange_b, self.trade_secid_b, self.last_price_b, self.trade_unit_b)
        self.pos_side_up = True

    def close_side_up(self):
        self.close_short(self.trade_exchange_a, self.trade_secid_a, self.last_price_a, self.trade_unit_a)
        self.close_long(self.trade_exchange_b, self.trade_secid_b, self.last_price_b, self.trade_unit_b)
        self.pos_side_up = False

    def open_side_down(self):
        self.open_long(self.trade_exchange_a, self.trade_secid_a, self.last_price_a, self.trade_unit_a)
        self.open_short(self.trade_exchange_b, self.trade_secid_b, self.last_price_b, self.trade_unit_b)
        self.pos_side_down = True

    def close_side_down(self):
        self.close_long(self.trade_exchange_a, self.trade_secid_a, self.last_price_a, self.trade_unit_a)
        self.close_short(self.trade_exchange_b, self.trade_secid_b, self.last_price_b, self.trade_unit_b)
        self.pos_side_down = False

    def algo_action(self):
        # type: () -> object

        latest_a = self.close_buffer_symbol_a.pop()
        lna = log(latest_a)

        latest_b = self.close_buffer_symbol_b.pop()
        lnb = log(latest_b)


        diff = lna - lnb
       #print (diff)
        #print(self.stop_lose_threshold)

        if diff > self.stop_lose_threshold:
            self.close_side_up()
            #print ('a')
        elif diff > self.significant_diff and diff < self.stop_lose_threshold:
            self.open_side_up()
            #print ('b')
        elif diff < - self.stop_lose_threshold:
            self.close_side_down()
            #print ('c')
        elif diff < - self.significant_diff and diff > - self.stop_lose_threshold:
            self.open_side_down()
            #print ('d')
        elif abs(diff) < self.threshold:
            if self.pos_side_up:
                self.close_side_up()
            if self.pos_side_down:
                self.close_side_down()


    def check_position(self):
        """  TODO: check if one leg position and close it  """
        ps = self.get_positions()
        count = len(ps)
        if count % 2 != 0:
            self.at_risk += 1
            ## if more than 4 tick data passed, need to force quit
            if self.at_risk < 4:
                return

            for p in ps:
                if self.pos_side_up:
                    if p.side == OrderSide_Ask:
                        self.close_short(p.exchange, p.sec_id, self.last_price_a, p.volume)
                    elif p.side == OrderSide_Bid:
                        self.close_long(p.exchange, p.sec_id, self.last_price_b, p.volume)
                if self.pos_side_down:
                    if p.side == OrderSide_Ask:
                        self.close_short(p.exchange, p.sec_id, self.last_price_b, p.volume)
                    elif p.side == OrderSide_Bid:
                        self.close_long(p.exchange, p.sec_id, self.last_price_a, p.volume)
        else:
            self.at_risk = 0


if __name__ == '__main__':
    #import pdb; pdb.set_trace()
    dm = StatArb(config_file='strategy_sa.ini')
    ret = dm.run()
    print("Statistics Arbitrage: ", dm.get_strerror(ret))
```


**如果想了解相关的python函数和掘金接口函数，走下方通道：**

[http://zjshe.cn/q/forum.php?mod=viewthread&tid=59&extra=page%3D1](https://link.zhihu.com/?target=http%3A//zjshe.cn/q/forum.php%3Fmod%3Dviewthread%26tid%3D59%26extra%3Dpage%253D1)

**有不了解的可以给小编留言哦，小编会细心为大家解答~**


