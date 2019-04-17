# Netty编解码框架：Java序列化、Protobuf、 Marshalling - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月22日 11:44:51[boonya](https://me.csdn.net/boonya)阅读数：752








文章来源：[http://www.infoq.com/cn/articles/netty-codec-framework-analyse/](http://www.infoq.com/cn/articles/netty-codec-framework-analyse/)


## 1. 编解码技术

通常我们也习惯将编码（Encode）称为序列化（serialization），它将对象序列化为字节数组，用于网络传输、数据持久化或者其它用途。反之，解码（Decode）/反序列化（deserialization）把从网络、磁盘等读取的字节数组还原成原始对象（通常是原始对象的拷贝），以方便后续的业务逻辑操作。进行远程跨进程服务调用时（例如RPC调用），需要使用特定的编解码技术，对需要进行网络传输的对象做编码或者解码，以便完成远程调用。

### 1.2. 常用的编解码框架

#### 1.2.1. Java序列化

相信大多数Java程序员接触到的第一种序列化或者编解码技术就是Java默认提供的序列化机制，需要序列化的Java对象只需要实现java.io.Serializable接口并生成序列化ID，这个类就能够通过java.io.ObjectInput和java.io.ObjectOutput序列化和反序列化。

由于使用简单，开发门槛低，Java序列化得到了广泛的应用，但是由于它自身存在很多缺点，因此大多数的RPC框架并没有选择它。Java序列化的主要缺点如下：

1) 无法跨语言：是Java序列化最致命的问题。对于跨进程的服务调用，服务提供者可能会使用C++或者其它语言开发，当我们需要和异构语言进程交互时，Java序列化就难以胜任。由于Java序列化技术是Java语言内部的私有协议，其它语言并不支持，对于用户来说它完全是黑盒。Java序列化后的字节数组，别的语言无法进行反序列化，这就严重阻碍了它的应用范围；

2) 序列化后的码流太大: 例如使用二进制编解码技术对同一个复杂的POJO对象进行编码，它的码流仅仅为Java序列化之后的20%左右；目前主流的编解码框架，序列化之后的码流都远远小于原生的Java序列化；

3) 序列化效率差：在相同的硬件条件下、对同一个POJO对象做100W次序列化，二进制编码和Java原生序列化的性能对比测试如下图所示：Java原生序列化的耗时是二进制编码的16.2倍，效率非常差。

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329020.png)

图1-1 二进制编码和Java原生序列化性能对比

#### 1.2.2. Google的Protobuf

Protobuf全称Google Protocol Buffers，它由谷歌开源而来，在谷歌内部久经考验。它将数据结构以.proto文件进行描述，通过代码生成工具可以生成对应数据结构的POJO对象和Protobuf相关的方法和属性。

它的特点如下：

1) 结构化数据存储格式（XML，JSON等）；

2) 高效的编解码性能；

3) 语言无关、平台无关、扩展性好；

4) 官方支持Java、C++和Python三种语言。

首先我们来看下为什么不使用XML，尽管XML的可读性和可扩展性非常好，也非常适合描述数据结构，但是XML解析的时间开销和XML为了可读性而牺牲的空间开销都非常大，因此不适合做高性能的通信协议。Protobuf使用二进制编码，在空间和性能上具有更大的优势。

Protobuf另一个比较吸引人的地方就是它的数据描述文件和代码生成机制，利用数据描述文件对数据结构进行说明的优点如下：

1) 文本化的数据结构描述语言，可以实现语言和平台无关，特别适合异构系统间的集成；

2) 通过标识字段的顺序，可以实现协议的前向兼容；

3) 自动代码生成，不需要手工编写同样数据结构的C++和Java版本；

4) 方便后续的管理和维护。相比于代码，结构化的文档更容易管理和维护。

#### 1.2.3. Apache的Thrift

Thrift源于Facebook，在2007年Facebook将Thrift作为一个开源项目提交给Apache基金会。对于当时的Facebook来说，创造Thrift是为了解决Facebook各系统间大数据量的传输通信以及系统之间语言环境不同需要跨平台的特性，因此Thrift可以支持多种程序语言，如C++、C#、Cocoa、Erlang、Haskell、Java、Ocami、Perl、PHP、Python、Ruby和Smalltalk。

在多种不同的语言之间通信，Thrift可以作为高性能的通信中间件使用，它支持数据（对象）序列化和多种类型的RPC服务。Thrift适用于静态的数据交换，需要先确定好它的数据结构，当数据结构发生变化时，必须重新编辑IDL文件，生成代码和编译，这一点跟其他IDL工具相比可以视为是Thrift的弱项。Thrift适用于搭建大型数据交换及存储的通用工具，对于大型系统中的内部数据传输，相对于JSON和XML在性能和传输大小上都有明显的优势。

Thrift主要由5部分组成：

1) 语言系统以及IDL编译器：负责由用户给定的IDL文件生成相应语言的接口代码；

2) TProtocol：RPC的协议层，可以选择多种不同的对象序列化方式，如JSON和Binary；

3) TTransport：RPC的传输层，同样可以选择不同的传输层实现，如socket、NIO、MemoryBuffer等；

4) TProcessor：作为协议层和用户提供的服务实现之间的纽带，负责调用服务实现的接口；

5) TServer：聚合TProtocol、TTransport和TProcessor等对象。

我们重点关注的是编解码框架，与之对应的就是TProtocol。由于Thrift的RPC服务调用和编解码框架绑定在一起，所以，通常我们使用Thrift的时候会采取RPC框架的方式。但是，它的TProtocol编解码框架还是可以以类库的方式独立使用的。

与Protobuf比较类似的是，Thrift通过IDL描述接口和数据结构定义，它支持8种Java基本类型、Map、Set和List，支持可选和必选定义，功能非常强大。因为可以定义数据结构中字段的顺序，所以它也可以支持协议的前向兼容。

Thrift支持三种比较典型的编解码方式：

1) 通用的二进制编解码；

2) 压缩二进制编解码；

3) 优化的可选字段压缩编解码。

由于支持二进制压缩编解码，Thrift的编解码性能表现也相当优异，远远超过Java序列化和RMI等。

#### 1.2.4. JBoss Marshalling

JBoss Marshalling是一个Java对象的序列化API包，修正了JDK自带的序列化包的很多问题，但又保持跟java.io.Serializable接口的兼容；同时增加了一些可调的参数和附加的特性，并且这些参数和特性可通过工厂类进行配置。

相比于传统的Java序列化机制，它的优点如下：

1) 可插拔的类解析器，提供更加便捷的类加载定制策略，通过一个接口即可实现定制；

2) 可插拔的对象替换技术，不需要通过继承的方式；

3) 可插拔的预定义类缓存表，可以减小序列化的字节数组长度，提升常用类型的对象序列化性能；

4) 无须实现java.io.Serializable接口，即可实现Java序列化；

5) 通过缓存技术提升对象的序列化性能。

相比于前面介绍的两种编解码框架，JBoss Marshalling更多是在JBoss内部使用，应用范围有限。

#### 1.2.5. 其它编解码框架

除了上述介绍的编解码框架和技术之外，比较常用的还有MessagePack、kryo、hession和Json等。限于篇幅所限，不再一一枚举，感兴趣的朋友可以自行查阅相关资料学习。

## 2. Netty编解码框架

### 2.1. Netty为什么要提供编解码框架

作为一个高性能的异步、NIO通信框架，编解码框架是Netty的重要组成部分。尽管站在微内核的角度看，编解码框架并不是Netty微内核的组成部分，但是通过ChannelHandler定制扩展出的编解码框架却是不可或缺的。

下面我们从几个角度详细谈下这个话题，首先一起看下Netty的逻辑架构图：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329021.png)

图2-1 Netty逻辑架构图

从网络读取的inbound消息，需要经过解码，将二进制的数据报转换成应用层协议消息或者业务消息，才能够被上层的应用逻辑识别和处理；同理，用户发送到网络的outbound业务消息，需要经过编码转换成二进制字节数组（对于Netty就是ByteBuf）才能够发送到网络对端。编码和解码功能是NIO框架的有机组成部分，无论是由业务定制扩展实现，还是NIO框架内置编解码能力，该功能是必不可少的。

为了降低用户的开发难度，Netty对常用的功能和API做了装饰，以屏蔽底层的实现细节。编解码功能的定制，对于熟悉Netty底层实现的开发者而言，直接基于ChannelHandler扩展开发，难度并不是很大。但是对于大多数初学者或者不愿意去了解底层实现细节的用户，需要提供给他们更简单的类库和API，而不是ChannelHandler。

Netty在这方面做得非常出色，针对编解码功能，它既提供了通用的编解码框架供用户扩展，又提供了常用的编解码类库供用户直接使用。在保证定制扩展性的基础之上，尽量降低用户的开发工作量和开发门槛，提升开发效率。

Netty预置的编解码功能列表如下：base64、Protobuf、JBoss Marshalling、spdy等。

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329022.png)

图2-2 Netty预置的编解码功能列表

### 2.2. 常用的解码器

#### 2.2.1. LineBasedFrameDecoder解码器

LineBasedFrameDecoder是回车换行解码器，如果用户发送的消息以回车换行符作为消息结束的标识，则可以直接使用Netty的LineBasedFrameDecoder对消息进行解码，只需要在初始化Netty服务端或者客户端时将LineBasedFrameDecoder正确的添加到ChannelPipeline中即可，不需要自己重新实现一套换行解码器。

LineBasedFrameDecoder的工作原理是它依次遍历ByteBuf中的可读字节，判断看是否有“\n”或者“\r\n”，如果有，就以此位置为结束位置，从可读索引到结束位置区间的字节就组成了一行。它是以换行符为结束标志的解码器，支持携带结束符或者不携带结束符两种解码方式，同时支持配置单行的最大长度。如果连续读取到最大长度后仍然没有发现换行符，就会抛出异常，同时忽略掉之前读到的异常码流。防止由于数据报没有携带换行符导致接收到ByteBuf无限制积压，引起系统内存溢出。

它的使用效果如下：
解码之前：
+------------------------------------------------------------------+
                        接收到的数据报
“This is a netty example for using the nio framework.\r\n When you“
+------------------------------------------------------------------+
解码之后的ChannelHandler接收到的Object如下：
+------------------------------------------------------------------+
                        解码之后的文本消息
“This is a netty example for using the nio framework.“
+------------------------------------------------------------------+
通常情况下，LineBasedFrameDecoder会和StringDecoder配合使用，组合成按行切换的文本解码器，对于文本类协议的解析，文本换行解码器非常实用，例如对HTTP消息头的解析、FTP协议消息的解析等。

下面我们简单给出文本换行解码器的使用示例：
@Override
protected void initChannel(SocketChannel arg0) throws Exception {
   arg0.pipeline().addLast(new LineBasedFrameDecoder(1024));
   arg0.pipeline().addLast(new StringDecoder());
   arg0.pipeline().addLast(new UserServerHandler());
}

初始化Channel的时候，首先将LineBasedFrameDecoder添加到ChannelPipeline中，然后再依次添加字符串解码器StringDecoder，业务Handler。

#### 2.2.2. DelimiterBasedFrameDecoder解码器

DelimiterBasedFrameDecoder是分隔符解码器，用户可以指定消息结束的分隔符，它可以自动完成以分隔符作为码流结束标识的消息的解码。回车换行解码器实际上是一种特殊的DelimiterBasedFrameDecoder解码器。

分隔符解码器在实际工作中也有很广泛的应用，笔者所从事的电信行业，很多简单的文本私有协议，都是以特殊的分隔符作为消息结束的标识，特别是对于那些使用长连接的基于文本的私有协议。

分隔符的指定：与大家的习惯不同，分隔符并非以char或者string作为构造参数，而是ByteBuf，下面我们就结合实际例子给出它的用法。

假如消息以“$_”作为分隔符，服务端或者客户端初始化ChannelPipeline的代码实例如下：
@Override
public void initChannel(SocketChannel ch)
	throws Exception {
    ByteBuf delimiter = Unpooled.copiedBuffer("$_"
	    .getBytes());
   ch.pipeline().addLast(
	    new DelimiterBasedFrameDecoder(1024,
		    delimiter));
   ch.pipeline().addLast(new StringDecoder());
   ch.pipeline().addLast(new UserServerHandler());
}

首先将“$_”转换成ByteBuf对象，作为参数构造DelimiterBasedFrameDecoder，将其添加到ChannelPipeline中，然后依次添加字符串解码器（通常用于文本解码）和用户Handler，请注意解码器和Handler的添加顺序，如果顺序颠倒，会导致消息解码失败。

DelimiterBasedFrameDecoder原理分析：解码时，判断当前已经读取的ByteBuf中是否包含分隔符ByteBuf，如果包含，则截取对应的ByteBuf返回，源码如下：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329023.png)

详细分析下indexOf(buffer, delim)方法的实现，代码如下：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329024.png)

该算法与Java String中的搜索算法类似，对于原字符串使用两个指针来进行搜索，如果搜索成功，则返回索引位置，否则返回-1。

#### 2.2.3. FixedLengthFrameDecoder解码器

FixedLengthFrameDecoder是固定长度解码器，它能够按照指定的长度对消息进行自动解码，开发者不需要考虑TCP的粘包/拆包等问题，非常实用。

对于定长消息，如果消息实际长度小于定长，则往往会进行补位操作，它在一定程度上导致了空间和资源的浪费。但是它的优点也是非常明显的，编解码比较简单，因此在实际项目中仍然有一定的应用场景。

利用FixedLengthFrameDecoder解码器，无论一次接收到多少数据报，它都会按照构造函数中设置的固定长度进行解码，如果是半包消息，FixedLengthFrameDecoder会缓存半包消息并等待下个包到达后进行拼包，直到读取到一个完整的包。

假如单条消息的长度是20字节，使用FixedLengthFrameDecoder解码器的效果如下：
解码前：
+------------------------------------------------------------------+
                        接收到的数据报
“HELLO NETTY FOR USER DEVELOPER“
+------------------------------------------------------------------+
解码后：
+------------------------------------------------------------------+
                        解码后的数据报
“HELLO NETTY FOR USER“
+------------------------------------------------------------------+
#### 2.2.4. LengthFieldBasedFrameDecoder解码器

了解TCP通信机制的读者应该都知道TCP底层的粘包和拆包，当我们在接收消息的时候，显示不能认为读取到的报文就是个整包消息，特别是对于采用非阻塞I/O和长连接通信的程序。

如何区分一个整包消息，通常有如下4种做法：

1) 固定长度，例如每120个字节代表一个整包消息，不足的前面补位。解码器在处理这类定常消息的时候比较简单，每次读到指定长度的字节后再进行解码；

2) 通过回车换行符区分消息，例如HTTP协议。这类区分消息的方式多用于文本协议；

3) 通过特定的分隔符区分整包消息；

4) 通过在协议头/消息头中设置长度字段来标识整包消息。

前三种解码器之前的章节已经做了详细介绍，下面让我们来一起学习最后一种通用解码器-LengthFieldBasedFrameDecoder。

大多数的协议（私有或者公有），协议头中会携带长度字段，用于标识消息体或者整包消息的长度，例如SMPP、HTTP协议等。由于基于长度解码需求的通用性，以及为了降低用户的协议开发难度，Netty提供了LengthFieldBasedFrameDecoder，自动屏蔽TCP底层的拆包和粘包问题，只需要传入正确的参数，即可轻松解决“读半包“问题。

下面我们看看如何通过参数组合的不同来实现不同的“半包”读取策略。第一种常用的方式是消息的第一个字段是长度字段，后面是消息体，消息头中只包含一个长度字段。它的消息结构定义如图所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329025.png)

图2-3 解码前的字节缓冲区（14字节）

使用以下参数组合进行解码：

1) lengthFieldOffset = 0；

2) lengthFieldLength = 2；

3) lengthAdjustment = 0；

4) initialBytesToStrip = 0。

解码后的字节缓冲区内容如图所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329026.png)

图2-4 解码后的字节缓冲区（14字节）

通过ByteBuf.readableBytes()方法我们可以获取当前消息的长度，所以解码后的字节缓冲区可以不携带长度字段，由于长度字段在起始位置并且长度为2，所以将initialBytesToStrip设置为2，参数组合修改为：

1) lengthFieldOffset = 0；

2) lengthFieldLength = 2；

3) lengthAdjustment = 0；

4) initialBytesToStrip = 2。

解码后的字节缓冲区内容如图所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329027.png)

图2-5 跳过长度字段解码后的字节缓冲区（12字节）

解码后的字节缓冲区丢弃了长度字段，仅仅包含消息体，对于大多数的协议，解码之后消息长度没有用处，因此可以丢弃。

在大多数的应用场景中，长度字段仅用来标识消息体的长度，这类协议通常由消息长度字段+消息体组成，如上图所示的几个例子。但是，对于某些协议，长度字段还包含了消息头的长度。在这种应用场景中，往往需要使用lengthAdjustment进行修正。由于整个消息（包含消息头）的长度往往大于消息体的长度，所以，lengthAdjustment为负数。图2-6展示了通过指定lengthAdjustment字段来包含消息头的长度：

1) lengthFieldOffset = 0；

2) lengthFieldLength = 2；

3) lengthAdjustment = -2；

4) initialBytesToStrip = 0。

解码之前的码流：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329028.png)

图2-6 包含长度字段自身的码流

解码之后的码流：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329029.png)

图2-7 解码后的码流

由于协议种类繁多，并不是所有的协议都将长度字段放在消息头的首位，当标识消息长度的字段位于消息头的中间或者尾部时，需要使用lengthFieldOffset字段进行标识，下面的参数组合给出了如何解决消息长度字段不在首位的问题：

1) lengthFieldOffset = 2；

2) lengthFieldLength = 3；

3) lengthAdjustment = 0；

4) initialBytesToStrip = 0。

其中lengthFieldOffset表示长度字段在消息头中偏移的字节数，lengthFieldLength 表示长度字段自身的长度，解码效果如下：

解码之前：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329030.png)

图2-8 长度字段偏移的原始码流

解码之后：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329031.png)

图2-9长度字段偏移解码后的码流

由于消息头1的长度为2，所以长度字段的偏移量为2；消息长度字段Length为3，所以lengthFieldLength值为3。由于长度字段仅仅标识消息体的长度，所以lengthAdjustment和initialBytesToStrip都为0。

最后一种场景是长度字段夹在两个消息头之间或者长度字段位于消息头的中间，前后都有其它消息头字段，在这种场景下如果想忽略长度字段以及其前面的其它消息头字段，则可以通过initialBytesToStrip参数来跳过要忽略的字节长度，它的组合配置示意如下：

1) lengthFieldOffset = 1；

2) lengthFieldLength = 2；

3) lengthAdjustment = 1；

4) initialBytesToStrip = 3。

解码之前的码流（16字节）：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329032.png)

图2-10长度字段夹在消息头中间的原始码流（16字节）

解码之后的码流（13字节）：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329033.png)

图2-11长度字段夹在消息头中间解码后的码流（13字节）

由于HDR1的长度为1，所以长度字段的偏移量lengthFieldOffset为1；长度字段为2个字节，所以lengthFieldLength为2。由于长度字段是消息体的长度，解码后如果携带消息头中的字段，则需要使用lengthAdjustment进行调整，此处它的值为1，代表的是HDR2的长度，最后由于解码后的缓冲区要忽略长度字段和HDR1部分，所以lengthAdjustment为3。解码后的结果为13个字节，HDR1和Length字段被忽略。

事实上，通过4个参数的不同组合，可以达到不同的解码效果，用户在使用过程中可以根据业务的实际情况进行灵活调整。

由于TCP存在粘包和组包问题，所以通常情况下用户需要自己处理半包消息。利用LengthFieldBasedFrameDecoder解码器可以自动解决半包问题，它的习惯用法如下：
pipeline.addLast("frameDecoder", new LengthFieldBasedFrameDecoder(65536，0，2));
pipeline.addLast("UserDecoder", new UserDecoder());

在pipeline中增加LengthFieldBasedFrameDecoder解码器，指定正确的参数组合，它可以将Netty的ByteBuf解码成整包消息，后面的用户解码器拿到的就是个完整的数据报，按照逻辑正常进行解码即可，不再需要额外考虑“读半包”问题，降低了用户的开发难度。

### 2.3. 常用的编码器

Netty并没有提供与2.2章节匹配的编码器，原因如下：

1) 2.2章节介绍的4种常用的解码器本质都是解析一个完整的数据报给后端，主要用于解决TCP底层粘包和拆包；对于编码，就是将POJO对象序列化为ByteBuf，不需要与TCP层面打交道，也就不存在半包编码问题。从应用场景和需要解决的实际问题角度看，双方是非对等的；

2) 很难抽象出合适的编码器，对于不同的用户和应用场景，序列化技术不尽相同，在Netty底层统一抽象封装也并不合适。

Netty默认提供了丰富的编解码框架供用户集成使用，本文对较常用的Java序列化编码器进行讲解。其它的编码器，实现方式大同小异。

#### 2.3.1. ObjectEncoder编码器

ObjectEncoder是Java序列化编码器，它负责将实现Serializable接口的对象序列化为byte []，然后写入到ByteBuf中用于消息的跨网络传输。

下面我们一起分析下它的实现：

首先，我们发现它继承自MessageToByteEncoder，它的作用就是将对象编码成ByteBuf：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329034.png)

如果要使用Java序列化，对象必须实现Serializable接口，因此，它的泛型类型为Serializable。

MessageToByteEncoder的子类只需要实现encode(ChannelHandlerContext ctx, I msg, ByteBuf out)方法即可，下面我们重点关注encode方法的实现：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329035.png)

首先创建ByteBufOutputStream和ObjectOutputStream，用于将Object对象序列化到ByteBuf中，值得注意的是在writeObject之前需要先将长度字段（4个字节）预留，用于后续长度字段的更新。

依次写入长度占位符（4字节）、序列化之后的Object对象，之后根据ByteBuf的writeIndex计算序列化之后的码流长度，最后调用ByteBuf的setInt(int index, int value)更新长度占位符为实际的码流长度。

有个细节需要注意，更新码流长度字段使用了setInt方法而不是writeInt，原因就是setInt方法只更新内容，并不修改readerIndex和writerIndex。

## 3. Netty编解码框架可定制性

尽管Netty预置了丰富的编解码类库功能，但是在实际的业务开发过程中，总是需要对编解码功能做一些定制。使用Netty的编解码框架，可以非常方便的进行协议定制。本章节将对常用的支持定制的编解码类库进行讲解，以期让读者能够尽快熟悉和掌握编解码框架。

### 3.1. 解码器

#### 3.1.1. ByteToMessageDecoder抽象解码器

使用NIO进行网络编程时，往往需要将读取到的字节数组或者字节缓冲区解码为业务可以使用的POJO对象。为了方便业务将ByteBuf解码成业务POJO对象，Netty提供了ByteToMessageDecoder抽象工具解码类。

用户自定义解码器继承ByteToMessageDecoder，只需要实现void decode（ChannelHandler Context ctx, ByteBuf in, List<Object> out）抽象方法即可完成ByteBuf到POJO对象的解码。

由于ByteToMessageDecoder并没有考虑TCP粘包和拆包等场景，用户自定义解码器需要自己处理“读半包”问题。正因为如此，大多数场景不会直接继承ByteToMessageDecoder，而是继承另外一些更高级的解码器来屏蔽半包的处理。

实际项目中，通常将LengthFieldBasedFrameDecoder和ByteToMessageDecoder组合使用，前者负责将网络读取的数据报解码为整包消息，后者负责将整包消息解码为最终的业务对象。

除了和其它解码器组合形成新的解码器之外，ByteToMessageDecoder也是很多基础解码器的父类，它的继承关系如下图所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329036.png)

图3-1 ByteToMessageDecoder继承关系图

#### 3.1.2. MessageToMessageDecoder抽象解码器

MessageToMessageDecoder实际上是Netty的二次解码器，它的职责是将一个对象二次解码为其它对象。

为什么称它为二次解码器呢？我们知道，从SocketChannel读取到的TCP数据报是ByteBuffer，实际就是字节数组。我们首先需要将ByteBuffer缓冲区中的数据报读取出来，并将其解码为Java对象；然后对Java对象根据某些规则做二次解码，将其解码为另一个POJO对象。因为MessageToMessageDecoder在ByteToMessageDecoder之后，所以称之为二次解码器。

二次解码器在实际的商业项目中非常有用，以HTTP+XML协议栈为例，第一次解码往往是将字节数组解码成HttpRequest对象，然后对HttpRequest消息中的消息体字符串进行二次解码，将XML格式的字符串解码为POJO对象，这就用到了二次解码器。类似这样的场景还有很多，不再一一枚举。

事实上，做一个超级复杂的解码器将多个解码器组合成一个大而全的MessageToMessageDecoder解码器似乎也能解决多次解码的问题，但是采用这种方式的代码可维护性会非常差。例如，如果我们打算在HTTP+XML协议栈中增加一个打印码流的功能，即首次解码获取HttpRequest对象之后打印XML格式的码流。如果采用多个解码器组合，在中间插入一个打印消息体的Handler即可，不需要修改原有的代码；如果做一个大而全的解码器，就需要在解码的方法中增加打印码流的代码，可扩展性和可维护性都会变差。

用户的解码器只需要实现void decode(ChannelHandlerContext ctx, I msg, List<Object> out)抽象方法即可，由于它是将一个POJO解码为另一个POJO，所以一般不会涉及到半包的处理，相对于ByteToMessageDecoder更加简单些。它的继承关系图如下所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329037.png)

图3-2 MessageToMessageDecoder 解码器继承关系图

### 3.2. 编码器

#### 3.2.1. MessageToByteEncoder抽象编码器

MessageToByteEncoder负责将POJO对象编码成ByteBuf，用户的编码器继承Message ToByteEncoder，实现void encode(ChannelHandlerContext ctx, I msg, ByteBuf out)接口接口，示例代码如下：
public class IntegerEncoder extends MessageToByteEncoder<Integer> {
      @Override
      public void encode(ChannelHandlerContext ctx, Integer msg,ByteBuf out)
         throws Exception {
             out.writeInt(msg);
          }
      }

它的实现原理如下：调用write操作时，首先判断当前编码器是否支持需要发送的消息，如果不支持则直接透传；如果支持则判断缓冲区的类型，对于直接内存分配ioBuffer（堆外内存），对于堆内存通过heapBuffer方法分配，源码如下：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329038.png)

编码使用的缓冲区分配完成之后，调用encode抽象方法进行编码，方法定义如下：它由子类负责具体实现。

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329039.png)

编码完成之后，调用ReferenceCountUtil的release方法释放编码对象msg。对编码后的ByteBuf进行以下判断：

1) 如果缓冲区包含可发送的字节，则调用ChannelHandlerContext的write方法发送ByteBuf；

2) 如果缓冲区没有包含可写的字节，则需要释放编码后的ByteBuf，写入一个空的ByteBuf到ChannelHandlerContext中。

发送操作完成之后，在方法退出之前释放编码缓冲区ByteBuf对象。

#### 3.2.2. MessageToMessageEncoder抽象编码器

将一个POJO对象编码成另一个对象，以HTTP+XML协议为例，它的一种实现方式是：先将POJO对象编码成XML字符串，再将字符串编码为HTTP请求或者应答消息。对于复杂协议，往往需要经历多次编码，为了便于功能扩展，可以通过多个编码器组合来实现相关功能。

用户的解码器继承MessageToMessageEncoder解码器，实现void encode(Channel HandlerContext ctx, I msg, List<Object> out)方法即可。注意，它与MessageToByteEncoder的区别是输出是对象列表而不是ByteBuf，示例代码如下：
public class IntegerToStringEncoder extends MessageToMessageEncoder <Integer> 
 {
          @Override
          public void encode(ChannelHandlerContext ctx, Integer message, 
             List<Object> out)
                  throws Exception 
          {
              out.add(message.toString());
          }
      }
MessageToMessageEncoder编码器的实现原理与之前分析的MessageToByteEncoder相似，唯一的差别是它编码后的输出是个中间对象，并非最终可传输的ByteBuf。

简单看下它的源码实现：创建RecyclableArrayList对象，判断当前需要编码的对象是否是编码器可处理的类型，如果不是，则忽略，执行下一个ChannelHandler的write方法。

具体的编码方法实现由用户子类编码器负责完成，如果编码后的RecyclableArrayList为空，说明编码没有成功，释放RecyclableArrayList引用。

如果编码成功，则通过遍历RecyclableArrayList，循环发送编码后的POJO对象，代码如下所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329040.png)

#### 3.2.3. LengthFieldPrepender编码器

如果协议中的第一个字段为长度字段，Netty提供了LengthFieldPrepender编码器，它可以计算当前待发送消息的二进制字节长度，将该长度添加到ByteBuf的缓冲区头中，如图所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329041.png)

图3-3 LengthFieldPrepender编码器

通过LengthFieldPrepender可以将待发送消息的长度写入到ByteBuf的前2个字节，编码后的消息组成为长度字段+原消息的方式。

通过设置LengthFieldPrepender为true，消息长度将包含长度本身占用的字节数，打开LengthFieldPrepender后，图3-3示例中的编码结果如下图所示：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329042.png)

图3-4 打开LengthFieldPrepender开关后编码效果

LengthFieldPrepender工作原理分析如下：首先对长度字段进行设置，如果需要包含消息长度自身，则在原来长度的基础之上再加上lengthFieldLength的长度。

如果调整后的消息长度小于0，则抛出参数非法异常。对消息长度自身所占的字节数进行判断，以便采用正确的方法将长度字段写入到ByteBuf中，共有以下6种可能：

1) 长度字段所占字节为1：如果使用1个Byte字节代表消息长度，则最大长度需要小于256个字节。对长度进行校验，如果校验失败，则抛出参数非法异常；若校验通过，则创建新的ByteBuf并通过writeByte将长度值写入到ByteBuf中；

2) 长度字段所占字节为2：如果使用2个Byte字节代表消息长度，则最大长度需要小于65536个字节，对长度进行校验，如果校验失败，则抛出参数非法异常；若校验通过，则创建新的ByteBuf并通过writeShort将长度值写入到ByteBuf中；

3) 长度字段所占字节为3：如果使用3个Byte字节代表消息长度，则最大长度需要小于16777216个字节，对长度进行校验，如果校验失败，则抛出参数非法异常；若校验通过，则创建新的ByteBuf并通过writeMedium将长度值写入到ByteBuf中；

4) 长度字段所占字节为4：创建新的ByteBuf，并通过writeInt将长度值写入到ByteBuf中；

5) 长度字段所占字节为8：创建新的ByteBuf，并通过writeLong将长度值写入到ByteBuf中；

6) 其它长度值：直接抛出Error。

相关代码如下：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329043.png)

最后将原需要发送的ByteBuf复制到List<Object> out中，完成编码：

![](http://cdn.infoqstatic.com/statics_s2_20170919-0429u7/resource/articles/netty-codec-framework-analyse/zh/resources/0329044.png)

## 4. 作者简介

**李林锋**，2007年毕业于东北大学，2008年进入华为公司从事高性能通信软件的设计和开发工作，有7年NIO设计和开发经验，精通Netty、Mina等NIO框架和平台中间件，现任华为软件平台架构部架构师，《Netty权威指南》作者。



## 5. 代码学习



Github地址：[https://github.com/SunflowersOfJava/boonya-learning-netty-book](https://github.com/SunflowersOfJava/boonya-learning-netty-book)
![](https://img-blog.csdn.net/20170922114903993)
注：代码下载导入自己的IDE就可以运行了，没什么好贴出来的还占用篇幅。








