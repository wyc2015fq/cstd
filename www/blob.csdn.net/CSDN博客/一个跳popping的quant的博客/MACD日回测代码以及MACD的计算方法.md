# MACD日回测代码以及MACD的计算方法 - 一个跳popping的quant的博客 - CSDN博客





2017年09月13日 19:56:55[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：6620
所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)









开始学习期货的量化交易，从米筐API上拷贝的一个关于股指期货主力合约日级别MACD日回测的入门代码： 

首先，先看一下关于MACD的介绍以及计算方式：MACD称为指数平滑移动平均线，是从双指数移动平均线发展而来，由快的指数移动平均线（EMA12）减去慢的指数移动平均线（EMA26）得到快线DIF，再用2×（快线DIF-DIF的9日加权移动均线DEA）得到MACD柱。 

关于以上的几种指标： 

EMA12：指数移动平均线。EMA(12)＝(收盘价－昨日的EMA)×2/13＋昨日的EMA 

=收盘价*2/13+昨日的EMA×11/13（股票上市的第一天的EMA是0) 

这样意思很明显了，这个EMA（12）的计算值表示的是今天的收盘价的权重分配为2/13,前一天计算的EMA分配为11/13。 

EMA26：指数移动平均线。同理EMA(12)，EMA（26）=收盘价*25/27+昨日的EMA×2/27。 

DIF：离差值。即12日EMA数值减去26日EMA数值。即为talib-MACD返回值macd。 

DEA：离差平均值。今日DEA = （前一日DEA X 8/10 + 今日DIF X 2/10），即为talib-MACD返回值signal。 

DIF与它自己的移动平均之间差距的大小一般BAR=（DIF-DEA)×2，即为MACD柱状图。但是talib中MACD的计算是bar = (DIF-DEA)×1。
```python
# 可以自己import我们平台支持的第三方python模块，比如pandas、numpy等
import talib


# 在这个方法中编写任何的初始化逻辑。context对象将会在你的算法策略的任何方法之间做传递
def init(context):
    # context内引入全局变量s1，存储目标合约信息
    context.s1 = 'IF1606'

    # 使用MACD需要设置长短均线和macd平均线的参数
    context.SHORTPERIOD = 12
    context.LONGPERIOD = 26
    context.SMOOTHPERIOD = 9
    context.OBSERVATION = 50

    #初始化时订阅合约行情。订阅之后的合约行情会在handle_bar中进行更新
    subscribe(context.s1)


# 你选择的期货数据更新将会触发此段逻辑，例如日线或分钟线更新
def handle_bar(context, bar_dict):
    # 开始编写你的主要的算法逻辑
    # 获取历史收盘价序列，history_bars函数直接返回ndarray，方便之后的有关指标计算
    prices = history_bars(context.s1, context.OBSERVATION, '1d', 'close')

    # 用Talib计算MACD取值，得到三个时间序列数组，分别为macd,signal 和 hist
    macd, signal, hist = talib.MACD(prices, context.SHORTPERIOD,
                                    context.LONGPERIOD, context.SMOOTHPERIOD)

    # macd 是长短均线的差值（即DIF），signal是macd的均线（即DEA），如果短均线从下往上突破长均线，为入场信号，进行买入开仓操作
    if macd[-1] - signal[-1] > 0 and macd[-2] - signal[-2] < 0:
        sell_qty = context.portfolio.positions[context.s1].sell_quantity
        # 先判断当前卖方仓位，如果有，则进行平仓操作
        if sell_qty > 0:
            buy_close(context.s1, 1)
        # 买入开仓
        buy_open(context.s1, 1)

    if macd[-1] - signal[-1] < 0 and macd[-2] - signal[-2] > 0:
        buy_qty = context.portfolio.positions[context.s1].buy_quantity
        # 先判断当前买方仓位，如果有，则进行平仓操作
        if buy_qty > 0:
            sell_close(context.s1, 1)
        # 卖出开仓
        sell_open(context.s1, 1)
```




