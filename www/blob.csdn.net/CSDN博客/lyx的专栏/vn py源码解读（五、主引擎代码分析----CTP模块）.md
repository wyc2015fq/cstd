# vn.py源码解读（五、主引擎代码分析----CTP模块） - lyx的专栏 - CSDN博客





2018年12月07日 19:07:05[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：517
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)









        上一篇文章讲了MainEngine中的初始化函数，重点是DataEngine的讲解。有了对行情数据的处理，还需要有行情数据的来源。在MainEngine的初始化函数后面的一个函数就是addGateway函数。vnpy的作者还是有很大格局的，希望自己的作品可以兼容很多数据交易接口，所以就比较通用，这里就把ctp这种接口叫做Gateway，而且后面写的也比较复杂，目的就是为了集大成，还是很佩服的。这里我们就来分析一下数据交易接口这边是怎么挂到MainEngine上，以及怎么工作的。

## 1.addGateway

```python
def addGateway(self, gatewayModule):
        """添加底层接口"""
        gatewayName = gatewayModule.gatewayName
        
        # 创建接口实例
        self.gatewayDict[gatewayName] = gatewayModule.gatewayClass(self.eventEngine, 
                                                                   gatewayName)
        
        # 设置接口轮询
        if gatewayModule.gatewayQryEnabled:
            self.gatewayDict[gatewayName].setQryEnabled(gatewayModule.gatewayQryEnabled)
                
        # 保存接口详细信息
        d = {
            'gatewayName': gatewayModule.gatewayName,
            'gatewayDisplayName': gatewayModule.gatewayDisplayName,
            'gatewayType': gatewayModule.gatewayType
        }
        self.gatewayDetailList.append(d)
```

        这个方法还是很简单的，不难，在一开始获得一个叫做gatewayModule的名称，然后通过这个gatewayModule来获得一个接口的实例，然后设置轮询，然后把接口数据保存到用于存储接口信息的列表上。

        好，那么我们一步步来看。首先，什么是gatewayModule,然后是是这个获得的接口实例是什么？轮询是怎么设置的？

## 2.gatewayModule

        我们来回顾一下runCtangTrading的代码。
`me.addGateway(ctpGateway)`
        这里的ctpGateway就是这个所谓的gatewayModule。这里，ctpGateway其实是一个文件夹的名称，我们都知道，其实在python中，一个文件夹下面如果有一个__init__.py文件的话，那么这个文件夹就不是一个简单的文件夹，而是一个package，而__init__.py文件某种意义上就可以认为是这个package的一个初始化（这里说的不是特别准确，为了方便大家理解）。所以，我们找到这个文件夹，并打开__init__.py来看一下。

```python
from __future__ import absolute_import
from vnpy.trader import vtConstant
from .ctpGateway import CtpGateway

gatewayClass = CtpGateway
gatewayName = 'CTP'
gatewayDisplayName = 'CTP'
gatewayType = vtConstant.GATEWAYTYPE_FUTURES
gatewayQryEnabled = True
```

        到这里，我们就知道传递进去的gatewayModule是什么了，说白了就是这个package。

## 3.接口的实例

```python
# 创建接口实例
self.gatewayDict[gatewayName] = gatewayModule.gatewayClass(self.eventEngine, 
                                                                   gatewayName)
```

        我们发现，加下来，程序利用了我们的gatewayModule来构建了一个接口实例，并存到一个字典里面。显然，这里的gatewayName就是上面init文件中的gatewayName，而

gatewayModule.gatewayClass(self.eventEngine,  gatewayName)，其实就是init里面的CtpGateway（self.eventEngine,  gatewayName），那么我们看一下这个CtpGateway是怎么写的就知道这个东西是怎么实例化的了。

```python
class CtpGateway(VtGateway):
    """CTP接口"""

    #----------------------------------------------------------------------
    def __init__(self, eventEngine, gatewayName='CTP'):
        """Constructor"""
        super(CtpGateway, self).__init__(eventEngine, gatewayName)
        
        self.mdApi = CtpMdApi(self)     # 行情API
        self.tdApi = CtpTdApi(self)     # 交易API
        
        self.mdConnected = False        # 行情API连接状态，登录完成后为True
        self.tdConnected = False        # 交易API连接状态
        
        self.qryEnabled = False         # 循环查询
        
        self.fileName = self.gatewayName + '_connect.json'
        self.filePath = getJsonPath(self.fileName, __file__)
```

        首先，这个类继承了VtGateway而来的，其次，我们简单看一下初始化函数，还是比较容易看明白的，分别初始化了ctp的行情前置（MarketFront）和交易前置（TradeFront），作者叫做行情API和交易API，都可以。然后是初始化了一些ctp的控制常量和ctp配置文件的地址。

        我们先不看这个类下面还有别的什么，我们先来看一下它的父类VtGateway吧。

```python
class VtGateway(object):
    """交易接口"""

    #----------------------------------------------------------------------
    def __init__(self, eventEngine, gatewayName):
        """Constructor"""
        self.eventEngine = eventEngine
        self.gatewayName = gatewayName
```

        没有什么特别的，初始化的时候，把事件引擎和名称给挂上。但是后面别的方法都特别重要。

```python
def onTick(self, tick):
        """市场行情推送"""
        # 通用事件
        event1 = Event(type_=EVENT_TICK)
        event1.dict_['data'] = tick
        self.eventEngine.put(event1)
        
        # 特定合约代码的事件
        event2 = Event(type_=EVENT_TICK+tick.vtSymbol)
        event2.dict_['data'] = tick
        self.eventEngine.put(event2)
```

        我们看一下这个onTick方法，首先，这个方法接受的是一个tick，然后创造了两个事件，一个是EVENT_TICK类型的事件，一个是针对特定交易代码的事件。这个父类当中还有很多别的方法们比如onTrade，都是这样的功能，用于接受一个变量，然后把这个变量包装成一个事件put出去，也就是发布事件。

```python
def onTrade(self, trade):
        """成交信息推送"""
        # 通用事件
        event1 = Event(type_=EVENT_TRADE)
        event1.dict_['data'] = trade
        self.eventEngine.put(event1)
        
        # 特定合约的成交事件
        event2 = Event(type_=EVENT_TRADE+trade.vtSymbol)
        event2.dict_['data'] = trade
        self.eventEngine.put(event2)        
    
    #----------------------------------------------------------------------
    def onOrder(self, order):
        """订单变化推送"""
        # 通用事件
        event1 = Event(type_=EVENT_ORDER)
        event1.dict_['data'] = order
        self.eventEngine.put(event1)
        
        # 特定订单编号的事件
        event2 = Event(type_=EVENT_ORDER+order.vtOrderID)
        event2.dict_['data'] = order
        self.eventEngine.put(event2)
```

        所以，到这里我们就明白了，谁调用了这几个函数，谁就是在发布事件。

        我们回到初始化函数，当中有两个API的类，CtpMdApi和CtpTdApi，那么我们来看一下，

```python
class CtpMdApi(MdApi):
    """CTP行情API实现"""

    #----------------------------------------------------------------------
    def __init__(self, gateway):
        """Constructor"""
        super(CtpMdApi, self).__init__()
```

```python
class CtpTdApi(TdApi):
    """CTP交易API实现"""
    
    #----------------------------------------------------------------------
    def __init__(self, gateway):
        """API对象的初始化函数"""
        super(CtpTdApi, self).__init__()
```

        大家注意到，这两个类都各自继承了父类MdApi和TdApi，而这两个类的代码，大家是看不到的，是ctp编译之后的，只能import使用，而不知道其代码。我们看一下vnpy/api/ctp下面的init的代码：

```python
from __future__ import absolute_import
from .vnctpmd import MdApi
from .vnctptd import TdApi
from .ctp_data_type import defineDict
```

        而vnctpmd文件夹里面是没有MdApi这个py文件的。后面有机会讲ctp封装的时候给大家解释这个。

        讲了这么多，有点乱了，我们回到最开始的addGateway，
`self.gatewayDict[gatewayName].setQryEnabled(gatewayModule.gatewayQryEnabled)`
        我们来看一下这个setQryEnabled方法，也就是CtpGateway类中的这个方法，我们发现，就是修改了一个控制量而已，把这个控制量设置为init文件里面的qryEnabled，也就是True。

```python
def setQryEnabled(self, qryEnabled):
        """设置是否要启动循环查询"""
        self.qryEnabled = qryEnabled
```

        这个就没什么特别的了。

        然后我们看一下MainEngine中别的和gateway有关的函数。

```python
def getGateway(self, gatewayName):
        """获取接口"""
        if gatewayName in self.gatewayDict:
            return self.gatewayDict[gatewayName]
        else:
            self.writeLog(text.GATEWAY_NOT_EXIST.format(gateway=gatewayName))
            return None
```

        很显然，其实就是把CtpGateway这个实例化了的类返回出去了。

## 4.接口的连接

        然后是接口的connect函数，也就是连接函数

```python
def connect(self, gatewayName):
        """连接特定名称的接口"""
        gateway = self.getGateway(gatewayName)
        
        if gateway:
            gateway.connect()
            
            # 接口连接后自动执行数据库连接的任务
            self.dbConnect()
```

        我们看到，就是调用了CtpGateway类的connect方法。我们再回去看一下这个方法是怎么样的。

```python
def connect(self):
        """连接"""
        try:
            f = file(self.filePath)
        except IOError:
            log = VtLogData()
            log.gatewayName = self.gatewayName
            log.logContent = text.LOADING_ERROR
            self.onLog(log)
            return
        
        # 解析json文件
        setting = json.load(f)
        try:
            userID = str(setting['userID'])
            password = str(setting['password'])
            brokerID = str(setting['brokerID'])
            tdAddress = str(setting['tdAddress'])
            mdAddress = str(setting['mdAddress'])
            
            # 如果json文件提供了验证码
            if 'authCode' in setting: 
                authCode = str(setting['authCode'])
                userProductInfo = str(setting['userProductInfo'])
                self.tdApi.requireAuthentication = True
            else:
                authCode = None
                userProductInfo = None

        except KeyError:
            log = VtLogData()
            log.gatewayName = self.gatewayName
            log.logContent = text.CONFIG_KEY_MISSING
            self.onLog(log)
            return            
        
        # 创建行情和交易接口对象
        self.mdApi.connect(userID, password, brokerID, mdAddress)
        self.tdApi.connect(userID, password, brokerID, tdAddress, authCode, userProductInfo)
        
        # 初始化并启动查询
        self.initQuery()
```

        前面一大部分呢就是获取ctp连接设置的参数，就不用多说了，我们看最后几行：

```python
# 创建行情和交易接口对象
        self.mdApi.connect(userID, password, brokerID, mdAddress)
        self.tdApi.connect(userID, password, brokerID, tdAddress, authCode, userProductInfo)
        
        # 初始化并启动查询
        self.initQuery()
```

        我们看到，调用了前面说的直接封装ctp的类的connect方法，我们来看一下，这两个方法是怎么样的。以MdApi，也就是行情前置为例，我们来看一下。

```python
def connect(self, userID, password, brokerID, address):
        """初始化连接"""
        self.userID = userID                # 账号
        self.password = password            # 密码
        self.brokerID = brokerID            # 经纪商代码
        self.address = address              # 服务器地址
        
        # 如果尚未建立服务器连接，则进行连接
        if not self.connectionStatus:
            # 创建C++环境中的API对象，这里传入的参数是需要用来保存.con文件的文件夹路径
            path = getTempPath(self.gatewayName + '_')
            self.createFtdcMdApi(path)
            
            # 注册服务器地址
            self.registerFront(self.address)
            
            # 初始化连接，成功会调用onFrontConnected
            self.init()
            
        # 若已经连接但尚未登录，则进行登录
        else:
            if not self.loginStatus:
                self.login()
```

        前面同样是做了一大段的设置，虽然我个人觉得，这个真的没有必要这么复杂，类内部的变量太多了。我们看一下过程，如果连接状态是没有连接，那么就开始连接，先获取temp文件夹的路径，没有什么特别的，用于后续文件的存放。然后调用了createFtdcMdApi方法。

        这个createFtdcMdApi方法我们在类中是找不到定义的，所以说明是父类提供的方法，子类继承过来了罢了。所以，我们不需要知道什么，只需要知道这行代码是创造了一个API对象，为了后面可以使用ctp。后面的几个方法也都是在父类中定义的，我们只需要知道功能即可。其中，init（）方法会调用onFrontConnected，我们看一下这个方法：

```python
def onFrontConnected(self):
        """服务器连接"""
        self.connectionStatus = True
    
        self.writeLog(text.TRADING_SERVER_CONNECTED)
        
        if self.requireAuthentication:
            self.authenticate()
        else:
            self.login()
```

        我们发现，在这个方法中，分别是进行了连接状态的更新，日志的写入，以及如果通过了验证码（一般没有这个），那么就登入。

        交易前置也是一样的道理，在CTP中交易接口和行情接口是分开的。这样我们就讲完了接口的连接。

        同样的，行情订阅的结构也是这样类似的，在这里就不多赘述了。













