
# 量化交易研究———高级篇（2）MACD检测实现 - leofionn的博客 - CSDN博客


2018年05月21日 10:30:45[leofionn](https://me.csdn.net/qq_36142114)阅读数：471


import talib
import numpy as np
import pandas as pd

securities=['601006.SH','601008.SH','601155.SH','600519.SH','300136.SZ','600741.SH']
count = 0
for stock in securities:
values = get_price(stock, None, '20180105', '1d', ['close', 'high', 'low'], True, None, 40, is_panel=1)
DIFF, DEA, MACD = talib.MACD(values['close'].values,
fastperiod=12, slowperiod=26, signalperiod=9)
\# 若出现MACD死叉（MACD变为负值），则卖出股票
if MACD[-1] < 0:
log.info(MACD[-1])
order_target(stock, 0)

\# 买入股票
for stock in account.iwencai_securities:
\# 若股票数量到达限制，则跳出
if len(account.positions) >= account.n:
break
if stock not in account.positions:
\# 获取股票收盘价数据
values = data.attribute_history(stock, ['close'], 100, '1d', False, None)
if values.empty:
continue
\# 计算MACD值
DIFF, DEA, MACD = talib.MACD(values['close'].values,
fastperiod=12, slowperiod=26, signalperiod=9)
\# 若出现MACD金叉，则买入1/n仓位的股票
if MACD[-1] > 0 and MACD[-2] < 0:
order_target_percent(stock, 1/account.n)

