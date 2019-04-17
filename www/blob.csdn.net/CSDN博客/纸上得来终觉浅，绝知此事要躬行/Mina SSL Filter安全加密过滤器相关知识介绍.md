# Mina SSL Filter安全加密过滤器相关知识介绍 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月15日 17:43:04[boonya](https://me.csdn.net/boonya)阅读数：3774
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina SSLFilter(Apahce Mina user guide Chapter11 SSL Filter)](http://mina.apache.org/mina-project/userguide/ch11-ssl-filter/ch11-ssl-filter.html)

SslFilter过滤器是负责管理数据的加密和解密通过安全连接。每当你需要建立一个安全连接,或将现有的连接使它安全,你必须添加SslFilter过滤器链。


任何会话可以修改它的信息过滤器链,它允许使用协议像startTLS打开连接。

请注意,虽然这个名字包括SSL,SslFilter支持TLS。实际上,TLS已经支持取代SSL,但是由于历史原因,SSL仍然广泛使用。

### 1.1.1. **基本应用**

如果你想让你的应用程序支持SSL / TLS,只需添加的SslFilter链:

```java
...

DefaultIoFilterChainBuilder chain = acceptor.getFilterChain();

SslFilter sslFilter = new SslFilter(sslContext);

chain.addFirst("sslFilter", sslFilter);

...
```
你当然也需要一个SslContext实例:


```java
SSLContext sslContext;

    try

    {

        // Initialize the SSLContext to work with our key managers.

        sslContext = SSLContext.getInstance( "TLS" );

        sslContext.init( ... ); // Provide the needed KeyManager[], TrustManager[] and SecureRandom instances

    }

    catch ( Exception e )

    {

        // Handle your exception

    }
```
这取决于你提供的KeyManager,TrustManager和SecureRandom实例。


确保注入SslFilter到过滤器链中的第一的位置!

稍后我们将看到如何创建一个SSLContext详细为例。

### 1.1.2. **理论**

如果你想要更深入地理解它是如何工作的,请阅读下面的段落…

#### 1.1.2.1. **基本的SSL**

我们不打算解释SSL是如何工作的，在此之外有非常[好的书](http://www.amazon.com/SSL-TLS-Designing-Building-Systems/dp/0201615983)。我们将给一个快速介绍它是如何工作的,以及它是如何在Mina中实现的。



首先,你必须明白SSL / TLS协议中定义的RFCs:[TLS
 1.0](https://www.rfc-editor.org/rfc/rfc2246.txt), [TLS 1.1](https://www.rfc-editor.org/rfc/rfc4346.txt) 和[TLS 1.2](https://www.rfc-editor.org/rfc/rfc5246.txt)，这里有关于[TLS
 1.3](https://tlswg.github.io/tls13-spec/)的介绍。

在成为TLS之前，它最初是由Netscape命名为SSL(从1.0到3.0)。如今,SSL
 2.0 * 3.0和SSL * *已经弃用并且不应该被使用了。

### 1.1.3. **SSL/TLS协议**

因为它是一个协议,它需要客户端和服务器之间的一些会话。这是所有的SSL
 / TLS是什么:关于描述该会话。

这足以知道任何保证交换排除是一个过程阶段，叫做握手，它的角色是客户端和服务器之间达成协议将加密方法来使用。一个基本的SSL
 / TLS会话将会看起来像下面这样:

![](https://img-blog.csdn.net/20160615173722428)

正如你所看到的这张照片,这是一个2阶段协议:第一次握手,然后完成客户端和服务器将能够交换数据加密。

#### 1.1.3.1. **握手**

一般地，它是关于谈判阶段许多元素将用于加密数据。在本文的上下文中细节不是很有趣的，足够说许多消息是客户机和服务器之间交换,并且没有数据可以发送在这个阶段。



实际上,握手开始有两个条件:服务器必须等待一些握手消息到达，客户端必须发送ClientHello消息。



我们使用Java SSLEngine类来管理整个SSL / TLS协议。Mina应该关注的现状是会话，这样它将能够获取和处理客户端HelloClient消息。当你在过滤器链中注入SslFilter，有几件事发生：
- 创建SslHandler实例(我们每个会话创建一个实例)。这个SslHandler实例负责整个处理(握手和即将到来的消息的加密/解密)。
- SslHandler使用SslContext实例创建一个SSLEngine，已附加到SslFilter。
- SslEngine被实例配置和初始化。
- SslHandler实例存储到Session会话中。
- 除非特殊需要，我们发起握手(有不同的含义在客户端和服务器端,客户端将发送ClientHello消息,而服务器切换到一个模式,它等待一些数据打开)。注意握手初始化以后可以做，如果需要的话。



  我们都是set。接下来的几个步骤是纯粹的SSL / TLS协议交换。如果session.write()方法被调用时,该消息将被排队等待握手完成。任何等待消息当时SslFilter添加到链会导致SSL
 / TLS握手失败,所以当你想注入时确保你有一个干净的地方。我们也不会收到任何消息,这并不是一个SSL
 / TLS协议消息。



最后一点很重要,如果你想实现StartTLS:因为它允许您的应用程序在任何时候切换，从一个纯文本交换加密交换，你要确保两侧没有等待消息。显然,在客户端——启动StartTLS——每个等待消息已被发送，在StartTLS可以发送消息之前，但它必须阻止任何其他信息,不属于以下握手,直到完成握手。在服务器端,一旦StartTLS消息已经收到,没有消息应该写入远程对等。



事实上，注入链中的SslFilter应该阻止任何交换不握手协议的一部分,直到握手完成。如果你提交一个消息发送和加密前的握手已经完成，消息不会被拒绝，但在握手已完成时会加入队列并处理。



在此之后，每个发送的消息将会通过SslHandler实例被加密,和每条消息收到必须通过SslHandler完全解密，然后提供给下一个过滤器。

#### 1.1.3.2. **发送数据**

好，握手已完成。你的SslFilter准备好处理传入和传出消息。让我们聚焦那些你要写的session会话。



一个重要的事情是你可以写多个消息在同一session会话(如果你有一个Executor链)。问题在于SSLEngine不能够处理多个消息。我们需要序列化消息被写入。更糟糕的是:你不能处理传入消息,并在同一时间传出消息。



总而言之，SSL
 / TLS处理就像一个黑盒，只接受一个输入并且无法处理任何东西，直到它完成了它的任务。以下为传出消息模式代表了它的工作方式。

![](https://img-blog.csdn.net/20160615174033823)




这与传入消息没什么不同，除了我们之间不会有一个Executor的IoProcessor和SslFilter。这让事情变得更简单，除了一个重要的事情发生：当我们处理传入消息的时候，我们不能处理输出消息。请注意，它还适用于反过来:当一个即将离任的消息被处理,我们不能处理传入消息:

![](https://img-blog.csdn.net/20160615174126762)


注意：重要的是SslHander无法处理超过一个消息。

### 1.1.4. **Mina2的SSL/TLS**

现在,我们将深入一点，深入Mina代码。我们将涵盖所有过滤器操作:
- **· Management**
- o init()
- o destroy()
- o onPreAdd(IoFilterChain, String, NextFilter)
- o onPostAdd(IoFilterChain, String, NextFilter)
- o onPreRemove(IoFilterChain, String, NextFilter)
- o onPostRemove(IoFilterChain, String, NextFilter)
- **· Session events**
- o sessionCreated(NextFilter, IoSession)
- o sessionOpened(NextFilter, IoSession)
- o sessionClosed(NextFilter, IoSession)
- o sessionIdle(NextFilter, IoSession, IdleStatus)
- o exceptionCaught(NextFilter, IoSession, Throwable)
- o filterClose(NextFilter, IoSession)
- o inputClosed(NextFilter, IoSession)
- **· Messages events**
- o messageReceived(NextFilter, IoSession, Object)
- o filterWrite(NextFilter, IoSession, WriteRequest)
- o messageSent(NextFilter, IoSession, WriteRequest)



#### 1.1.4.1. **Management**

这里有过滤器的管理方法:

##### 1.1.4.1.1. **onPreAdd**

这就是我们创建SslHandler实例,并初始化它。我们还定义了支持密码。

SslHandler实例将本身创建一个SSLEngine的实例，并配置SslFilter它所有的参数设置:
- 如果这是客户端或服务器端
- 在服务器端,国旗,表示我们想要或需要客户端身份验证
- 启用密码的列表
- 协议列表

当它完成的时候,这个实例的引用会存储在session会话的属性。

##### 1.1.4.1.2. **onPostAdd**

如果不影响推迟，这就是我们开始握手的地方。这就是这个方法要做的。所有的逻辑是由SslHandler实现。

##### 1.1.4.1.3. **onPreRemove**

在这里,我们停止SSL会话并清理会话(从会话的属性中删除过滤器从会话的链和SslHandler实例)。在刷新了任何尚未处理的事件之后，Sslhandler实例也要被销毁。

#### 1.1.4.2. **Session events**

这里有事件传播，通过SslFilter过滤器链并处理:

##### 1.1.4.2.1. **sessionClosed**

我们只是销毁SslHandler实例。

##### 1.1.4.2.2. **exceptionCaught**

我们有一个特殊的任务进行异常，由于关闭session会话引起：我们必须收集所有的消息回答epending将它们添加到异常传播。

##### 1.1.4.2.3. **filterClose**

在这里,如果有一个SSL会话开始,我们需要关闭它。在任何情况下,我们传播事件链到下一个过滤器。

#### 1.1.4.3. **Messages events**

最后,同样重要的是,这三个事件相对于消息:

##### 1.1.4.3.1. **messageReceived**

当我们从套接字读取一些数据时，收到这个事件。我们必须照顾一些角落情况:已完成握手。握手已经启动但仍未完成*没有握手开始,和SslHandler尚未初始化。



根据频率列出这三个用例的顺序。让我们看看会发生什么对这些用例。

##### 1.1.4.3.2. **The handshake has been completed**

好!这意味着每一个传入消息封装在一个SSL / TLS信封,而且应该解密。现在,我们讨论的是关于消息的，但我们实际上收到字节，可能需要进行聚合，以形成完整的信息(至少在TCP)。如果消息是支离破碎的，我们会收到许多缓冲区，当我们将收到最后一块时我们将能够完全解密它。记住，我们阻塞了所有的处理，它可以对这次session会话阻塞SslHandler实例很长一段时间……



在任何情况下,每个数据块都是由SslHandler处理,它代表SslEngine解密接收的字节。



这是我们已经实现的基本算法messageReceived():

get the session's sslHandler

syncrhonized on sshHandler {

    if handshake completed

        then

            get the sslHandler decrypting the data

            if the application buffer is completed, push it into the message to forward to the IoHandler

        else

            enqueue the incoming data}

flush the messages if any



这里的重要部分是SslHandler将累积数据,直到有一个完整的消息进入链。这可能需要一段时间，并且有许多套接字读取。原因是SSLEngine无法处理消息，除非它有所有字节符合完全解码的信息。



提示:增加传输缓冲区大小限制往返需要的数量来发送一个大消息。



##### 1.1.4.3.3. **The handshake has not been completed**

意味着收到消息是握手协议的一部分。没有将传播到IoHandler,消息将被SslHandler消费。

直到完成完整的握手,每个传入的数据将被视为一个握手规范消息。

同时,消息IoHandler将加入队列,等待握手完成。

这是一个模式代表完整的过程,当数据在两个传送接收:

![](https://img-blog.csdn.net/20160615174156301)

##### 1.1.4.3.4. **filterwWrite**

这个事件处理时IoSession.write()方法被调用。

如果SSL session会话没有开始,我们只是积累写的消息。它将在稍后发送。



在这里有一个棘手的参数,对于一些非常具体的需要。通常,当实现startTLS协议,服务器切换从一个非安全连接到一个安全连接的意思是一个应用程序消息(和潜在的响应),我们需要响应发送回客户机之前SslFilter安装(否则,响应将被阻塞,etablishement安全连接会失败)。这是DISABLE_ENCRYPTION_ONCE属性。它包含什么都无所谓(它可以是一个布尔值),这是足够的为这个参数在会话第一消息bypasse
 SslFilter。



我们控制在会话的属性中存在DISABLE_ENCRYPTION_ONCE标识,并且如果存在,我们将它从session会话中移除，并且推送消息进入消息队列被发送。

此外，如果握手还没有完成,我们保持消息队列中,如果完成,我们加密和调度写。

如果已经安排写一些消息，我们就释放。

##### 1.1.4.3.5. **messageSent **

在这里,它只是一种回到未加密的IoHandler消息传播。

### 1.1.5. **SSLContext初始化**

我们看到,为了建立SSL会话,我们需要创建一个SSLContext。这是代码:

```java
SSLContext sslContext;

try{

    // Initialize the SSLContext to work with our key managers.

    sslContext = SSLContext.getInstance( "TLS" );

    sslContext.init( ... );

   // Provide the needed KeyManager[], TrustManager[] and SecureRandom instances
}catch ( Exception e ){

    // Handle your exception
}
```

我们没有暴露的是构造函数和init()方法。

SSLContext可以影响——通过其构造函数创建,或者我们让静态工厂返回一个实例(这是我们所做的在前面的代码。第二种方法非常简单,适合大多数时候。这足以通过使用协议的名称,这是:
- · **SSL**
- · **SSLv2**
- · **SSLv3**
- · **TLS**
- · **TLSv1**
- · **TLSv1.1**
- · **TLSv1.2** (not supported in Java 6)

这里强烈建议选择更高的算法(即TLSv1.2)如果你的客户支持它。

init()方法接受3个参数:
- · a **KeyManager** (can be null)
- · a **TrustManager** (can be null)
- · a random generator (can be null)

如果参数被设置为null,安装安全提供者会选择优先级最高的实现。



