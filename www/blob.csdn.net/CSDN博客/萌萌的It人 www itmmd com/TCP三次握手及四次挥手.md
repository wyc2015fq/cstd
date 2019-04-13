
# TCP三次握手及四次挥手 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月20日 17:13:16[Jlins](https://me.csdn.net/dyllove98)阅读数：1661标签：[tcp																](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)


相对于SOCKET开发者,TCP创建过程和链接折除过程是由TCP/IP协议栈自动创建的.因此开发者并不需要控制这个过程.但是对于理解TCP底层运作机制,相当有帮助.而且对于有网络协议工程师之类笔试,几乎是必考的内容.企业对这个问题热情之高,出乎我的意料：-）。有时上午面试前强调这个问题，并重复讲一次，下午几乎每一个人都被问到这个问题。
因此在这里详细解释一下这两个过程。
TCP三次握手
所谓三次握手(Three-way Handshake)，是指建立一个TCP连接时，需要客户端和服务器总共发送3个包。
三次握手的目的是连接服务器指定端口，建立TCP连接,并同步连接双方的序列号和确认号并交换 TCP 窗口大小信息.在socket编程中，客户端执行connect()时。将触发三次握手。
![](http://blogimg.chinaunix.net/blog/upfile2/100327002629.png)
第一次握手:
客户端发送一个TCP的SYN标志位置1的包指明客户打算连接的服务器的端口，以及初始序号X,保存在包头的序列号(Sequence Number)字段里。
![](http://blogimg.chinaunix.net/blog/upfile2/100327002911.png)
第二次握手:
服务器发回确认包(ACK)应答。即SYN标志位和ACK标志位均为1同时，将确认序号(Acknowledgement Number)设置为客户的I S N加1以.即X+1。
![](http://blogimg.chinaunix.net/blog/upfile2/100327003054.png)
第三次握手.
客户端再次发送确认包(ACK) SYN标志位为0,ACK标志位为1.并且把服务器发来ACK的序号字段+1,放在确定字段中发送给对方.并且在数据段放写ISN的+1![](http://blogimg.chinaunix.net/blog/upfile2/100327003214.png)
SYN攻击
在三次握手过程中，服务器发送SYN-ACK之后，收到客户端的ACK之前的TCP连接称为半连接(half-open connect).此时服务器处于Syn_RECV状态.当收到ACK后，服务器转入ESTABLISHED状态.
Syn攻击就是 攻击客户端 在短时间内伪造大量不存在的IP地址，向服务器不断地发送syn包，服务器回复确认包，并等待客户的确认，由于源地址是不存在的，服务器需要不断的重发直 至超时，这些伪造的SYN包将长时间占用未连接队列，正常的SYN请求被丢弃，目标系统运行缓慢，严重者引起网络堵塞甚至系统瘫痪。
Syn攻击是一个典型的DDOS攻击。检测SYN攻击非常的方便，当你在服务器上看到大量的半连接状态时，特别是源IP地址是随机的，基本上可以断定这是一次SYN攻击.在Linux下可以如下命令检测是否被Syn攻击
netstat -n -p TCP | grep SYN_RECV
一般较新的TCP/IP协议栈都对这一过程进行修正来防范Syn攻击，修改tcp协议实现。主要方法有SynAttackProtect保护机制、SYN cookies技术、增加最大半连接和缩短超时时间等.
但是不能完全防范syn攻击。
TCP 四次挥手
TCP的连接的拆除需要发送四个包，因此称为四次挥手(four-way handshake)。客户端或服务器均可主动发起挥手动作，在socket编程中，任何一方执行close()操作即可产生挥手操作。
![](http://blogimg.chinaunix.net/blog/upfile2/100327022731.jpg)
参见wireshark抓包，实测的抓包结果并没有严格按挥手时序。我估计是时间间隔太短造成。
![](http://blogimg.chinaunix.net/blog/upfile2/100327023334.png)
针对tcp/ip协议的网络攻击：
Land攻击
通过发送源地址和目的地址相同,源端口和目的端口相同的ICMP
 echo报文或TCP syn请求报文,可以导致主机不断地向自己发送报文,最终导致系统崩溃。只要检查报文的源地址和目的地址是否相等、源端口和目的端口是否相等,就可以判断出是否为Land攻击。
Syn Flooding
利用TCP协议“三次握手”机制而发动的攻击。当Server(B)收到Client(A)的syn请求报文时,将发送一个(ack,syn)应答报文,同时创建一个控制结构,将其加入到一个队列中,等待对方的ack报文。接收到ack报文后,双方都进入连接状态,就可以发送数据。如果Server在一段时间内没有收到应答信息,则控制块将被释放。在TCP协议软件中,通常对每个端口等待建立连接的数目(即Backlog)有一定限制(Windows
 NT4.0 : 6 , Solaris : 32) ,当队列长度到达设定的阈值时,将丢弃后面到达的TCP
 Syn请求报文。如果攻击者不断发送大量的TCP syn报文,其他用户就无法再连接到被攻击主机。增加Backlog数目、减少连接等待时间等措施都无法阻止此种攻击。虽然“连接代理”技术可以保护网络内部主机不受攻击,但连接代理无法阻止本身受到Syn
 Flooding攻击,而且由于所有的数据都要经过连接代理处理,总的网络延迟将会增加。实时地监测TCP连接请求,过滤TCP
 Syn Flooding攻击报文,是阻止此攻击的较为有效的方法。本文给出的TCP
 Syn Flooding检测算法(算法1),运行入侵检测系统上,可以检测多种类型的Syn
 Flooding攻击,包括固定的源IP地址,随机变化的源地址等。
算法1TCP
 Syn Flooding检测算法
time window:时间窗口packet set :syn ,syn &ack ,ack报文集合
source ip :报文源IP地址
destination ip :报文目的IP地址
syn flood threshold :判断syn
 flood攻击的阈值
current time :当前系统时间
sys number = 0 ;
ack number = 0 ;
ack Syn number = 0 ;
for (packetx∈packet set) {
if (packetx. time2current time > = time window) {
Delete packetx from packet set ;
Continue ;
}
if (packetx. syn == 1)AND(packetx. ack == 0)
syn number ++ ;
else if (packetx. syn == 0)AND(packetx. ack == 1)
ack number ++ ;
else
ack syn number ++ ;
}
if ( (syn number >= syn flood threshold) AND( ( syn number > = 3 3
ack number)OR(syn number > = 3 3 syn ack number) ) )
syn flood attack = TURE;
else
syn flood attack = FALSE;
end
TCP会话劫持
利用TCP会话劫持,攻击者可以方便地修改、伪造数据。
它的基本原理如下:
TCP通过三次握手建立连接以后,主要采用滑动窗口机制来验证对方发送的数据。如果对方发送的数据不在自己的接收窗口内,则丢弃此数据,这种发送序号不在对方接收窗口的状态称为非同步状态。当通信双方进入非同步状态后,攻击者可以伪造发送序号在有效接收窗口内的报文,也可以截获报文,篡改内容后,再修改发送序号,而接收方会认为数据是有效数据。TCP劫持的关键在于使通信双方进入非同步状态。有多种方法可以达到此目的。如图2所示,在主机A发送syn请求后,B发送ack
 & syn进行应答,则A认为连接已经建立。此时,攻击者伪装成A向B发送一个rst报文,则B释放连接,攻击者继续伪装成A用自己的初始序列号和B建立新的连接,而A和B对此毫不觉察。当攻击者伪装成A和B建立连接后,A和B就已经进入了非同步状态。利用Telnet协议的NOP命令也可以使通信双方进入非同步状态。主机B接收到NOP命令后,并不进行任何操作,但确认序列号将会加1。如果攻击者伪装成A向B发送大量的NOP命令,则会造成A和B的非同步状态。
检测TCP劫持的关键在于检测非同步状态。如果不断收到在接收窗口之外的数据或确认报文,则可以确定遭到TCP劫持攻击。
TCP伪装
利用TCP劫持进行攻击时,攻击者必须能监测到双方的通信报文,才能得到双方的发送序号,但进行TCP伪装攻击时,攻击者不需要监测到双方的通信报文,就可以发动攻击，攻击者首先用真实的地址访问V提供的服务如WWW等,获得V的当前TCP初始序列号,然后伪装成C尝试建立到主机V的Telent端口的连接(攻击者要确保C关机或被攻击,无法响应外部报文)。连接建立以后,攻击者就可以向V发送一系列命令。此种攻击可以绕过防火墙或主机对IP地址的检查。
由于攻击者收不到来自V的任何信息,要成功进行攻击,必须满足两个条件:
1)建立连接时,攻击者需要知道V的当前初始序列号;
2)发送命令时,攻击者需要知道V的响应报文的数据长度。
条件2)可轻易满足。攻击者只要在能合法访问的主机上进行测试,就可以知道响应信息的内容和长度。TCP协议规定所有TCP连接使用同一个初始序列号计数器,每4us加1。当发送一个TCP连接请求时,使用计数器的当前值。由于初始序列号计数器随时间线性增加,因此,可以按照下式计算序列号的值:Seq(t)
 = Seq(t0) + (t - t0) 3 r其中,t为当前时间,t0为过去某一时间,r为序列号按时间增加的速率。由于很容易能得到t0
 ,Seq(t0) ,攻击者只需要测量攻击主机和V之间的网络传输时延就可以成功地进行攻击。虽然网络的延迟总是在随机变化,但在短时间内,仍然是相对稳定的。攻击者经过测量延迟的平均值,就可以估计出V使用的初始序列号。减小计数器的时间间隔,可以增加攻击的难度,但无法从根本上阻止此类攻击。如果初始序列号使用随机值,或在初始序列号中加入秘密信息,比如IP地址的HASH值等,就可以防止此种攻击。文献[8讨论了预防TCP序列号猜测的方法。在应用层使用认证的办法,也可以防止TCP伪装攻击。检测TCP伪装只能在进行连接时进行,当连接成功后,就无法再进行检测。当攻击者进行TCP伪装攻击时,首先需要探测TCP初始序列号的生成机制、测试网络延迟,然后再尝试建立连接。如果收到多个含有错误的确认序列号的ack报文,则可以断定受到了TCP伪装攻击。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

