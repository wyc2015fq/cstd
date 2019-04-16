# vn.py源码解读（七、回测代码解析） - lyx的专栏 - CSDN博客





2018年12月16日 10:33:02[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：794
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)









        原本想开始讲策略类的编写，后来觉得，结合回测代码其实能够更好的理解，所以先解读一下vnpy回测的代码吧，后续自己也想把vnpy回测的部分优化一下，毕竟我觉得可视化和回测结果方提高还有很多空间。

        我们解读的代码从runbacktesting.py开始。首先，和实盘中一样导入了一个策略。

```python
from vnpy.trader.app.ctaStrategy.strategy.strategyDoubleMa import DoubleMaStrategy
```

        紧接着，就获得了一个回测引擎的对象：

```python
engine = BacktestingEngine()
```

        我们先不管这个引擎，继续往下看，发现后面与engine有关的操作有这些：

```python
# 设置引擎的回测模式为K线
    engine.setBacktestingMode(engine.BAR_MODE)
    # 设置回测用的数据起始日期
    engine.setStartDate('20120101')
        # 设置产品相关参数
    engine.setSlippage(0.2)     # 股指1跳
    engine.setRate(0.3/10000)   # 万0.3
    engine.setSize(300)         # 股指合约大小 
    engine.setPriceTick(0.2)    # 股指最小价格变动
        # 设置使用的历史数据库
    engine.setDatabase(MINUTE_DB_NAME, 'IF0000')
        # 在引擎中创建策略对象
    d = {}
    engine.initStrategy(DoubleMaStrategy, d)
        # 开始跑回测
    engine.runBacktesting()
        # 显示回测结果
    engine.showBacktestingResult()
```

        那么我们就一个一个来看吧。

### 1.回测模式

        我们进入BacktestingEngine类的定义，然后把和mode有关的全部看一下：

```python
def setBacktestingMode(self, mode):
        """设置回测模式"""
        self.mode = mode
```

        首先，在外面调用的就是这个函数，然后，我们可以注意到，其实这个函数就是修改了一下mode变量。那么我们继续追杀这个变量出现过的地方。

        在初始化函数当中，对这个变量做了初始化设置。

```python
self.mode = self.BAR_MODE   # 回测模式，默认为K线
```

        然后是在loadHistoryData函数汇总，根据mode变量来判断。

```python
# 首先根据回测模式，确认要使用的数据类
        if self.mode == self.BAR_MODE:
            dataClass = VtBarData
            func = self.newBar
        else:
            dataClass = VtTickData
            func = self.newTick
```

        也就是，根据不同的回测模式，tick回测还是bar回测，我们在从数据库读取数据的时候，需要不同的数据的类。

        在runBacktesting函数中，也有这段代码。个人觉得，这里略微啰嗦了，为什么不用工厂模式来生产呢？

        然后是crossLimitOrder函数，也是就限价单撮合函数中，根据回测类型判断成交价格。

```python
# 先确定会撮合成交的价格，这里和限价单规则相反
        if self.mode == self.BAR_MODE:
            buyCrossPrice = self.bar.high    # 若买入方向停止单价格低于该价格，则会成交
            sellCrossPrice = self.bar.low    # 若卖出方向限价单价格高于该价格，则会成交
            bestCrossPrice = self.bar.open   # 最优成交价，买入停止单不能低于，卖出停止单不能高于
        else:
            buyCrossPrice = self.tick.lastPrice
            sellCrossPrice = self.tick.lastPrice
            bestCrossPrice = self.tick.lastPrice
```

        在crossStopOrder中也是。此外，在计算回测结果的时候，calculateBacktestingResult函数中，到最后交易日，会用最后的价格平仓了解。这里最后平仓价格的确定也回测模式有关：

```python
# 到最后交易日尚未平仓的交易，则以最后价格平仓
        if self.mode == self.BAR_MODE:
            endPrice = self.bar.close
        else:
            endPrice = self.tick.lastPrice
```

        还有的是参数优化函数里面，这一部分暂时不考虑。

### 2.回测设置

```python
# 设置回测用的数据起始日期
    engine.setStartDate('20120101')
        # 设置产品相关参数
    engine.setSlippage(0.2)     # 股指1跳
    engine.setRate(0.3/10000)   # 万0.3
    engine.setSize(300)         # 股指合约大小 
    engine.setPriceTick(0.2)    # 股指最小价格变动
```

        我们看一下这个函数的构成：

```python
def setStartDate(self, startDate='20100416', initDays=10):
        """设置回测的启动日期"""
        self.startDate = startDate
        self.initDays = initDays
        
        self.dataStartDate = datetime.strptime(startDate, '%Y%m%d')
        
        initTimeDelta = timedelta(initDays)
        self.strategyStartDate = self.dataStartDate + initTimeDelta
```

        我们可以发现，设置的startDate是数据启动的日子，因为被变成了dataStartDate，而前面有一个initDays，这个就很奇怪了，这个是用于计算指标的吗？比如我们的策略是20日均线，那么就应该需要20个交易日的数据启动时间。那么这个20的参数应该是策略类所有，而且是20个交易日。但是这里的initDays明显就是日历日。那么，我们就分别追杀一下dataStartDate和strategyStartDate的使用的地方吧。

        在init初始化函数中，有这样的注释：

```python
self.dataStartDate = None       # 回测数据开始日期，datetime对象
        self.dataEndDate = None         # 回测数据结束日期，datetime对象
        self.strategyStartDate = None   # 策略启动日期（即前面的数据用于初始化），datetime对象
```

        明确了dataStartDate是回测数据开始的时间，而strategyStartDate和dataStartDate之前的差距是用于数据初始化的时间，也就是策略启动时间是在数据启动之后的。那么疑问就是，中间这段数据初始化时间是应该在这个地方被确定的吗？我们继续追杀。

        在loadHistoryData方法里面，我们又碰到了这个strategyStartDate变量，我们来看一下：

```python
# 载入初始化需要用的数据        
        if self.hdsClient:
            initCursor = self.hdsClient.loadHistoryData(self.dbName,
                                                        self.symbol,
                                                        self.dataStartDate,
                                                        self.strategyStartDate)
        else:
            flt = {'datetime':{'$gte':self.dataStartDate,
                               '$lt':self.strategyStartDate}}        
            initCursor = collection.find(flt).sort('datetime')
```

```python
self.hdsClient = None       # 历史数据服务器客户端
```

        其中，初始化函数中的定义其实已经告诉我们了，hdsClient是一个数据来源的标志符，显然我们的数据来自于本地的Mongodb，而不是什么亚马逊这种的云服务器之类的，所以我们会使用下面的if分支语句。

        我们看到，传递进去的是dataStrategyDate和strategyStartDate。

### 3.数据库部分

        后面就涉及到一点mongodb数据库python读取的知识了，简单介绍一下。

        首先，我们通过robo可视化工具看一下现在mongodb数据库里面的数据情况。

![](https://img-blog.csdnimg.cn/20181215130307720.JPG)

        我们在这个loadHistoryData方法的一开始，先获得了数据库的连接：

```python
self.dbClient = pymongo.MongoClient(globalSetting['mongoHost'], globalSetting['mongoPort'])
        collection = self.dbClient[self.dbName][self.symbol]
```

        前面一行是获取一个连接，globalSetting就是VT_setting配置文件里面的内容：

"mongoHost": "localhost",
"mongoPort": 27017,
"mongoLogging": true,
        然后是通过连接获得一个集合。获得集合首先需要数据库的名称，然后需要具体collection的名称。数据库的名称在一开始的

```python
engine.setDatabase(MINUTE_DB_NAME, 'IF0000')
```

         其中，MINUTE_DB_NAME = 'VnTrader_1Min_Db'

```python
#----------------------------------------------------------------------
    def setDatabase(self, dbName, symbol):
        """设置历史数据所用的数据库"""
        self.dbName = dbName
        self.symbol = symbol
```

我们发现，就是这个detDatabase方法设置了数据库获取的参数。

那么，根据我们前面看到的mongodb数据库的结构，我们就知道了，这里其实就是获得了IF0000的数据集合。我们来看一下具体从mongodb中获取数据的代码：

```python
# 载入初始化需要用的数据        
        if self.hdsClient:
            initCursor = self.hdsClient.loadHistoryData(self.dbName,
                                                        self.symbol,
                                                        self.dataStartDate,
                                                        self.strategyStartDate)
        else:
            flt = {'datetime':{'$gte':self.dataStartDate,
                               '$lt':self.strategyStartDate}}        
            initCursor = collection.find(flt).sort('datetime')
        
        # 将数据从查询指针中读取出，并生成列表
        self.initData = []              # 清空initData列表
        for d in initCursor:
            data = dataClass()
            data.__dict__ = d
            self.initData.append(data)
```

        上面的代码中，我们发现flt语句，在mongodb中，$gte是great than equal的意思，也就是大于等于，lt是less than的意思，也就是小于。所以，上面的initCursor指的是获得大于等于dataStartDate，同时小于strategyStartDate的行情数据。后面的语句是从这个数据库游标中获取这部分数据，并把它保存在self.initData中。

```python
# 载入回测数据
        if self.hdsClient:
            self.dbCursor = self.hdsClient.loadHistoryData(self.dbName,
                                                           self.symbol,
                                                           self.strategyStartDate,
                                                           self.dataEndDate)
        else:
            if not self.dataEndDate:
                flt = {'datetime':{'$gte':self.strategyStartDate}}   # 数据过滤条件
            else:
                flt = {'datetime':{'$gte':self.strategyStartDate,
                                   '$lte':self.dataEndDate}}  
            self.dbCursor = collection.find(flt).sort('datetime')
```

        然后是获取比strategyStartDate大的行情数据，如果有dataEndDate，那么就这个为止，没有的话就取出strategyStartDate之后的所有数据。

        其中的dataEndDate可以通过setEndDate方法来设置。

        那么，获取了上面的两端数据的作用是什么呢？我们追杀完几个Date之后，开始追杀一下initData和后来的dbCursor吧。

在初始化函数汇中，

```python
self.initData = []          # 初始化用的数据
```

数据类型是一个列表。

```python
def loadBar(self, dbName, collectionName, startDate):
        """直接返回初始化数据列表中的Bar"""
        return self.initData
    
    #----------------------------------------------------------------------
    def loadTick(self, dbName, collectionName, startDate):
        """直接返回初始化数据列表中的Tick"""
        return self.initData
```

然后是在两个方法中，会返回initData。因为会根据mode来进行数据的读取，所以，这里的initData其实是分bar和tick两种模式的，所欲在load的时候也是分了两个函数。

其他地方就没有用到initData了，也就是说，在回测引擎中获取的数据是给别的地方调用的。

### 4.runBackTesting

而dbCursor在runBacktesting中被使用：


```python
def runBacktesting(self):
        """运行回测"""
        # 载入历史数据
        self.loadHistoryData()
        
        # 首先根据回测模式，确认要使用的数据类
        if self.mode == self.BAR_MODE:
            dataClass = VtBarData
            func = self.newBar
        else:
            dataClass = VtTickData
            func = self.newTick

        self.output(u'开始回测')
        
        self.strategy.onInit()
        self.strategy.inited = True
        self.output(u'策略初始化完成')
        
        self.strategy.trading = True
        self.strategy.onStart()
        self.output(u'策略启动完成')
        
        self.output(u'开始回放数据')

        for d in self.dbCursor:
            data = dataClass()
            data.__dict__ = d
            func(data)     
            
        self.output(u'数据回放结束')
```

        我们看到，其实是从数据游标dbCursor中依次获取数据，然后用func函数来处理，而func函数是什么呢？就是前面根据回测模式选择出来的函数，self.newTick或者self.newBar，我们来看一下newBar，

```python
def newBar(self, bar):
        """新的K线"""
        self.bar = bar
        self.dt = bar.datetime
        
        self.crossLimitOrder()      # 先撮合限价单
        self.crossStopOrder()       # 再撮合停止单
        self.strategy.onBar(bar)    # 推送K线到策略中
        
        self.updateDailyClose(bar.datetime, bar.close)
```

        我们看到，其实最核心的函数分别是crossLimitOrder和crossStopOrder，其实作用就是根据新的bar来判断有没有交易成交或者停止。也就是一个成交的逻辑。我们后面展开来讲。而这里还有一部分是strategy的onBar方法来接受bar，我们知道了，其实在回测过程中，每一次新的bar产生之后，都会调用strategy的onBar方法。也就是说，onBar方法的重写是编写策略的核心。

        策略的载入在初始化策略方法中完成：

```python
def initStrategy(self, strategyClass, setting=None):
        """
        初始化策略
        setting是策略的参数设置，如果使用类中写好的默认设置则可以不传该参数
        """
        self.strategy = strategyClass(self, setting)
        self.strategy.name = self.strategy.className
```

        这段比较简单，就不用多说了。

        最后就是运行runBacktesting方法开始获取每一个bar的行情，然后就是利用showBacktestingResult给出结果。

        那么我们就来看看最后这个showBacktestingResult的功能吧。如果后续自己想做点可视化和别的策略评价功能的话，最重要的就是改写这个方法。

        这个方法一开始会调用calculateBacktestingResult方法，我们继续追杀这个方法。而这个方法其实是策略回测结果展示最核心的一个方法，所有的pnl计算、胜率计算都在这个函数里面。后面一篇文章我们就仔细来拆解一下这个函数吧。







