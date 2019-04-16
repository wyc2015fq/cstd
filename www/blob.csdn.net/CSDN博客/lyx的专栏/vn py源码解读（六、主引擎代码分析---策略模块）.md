# vn.py源码解读（六、主引擎代码分析---策略模块） - lyx的专栏 - CSDN博客





2018年12月11日 20:23:28[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：547
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)









      之前在讲MainEngine的时候，有这样一个代码：

```python
me.addApp(ctaStrategy)
```

       这里，我们来看一下MainEngine里面这个addApp函数的代码：

```python
def addApp(self, appModule):
        """添加上层应用"""
        appName = appModule.appName
        
        # 创建应用实例
        self.appDict[appName] = appModule.appEngine(self, self.eventEngine)
        
        # 将应用引擎实例添加到主引擎的属性中
        self.__dict__[appName] = self.appDict[appName]
        
        # 保存应用信息
        d = {
            'appName': appModule.appName,
            'appDisplayName': appModule.appDisplayName,
            'appWidget': appModule.appWidget,
            'appIco': appModule.appIco
        }
        self.appDetailList.append(d)
```

      我发现，不管是变量命名还是整个过程，都和gateway很像，所以后面分析起来应该会很容易。

      同样的，我们看一下ctaStrategy的内容。同样的，在vnpy/trader/app/ctaStrategy文件夹下面的init文件中，是这样的代码：

```python
from .ctaEngine import CtaEngine
from .uiCtaWidget import CtaEngineManager

appName = 'CtaStrategy'
appDisplayName = u'CTA策略'
appEngine = CtaEngine
appWidget = CtaEngineManager
appIco = 'cta.ico'
```

         我们看一下，appEngine到底是一个怎么样的类。其实就是CtaEngine，

         我们大致看一下这个类是怎么实现的。

```python
class CtaEngine(AppEngine):
    """CTA策略引擎"""
    settingFileName = 'CTA_setting.json'
    settingfilePath = getJsonPath(settingFileName, __file__)
    
    STATUS_FINISHED = set([STATUS_REJECTED, STATUS_CANCELLED, STATUS_ALLTRADED])

    #----------------------------------------------------------------------
    def __init__(self, mainEngine, eventEngine):
        """Constructor"""
        self.mainEngine = mainEngine
        self.eventEngine = eventEngine
        
        # 当前日期
        self.today = todayDate()
        
        # 保存策略实例的字典
        # key为策略名称，value为策略实例，注意策略名称不允许重复
        self.strategyDict = {}
        
        # 保存vtSymbol和策略实例映射的字典（用于推送tick数据）
        # 由于可能多个strategy交易同一个vtSymbol，因此key为vtSymbol
        # value为包含所有相关strategy对象的list
        self.tickStrategyDict = {}
        
        # 保存vtOrderID和strategy对象映射的字典（用于推送order和trade数据）
        # key为vtOrderID，value为strategy对象
        self.orderStrategyDict = {}     
        
        # 本地停止单编号计数
        self.stopOrderCount = 0
        # stopOrderID = STOPORDERPREFIX + str(stopOrderCount)
        
        # 本地停止单字典
        # key为stopOrderID，value为stopOrder对象
        self.stopOrderDict = {}             # 停止单撤销后不会从本字典中删除
        self.workingStopOrderDict = {}      # 停止单撤销后会从本字典中删除
        
        # 保存策略名称和委托号列表的字典
        # key为name，value为保存orderID（限价+本地停止）的集合
        self.strategyOrderDict = {}
        
        # 成交号集合，用来过滤已经收到过的成交推送
        self.tradeSet = set()
        
        # 引擎类型为实盘
        self.engineType = ENGINETYPE_TRADING
        
        # 注册日式事件类型
        self.mainEngine.registerLogEvent(EVENT_CTA_LOG)
        
        # 注册事件监听
        self.registerEvent()
```

         我们发现，ctaEngine类的初始化函数居然要传入MainEngine和EventEngine，感觉有那么点点乱。。。init函数中，前面都比较简单，就是初始化了一些内部有使用的变量，从最后一个函数入手，注册事件监听。我们看一下这个方法：

```python
def registerEvent(self):
        """注册事件监听"""
        self.eventEngine.register(EVENT_TICK, self.processTickEvent)
        self.eventEngine.register(EVENT_ORDER, self.processOrderEvent)
        self.eventEngine.register(EVENT_TRADE, self.processTradeEvent)
```

         所以，我们现在只要考察这几个事件回调方法就可以了。

         具体的这几个方法的内容就不说了。这个类当中处理这些process开头的回调函数之外，还有其他的方法，用于产生下单、撤单之类的事件。这些方法包括：sendOrder、cancelOrder、sendStopOrder、cancelStopOrder。

         我们以sendOrder为例，

```python
def sendOrder(self, vtSymbol, orderType, price, volume, strategy):
        """发单"""
        contract = self.mainEngine.getContract(vtSymbol)
        
        req = VtOrderReq()
        req.symbol = contract.symbol
        req.exchange = contract.exchange
        req.vtSymbol = contract.vtSymbol
        req.price = self.roundToPriceTick(contract.priceTick, price)
        req.volume = volume
        
        req.productClass = strategy.productClass
        req.currency = strategy.currency        
        
        # 设计为CTA引擎发出的委托只允许使用限价单
        req.priceType = PRICETYPE_LIMITPRICE    
        
        # CTA委托类型映射
        if orderType == CTAORDER_BUY:
            req.direction = DIRECTION_LONG
            req.offset = OFFSET_OPEN
            
        elif orderType == CTAORDER_SELL:
            req.direction = DIRECTION_SHORT
            req.offset = OFFSET_CLOSE
                
        elif orderType == CTAORDER_SHORT:
            req.direction = DIRECTION_SHORT
            req.offset = OFFSET_OPEN
            
        elif orderType == CTAORDER_COVER:
            req.direction = DIRECTION_LONG
            req.offset = OFFSET_CLOSE
            
        # 委托转换
        reqList = self.mainEngine.convertOrderReq(req)
        vtOrderIDList = []
        
        if not reqList:
            return vtOrderIDList
        
        for convertedReq in reqList:
            vtOrderID = self.mainEngine.sendOrder(convertedReq, contract.gatewayName)    # 发单
            self.orderStrategyDict[vtOrderID] = strategy                                 # 保存vtOrderID和策略的映射关系
            self.strategyOrderDict[strategy.name].add(vtOrderID)                         # 添加到策略委托号集合中
            vtOrderIDList.append(vtOrderID)
            
        self.writeCtaLog(u'策略%s发送委托，%s，%s，%s@%s' 
                         %(strategy.name, vtSymbol, req.direction, volume, price))
        
        return vtOrderIDList
```

         首先，从MainEngine中获得所有合约的数据，然后构建一个用于传递订单信息的类：


```python
req = VtOrderReq()
        req.symbol = contract.symbol
        req.exchange = contract.exchange
        req.vtSymbol = contract.vtSymbol
        req.price = self.roundToPriceTick(contract.priceTick, price)
        req.volume = volume
        
        req.productClass = strategy.productClass
        req.currency = strategy.currency        
        
        # 设计为CTA引擎发出的委托只允许使用限价单
        req.priceType = PRICETYPE_LIMITPRICE
```

         上面这段代码就是构造了一个VtOrder的类，我们看一下这个类是怎么定义的：

```python
class VtOrderReq(object):
    """发单时传入的对象类"""

    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        self.symbol = EMPTY_STRING              # 代码
        self.exchange = EMPTY_STRING            # 交易所
        self.vtSymbol = EMPTY_STRING            # VT合约代码
        self.price = EMPTY_FLOAT                # 价格
        self.volume = EMPTY_INT                 # 数量
    
        self.priceType = EMPTY_STRING           # 价格类型
        self.direction = EMPTY_STRING           # 买卖
        self.offset = EMPTY_STRING              # 开平
        
        # 以下为IB相关
        self.productClass = EMPTY_UNICODE       # 合约类型
        self.currency = EMPTY_STRING            # 合约货币
        self.expiry = EMPTY_STRING              # 到期日
        self.strikePrice = EMPTY_FLOAT          # 行权价
        self.optionType = EMPTY_UNICODE         # 期权类型     
        self.lastTradeDateOrContractMonth = EMPTY_STRING   # 合约月,IB专用
        self.multiplier = EMPTY_STRING                     # 乘数,IB专用
```

         我们发现这个类很简单，其实就是合约信息。同时强行将订单类型设置为限价单。接下来的代码就是CTA接口的一些映射，

```python
# 委托转换
 reqList = self.mainEngine.convertOrderReq(req)
```

         在正式发送订单的时候，有一步委托转换的过程，vnpy作者把这个过程写在了DataEngine里面，代码如下：

首先是调用了MainEngine的转换函数，然后进一步调用dataEngine

```python
def convertOrderReq(self, req):
        """转换委托请求"""
        return self.dataEngine.convertOrderReq(req)
```

         我们发现，dataEngine里面是这样的结果：

```python
def convertOrderReq(self, req):
        """根据规则转换委托请求"""
        detail = self.detailDict.get(req.vtSymbol, None)
        if not detail:
            return [req]
        else:
            return detail.convertOrderReq(req)
```

         讲真的，还是很奇怪的，决定的很多功能为什么要写这么多层次。我们继续追踪一下detail的covertOrderReq方法是什么功能。

         我们查看代码可以只到，detail里面存储的是相关合约的持仓信息：
`detail = PositionDetail(vtSymbol, contract)`
         既然这样，我们看一下PositionDetail类的定义：

```python
class PositionDetail(object):
    """本地维护的持仓信息"""
    WORKING_STATUS = [STATUS_UNKNOWN, STATUS_NOTTRADED, STATUS_PARTTRADED]
    
    MODE_NORMAL = 'normal'          # 普通模式
    MODE_SHFE = 'shfe'              # 上期所今昨分别平仓
    MODE_TDPENALTY = 'tdpenalty'    # 平今惩罚

    #----------------------------------------------------------------------
    def __init__(self, vtSymbol, contract=None):
        """Constructor"""
        self.vtSymbol = vtSymbol
        self.symbol = EMPTY_STRING
        self.exchange = EMPTY_STRING
        self.name = EMPTY_UNICODE    
        self.size = 1
        。。。。。。
        。。。。。。
        。。。。。。
        。。。。。。
```

         nvpy作者的注释说明了这个类的作用，就是用于本地维护持仓信息。

         里面的方法有这么几类吧：
- 信息更新类，比如updateTrade
- 计算类，比尔calculatePosition
- 还有就是我们上面提到的，工具类。

```python
def convertOrderReq(self, req):
        """转换委托请求"""
        # 普通模式无需转换
        if self.mode is self.MODE_NORMAL:
            return [req]
        
        # 上期所模式拆分今昨，优先平今
        elif self.mode is self.MODE_SHFE:
            # 开仓无需转换
            if req.offset is OFFSET_OPEN:
                return [req]
            
            # 多头
            if req.direction is DIRECTION_LONG:
                posAvailable = self.shortPos - self.shortPosFrozen
                tdAvailable = self.shortTd- self.shortTdFrozen
                ydAvailable = self.shortYd - self.shortYdFrozen            
            # 空头
            else:
                posAvailable = self.longPos - self.longPosFrozen
                tdAvailable = self.longTd - self.longTdFrozen
                ydAvailable = self.longYd - self.longYdFrozen
                
            # 平仓量超过总可用，拒绝，返回空列表
            if req.volume > posAvailable:
                return []
            # 平仓量小于今可用，全部平今
            elif req.volume <= tdAvailable:
                req.offset = OFFSET_CLOSETODAY
                return [req]
            # 平仓量大于今可用，平今再平昨
            else:
                l = []
                
                if tdAvailable > 0:
                    reqTd = copy(req)
                    reqTd.offset = OFFSET_CLOSETODAY
                    reqTd.volume = tdAvailable
                    l.append(reqTd)
                    
                reqYd = copy(req)
                reqYd.offset = OFFSET_CLOSEYESTERDAY
                reqYd.volume = req.volume - tdAvailable
                l.append(reqYd)
                
                return l
            
        # 平今惩罚模式，没有今仓则平昨，否则锁仓
        elif self.mode is self.MODE_TDPENALTY:
            # 多头
            if req.direction is DIRECTION_LONG:
                td = self.shortTd
                ydAvailable = self.shortYd - self.shortYdFrozen
            # 空头
            else:
                td = self.longTd
                ydAvailable = self.longYd - self.longYdFrozen
                
            # 这里针对开仓和平仓委托均使用一套逻辑
            
            # 如果有今仓，则只能开仓（或锁仓）
            if td:
                req.offset = OFFSET_OPEN
                return [req]
            # 如果平仓量小于昨可用，全部平昨
            elif req.volume <= ydAvailable:
                if self.exchange is EXCHANGE_SHFE:
                    req.offset = OFFSET_CLOSEYESTERDAY
                else:
                    req.offset = OFFSET_CLOSE
                return [req]
            # 平仓量大于昨可用，平仓再反向开仓
            else:
                l = []
                
                if ydAvailable > 0:
                    reqClose = copy(req)
                    if self.exchange is EXCHANGE_SHFE:
                        reqClose.offset = OFFSET_CLOSEYESTERDAY
                    else:
                        reqClose.offset = OFFSET_CLOSE
                    reqClose.volume = ydAvailable
                    
                    l.append(reqClose)
                    
                reqOpen = copy(req)
                reqOpen.offset = OFFSET_OPEN
                reqOpen.volume = req.volume - ydAvailable
                l.append(reqOpen)
                
                return l
        
        # 其他情况则直接返回空
        return []
```

         我们发现，这个转换是根据交易模式而变的，

```
MODE_NORMAL = 'normal'          # 普通模式
MODE_SHFE = 'shfe'              # 上期所今昨分别平仓
MODE_TDPENALTY = 'tdpenalty'    # 平今惩罚
```

        其实就是根据当前持仓来判断一下这次下单应该怎么样调整，是锁仓还是和之前的仓位轧差之后算差值进行下单。

         介绍完了下单交易的函数，还有剩下的和策略初始化、策略停止、开始有关。我们就来继续看一下。

         首先，我们回到最开始的CtaRunTrading的文件，我们发现：

```python
cta = me.getApp(ctaStrategy.appName)

    cta.loadSetting()
    le.info(u'CTA策略载入成功')

    cta.initAll()
    le.info(u'CTA策略初始化成功')

    cta.startAll()
    le.info(u'CTA策略启动成功')
```

         上面这些代码我们详细来挖掘一下背后调用了什么。

         首先是getApp。这个很简单，就是把策略整个模块给返回出来：

```python
def getApp(self, appName):
        """获取APP引擎对象"""
        return self.appDict[appName]
```

         然后，我们拿到的其实就是前面讲的这样的一个ctaEngine的类，然后分别调用这个类的loadingSetting方法、initAll方法和startAll方法。

         我们就按照这个顺序来看一下：

```python
def loadSetting(self):
        """读取策略配置"""
        with open(self.settingfilePath) as f:
            l = json.load(f)
            
            for setting in l:
                self.loadStrategy(setting)
```

 首先是打开一个配置文件，我们可以在类定义的一开始看一下文件的路径在哪里：

```
settingFileName = 'CTA_setting.json'
settingfilePath = getJsonPath(settingFileName, __file__)
```

我们就来看一下这个文件里面是什么：

```
[


    {
        "name": "atr rsi",
        "className": "AtrRsiStrategy",
        "vtSymbol": "rb1901"
    },

    {
        "name": "king keltner",
        "className": "KkStrategy",
        "vtSymbol": "rb1901"
    }
]
```

         这个我们在最开始的文章中讲过，这边就衔接起来了。我们看到，读取了设置文件之后，就会开始调用loadStrategy方法，并把每一个setting中的部分传给这个方法。我们看一下loadStrategy的代码：

```python
def loadStrategy(self, setting):
        """载入策略"""
        try:
            name = setting['name']
            className = setting['className']
        except Exception:
            msg = traceback.format_exc()
            self.writeCtaLog(u'载入策略出错：%s' %msg)
            return
        
        # 获取策略类
        strategyClass = STRATEGY_CLASS.get(className, None)
        if not strategyClass:
            self.writeCtaLog(u'找不到策略类：%s' %className)
            return
        
        # 防止策略重名
        if name in self.strategyDict:
            self.writeCtaLog(u'策略实例重名：%s' %name)
        else:
            # 创建策略实例
            strategy = strategyClass(self, setting)  
            self.strategyDict[name] = strategy
            
            # 创建委托号列表
            self.strategyOrderDict[name] = set()
            
            # 保存Tick映射关系
            if strategy.vtSymbol in self.tickStrategyDict:
                l = self.tickStrategyDict[strategy.vtSymbol]
            else:
                l = []
                self.tickStrategyDict[strategy.vtSymbol] = l
            l.append(strategy)
```

         首先，解析出setting里面的策略名称和策略类的名称，然后根据策略类的名称来获得这个策略。那么STRATEGY_CLASS是什么呢？

![](https://img-blog.csdnimg.cn/2018120915582671.JPG)

         我们查看一下这个init文件之后就可以知道，里面运行的就是对这个变量进行赋值。里面的代码就不说了，总之就是遍历一下文件夹下面的代码。说真的，这样的写法笔者是不敢恭维的。不过目前阶段我们属于学习源码的阶段，后续笔者可能会改写一个个人觉得比较合理的版本。

         我们回来，这个STRATEGY_CLASS其实就是保存了我们写的策略的代码。nvpy自带的策略的源码就在这个strategy文件夹下面：
![](https://img-blog.csdnimg.cn/20181209160349159.JPG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3F0bHl4,size_16,color_FFFFFF,t_70)

         说白了，其实就是按照需要的策略，我们在init文件里面进行动态import，存储起来。

```python
# 遍历模块下的对象，只有名称中包含'Strategy'的才是策略类
        for k in dir(module):
            if 'Strategy' in k:
                v = module.__getattribute__(k)
                STRATEGY_CLASS[k] = v
```

         我们看到了这个STRATEGY_CLASS的value是我们写的strategy实现的py文件，后续会讲解这个py文件怎么写。说白了，loadingStrategy的作用就是把我们写的策略实现的py文件做一个载入。其实我觉得这样的设计方法是很好的，可以做到很好的隔离作用，这样实盘和回测都是同一个策略类，但是作用确有两处。

         然后是initAll方法。

```python
def initAll(self):
        """全部初始化"""
        for name in self.strategyDict.keys():
            self.initStrategy(name)
```

         其实就是遍历了每一个写好的策略类，然后调用初始化方法将其初始化。

```python
def initStrategy(self, name):
        """初始化策略"""
        if name in self.strategyDict:
            strategy = self.strategyDict[name]
            
            if not strategy.inited:
                self.callStrategyFunc(strategy, strategy.onInit)
                strategy.inited = True
                
                self.loadSyncData(strategy)                             # 初始化完成后加载同步数据
                self.subscribeMarketData(strategy)                      # 加载同步数据后再订阅行情
            else:
                self.writeCtaLog(u'请勿重复初始化策略实例：%s' %name)
        else:
            self.writeCtaLog(u'策略实例不存在：%s' %name)
```

         我们可以来看一下过程，首先是从存储策略的字典中获取策略类自身，然后检查是否被初始化了，如果没有，则使用calltrategyFunc函数来进行初始化。我们来看一下这个函数是什么：

```python
def callStrategyFunc(self, strategy, func, params=None):
        """调用策略的函数，若触发异常则捕捉"""
        try:
            if params:
                func(params)
            else:
                func()
        except Exception:
            # 停止策略，修改状态为未初始化
            strategy.trading = False
            strategy.inited = False
            
            # 发出日志
            content = '\n'.join([u'策略%s触发异常已停止' %strategy.name,
                                traceback.format_exc()])
            self.writeCtaLog(content)
```

         其实这个函数有点像一个装饰器，本质上调用的就是策略类的onInit（）方法。至于关于策略类的方法我们留在后面讲。总之，这里看上去似乎更加复杂了，最后本质上还是调用了一个方法罢了，不过把异常捕捉起来。其实我觉得这样设计是合理的，还是之前的原因，策略归策略，其他的都应该隔离开来，包括异常检测之类的，很多情况不应该在策略编写的过程中考虑进去。固然捕捉异常放在策略类里面的初始化函数也是可以的，但是在逻辑上似乎就不那么解耦合了。

         然后是loadSyncData方法，其实就是从数据库读取属于这个策略的持仓；最后是订阅策略行情。我们的策略是作用于特定的合约上的，所以需要策略类通过ctp来进行行情的订阅：subscribeMarketData。

         到这里，基本的除了策略类之外的主要逻辑关系都梳理了一下，可能一下子有点绕，但是多看几遍，画几个图，大概就能理解了。









