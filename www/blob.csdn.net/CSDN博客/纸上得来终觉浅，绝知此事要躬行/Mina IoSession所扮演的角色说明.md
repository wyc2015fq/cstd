# Mina IoSession所扮演的角色说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月02日 18:12:52[boonya](https://me.csdn.net/boonya)阅读数：1700
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina session(Apache user guide Chapter4 session)](http://mina.apache.org/mina-project/userguide/ch4-session/ch4-session.html)


Session会话是Mina的核心:每次客户端连接到服务器,创建一个新的session会话,并将保存在内存中,直到客户端断开连接。Session会话连接,用于存储持久化信息，并且服务器可能需要处理任何类型的信息在请求期间,并最终在整个会话期间存活。

### 1.1.1. **Session状态**

在系统运行期间，一个session会话有一个状态：
- · **Connected : session会话创建并可用**
- **· Idle : 在一段时间内session会话没有处理任何请求 (时间是可配置的)**
- o Idle for read : no read has actually been made for a period of time
- o Idle for write : no write has actually been made for a period of time
- o Idle for both : no read nor write for a period of time
- **· Closing : 会话正在被关闭，未完全关闭(the remaining messages are being flushed, cleaning up is not terminated)**
- **· Closed : 会话已完全关闭无法恢复.**

以下是传输中可能的session状态图表：

![](https://img-blog.csdn.net/20160602181120086)

### 1.1.2. **Session配置**

许多不同的参数可以设置为一个特定的Session会话:
- · receive buffer size
- · sending buffer size
- · Idle time
- · Write timeOut

加强其他配置,根据使用的传输类型(见第6章-Transports)。

### 1.1.3. **管理用户定义的属性**

Session可能需要存储一些可以使用的数据。这是通过使用专用的数据结构与每个会话相关联。这是一个键值关联,可以存储任何开发人员可能希望保持保留类型的数据。

例如,如果你想跟踪请求用户发送的数量自创建会话,很容易将它存储到这个Map对象:只需要创建一个将与此相关的关键值。

```java
<span style="font-size:12px;">...

int counterValue = session.getAttribute( "counter" );

session.setAttribute( "counter", counterValue + 1 );

...</span>
```

我们有一种方法来处理存储到会话属性:属性是一个键-值对,可以从session会话和可读容器中添加,删除。

这个容器创建时自动创建会话,会话终止时将会被处理掉。

### 1.1.4. **定义容器**

当我们说,这个容器是一个键-值容器,默认的Map对象,但它也可以定义另一个数据结构,如果一个想处理长寿命数据,或为了避免在内存中存储所有的数据,如果他们都很大:我们可以实现一个接口和一个工厂,在创建session会话时将被用于创建这个容器。

这个代码片段显示了如何创建容器在会话初始化:

```java
<span style="font-size:12px;">protected final void initSession(IoSession session,

        IoFuture future, IoSessionInitializer sessionInitializer) {
    ...

    try {
        ((AbstractIoSession) session).setAttributeMap(session.getService()

                .getSessionDataStructureFactory().getAttributeMap(session));

    } catch (IoSessionInitializationException e) {
        throw e;
    } catch (Exception e) {

        throw new IoSessionInitializationException(

                "Failed to initialize an attributeMap.", e);
    }

    ...</span>
```



这里如果我们想定义另一种容器，我们可以实现工厂接口:

```java
<span style="font-size:12px;">public interface IoSessionDataStructureFactory {

    /**

     * Returns an {@link IoSessionAttributeMap} which is going to be associated

     * with the specified <tt>session</tt>.  Please note that the returned

     * implementation must be thread-safe.

     */

     IoSessionAttributeMap getAttributeMap(IoSession session) throws Exception;

 }</span>
```

### 1.1.5. **过滤器链**

       每个会话与一连串的过滤器关联,将处理传入请求或一个即将移出的消息接收或发射。这些过滤器是特定的每个会话都是独立的,即使大多数的情况下,我们将使用同一过滤器链的所有现有会话。

然而,它可以动态地修改单个会话的链,例如通过添加日志过滤器链中的一个特定的会话。

### 1.1.6. **统计**

每个会话也会保持跟踪会话，记录此session都做了什么:
- · number of bytes received/sent
- · number of messages received/sent
- · Idle status
- · throughput

还有许多其他有用的信息。

### 1.1.7. **处理器**

最后,同样重要的是,一个会话附加到处理程序,负责调度信息到您的应用程序。这个处理程序也将通过使用session会话发送包响应,简单地通过调用write()方法:

```java
<span style="font-size:12px;">...

session.write(“your message”);

...</span>
```







