# What’s new in Channels 2 摘译 - =朝晖= - 博客园
# [What’s new in Channels 2 摘译](https://www.cnblogs.com/dhcn/p/9005040.html)
     最近准备在一个老Django项目上启用Channels，Channels于今年2月2日发布2.0版本，这个版本包含很多不向前兼容的特性，为了新特性调研的需要，也为了方便社区，我把新版本的What's New摘要汉化一下。
     一、**依赖要求**：因为对Py3标准库asyncio的依赖，Channels必须3.5以上的版本，有些依赖甚至需要3.5.2以上的版本
     二、**设计概念体系**发生巨大变化：
          2.1、**通道层和进程**：v1网络前端和应用逻辑通过通道层**跨进程**消息事件通信，v2 Django Code通过线程池在一个**进程内**执行，把网络前端和应用逻辑结合在一起。
          2.2、**应用实例**：面向一个Socket的所有处理在一个进程类执行，Channel层只用来消息广播，比如聊天消息在不同应用实例之间的传递。
          2.3、**新的消费者：**V2采用一种[乌龟驮乌龟一直向下](https://en.wikipedia.org/wiki/Turtles_all_the_way_down)的设计，系统的每个方面都设计成一个包含消费者和路由系统的ASGI应用。
          2.5、**完全异步：**除了View和ORM，剩下的绝大多数都是异步代码
     三、**被移除的组件：**绑定框架被完全移除
     四、**如何升级你的代码**：
         4.1、旧的routing和消费者函数都没戏了，必须来新的基于类的消费者和ProtocolTypeRouter定义方式
         4.2、channel_session 和 enforce_ordering不在需要
         4.3、Auth机制由装饰器变成AuthMiddlewareStack配置，包括Session在内的Message读取方式变成self.scope["name"]
         4.4、通道层不再是必须，除channels_redis之外的都被异步调度直接取代，通道层的配置和使用方式也发生变化
         4.5、Groups对象不在存在，其特性机制被几个函数取代
         4.6、Delay Server可用异步消费者取代
         4.7、测试框架已经完全被重写成异步的。
     备注：
     新版Channels代码样例使用了Py3.5的新异步语法async和await，不熟悉的可以看：https://zhuanlan.zhihu.com/p/27258289 ，不过建议最好看看《流畅的Python》了解一下异步协程。

