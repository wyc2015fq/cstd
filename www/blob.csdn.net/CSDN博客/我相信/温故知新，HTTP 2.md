# 温故知新，HTTP/2 - 我相信...... - CSDN博客





2019年01月07日 22:22:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：200








去年年底，据国际互联网工程任务组（ IETF ）消息，HTTP-over-QUIC 实验性协议将被重命名为 HTTP/3，即有望成为 HTTP 协议的第三个正式版本，也就是说HTTP/3可能要来了。 该消息是如此的惹人注目，是因为HTTP是我们身边的协议，Web应用都离不开它。 

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrXhicM5FyIsnNvECK0eLianZIpL8MBgiclTVkH5vDsLdicu8cWSddddusXw/640?wx_fmt=png)

温故知新，梳理一下过往，或许更能够理解未来。

## HTTP1.x的过往

HTTP协议大约诞生在我上大一的时候，好像是HTTP0.9，客户端请求和服务器响应都是ascii码，客户端以回车符结尾，服务器返回HTML。后来的HTTP1.0，服务器响应增加了很多状态，请求和响应也多了很多的header，响应的内容也不再局限于纯文本了。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrfibqdrBP2RykwT3eYvaA0q5tCWq3A56hibupAsCgW9HdySdYoplATf3w/640?wx_fmt=jpeg)

HTTP是一个应用层协议，由请求和响应构成，是一个标准的客户端服务器模型，是一个无状态的协议。HTTP是建立在TCP之上的，每个请求都要经历三次握手和慢启动。客户端是依据域名来向服务器建立连接，一般PC端的浏览器支持同域6～8个连接，手机端的连接数则一般控制在4～6个。连接数不是越多越好，资源开销和整体延迟都会随之增大。

HTTP 1.1 导致了2000年的互联网热潮。HTTP1.1 支持只发送header信息(不带任何body信息)，如果服务器认为客户端有权限请求服务器，则返回100，否则返回401。客户端如果接受到100，才开始把请求body发送到服务器。这样当服务器返回401的时候，客户端就可以不用发送请求body了，节约了带宽。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrFeQvDpwDEpVCLVJEc154p5GQRoBNQRicOsicM7cFThl4Mf144UCuCicQA/640?wx_fmt=jpeg)

另外HTTP还支持传送内容的一部分。这样当客户端已经有一部分的资源后，只需要跟服务器请求另外的部分资源即可。RANGE:bytes是HTTP/1.1新增内容，HTTP/1.0每次传送文件都是从文件头开始，即0字节处开始。RANGE:bytes=XXX表示要求服务器从文件XXX字节处开始传送，这大概就是平时所说的断点续传。

相关的部分协议标准如下：
|协议编号|协议名称|简要描述|
|----|----|----|
| | | |
| | | |
| | | |
| | | |
| | | |
| | | |
RFC7230 HTTP/1.1: Message Syntax and Routing底层消息解析和连接管理等RFC7231HTTP/1.1: Semantics and Content方法、状态码和header等RFC7232HTTP/1.1: Conditional Requests例如If-Modified-SinceRFC7233HTTP/1.1: Range Requests获取部分内容等RFC7234HTTP/1.1: Caching浏览器和中介缓存等RFC7235HTTP/1.1: AuthenticationHTTP 的一个authentication框架等


现如今，Web应用不再单纯是web 网页，还有支持多设备和多媒体。 一个SPA的应用可能有上百的连接，模块拆分导致了更多的请求，大部分时间都消耗在网络上。HTTP 1.x header 往往较大，且无法压缩。TCP协议利用过低，不可复用连接，连接数限制且协议过于庞大。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrNw3jvPhibXOthkyQI9l04OvQFxz6PHuP1QKBKjcRnEzoVNFUSqzbGnA/640?wx_fmt=jpeg)

## HTTP1.x遇到的问题和解决方案

HTTP1.x主要存在连接无法复用和head of line blocking这两个问题。在第一个请求没有收到回复之前，后续从应用层发出的请求只能排队。网络通畅的时候性能影响不大，一旦第一个请求没有抵达服务器，或者response因为网络阻塞没有及时返回，就会影响所有后续请求。

HTTP1.0协议头里可以设置Connection:Keep-Alive。在header里设置Keep-Alive可以在一定时间内复用连接，具体复用时间的长短可以由服务器控制，一般在15秒左右，这与运营商蜂窝网络的linger time相关。HTTP1.1之后Connection的默认值就是Keep-Alive，如果要关闭连接复用需要显式的设置Connection:Close。这对PC端浏览器的体验帮助很大，因为大部分的请求在集中在一小段时间以内。但移动app的请求比较分散且时间跨度相对较大，一般会从应用层寻求其它解决方案，长连接方案或者伪长连接方案。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYribm1SXXRZS33OhIicHFjicnicbo9whVtAo6j392bwORrJHRLl2r4rTF0TA/640?wx_fmt=jpeg)

为了解决HTTP连接复用，可以采用长轮询，HTTP streaming和websocket等方式。

和传统的HTTP短链接相比，长连接轮询会在用户增长的时候极大的增加服务器压力。移动端网络环境复杂，像wifi和4g的网络切换等，这些场景都需要考虑重建连接。长轮询方式稳定性并不好，需要做好数据可靠性的保证，比如重发和ack机制。而且，response有可能会被中间代理cache住，要处理好业务数据的过期机制。

HTTP streaming是通过在server response的头部里增加"Transfer Encoding: chunked"来告诉客户端后续还会有新的数据。如果永远不会结束，客户端就会一直处于等待response的过程中。代理服务器会等待服务器的response结束之后才会将结果推送到请求客户端。对于streaming这种业务数据无法按照请求来做分割，所以客户端每收到一块数据都需要自己做协议解析。显然这个数据通道也是单向的，还有个缺陷就是不会产生重复的header数据。

websocket提供双向的数据通道，优势在于提供了message的概念，比基于字节流的tcp socket使用更简单，同时又提供了传统的HTTP所缺少的长连接功能。但代价相对较高，基于tcp的socket编程技术难度相对复杂很多，而且需要自己制定协议。

## HTTP/2 要点

HTTP2.0是以SPDY为原型进行讨论和标准化的，采用二进制格式传输数据，而非 HTTP/1.x 的文本格式。请求和响应都统一为流，对消息头采用 HPACK 进行压缩传输，能够节省消息头占用的网络的流量。多路复用，就是所有的请求都是通过一个 TCP 连接并发完成，并支持Server Push和基于优先级的流量控制。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYr90YGu9bt5lL4ia2norib2UDibs5uvDG2owr349RSMicEaXxicxHVrLaIBVQ/640?wx_fmt=png)

### HTTP/2 中的帧

帧（frame）是HTTP2中最小的通信单位，每个帧都会有帧header，每个帧用来承载HTTP header 或负荷数据，或其他特定类型的帧。帧是遵循二进制编码的。帧格式如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrqncqsfccialXpjhwM2Y8Z07X0cdtFBJnKqFS3cPWmd4RE4yzT1YoI6A/640?wx_fmt=png)

length定义了整个帧的长度，type定义帧主要有10种的类型：
|帧类型|code|
|----|----|
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
DATA0x0HEADERS0x1PRIORITY0x2RST*STREAM*0x3PUSHPROMISE0x4SETTINGS0x5PING0x6GOAWAY0x7WINDOW_UPDATE0x8CONTINUATION0x9


flags用位定义了一些重要的参数,stream id用作流控制，而payload才是请求的正文。

虽然协议的格式和HTTP1.x完全不同了，但并没有改变HTTP1.x的语义，只是把原来HTTP1.x的header和body部分用frame重新封装了一层而已。调试的时候浏览器甚至会把HTTP2.0的frame自动还原成HTTP1.x的格式。HTTP2.0与HTTP1.0的对比如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrmX7Mtet3dZyh3IoRYArT5ZsEvCdRhCXiboF6BOicffWJblsA2FFmLiakQ/640?wx_fmt=png)

### HTTP/2 中的header 压缩

HTTP1.x的header由于cookie和user agent很容易变得较大，而且每次都要重复发送。HTTP/2使用encoder来减少需要传输的header大小，通讯双方各自cache一份header fields表，既避免了重复header的传输，又减小了需要传输的大小。高效的压缩算法可以很大的压缩header，减少发送包的数量从而降低延迟。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrBmLyECNhuygl89iacm0pUnd9XllhQicxHUJyQK4ibCzTRvnVtBF0wD0dA/640?wx_fmt=png)

HTTP/2中的HPACK使用一份索引表来定义常用的 HTTP Header,保留原有的header list的顺序,通过索引键值压缩。 静态表中包含了一些预定义的header字段，动态表默认是空的，会在头部解压缩的时候确定是否添加entry。客户端和服务器端使用header表来跟踪和存储之前发送的每一个键值对。在tcp连接期间，二者共同维护和更新。对于无法用索引替代的字符，有的会采用哈夫曼编码压缩。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYreMpODUQtqu6lgtblu7TWtj8BFVh7x8ZP13N236XHyF8Na3BaBmp5dQ/640?wx_fmt=png)

### HTTP/2 中的多路复用

把HTTP 消息分解为独立的帧，交错发送，然后在另一端根据Stream ID 重新组装是HTTP 2.0 最重要的一项增强。每个 Frame Header 都有一个 Stream ID。每次请求/响应使用不同的 Stream ID。通过 Stream ID 标识，所有的请求和响应都可以同时跑在一个TCP 连接上了。 下图是 HTTP 和 spdy的并发模型对比：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrxrng3RwXyuywEatodmgR8R24FYFkMp779ZGjw98XcDIDfL9LjlyiagA/640?wx_fmt=png)

和一般TCP连接释放一样，如果客户端没有数据要请求，或服务端数据发送完毕后，会主动发送关闭连接的报文。或者是服务端连续发送探测报文，客户端无响应，服务端就关闭了这个连接。

当流并发时，就会涉及到流的优先级和依赖。优先级高的流会被优先发送。每个HTTP/2流里面可以带有优先级(31位，0为优先级最高)的值，这个值确定着客户端和服务器处理不同的流采取不同的优先级策略，高优先级的流都应该优先发送。图片请求的优先级要低于CSS和SCRIPT脚本，这可以确保重要的东西可以被优先加载。，但又不会绝对的，绝对地遵守可能又会引入队列阻塞的问题：高优先级的请求慢导致阻塞其他资源交付。

从tcp连接和网络来看，优先级使得网络拥塞得到改善，慢启动时间减少，拥塞和丢包恢复速度变快。

### HTTP/2 中的Push

Server Push 就是服务器向客户端推送资源而无需客户端明确地请求，或者服务器可以对一个客户端请求发送多个响应。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYratTs6hwNJDk6BvSBdw1g5fuTcryzuVMCSk3WlaibTkCia6T0wZuknfwA/640?wx_fmt=jpeg)

当服务端需要主动推送某个资源时，便会发送一个 Frame Type 为 PUSH_PROMISE 的 帧，里面带了 PUSH 需要新建的 Stream ID。客户端解析 帧时，发现它是一个 PUSH_PROMISE 类型，便会准备接收服务端要推送的流。

HTTP/2连接建立后，客户端与服务器交换SETTINGS 帧，以此来限定双向并发流的最大数量。因此，客户端可以限定推送流的数量，或者通过把这个值设置为0，完全禁用服务器推送，而且，所有推送的资源都遵守同源策略。服务器不能随便将第三方资源推送给客户端，而必须是经过双方确认才行。

所有服务器推送流都由PUSH_PROMISE 发起，PUSH_PROMISE 帧必须在返回响应之前发送，以免客户端出现竞态条件。客户端接收到PUSH_PROMISE 帧之后，可以视自身需求选择拒绝这个流。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYrFJribFHtpgOvkzM5eDGswklJrC3HTYVcibIo9VtMUwsJWicrK0KibCQLiaA/640?wx_fmt=jpeg)

## 基于HTTP/2的开发

HTTP/2 已经得到了较为广泛的支持，服务器的支持包括：
- 
Apache HTTP Server 2.4.17+ 

- 
Apache Tomcat 8.5+

- 
NGINX 1.9.5+

- 
面向PHP的Swoole

- 
面向Python 的Twisted

- 
...


支持HTTP/2的客户端包括：
- 
Chromium 

- 
Mozilla Firefox

- 
curl and libcurl

- 
OkHTTP （java ，Android）

- 
面向Obj-C/swift 的 WKWebView

- 
...


客户端与服务器同时支持HTTP/2的包括：
- 
Jetty/Netty

- 
lua-HTTP

- 
Node.js 8.4.0+

- 
面向perl 的 Protocol::HTTP2

- 
面向Go 的HTTP2

- 
...


支持HTTP/2的代理中介包括：
- 
HAProxy

- 
ngHTTP2

- 
GFE

- 
...


详情可以参考HTTPs://github.com/HTTP2/HTTP2-spec/wiki/Implementations。

调试工具可以使用chrome的浏览器以及Wireshark等等。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczqCrRfuoMuenOEvmv5J8dYr35cibqy5SBMd7hY5M2paibicFMR2zXx5LWz1TImWzFeoKWTfVaNnbKm6Q/640?wx_fmt=jpeg)

在开发中使用了HTTP/2 并不是万事大吉了，在HTTP1.X 中的一些优化还需要继续使用，例如减少DNS查询和重定向，CDN的使用，对代码、图片等资源的压缩，对文本开启GZip，以及使用HTTP的缓存机制（Expires/Cache-Control和Last-Modified / ETag）等等。对于那些可以感知缓存的资源内联或者Push 消息，可以利用cookie 协助用户标记。

由于HTTP/2基于单个TCP连接，容易受到 Head of Line Blocking 的影响，从而导致传输速度受限，还会受到TCP丢包的影响，所以HTTP/2在资源数量较少的网站可能效果不明显。TCP协议的升级依赖于操作系统内核的升级，尤其是网络操作系统的升级往往不可控，因此业界开始重新审视UDP， HTTP/3 所使用的QUIC 就是基于UDP协议的。

HTTP/3 何时才能实施呢？整个互联网支持HTTP/3 可能还需要一段不短的时间吧！

参考资料：
- 
《HTTP 权威指南》

- 
HTTPs://HTTP2.akamai.com/demo

- 
HTTP://HTTP2.loadimpact.com/entry/

- 
RFC 7540 - Hypertext Transfer Protocol Version 2 (HTTP/2) 

- 
RFC 7541 - HPACK: Header Compression for HTTP/2






