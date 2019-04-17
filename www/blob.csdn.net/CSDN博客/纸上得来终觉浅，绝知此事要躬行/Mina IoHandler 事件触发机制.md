# Mina IoHandler 事件触发机制 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月04日 12:37:17[boonya](https://me.csdn.net/boonya)阅读数：6416
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina IoHandler（Apache Mina user guide Chapter8 Handler）](http://mina.apache.org/mina-project/userguide/ch7-handler/ch7-handler.html)



所有处理I / O事件都由Mina触发。接口所有活动中心的完成，都在过滤器链结束之后。
- IoHandler有如下方法：
- · sessionCreated
- · sessionOpened
- · sessionClosed
- · sessionIdle
- · exceptionCaught
- · messageReceived
- · messageSent



### 1.1.1. **sessionCreated事件**



    当创建一个新连接时触发Session会话创建的事件。对TCP而言，它是的连接接受的结果，当接收到一个UDP数据包时对于UDP它是生成。这个函数可以用来初始化Session会话属性,并为一个特定的连接执行一次活动。

这个函数从I / O处理器线程上下文调用，因此应该实现一种消耗时间最少的方式，作为相同的线程会处理多个Session会话。

### 1.1.2. **sessionOpened事件**

当打开连接时，Session会话打开事件被调用。它总是在sessionCreated事件创建之后被调用。如果是一个已配置上网线程模型，这个函数被用于调用线程以外的I
 / O处理器线程。

### 1.1.3. **sessionClosed事件**

当Session关闭时，session会话的close事件被关闭。Session会话会清理活动，比如现场清理可以执行。

### 1.1.4. **sessionIdle事件**

当一个会话变得空闲时，触发会话空闲事件。这个函数不执行UDP传输。

### 1.1.5. **exceptionCaught事件**

当由用户代码或Mina抛出异常时，这个函数被调用。如果异常时一个IOException，则连接被关闭。

### 1.1.6. **messageReceived事件**

当消息被接收到时，消息接收事件触发。这就是大多数应用程序需要处理的地方。你需要关注所有你预期的消息类型。

### 1.1.7. **messageSent事件**

无论何时，当消息即响应发送时，消息发送事件被触发(调用IoSession.write())。




