# 刨根问底 HTTP 和 WebSocket 协议（下） - 文章 - 伯乐在线
原文出处： [TheAlchemist](http://www.jianshu.com/p/f666da1b1835)
![1966024-9a18af90d272914d](http://jbcdn2.b0.upaiyun.com/2016/09/4bd04981685859ce29f8e5c169a080d6.png)
*HTML5的新成员：WebSocket*
[上篇介绍了HTTP1.1协议的基本内容](http://blog.jobbole.com/105953/)，这篇文章将继续分析WebSocket协议，然后对这两个进行简单的比较。
## WebSocket
WebSocket协议还很年轻，RFC文档相比HTTP的发布时间也很短，它的诞生是为了创建一种「双向通信」的协议，来作为HTTP协议的一个替代者。那么首先看一下它和HTTP（或者HTTP的长连接）的区别。
## 为什么要用 WebSocket 来替代 HTTP
上一篇中提到WebSocket的目的就是解决网络传输中的双向通信的问题，HTTP1.1默认使用持久连接（persistent connection），在一个TCP连接上也可以传输多个Request/Response消息对，但是HTTP的基本模型还是一个Request对应一个Response。这在双向通信（客户端要向服务器传送数据，同时服务器也需要实时的向客户端传送信息，一个聊天系统就是典型的双向通信）时一般会使用这样几种解决方案：
- 轮询（polling），轮询就会造成对网络和通信双方的资源的浪费，且非实时。
- 长轮询，客户端发送一个超时时间很长的Request，服务器hold住这个连接，在有新数据到达时返回Response，相比#1，占用的网络带宽少了，其他类似。
- 长连接，其实有些人对长连接的概念是模糊不清的，我这里讲的其实是HTTP的长连接（1）。如果你使用Socket来建立TCP的长连接（2），那么，这个长连接（2）跟我们这里要讨论的WebSocket是一样的，实际上TCP长连接就是WebSocket的基础，但是如果是HTTP的长连接，本质上还是Request/Response消息对，仍然会造成资源的浪费、实时性不强等问题。
![](http://ww3.sinaimg.cn/large/63918611gw1f81sxumx2lj20go0adjsk.jpg)
*HTTP的长连接模型*
## 协议基础
WebSocket的目的是取代HTTP在双向通信场景下的使用，而且它的实现方式有些也是基于HTTP的（WS的默认端口是80和443）。现有的网络环境（客户端、服务器、网络中间人、代理等）对HTTP都有很好的支持，所以这样做可以充分利用现有的HTTP的基础设施，有点向下兼容的意味。
简单来讲，WS协议有两部分组成：握手和数据传输。
## 握手（handshake）
出于兼容性的考虑，WS的握手使用HTTP来实现（此文档中提到未来有可能会使用专用的端口和方法来实现握手），客户端的握手消息就是一个「普通的，带有Upgrade头的，HTTP Request消息」。所以这一个小节到内容大部分都来自于RFC2616，这里只是它的一种应用形式，下面是RFC6455文档中给出的一个客户端握手消息示例：


```
GET /chat HTTP/1.1            //1
    Host: server.example.com   //2
    Upgrade: websocket            //3
    Connection: Upgrade            //4
    Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==            //5
    Origin: http://example.com            //6
    Sec-WebSocket-Protocol: chat, superchat            //7
    Sec-WebSocket-Version: 13            //8
```
可以看到，前两行跟HTTP的Request的起始行一模一样，而真正在WS的握手过程中起到作用的是下面几个header域。
Upgrade：upgrade是HTTP1.1中用于定义转换协议的header域。它表示，如果服务器支持的话，客户端希望使用现有的「网络层」已经建立好的这个「连接（此处是TCP连接）」，切换到另外一个「应用层」（此处是WebSocket）协议。
Connection：HTTP1.1中规定Upgrade只能应用在「直接连接」中，所以带有Upgrade头的HTTP1.1消息必须含有Connection头，因为Connection头的意义就是，任何接收到此消息的人（往往是代理服务器）都要在转发此消息之前处理掉Connection中指定的域（不转发Upgrade域）。
如果客户端和服务器之间是通过代理连接的，那么在发送这个握手消息之前首先要发送CONNECT消息来建立直接连接。
Sec-WebSocket-＊：第7行标识了客户端支持的子协议的列表（关于子协议会在下面介绍），第8行标识了客户端支持的WS协议的版本列表，第5行用来发送给服务器使用（服务器会使用此字段组装成另一个key值放在握手返回信息里发送客户端）。
Origin：作安全使用，防止跨站攻击，浏览器一般会使用这个来标识原始域。
如果服务器接受了这个请求，可能会发送如下这样的返回信息，这是一个标准的HTTP的Response消息。101表示服务器收到了客户端切换协议的请求，并且同意切换到此协议。RFC2616规定只有切换到的协议「比HTTP1.1更好」的时候才能同意切换。


```
HTTP/1.1 101 Switching Protocols //1
    Upgrade: websocket. //2
    Connection: Upgrade. //3
    Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=  //4
    Sec-WebSocket-Protocol: chat. //5
```
## WebSocket 协议 Uri
ws协议默认使用80端口，wss协议默认使用443端口。


```
ws-URI = "ws:" "//" host [ ":" port ] path [ "?" query ]
      wss-URI = "wss:" "//" host [ ":" port ] path [ "?" query ]
      host = <host, defined in [RFC3986], Section 3.2.2>
      port = <port, defined in [RFC3986], Section 3.2.3>
      path = <path-abempty, defined in [RFC3986], Section 3.3>
      query = <query, defined in [RFC3986], Section 3.4>
```
## 在客户端发送握手之前要做的一些小事
在握手之前，客户端首先要先建立连接，一个客户端对于一个相同的目标地址（通常是域名或者IP地址，不是资源地址）同一时刻只能有一个处于CONNECTING状态（就是正在建立连接）的连接。从建立连接到发送握手消息这个过程大致是这样的：
- 客户端检查输入的Uri是否合法。
- 客户端判断，如果当前已有指向此目标地址（IP地址）的连接（A）仍处于CONNECTING状态，需要等待这个连接（A）建立成功，或者建立失败之后才能继续建立新的连接。
PS：如果当前连接是处于代理的网络环境中，无法判断IP地址是否相同，则认为每一个Host地址为一个单独的目标地址，同时客户端应当限制同时处于CONNECTING状态的连接数。
PPS：这样可以防止一部分的DDOS攻击。
PPPS：客户端并不限制同时处于「已成功」状态的连接数，但是如果一个客户端「持有大量已成功状态的连接的」，服务器或许会拒绝此客户端请求的新连接。
- 如果客户端处于一个代理环境中，它首先要请求它的代理来建立一个到达目标地址的TCP连接。
例如，如果客户端处于代理环境中，它想要连接某目标地址的80端口，它可能要收现发送以下消息：


```
CONNECT example.com:80 HTTP/1.1
       Host: example.com
```
如果客户端没有处于代理环境中，它就要首先建立一个到达目标地址的直接的TCP连接。
4.如果上一步中的TCP连接建立失败，则此WebSocket连接失败。
5.如果协议是wss，则在上一步建立的TCP连接之上，使用TSL发送握手信息。如果失败，则此WebSocket连接失败；如果成功，则以后的所有数据都要通过此TSL通道进行发送。
## 对于客户端握手信息的一些小要求
- 握手必须是RFC2616中定义的Request消息
- 此Request消息的方法必须是GET，HTTP版本必须大于1.1 。
以下是某WS的Uri对应的Request消息：


```
ws://example.com/chat
 GET /chat HTTP/1.1
```
3.此Request消息中Request-URI部分（RFC2616中的概念）所定义的资型必须和WS协议的Uri中定义的资源相同。
4.此Request消息中必须含有Host头域，其内容必须和WS的Uri中定义的相同。
5.此Request消息必须包含Upgrade头域，其内容必须包含websocket关键字。
6.此Request消息必须包含Connection头域，其内容必须包含Upgrade指令。
7.此Request消息必须包含Sec-WebSocket-Key头域，其内容是一个Base64编码的16位随机字符。
8.如果客户端是浏览器，此Request消息必须包含Origin头域，其内容是参考RFC6454。
9.此Request消息必须包含Sec-WebSocket-Version头域，在此协议中定义的版本号是13。
10.此Request消息可能包含Sec-WebSocket-Protocol头域，其意义如上文中所述。
11.此Request消息可能包含Sec-WebSocket-Extensions头域，客户端和服务器可以使用此header来进行一些功能的扩展。
12.此Request消息可能包含任何合法的头域。如RFC2616中定义的那些。
## 在客户端接收到 Response 握手消息之后要做的一些事情
- 如果返回的返回码不是101，则按照RFC2616进行处理。如果是101，进行下一步，开始解析header域，所有header域的值不区分大小写。
- 判断是否含有Upgrade头，且内容包含websocket。
- 判断是否含有Connection头，且内容包含Upgrade
- 判断是否含有Sec-WebSocket-Accept头，其内容在下面介绍。
- 如果含有Sec-WebSocket-Extensions头，要判断是否之前的Request握手带有此内容，如果没有，则连接失败。
- 如果含有Sec-WebSocket-Protocol头，要判断是否之前的Request握手带有此协议，如果没有，则连接失败。
## 服务端的概念
服务端指的是所有参与处理WebSocket消息的基础设施，比如如果某服务器使用Nginx（A）来处理WebSocket，然后把处理后的消息传给响应的服务器（B），那么A和B都是这里要讨论的服务端的范畴。
## 接受了客户端的连接请求，服务端要做的一些事情
如果请求是HTTPS，则首先要使用TLS进行握手，如果失败，则关闭连接，如果成功，则之后的数据都通过此通道进行发送。
之后服务端可以进行一些客户端验证步骤（包括对客户端header域的验证），如果需要，则按照RFC2616来进行错误码的返回。
如果一切都成功，则返回成功的Response握手消息。
## 服务端发送的成功的 Response 握手
此握手消息是一个标准的HTTP Response消息，同时它包含了以下几个部分：
- 状态行（如上一篇RFC2616中所述）
- Upgrade头域，内容为websocket
- Connection头域，内容为Upgrade
- Sec-WebSocket-Accept头域，其内容的生成步骤：
a.首先将Sec-WebSocket-Key的内容加上字符串258EAFA5-E914-47DA-95CA-C5AB0DC85B11（一个UUID）。
b.将#1中生成的字符串进行SHA1编码。
c.将#2中生成的字符串进行Base64编码。
- Sec-WebSocket-Protocol头域（可选）
- Sec-WebSocket-Extensions头域（可选）
一旦这个握手发出去，服务端就认为此WebSocket连接已经建立成功，处于OPEN状态。它就可以开始发送数据了。
## WebSocket 的一些扩展
Sec-WebSocket-Version可以被通信双方用来支持更多的协议的扩展，RFC6455中定义的值为13，WebSocket的客户端和服务端可能回自定义更多的版本号来支持更多的功能。其使用方法如上文所述。
## 发送数据
WebSocket中所有发送的数据使用帧的形式发送。客户端发送的数据帧都要经过掩码处理，服务端发送的所有数据帧都不能经过掩码处理。否则对方需要发送关闭帧。
一个帧包含一个帧类型的标识码，一个负载长度，和负载。负载包括扩展内容和应用内容。
## 帧类型
帧类型是由一个4位长的叫Opcode的值表示，任何WebSocket的通信方收到一个位置的帧类型，都要以连接失败的方式断开此连接。
RFC6455中定义的帧类型如下所示：
1.Opcode == 0 继续
表示此帧是一个继续帧，需要拼接在上一个收到的帧之后，来组成一个完整的消息。由于这种解析特性，非控制帧的发送和接收必须是相同的顺序。
2.Opcode == 1 文本帧
3.Opcode == 2 二进制帧
4.Opcode == 3 – 7 未来使用（非控制帧）
5.Opcode == 8 关闭连接（控制帧）
此帧可能会包含内容，以表示关闭连接的原因。
通信的某一方发送此帧来关闭WebSocket连接，收到此帧的一方如果之前没有发送此帧，则需要发送一个同样的关闭帧以确认关闭。如果双方同时发送此帧，则双方都需要发送回应的关闭帧。
理想情况服务端在确认WebSocket连接关闭后，关闭相应的TCP连接，而客户端需要等待服务端关闭此TCP连接，但客户端在某些情况下也可以关闭TCP连接。
6.Opcode == 9 Ping
类似于心跳，一方收到Ping，应当立即发送Pong作为响应。
7.Opcode == 10 Pong
如果通信一方并没有发送Ping，但是收到了Pong，并不要求它返回任何信息。Pong帧的内容应当和收到的Ping相同。可能会出现一方收到很多的Ping，但是只需要响应最近的那一次就可以了。
8.Opcode == 11 – 15 未来使用（控制帧）
## 帧的格式
具体的每一项代表什么意思在这里就不做详细的阐述了。


```
0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-------+-+-------------+-------------------------------+
 |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
 |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
 |N|V|V|V|       |S|             |   (if payload len==126/127)   |
 | |1|2|3|       |K|             |                               |
 +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
 |     Extended payload length continued, if payload len == 127  |
 + - - - - - - - - - - - - - - - +-------------------------------+
 |                               |Masking-key, if MASK set to 1  |
 +-------------------------------+-------------------------------+
 | Masking-key (continued)       |          Payload Data         |
 +-------------------------------- - - - - - - - - - - - - - - - +
 :                     Payload Data continued ...                :
 + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
 |                     Payload Data continued ...                |
 +---------------------------------------------------------------+
```
## 与HTTP比较
同样作为应用层的协议，WebSocket在现代的软件开发中被越来越多的实践，和HTTP有很多相似的地方，这里将它们简单的做一个纯个人、非权威的比较：
相同点
- 都是基于TCP的应用层协议。
- 都使用Request/Response模型进行连接的建立。
- 在连接的建立过程中对错误的处理方式相同，在这个阶段WS可能返回和HTTP相同的返回码。
- 都可以在网络中传输数据。
## 不同点
- WS使用HTTP来建立连接，但是定义了一系列新的header域，这些域在HTTP中并不会使用。
- WS的连接不能通过中间人来转发，它必须是一个直接连接。
- WS连接建立之后，通信双方都可以在任何时刻向另一方发送数据。
- WS连接建立之后，数据的传输使用帧来传递，不再需要Request消息。
- WS的数据帧有序。
## 待续
这一篇简单地将WebSocket协议介绍了一遍，篇幅有点长了，数据帧也没有来得及详述。下篇会继续深扒WebSocket帧传输，另外将通过实例探讨一些WebSocket协议实际使用中的问题。
