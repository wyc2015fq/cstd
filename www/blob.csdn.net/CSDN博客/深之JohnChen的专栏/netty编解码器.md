# netty编解码器 - 深之JohnChen的专栏 - CSDN博客

2017年12月02日 11:01:09[byxdaz](https://me.csdn.net/byxdaz)阅读数：763


﻿﻿

        netty提供了强大的编解码器框架，使得我们编写自定义的编解码器很容易，也容易封装个重用。

在网络应用中需要实现某种编解码器，将原始字节数据与自定义的消息对象进行互相转换。网络中都是以字节码的数据形式来传输数据的，服务器编码数据后发送到客户端，客户端需要对数据进行解码。编解码器由两部分组成：编码器、解码器。

解码器：负责将消息从字节或其他序列形式转成指定的消息对象； 

编码器：将消息对象转成字节或其他序列形式在网络上传输。 

        编码器和解码器的结构很简单，消息被编码后解码后会自动通过ReferenceCountUtil.release(message)释放，如果不想释放消息可以使用ReferenceCountUtil.retain(message)，这将会使引用数量增加而没有消息发布，大多数时候不需要这么做。

其实，各种编解码器的实现都是ChannelHandler的实现。

常用解码器

       TCP以流的方式传输数据，上层的应用协议为了对消息进行区分，经常采用的方式有如下4种：

Netty提供了4种解码器来解决对应的问题，分别是：

LineBasedFrameDecoder      换行符，将回车换行符作为消息结束符，例如：FTP协议，这种方式在文本协议中应用比较广泛。

DelimiterBaseFrameDecoder  分隔符，将特殊的分隔符作为消息的结束标志，回车换行符就是一种特殊的结束分隔符。

FixdLengthFrameDecoder     定长，消息长度固定，累计读取的长度总和为约定的定长长度后，就认为读到了一个完整的消息；将计数器置位，重新开始读取下一条报文。

LengthFieldBasedFrameDecoder 定义了一个长度的字段来表示消息的长度，因此能够处理可变长度的消息。将消息分为消息头和消息体，消息头固定位置增加一个表示长度的字段，通过长度字段来获取整包的信息。

      尽管Netty预置了丰富的编解码类库功能，但是在实际的业务开发过程中，总是需要对编解码功能做一些定制。使用Netty的编解码框架，可以非常方便的进行协议定制。Netty提供了丰富的解码器抽象基类，我们可以很容易的实现这些基类来自定义解码器。

ByteToMessageDecoder，解码字节到消息，使用NIO进行网络编程时，往往需要将读取到的字节数组或者字节缓冲区解码为业务可以使用的POJO对象。为了方便业务将ByteBuf解码成业务POJO对象。由于ByteToMessageDecoder并没有考虑TCP粘包和拆包等场景，用户自定义解码器需要自己处理“读半包”问题。正因为如此，大多数场景不会直接继承ByteToMessageDecoder，而是继承另外一些更高级的解码器来屏蔽半包的处理。实际项目中，通常将LengthFieldBasedFrameDecoder和ByteToMessageDecoder组合使用，前者负责将网络读取的数据报解码为整包消息，后者负责将整包消息解码为最终的业务对象。

MessageToMessageDecoder，解码消息到消息，MessageToMessageDecoder实际上是Netty的二次解码器，它的职责是将一个对象二次解码为其它对象。为什么称它为二次解码器呢？我们知道，从SocketChannel读取到的TCP数据报是ByteBuffer，实际就是字节数组。我们首先需要将ByteBuffer缓冲区中的数据报读取出来，并将其解码为Java对象；然后对Java对象根据某些规则做二次解码，将其解码为另一个POJO对象。因为MessageToMessageDecoder在ByteToMessageDecoder之后，所以称之为二次解码器。二次解码器在实际的商业项目中非常有用，以HTTP+XML协议栈为例，第一次解码往往是将字节数组解码成HttpRequest对象，然后对HttpRequest消息中的消息体字符串进行二次解码，将XML格式的字符串解码为POJO对象，这就用到了二次解码器。

常用编码器

        很难抽象出合适的编码器，对于不同的用户和应用场景，序列化技术不尽相同，在Netty底层统一抽象封装也并不合适。

Netty提供了一些基类，我们可以很简单的编码器。同样的，编码器有下面两种类型：

MessageToByteEncoder，消息对象编码成字节码，将POJO对象编码成ByteBuf，用户的编码器继承Message ToByteEncoder，实现void encode(ChannelHandlerContext ctx, I msg, ByteBuf out)接口接口。

MessageToMessageEncoder，消息对象编码成消息对象，将一个POJO对象编码成另一个对象，以HTTP+XML协议为例，它的一种实现方式是：先将POJO对象编码成XML字符串，再将字符串编码为HTTP请求或者应答消息。对于复杂协议，往往需要经历多次编码，为了便于功能扩展，可以通过多个编码器组合来实现相关功能。用户的解码器继承MessageToMessageEncoder解码器，实现void encode(Channel HandlerContext ctx, I msg, List<Object>
 out)方法即可。注意，它与MessageToByteEncoder的区别是输出是对象列表而不是ByteBuf。

