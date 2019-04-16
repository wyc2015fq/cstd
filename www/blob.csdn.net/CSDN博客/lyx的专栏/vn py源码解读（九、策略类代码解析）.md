# vn.py源码解读（九、策略类代码解析） - lyx的专栏 - CSDN博客





2019年04月05日 19:01:59[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：72
个人分类：[量化投资																[Python](https://blog.csdn.net/qtlyx/article/category/5969093)](https://blog.csdn.net/qtlyx/article/category/6276672)








      说到这个最重要的类了。这个类说白了就是策略的实现。和绝大部分回测框架一样，策略想法是一个类的抽象，一般会继承一个基础类模板，每一个真实运行的策略就是这个策略想法类的一个实例。好的，有点绕。我们来看代码吧。

      在vn.py中，每一个策略类开始不出意外都是下面这样的：

## 1、类的定义和类变量

```python
class TRStrategy(CtaTemplate):
    """学习版本"""
    className = 'TRStrategy'
    author = u'lyx'
```

      首先，这个策略继承了一个叫做CtaTemplate的父类，然后设定了策略的名称和作者。这里，在nvpy的框架下，策略的className要和策略的类名一样。

      vnpy作者的例子中，喜欢用类变量来设置一些策略的参数，个人觉得其实不是很合适。Vnpy中的作者的demo一般直接在后面会有策略的变量：

```python
atrLength = 22          # 计算ATR指标的窗口数   
    atrMaLength = 10        # 计算ATR均线的窗口数
    rsiLength = 5           # 计算RSI的窗口数
    rsiEntry = 16           # RSI的开仓信号
```

      也就是说这些变量是类变量，而不是实例变量。如果大家的python基础不太好的话。。。所谓的类变量就是这个变量保存在类的存储空间中，只要有一个地方改变了，所有这个类的实例都会变；但是实例类是属于实例的内存空间的，实例1和实例2的实例变量没有半毛钱关系，但是如果在一个实例中改变了对于的类变量，那么另一个实例的类变量的值也是会变的，因为，保存在类的变量空间中！这也就是为什么个人觉得这些参数放在初始化函数中更加合适的原因了，要不然有可能会有意外的情况发生。所以，我们就来看一下初始化函数吧。

## 2.类的初始化函数

```python
def __init__(self, ctaEngine, setting):
        """Constructor"""
        super(TRStrategy, self).__init__(ctaEngine, setting)
        
        # 创建K线合成器对象
        self.bg = BarGenerator(self.onBar)
        self.bg30 = BarGenerator(self.onBar, 30, self.on30minBar)
        self.am = ArrayManager()
```

      初始化函数比较有意思，会干三件事情，首先是获得一个ctaEngine，也就是把cta引擎放进去，这个可以是回测的也可以是实盘的。这里，会把这个回测引擎绑定进来，并且传给父类。但是这个其实是有问题的，最好不要这么显示的传递。后面就是构建了k线合成器和k线管理器。其中，k线管理器是后面策略的核心。

## 3、策略的初始化函数

      然后是初始化，不是类的初始化，而是策略的初始化函数。这个很好理解，初始化一个策略那么就是很简单的把参数初始化，同时把需要初始化的数据读出来。注意到一点，这里通过loadBar函数获取初始化需要的数据之后调用onBar函数。那么回测的逻辑相对好理解，实盘的逻辑呢？实盘的时候会去数据库里面找需要长度的历史行情数据，所以在实盘的时候，其实数据库是一个离不开的东西。（后面这里详细关注一下）

```python
def onInit(self):
        """初始化策略（必须由用户继承实现）"""
        self.writeCtaLog(u'%s策略初始化' % self.name)
        # 载入历史数据，并采用回放计算的方式初始化策略数值
        initData = self.loadBar(self.initDays)  # 这里的这个数据就是之前的数据，也就是从数据库或者本地获取的初始化的数据，
        for bar in initData:
            self.onBar(bar)

        self.putEvent()  # 回测中忽略这一方法
```

## 4、策略开始于结束的回调函数 

```python
def onStart(self):
        """启动策略（必须由用户继承实现）"""
        self.writeCtaLog(u'%s策略启动' % self.name)
        self.putEvent()

    #----------------------------------------------------------------------
    def onStop(self):
        """停止策略（必须由用户继承实现）"""
        self.writeCtaLog(u'%s策略停止' %self.name)
        self.putEvent()
```

      上面这两个回调函数一看就知道，策略启动和结束的时候会调用一遍，没有什么特别的。

## 5、tick驱动和bar驱动

      我们看一下onTick函数。这个函数很显然，就是当有tick数据来的时候就会进行一次回调函数。但是，我们的策略是bar级别的，不是tick级别的。如果是tick级别的，那么可以在这里进行策略逻辑的实现，由于不是，所以就需要去调用bargenerator，也就是updateTick。这里，当tick数据足以合成一个bar的时候，就会调用onBar函数。

```python
def onTick(self, tick):
        """收到行情TICK推送（必须由用户继承实现）"""
        self.bg.updateTick(tick)

def onBar(self, bar):
        """收到Bar推送（必须由用户继承实现）"""
        am = self.am        # 不明白这里为什么要这样
        am.updateBar(bar)
        if not am.inited:
            return
        
        # 计算快慢均线
        fastMa = am.sma(self.fastWindow, array=True)
        self.fastMa0 = fastMa[-1]
        self.fastMa1 = fastMa[-2]
        
        slowMa = am.sma(self.slowWindow, array=True)
        self.slowMa0 = slowMa[-1]
        self.slowMa1 = slowMa[-2]

        # 判断买卖
        crossOver = self.fastMa0>self.slowMa0 and self.fastMa1<self.slowMa1     # 金叉上穿
        crossBelow = self.fastMa0<self.slowMa0 and self.fastMa1>self.slowMa1    # 死叉下穿
        
        # 金叉和死叉的条件是互斥
        # 所有的委托均以K线收盘价委托（这里有一个实盘中无法成交的风险，考虑添加对模拟市价单类型的支持）
        if crossOver:
            # 如果金叉时手头没有持仓，则直接做多
            if self.pos == 0:
                self.buy(bar.close, 1)
            # 如果有空头持仓，则先平空，再做多
            elif self.pos < 0:
                self.cover(bar.close, 1)
                self.buy(bar.close, 1)
        # 死叉和金叉相反
        elif crossBelow:
            if self.pos == 0:
                self.short(bar.close, 1)
            elif self.pos > 0:
                self.sell(bar.close, 1)
                self.short(bar.close, 1)
                
        # 发出状态更新事件
        self.putEvent()
```

      这里的例子都是vnpy里面的源码，不是笔者写的哦。onBar函数就很好理解了，就是整个程序的核心逻辑，和backtrader的onbar是一个概念，来一根bar线触发一次，同时也在里面发送开仓、平仓信号。

## 6、onOrder和onTrader

      这两个函数，看到是on打头的就知道是某种事情发生后的回调函数了。Order就是交易所收到了你的单子了，就会调用一次； onTrader则是一笔撮合完成了就会调用Trader。所以，理想情况下，两者应该是一样的，但是如果有撤单，那么就会有onOrder而不会调用onTrader。此外，有些套利指令，只会有一个onOrder,而可以有两个onTrader。一般来说，我们都会把逻辑放在onOrder，有特殊情况才会去使用onTrader。

## 7、移动止损单

      我们的策略往往会有跟踪移动止损，在vnpy中，移动止损单讲道理是有一点点复制的，他的实现机制是不断更新本地止损单。我们来看一下vnpy的例子中的代码：

```python
if self.pos == 0:
            self.intraTradeHigh = bar.high
            self.intraTradeLow = bar.low

            # ATR数值上穿其移动平均线，说明行情短期内波动加大
            # 即处于趋势的概率较大，适合CTA开仓，也就是说，认为大波段来了！
            if self.atrValue > self.atrMa:
                # 使用RSI指标的趋势行情时，会在超买超卖区钝化特征，作为开仓信号
                if self.rsiValue > self.rsiBuy:
                    # 这里为了保证成交，选择超价5个整指数点下单
                    self.buy(bar.close+5, self.fixedSize)

                elif self.rsiValue < self.rsiSell:
                    self.short(bar.close-5, self.fixedSize)

        # 持有多头仓位
        elif self.pos > 0:
            # 计算多头持有期内的最高价，以及重置最低价
            self.intraTradeHigh = max(self.intraTradeHigh, bar.high)
            self.intraTradeLow = bar.low
            
            # 计算多头移动止损
            longStop = self.intraTradeHigh * (1-self.trailingPercent/100)

            # 发出本地止损委托
            self.sell(longStop, abs(self.pos), stop=True)

        # 持有空头仓位
        elif self.pos < 0:
            self.intraTradeLow = min(self.intraTradeLow, bar.low)
            self.intraTradeHigh = bar.high

            shortStop = self.intraTradeLow * (1+self.trailingPercent/100)
            self.cover(shortStop, abs(self.pos), stop=True)
```

      这是AtrRsi策略中作者给出的源码，我们来看一下这个结构：

```python
if self.pos == 0:
#记录这一次bar的最高最低价，万一成交了，这个就是移动止损点的开始的位置。
            self.intraTradeHigh = bar.high
            self.intraTradeLow = bar.low

           #判断指标做相应的买卖操作。
elif self.pos > 0:
如果当前持仓是多头头寸，那么就更新移动止损的本地止损单位置。
# 计算多头持有期内的最高价，以及重置最低价
            self.intraTradeHigh = max(self.intraTradeHigh, bar.high)
            self.intraTradeLow = bar.low
            
            # 计算多头移动止损
            longStop = self.intraTradeHigh * (1-self.trailingPercent/100)

            # 发出本地止损委托
            self.sell(longStop, abs(self.pos), stop=True)
```

      这里看起来还是很简单的。但是个人觉得，这个单是否加入跟踪止损，其实比较好的方法就是在一开始下单的时候就完成跟踪止损单的设置。

      这里面，我们发现，发出的是一个stop为True的单子。这种单子可以叫做停止单，也可以叫做止损单。做一些处理，也就是不断获得最高的价格就可以实现移动止损单。

      这里我们要注意，stop单，也就是止损单是本地维护的，换句话说，发送的stop单不会发给交易所，而是在本地保存下来，当有行情数据来的时候，就先判断一下行情是否触发停止单。做了一个简单的测试，确实是如此，在高点回落特定比例之后，止损单会自动触发。而这个跟踪止损比例在每一个策略中可以自己设定，也就是self.trailingPercent

![](https://img-blog.csdnimg.cn/20190405185758924.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9sdXlpeGlhby5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

## 8、其他长度是k线

      我们的实盘过程中，都是tick数据拟合成bar，如果策略的级别是一分钟bar级别，那么直接在onBar里面实现策略就好，但是如果我们的策略是多个timeframe或者说10分钟、15分钟这样的策略呢？

      Vnpy中可以通过多个bargenerator来实现。我们记住一点，你要在多少个时间维度上进行，就需要多少个bargenerator。而tick数据默认会产生分钟级别的数据。

      假设现在我们需要一个小时的bar，也就是60分钟，那么就可以这么写：

```python
self.bg = BarGenerator(self.onBar, 60, self.on60m)
```

      注意到我们这个bg，也就是bargenerator多了两个参数，一个是60，其实就是我们的bar需要多少根分钟bar，一个是这个60分钟的bar，也就是小时bar生成之后，回调的是哪个函数？也就是我们的策略写在那个函数里面。这里是on60m，我们简单的print bar的时间。

```python
def on60m(self, bar):

        print bar.datetime
```

这样是不是结束了呢？不是，我们还需要在onBar里面调用bg的updateBar：

```python
def onBar(self, bar):

        """收到Bar推送（必须由用户继承实现）"""

        self.bg.updateBar(bar)
```

      简单梳理一下，bg，也就是bargenerator是一个k线序列对颖的生成器，那么，初始化这个生成器的时候，就需要有onbar这个由tick来调用的函数和所对应bar的分钟级别，以及这个级别的bar线对应的回调函数。

      此外，在vnpy的一个例子中，我们看到，如果我们有两个序列的k线，也就是有两个bg，那么在tick这个地方只要调用其中一个的bg的updateTick就可以了。

```python
def onTick(self, tick):

        """收到行情TICK推送（必须由用户继承实现）"""

        # 只需要要在一个BarGenerator中合成1分钟K线

        self.bg5.updateTick(tick)

因为只有一个onBar，而在onBar中，可以更新两个bg的k线：

    def onBar(self, bar):

        """收到Bar推送（必须由用户继承实现）"""

        # 基于15分钟判断趋势过滤，因此先更新

        self.bg15.updateBar(bar)
        # 基于5分钟判断

        self.bg5.updateBar(bar)
```

   还有一点要注意的就是putevent和同步数据这些是放在交易的地方，而不是一定在onBar的地方。








