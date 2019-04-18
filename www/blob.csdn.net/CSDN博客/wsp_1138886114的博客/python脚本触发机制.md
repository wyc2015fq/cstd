# python脚本触发机制 - wsp_1138886114的博客 - CSDN博客





2018年11月21日 17:02:49[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：152
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









转载：[http://www.baidu.com/link?url=LKTaPVyzaZ9gU0zW-zwHezyo6YoRIjjqOt1mGrXrXGLiSGMfQMRZWkUU5tBWTlj0ZHvsAsAuWbqVwIWmLU6EPK&wd=&eqid=abcdfa450000b71c000000045bf51762](http://www.baidu.com/link?url=LKTaPVyzaZ9gU0zW-zwHezyo6YoRIjjqOt1mGrXrXGLiSGMfQMRZWkUU5tBWTlj0ZHvsAsAuWbqVwIWmLU6EPK&wd=&eqid=abcdfa450000b71c000000045bf51762)

##### 事件管理 [EventManager.py](http://EventManager.py)

```python
from queue import Queue, Empty
from threading import *


class EventManager:
    def __init__(self):
        """初始化事件管理器"""
        self.__eventQueue = Queue()  # 事件对象列表
        self.__active = False        # 事件管理器开关
        self.__thread = Thread(target=self.__Run)  # 事件处理线程

        # 这里的__handlers是一个字典，用来保存对应的事件的响应函数
        # 其中每个键对应的值是一个列表，列表中保存了对该事件监听的响应函数，一对多
        self.__handlers = {}  # {事件类型:[处理事件的方法]}

    def __Run(self):
        """引擎运行"""
        while self.__active == True:
            try:
                event = self.__eventQueue.get(block=True, timeout=1)  # 获取事件的阻塞时间设为1秒
                self.__EventProcess(event)
            except Empty:
                pass

    def __EventProcess(self, event):
        """处理事件"""
        if event.type_ in self.__handlers:  # 检查是否存在对该事件进行监听的处理函数
            for handler in self.__handlers[event.type_]:  # 若存在，则按顺序将事件传递给处理函数执行
                handler(event)


def Start(self):
    """启动"""
    self.__active = True  # 将事件管理器设为启动
    self.__thread.start()  # 启动事件处理线程


def Stop(self):
    """停止"""
    self.__active = False  # 将事件管理器设为停止
    self.__thread.join()  # 等待事件处理线程退出


def AddEventListener(self, type_, handler):
    """绑定事件和监听器处理函数"""
    # 尝试获取该事件类型对应的处理函数列表，若无则创建
    try:
        handlerList = self.__handlers[type_]
    except KeyError:
        handlerList = []

    self.__handlers[type_] = handlerList
    # 若要注册的处理器不在该事件的处理器列表中，则注册该事件
    if handler not in handlerList:
        handlerList.append(handler)


def RemoveEventListener(self, type_, handler):
    """移除监听器的处理函数"""
    # 读者自己试着实现


def SendEvent(self, event):
    """发送事件，向事件队列中存入事件"""
    self.__eventQueue.put(event)


"""事件对象"""


class Event:
    def __init__(self, type_=None):
        self.type_ = type_  # 事件类型
        self.dict = {}  # 字典用于保存具体的事件数据
```

##### 测试代码 [test.py](http://test.py)

```python
from threading import *
from EventManager import *
import time

# 事件名称 新文章
EVENT_ARTICAL = "Event_Artical"


# 事件源 公众号
class PublicAccounts:
    def __init__(self, eventManager):
        self.__eventManager = eventManager

    def WriteNewArtical(self):
        event = Event(type_=EVENT_ARTICAL)  # 事件对象，写了新文章
        event.dict["artical"] = u'如何写出更优雅的代码\n'
        self.__eventManager.SendEvent(event)  # 发送事件
        print(u'公众号发送新文章')


# 监听器 订阅者
class Listener:
    def __init__(self, username):
        self.__username = username

    # 监听器的处理函数 读文章
    def ReadArtical(self, event):
        print(u'%s 收到新文章' % self.__username)
        print(u'正在阅读新文章内容：%s' % event.dict["artical"])


"""测试函数"""
def test():
    listner1 = Listener("thinkroom")  # 订阅者1
    listner2 = Listener("steve")  # 订阅者2
    eventManager = EventManager()

    # 绑定事件和监听器响应函数(新文章)
    eventManager.AddEventListener(EVENT_ARTICAL, listner1.ReadArtical)
    eventManager.AddEventListener(EVENT_ARTICAL, listner2.ReadArtical)
    eventManager.Start()

    publicAcc = PublicAccounts(eventManager)
    while True:
        publicAcc.WriteNewArtical()
        time.sleep(2)


if __name__ == '__main__':
    test()
```



