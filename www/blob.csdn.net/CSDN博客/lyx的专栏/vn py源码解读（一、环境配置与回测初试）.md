# vn.py源码解读（一、环境配置与回测初试） - lyx的专栏 - CSDN博客





2017年11月12日 21:50:37[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：11135
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)









        近来忙于毕业找工作，也不知道能不能继续在量化界混了。周末比较闲，抽空研究了一下vn.py。有人说，为什么学那么多的回测平台呀。其实我个人觉得，做cta的话，两个回测平台还是要的，这样，当你的策略出现和你预计不符，而你有无法在代码逻辑层面找到问题的时候，你就可以用另外一个平台试一下，来看看到底是你的策略本身就不行，还是你的代码有着当前水平无法察觉的问题，甚至，可能回测平台本身存在一个bug。所以笔者之前学习的backtrader和pyalgotrade的目的就是这个，但是后续对于pyalgotrade没怎么用。前段时间看到vn.py和某Q开头的开源项目在网上开战，刚入门python的小朋友可能还不知道他们争论的是什么。

        闲话不说，总而言之，个人觉得，vn.py还是一个很不错的期货cta的平台，而且，不仅仅局限于回测，还可以直接当一个交易系统使用，整个代码框架也是相当不错的，只是似乎对刚入门的小朋友不是特别友好，入门级别的教程比较少。

# 1.安装mongodb

        vn.py用的数据库是mongdb，笔者也不知道为什么要这样，是为了速度么还是单纯表示与众不同的。说真的，笔者还是习惯用传统的sql数据库，不过还好，mongodb也入过门。怎么安装就不说了，安装好之后别忘了开启服务。

# 2.测试一下

        笔者使用的nv.py-1.7版本，本文主要介绍一下回测功能吧。在下载下来的文件夹下面有一个examples文件夹，里面有一个CtaBacktesting文件，里面有一个notebook，一个IF的分钟数据，还有三个python文件。

        我们先来运行一下loadCsv这个python文件，如果一切顺利的话，我们可以看到数据被写入mongdb的信息不断print出来。这个文件的功能就是告诉我们如何把数据存到数据库里。仔细看一下源码，其实可以发现，vn.py还是支持很多数据格式的，什么通达信上直接导到本地的数据啊之类的，功能还是很全的，很接地气。相对于国外的开源项目来说，基本都是要自己写数据格式处理程序的。有好处也有坏处吧。

        然后我们就可以打开notebook文件，然后不断运行就可以了。

        这里，笔者用DualThrust来作为例子讲解一下，毕竟这么经典的策略，大家很容易找到原理，也容易理解。

# 3.例子

        和别的回测项目一样，我们要现有一个回测的核心，在vn.py中叫做engine，引擎，还是比较好理解的。



```python
engine = BacktestingEngine()
```



        然后自然的，就是进行一些设置了。

        设置的话，那么无非就是数据和回测方式。数据的话，设置是使用tick模式还是bar模式，笔者目前还没有研究tick模式，而且我们的数据也是分钟bar，所以选择BAR_MODE，然后就是设置数据库的名称，vn.py已经定义好了名称了，当然，我们可以按照我们的数据品种自己定义。



```python
# 设置回测使用的数据
engine.setBacktestingMode(engine.BAR_MODE)    # 设置引擎的回测模式为K线
engine.setDatabase(MINUTE_DB_NAME, 'IF0000')  # 设置使用的历史数据库
engine.setStartDate('20120101')               # 设置回测用的数据起始日期
engine.setEndDate('20120201')
# 配置回测引擎参数
engine.setSlippage(0.2)     # 设置滑点为股指1跳
engine.setRate(0.3/10000)   # 设置手续费万0.3
engine.setSize(300)         # 设置股指合约大小 
engine.setPriceTick(0.2)    # 设置股指最小价格变动   
engine.setCapital(1000000)  # 设置回测本金
```

注：本文出现的大部分代码都是来自于vn.py的notebook文件里面自带的，并不是本人编写的。



接下来就是最核心的策略部分了。和很多开源平台一样，策略的封装就是一个类。



```python
from vnpy.trader.app.ctaStrategy.strategy.strategyDualThrust import DualThrustStrategy
```



vn.py的作者已经写了一个DualThrust的策略了。



然后就是设置策略的参数，策略的参数是以字典形式传入的。



```python
# 在引擎中创建策略对象
d = {}                     # 策略参数配置
engine.initStrategy(DualThrustStrategy, d)    # 创建策略对象
# 运行回测
engine.runBacktesting()          # 运行回测
```



上面的d就是。至于有什么参数，则与策略类的编写有关。



# 4.策略类

整个DualThrust的实现就不说了，大家可以去文件里面找怎么实现的。主要说一下框架吧。



```python
# 参数列表，保存了参数的名称
    paramList = ['name',
                 'className',
                 'author',
                 'vtSymbol',
                 'k1',
                 'k2']
```



在类里面，定义了参数表。我们知道，DualThrust如果只考虑前面一天的价格的话，也就是N为1的话，其实参数就是两个，上轨K和下轨K，也就是这里的K1和K2，大家修改上面的参数字典d就可以了。不传参数的话，当然就是默认参数。



然后就是所有事件驱动类型的回测框架都会有的那个触发函数了，也就是我们所有逻辑的核心。



```python
def onBar(self, bar):
        print 'on_bar method'
        print bar.datetime.date()
        """收到Bar推送（必须由用户继承实现）"""
        # 撤销之前发出的尚未成交的委托（包括限价单和停止单）
        for orderID in self.orderList:
            self.cancelOrder(orderID)
        self.orderList = []

        # 计算指标数值
        self.barList.append(bar)
        
        if len(self.barList) <= 2:
            return
        else:
            self.barList.pop(0)
        lastBar = self.barList[-2]
        
        # 新的一天
        if lastBar.datetime.date() != bar.datetime.date():
            # 注意，这段代码其实只在改变日期那一天执行一次
            # 如果已经初始化
            if self.dayHigh:
                self.range = self.dayHigh - self.dayLow
                self.longEntry = bar.open + self.k1 * self.range
                self.shortEntry = bar.open - self.k2 * self.range           
                
            self.dayOpen = bar.open
            self.dayHigh = bar.high
            self.dayLow = bar.low

            self.longEntered = False
            self.shortEntered = False
        else:
            self.dayHigh = max(self.dayHigh, bar.high)
            self.dayLow = min(self.dayLow, bar.low)

        # 尚未到收盘
        if not self.range:
            return

        if bar.datetime.time() < self.exitTime:
            if self.pos == 0:
                if bar.close > self.dayOpen:
                    if not self.longEntered:
                        vtOrderID = self.buy(self.longEntry, self.fixedSize, stop=True)
                        self.orderList.append(vtOrderID)
                else:
                    if not self.shortEntered:
                        vtOrderID = self.short(self.shortEntry, self.fixedSize, stop=True)
                        self.orderList.append(vtOrderID)
    
            # 持有多头仓位
            elif self.pos > 0:
                self.longEntered = True

                # 多头止损单
                vtOrderID = self.sell(self.shortEntry, self.fixedSize, stop=True)
                self.orderList.append(vtOrderID)
                
                # 空头开仓单
                if not self.shortEntered:
                    vtOrderID = self.short(self.shortEntry, self.fixedSize, stop=True)
                    self.orderList.append(vtOrderID)
                
            # 持有空头仓位
            elif self.pos < 0:
                self.shortEntered = True

                # 空头止损单
                vtOrderID = self.cover(self.longEntry, self.fixedSize, stop=True)
                self.orderList.append(vtOrderID)
                
                # 多头开仓单
                if not self.longEntered:
                    vtOrderID = self.buy(self.longEntry, self.fixedSize, stop=True)
                    self.orderList.append(vtOrderID)  
            
        # 收盘平仓
        else:
            if self.pos > 0:
                vtOrderID = self.sell(bar.close * 0.99, abs(self.pos))
                self.orderList.append(vtOrderID)
            elif self.pos < 0:
                vtOrderID = self.cover(bar.close * 1.01, abs(self.pos))
                self.orderList.append(vtOrderID) 
 
        # 发出状态更新事件
        self.putEvent()
```



       在vn.py中叫onBar这个函数。逻辑还是很简单的。



        总而言之，这是一个不错的工具，二DualThrust其实也是一个相对不错的策略，和还有一个也是日内交易的策略，忘记叫啥了，不但简单，而且效果巨好。后期如果找到工作了，还在量化界混的话，写个教程，如何在树莓派上跑几个这样的策略，然后放在印钱（血本无归）。



