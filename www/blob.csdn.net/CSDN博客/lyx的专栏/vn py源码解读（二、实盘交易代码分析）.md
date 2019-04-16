# vn.py源码解读（二、实盘交易代码分析） - lyx的专栏 - CSDN博客





2018年12月01日 12:03:04[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：915
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)









        离上一篇和vnpy有关的文章整整一年了。这一年似乎过得异常的快，快到让人觉得没有成长。可能是工作原因吧，时间一下子就会过去；亦或是自己懈怠了。

        一年前vnpy网上的教程还很少，而现在渐渐多了起来，量化交易学习的人群也渐渐多了起来了吧。

之前的文章简单介绍了一下vnpy的配置和回测的代码的简单解析。其实vnpy对我的吸引力在回测功能上面几乎没有。说句真心话，回测框架比vnpy好的框架太多了，但是都是国外的，能够和ctp结合，进行实盘的，目前看来只有大商所的飞创还可以自己写写代码，但是是java的，虽然笔者也能写写java，但是策略编写太复杂，框架设计的时候不够友好。而且，java扩展功能太不方便。所以nvpy可能是一个比较好的选择吧，至少可以作为一个学习对象。

        所以这一篇将重点讲述一下实盘部分的代码。具体在nvpy项目文件下的example里面有一个CtaTrading。

        我们先来看一下代码吧，我们不关心别的，只关心runChildProcess这个子进程函数，这个里面完成了一个策略的运行前准备到策略开始的全过程。

```python
def runChildProcess():
    """子进程运行函数"""
    print('-'*20)
    
    # 创建日志引擎
    le = LogEngine()
    le.setLogLevel(le.LEVEL_INFO)
    le.addConsoleHandler()
    le.addFileHandler()
    
    le.info(u'启动CTA策略运行子进程')
    
    ee = EventEngine2()
    le.info(u'事件引擎创建成功')
    
    me = MainEngine(ee)
    me.addGateway(ctpGateway)
    me.addApp(ctaStrategy)
    le.info(u'主引擎创建成功')
    
    ee.register(EVENT_LOG, le.processLogEvent)
    ee.register(EVENT_CTA_LOG, le.processLogEvent)
    ee.register(EVENT_ERROR, processErrorEvent)
    le.info(u'注册日志事件监听')
    
    me.connect('CTP')
    le.info(u'连接CTP接口')
    
    sleep(10)                       # 等待CTP接口初始化
    me.dataEngine.saveContracts()   # 保存合约信息到文件
    
    cta = me.getApp(ctaStrategy.appName)
    
    cta.loadSetting()
    le.info(u'CTA策略载入成功')
    
    cta.initAll()
    le.info(u'CTA策略初始化成功')
    
    cta.startAll()
    le.info(u'CTA策略启动成功')
    
    while True:
        sleep(1)
```

        我们来拆分一下步骤。

        假设我们现在是一个写程序的人，那么现在我们要实现一个策略的实盘运行，我想大框架的步骤应该分成这样几个：
- 策略运行前的设置
- 策略载入
- 策略执行

        那么vnpy是一个事件驱动类型的框架，所以策略载入前的设置包含事件驱动引擎的初始化。

        好，那么我们来看一下具体是怎么实现的。

a.运行环境设置，初始化日志生成模块。这个比较好理解，我们需要一个日志文件来监控我们策略的执行

```
# 创建日志引擎
    le = LogEngine()
    le.setLogLevel(le.LEVEL_INFO)
    le.addConsoleHandler()
    le.addFileHandler()
    
    le.info(u'启动CTA策略运行子进程')
```

b.初始化事件引擎

        有的同学可能不知道什么是事件引擎。大家可以这么想，所谓的事件驱动的回测框架，就是把行情数据的到来当成是事件，那么事件引擎的作用就是被事件叫起来，并执行相应的动作。

```python
ee = EventEngine2()
le.info(u'事件引擎创建成功')
```

c.需要一个大的类来把策略、ctp行情、事件引擎comgbine起来。nvpy的作者把这个叫做主引擎（MainEngine），个人觉得不太适合叫做Engine，不过也无伤大雅吧。

```python
me = MainEngine(ee)
    me.addGateway(ctpGateway)
    me.addApp(ctaStrategy)
    le.info(u'主引擎创建成功')
    
    ee.register(EVENT_LOG, le.processLogEvent)
    ee.register(EVENT_CTA_LOG, le.processLogEvent)
    ee.register(EVENT_ERROR, processErrorEvent)
    le.info(u'注册日志事件监听')
    
    me.connect('CTP')
    le.info(u'连接CTP接口')
    
    sleep(10)                       # 等待CTP接口初始化
    me.dataEngine.saveContracts()   # 保存合约信息到文件
```

        这里其实完成的东西有点多，首先，把ee这个事件引擎的实例挂上去，然后把ctp挂上去，说是挂ctp，其实是挂数据接口。nvpy数据流的传送是一个很有意思的东西，下一次讲吧。大家也可以看看vnpy作者的知乎文章：[https://zhuanlan.zhihu.com/p/32848878](https://zhuanlan.zhihu.com/p/32848878)讲的有点简略，笔者下一篇详细展开讲一下。

        然后把策略挂上去。挂策略的时候可能有点懵，这样就挂上去了？策略的设置在文件夹的CTA_setting里面。

```
[
    {
        "name": "double ema",
        "className": "EmaDemoStrategy",
        "vtSymbol": "IF1706"
    },

    {
        "name": "atr rsi",
        "className": "AtrRsiStrategy",
        "vtSymbol": "IC1706"
    },

    {
        "name": "king keltner",
        "className": "KkStrategy",
        "vtSymbol": "IH1706"
    }
]
```

        可以修改、删除、增加策略的名称和策略对应的标的。其中，className对应的是vnpy/trader/app/ctaStrategy/strategy下的策略py文件里面的strategy类的名称。比如：

```python
class KkStrategy(CtaTemplate):
    """基于King Keltner通道的交易策略"""
    className = 'KkStrategy'
    author = u'用Python的交易员'

    # 策略参数
    kkLength = 11           # 计算通道中值的窗口数
    kkDev = 1.6             # 计算通道宽度的偏差
    trailingPrcnt = 0.8     # 移动止损
    initDays = 10           # 初始化数据所用的天数
    fixedSize = 1           # 每次交易的数量
```

        strategyKingKeltner.py中的部分代码如上。

        东西都挂完之后对事件驱动的引擎做一些log事件注册。

```python
ee.register(EVENT_LOG, le.processLogEvent)
    ee.register(EVENT_CTA_LOG, le.processLogEvent)
    ee.register(EVENT_ERROR, processErrorEvent)
    le.info(u'注册日志事件监听')
```

d.做好了设置之后，其实策略的初始化和策略的执行很简单。

```python
cta = me.getApp(ctaStrategy.appName)
    
    cta.loadSetting()
    le.info(u'CTA策略载入成功')
    
    cta.initAll()
    le.info(u'CTA策略初始化成功')
    
    cta.startAll()
    le.info(u'CTA策略启动成功')
```





