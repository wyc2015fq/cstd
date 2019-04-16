# vn.py源码解读（八、回测结果计算代码解析） - lyx的专栏 - CSDN博客





2018年12月29日 17:52:25[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：731








        我们核心关注一下calculateBacktestingResult这个方法，这个方法中最核心的是一个大循环。

```python
for trade in self.tradeDict.values():
            # 复制成交对象，因为下面的开平仓交易配对涉及到对成交数量的修改
            # 若不进行复制直接操作，则计算完后所有成交的数量会变成0
            trade = copy.copy(trade)
```

        整个循环中，最核心的就是那个tradeDict，这个是一个ordered字典，也就是有序的字典。里面的value是VtTradeDate对象。我们来看一下这个类的实现：

```python
class VtTradeData(VtBaseData):
    """成交数据类"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        super(VtTradeData, self).__init__()
        
        # 代码编号相关
        self.symbol = EMPTY_STRING              # 合约代码
        self.exchange = EMPTY_STRING            # 交易所代码
        self.vtSymbol = EMPTY_STRING            # 合约在vt系统中的唯一代码，通常是 合约代码.交易所代码
        
        self.tradeID = EMPTY_STRING             # 成交编号
        self.vtTradeID = EMPTY_STRING           # 成交在vt系统中的唯一编号，通常是 Gateway名.成交编号
        
        self.orderID = EMPTY_STRING             # 订单编号
        self.vtOrderID = EMPTY_STRING           # 订单在vt系统中的唯一编号，通常是 Gateway名.订单编号
        
        # 成交相关
        self.direction = EMPTY_UNICODE          # 成交方向
        self.offset = EMPTY_UNICODE             # 成交开平仓
        self.price = EMPTY_FLOAT                # 成交价格
        self.volume = EMPTY_INT                 # 成交数量
        self.tradeTime = EMPTY_STRING           # 成交时间
```

        那么，很显然，这是在回测成交的过程中被放到这个tradeDict中的。我们去crossLimitOrder方法中看一下。

```python
trade = VtTradeData()
                trade.vtSymbol = order.vtSymbol
                trade.tradeID = tradeID
                trade.vtTradeID = tradeID
                trade.orderID = order.orderID
                trade.vtOrderID = order.orderID
                trade.direction = order.direction
                trade.offset = order.offset
                
                # 以买入为例：
                # 1. 假设当根K线的OHLC分别为：100, 125, 90, 110
                # 2. 假设在上一根K线结束(也是当前K线开始)的时刻，策略发出的委托为限价105
                # 3. 则在实际中的成交价会是100而不是105，因为委托发出时市场的最优价格是100
                if buyCross:
                    trade.price = min(order.price, buyBestCrossPrice)
                    self.strategy.pos += order.totalVolume
                else:
                    trade.price = max(order.price, sellBestCrossPrice)
                    self.strategy.pos -= order.totalVolume
                
                trade.volume = order.totalVolume
                trade.tradeTime = self.dt.strftime('%H:%M:%S')
                trade.dt = self.dt
                self.strategy.onTrade(trade)
                
                self.tradeDict[tradeID] = trade
```

        大概是上面这段代码涉及的，先创建一个VtTradeData对象，然后依次赋值，最后放到tradeDict中。但是，笔者有一个疑问，就是VtTradeData中并没有定义dt这个属性，但是确实是可以赋值也可以在后面进行获取，难道是版本不对？留一个Q在这里，可能是笔者自己对python还有什么盲点吧。

我们简单看一下其中的一些属性吧：

![](https://img-blog.csdnimg.cn/2018121616232130.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)

我们发现交易时间和dt其实信息是重复的，不知道这样设计的原因。

![](https://img-blog.csdnimg.cn/20181216162618553.JPG)

orderID就很简单，就是1，2,3，一直往后。

然后我们继续看整个遍历tradeDict背后的详细代码逻辑。

```python
if trade.direction == DIRECTION_LONG:
```

首先，我们判断一下交易的方向，是多还是空，如果是多头交易，那么继续判断：

```python
if not shortTrade:
                    longTrade.append(trade)
                # 当前多头交易为平空
                else:
                    while True:
                        entryTrade = shortTrade[0]
                        exitTrade = trade
```

然后查看一下，shortTrade这个list是不是空的。如果是空的，说明目前没有空头的头寸，直接放入多头的list，也就是longTrade就可以了。如果不是就去轧差。

轧差的代码如下：

```python
while True:
                        entryTrade = shortTrade[0]
                        exitTrade = trade
                        
                        # 清算开平仓交易
                        closedVolume = min(exitTrade.volume, entryTrade.volume)
                        result = TradingResult(entryTrade.price, entryTrade.dt, 
                                               exitTrade.price, exitTrade.dt,
                                               -closedVolume, self.rate, self.slippage, self.size)
                        resultList.append(result)
                        
                        posList.extend([-1,0])
                        tradeTimeList.extend([result.entryDt, result.exitDt])
                        
                        # 计算未清算部分
                        entryTrade.volume -= closedVolume
                        exitTrade.volume -= closedVolume
                        
                        # 如果开仓交易已经全部清算，则从列表中移除
                        if not entryTrade.volume:
                            shortTrade.pop(0)
                        
                        # 如果平仓交易已经全部清算，则退出循环
                        if not exitTrade.volume:
                            break
                        
                        # 如果平仓交易未全部清算，
                        if exitTrade.volume:
                            # 且开仓交易已经全部清算完，则平仓交易剩余的部分
                            # 等于新的反向开仓交易，添加到队列中
                            if not shortTrade:
                                longTrade.append(exitTrade)
                                break
                            # 如果开仓交易还有剩余，则进入下一轮循环
                            else:
                                pass
```

首先，拿出空头头寸的第一笔敞口，然后计算一下这一笔空头敞口和当前这笔多单的volume大小，按照小的数字来平。

```python
# 清算开平仓交易
                        closedVolume = min(exitTrade.volume, entryTrade.volume)
                        result = TradingResult(entryTrade.price, entryTrade.dt, 
                                               exitTrade.price, exitTrade.dt,
                                               -closedVolume, self.rate, self.slippage, self.size)
                        resultList.append(result)
```

具体两笔交易的轧差结果，通过TradingResult这个class来计算。

```python
class TradingResult(object):
    """每笔交易的结果"""

    #----------------------------------------------------------------------
    def __init__(self, entryPrice, entryDt, exitPrice, 
                 exitDt, volume, rate, slippage, size):
        """Constructor"""
        self.entryPrice = entryPrice    # 开仓价格
        self.exitPrice = exitPrice      # 平仓价格
        
        self.entryDt = entryDt          # 开仓时间datetime    
        self.exitDt = exitDt            # 平仓时间
        
        self.volume = volume    # 交易数量（+/-代表方向）
        
        self.turnover = (self.entryPrice+self.exitPrice)*size*abs(volume)   # 成交金额
        self.commission = self.turnover*rate                                # 手续费成本
        self.slippage = slippage*2*size*abs(volume)                         # 滑点成本
        self.pnl = ((self.exitPrice - self.entryPrice) * volume * size 
                    - self.commission - self.slippage)                      # 净盈亏
```

我们可以看到，这个class初始化之后的对象其实含有了佣金，slippage和pnl。总而言之，撮合之后，会有一个resultList。

然后，我们直接看一下存储result的list和trade.Dict当中的内容。也就是撮合成交结果和交易信息。

resultList[0]

entryDt： datetime.datetime(2015, 1, 12, 10, 6)

exitDt：datetime.datetime(2015, 1, 12, 14, 6)

self.tradeDict.values()[0]

dt：datetime.datetime(2015, 1, 12, 10, 6)

self.tradeDict.values()[1]

dt:datetime.datetime(2015, 1, 12, 14, 6)

我们可以看到，第一笔result的进入日期是12年10月6日；平仓的日期是12年14月6日。然后在存储交易的字典里面，第一笔交易记录就是指第一个result的开仓交易，第二笔交易，笔者设置的是第二笔交易就直接平仓了。所以第二笔交易就是第一个结果的平仓交易。所以，其实理论上，如果正反的笔数差不多的话，然后分布比较对称的话，那么resultlist的长度大概是存储交易的字典的一半。

后面过多的细节就不赘述了，vnpy给出的每笔交易维度的数据就存储在上面的tradeDict中，按照交易日维度的数据则在另外一个一个函数中计算并给出。

nvpy给出了一个showDailyResult的函数。我们看一下前面几行代码。

```python
def showDailyResult(self, df=None, result=None):
        """显示按日统计的交易结果"""
        if df is None:
            df = self.calculateDailyResult()
            df, result = self.calculateDailyStatistics(df)
        self.daily_result_store = df
```

我们发现，有一个返回df函数的calculateDailyResult函数。这个函数计算了daily级别的结果，并把一些指标保存在一个DataFrame里面。

这个函数的代码也补长：

```python
def calculateDailyResult(self):
        """计算按日统计的交易结果"""
        self.output(u'计算按日统计结果')
        
        # 检查成交记录
        if not self.tradeDict:
            self.output(u'成交记录为空，无法计算回测结果')
            return {}
        
        # 将成交添加到每日交易结果中
        for trade in self.tradeDict.values():
            date = trade.dt.date()
            dailyResult = self.dailyResultDict[date]
            dailyResult.addTrade(trade)
            
        # 遍历计算每日结果
        previousClose = 0
        openPosition = 0
        for dailyResult in self.dailyResultDict.values():
            dailyResult.previousClose = previousClose
            previousClose = dailyResult.closePrice
            
            dailyResult.calculatePnl(openPosition, self.size, self.rate, self.slippage )
            openPosition = dailyResult.closePosition
            
        # 生成DataFrame
        resultDict = {k:[] for k in dailyResult.__dict__.keys()}
        for dailyResult in self.dailyResultDict.values():
            for k, v in dailyResult.__dict__.items():
                resultDict[k].append(v)
                
        resultDf = pd.DataFrame.from_dict(resultDict)
        
        # 计算衍生数据
        resultDf = resultDf.set_index('date')
        
        return resultDf
```

        其实很简单，代码细节大家看一下就可以了解了，我们来查看一下df里面的东西。

![](https://img-blog.csdnimg.cn/20181229174633397.JPG)

        我们可以看到，这个df有这么多列，这里没有展示的是index。它的index是日期。我们简单解释一下这些内容。closePosition就是这一天收盘后的持仓、closePrice就是当天的收盘价、commission就是这一天的手续费、netPnl就是这一天的净盈亏，包括了手续费和滑点等其他可能的费用、openPosition就是开盘的时候的仓位。其实大部分都是字面意思吧。

        我们其实可以根据这些数据完成很多后续的测试，而这也是后面最重要的一个工作，比如对return进行蒙特卡洛仿真等等。









