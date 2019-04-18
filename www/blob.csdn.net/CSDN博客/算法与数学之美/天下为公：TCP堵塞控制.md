# 天下为公：TCP堵塞控制 - 算法与数学之美 - CSDN博客
2017年10月27日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：169
在 TCP 协议中，我们使用连接记录 TCP 两端的状态，使用编号和分段实现了 TCP 传输的有序，使用广播窗口来实现了发送方和接收方处理能力的匹配，还使用重复发送机制来实现 TCP 传输的可靠性。最初的 TCP 协议就是由上述的几个方面构成。1980 年代，TCP 协议又引入了流量控制机制。

****令人头痛的堵车****
从 1980 年代开始，网络变得繁忙。许多网络中出现了大量的堵塞（congestion）。堵塞类似于现实中的堵车。网络被称为“信息高速公路”。许多 IP 包在网络中行驶，并经过一个一个像十字路口一样的路由器，直到到达目的地。一个路由器如果过度繁忙，会丢弃一些 IP 包。UDP
 协议不保证传输的可靠性，所以丢失就丢失了。而 TCP 协议需要保证传输的可靠性，当包含有 TCP 片段的 IP 包丢失时，TCP 协议会重复发送 TCP 片段。于是，更多的“汽车”进入到公路中，原本繁忙的路由器变得更加繁忙，更多的 IP 包丢失。这样就构成了一个恶性循环。这样的情况被称为堵塞崩溃（congestion collapse）。每个发送方为了保证自己的发送质量而乱发车，是造成堵塞崩溃的主要原因。当时的网络中高达 90%的传输资源可能被堵塞崩溃所浪费。
为了弥补这一缺陷，从 1980 年代开始，TCP 协议中开始加入堵塞控制（congestion control）的功能，以避免堵塞崩溃的出现。多个算法被提出并实施，大大改善了网络的交通状况。流量控制类似于生活中的真实交通。现实中，当我们遇到堵车，可能就会希望兴建立交桥和高架，或者希望有一位交警来疏导交通。而 TCP 协议的堵塞控制是通过约束自己实现的。当
 TCP 的发送方探测到网络交通拥堵时，会控制自己发送片段的速率，以缓解网络的交通状况，避免堵塞崩溃。简言之，TCP 协议规定了发送方需要遵守的“公德”。
我们先来说明堵塞是如何探测的。在 TCP 重新发送中，我们已经总结了两种推测 TCP 片段丢失的方法：ACK 超时和重复 ACK。一旦发送方认为 TCP 片段丢失，则认为网络中出现堵塞。另一方面，TCP 发送方是如何控制发送速率呢？TCP 协议通过控制滑窗大小来控制发送速率。在 TCP 滑窗管理中，我们已经见到了一个窗口限制，就是广播窗口大小，以实现 TCP
 流量控制。TCP 还会维护一个阻塞窗口大小，以根据网络状况来调整滑窗大小。真实滑窗大小取这两个滑窗限制的最小值，从而同时满足流量控制和堵塞控制的限制。我们下面就来看一看阻塞窗口。

**阻塞窗口**
阻塞窗口总是处于两种状态的一个。这两种状态是慢起动（slow start）和堵塞避免（congestion avoidance）。下面是它们的示意图：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkxywDlVox1xHvZKHUgONBZEdmhtytNME5MsbS3UzgPIZJFI176iaeVM23pM5icY0ucWFjHlDUuXpqeA/0?wx_fmt=png)
上图是概念性的。实际的实施要比上图复杂，而且根据算法不同会有不同的版本。cwnd 代表阻塞窗口大小（congestion window size）。我们以片段的个数为单位，来表示阻塞窗口大小 。实际应用中会以字节为单位，但并不影响这里的讲解。
阻塞窗口从慢启动状态开始。慢启动的特点是初始速率低，但速率不断倍增。每次进入到慢启动状态时，阻塞窗口大小都需要重置为初始值 1。发送方每接收到一个正确的 ACK，就会将阻塞窗口大小增加 1，从而实现速率的倍增。需要注意的是，由于累计 ACK，速率增长可能会小于倍增。
当阻塞窗口大小达到阈值（图中 ssthresh）时，阻塞窗口进入到阻塞避免状态。发送速率会继续增长。发送方在每个窗户所有片段成功传输后，将窗口尺寸增加 1，等效于每个往返时间增加 1。所以在阻塞避免模式下，阻塞窗口大小线性增长，增长速率慢。如果在阻塞避免下有片段丢失，重新回到慢启动状态，并将阈值更新为阻塞窗口大小的一半。
我们看到，触及阈值是从慢启动到 c 阻塞避免的切换点。而片段丢失是阻塞避免到慢启动的切换点。一开始阈值一般比较大，所以慢启动可能在切换成阻塞避免之前就丢失片段。这种情况下，慢启动会重新开始，而阈值更新为阻塞窗口大小的一半。
总的来说，发送速率总是在增长。如果片段丢失，则重置速率为 1，并快速增长。增长到一定程度，则进入到慢性增长。快速增长和慢性增长的切换点会随着网络状况更新。通过上面的机制，让发送速率处于动态平衡，不断的尝试更大值。初始时增长块，而接近饱和时增长慢。但一旦尝试过度，则迅速重置，以免造成网络负担。阻塞控制有效的提高了互联网的利用率，但依然不完善。一个常见的问题是阻塞窗口小在接近饱和时线性增长，因此对新增的网络带宽不敏感。

****TCP 实践****
TCP 协议利用流量控制机制来实现整个网络的总体效率。到现在为止，已经讲解了 TCP 的几大模块：分段与流，滑窗，连接，流量控制，重新发送，堵塞控制。现在，我们可以在编程中实际利用一下 TCP 协议。
在 Python 中，我们可以用标准库中的 socket 包来建立 TCP 连接。这个包已经用于 UDP 协议的套接字编程，它同样可以用于 TCP 协议的套接字编程。我们需要写两个程序，分别用于 TCP 连接的服务器端和客户端。一旦连接建立，双方可以相互通信。下面是服务器端的程序。我们用 bind()方法来赋予套接字以固定的地址和端口，用 listen()方法来被动的监听该端口。当有客户尝试用
 connect()方法连接的时候，服务器使用 accept()接受连接，从而建立一个 TCP 连接：
> 
# Written by Vamei
# Server side
import socket
# Address
HOST = ''
PORT = 8000
reply = 'Yes'
# Configure socket
s      = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
# passively wait, 3: maximum number of connections in the queue
s.listen(3)
# accept and establish connection
conn, addr = s.accept()
# receive message
request    = conn.recv(1024)
print 'request is: ',request
print 'Connected by', addr
# send message
conn.sendall(reply)
# close connection
conn.close()
上面的程序中，socket.socket()创建一个 socket 对象，并说明 socket 使用的是 IPv4（AF_INET，IP version 4）和 TCP 协议（SOCK_STREAM）。服务器写好了，下面是客户。在客户的程序中，我们主动使用 connect()方法来搜索服务器端的 IP 地址和端口，以便客户可以找到服务器，并建立连接:
> 
# Written by Vamei
# Client side
import socket
# Address
HOST = '172.20.202.155'
PORT = 8000
request = 'can you hear me?'
# configure socket
s       = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
# send message
s.sendall(request)
# receive message
reply   = s.recv(1024)
print 'reply is: ',reply
# close connection
s.close()
TCP 协议是双向的。因此，我们在连接两端都可以调用 recv()方法来接收信息，调用 sendall()方法来发送信息。这样，我们就可以在分处于两台计算机的两个进程间进行通信了。当通信结束的时候，我们使用 close()方法来关闭 TCP 连接。为了展示网络通信，上面程序最好运行于两台电脑。但如果没有两台计算机做实验，也可以将客户端的 IP 改为 127.0.0.1。这是个特殊的
 IP 地址，指向主机自身。这样，我们在同一台电脑上建立了 TCP 连接。

****总结****
流量控制要求参与 TCP 通信的各方守公德，从而提高了 TCP 通信的整体效率。这一篇的最后，还有一个实际建立 TCP 连接的例子，用 Python 语言实现。到了这里，TCP 协议的介绍就可以告一段落了。下一章我们将进入应用层。
☞  [哈尔莫斯：怎样做数学研究](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=1&sn=35143b89b06fe4f5273f210b2d6a7c91&chksm=8b7e3290bc09bb86f7bb3f158d993df3f019a7e9ce3bc8897e164e35a2ebe5a4e0bdcc111089&scene=21#wechat_redirect)
☞  [扎克伯格2017年哈佛大学毕业演讲](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=2&sn=c75293463823e4d6769638e54b64f3ec&chksm=8b7e3290bc09bb86dc1e3f8e78d0b6de8811d75f3dcb092766fcb8ba0bab1cd9ba1ddfcef3b9&scene=21#wechat_redirect)
☞  [线性代数在组合数学中的应用](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554141&idx=1&sn=74a74c4e4d08eba0dd734528aa0b08e7&chksm=8b7e32c6bc09bbd073b34c22004ac6e4d99c8a0caa64c7d3dbaa8fd55e6ef1fc87ed545b8b7e&scene=21#wechat_redirect)
☞  [你见过真的菲利普曲线吗？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554105&idx=1&sn=224ab0d38fb57facea70081385360d58&chksm=8b7e3222bc09bb34d3b6df665087e64b233778ed427598d08e809f96261e898c1c0de6188bbc&scene=21#wechat_redirect)
☞  [支持向量机(SVM)的故事是这样子的](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554096&idx=1&sn=46783e6ace661a3ccbd8a6e00fb17bf9&chksm=8b7e322bbc09bb3d73dc240f2280bddf2ef8b7824a459a24bd7f6eeadd60edb96e690d467f6e&scene=21#wechat_redirect)
☞  [深度神经网络中的数学，对你来说会不会太难？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554077&idx=2&sn=3ecd63f2205fd59df8c360c97c943ef6&chksm=8b7e3206bc09bb10a36b09547efe0c54f41423b180622c1fdc7f14747ccc8f8fecee3a12e2cd&scene=21#wechat_redirect)
☞  [编程需要知道多少数学知识？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554062&idx=1&sn=17f0a88d5e15d1adfc29c690a0b1b89b&chksm=8b7e3215bc09bb038c6caa59d0f49cedd929f9be1104beea3411186cf4c81de69efc71a17883&scene=21#wechat_redirect)
☞  [陈省身——什么是几何学](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553994&idx=2&sn=74f67a1a3ac5c705f51f2ba619b717f6&chksm=8b7e3251bc09bb47dce73319948780081efe0333ffae99ea04a9eeabbcfcb38a29b4b73fb7c1&scene=21#wechat_redirect)
☞  [模式识别研究的回顾与展望](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553949&idx=2&sn=d171680964df774397efd9db81c00347&chksm=8b7e3386bc09ba90bf0f6e1cabf82ba86ff94630cb5ee2e0f14ff9455db52be32ddbc289d237&scene=21#wechat_redirect)
☞  [曲面论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553747&idx=1&sn=e25f866d510cf2338b6d9e1b32bafb62&chksm=8b7e3348bc09ba5ea1caaf2a7bfcd80a7e7559b1983e473eda2206e56df7f38ef3cecf2f77c7&scene=21#wechat_redirect)
☞  [自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553811&idx=1&sn=000305074471c3d4c681c9cfd4e4bc93&chksm=8b7e3308bc09ba1e3043f5568a3a75a045285a1de97e4da36918bac68e7c6d579ad5d8cc25ab&scene=21#wechat_redirect)
☞  [如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553670&idx=1&sn=ea75a448c016f7229e4cb298f6017614&chksm=8b7e309dbc09b98bc622acdf1223c7c2f743609d0a577dd43c9e9d98ab4da4314be7c1002bd5&scene=21#wechat_redirect)
☞  [华裔天才数学家陶哲轩自述](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553646&idx=2&sn=bbf8f1be1ca1c66ad3f3270babea6885&chksm=8b7e30f5bc09b9e3e1a4fa735412e2fcb20df9e78f2f346bf578018ceab77de6326095d1bf71&scene=21#wechat_redirect)
☞  [代数，分析，几何与拓扑，现代数学的三大方法论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553596&idx=1&sn=bc5064e871831f862db6d19c3de6327e&chksm=8b7e3027bc09b93194fa09b25e2df400421c062927bb9120912875f8aaf0bb25553fc8f51e3b&scene=21#wechat_redirect)
![0?wx_fmt=gif](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkwJ4BpvBcQhGAbtWZZvV69s7GickZGibsKgYkTQkiaZfLYOmGS9iaaoibadibGJhT18OVZkfeJmCSUSD0zw/0?wx_fmt=gif)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域。
稿件一经采用，我们将奉上稿酬。
投稿邮箱：math_alg@163.com
商务合作：微信号hengzi5809
![0?wx_fmt=gif](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkxN5SJPzhu6icTXrIpMZqSdFzG0y6ib1c9enWGK3GxfHTRIN7ich2kzqepNvMHfktp4Ir88ibolsDBuhQ/0?wx_fmt=gif)
