# 直播推流实现RTMP协议的一些注意事项 - DoubleLi - 博客园







—— 2017-2-12 更新
RTMP 协议整理了一下，包括rtmp 消息类型，rtmp 如何分块，rtmp分块例子。 用脑图整理了一下，使用[Xmind](http://www.xmindchina.net/) 打开，URL： [https://github.com/gezhaoyou/RtmpMindmap](https://github.com/gezhaoyou/RtmpMindmap)
- rtmp 消息类型 

![](https://upload-images.jianshu.io/upload_images/1111194-28eba38a83dc80f3.png?imageMogr2/auto-orient/strip%7CimageView2/2)

Paste_Image.png

- rtmp 消息分块

![](https://upload-images.jianshu.io/upload_images/1111194-0042caecfcef441f.png?imageMogr2/auto-orient/strip%7CimageView2/2)

Paste_Image.png


### 总体介绍

前一段时间写过一篇文章: [iOS直播视频数据采集、硬编码保存h264文件](http://www.jianshu.com/p/337830891996)，比较详细的记录了在做iOS端进行视频数据采集和编码的过程，下一步要做的就是RTMP协议推流。因为在公司将RTMP协议用Java 和 Swift 分别实现了一遍，所以对这块比较了解，中间遇到了不少坑，记录下来也怕自己忘掉。
RTMP协议是 Adobe 公司开发的一个基于TCP的应用层协议，Adobe 公司也公布了关于RTMP的规范，但是这个协议规范介绍的有些地方非常模糊，很多东西和实际应用是有差别的。网上也有不少关于这个协议的介绍，但都不是太详细。我遇到的比较好的参考资料就是这篇：[带你吃透RTMP](http://mingyangshang.github.io/2016/03/06/RTMP%E5%8D%8F%E8%AE%AE/), 这篇文章只是在理论上对RTMP进行了比较详细的解释，很多东西还是和实际应用有出入。我这篇文章只是把遇到的一些坑记录下来，并不是详解RTMP消息的。
另外懂RTMP消息拆包分包，而不真正的写写的话是很难把RTMP协议弄得的很清楚，关于RTMP协议的实现也是比较麻烦的事，懂和做事两回事。
另外用wireshark 抓一下包的话可以非常直观的看到RTMP通信的过程，对理解RTMP非常有帮助，在调试代码的时候也大量借助wireshark排错，是一个非常有用的工具。

### 1. RTMP 握手

RTMP 握手分为简单握手和复杂握手，现在Adobe公司使用RTMP协议的产品应该用的都是复杂握手，这里不介绍，只说简单握手。 按照网上的说法RTMP握手的过程如下 

> - 握手开始于客户端发送C0、C1块。服务器收到C0或C1后发送S0和S1。  
- 当客户端收齐S0和S1后，开始发送C2。当服务器收齐C0和C1后，开始发送S2。  
- 当客户端和服务器分别收到S2和C2后，握手完成。  


在实际工程应用中，一般是客户端先将`C0`, `C1`块同时发出，服务器在收到`C1` 之后同时将`S0`, `S1`, `S2`发给客户端。S2的内容就是收到的C1块的内容。之后客户端收到S1块，并原样返回给服务器，简单握手完成。按照RTMP协议个要求，客户端需要校验C1块的内容和S2块的内容是否相同，相同的话才彻底完成握手过程，实际编写程序用一般都不去做校验。
RTMP握手的这个过程就是完成了两件事：1. 校验客户端和服务器端RTMP协议版本号，2. 是发了一堆数据，猜想应该是测试一下网络状况，看看有没有传错或者不能传的情况。RTMP握手是整个RTMP协议中最容易实现的一步，接下来才是大头。  

### 2. RTMP 分块

创建RTMP连接算是比较难的地方，开始涉及消息分块（chunking）和 AFM（也是Adobe家的东西）格式数据的一些东西，在上面提到的文章中也有介绍为什要进行RTMP分块。

#### Chunk Size

RTMP是按照chunk size进行分块，chunk size指的是 chunk的payload部分的大小，不包括chunk basic header 和 chunk message header，即chunk的body的大小。客户端和服务器端各自维护了两个chunk size, 分别是自身分块的chunk size 和 对端 的chunk size, 默认的这两个chunk size都是128字节。通过向对端发送`set chunk size` 消息告知对方更改了 chunk size的大小，即告诉对端：我接下来要以xxx个字节拆分RTMP消息，你在接收到消息的时候就按照新的chunk size 来组包。
在实际写代码的时候一般会把chunk size设置的很大，有的会设置为4096，FFMPEG推流的时候设置的是 60*1000，这样设置的好处是避免了频繁的拆包组包，占用过多的CPU。设置太大的话也不好，一个很大的包如果发错了，或者丢失了，播放端就会出现长时间的花屏或者黑屏等现象。  

#### Chunk Type

RTMP 分成的Chunk有4中类型，可以通过 chunk basic header的 高两位指定，一般在拆包的时候会把一个RTMP消息拆成以 Type_0 类型开始的chunk，之后的包拆成 Type_3 类型的chunk，我查看了有不少代码也是这样实现的，这样也是最简单的实现。
RTMP 中关于Message 分chunk只举了两个例子，这两个例子不是很具有代表性。假如第二个message和第一个message的message stream ID 相同，并且第二个message的长度也大于了chunk size，那么该如何拆包？当时查了很多资料，都没有介绍。后来看了一些源码，发现第二个message可以拆成Type_1类型一个chunk, message剩余的部分拆成Type_3类型的chunk。FFMPEG中好像就是这么做的。  

### 3. RTMP 消息

关于推流的过程，RTMP的协议文档上给了一个示例，而真实的RTMP通信过程和它有较大的差异，只说推流，RTMP播放端我没有做过。  

#### Connect消息

握手之后先发送一个connect 命令消息，命令里面包含什么东西，协议中没有说，真实通信中要指定一些编解码的信息，这些信息是以AMF格式发送的, 下面是用swift 写的connect命令包含的参数信息：  

```
transactionID += 1 // 0x01
        let command:RTMPCommandMessage = RTMPCommandMessage(commandName: "connect", transactionId: transactionID, messageStreamId: 0x00)
        let objects:Amf0Object = Amf0Object()
        objects.setProperties("app", value: rtmpSocket.appName)
        objects.setProperties("flashVer",value: "FMLE/3.0 (compatible; FMSc/1.0)")
        objects.setProperties("swfUrl", value:"")
        objects.setProperties("tcUrl", value: "rtmp://" + rtmpSocket.hostname + "/" + rtmpSocket.appName)
        objects.setProperties("fpad", value: false)
        objects.setProperties("capabilities", value:239)
        objects.setProperties("audioCodecs", value:3575)
        objects.setProperties("videoCodecs", value:252)
        objects.setProperties("videoFunction",value: 1)
        objects.setProperties("pageUrl",value: "")
        objects.setProperties("objectEncoding",value: 0)
```

这些信息具体什么意思我也不太明白，协议中也没有，都是我在看librtmp，srs-librtmp这些源码，以及用wireshark 抓包的时候看到的。其中参数少一两个貌似也没问题，但是`audioCodecs`和`videoCodecs`这两个指定音视频编码信息的不能少。
服务器返回的是一个_result命令类型消息，这个消息的payload length一般不会大于128字节，但是在最新的nginx-rtmp中返回的消息长度会大于128字节，所以一定要做好收包，组包的工作。
关于消息的transactionID是用来标识command类型的消息的，服务器返回的_result消息可以通过 transactionID来区分是对哪个命令的回应，connect 命令发完之后还要发送其他命令消息，要保证他们的transactionID不相同。
发送完connect命令之后一般会发一个 set chunk size消息来设置chunk size 的大小，也可以不发。
Window Acknowledgement Size 是设置接收端消息窗口大小，一般是2500000字节，即告诉客户端你在收到我设置的窗口大小的这么多数据之后给我返回一个ACK消息，告诉我你收到了这么多消息。在实际做推流的时候推流端要接收很少的服务器数据，远远到达不了窗口大小，所以基本不用考虑这点。而对于服务器返回的ACK消息一般也不做处理，我们默认服务器都已经收到了这么多消息。
之后要等待服务器对于connect的回应的，一般是把服务器返回的chunk都读完组成完整的RTMP消息，没有错误就可以进行下一步了。

#### Create Stream 消息

 创建完RTMP连接之后就可以创建RTMP流，客户端要想服务器发送一个`releaseStream`命令消息，之后是`FCPublish`命令消息，在之后是`createStream`命令消息。当发送完`createStream`消息之后，解析服务器返回的消息会得到一个`stream ID`, 这个ID也就是以后和服务器通信的 `message stream ID`, 一般返回的是1，不固定。  

#### Publish Stream

推流准备工作的最后一步是 Publish Stream，即向服务器发一个`publish`命令，这个命令的message stream ID 就是上面 create stream 之后服务器返回的stream ID，发完这个命令一般不用等待服务器返回的回应，直接下一步发送音视频数据。有些rtmp库 还会发`setMetaData`消息，这个消息可以发也可以不发，里面包含了一些音视频编码的信息。  

### 4. 发布音视频

当以上工作都完成的时候，就可以发送音视频了。音视频RTMP消息的Payload中都放的是按照FLV-TAG格式封的音视频包，具体可以参照FLV协议文档。  

### 5. 关于RTMP的时间戳

RTMP的时间戳在发送音视频之前都为零，开始发送音视频消息的时候只要保证时间戳是单增的基本就可以正常播放音视频。我读Srs-librtmp的源码，发现他们是用h264的dts作为时间戳的。我在用java写的时候是先获取了下当前系统时间，然后每次发送消息的时候都与这个起始时间相减，得到时间戳。

### 6. 关于Chunk Stream ID

RTMP 的Chunk Steam ID是用来区分某一个chunk是属于哪一个message的 ,0和1是保留的。每次在发送一个不同类型的RTMP消息时都要有不用的chunk stream ID, 如上一个Message 是command类型的，之后要发送视频类型的消息，视频消息的chunk stream ID 要保证和上面 command类型的消息不同。每一种消息类型的起始chunk 的类型必须是 Type_0 类型的，表明我是一个新的消息的起始。

另外这篇文章有些地方还是说的模糊，以后有时间慢慢丰富吧。




from：http://www.jianshu.com/p/00aceabce944











