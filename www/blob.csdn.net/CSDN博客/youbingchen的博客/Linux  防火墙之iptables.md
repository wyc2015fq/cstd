# Linux  防火墙之iptables - youbingchen的博客 - CSDN博客





2016年06月16日 15:34:08[youbingchen](https://me.csdn.net/youbingchen)阅读数：430








防火墙最主要的任务就是规划出： 

+ 切割被信任(子域)与不被 信任(Internet)的 网段 

+ 切分出可提供Internet的服务与必须受到保护的服务 

+ 分析出可接受与不可接受的封包状态。
# Linux 系统上 防火墙的主要类别



- Netfilter(封包过滤机制） 




> 
就是分析进入主机网络分包，将分包 表头数据提取出来进行分析，以决定是否丢弃。由于这种方式可以直接 分析封包 表头的数据，所以包括(MAC),软件地址（IP），TCP,UDP,ICMP等封包 的 消息 都可以进行过滤分析 的 功能。Netfilter提供了iptables这个软件来作为防火墙封包过滤的指令。由于Netfilter是核心内建的功能，因此它的效率非常的 高。
  
- TCP Wrappers(程序控管） 

  另一种抵挡封包进入的办法，为透过服务器程序的外挂（tcpd)来处置，与封包过滤 不同，这种机制主要是分析谁对某程序进行存取，然后透过规则去分析服务器谁能够联机，谁不能联机，主要透过分析服务器程序来掌控，因此与启动的出口无关，只与程序的名称有关， 举例来说，我们知道 FTP 可以启动在非正规的 port 21 进行监听，当你透过 Linux 内建的 TCP wrappers 限制 FTP 时， 那么你只要知道 FTP 的软件名称 (vsftpd) ，然后对他作限制，则不管 FTP 启动在哪个端口口，都会被该规则管理的
- proxy 代理 

  其实代理服务器 是一种网络服务，下图为例： 
![这里写图片描述](https://img-blog.csdn.net/20160616153151328)

  以上图为例，当 Client 端想要前往 Internet 取得 Google 的数据时，他取得数据的流程是这样的：- 1.client 会向 proxy server 要求数据，请 proxy 帮忙处理；
- 2.proxy 可以分析使用者的 IP 来源是否合法？使用者想要去的 Google 服务器是否合法？ 如果这个 client 的要求都合法的话，那么 proxy 就会主动的帮忙 client 前往 Google 取得资料； 

  Google 所回传的数据是传给 proxy server 的喔，所以 Google 服务器上面看到的是 proxy server 的 IP; 

  +　3.最后 proxy 将 Google 回传的数据送给 client。


# Netfilter 的rule

![这里写图片描述](https://img-blog.csdn.net/20160616153206079)

> 
当一个网络封包要进入到主机之前，会先经由 NetFilter 进行检查，那就是 iptables 的规则了。 检查通过则接受 (ACCEPT) 进入本机取得资源，如果检查不通过，则可能予以丢弃 (DROP) ！ 上图中主要的目的在告知你：『规则是有顺序的』！例如当网络封包进入 Rule 1 的比对时， 如果比对结果符合 Rule 1 ，此时这个网络封包就会进行 Action 1 的动作，而不会理会后续的 Rule 2, Rule 3…. 等规则的分析了。


# iptables 的表格和链

上图只是众多表格 中的 一条 链，防火墙软件里面有多个表格，每个 表格都定义出自己的默认政策与规则，同时每个表格的用途都不相同。 
![这里写图片描述](https://img-blog.csdn.net/20160616153255627)
> 
inux 的 iptables 至少就有三个表格，包括管理本机进出的filter、管理后端主机(防火墙内部的其他计算机) 的 nat、管理特殊旗标使用的mangle(较少使用)。更有甚者，我们还可以自定义额外的链呢！每个表格与其中链的用途分别是这样的： 

  +  filter(过滤器)：主要进入Linux本机的封包有关 

     + Input：主要与想要进入Linux本机的分包有关 

     + Output：主要与Linux本机所送出的封包相关 

     + FORWARD：与本机没有什么关系，主要是传递封包到后端的计算机，与下列的nat table相关 

  + nat(地址转换）： 这个表格主要在进行来源与目的的 ip地址或端口转换，与了linux 本机不相关，主要与Linux主机后的局域网网络内计算机相关 

      + PRERROUTING: 在 进行路由判断之前要进行规则(DNAT/RECDIRECT) 

      + POSTROUTING: 在进行路由判断之后要进行的规则（SNAT/MASQUERADE) 

      + OUTPUT: 与发送出去的封包有关。 

  + mangle（破坏者）：这个表格主要是与特殊的封包的路由旗标有关，原来只有PREROUTINNG以及 OUTPUT链。后面加入INPUT以及FORWARD链。 

  所以说，如果你的 Linux 是作为 www 服务，那么要开放客户端对你的 www 要求有响应，就得要处理 filter 的 INPUT 链； 而如果你的 Linux 是作为局域网络的路由器，那么就得要分析 nat 的各个链以及 filter 的 FORWARD 链才行。（如下图：盗用鸟哥的图） 
![这里写图片描述](https://img-blog.csdn.net/20160616153310941)

  防火墙事实上是用来管制 LAN 内的其他主机的话，那么你就必须要再针对 filter 的 FORWARD 这条链，还有 nat 的 PREROUTING, POSTROUTING 以及 OUTPUT 进行额外的规则订定才行。
# iptables 实战

> 
上面讲一大堆的理论，接下来来点实战教程。


### iptables的 语法

```
iptables [-t tables] [-L] [-nV]
# 选项与参数
# -t : 后面接table,例如nat或filter，若省略，默认是filter
# -L: 列出目前的table的规则
# -n:不进行 IP与HOSTNAME的反查，显示讯息的速度会很快
# -v:  列出更多的信息，包括通过该规则的封包总位数，相关网络接口等
```

![这里写图片描述](https://img-blog.csdn.net/20160616153339754)

+ target：代表进行的动作， ACCEPT 是放行，而 REJECT 则是拒绝，此外，尚有 DROP (丢弃) 的项目 

+ prot：代表使用的封包协议，主要有 tcp, udp 及 icmp 三种封包格式； 

+ opt：额外的选项说明 

+ source ：代表此规则是针对哪个来源 IP进行限制？ 

+ destination ：代表此规则是针对哪个目标 IP进行限制？
```
iptables -F
iptables -X
iptables -Z
# 上面三个指令会将防火墙的所有的规则都清楚，但是不会改变预设政策
```

#### 定义预设政策

语法：

```
iptables [-t nat] -P [INPUT,OUTPUT<FORWARD][ACCEPT,DROP]
选项与参数
-P: 定义政策
ACCEPT：该封包可接受
DROP: 该封包直接丢弃，不会让client端知道为何被丢弃
```

#### 封包的基础比对

语法：

```
iptables [-AI链名 ] [-io 网络接口] [-p 协议] [-s  来源IP/网域] [-d 目标IP/网域] -j [ACCEPT REJECT DROP LOG]
# 选项详解
-AI 链名
    -A: 新增一条规则，该规则增加 在原本规则的最后的一条
    -Ｉ：　插入一条规则，如果没有指定规则的顺序，默认是插入第一天规则。
- io 网络接口：
    -i: 封包所进入哪个网络接口，例如 eth0，lo等 接口。需要与INPUT配合
    -o: 封包所传出的哪个网络接口，需要与OUTPUT链配合。
-p 协议: 设定此规则适用于哪一种封包格式
     主要封包格式有:tcp,udp,icmp及all。
-s 来源 IP/网域：
   IP: 192.0.100
   网域: 192.168.0.0/24或者192.168.0.0/255.255.255.0  !表示除了之外
-d 目标IP/网域
-j: 后面接动作，主要的动作有接受（ACCEPT)、丢弃（DROP）、拒绝 （REJECT）及记录（LOG）
```

#### TCP、UDP的规则 比对：针对端口设定
`iptables [-AI链名 ] [-io 网络接口] [-p 协议] [-s  来源IP/网域] [-sport 源端口] [-d 目标IP/网域] -[dport 目的端口] -j [ACCEPT REJECT DROP LOG]`
#### iptables 外挂模块：mac 与 state

> 
在 kernel 2.2 以前使用 ipchains 管理防火墙时，通常会让系统管理员相当头痛！因为 ipchains 没有所谓的封包状态模块，因此我们必须要针对封包的进、出方向进行管控。举例来说，如果你想要联机到远程主机的 port 22 时，你必须要针对两条规则来设定：


```
本机端的 1024:65535 到远程的 port 22 必须要放行 (OUTPUT 链)；
远程主机 port 22 到本机的 1024:65535 必须放行 (INPUT 链)；
```

！因为如果你要联机到 10 部主机的port22时，假设OUTPUT为预设开启(ACCEPT)，你依旧需要填写十行规则，让那十部远程主机的 port 22 可以联机到你的本地端主机上。 那如果开启全部的 port 22 呢？又担心某些恶意主机会主动以 port 22 联机到你的机器上！ 同样的道理，如果你要让本地端主机可以连到外部的 port 80 (WWW 服务)，那就更不得了～ 这就是网络联机是双向的一个很重要的概念！好在我们的 iptables 免除了这个困扰！他可以透过一个状态模块来分析。

```
iptables -A INPUT [-m state] [--state 状态]
# 选项 
-m :  一些 iptables的 外挂模块，常见 的 有：
   state:状态模块
   mac：网卡硬件地址
--  state : 一些封包的状态，主要有
   INVALID: 无效的封包，如数据损坏的包
   ESTABLISHED:已经联机成功 的联机的状态
   NEW：想要新联机的封包的状态
   RELATED：表示 这个 封包是与我们主机发送出去的封包有关。
```

如果有些不老实的人，老是可以透过修改 IP 去尝试透过路由器往外跑，那你该怎么办？ 难道将整个区网拒绝？并不需要的，你可以透过之前谈到的 ARP 相关概念，去捉到那部主机的 MAC ，然后透过上头的这个机制， 将该主机整个 DROP 掉即可。不管他改了什么 IP ，除非他知道你是用网卡的 MAC 来管理。











