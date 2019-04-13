
# Python 学习入门（29）—— 消息队列 - 阳光岛主 - CSDN博客

2013年12月21日 19:18:23[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：12871所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



[snakeMQ](http://www.snakemq.net/)是一个跨平台的[Python](http://www.oschina.net/p/python)消息队列库。消息队列让不同主机间通信变得简单可靠。使用snakeMQ，只需要发送消息，剩下的事都交给snakeMQ处理。
### 特色：
纯python实现，跨平台
自动重连接
可靠发送--可配置的消息方式与消息超时方式
持久化/临时 两种队列
支持异步 -- poll()
symmetrical -- 单个TCP连接可用于双工通讯
多数据库支持 -- SQLite、MongoDB……
brokerless - 类似[ZeroMQ](http://my.oschina.net/kjpioo/admin/oschina.net/p/ZeroMQ)的实现原理
扩展模块：RPC, bandwidth throttling
与AMQP, ZeroMQ等其他消息系统不兼容。

[ZeroMQ](https://github.com/zeromq/pyzmq)是一个很有个性的项目，它原来是定位为“史上最快消息队列”，所以名字里面有“MQ”两个字母，但是后来逐渐演变发展，慢慢淡化了消息队列的身影，改称为消息内核，或者消息层了。从网络通信的角度看，它处于会话层之上，应用层之下，有了它，你甚至不需要自己写一行的socket函数调用就能完成复杂的网络通信工作。
[zeromq异步消息队列](http://rfyiamcool.blog.51cto.com/1030776/1207004)
源码托管：[zeromq pyzmq](https://github.com/zeromq/pyzmq)
消息事件：[gevent-zeromq](https://github.com/tmc/gevent-zeromq)

[RabbitMQ](http://www.rabbitmq.com/)作为一个工业级的消息队列服务器，在其客户端手册列表的Python段当中推荐了一篇blog ，作为RabbitMQ+Python的入门手册再合适不过了。不过，正如其标题Rabbit and Warrens （兔子和养兔场）一样，，这篇英文写的相当俏皮，以至于对于我等非英文读者来说不像一般的技术文档那么好懂，所以，翻译一下吧。翻译过了，希望其他人可以少用一些时间。翻译水平有限，不可能像原文一样俏皮，部分地方可能就意译了，希望以容易懂为准。想看看老外的幽默的，推荐去看原文，其实，也不是那么难理解……
[RabbitMQ+Python入门经典](http://www.searchsoa.com.cn/showcontent_43337.htm)

**[pyActiveMQ](https://code.google.com/p/pyactivemq/)**是一个用来连接消息中间件[ActiveMQ](http://www.oschina.net/p/activemq)的Python模块。pyActiveMQ 封装了[ActiveMQ-CPP](http://activemq.apache.org/cms/)。
**[RestMQ](http://restmq.com/)**是一个以[Redis](http://www.oschina.net/p/redis)为底层存储，提供HTTP接口的队列系统。


目前业界有很多MQ产品，作如下对比：
## RabbitMQ
使用Erlang编写的一个开源的消息队列，本身支持很多的协议：AMQP，XMPP, SMTP, STOMP，也正是如此，使的它变的非常重量级，更适合于企业级的开发。同时实现了一个经纪人(Broker)构架，这意味着消息在发送给客户端时先在中心队列排队。对路由(Routing)，负载均衡(Load balance)或者数据持久化都有很好的支持。

## Redis
一个Key-Value的NoSQL数据库，开发维护很活跃，虽然它是一个Key-Value数据库存储系统，但它本身支持MQ功能，所以完全可以当做一个轻量级的队列服务来使用。对于RabbitMQ和Redis的入队和出队操作，各执行100万次，每10万次记录一次执行时间。测试数据分为128Bytes、512Bytes、1K和10K四个不同大小的数据。实验表明：
1）入队时，当数据比较小时Redis的性能要高于RabbitMQ，而如果数据大小超过了10K，Redis则慢的无法忍受；
2）出队时，无论数据大小，Redis都表现出非常好的性能，而RabbitMQ的出队性能则远低于Redis。
|入队
|出队
|
|128B
|512B
|1K
|10K
|128B
|512B
|1K
|10K
|
|Redis
|16088
|15961
|17094
|25
|15955
|20449
|18098
|9355
|
|RabbitMQ
|10627
|9916
|9370
|2366
|3219
|3174
|2982
|1588
|

## ZeroMQ
号称最快的消息队列系统，尤其针对大吞吐量的需求场景。ZMQ能够实现RabbitMQ不擅长的高级/复杂的队列，但是开发人员需要自己组合多种技术框架，技术上的复杂度是对这MQ能够应用成功的挑战。ZeroMQ具有一个独特的非中间件的模式，你不需要安装和运行一个消息服务器或中间件，因为你的应用程序将扮演了这个服务角色。你只需要简单的引用ZeroMQ程序库，可以使用NuGet安装，然后你就可以愉快的在应用程序之间发送消息了。但是ZeroMQ仅提供非持久性的队列，也就是说如果down机，数据将会丢失。其中，Twitter的Storm中使用ZeroMQ作为数据流的传输。

## ActiveMQ
是Apache下的一个子项目， 类似于ZeroMQ，它能够以代理人和点对点的技术实现队列。同时类似于RabbitMQ，它少量代码就可以高效地实现高级应用场景。RabbitMQ、ZeroMQ、ActiveMQ均支持常用的多种语言客户端 C++、Java、.Net,、Python、 Php、 Ruby等。

## Jafka/Kafka
Kafka是Apache下的一个子项目，是一个高性能跨语言分布式Publish/Subscribe消息队列系统，而Jafka是在Kafka之上孵化而来的，即Kafka的一个升级版。具有以下特性：快速持久化，可以在O(1)的系统开销下进行消息持久化；高吞吐，在一台普通的服务器上既可以达到10W/s的吞吐速率；完全的分布式系统，Broker、Producer、Consumer都原生自动支持分布式，自动实现复杂均衡；支持Hadoop数据并行加载，对于像Hadoop的一样的日志数据和离线分析系统，但又要求实时处理的限制，这是一个可行的解决方案。Kafka通过Hadoop的并行加载机制来统一了在线和离线的消息处理，这一点也是本课题所研究系统所看重的。Apache Kafka相对于ActiveMQ是一个非常轻量级的消息系统，除了性能非常好之外，还是一个工作良好的分布式系统。
其他一些队列列表HornetQ、Apache Qpid、Sparrow、Starling、Kestrel、Beanstalkd、Amazon SQS就不再一一分析。

**原生队列实现：**
```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import Queue
import threading
import urllib, urllib2
import time
# 队列大小
myqueue = Queue.Queue(maxsize=0)    
queue = Queue.Queue()
hosts = ["http://1","http://2","http://3"]

lock = threading.Lock()
def printMsg(msg):
    global lock
    if lock.acquire():
        print(msg)
        lock.release()
            
class ThreadUrl(threading.Thread):
    def __init__(self, queue, htint):
        threading.Thread.__init__(self)
        self.queue = queue
        self.Ht = htint         # 线程ID
        
            
    def run(self):
        while True:
            host = self.queue.get()                     # get()方法从队头删除并返回一个项目
            printMsg("thread_id: " + self.getName() + ";\t htint: " + str(self.Ht) + " --- host: " + host)
            printMsg("qsize: %d" % self.queue.qsize())  # 返回队列的大小，近似值
            if self.queue.empty():                      # 如果队列为空
                printMsg("queue is empty of " + self.getName())
            self.queue.task_done()  # 退出
def main():
    # spawn a pool of threads, and pass them queue instance
    for i in range(5):
        t = ThreadUrl(queue, i)
        t.setDaemon(True)
        t.start()
        # populate queue with data
        for host in hosts:          # 往线程中填充数据
            printMsg("queue put()")
            queue.put(host)         # 插入队列
        queue.join()                # 等待队列直到全部被处理完
if __name__ == "__main__":
    start = time.time()
    main()
    time.sleep(1)
    costTime = time.time() - start - 1
    print "Elapsed Time: %s (s)" % costTime     # 计时
```
运行结果：
```python
queue put()
queue put()
queue put()
thread_id: Thread-1;	 htint: 0 --- host: http://1
qsize: 2
thread_id: Thread-1;	 htint: 0 --- host: http://2
qsize: 1
thread_id: Thread-1;	 htint: 0 --- host: http://3
qsize: 0
queue is empty of Thread-1
queue put()
queue put()
queue put()
thread_id: Thread-2;	 htint: 1 --- host: http://1
qsize: 2
thread_id: Thread-2;	 htint: 1 --- host: http://2
qsize: 1
thread_id: Thread-2;	 htint: 1 --- host: http://3
qsize: 0
queue is empty of Thread-2
queue put()
queue put()
queue put()
thread_id: Thread-3;	 htint: 2 --- host: http://1
thread_id: Thread-2;	 htint: 1 --- host: http://2
qsize: 1
thread_id: Thread-2;	 htint: 1 --- host: http://3
qsize: 0
queue is empty of Thread-2
qsize: 0
queue is empty of Thread-3
queue put()
queue put()
queue put()
thread_id: Thread-1;	 htint: 0 --- host: http://1
qsize: 2
thread_id: Thread-1;	 htint: 0 --- host: http://2
qsize: 1
thread_id: Thread-1;	 htint: 0 --- host: http://3
qsize: 0
queue is empty of Thread-1
queue put()
queue put()
queue put()
thread_id: Thread-2;	 htint: 1 --- host: http://1
qsize: 2
thread_id: Thread-2;	 htint: 1 --- host: http://2
qsize: 1
thread_id: Thread-1;	 htint: 0 --- host: http://3
queue is empty of Thread-2
qsize: 0
queue is empty of Thread-1
Elapsed Time: 0.00404787063599 (s)
```


