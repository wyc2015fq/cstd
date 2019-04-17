# Mina IoFilter注意事项说明 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月03日 18:19:14[boonya](https://me.csdn.net/boonya)阅读数：2989
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina过滤器(Mina user guide Chapter5 Filter)](http://mina.apache.org/mina-project/userguide/ch5-filters/ch5-filters.html)




 IoFilter是Mina核心结构提供的一个很重要的角色。它过滤了所有在IoService和IoHandler之间的I
 / O事件和请求。如果你有一个使用web应用程序编程的经验,你可以放心地认为这是一个类似的Servlet过滤器。Mina提供了许多开箱即用的过滤器加速网络应用程序开发的速度，通过简化使用开箱即用过滤器等典型的横切关注点:
- · LoggingFilter 日志揭露所有的事件和请求.
- · ProtocolCodecFilter 从ByteBuffe转化为消息对应的POJO
- · CompressionFilter 压缩所有数据.
- · SSLFilter adds SSL - TLS - StartTLS support.安全过滤器
- · 可以自己实现更多的过滤器



       在本教程中，我们将介绍如何实现一个现实世界的IoFilter用例。一般很容易实现一个IoFilter,但你可能还需要知道Mina内部的细节。任何相关的内部属性将被解释。

### 1.1.1. **已经存在的过滤器**

我们已经创建了许多的过滤器，下面用表格列举一下已经存在的过滤器，并附上简单的使用说明：
|**Filter**|**class**|**描述**|
|----|----|----|
|Blacklist|[BlacklistFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/firewall/BlacklistFilter.html)|块连接远程地址黑名单过滤器|
|Buffered Write|[BufferedWriteFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/buffer/BufferedWriteFilter.html)|用BufferedOutputStream做缓冲输出请求|
|Compression|[CompressionFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/compression/CompressionFilter.html)||
|ConnectionThrottle|[ConnectionThrottleFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/firewall/ConnectionThrottleFilter.html)||
|ErrorGenerating|[ErrorGeneratingFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/errorgenerating/ErrorGeneratingFilter.html)||
|Executor|[ExecutorFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/executor/ExecutorFilter.html)||
|FileRegionWrite|[FileRegionWriteFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/stream/FileRegionWriteFilter.html)||
|KeepAlive|[KeepAliveFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/keepalive/KeepAliveFilter.html)||
|Logging|[LoggingFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/logging/LoggingFilter.html)|日志记录事件消息，如：MessageReceived，MessageSent,SessionOpened,|
|MDC Injection|[MdcInjectionFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/logging/MdcInjectionFilter.html)|将IoSession 属性注入MDC|
|Noop|[NoopFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/util/NoopFilter.html)|一个什么也不做的过滤器，只对测试有用|
|Profiler|[ProfilerTimerFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/statistic/ProfilerTimerFilter.html)|剖析事件消息，如： MessageReceived, MessageSent, SessionOpened, ...|
|ProtocolCodec|[ProtocolCodecFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/codec/ProtocolCodecFilter.html)|编解码过滤器|
|Proxy|[ProxyFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/proxy/filter/ProxyFilter.html)||
|Reference counting|[ReferenceCountingFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/util/ReferenceCountingFilter.html)|跟踪数字用法的过滤器|
|RequestResponse|[RequestResponseFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/reqres/RequestResponseFilter.html)||
|SessionAttributeInitializing|[SessionAttributeInitializingFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/util/SessionAttributeInitializingFilter.html)||
|StreamWrite|[StreamWriteFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/stream/StreamWriteFilter.html)||
|SslFilter|[SslFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/ssl/SslFilter.html)||
|WriteRequest|[WriteRequestFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/util/WriteRequestFilter.html)||



### 1.1.2. **可选事件的重载**

您可以扩展IoAdapter代替直接实现IoFilter。除非重载,任何收到事件将立即期待下一个过滤器:

```java
public class MyFilter extends IoFilterAdapter {

    @Override

    public void sessionOpened(NextFilter nextFilter, IoSession session) throws Exception {

        // Some logic here...

        nextFilter.sessionOpened(session);

        // Some other logic here...

    }}
```

### 1.1.3. **转换一个写入请求**

如果你要通过IoSession.write()改变传入写请求,事情可能会变得很棘手。例如,让我们假设你的过滤器会从HighLevelMessage到 LowLevelMessage进行过滤，IoSession.write()被 HighLevelMessage对象所调用。你可以插入适当的转换代码过滤的filterWrite()方法,并认为这样就够了。然而,你必须注意,您还需要照顾messageSent事件，因为IoHandler或任何过滤器接下来会期望messageSent()将HighLevelMessage作为方法的参数。由于它是非理性的，当调用者HighLevelMessage准备通知LowLevelMessage，实际上LowLevelMessage已发送完成。因此,如果你的过滤器执行转换，你必须实现filterWrite()和messageSent()。


       仍需注意,您还需要执行类似的机制,即使输入和输出对象的类型是相同的(例如CompressionFilter)。因为IoSession.write()的调用者将期望他通过messageSent()写入到对应的handler方法。

假设你正在实施一个过滤器,将一个字符串转换为一个char[]。你的过滤器filterWrite()将看起来像下面的:

```java
public void filterWrite(NextFilter nextFilter, IoSession session, WriteRequest request) {

    nextFilter.filterWrite(

        session, new DefaultWriteRequest(

                ((String) request.getMessage()).toCharArray(), request.getFuture(), request.getDestination()));
}
```

现在,我们需要在messageSent()方法中做反转:

```java
public void messageSent(NextFilter nextFilter, IoSession session, Object message) {

    nextFilter.messageSent(session, new String((char[]) message));
}
```

String-to-ByteBuffer转换呢?我们可以更有效率,因为我们不需要重建原始消息(字符串)。然而,这是比之前更复杂的例子：

```java
public void filterWrite(NextFilter nextFilter, IoSession session, WriteRequest request) {

    String m = (String) request.getMessage();

    ByteBuffer newBuffer = new MyByteBuffer(m, ByteBuffer.wrap(m.getBytes());

 

    nextFilter.filterWrite(

            session, new WriteRequest(newBuffer, request.getFuture(), request.getDestination()));}

public void messageSent(NextFilter nextFilter, IoSession session, Object message) {

    if (message instanceof MyByteBuffer) {

        nextFilter.messageSent(session, ((MyByteBuffer) message).originalValue);

    } else {

        nextFilter.messageSent(session, message);

    }}

private static class MyByteBuffer extends ByteBufferProxy {

    private final Object originalValue;

    private MyByteBuffer(Object originalValue, ByteBuffer encodedValue) {

        super(encodedValue);

        this.originalValue = originalValue;

    }}
```

如果您使用的是Mina2.0,他与1.0和1.1有些不同。请同时参考[CompressionFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/compression/CompressionFilter.html)和[RequestResponseFilter](http://mina.apache.org/mina-project/xref/org/apache/mina/filter/reqres/RequestResponseFilter.html)。

### 1.1.4. **当心过滤器sessionCreated事件**

sessionCreated是一个特殊的事件,必须在I / O处理器线程中执行(参见配置线程模型)。sessionCreated从不转发事件给其他线程。

```java
public void sessionCreated(NextFilter nextFilter, IoSession session) throws Exception {

    // ...

    nextFilter.sessionCreated(session);}

// DON'T DO THIS!public void sessionCreated(final NextFilter nextFilter, final IoSession session) throws Exception {

    Executor executor = ...;

    executor.execute(new Runnable() {

        nextFilter.sessionCreated(session);

        });

    }
```

### 1.1.5. **注意空的Buffers缓冲区**

 Mina使用空缓冲区作为一个内部信号的情况。空缓冲区有时会成为一个问题,因为它会导致IndexOutOfBoundsException等各种异常。本节解释如何避免这种难以预料的情况。

ProtocolCodecFilter（编解码过滤器）使用一个空缓冲区(即buf.hasRemaining()= 0)标记消息结尾。如果你的过滤器放置在ProtocolCodecFilter之前,请确保你的过滤器处理空缓冲区。如果缓冲区为空，你的下一个过滤器过滤实现可能会抛出一个意想不到的异常:

```java
public void messageSent(NextFilter nextFilter, IoSession session, Object message) {

    if (message instanceof ByteBuffer && !((ByteBuffer) message).hasRemaining()) {

        nextFilter.messageSent(nextFilter, session, message);

        return;

    }

    ...}

public void filterWrite(NextFilter nextFilter, IoSession session, WriteRequest request) {

    Object message = request.getMessage();

    if (message instanceof ByteBuffer && !((ByteBuffer) message).hasRemaining()) {

        nextFilter.filterWrite(nextFilter, session, request);

        return;

    }

    ...}
```


我们总是要为每个过滤器插入if块吗?幸运的是,你不需要。处理空缓冲区的黄金法则:
- 如果你的过滤器工作没有任何问题,即使缓冲区为空,不需要添加if块。　
- 如果你的过滤器放置ProtocolCodecFilter之后,你不需要添加if块。
- 否则,你需要添加if块。



如果你需要if块,请记住你不总是需要遵循上面的例子。您可以检查是否缓冲区为空，无论你想怎样都行，只要你的过滤器不抛出一个意想不到的异常。




