# TCP-IP协议，HTTP协议等相关概念的学习





## 转载：<https://blog.csdn.net/chenliguan/article/details/79146124>

## 1 详解TCP

### 1.1 TCP是什么？

　　TCP是Tranfer Control Protocol的简称，**TCP协议是一种面向连接的、可靠的、基于字节流的运输层通信协议。**通过TCP协议传输，得到的是一个顺序的无差错的数据流。发送方和接收方的成对的两个socket之间必须建立连接，以便在TCP协议的基础上进行通信，当一个socket（通常都是server socket）等待建立连接时，另一个socket可以要求进行连接，一旦这两个socket连接起来，它们就可以进行双向数据传输，双方都可以进行发送或接收操作。 
　　首先来看看OSI的七层模型： 
![这里写图片描述](https://img-blog.csdn.net/20180123232732740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180508103540624?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NoZW5saWd1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　在OSI七层模型中，每一层的作用和对应的协议如下： 
![这里写图片描述](https://img-blog.csdn.net/20180123232826778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
　　要进行更深层次的剖析，就需要了解并熟记TCP协议的数据格式和每个字段的含义： 
![这里写图片描述](https://img-blog.csdn.net/20180123233136881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- Source Port和Destination Port:分别占用16位，表示源端口号和目的端口号；用于区别主机中的不同进程，而IP地址是用来区分不同的主机的，源端口号和目的端口号配合上IP首部中的源IP地址和目的IP地址就能唯一的确定一个TCP连接；

- **Sequence Number**:用来标识从TCP发端向TCP收端发送的数据字节流，它表示在这个报文段中的的第一个数据字节在数据流中的**序号**；主要用来**解决网络报乱序**的问题；

- **Acknowledgment Number**:32位**确认序列号**包含发送确认的一端所期望收到的下一个序号，因此，确认序号应当是上次已成功收到数据字节序号加1。不过，只有当标志位中的ACK标志（下面介绍）为1时该确认序列号的字段才有效。主要用来**解决不丢包**的问题；

- Offset:给出首部中32 bit字的数目，需要这个值是因为任选字段的长度是可变的。这个字段占4bit（最多能表示15个32bit的的字，即4*15=60个字节的首部长度），因此TCP最多有60字节的首部。然而，没有任选字段，正常的长度是20字节；

- TCP Flags:TCP首部中有6个标志比特，它们中的多个可同时被设置为1，主要是用于操控TCP的状态机的，依次为URG，ACK，PSH，RST，SYN，FIN。每个标志位的意思如下： 

  URG：此标志表示TCP包的紧急指针域（后面马上就要说到）有效，用来保证TCP连接不被中断，并且督促中间层设备要尽快处理这些数据； 

  - **ACK**：此标志**表示应答域有效**，就是说前面所说的TCP应答号将会包含在TCP数据包中；有两个取值：0和1，为1的时候表示应答域有效，反之为0；
  - PSH：这个标志位表示Push操作。所谓Push操作就是指在数据包到达接收端以后，立即传送给应用程序，而不是在缓冲区中排队；
  - RST：这个标志表示连接复位请求。用来复位那些产生错误的连接，也被用来拒绝错误和非法的数据包；
  - **SY\**N：**表示同步序号，用来建立连接**。SYN标志位和ACK标志位搭配使用，当连接请求的时候，SYN=1，ACK=0；连接被响应的时候，SYN=1，ACK=1；这个标志的数据包经常被用来进行端口扫描。扫描者发送一个只有SYN的数据包，如果对方主机响应了一个数据包回来 ，就表明这台主机存在这个端口；但是由于这种扫描方式只是进行TCP三次握手的第一次握手，因此这种扫描的成功表示被扫描的机器不很安全，一台安全的主机将会强制要求一个连接严格的进行TCP的三次握手；
  - **FIN**： 表示发送端已经达到数据末尾，也就是说**双方的数据传送完成，没有数据可以传送了**，发送FIN标志位的TCP数据包后，连接将被断开。这个标志的数据包也经常被用于进行端口扫描。

- Window:窗口大小，也就是有名的滑动窗口，用来进行流量控制；这是一个复杂的问题，这篇博文中并不会进行总结的；

### 1.2 应用

　　当对网络通讯质量有要求的时候，比如：整个数据要准确无误的传递给对方，这往往用于一些要求可靠的应用，比如HTTP、HTTPS、FTP等传输文件的协议，POP、SMTP等邮件传输的协议。在日常生活中，常见使用TCP协议的应用如下：浏览器，用的HTTPFlashFXP，用的FTPOutlook，用的POP、SMTPPutty，用的Telnet、SSHQQ文件传输.

### 1.3 三次握手是什么？

　　TCP是面向连接的，无论哪一方向另一方发送数据之前，都必须先在双方之间建立一条连接。在TCP/IP协议中，TCP协议提供可靠的连接服务，连接是通过三次握手进行初始化的。三次握手的目的是同步连接双方的序列号和确认号并交换 TCP窗口大小信息。 
　　（1）第一次握手：Client将标志位SYN置为1，随机产生一个值seq=x，并将该数据包发送给Server，Client进入SYN_SENT状态，等待Server确认。 
　　 （2）第二次握手：Server收到数据包后由标志位SYN=1知道Client请求建立连接，Server将标志位SYN和ACK都置为1，ack (number )=x+1，随机产生一个值seq=y，并将该数据包发送给Client以确认连接请求，Server进入SYN_RCVD状态。 
　　（3）第三次握手：Client收到确认后，检查ACK是否为Client发送的seq+1，即x+1；如果正确则将标志位ACK置为1，ack=y+1，并将该数据包发送给Server。Server检查ack是否为Server发送的seq+1，即y+1，如果正确则连接建立成功，Client和Server进入ESTABLISHED状态，完成三次握手。 
![这里写图片描述](https://img-blog.csdn.net/2018030908473821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 1.4 为什么要三次握手?

　　概括：为了防止已失效的连接请求报文段突然又传送到了服务端，因而产生错误。 
　　具体：client发出的第一个连接请求报文段并没有丢失，而是在某个网络结点长时间的滞留了，以致延误到连接释放以后的某个时间才到达server，本来这是一个早已失效的报文段。但server收到此失效的连接请求报文段后，就误认为是client再次发出的一个新的连接请求，于是就向client发出确认报文段，同意建立连接。假设不采用“三次握手”，那么只要server发出确认，新的连接就建立了。由于现在client并没有发出建立连接的请求，因此不会理睬server的确认，也不会向server发送数据。但server却以为新的运输连接已经建立，并一直等待client发来数据。这样，server的很多资源就白白浪费掉了。采用“三次握手”的办法可以防止上述现象发生。例如刚才那种情况，client不会向server的确认发出确认；server由于收不到确认，就知道client并没有要求建立连接。

### 1.5 四次分手？

　　当客户端和服务器通过三次握手建立了TCP连接以后，当数据传送完毕，肯定是要断开TCP连接的啊。那对于TCP的断开连接，这里就有了神秘的“四次分手”。 
　　（1）第一次分手：客户端设置seq=x，向服务端发送一个FIN报文段；此时，主机1进入FIN_WAIT_1状态；这表示客户端没有数据要发送给服务端了； 
　　（2）第二次分手：服务端收到了客户端发送的FIN=1，向客户端回复ACK=1，ack=收到的seq+1=x+1，然后客户端进入FIN_WAIT_2状态；服务端告诉客户端，在等待自己去关闭连接； 
　　（3）第三次分手：服务端向客户端发送FIN=1，设置seq=y，请求关闭连接，同时服务端进入LAST_ACK状态； 
　　（4）第四次分手：客户端收到服务端发送的FIN，向服务端发送ACK=1，ack=收到的seq+1=y+1，然后主机1进入TIME_WAIT状态；服务端收到客户端的ACK报文段以后，就关闭连接；此时，客户端等待2MSL后依然没有收到回复，则证明Server端已正常关闭，客户端也关闭连接。 
![这里写图片描述](https://img-blog.csdn.net/20180308093232654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 1.6 为什么要四次分手?

　　TCP是全双工模式，当客户端发出FIN报文段时，表示客户端没有数据要发送了，告诉服务端它的数据已经全部发送完毕了，这时客户端还可以接受来自服务端的数据。当服务端返回ACK报文段时，表示它已经知道客户端没有数据发送了，但是服务端还是可以发送数据到客户端的。当服务端也发送了FIN报文段时，这时表示服务端也没有数据要发送了，就告诉客户端它也没有数据要发送了，就会中断这次TCP连接。 
　　如果要正确的理解四次分手的原理，就需要了解四次分手过程中的状态变化： 
　　（1）FIN_WAIT_1: 表示等待服务端的FIN报文。当Socket在ESTABLISHED状态时，客户端想主动关闭连接，向服务端发送了FIN报文，Socket进入到FIN_WAIT_1状态。（客户端） 
　　（2）CLOSE_WAIT：表示服务端在等待自己去关闭连接。当客户端关闭一个Socket后发送FIN报文给服务端，会回应一个ACK报文给客户端，Socket进入到CLOSE_WAIT状态。（服务端） 
　　（3）FIN_WAIT_2：当服务端回应ACK报文后，Socket进入到FIN_WAIT_2状态，表示半连接，即服务端要求close连接，还告诉客户端，我暂时还有数据需要传送给你(ACK信息)，稍后再关闭连接。（客户端） 
　　（4）LAST_ACK: 表示服务端在发送FIN报文后，最后等待客户端的ACK报文。（服务端） 
　　（5）TIME_WAIT: 表示收到了对方的FIN报文，并发送出了ACK报文，就等2MSL后即回到CLOSED状态。如果FIN_WAIT1状态下，收到了对方同时带FIN标志和ACK标志的报文时，可以直接进入到TIME_WAIT状态，而无须经过FIN_WAIT_2状态。（客户端） 
　　（6）CLOSED: 表示连接中断。当服务端收到ACK报文后，Socket进入到CLOSED状态。（服务端） 
[参考链接：为什么要三次握，要四次分手](http://blog.csdn.net/farmwang/article/details/64135224)

### 1.7 抓包分析三次握手和四次挥手

（1）抓包工具对比

- Wireshark使用WinPCAP作为接口，直接与网卡进行数据报文交换。所以它能显示信息。
- Fiddler其实是一个代理服务器，所有的服务请求会从其流过。所所以他能查看和操作一些待转发的数据。

（2）参考链接 
[wireshark抓包详细图文教程-具体观察三次握手和四次挥手](https://jingyan.baidu.com/article/c35dbcb0866b698916fcbc81.html)

## 2 详解UDP

### 2.1 UDP是什么？

　　UDP是User Datagram Protocol的简称，**是一种无连接的协议，每个数据报都是一个独立的信息，包括完整的源地址或目的地址，它在网络上以任何可能的路径传往目的地，因此能否到达目的地，到达目的地的时间以及内容的正确性都是不能被保证的。**

### 2.2 应用

　　当对网络通讯质量要求不高的时候，要求网络通讯速度能尽量的快，这时就可以使用UDP。比如，日常生活中，常见使用UDP协议的应用如下：QQ语音QQ视频TFTP。

### 2.3 TCP与UDP的区别

![这里写图片描述](https://img-blog.csdn.net/20180124092701093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 2.4 为什么UDP比TCP快

- TCP需要三次握手
- TCP有拥塞控制，控制流量等机制
- 对数据内容正确性的检验必然占用计算机的处理时间和网络的带宽，因此TCP传输的效率不如UDP高。

### 2.5 为什么TCP比UDP可靠

- TCP是面向有连接的，建立连接之后才发送数据；而UDP则不管对方存不存在都会发送数据。
- TCP有确认机制，接收端每收到一个正确包都会回应给发送端。超时或者数据包不完整的话发送端会重传。UDP没有，因此可能丢包。

## 3 详解HTTP

### 3.1 HTTP是什么？

　　HTTP协议即超文本传送协议(Hypertext Transfer Protocol )，是一个应用层协议，由请求和响应构成，是一个标准的客户端服务器模型，是Web联网的基础，也是手机联网常用的协议之一，HTTP协议是建立在TCP协议之上的一种应用。 
　　HTTP连接最显著的特点是：**客户端发送的每次请求都需要服务器回送响应，在请求结束后，会主动释放连接。从建立连接到关闭连接的过程称为“一次连接”**。

### 3.2 HTTP协议如何使用TCP连接

[模拟TCP/HTTP关系的就是：电话/人，先连接，后通信的模式。](https://www.zhihu.com/question/51996213)

### 3.3 HTTP请求报文结构

（1）HTTP请求报文格式 
![这里写图片描述](https://img-blog.csdn.net/20180125002005399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
（2）HTTP请求报文内容详解 
![这里写图片描述](https://img-blog.csdn.net/20180318112316719?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2NoZW5saWd1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- 请求行：由3部分组成，分别为：请求方法、URL（见备注1）以及协议版本，之间由空格分隔。 
  - 请求方法，GET和POST是最常见的HTTP方法，除此以外还包括DELETE、HEAD、OPTIONS、PUT、TRACE。
  - 为请求对应的URL地址，它和报文头的Host属性组成完整的请求URL，
  - 协议名称及版本号，协议版本的格式为：HTTP/主版本号.次版本号，常用的有HTTP/1.0和HTTP/1.1。
- 请求头部，报文头包含若干个属性，格式为“属性名:属性值”，服务端据此获取客户端的信息。 请求头部的最后会有一个空行，表示请求头部结束，接下来为请求正文。 
  ![这里写图片描述](https://img-blog.csdn.net/20180318111521285?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2NoZW5saWd1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 请求正文，可选部分，比如GET请求就没有请求正文。它将一个页面表单中的组件值通过param1=value1&param2=value2的键值对形式编码成一个格式化串，它承载多个请求参数的数据。

（3） 生活例子 
　　快到中午了，张三丰不想去食堂吃饭，于是打电话叫外卖：老板，我要一份[鱼香肉丝]，要12：30之前给我送过来哦，我在江湖湖公司研发部，叫张三丰。 
　　这里，你要[鱼香肉丝]相当于HTTP报文体，而“12：30之前送过来”，你叫“张三丰”等信息就相当于HTTP的报文头。它们是一些附属信息，帮忙你和饭店老板顺利完成这次交易。

### 3.4 HTTP响应报文结构

（1）HTTP响应报文格式 
![这里写图片描述](https://img-blog.csdn.net/20180125001946341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
（2）HTTP响应报文例子 
![这里写图片描述](https://img-blog.csdn.net/20180318112929730?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2NoZW5saWd1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
　　状态行：由3部分组成，分别为：协议版本，状态码，状态码描述。状态代码：为3位数字，200~299的状态码表示成功，300~399的状态码指资源重定向，400~499的状态码指客户端请求出错，500~599的状态码指服务端出错（HTTP/1.1向协议中引入了信息性状态码，范围为100~199），常见如下： 
![这里写图片描述](https://img-blog.csdn.net/20180125002615364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
　　响应头部，常见如下： 
![这里写图片描述](https://img-blog.csdn.net/20180125002751959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpZ3Vhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 3.5 HTTP中get和post的区别

（1） GET-从指定的服务器中获取数据。使用GET方法时，查询字符串（键值对）被附加在URL地址后面一起发送到服务器：/test/demo_form.jsp?name1=value1&name2=value2，特点：

- GET请求主要用以获取数据；
- GET执行效率略高。
- GET请求有长度限制，提交的数据最多只能是1024字节；
- GET请求能够被缓存、会保存在浏览器的浏览记录中、请求的URL能够保存为浏览器书签；

（2） POST-提交数据给指定的服务器。使用POST方法时，查询字符串在POST信息中单独存在，和HTTP请求一起发送到服务器，特点：

- POST请求主要用以提交数据给指定的服务器；
- POST执行效率低；
- POST请求没有长度限制；
- POST更加安全；
- POST请求不能被缓存下来、不会保存在浏览器浏览记录中、请求的URL无法保存为浏览器书签；

### 3.6 HTTP1.1与1.0的区别

　　在HTTP1.0版本，默认使用的是“短连接”（那时候是Web诞生初期，网页相对简单，“短连接”的问题不大）；到了1995年底开始制定 HTTP1.1草案的时候，网页已经开始变得复杂，这时候再用短连接的方式，效率太低下了，所以在HTTP1.1中，默认采用的是“Keep-Alive”的方式，即是“长连接”。在请求头部加入了这行代码：**Connection:keep-alive，即是“长连接”；TCP连接在发送后将仍然保持打开状态，于是浏览器可以继续通过相同的连接发送请求**。保持连接节省了为每个请求建立新连接所需的时间，还节约了网络带宽。

### 3.7 HTTP2.0与1.1的区别

　　HTTP2.0中新的**二进制分帧层将HTTP消息分解为互不依赖的帧，然后乱序发送，最后在另一端按照每个包重新组装，就实现了一个连接上有多个请求和响应，从而带来了巨大的性能提升**： 
![这里写图片描述](https://img-blog.csdn.net/20180318100027259?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2NoZW5saWd1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
（1）一个连接可以并行发起多个请求和响应； 
（2）并行交错的发送请求、发送响应，请求之间、响应之间户不影响； 
（3）消除不必要的延迟，从而减少页面加载时间。

## 4 详解Socket

### 4.1 套接字（socket）概念

　　套接字（socket）是对TCP/IP协议的封装，是通信的基石，是支持TCP/IP协议的网络通信的基本操作单元。Socket本身并不是协议，而是一个调用接口(API)，通过Socket，我们才能使用TCP/IP协议。它是网络通信过程中端点的抽象表示，包含进行网络通信必须的五种信息：**连接使用的协议，本地主机的IP地址，本地进程的协议端口，远地主机的IP地址，远地进程的协议端口。**

### 4.2 利用Socket建立网络连接的步骤

　　建立Socket连接至少需要一对套接字，其中一个运行于客户端，称为ClientSocket ；另一个运行于服务器端，称为ServerSocket 。套接字之间的连接过程分为三个步骤：服务器监听，客户端请求，连接确认。 
　　（1）**服务器监听**：服务器端套接字并不定位具体的客户端套接字，而是处于等待连接的状态，实时监控网络状态，等待客户端的连接请求。 
　　（2）**客户端请求**：指客户端的套接字提出连接请求，要连接的目标是服务器端的套接字。为此，客户端的套接字必须首先描述它要连接的服务器的套接字，指出服务器端套接字的地址和端口号，然后就向服务器端套接字提出连接请求。 
　　（3）**连接确认**：当服务器端套接字监听到或者说接收到客户端套接字的连接请求时，就响应客户端套接字的请求，建立一个新的线程，把服务器端套接字的描述发给客户端，一旦客户端确认了此描述，双方就正式建立连接。而服务器端套接字继续处于监听状态，继续接收其他客户端套接字的连接请求。

### 4.3 Socket连接与TCP连接

　　创建Socket连接时，可以指定使用的传输层协议，Socket可以支持不同的传输层协议（TCP或UDP），当使用TCP协议进行连接时，该Socket连接就是一个TCP连接。

### 4.4 Socket连接与HTTP连接

　　比喻说：HTTP是轿车，提供了封装或者显示数据的具体形式；Socket是发动机，提供了网络通信的能力。 
　　由于通常情况下**Socket连接就是TCP连接，因此Socket连接一旦建立，通信双方即可开始相互发送数据内容，直到双方连接断开**。但在实际网络应用中，客户端到服务器之间的通信往往需要穿越多个中间节点，例如路由器、网关、防火墙等，大部分防火墙默认会关闭长时间处于非活跃状态的连接而导致 Socket 连接断连，因此需要通过轮询告诉网络，该连接处于活跃状态。 
　　**HTTP连接使用的是“请求—响应”的方式，在请求时需要先建立连接，接着需要客户端向服务器发出请求后，服务器端才能回复数据。** 
　　很多情况下，需要服务器端主动向客户端推送数据，保持客户端与服务器数据的实时与同步。此时若双方建立的是Socket连接，服务器就可以直接将数据传送给客户端；若双方建立的是HTTP连接，则服务器需要等到客户端发送一次请求后才能将数据传回给客户端，因此，客户端定时向服务器端发送连接请求，不仅可以保持在线，同时也是在“询问”服务器是否有新的数据，如果有就将数据传给客户端。

### 4.5 参考链接

[socket连接和http连接的区别](http://blog.csdn.net/wwd0501/article/details/52412396) 
[TCP/IP、Http、Socket的区别](http://lib.csdn.net/article/computernetworks/20534)

## 4 详解HTTPS

### 4.1 HTTPS概念

　　**HTTPS协议 = HTTP协议 + SSL/TLS协议**，在HTTPS数据传输的过程中，需要用SSL/TLS对数据进行加密和解密，需要用HTTP对加密后的数据进行传输，由此可以看出HTTPS是由HTTP和SSL/TLS一起合作完成的。 
　　SSL的全称是Secure Sockets Layer，即安全套接层协议，是为网络通信提供安全及数据完整性的一种安全协议。SSL协议在1994年被Netscape发明，后来各个浏览器均支持SSL，其最新的版本是3.0。 
　　TLS的全称是Transport Layer Security，即安全传输层协议。在TLS与SSL3.0之间存在着显著的差别，主要是它们所支持的加密算法不同，所以TLS与SSL3.0不能互操作。虽然TLS与SSL3.0在加密算法上不同，但是在我们理解HTTPS的过程中，我们可以把SSL和TLS看做是同一个协议。

### 4.2 HTTPS加密的过程

![这里写图片描述](https://img-blog.csdn.net/20180318102411580?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2NoZW5saWd1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
[图片来源于：图解HTTPS](http://limboy.me/tech/2011/02/19/https-workflow.html) 
　　一个HTTPS请求实际上包含了两次HTTP传输，可以细分为8步： 
（1）客户端向服务器发起HTTPS请求，连接到服务器的443端口。 
（2）服务器端有一个密钥对，即公钥和私钥，服务器端保存着私钥，不能将其泄露，公钥可以发送给任何人。 
（3）服务器发送了一个SSL证书给客户端，SSL 证书中包含的具体内容有：证书的发布机构CA、证书的有效期、公钥、证书所有者、签名。 
（4）客户端收到服务器端的SSL证书之后，验证服务器发送的数字证书的合法性，如果发现发现数字证书有问题，那么HTTPS传输就无法继续。如果数字证书合格，那么客户端会生成一个随机值，这个随机值就是用于进行对称加密的密钥；然后用公钥对对称密钥进行加密，变成密文。至此，HTTPS中的第一次HTTP请求结束。 
（5）客户端会发起HTTPS中的第二个HTTP请求，将加密之后的客户端密钥发送给服务器。 
（6）服务器接收到客户端发来的密文之后，会用自己的私钥对其进行非对称解密，解密之后的明文就是对称密钥，然后用对称密钥对数据进行对称加密。 
（7）服务器将加密后的密文发送给客户端。 
（8）客户端收到服务器发送来的密文，用对称密钥对其进行对称解密，得到服务器发送的数据。这样HTTPS中的第二个HTTP请求结束，整个HTTPS传输完成。

　　第三步中，当客户端接收到服务器的数字证书的时候，会进行如下验证： 
（1）首先客户端会用设备中内置的CA的公钥尝试解密数字证书，如果所有内置的CA的公钥都无法解密该数字证书，说明该数字证书不是由一个全球知名的CA签发的，这样客户端就无法信任该服务器的数字证书。 
（2）如果有一个CA的公钥能够成功解密该数字证书，说明该数字证书就是由该CA的私钥签发的。 
（3）还需要检查客户端当前访问的服务器的域名是与数字证书中提供的“颁发给”这一项吻合，以及检查数字证书是否过期等。

### 4.3 HTTP与HTTPS的区别

- HTTPS加密传输协议，HTTP明文传输协议；
- HTTPS需要用SSL证书，HTTP不用；
- HTTPS比HTTP更加安全搜索引擎更友；
- HTTPS标准端口443，HTTP标准端口80；
- HTTPS基于传输层，HTTP基于应用层；
- HTTPS浏览器显示绿色安全锁HTTP没显示。 
  ![这里写图片描述](https://t11.baidu.com/it/u=4156134628,1154602463&fm=173&s=082B57308D22452A086440CA0300A0B3&w=359&h=173&img.JPG)

### 4.4 HTTPS参考链接

[HTTPS 理论基础及其在 Android 中的最佳实践](https://juejin.im/entry/575ab9b36be3ff0069492bc6) 
[HTTPS 原理详解](https://baijiahao.baidu.com/s?id=1570143475599137&wfr=spider&for=pc) 
[聊聊HTTPS和SSL/TLS协议](http://www.techug.com/post/https-ssl-tls.html) 
[深入理解HTTPS协议](https://juejin.im/post/5a2fbe1b51882507ae25f991) 
[HTTPS加密原理](https://juejin.im/entry/5a9ac15bf265da239e4d8831)

## 5 一次完整的HTTP请求所经历的7个步骤

- 1、建立TCP/IP连接 
  在HTTP工作开始之前，客户端与服务器通过TCP三次握手建立连接。
- 2、客户端向服务器发送HTTP请求行 
  建立了TCP连接，客户端向服务器发送HTTP请求行，例如：GET/sample/hello.jsp HTTP/1.1 。
- 3、客户端发送请求头和请求体 
  客户端向服务器发送请求头信息、请求体内容，最后客户端会发送一空白行表示客户端请求完毕。
- 4、服务器应答响应行 
  服务器会做出应答，表示对客户端请求的应答， 状态行：HTTP/1.1 200 OK 。
- 5、服务器向客户端发送响应头信息 
  服务器向客户端发送头信息后，它会发送一个空白行来表示头信息的发送到此为结束。
- 6、服务器向客户端发送响应包体 
  接着，服务器以Content-Type响应头信息所描述的格式向客户端发送响应包——所请求的实际数据。
- 7、 服务器关闭TCP连接 
  如果浏览器或者服务器在其头信息加入了这行代码：Connection:keep-alive ，TCP连接在发送后将仍然保持打开状态，客户端可以继续通过相同的连接发送请求。

## 6 其他参考链接

[Java 篇](http://eddy.wiki/interview-java.html) 
[HTTP请求行、请求头、请求体详解](http://blog.csdn.net/u010256388/article/details/68491509) 
[HTTP请求、响应报文格式](http://blog.csdn.net/a19881029/article/details/14002273) 
[一次完整的HTTP请求所经历的7个步骤](https://www.cnblogs.com/jiu0821/p/5641600.html) 
[http.png](https://github.com/JackyAndroid/AndroidInterview-Q-A/blob/master/picture/http.png)