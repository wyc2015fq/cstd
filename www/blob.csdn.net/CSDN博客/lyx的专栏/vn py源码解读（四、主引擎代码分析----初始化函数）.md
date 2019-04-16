# vn.py源码解读（四、主引擎代码分析----初始化函数） - lyx的专栏 - CSDN博客





2018年12月06日 21:51:37[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：621
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)











        vnpy有一个叫做主引擎的东西，在三里面也说过，个人觉得这个应该是一个运行框架的东西，不应该叫做引擎，不过没关系，名字而已嘛。这一篇呢主要就是分析一下主引擎的代码。

```python
class MainEngine(object):
    """主引擎"""

    #----------------------------------------------------------------------
    def __init__(self, eventEngine):
        """Constructor"""
        
    #----------------------------------------------------------------------
    def addGateway(self, gatewayModule):
               
    #----------------------------------------------------------------------
    def addApp(self, appModule):
       
    #----------------------------------------------------------------------
    def getGateway(self, gatewayName):
                
    #----------------------------------------------------------------------
    def connect(self, gatewayName):
                 
    #----------------------------------------------------------------------
    def subscribe(self, subscribeReq, gatewayName):
          
    #----------------------------------------------------------------------
    def sendOrder(self, orderReq, gatewayName):
                
    #----------------------------------------------------------------------
    def cancelOrder(self, cancelOrderReq, gatewayName):
        """对特定接口撤单"""
          
    #----------------------------------------------------------------------
    def qryAccount(self, gatewayName):
        """查询特定接口的账户"""
                    
    #----------------------------------------------------------------------
    def qryPosition(self, gatewayName):
        """查询特定接口的持仓"""
        
    #----------------------------------------------------------------------
    def exit(self):
        """退出程序前调用，保证正常退出"""        
            
    #----------------------------------------------------------------------
    def writeLog(self, content):
        """快速发出日志事件"""
                  
    #----------------------------------------------------------------------
    def dbConnect(self):
        """连接MongoDB数据库"""
            
    #----------------------------------------------------------------------
    def dbInsert(self, dbName, collectionName, d):
        """向MongoDB中插入数据，d是具体数据"""
           
    #----------------------------------------------------------------------
    def dbQuery(self, dbName, collectionName, d, sortKey='', sortDirection=ASCENDING):
        """从MongoDB中读取数据，d是查询要求，返回的是数据库查询的指针"""
        
    #----------------------------------------------------------------------
    def dbUpdate(self, dbName, collectionName, d, flt, upsert=False):
        """向MongoDB中更新数据，d是具体数据，flt是过滤条件，upsert代表若无是否要插入"""
    
    #----------------------------------------------------------------------
    def dbDelete(self, dbName, collectionName, flt):
        """从数据库中删除数据，flt是过滤条件"""
            
    #----------------------------------------------------------------------
    def dbLogging(self, event):
        """向MongoDB中插入日志"""
    
    #----------------------------------------------------------------------
    def getTick(self, vtSymbol):
        """查询行情"""  
    
    #----------------------------------------------------------------------
    def getContract(self, vtSymbol):
        """查询合约"""
    
    #----------------------------------------------------------------------
    def getAllContracts(self):
        """查询所有合约（返回列表）"""
    
    #----------------------------------------------------------------------
    def getOrder(self, vtOrderID):
        """查询委托"""
    
    #----------------------------------------------------------------------
    def getPositionDetail(self, vtSymbol):
        """查询持仓细节"""
    
    #----------------------------------------------------------------------
    def getAllWorkingOrders(self):
        """查询所有的活跃的委托（返回列表）"""
    
    #----------------------------------------------------------------------
    def getAllOrders(self):
        """查询所有委托"""
    
    #----------------------------------------------------------------------
    def getAllTrades(self):
        """查询所有成交"""
    
    #----------------------------------------------------------------------
    def getAllAccounts(self):
        """查询所有账户"""
    
    #----------------------------------------------------------------------
    def getAllPositions(self):
        """查询所有持仓"""
    
    #----------------------------------------------------------------------
    def getAllPositionDetails(self):
        """查询本地持仓缓存细节"""
    
    #----------------------------------------------------------------------
    def getAllGatewayDetails(self):
        """查询引擎中所有底层接口的信息"""
    
    #----------------------------------------------------------------------
    def getAllAppDetails(self):
        """查询引擎中所有上层应用的信息"""
       
    
    #----------------------------------------------------------------------
    def getApp(self, appName):
        """获取APP引擎对象"""
    
    #----------------------------------------------------------------------
    def initLogEngine(self):
        """初始化日志引擎"""
    
    #----------------------------------------------------------------------
    def registerLogEvent(self, eventType):
        """注册日志事件监听"""
    
    #----------------------------------------------------------------------
    def convertOrderReq(self, req):
        """转换委托请求"""
        return self.dataEngine.convertOrderReq(req)

    #----------------------------------------------------------------------
    def getLog(self):
        """查询日志"""
        return self.dataEngine.getLog()
    
    #----------------------------------------------------------------------
    def getError(self):
        """查询错误"""
        return self.dataEngine.getError()
```

        这个MainEngine的类还是很长的，这里就删掉了很多中间的定义，在后面讲解的时候一一放上去，大家先有个大概。

        还是老样子，假设我们自己要写一个这样的把日志接口、交易策略、ctp实时行情、和数据库交互放在一起，那么我们会怎么写？我们先来看一下初始化函数吧。

## 1.初始化函数

        初始化函数中，首先，把我们之前讲的初始化好的事件引擎给挂上，然后把负责数据的数据引擎给挂上。我们发现，这里出现了一个DataEngine的东西，我们看一下这个是怎么定义的。

```
# 记录今日日期
        self.todayDate = datetime.now().strftime('%Y%m%d')
        
        # 绑定事件引擎
        self.eventEngine = eventEngine
        self.eventEngine.start()
        
        # 创建数据引擎
        self.dataEngine = DataEngine(self.eventEngine)
```

        这是DataEngine的全部代码，我们进一步来细分。

```python
class DataEngine(object):
    """数据引擎"""
    contractFileName = 'ContractData.vt'
    contractFilePath = getTempPath(contractFileName)
    
    FINISHED_STATUS = [STATUS_ALLTRADED, STATUS_REJECTED, STATUS_CANCELLED]

    #----------------------------------------------------------------------
    def __init__(self, eventEngine):
        """Constructor"""
        self.eventEngine = eventEngine
        
        # 保存数据的字典和列表
        self.tickDict = {}
        self.contractDict = {}
        self.orderDict = {}
        self.workingOrderDict = {}  # 可撤销委托
        self.tradeDict = {}
        self.accountDict = {}
        self.positionDict= {}
        self.logList = []
        self.errorList = []
        
        # 持仓细节相关
        self.detailDict = {}                                # vtSymbol:PositionDetail
        self.tdPenaltyList = globalSetting['tdPenalty']     # 平今手续费惩罚的产品代码列表
        
        # 读取保存在硬盘的合约数据
        self.loadContracts()
        
        # 注册事件监听
        self.registerEvent()
    
    #----------------------------------------------------------------------
    def registerEvent(self):
        """注册事件监听"""
        self.eventEngine.register(EVENT_TICK, self.processTickEvent)
        self.eventEngine.register(EVENT_CONTRACT, self.processContractEvent)
        self.eventEngine.register(EVENT_ORDER, self.processOrderEvent)
        self.eventEngine.register(EVENT_TRADE, self.processTradeEvent)
        self.eventEngine.register(EVENT_POSITION, self.processPositionEvent)
        self.eventEngine.register(EVENT_ACCOUNT, self.processAccountEvent)
        self.eventEngine.register(EVENT_LOG, self.processLogEvent)
        self.eventEngine.register(EVENT_ERROR, self.processErrorEvent)
        
    #----------------------------------------------------------------------
    def processTickEvent(self, event):
        """处理成交事件"""
        tick = event.dict_['data']
        self.tickDict[tick.vtSymbol] = tick    
    
    #----------------------------------------------------------------------
    def processContractEvent(self, event):
        """处理合约事件"""
        contract = event.dict_['data']
        self.contractDict[contract.vtSymbol] = contract
        self.contractDict[contract.symbol] = contract       # 使用常规代码（不包括交易所）可能导致重复
    
    #----------------------------------------------------------------------
    def processOrderEvent(self, event):
        """处理委托事件"""
        order = event.dict_['data']        
        self.orderDict[order.vtOrderID] = order
        
        # 如果订单的状态是全部成交或者撤销，则需要从workingOrderDict中移除
        if order.status in self.FINISHED_STATUS:
            if order.vtOrderID in self.workingOrderDict:
                del self.workingOrderDict[order.vtOrderID]
        # 否则则更新字典中的数据        
        else:
            self.workingOrderDict[order.vtOrderID] = order
            
        # 更新到持仓细节中
        detail = self.getPositionDetail(order.vtSymbol)
        detail.updateOrder(order)            
            
    #----------------------------------------------------------------------
    def processTradeEvent(self, event):
        """处理成交事件"""
        trade = event.dict_['data']
        
        self.tradeDict[trade.vtTradeID] = trade
    
        # 更新到持仓细节中
        detail = self.getPositionDetail(trade.vtSymbol)
        detail.updateTrade(trade)        

    #----------------------------------------------------------------------
    def processPositionEvent(self, event):
        """处理持仓事件"""
        pos = event.dict_['data']
        
        self.positionDict[pos.vtPositionName] = pos
    
        # 更新到持仓细节中
        detail = self.getPositionDetail(pos.vtSymbol)
        detail.updatePosition(pos)                
        
    #----------------------------------------------------------------------
    def processAccountEvent(self, event):
        """处理账户事件"""
        account = event.dict_['data']
        self.accountDict[account.vtAccountID] = account
    
    #----------------------------------------------------------------------
    def processLogEvent(self, event):
        """处理日志事件"""
        log = event.dict_['data']
        self.logList.append(log)
    
    #----------------------------------------------------------------------
    def processErrorEvent(self, event):
        """处理错误事件"""
        error = event.dict_['data']
        self.errorList.append(error)
        
    #----------------------------------------------------------------------
    def getTick(self, vtSymbol):
        """查询行情对象"""
        try:
            return self.tickDict[vtSymbol]
        except KeyError:
            return None        
    
    #----------------------------------------------------------------------
    def getContract(self, vtSymbol):
        """查询合约对象"""
        try:
            return self.contractDict[vtSymbol]
        except KeyError:
            return None
        
    #----------------------------------------------------------------------
    def getAllContracts(self):
        """查询所有合约对象（返回列表）"""
        return self.contractDict.values()
    
    #----------------------------------------------------------------------
    def saveContracts(self):
        """保存所有合约对象到硬盘"""
        f = shelve.open(self.contractFilePath)
        f['data'] = self.contractDict
        f.close()
    
    #----------------------------------------------------------------------
    def loadContracts(self):
        """从硬盘读取合约对象"""
        f = shelve.open(self.contractFilePath)
        if 'data' in f:
            d = f['data']
            for key, value in d.items():
                self.contractDict[key] = value
        f.close()
        
    #----------------------------------------------------------------------
    def getOrder(self, vtOrderID):
        """查询委托"""
        try:
            return self.orderDict[vtOrderID]
        except KeyError:
            return None
    
    #----------------------------------------------------------------------
    def getAllWorkingOrders(self):
        """查询所有活动委托（返回列表）"""
        return self.workingOrderDict.values()
    
    #----------------------------------------------------------------------
    def getAllOrders(self):
        """获取所有委托"""
        return self.orderDict.values()
    
    #----------------------------------------------------------------------
    def getAllTrades(self):
        """获取所有成交"""
        return self.tradeDict.values()
    
    #----------------------------------------------------------------------
    def getAllPositions(self):
        """获取所有持仓"""
        return self.positionDict.values()
    
    #----------------------------------------------------------------------
    def getAllAccounts(self):
        """获取所有资金"""
        return self.accountDict.values()
    
    #----------------------------------------------------------------------
    def getPositionDetail(self, vtSymbol):
        """查询持仓细节"""
        if vtSymbol in self.detailDict:
            detail = self.detailDict[vtSymbol]
        else:
            contract = self.getContract(vtSymbol)
            detail = PositionDetail(vtSymbol, contract)
            self.detailDict[vtSymbol] = detail
            
            # 设置持仓细节的委托转换模式
            contract = self.getContract(vtSymbol)
            
            if contract:
                detail.exchange = contract.exchange
                
                # 上期所合约
                if contract.exchange == EXCHANGE_SHFE:
                    detail.mode = detail.MODE_SHFE
                
                # 检查是否有平今惩罚
                for productID in self.tdPenaltyList:
                    if str(productID) in contract.symbol:
                        detail.mode = detail.MODE_TDPENALTY
                
        return detail
    
    #----------------------------------------------------------------------
    def getAllPositionDetails(self):
        """查询所有本地持仓缓存细节"""
        return self.detailDict.values()
    
    #----------------------------------------------------------------------
    def updateOrderReq(self, req, vtOrderID):
        """委托请求更新"""
        vtSymbol = req.vtSymbol
            
        detail = self.getPositionDetail(vtSymbol)
        detail.updateOrderReq(req, vtOrderID)
    
    #----------------------------------------------------------------------
    def convertOrderReq(self, req):
        """根据规则转换委托请求"""
        detail = self.detailDict.get(req.vtSymbol, None)
        if not detail:
            return [req]
        else:
            return detail.convertOrderReq(req)

    #----------------------------------------------------------------------
    def getLog(self):
        """获取日志"""
        return self.logList
    
    #----------------------------------------------------------------------
    def getError(self):
        """获取错误"""
        return self.errorList
```

## 2.DataEngine的初始化函数

        我们看到，首先把之前的事件引擎传过来了，初始化了一系列将要用到的存储数据的变量。

```python
def __init__(self, eventEngine):
        """Constructor"""
        self.eventEngine = eventEngine
        
        # 保存数据的字典和列表
        self.tickDict = {}
        self.contractDict = {}
        self.orderDict = {}
        self.workingOrderDict = {}  # 可撤销委托
        self.tradeDict = {}
        self.accountDict = {}
        self.positionDict= {}
        self.logList = []
        self.errorList = []
        
        # 持仓细节相关
        self.detailDict = {}                                # vtSymbol:PositionDetail
        self.tdPenaltyList = globalSetting['tdPenalty']     # 平今手续费惩罚的产品代码列表
        
        # 读取保存在硬盘的合约数据
        self.loadContracts()
        
        # 注册事件监听
        self.registerEvent()
```

        然后读取平今手续费大的期货列表。存在globalSetting里面，而这个变量其实在vtGlobal中被从文件中读取出来了，

```python
settingFileName = "VT_setting.json"
globalSetting = loadJsonSetting(settingFileName)
```

        VT_setting.json中有这样的一行：
`"tdPenalty": ["IF", "IH", "IC"]`
        所以也就是股指期货是日内平仓手续费惩罚的，如果后面放开了惩罚，那么可以去掉。

        然后是读取合约文件，

```python
def loadContracts(self):
        """从硬盘读取合约对象"""
        f = shelve.open(self.contractFilePath)
        if 'data' in f:
            d = f['data']
            for key, value in d.items():
                self.contractDict[key] = value
        f.close()
```

其中

```
contractFileName = 'ContractData.vt'
contractFilePath = getTempPath(contractFileName)
```

        其实这是从本都读取了一个文件，而这个文件其实是ctp连接之后获取的，获取了当天的交易代码已经代码的信息。这里的key和value中的key就是代码，比如rb1901，而value则是对应的vnpy自定义的一个合约类，类的定义如下：

```python
class VtContractData(VtBaseData):
    """合约详细信息类"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        super(VtContractData, self).__init__()
        
        self.symbol = EMPTY_STRING              # 代码
        self.exchange = EMPTY_STRING            # 交易所代码
        self.vtSymbol = EMPTY_STRING            # 合约在vt系统中的唯一代码，通常是 合约代码.交易所代码
        self.name = EMPTY_UNICODE               # 合约中文名
        
        self.productClass = EMPTY_UNICODE       # 合约类型
        self.size = EMPTY_INT                   # 合约大小
        self.priceTick = EMPTY_FLOAT            # 合约最小价格TICK
        
        # 期权相关
        self.strikePrice = EMPTY_FLOAT          # 期权行权价
        self.underlyingSymbol = EMPTY_STRING    # 标的物合约代码
        self.optionType = EMPTY_UNICODE         # 期权类型
        self.expiryDate = EMPTY_STRING          # 到期日
```

        还是很详细的。

        然后是把需要监听的事件给挂上去了

```python
def registerEvent(self):
        """注册事件监听"""
        self.eventEngine.register(EVENT_TICK, self.processTickEvent)
        self.eventEngine.register(EVENT_CONTRACT, self.processContractEvent)
        self.eventEngine.register(EVENT_ORDER, self.processOrderEvent)
        self.eventEngine.register(EVENT_TRADE, self.processTradeEvent)
        self.eventEngine.register(EVENT_POSITION, self.processPositionEvent)
        self.eventEngine.register(EVENT_ACCOUNT, self.processAccountEvent)
        self.eventEngine.register(EVENT_LOG, self.processLogEvent)
        self.eventEngine.register(EVENT_ERROR, self.processErrorEvent)
```

        这个函数里面，一次注册了八个事件，同时类里面的其他方法基本就是用来对这些事件进行响应的回调方法。

        比如：

```python
def processTickEvent(self, event):
        """处理成交事件"""
        tick = event.dict_['data']
        self.tickDict[tick.vtSymbol] = tick
```

        这个事件处理函数的作用就是，在某个地方触发了TICK事件，那么我们的事件引擎就会调用这个方法，来把event事件对象中的数据拿出来方法哦tickDict当中。至于TICK事件什么时候发生，那么就是后面会讨论的事情了。

        那么DataEngine就可以讨论到这里了，后面碰到相应事件的时候我们再回来看这里的事件处理回调函数。

        MainEngine初始化了DataEngine之后，就开始初始化一下别的东西了。

```python
# MongoDB数据库相关
        self.dbClient = None    # MongoDB客户端对象
        
        # 接口实例
        self.gatewayDict = OrderedDict()
        self.gatewayDetailList = []
        
        # 应用模块实例
        self. appDict = OrderedDict()
        self.appDetailList = []
        
        # 风控引擎实例（特殊独立对象）
        self.rmEngine = None
```

        这里，MainEngine初始化了MongoDB数据库对象，数据接口实例对象，也就是我们后面的CTP，还有应用模块，也就是我们的策略的实例存放的字典，还有风控引擎，这个我们先不讨论，因为我们先不使用风控模块。

        然后是日志的初始化。

```python
# 日志引擎实例
        self.logEngine = None
        self.initLogEngine()
```

        这里，我们发现在MainEngine类里面又来了一个logEngine，是不是很神奇呢？我们来看一下initLogEngine以及logEngine类就知道了，这个类的功能很单一，不和别的东西联动，所以外面的logEngine归外面的，类里面的归里面的，谁都不犯着谁。

```python
def initLogEngine(self):
        """初始化日志引擎"""
        if not globalSetting["logActive"]:
            return
        
        # 创建引擎
        self.logEngine = LogEngine()
        
        # 设置日志级别
        levelDict = {
            "debug": LogEngine.LEVEL_DEBUG,
            "info": LogEngine.LEVEL_INFO,
            "warn": LogEngine.LEVEL_WARN,
            "error": LogEngine.LEVEL_ERROR,
            "critical": LogEngine.LEVEL_CRITICAL,
        }
        level = levelDict.get(globalSetting["logLevel"], LogEngine.LEVEL_CRITICAL)
        self.logEngine.setLogLevel(level)
        
        # 设置输出
        if globalSetting['logConsole']:
            self.logEngine.addConsoleHandler()
            
        if globalSetting['logFile']:
            self.logEngine.addFileHandler()
            
        # 注册事件监听
        self.registerLogEvent(EVENT_LOG)
```

        首先是读取设置文件中对日志的设置，然后初始化了一个日志类：

        self.logEngine = LogEngine()

        那我们就来看看logEngine究竟是一个怎么样的类。代码很长，大家可以自己去看一下源代码。这里只把讲解部分放上来，后面的文章也是这样。

```python
def __init__(self):
        """Constructor"""
        self.logger = logging.getLogger()        
        self.formatter = logging.Formatter('%(asctime)s  %(levelname)s: %(message)s')
        self.level = self.LEVEL_CRITICAL
        
        self.consoleHandler = None
        self.fileHandler = None
        
        # 添加NullHandler防止无handler的错误输出
        nullHandler = logging.NullHandler()
        self.logger.addHandler(nullHandler)    
        
        # 日志级别函数映射
        self.levelFunctionDict = {
            self.LEVEL_DEBUG: self.debug,
            self.LEVEL_INFO: self.info,
            self.LEVEL_WARN: self.warn,
            self.LEVEL_ERROR: self.error,
            self.LEVEL_CRITICAL: self.critical,
        }
```

        其实代码很简单，就是用来python原生的logging包，然后进行一些信息输出的封装罢了。重点看一下不同级别的日志实现函数，比如debug级别的：

```python
def debug(self, msg):
        """开发时用"""
        self.logger.debug(msg)
        
    #----------------------------------------------------------------------
    def info(self, msg):
        """正常输出"""
        self.logger.info(msg)
```

        还有一个比较重要的类中的方法就是log事件的回调函数，

```python
def processLogEvent(self, event):
        """处理日志事件"""
        log = event.dict_['data']
        function = self.levelFunctionDict[log.logLevel]     # 获取日志级别对应的处理函数
        msg = '\t'.join([log.gatewayName, log.logContent])
        function(msg)
```

        其实逻辑很简单，就是发生log事件的时候，通过获取这个log事件的级别来进行输出，debug级别就用debug，info级别就用info。

        我们来看一下默认的设置文件中对log的设置：

```
"logActive": true,
"logLevel": "debug",
"logConsole": true,
"logFile": true,
```

        所以，我们看到在MainEngine的initLogEngine方法中，我们对类内的设置都是这样来的，我们来看一下我们日志文件的输出内容，在temp文件夹下面。

比如，有这样的内容：

```
2018-11-29 21:52:01,707  INFO: 启动CTA策略运行子进程
2018-11-29 21:52:01,707  INFO: 事件引擎创建成功
2018-11-29 21:52:01,733  INFO: 主引擎创建成功
2018-11-29 21:52:01,734  INFO: 注册日志事件监听
2018-11-29 21:52:01,756  INFO: 连接CTP接口
2018-11-29 21:52:01,760  INFO: MAIN_ENGINE MongoDB连接成功
2018-11-29 21:52:01,773  INFO: CTP 交易服务器连接成功
2018-11-29 21:52:01,782  INFO: CTP 行情服务器连接成功
2018-11-29 21:52:01,851  INFO: CTP 行情服务器登录完成
2018-11-29 21:52:01,852  INFO: CTP 交易服务器登录完成
2018-11-29 21:52:02,025  INFO: CTP 结算信息确认完成
2018-11-29 21:52:02,688  INFO: CTP 交易合约信息获取完成
2018-11-29 21:52:11,782  INFO: CTA策略载入成功
2018-11-29 21:52:11,788  INFO: CTA策略初始化成功
2018-11-29 21:52:11,788  INFO: CTA策略启动成功
2018-11-29 21:52:11,809  INFO: CTA_STRATEGY    找不到策略类：EmaDemoStrategy
2018-11-29 21:52:11,809  INFO: CTA_STRATEGY    atr rsi:atr rsi策略初始化
2018-11-29 21:52:11,809  INFO: CTA_STRATEGY    atr rsi的交易合约IC1706无法找到
2018-11-29 21:52:11,811  INFO: CTA_STRATEGY    king keltner:king keltner策略初始化
2018-11-29 21:52:11,811  INFO: CTA_STRATEGY    king keltner的交易合约IH1706无法找到
2018-11-29 21:52:11,812  INFO: CTA_STRATEGY    atr rsi:atr rsi策略启动
2018-11-29 21:52:11,812  INFO: CTA_STRATEGY    king keltner:king keltner策略启动
```

        在initLogEngine函数中，还有最后一个，就是注册日志事件函数

```python
# 注册事件监听
self.registerLogEvent(EVENT_LOG)
```

        那么我们就来看看这个函数是什么。

```python
def registerLogEvent(self, eventType):
        """注册日志事件监听"""
        if self.logEngine:
            self.eventEngine.register(eventType, self.logEngine.processLogEvent)
```

        没什么特别的，就是之前说的处理函数和EVENT_LOG挂钩了，虽然笔者觉得这里vnpy写的真的有点繁琐，逻辑有点绕。

        到这里，我们才算把主引擎，也就是MainEngine的初始化函数讲完。接下来是一个很重要的东西，就是CTP的加入，就留在下一篇里面来讲吧。CTP和策略的加入算是整个实盘交易代码的核心吧。





