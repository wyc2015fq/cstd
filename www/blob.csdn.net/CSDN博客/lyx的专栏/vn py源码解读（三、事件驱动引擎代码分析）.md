# vn.py源码解读（三、事件驱动引擎代码分析） - lyx的专栏 - CSDN博客





2018年12月02日 12:03:12[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：548
所属专栏：[vnpy源码解读](https://blog.csdn.net/column/details/30705.html)









        先抛开一切，我们来想一想，如果自己要写一个事件驱动引擎会怎么写？之前也说过，所谓的事情驱动就是你要监听一些事件，当某些事件发生的时候，要分配相对应的方法进行处理。完成这个过程的东西我们抽象出来之后就叫做事件驱动引擎了。那么，如果我们自己写的话，应该有这样几个功能：
- 1.事件的注册和取消，使用者可以根据自己的需求来设置引擎需要关心那些事件
- 2.事件对于的处理方法的挂钩。显然，一个事件可以由多个方法来处理，也可以一个方法处理多个事件。
- 3.不断监听事件的发生与否，如果发生就进行相应的处理，也就是调用设置好的函数。

        有了上面的共识，我们就可以开始分析源代码了。在nvpy/event/eventEngine.py里面，我们可以看到源代码。

        里面有三个类，分别是EventEngine,EventEngine2 和Event。其中，EventEngine和EventEngine2是一回事，只不过在定时器的使用过程中一个用了pyqt包里面的定时器Qtimer，一个用了python的线程自己实现的。那么这里我们还是讨论EventEngine，毕竟别的地方没什么区别。

```python
class EventEngine(object):
    """
    事件驱动引擎
    事件驱动引擎中所有的变量都设置为了私有，这是为了防止不小心
    从外部修改了这些变量的值或状态，导致bug。
    
    变量说明
    __queue：私有变量，事件队列
    __active：私有变量，事件引擎开关
    __thread：私有变量，事件处理线程
    __timer：私有变量，计时器
    __handlers：私有变量，事件处理函数字典
    
    
    方法说明
    __run: 私有方法，事件处理线程连续运行用
    __process: 私有方法，处理事件，调用注册在引擎中的监听函数
    __onTimer：私有方法，计时器固定事件间隔触发后，向事件队列中存入计时器事件
    start: 公共方法，启动引擎
    stop：公共方法，停止引擎
    register：公共方法，向引擎中注册监听函数
    unregister：公共方法，向引擎中注销监听函数
    put：公共方法，向事件队列中存入新的事件
    
    事件监听函数必须定义为输入参数仅为一个event对象，即：
    
    函数
    def func(event)
        ...
    
    对象方法
    def method(self, event)
        ...
        
    """

    #----------------------------------------------------------------------
    def __init__(self):
        """初始化事件引擎"""
        # 事件队列
        self.__queue = Queue()
        
        # 事件引擎开关
        self.__active = False
        
        # 事件处理线程
        self.__thread = Thread(target=self.__run)
        
        # 计时器，用于触发计时器事件
        self.__timer = QTimer()
        self.__timer.timeout.connect(self.__onTimer)
        
        # 这里的__handlers是一个字典，用来保存对应的事件调用关系
        # 其中每个键对应的值是一个列表，列表中保存了对该事件进行监听的函数功能
        self.__handlers = defaultdict(list)
        
        # __generalHandlers是一个列表，用来保存通用回调函数（所有事件均调用）
        self.__generalHandlers = []
        
    #----------------------------------------------------------------------
    def __run(self):
        """引擎运行"""
        while self.__active == True:
            try:
                event = self.__queue.get(block=True, timeout=1)  # 获取事件的阻塞时间设为1秒
                self.__process(event)
            except Empty:
                pass
            
    #----------------------------------------------------------------------
    def __process(self, event):
        """处理事件"""
        # 检查是否存在对该事件进行监听的处理函数
        if event.type_ in self.__handlers:
            # 若存在，则按顺序将事件传递给处理函数执行
            [handler(event) for handler in self.__handlers[event.type_]]
            
            # 以上语句为Python列表解析方式的写法，对应的常规循环写法为：
            #for handler in self.__handlers[event.type_]:
                #handler(event) 
        
        # 调用通用处理函数进行处理
        if self.__generalHandlers:
            [handler(event) for handler in self.__generalHandlers]
               
    #----------------------------------------------------------------------
    def __onTimer(self):
        """向事件队列中存入计时器事件"""
        # 创建计时器事件
        event = Event(type_=EVENT_TIMER)
        
        # 向队列中存入计时器事件
        self.put(event)    

    #----------------------------------------------------------------------
    def start(self, timer=True):
        """
        引擎启动
        timer：是否要启动计时器
        """
        # 将引擎设为启动
        self.__active = True
        
        # 启动事件处理线程
        self.__thread.start()
        
        # 启动计时器，计时器事件间隔默认设定为1秒
        if timer:
            self.__timer.start(1000)   # 这是设置时间定时器时间间隔的方法
    
    #----------------------------------------------------------------------
    def stop(self):
        """停止引擎"""
        # 将引擎设为停止
        self.__active = False
        
        # 停止计时器
        self.__timer.stop()
        
        # 等待事件处理线程退出
        self.__thread.join()
            
    #----------------------------------------------------------------------
    def register(self, type_, handler):
        """注册事件处理函数监听"""
        # 尝试获取该事件类型对应的处理函数列表，若无defaultDict会自动创建新的list
        handlerList = self.__handlers[type_]
        
        # 若要注册的处理器不在该事件的处理器列表中，则注册该事件
        if handler not in handlerList:
            handlerList.append(handler)
            
    #----------------------------------------------------------------------
    def unregister(self, type_, handler):
        """注销事件处理函数监听"""
        # 尝试获取该事件类型对应的处理函数列表，若无则忽略该次注销请求   
        handlerList = self.__handlers[type_]
            
        # 如果该函数存在于列表中，则移除
        if handler in handlerList:
            handlerList.remove(handler)

        # 如果函数列表为空，则从引擎中移除该事件类型
        if not handlerList:
            del self.__handlers[type_]
            
    #----------------------------------------------------------------------
    def put(self, event):
        """向事件队列中存入事件"""
        self.__queue.put(event)
        
    #----------------------------------------------------------------------
    def registerGeneralHandler(self, handler):
        """注册通用事件处理函数监听"""
        if handler not in self.__generalHandlers:
            self.__generalHandlers.append(handler)
            
    #----------------------------------------------------------------------
    def unregisterGeneralHandler(self, handler):
        """注销通用事件处理函数监听"""
        if handler in self.__generalHandlers:
            self.__generalHandlers.remove(handler)
```

        这是作者这个类的所有代码，讲道理，vnpy作者注释还是写的很不错的，所以笔者看起来比较快，看一下注释，扫一下代码基本就知道怎么回事了。

### 1.类的初始化方法

        我们先来讲这个类的初始化方法。

```python
def __init__(self):
        """初始化事件引擎"""
        # 事件队列
        self.__queue = Queue()
        
        # 事件引擎开关
        self.__active = False
        
        # 事件处理线程
        self.__thread = Thread(target=self.__run)
        
        # 计时器，用于触发计时器事件
        self.__timer = QTimer()
        self.__timer.timeout.connect(self.__onTimer)
        
        # 这里的__handlers是一个字典，用来保存对应的事件调用关系
        # 其中每个键对应的值是一个列表，列表中保存了对该事件进行监听的函数功能
        self.__handlers = defaultdict(list)
        
        # __generalHandlers是一个列表，用来保存通用回调函数（所有事件均调用）
        self.__generalHandlers = []
```

        首先是构造了一个事件队列。根据我们之前讲的，事件引擎就是监听事件，并调用相对于的方法来对事件进行响应。那么很显然我们就需要一个放置事件的地方，当我们的引擎监听到一个，调用处理的函数的时候，可以去处理下一个。这里，self.__queue = Queue()就是初始化了一个FIFO，FIFO就是先进先出（First In First Out），也就是排队，先被监听到的事件先调用处理的函数，很公平。当然，从作者的这个FIFO中可以看出，其实事件是没有优先级的，相对来说简化处理了，其实如果比较复杂的话，应当考虑事件队列中事件的优先级问题。

        引擎的开关就没什么好说的，就是控制引擎是否工作。

      # 事件处理线程

        self.__thread = Thread(target=self.__run)

        这个是开启了一个线程用于处理事件，我们可以先看一下这个类的run函数是什么        

### 2.run函数

```python
def __run(self):
        """引擎运行"""
        while self.__active == True:
            try:
                event = self.__queue.get(block=True, timeout=1)  # 获取事件的阻塞时间设为1秒
                self.__process(event)
            except Empty:
                pass
```

        这个run其实就是一个大循环。如果引擎是被开启的，那么就会不断的去读取事件队列，也就是queue，之前初始化的FIFO中的数据。Queue类型获取数据的方法是get，这里参数block是True，说明是一个阻塞式获取，所谓的阻塞在这里可以认为是等待。也就是如果get的时候，queue是空的，那么我就等待timeout秒，在这里是1秒。一秒之后还是没有数据，那么就报empty、异常，这个异常被下面的except捕获。当然作者在这里没有做任何处理，其实从后面的代码来看，理论上是不应该出现异常的，稍后会提到。

       当然，如果我们设置blocking = False，当queue是空的时候，直接报 empty异常。

        那么，我们从事件队列中get到了一个event之后会怎么样呢？我们看到是交给__process了。那么就进一步看一下，process是什么。

### 3.process函数

```python
def __process(self, event):
        """处理事件"""
        # 检查是否存在对该事件进行监听的处理函数
        if event.type_ in self.__handlers:
            # 若存在，则按顺序将事件传递给处理函数执行
            [handler(event) for handler in self.__handlers[event.type_]]
            
            # 以上语句为Python列表解析方式的写法，对应的常规循环写法为：
            #for handler in self.__handlers[event.type_]:
                #handler(event) 
        
        # 调用通用处理函数进行处理
        if self.__generalHandlers:
            [handler(event) for handler in self.__generalHandlers]
```

        当我们的引擎从事件队列获得一个event之后，调用上面这个函数。这个函数首先看一下事件类型在不在handlers里面，那么看一下handlers是个什么东西。

        在初始化函数中，handlers被初始化为

```python
# 这里的__handlers是一个字典，用来保存对应的事件调用关系
        # 其中每个键对应的值是一个列表，列表中保存了对该事件进行监听的函数功能
        self.__handlers = defaultdict(list)
        
        # __generalHandlers是一个列表，用来保存通用回调函数（所有事件均调用）
        self.__generalHandlers = []
```

        这里作者的注释其实相当清楚了，就是一个字典，key是事件类型，value是一个list，里存的是处理该事件的方法。还有一个generalHandlers，这个没有key，就是一个list，因为他是一个通用事件处理列表，说白了就是不管是啥事件，这里面的方法都要处理一下。

        好，那么到这里我们就明白了，我们事件处理现场thread的线程函数是run，这个函数的功能就是while循环从事件队列中获取事件，然后把事件交给处理函数precess去进行事件和事件对于的处理函数的匹配，并调用处理函数来处理。

### 4.定时器

        初始化函数还差一个，就是定时器

```python
# 计时器，用于触发计时器事件
 self.__timer = QTimer()
self.__timer.timeout.connect(self.__onTimer)
```

        定时器很简单，初始化，然后绑定定时器到时间的时候调用的函数。timeout.connect就是用来绑定函数的，函数叫做onTimer。那么我们就来看看这个函数吧。

```python
def __onTimer(self):
        """向事件队列中存入计时器事件"""
        # 创建计时器事件
        event = Event(type=EVENT_TIMER)
        
        # 向队列中存入计时器事件
        self.put(event)
```

        这里，我们看到每当定时器发作的时候，就会创建一个叫做EVENT_TIMER的事件。那么这个事件的类是Event。我们就来看一下这个类吧。

```python
class Event:
    """事件对象"""

    #----------------------------------------------------------------------
    def __init__(self, type_=None):
        """Constructor"""
        self.type_ = type_      # 事件类型
        self.dict_ = {}         # 字典用于保存具体的事件数据
```

        这个类不要太简单，其实就是记录了事件的类型。一般的事件驱动引擎讲道理至少还应该有个优先级什么的。

### 5.开始和结束

        那么，定时器讲完了，就来讲一下别的吧。我们引擎的开始和停止方法。其实特别简单，开始方法就是先激活，然后让我们之前分析的处理事件的线程打开，然后是设置定时器的定时间隔为1秒。也就是1秒钟产生一个定时器事件。这也就是为什么我之前说在queue中get事件的时候理论上不应该有异常产生的原因。

```python
def start(self, timer=True):
        """
        引擎启动
        timer：是否要启动计时器
        """
        # 将引擎设为启动
        self.__active = True
        
        # 启动事件处理线程
        self.__thread.start()
        
        # 启动计时器，计时器事件间隔默认设定为1秒
        if timer:
            self.__timer.start(1000)   # 这是设置时间定时器时间间隔的方法
    
    #----------------------------------------------------------------------
    def stop(self):
        """停止引擎"""
        # 将引擎设为停止
        self.__active = False
        
        # 停止计时器
        self.__timer.stop()
        
        # 等待事件处理线程退出
        self.__thread.join()
```

        结束就更加简单了，定时器停止，线程退出就可以了，没什么好说的。

### 6.注册和取消注册

        到这里，我们回想一下一开始说的一个事件应该有的基本功能，我们发现，少了事件的注册。

```python
def register(self, type_, handler):
        """注册事件处理函数监听"""
        # 尝试获取该事件类型对应的处理函数列表，若无defaultDict会自动创建新的list
        handlerList = self.__handlers[type_]
        
        # 若要注册的处理器不在该事件的处理器列表中，则注册该事件
        if handler not in handlerList:
            handlerList.append(handler)
            
    #----------------------------------------------------------------------
    def unregister(self, type_, handler):
        """注销事件处理函数监听"""
        # 尝试获取该事件类型对应的处理函数列表，若无则忽略该次注销请求   
        handlerList = self.__handlers[type_]
            
        # 如果该函数存在于列表中，则移除
        if handler in handlerList:
            handlerList.remove(handler)

        # 如果函数列表为空，则从引擎中移除该事件类型
        if not handlerList:
            del self.__handlers[type_]
```

       这部分代码相对来说比较简单，就是在字典中增加和移除罢了。通用的事件处理也一样

```python
#----------------------------------------------------------------------
    def registerGeneralHandler(self, handler):
        """注册通用事件处理函数监听"""
        if handler not in self.__generalHandlers:
            self.__generalHandlers.append(handler)
            
    #----------------------------------------------------------------------
    def unregisterGeneralHandler(self, handler):
        """注销通用事件处理函数监听"""
        if handler in self.__generalHandlers:
            self.__generalHandlers.remove(handler)
```

        我们结合一下上一次讲的CtaTrading中的代码，来看一下具体是怎么用的。

```python
ee.register(EVENT_LOG, le.processLogEvent)
    ee.register(EVENT_CTA_LOG, le.processLogEvent)
    ee.register(EVENT_ERROR, processErrorEvent)
    le.info(u'注册日志事件监听')
```

        前面是事件类型，后面的对应的处理函数，那么当事件发生的时候，我们的事件引擎就可以去调用对应的方法来进行处理了。

### 7.put

        还有最后一个方法，就是put。也就是告诉事件引擎，什么事情发生了，当发生之后，事件引擎就会把这个事件放到事件队列中。

```python
def put(self, event):
        """向事件队列中存入事件"""
        self.__queue.put(event)
```





