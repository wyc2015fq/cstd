
# 详解Python设计模式编程中观察者模式与策略模式的运用​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年07月27日 22:01:41[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：55标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


**[详解Python设计模式编程中观察者模式与策略模式的运用](http://cda.pinggu.org/view/26207.html)**
这篇文章主要介绍了Python设计模式编程中观察者模式与策略模式的运用,观察者模式和策略模式都可以归类为结构型的设计模式,需要的朋友可以参考下
观察者模式
观察者模式：又叫发布订阅模式，定义了一种一对多的依赖关系，让多个观察者对象同时监听某一个主题对象，这个主题对象的状态发生变化时，会通知所有观察者对象，是他们能自动更新自己。
代码结构
class Topic(object):
"""主题类。保存所有观察者实例的引用，每个主题都可以有很多观察者
可以增加和删除观察者"""
def __init__(self):
self.obs = []
def Attach(self, ob):
self.obs.append(ob)
def Detach(self, ob):
self.obs.remove(ob)
def Notify(self):
for ob in self.obs:
ob.Update()
class Observer(object):
"""抽象观察者类，收到主题的变更通知时，更新自己"""
def Update(self):
raise NotImplementedError()
class ConcreteTopic(object):
"""一个具体主题"""
def __init__(self):
self.state = None
def ChangeState(self, newState):
self.state = newState
self.Notify()
class ConcreteObserver(object):
"""一个具体监听类"""
def __init__(self, topic):
self.topic = topic
def Update(self):
print self.topic.state
def client():
topic = ConcreteTopic()
topic.Attach(ConcreteObserver(topic))
topic.ChangeState('New State')
众多MQ中间件都是采用这种模式的思想来实现的。
观察者模式可以让主题和观察者之间解耦，互相之间尽可能少的依赖。不过抽象主题和抽象观察者之间还是有耦合的。
策略模式
策略模式： 定义了算法家族，分别封装起来，让他们之间可以互相替换。此模式让算法的变化不影响使用算法的客户。
代码框架
class Strategy(object):
"""抽象算法类"""
def AlgorithmInterface(self):
raise NotImplementedError()
class ConcreteStrategyA(Strategy):
def AlgorithmInterface(self):
print '算法Ａ'
class ConcreteStrategyB(Strategy):
def AlgorithmInterface(self):
print '算法B'
class Context(object):
"""上下文，作用就是封装策略的实现细节，用户只需要知道有哪些策略可用"""
def __init__(self, strategy):
\# 初始化时传入具体的策略实例
self.strategy = strategy
def ContextInterface(self):
\# 负责调用具体的策略实例的接口
self.strategy.AlgorithmInterface()
def client(cond):
\# 策略模式的使用演示
\# 用户只需要根据不同的条件，将具体的算法实现类传递给Context，
\# 然后调用Context暴露给用户的接口就行了。
if cond == 'A':
context = Context(ConcreteStrategyA())
elif cond == 'B':
context = Context(ConcreteStrategyB())
result = context.ContextInterface()
策略模式解决那类问题
在回答这个问题之前，先说下对策略模式的使用方式的感觉。上面的client函数，怎么看起来就像是简单工厂模式中的工厂函数呢？确实如此，实际上策略模式可以和简工厂模式结合起来，将更多细节封装在策略模式内部，让使用者更容易的使用。
那么策略模式和简单工厂模式有什么不同呢？策略模式中的算法是用来解决同一个问题的，根据时间、条件不同，算法的具体细节有差异，但最终解决的是同一个问题。在需求分析过程中，当听到需要在不同时间应用不同的业务规则，就可以考虑使用策略模式来处理这种变化的可能性。
缺点
使用者需要知道每一种策略的具体含义，并负责选择策略
改进
结合简单工厂模式，将策略选择封装在Context内部，解放client：
class Context(object):
def __init__(self, cond):
if cond == 'A':
self.strategy = Context(ConcreteStrategyA())
elif cond == 'B':
self.strategy = Context(ConcreteStrategyB())
def ContextInterface(self):
self.strategy.AlgorithmInterface()

def client(cond):
context = Context(cond)
result = context.ContextInterface()
改进后的遗留问题
每次需要增加新的策略时，就需要修改Context的构造函数，增加一个新的判断分支。

