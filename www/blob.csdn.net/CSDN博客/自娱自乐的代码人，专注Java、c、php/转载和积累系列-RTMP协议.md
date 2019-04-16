# 转载和积累系列 - RTMP协议 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年12月10日 15:53:13[initphp](https://me.csdn.net/initphp)阅读数：1353








### 介绍

RTMP协议是被Flash用于对象,视频,音频的传输.该协议建立在TCP协议或者轮询HTTP协议之上。RTMP协议就像一个用来装数据包的容器,这些数据可以是AMF格式的数据,也可以是FLV中的视/音频数据。一个单一的连接可以通过不同的通道传输多路网络流.这些通道中的包都是按照固定大小的包传输的。




### 网络连接

一个Actionscript连接并播放一个流的简单代码:



```
var videoInstance:Video = your_video_instance;
var nc:NetConnection = new NetConnection();
var connected:Boolean = nc.connect("rtmp:/localhost/myapp");
var ns:NetStream = new NetStream(nc);
videoInstance.attachVideo(ns);
ns.play("flvName");
```


默认端口为1935






### 握手

Client → Server :向服务器发出握手请求.这不属于协议包一部分,该握手请求第一个字节为(0×03),其后跟着1536个字节.经管看上去这部分的内容对于RTMP协议来说并不是至关重要的,但也不可随意对待.

Server → Client :服务器向客户端回应握手请求.这部分的数据仍然不属于RTMP协议的部分.该回应的其实字节仍然为(0x03),但是后边跟着个长度为1536个字节(一共为3072 )的包块.第一个1536块看上去似乎可以是任意内容,甚至好像可以是Null都没有关系.第二个1536的代码块,是上一步客户端向服务器端发送的握手请求的内容.

Client→Server:把上一步服务器向客户端回应的第二块1536个字节的数据块.


至此客户端与服务器端的握手结束,下面将发送RTMP协议的包内容.


Client → Server :向服务器发送连接包.

Server → Client :服务器回应.

... .... 等等... ...




### RTMP数据类型
|0×01|Chunk Size|changes the chunk size for packets|
|----|----|----|
|0×02|Unknown|anyone know this one?|
|0×03|Bytes Read|send every x bytes read by both sides|
|0×04|Ping|ping is a stream control message, has subtypes|
|0×05|Server BW|the servers downstream bw|
|0×06|Client BW|the clients upstream bw|
|0×07|Unknown|anyone know this one?|
|0×08|Audio Data|packet containing audio|
|0×09|Video Data|packet containing video data|
|0x0A - 0×11|Unknown|anyone know?|
|0×12|Notify|an invoke which does not expect a reply|
|0×13|Shared Object|has subtypes|
|0×14|Invoke|like remoting call, used for stream actions too.|

### Shared Object 数据类型


|0×01|Connect|
|----|----|
|0×02|Disconnect|
|0×03|Set Attribute|
|0×04|Update Data|
|0×05|Update Attribute|
|0×06|Send Message|
|0×07|Status|
|0×08|Clear Data|
|0×09|Delete Data|
|0x0A|Delete Attribute|
|0x0B|Initial Data|




### 包结构

RTMP包 包含一个固定长度的包头和一个最长为128字节的包体.包头可以是下面4种长度的任意一种:12, 8, 4, or 1 byte(s).

第一个字节的前两个Bit很重要,它决定了包头的长度.它可以用掩码0xC0进行"与"计算.下面的表格罗列了可能的包头长度:


|Bits|Header Length|
|----|----|
|00|12 bytes|
|01|8 bytes|
|10|4 bytes|
|11|1 byte|

我们在这里讨论关RTMP包结构的问题并不是非常的详细.我们在以后有时间会讨论关于AMF的问题(敬请期待...),其实RTMP包结构就是使用了AMF格式.

**流**


过下面可以列一个关于客户端向服务器端发送流的流程:


Client→Server :发送一个创建流的请求.

Server→Client :返回一个表示流的索引号.

Client→Server :开始发送.

Client→Server :发送视音频数据包(这些包在同一个频道(channel)并用流的索引号来唯一标识).




### RTMP规范分析



RTMP协议是一个互联网TCP/IP五层体系结构中应用层的协议。RTMP协议中基本的数据单元称为消息（Message）。当RTMP协议在互联网中传输数据的时候，消息会被拆分成更小的单元，称为消息块（Chunk）。

1 消息

消息是RTMP协议中基本的数据单元。不同种类的消息包含不同的Message Type ID，代表不同的功能。RTMP协议中一共规定了十多种消息类型，分别发挥着不同的作用。例如，Message Type ID在1-7的消息用于协议控制，这些消息一般是RTMP协议自身管理要使用的消息，用户一般情况下无需操作其中的数据。Message Type ID为8，9的消息分别用于传输音频和视频数据。Message Type ID为15-20的消息用于发送AMF编码的命令，负责用户与服务器之间的交互，比如播放，暂停等等。消息首部（Message
 Header）有四部分组成：标志消息类型的Message Type ID，标志消息长度的Payload Length，标识时间戳的Timestamp，标识消息所属媒体流的Stream ID。消息的报文结构如图3所示。
![](https://img-blog.csdn.net/20141210155159566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2 消息块

在网络上传输数据时，消息需要被拆分成较小的数据块，才适合在相应的网络环境上传输。RTMP协议中规定，消息在网络上传输时被拆分成消息块（Chunk）。消息块首部（Chunk Header）有三部分组成：用于标识本块的Chunk Basic Header，用于标识本块负载所属消息的Chunk Message Header，以及当时间戳溢出时才出现的Extended Timestamp。消息块的报文结构如图4所示。
![](https://img-blog.csdn.net/20141210155225026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3 消息分块

在消息被分割成几个消息块的过程中，消息负载部分（Message Body）被分割成大小固定的数据块（默认是128字节，最后一个数据块可以小于该固定长度），并在其首部加上消息块首部（Chunk Header），就组成了相应的消息块。消息分块过程如图5所示，一个大小为307字节的消息被分割成128字节的消息块（除了最后一个）。
![](https://img-blog.csdn.net/20141210155251328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



RTMP传输媒体数据的过程中，发送端首先把媒体数据封装成消息，然后把消息分割成消息块，最后将分割后的消息块通过TCP协议发送出去。接收端在通过TCP协议收到数据后，首先把消息块重新组合成消息，然后通过对消息进行解封装处理就可以恢复出媒体数据。




