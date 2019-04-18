# 大数据学习笔记(十二) -- network - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月28日 11:12:54[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：57


**Table of Contents**

[网络连接基础知识](#%E7%BD%91%E7%BB%9C%E8%BF%9E%E6%8E%A5%E5%9F%BA%E7%A1%80%E7%9F%A5%E8%AF%86)

[关于nginx和LVS](#%E5%85%B3%E4%BA%8Enginx%E5%92%8CLVS%C2%A0)

[S_NAT](#S_NAT)

[D_NAT](#D_NAT)

[LVS 模式一](#LVS%20%E6%A8%A1%E5%BC%8F%E4%B8%80)

[LVS 模式二 (DR模式)](#LVS%20%E6%A8%A1%E5%BC%8F%E4%BA%8C%20(DR%E6%A8%A1%E5%BC%8F))

[LVS模式三 管道技术   VPN](#LVS%E6%A8%A1%E5%BC%8F%E4%B8%89%20%E7%AE%A1%E9%81%93%E6%8A%80%E6%9C%AF%C2%A0%20%C2%A0VPN)

[​](#%E2%80%8B)

# 网络连接基础知识

下面有两幅图，第一幅是介绍 TCP/IP协议的4层分别在做什么，第二幅就是TCP/IP这四层所填写的表，图二的表填写完整后才会真正地送出数据包。

![](https://img-blog.csdn.net/20180928092915504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180928092054474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

说明 ：
- 应用层 就是 客户端与服务端 沟通之间语言的一种同意，就好比一个中国人跟一个俄罗斯人，要想沟通可以都使用英语，各自把自己说的话，翻译成英语，对方收到以后再翻译成自己说的话。这就是应用层干的事，对请求打包并格式化。
- 传输层 就是 客户端与服务端 建立连接，但仅仅是建立了一个连接，这里并没有真正的发送数据，TCP建立连接时 会三次握手进行确认，第一次将图二表中紫色边框表格中的 Flag(6) 替换为 sync ，服务端收到以后 在 sync 的基础上加上一个ack 再发送给 客户端，客户端收到以后再将ack 返回给 服务端，这样双方就知道 各自 输入通道和输出通道 都能够走通，并且可以和对方建立连接。断开连接需要4次，客户端告诉服务端我要断开连接，服务端收到以后告诉客户端OK，然后客户端和服务端基本上同时断开连接。
- 网络层  我们通过 ip 地址 与 子网掩码 与 运算，得出的结果 作为 网段号，处于同一个网段号下的 称为局域网，局域网内的多台计算机往往通过 交换机连接。交互机又连接路由器，路由器负责跟外网进行连接。当我们从客户端发送一堆数据包给服务器时，客户端会遵循 "下一跳原则"   将数据包先传递给离它最近的机器，然后再进行转发。下一跳 跳到哪里，可以通过route 命令查看

```bash
[root@node001 network-scripts]# route
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
192.168.46.0    *               255.255.255.0   U     0      0        0 eth0
link-local      *               255.255.0.0     U     1002   0        0 eth0
default         192.168.46.2    0.0.0.0         UG    0      0        0 eth0
```

比如说我们想给百度发送数据，那我们就用百度的 ip地址 与 上面的Genmask 进行与运算，如果计算结果 等于 Destination的结果，那么下一跳的 ip地址就是 对应的 Gateway地址 ， 也就是说网络层干的事就是将数据发送到外网上面。

以下命令可以查看ip地址绑定的 网卡号 
- 链路层 "下一跳" 如果想跳过去，需要知道对方的MAC地址(网卡号)，想要知道MAC地址，就需要使用ARP协议。

```bash
[root@node001 network-scripts]# arp -a
? (192.168.46.2) at 00:50:56:f4:6c:f2 [ether] on eth0
? (192.168.46.1) at 00:50:56:c0:00:08 [ether] on eth0
```

首先客户端 由于不知道 路由器的MAC地址，所以它将 目标MAC填写为 全 F ，这代表广播。这会使同局域网下的所有电脑接收到该条信息。然后会各自确定 自己是否是 客户端 下一跳 跳到的那个ip。如果不是就把数据包丢弃。这时路由器发现自己是下一跳的地址，那么就会返回信息。由于交换机有一个 表用来记录 交换机上所插的端口对应的机器的MAC地址，所以就将路由器的MAC反回给了客户端。最终客户端将 图二的表填写完成就可以真正的进行数据传输了。

# 关于nginx和LVS 

![](https://img-blog.csdn.net/20180928110757845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

LVS服务器的角色是 第四层 即传输层，但是它没有进行三次握手，LVS 可以实现负载均衡，也就是转发请求，它接收到一个数据包查看一下端口号，然后就会进行转发。因此它的效率很高。

nginx 的角色是 第七层，即应用层。它可以看到客户端的请求内容，它的优点就是可以支持 5w 个活跃用户同时访问。而tomcat才不到 1000。所以它也用来做负载均衡。

一般都是   LVS 再连接 几个 nginx 从而实现负载均衡。

# S_NAT

比如说我们在自己家电脑上访问百度，即向百度发送请求，请求的客户端的ip为 192.168.46.222:1613 ，另一台家里电脑也访问百度，请求的客户端的ip为192.168.46.223:1313 , 这两个请求如果想在网络中传输，必须先经过路由器，在路由器当中会将 这两个 ip 转换为 公网 ip，因为我们自己电脑的ip实际上是与其他人电脑ip 存在相同的，由于网民太多。。 但是在公网上传输必须保证 ip地址是唯一的， 所以当我们花钱给家里面办网，那些公司就会给我们申请一个公网ip，路由器就将私有 ip 转换为公网 ip ， 并随机为这两个 请求 生成两个 端口，并与 原 ip : port 进行映射， 映射的目的就是当服务器返回结果时，路由器知道具体返回给哪个主机。  

如 路由器 生成 两个端口号 123 和 124 ，分别分配给 1613 和 1313 。 

则 在路由器中 会做 以下映射

                      123   --->   192.168.46.222:1613

                      124   --->   192.168.46.223:1313 

下一步 就是改变 请求的 ip 和 端口号 以便在公网中传输

假设家里的公网ip 为 18.18.18.8

                       18.18.18.8:123  

                       18.18.18.8:124

# D_NAT

上面的S_NAT 是将 客户端的IP更改为公网的IP，D_NAT是将 服务端的IP更改为真实的服务器IP，负载均衡服务器对其进行了应用

![](https://img-blog.csdn.net/20180928161052341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### LVS 模式一

客户端发送请求给LVS，LVS更改请求地址为真正的服务器地址，然后发送请求，服务器处理完请求给LVS。LVS将返回的地址改为客户端。 

![](https://img-blog.csdn.net/20180928161132883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### LVS 模式二 (DR模式)

实现机制与区别：

模式一存在的问题就是 服务器返回请求很慢，并且负载均衡压力较大。

模式二通过真实服务器直接将返回结果给客户端从而减轻负载均衡的压力，之所以可以实现是因为模式二的真是服务端中内置了负载均衡的ip，该ip对内可见，对外隐藏。

模式一的负载均衡访问服务器是通过外网 跳多次访问到的。

模式二是通过寻找服务器的MAC地址 从而请求服务器，也就是说模式二 负载均衡器和真正服务器必须在同一个局域网内。

![](https://img-blog.csdn.net/20180928162123727?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### LVS模式三 管道技术   VPN

### ![](https://img-blog.csdn.net/20180928201837818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们之间访问国外的网站由于防火墙的原因我们无法找到国外的节点。使用VPN时，我们向国外网站发送一条请求，请求的数据包会被套上一个数据包从而发送到LVS，LVS获取到这个数据包，拆开外层内容，就会知道我们想要访问的国外网站是哪个，然后再套上一层数据包去向外国网站发送请求，国外网站受到请求返回数据时的数据包也会被套上一层从而能返回给DNS，DNS拆开数据包会知道这是国外网站返回给我们的，然后再套上一个数据包使其发送给我们。

