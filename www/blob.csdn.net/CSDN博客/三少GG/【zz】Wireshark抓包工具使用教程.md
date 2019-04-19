# 【zz】Wireshark抓包工具使用教程 - 三少GG - CSDN博客
2012年06月12日 01:26:00[三少GG](https://me.csdn.net/scut1135)阅读数：2748

**[Wireshark抓包工具使用教程以及常用抓包规则](http://fangxin.blog.51cto.com/1125131/735178)**
Wireshark是一个非常好用的抓包工具，当我们遇到一些和网络相关的问题时，可以通过这个工具进行分析，不过要说明的是，这只是一个工具，用法是非常灵活的，所以今天讲述的内容可能无法直接帮你解决问题，但是只要你有解决问题的思路，学习用这个软件就非常有用了。
Wireshark官方下载地址：[http://www.wireshark.org/download.html](http://www.wireshark.org/download.html)
如果记不住，可以在百度中输入Wir就可以看到百度智能匹配的关键词了，选择第一个地址进去下载即可。![](http://img1.51cto.com/attachment/201112/100712927.png)
简单介绍下这个软件的一些常用按钮，因为本人也是接触这个软件不久，所以就简单的说下最常用的按钮好了，打开软件后，下面红框中的按钮从左到右依次是：
-列表显示所有网卡的网络包情况，一般用的很少；
**-显示抓包选项，一般都是点这个按钮开始抓包；**
-开始新的抓包，一般用的也很少；
**-停止抓包，当你抓完包之后，就是点这个停止了；-清空当前已经抓到的数据包，可以防止抓包时间过长机器变卡；**
而实际上，一般我们只要知道上面加粗部分的按钮功能，就可以完成抓包了，剩下的就是如何抓你想要的数据包，如何分析的问题了。
![](http://img1.51cto.com/attachment/201112/100819162.png)
接下来说下抓包选项界面，也就是点第二个按钮出来的界面，同样，这里也只介绍最常用的几个功能，首先下图中最上面的红框是选择需要抓的网卡，选择好网卡后会在下面显示这个网卡的IP地址。
然后Capture Filter中就是要写抓包规则的地方，也叫做“过滤规则”，我们下面要说的很多规则都是要写到这个框里的，规则写好后，点下面的Start就开始抓包了。
![](http://img1.51cto.com/attachment/201112/100900619.png)
当抓包结束之后，如果你需要把抓到的数据包找其他人分析，那么可以点菜单上的file，然后点Save As保存抓到的数据包，如下图：
![](http://img1.51cto.com/attachment/201112/100932901.png)
ok，到这里，基础的使用方法说完了，接下来步入很关键的内容。
使用Wireshark时最常见的问题，是当您使用默认设置时，会得到大量冗余信息，以至于很难找到自己需要的部分。这就是为什么过滤器会如此重要。它们可以帮助我们在庞杂的结果中迅速找到我们需要的信息。
**过滤器的区别捕捉过滤器（CaptureFilters）：**用于决定将什么样的信息记录在捕捉结果中。需要在开始捕捉前设置。
**显示过滤器（DisplayFilters）：**在捕捉结果中进行详细查找。他们可以在得到捕捉结果后随意修改。
那么我应该使用哪一种过滤器呢？
**两种过滤器的目的是不同的。**
捕捉过滤器是数据经过的第一层过滤器，它用于控制捕捉数据的数量，以避免产生过大的日志文件。
显示过滤器是一种更为强大（复杂）的过滤器。它允许您在日志文件中迅速准确地找到所需要的记录。
两种过滤器使用的语法是完全不同的。
**捕捉过滤器**
![](http://img1.51cto.com/attachment/201112/101047482.png)
**Protocol（协议）:**
可能的值: ether, fddi, ip, arp, rarp, decnet, lat, sca, moprc, mopdl, tcp and udp.
如果没有特别指明是什么协议，则默认使用所有支持的协议。
**Direction（方向）:**
可能的值: src, dst, src and dst, src or dst
如果没有特别指明来源或目的地，则默认使用 “src or dst” 作为关键字。
例如，”host 10.2.2.2″与”src or dst host 10.2.2.2″是一样的。
**Host(s):**
可能的值： net, port, host, portrange.
如果没有指定此值，则默认使用”host”关键字。
例如，”src 10.1.1.1″与”src host 10.1.1.1″相同。
**Logical Operations（逻辑运算）:**
可能的值：not, and, or.
否(“not”)具有最高的优先级。或(“or”)和与(“and”)具有相同的优先级，运算时从左至右进行。
例如，
“not tcp port 3128 and tcp port 23″与”(not tcp port 3128) and tcp port 23″相同。
“not tcp port 3128 and tcp port 23″与”not (tcp port 3128 and tcp port 23)”不同。
例子：
**tcp dst port 3128** //捕捉目的TCP端口为3128的封包。
**ip src host 10.1.1.1** //捕捉来源IP地址为10.1.1.1的封包。
**host 10.1.2.3** //捕捉目的或来源IP地址为10.1.2.3的封包。
**ether host e0-05-c5-44-b1-3c** //捕捉目的或来源MAC地址为e0-05-c5-44-b1-3c的封包。如果你想抓本机与所有外网通讯的数据包时，可以将这里的mac地址换成路由的mac地址即可。
**src portrange 2000-2500** //捕捉来源为UDP或TCP，并且端口号在2000至2500范围内的封包。
**not imcp** //显示除了icmp以外的所有封包。（icmp通常被ping工具使用）
**src host 10.7.2.12 and not dst net 10.200.0.0/16** //显示来源IP地址为10.7.2.12，但目的地不是10.200.0.0/16的封包。
**(src host 10.4.1.12 or src net 10.6.0.0/16) and tcp dst portrange 200-10000 and dst net 10.0.0.0/8** //捕捉来源IP为10.4.1.12或者来源网络为10.6.0.0/16，目的地TCP端口号在200至10000之间，并且目的位于网络 10.0.0.0/8内的所有封包。
**src net 192.168.0.0/24 src net 192.168.0.0 mask 255.255.255.0** //捕捉源地址为192.168.0.0网络内的所有封包。
**注意事项：**
当使用关键字作为值时，需使用反斜杠“/”。
“ether proto /ip” (与关键字”ip”相同).
这样写将会以IP协议作为目标。
“ip proto /icmp” (与关键字”icmp”相同).
这样写将会以ping工具常用的icmp作为目标。
可以在”ip”或”ether”后面使用”multicast”及”broadcast”关键字。
当您想排除广播请求时，”no broadcast”就会非常有用。
**Protocol（协议）:**
您可以使用大量位于OSI模型第2至7层的协议。点击”Expression…”按钮后，您可以看到它们。
比如：IP，TCP，DNS，SSH
**String1, String2 (可选项):**
协议的子类。
点击相关父类旁的”+”号，然后选择其子类。
**Comparison operators （比较运算符）:**
可以使用6种比较运算符：
![](http://img1.51cto.com/attachment/201112/101202225.png)
**Logical e­xpressions（逻辑运算符）:**
![](http://img1.51cto.com/attachment/201112/101215970.png)
**显示过滤器**
![](http://img1.51cto.com/attachment/201112/101236176.png)
**例子：snmp** || **dns** || **icmp** //显示SNMP或DNS或ICMP封包。
**ip.addr == 10.1.1.1** //显示来源或目的IP地址为10.1.1.1的封包。
ip.src != 10.1.2.3 or ip.dst != 10.4.5.6 //显示来源不为10.1.2.3或者目的不为10.4.5.6的封包。
换句话说，显示的封包将会为：
来源IP：除了10.1.2.3以外任意；目的IP：任意
以及
来源IP：任意；目的IP：除了10.4.5.6以外任意
**ip.src != 10.1.2.3 and ip.dst != 10.4.5.6** //显示来源不为10.1.2.3并且目的IP不为10.4.5.6的封包。
换句话说，显示的封包将会为：
来源IP：除了10.1.2.3以外任意；同时须满足，目的IP：除了10.4.5.6以外任意
**tcp.port == 25** //显示来源或目的TCP端口号为25的封包。 
**tcp.dstport == 25** //显示目的TCP端口号为25的封包。 
**tcp.flags** //显示包含TCP标志的封包。 
**tcp.flags.syn == 0×02** //显示包含TCP SYN标志的封包。 
如果过滤器的语法是正确的，表达式的背景呈绿色。如果呈红色，说明表达式有误。
更为详细的说明请见：[http://openmaniak.com/cn/wireshark_filters.php](http://openmaniak.com/cn/wireshark_filters.php)
以上只是抓包和简单的过滤，那么其实如果你要想达到能够分析这些网络包的要求时，还需要了解下一些数据包的标记，比如我们常说的TCP三次握手是怎么回事？
**三次握手Three-way Handshake**
一个虚拟连接的建立是通过三次握手来实现的
**1. (Client) –> [SYN] –> (Server)**
假如Client和Server通讯. 当Client要和Server通信时，Client首先向Server发一个SYN (Synchronize) 标记的包，告诉Server请求建立连接.
注意: 一个 SYN包就是仅SYN标记设为1的TCP包(参见TCP包头Resources). 认识到这点很重要，只有当Server收到Client发来的SYN包，才可建立连接，除此之外别无他法。因此，如果你的防火墙丢弃所有的发往外网接口的SYN包，那么你将不 能让外部任何主机主动建立连接。
**2. (Client) <– [SYN/ACK] <–(Server)**
接着，Server收到来自Client发来的SYN包后，会发一个对SYN包的确认包(SYN/ACK)给Client，表示对第一个SYN包的确认，并继续握手操作.
注意: SYN/ACK包是仅SYN 和 ACK 标记为1的包.
**3. (Client) –> [ACK] –> (Server)**
Client收到来自Server的SYN/ACK 包,Client会再向Server发一个确认包(ACK)，通知Server连接已建立。至此，三次握手完成，一个TCP连接完成。
Note: ACK包就是仅ACK 标记设为1的TCP包. 需要注意的是当三此握手完成、连接建立以后，TCP连接的每个包都会设置ACK位。
这就是为何连接跟踪很重要的原因了. 没有连接跟踪,防火墙将无法判断收到的ACK包是否属于一个已经建立的连接.一般的包过滤(Ipchains)收到ACK包时,会让它通过(这绝对不是个 好主意). 而当状态型防火墙收到此种包时，它会先在连接表中查找是否属于哪个已建连接，否则丢弃该包。
**四次握手Four-way Handshake**
四次握手用来关闭已建立的TCP连接
1. (Client) –> ACK/FIN –> (Server)
2. (Client) <– ACK <– (Server)
3. (Client) <– ACK/FIN <– (Server)
4. (Client) –> ACK –> (Server)
注意: 由于TCP连接是双向连接, 因此关闭连接需要在两个方向上做。**ACK/FIN 包(ACK 和FIN 标记设为1)通常被认为是FIN(终结)包**.然而, 由于连接还没有关闭, FIN包总是打上ACK标记. 没有ACK标记而仅有FIN标记的包不是合法的包，并且通常被认为是恶意的。
**连接复位Resetting a connection**
四次握手不是关闭TCP连接的唯一方法. 有时,如果主机需要尽快关闭连接(或连接超时,端口或主机不可达),RST (Reset)包将被发送. 注意在，由于RST包不是TCP连接中的必须部分, 可以只发送RST包(即不带ACK标记). 但在正常的TCP连接中RST包可以带ACK确认标记
请注意RST包是可以不要收到方确认的?
无效的TCP标记Invalid TCP Flags
到目前为止，你已经看到了 SYN, ACK, FIN, 和RST 标记. 另外，还有PSH (Push) 和URG (Urgent)标记.
最常见的非法组合是SYN/FIN 包. 注意:由于 SYN包是用来初始化连接的, 它不可能和 FIN和RST标记一起出现. 这也是一个恶意攻击.
由于现在大多数防火墙已知 SYN/FIN 包, 别的一些组合,例如SYN/FIN/PSH, SYN/FIN/RST, SYN/FIN/RST/PSH。很明显，当网络中出现这种包时，很你的网络肯定受到攻击了。
别的已知的非法包有FIN (无ACK标记)和”NULL”包。如同早先讨论的，由于ACK/FIN包的出现是为了关闭一个TCP连接，那么正常的FIN包总是带有 ACK 标记。”NULL”包就是没有任何TCP标记的包(URG,ACK,PSH,RST,SYN,FIN都为0)。
到目前为止，正常的网络活动下，TCP协议栈不可能产生带有上面提到的任何一种标记组合的TCP包。当你发现这些不正常的包时，肯定有人对你的网络不怀好意。
**UDP (用户数据包协议User Datagram Protocol)**
TCP是面向连接的，而UDP是非连接的协议。UDP没有对接受进行确认的标记和确认机制。对丢包的处理是在应用层来完成的。(or accidental arrival).
此处需要重点注意的事情是：在正常情况下，当UDP包到达一个关闭的端口时，会返回一个UDP复位包。由于UDP是非面向连接的, 因此没有任何确认信息来确认包是否正确到达目的地。因此如果你的防火墙丢弃UDP包，它会开放所有的UDP端口(?)。
由于Internet上正常情况下一些包将被丢弃，甚至某些发往已关闭端口(非防火墙的)的UDP包将不会到达目的，它们将返回一个复位UDP包。
因为这个原因，UDP端口扫描总是不精确、不可靠的。
看起来大UDP包的碎片是常见的DOS (Denial of Service)攻击的常见形式 (这里有个DOS攻击的例子，[http://grc.com/dos/grcdos.htm](http://grc.com/dos/grcdos.htm) ).
ICMP (网间控制消息协议Internet Control Message Protocol)
如同名字一样， ICMP用来在主机/路由器之间传递控制信息的协议。 ICMP包可以包含诊断信息(ping, traceroute - 注意目前unix系统中的traceroute用UDP包而不是ICMP)，错误信息(网络/主机/端口 不可达 network/host/port unreachable), 信息(时间戳timestamp, 地址掩码address mask request, etc.)，或控制信息 (source quench, redirect, etc.) 。
你可以在[http://www.iana.org/assignments/icmp-parameters](http://www.iana.org/assignments/icmp-parameters) 中找到ICMP包的类型。
尽管ICMP通常是无害的，还是有些类型的ICMP信息需要丢弃。
Redirect (5), Alternate Host Address (6), Router Advertisement (9) 能用来转发通讯。
Echo (8), Timestamp (13) and Address Mask Request (17) 能用来分别判断主机是否起来，本地时间 和地址掩码。注意它们是和返回的信息类别有关的。 它们自己本身是不能被利用的，但它们泄露出的信息对攻击者是有用的。
ICMP消息有时也被用来作为DOS攻击的一部分(例如：洪水ping flood ping,死 ping ?呵呵，有趣 ping of death)?/p>
包碎片注意A Note About Packet Fragmentation
如果一个包的大小超过了TCP的最大段长度MSS (Maximum Segment Size) 或MTU (Maximum Transmission Unit)，能够把此包发往目的的唯一方法是把此包分片。由于包分片是正常的，它可以被利用来做恶意的攻击。
因为分片的包的第一个分片包含一个包头，若没有包分片的重组功能，包过滤器不可能检测附加的包分片。典型的攻击Typical attacks involve in overlapping the packet data in which packet header is 典型的攻击Typical attacks involve in overlapping the packet data in which packet header isnormal until is it overwritten with different
 destination IP (or port) thereby bypassing firewall rules。包分片能作为 DOS 攻击的一部分，它可以crash older IP stacks 或涨死CPU连接能力。
Netfilter/Iptables中的连接跟踪代码能自动做分片重组。它仍有弱点，可能受到饱和连接攻击，可以把CPU资源耗光。
OK，到此为止，关于Wireshark抓包工具的一些小教程已经写完了，而导致我想写这么一个纠结的教程的原因是，前几天通过这个抓包解决了梦幻西游在网维大师无盘上容易掉线的问题，当时捕捉到梦幻西游掉线时的数据包是这样的。
注意下图中的红色数据，123.58.184.241是梦幻西游的服务器，而192.168.1.41是玩梦幻西游的客户机，在掉线时，发现是先有梦幻西游的服务器向客户机发送一个[FIN,ACK]数据包，根据上面的解释，FIN标记的数据包是代表要断开连接的意思，而接着客户机又回给服务器一个确认断开链接包。当看到这个抓包数据时，就意识到，大家说的在网维大师系统虚拟盘上梦幻爱掉线的问题，并非普通的网络问题，因为通过数据包的信息来看，是梦幻服务器主动要求断开链接，产生这个情况无非是以下几个原因：
1、服务器发现客户端非法，比如有外挂什么的，踢掉了客户机；
2、服务器压力大，踢掉了客户机；
3、总之不是客户端问题导致的掉线；
![](http://img1.51cto.com/attachment/201112/101341578.png)
那么既然结论是如此，为什么会有在网维大师系统虚拟盘上容易出现梦幻掉线问题呢？原因是由于网维大师系统虚拟盘是模拟真实硬盘方式来实现的，而在模拟过程中，将硬盘的序列号设置为固定过的OSDIY888了，而梦幻西游刚好后识别客户机硬盘信息，发现大量客户端的硬盘序列号都是一样的，就认为是作弊或者使用挂机外挂了，结果就导致随机被服务器踢下线的情况发生，后来我们将硬盘序列号设置为空，则没再出现该问题。这个问题在未来的新版本中会解决掉。
说这个案例的目的并不是为了说明抓包多有用，而是想说明一些解决问题的思路和方法，有些人是有思路，但是缺方法，比如不会用工具，而有些人收集了很多工具却不会用，而我其实就属于后者，几年前就收集了n多工具，但是用到的没几个。慢慢的学会用这些工具后，发现思维+工具，解决问题是效率暴增，接下来的几天里，会陆续介绍写小工具给大家，也希望大家有空学习下，有问题先百度，再自己摸索，而不是一味的求助，毕竟求人不如求己！自己能直接搞定，是皆大欢喜的事情~
**注意：由于某些系统为了防止ARP攻击，都免疫掉了一个Npptools.dll文件，这会导致该软件无法正常安装，打下这个补丁就可以了。**
[wireshark的使用教程](http://apps.hi.baidu.com/share/detail/24512155)
wireshark是一款抓包软件，比较易用，在平常可以利用它抓包，分析协议或者监控网络，是一个比较好的工具，因为最近在研究这个，所以就写一下教程，方便大家学习。
       这里先说Wireshark的启动界面和抓包界面
       启动界面：
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/f88acdd117125104970a16b0.jpg)
抓包界面的启动是
按file下的按钮
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/ccd1b38ad23097a4a4c272b1.jpg)
之后会出现
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/908ff8143bf5bf61f3de32b2.jpg)
这个是网卡的显示，因为我有虚拟机所以会显示虚拟网卡，我们现在抓的是真实网卡上的包所以在以太网卡右边点击start 开始抓包
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/9414003aacc6811397ddd8bc.jpg)
这个就是抓包的界面了（也是主界面）
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/b56c7f4ed51bc9c9d72afcbf.jpg)
Wireshark主窗口由如下部分组成：
- 
菜单——用于开始操作。
- 
主工具栏——提供快速访问菜单中经常用到的项目的功能。
- 
Fiter toolbar/过滤工具栏——提供处理当前显示过滤得方法。
- 
Packet List面板——显示打开文件的每个包的摘要。点击面板中的单独条目，包的其他情况将会显示在另外两个面板中。
- 
Packet detail面板——显示您在Packet list面板中选择的包的更多详情。
- 
Packet bytes面板——显示您在Packet list面板选择的包的数据，以及在Packet details面板高亮显示的字段。
- 
状态栏——显示当前程序状态以及捕捉数据的更多详情。
**1.菜单栏**
主菜单包括以下几个项目:
File ——包括打开、合并捕捉文件，save/保存,Print/打印,Export/导出捕捉文件的全部或部分。以及退出Wireshark项.
- 
- 
Edit ——包括如下项目：查找包，时间参考，标记一个多个包，设置预设参数。（剪切，拷贝，粘贴不能立即执行。）
- 
- 
View ——控制捕捉数据的显示方式，包括颜色，字体缩放，将包显示在分离的窗口，展开或收缩详情面版的地树状节点
- 
- 
GO ——包含到指定包的功能。
- 
- 
Analyze ——包含处理显示过滤，允许或禁止分析协议，配置用户指定解码和追踪TCP流等功能。
- 
- 
Statistics ——包括的菜单项用户显示多个统计窗口，包括关于捕捉包的摘要，协议层次统计等等。
- 
- 
- Help ——包含一些辅助用户的参考内容。如访问一些基本的帮助文件，支持的协议列表，用户手册。在线访问一些网站，“关于”
- 
- 
- 
- **2.工具栏（略）**
- 
- 
- 
- 
- 
- 
- 
- 
- 
**3.过滤工具栏**
- 
- 
- ![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/f88acdd1172c5104970a169e.jpg)
- 
- 
- 
- 
- 
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/f3a03c80148ac0920df4d260.jpg)会弹出对话框
- ![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/abpic/item/a566d7130f53a04ecb80c463.jpg)
- 
- 这个和在工具栏上输入协议来查找包的结果是一样的，只是它方便点
- 
- 在工具栏上输![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/25606fd33e9ae2e3562c846e.jpg)
- 
- 
点击在此区域输入或修改显示的过滤字符，在输入过程中会进行语法检查。如果您输入的格式不正确，或者未输入完成，则背景显示为红色。直到您输入合法的表达式，背景会变为绿色。你可以点击下拉列表选择您先前键入的过滤字符。列表会一直保留，即使您重新启动程序。
注意：- - 做完修改之后，记得点击右边的Apply(应用)按钮，或者回车，以使过滤生效。
- 输入框的内容同时也是当前过滤器的内容（当前过滤器的内容会反映在输入框）
**4.封包列表**![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/a36eca533b93a4bf8d543070.jpg)
- - 
![wireshark packet filter pane](http://openmaniak.com/wireshark/wireshark_packet-list-pane2_small.png)
封包列表中显示所有已经捕获的封包。在这里您可以看到发送或接收方的MAC/IP地址，TCP/UDP端口号，协议或者封包的内容。
如果捕获的是一个OSI layer 2的封包，您在Source（来源）和Destination（目的地）列中看到的将是MAC地址，当然，此时Port（端口）列将会为空。
如果捕获的是一个OSI layer 3或者更高层的封包，您在Source（来源）和Destination（目的地）列中看到的将是IP地址。Port（端口）列仅会在这个封包属于第4或者更高层时才会显示。
您可以在这里添加/删除列或者改变各列的颜色：
Edit menu -> Preferences
**5.封包详细信息**
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/224de812b3b92931203f2e73.jpg)
这里显示的是在封包列表中被选中项目的详细信息。
信息按照不同的OSI layer进行了分组，您可以展开每个项目查看。
ps：wireshark会用不难，难的是会看懂这些包
（如果对这些包头信息不了解的可以自己去查查资料）
**6. 16进制数据**
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/a9a2cac149621a65b219a87e.jpg)
“解析器”在Wireshark中也被叫做“16进制数据查看面板”。这里显示的内容与“封包详细信息”中相同，只是改为以16进制的格式表述。
在这里我们还要学会在大量的信息中**过滤**和**分析**我们需要的信息
过滤：
过滤器会如此重要。它们可以帮助我们在庞杂的结果中迅速找到我们需要的信息。
- 
|--|捕捉过滤器：用于决定将什么样的信息记录在捕捉结果中。需要在开始捕捉前设置。显示过滤器：在捕捉结果中进行详细查找。他们可以在得到捕捉结果后随意修改。|
两种过滤器的目的是不同的。
1.捕捉过滤器是数据经过的第一层过滤器，它用于控制捕捉数据的数量，以避免产生过大的日志文件。
2.显示过滤器是一种更为强大（复杂）的过滤器。它允许您在日志文件中迅速准确地找到所需要的记录。
**1.捕捉过滤器**
捕捉过滤器的语法与其它使用Lipcap（Linux）或者Winpcap（Windows）库开发的软件一样，比如著名的TCPdump。捕捉过滤器必须在开始捕捉前设置完毕，这一点跟显示过滤器是不同的。
设置捕捉过滤器的步骤是：
- 选择 capture -> options。
- 填写"capture filter"栏或者点击"capture filter"按钮为您的过滤器起一个名字并保存，以便在今后的捕捉中继续使用这个过滤器。
- 点击开始（Start）进行捕捉。
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/ca7a951df77e3dc5e1fe0b7a.jpg)
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/5c9c1ef6518672e9f3d3857a.jpg)
|语法：||**Protocol**||**Direction**||**Host(s)**||**Value**||**Logical Operations**||**Other expression_r**|
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|例子：||tcp||dst||10.1.1.1||80||and||tcp dst 10.2.2.2 3128|
![](http://openmaniak.com/image/droite.gif)**Protocol（协议）**:
可能的值: ether, fddi, ip, arp, rarp, decnet, lat, sca, moprc, mopdl, tcp and udp.
如果没有特别指明是什么协议，则默认使用所有支持的协议。
![](http://openmaniak.com/image/droite.gif)**Direction（方向）**:
可能的值: src, dst, src and dst, src or dst
如果没有特别指明来源或目的地，则默认使用 "src or dst" 作为关键字。
例如，"host 10.2.2.2"与"src or dst host 10.2.2.2"是一样的。
![](http://openmaniak.com/image/droite.gif)**Host(s)**:
可能的值： net, port, host, portrange.
如果没有指定此值，则默认使用"host"关键字。
例如，"src 10.1.1.1"与"src host 10.1.1.1"相同。
![](http://openmaniak.com/image/droite.gif)**Logical Operations（逻辑运算）**:
可能的值：not, and, or.
否("not")具有最高的优先级。或("or")和与("and")具有相同的优先级，运算时从左至右进行。
例如，
"not tcp port 3128 and tcp port 23"与"(not tcp port 3128) and tcp port 23"相同。
"not tcp port 3128 and tcp port 23"与"not (tcp port 3128 and tcp port 23)"不同。
例子：
**tcp dst port 3128**
显示目的TCP端口为3128的封包。
**ip src host 10.1.1.1**
显示来源IP地址为10.1.1.1的封包。
**host 10.1.2.3**
显示目的或来源IP地址为10.1.2.3的封包。
**src portrange 2000-2500**
显示来源为UDP或TCP，并且端口号在2000至2500范围内的封包。
**not imcp**
显示除了icmp以外的所有封包。（icmp通常被ping工具使用）
**src host 10.7.2.12 and not dst net 10.200.0.0/16**
显示来源IP地址为10.7.2.12，但目的地不是10.200.0.0/16的封包。
**(src host 10.4.1.12 or src net 10.6.0.0/16) and tcp dst portrange 200-10000 and dst net 10.0.0.0/8**
显示来源IP为10.4.1.12或者来源网络为10.6.0.0/16，目的地TCP端口号在200至10000之间，并且目的位于网络10.0.0.0/8内的所有封包。
注意事项：
当使用关键字作为值时，需使用反斜杠“\”。
"ether proto \ip" (与关键字"ip"相同).
这样写将会以IP协议作为目标。
"ip proto \icmp" (与关键字"icmp"相同).
这样写将会以ping工具常用的icmp作为目标。
可以在"ip"或"ether"后面使用"multicast"及"broadcast"关键字。
当您想排除广播请求时，"no broadcast"就会非常有用。
**![](http://openmaniak.com/image/carre.gif) 2. 显示过滤器**
通常经过捕捉过滤器过滤后的数据还是很复杂。此时您可以使用显示过滤器进行更加细致的查找。
它的功能比捕捉过滤器更为强大，而且在您想修改过滤器条件时，并不需要重新捕捉一次。
|语法：||**Protocol**|**.**|**String 1**|**.**|**String 2**||**Comparisonoperator**||**Value**||**LogicalOperations**||**Otherexpression_r**|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|例子：||ftp||passive||ip||==||10.2.3.4||xor||icmp.type|
![](http://openmaniak.com/image/droite.gif)**Protocol（协议）**:
您可以使用大量位于OSI模型第2至7层的协议。点击"Expression..."按钮后，您可以看到它们。
比如：IP，TCP，DNS，SSH
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/e8945ff1aee3f960342acc4f.jpg)
![](http://openmaniak.com/image/droite.gif)**String1, String2** (可选项):
协议的子类。
点击相关父类旁的"》"号，然后选择其子类。
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/bb498213518866f6a6ef3f4a.jpg)
![](http://openmaniak.com/image/droite.gif)**Comparison operators （比较运算符）**:
可以使用6种比较运算符：
|英文写法：|C语言写法：|含义：|
|----|----|----|
|eq|==|等于|
|ne|!=|不等于|
|gt|>|大于|
|lt|<|小于|
|ge|>=|大于等于|
|le|<=|小于等于|
![](http://openmaniak.com/image/droite.gif)**Logical expression_rs（逻辑运算符）**:
|英文写法：|C语言写法：|含义：|
|----|----|----|
|and|&&|逻辑与|
|or||||逻辑或|
|xor|^^|逻辑异或|
|not|!|逻辑非|
被程序员们熟知的逻辑异或是一种排除性的或。当其被用在过滤器的两个条件之间时，只有当且仅当其中的一个条件满足时，这样的结果才会被显示在屏幕上。
让我们举个例子：
"tcp.dstport 80 xor tcp.dstport 1025"
只有当目的TCP端口为80或者来源于端口1025（但又不能同时满足这两点）时，这样的封包才会被显示。
例子：
|**snmp || dns || icmp**|显示SNMP或DNS或ICMP封包。|
**ip.addr == 10.1.1.1**
显示来源或目的IP地址为10.1.1.1的封包。
**ip.src != 10.1.2.3 or ip.dst != 10.4.5.6**
显示来源不为10.1.2.3或者目的不为10.4.5.6的封包。
换句话说，显示的封包将会为：
来源IP：除了10.1.2.3以外任意；目的IP：任意
以及
来源IP：任意；目的IP：除了10.4.5.6以外任意
**ip.src != 10.1.2.3 and ip.dst != 10.4.5.6**
显示来源不为10.1.2.3并且目的IP不为10.4.5.6的封包。
换句话说，显示的封包将会为：
来源IP：除了10.1.2.3以外任意；同时须满足，目的IP：除了10.4.5.6以外任意
|**tcp.port == 25**|显示来源或目的TCP端口号为25的封包。|
|**tcp.dstport == 25**|显示目的TCP端口号为25的封包。|
|**tcp.flags**|显示包含TCP标志的封包。|
|**tcp.flags.syn == 0x02**|显示包含TCP SYN标志的封包。|
如果过滤器的语法是正确的，表达式的背景呈绿色。如果呈红色，说明表达式有误。
**分析：**
**"Follow TCP Stream"**
如果你处理TCP协议，想要查看Tcp流中的应用层数据，"Following TCP streams"功能将会很有用。如果你项查看telnet流中的密码，或者你想尝试弄明白一个数据流。或者你仅仅只需要一个显示过滤来显示某个TCP流的包。这些都可以通过Wireshark的"Following TCP streams"功能来实现。
在包列表中选择一个你感兴趣的TCP包，然后选择Wireshark工具栏菜单的"Following TCP Streams"选项(或者使用包列表鼠标右键的上下文菜单)。然后，Wireshark就会创建合适的显示过滤器，并弹出一个对话框显示TCP流的所有数据。
### "Follow TCP Stream"对话框
![](http://hiphotos.baidu.com/jisuanji%B0%AE%BA%C3%D5%DF/pic/item/a297f747d3e6c40f86947350.jpg)
流的内容出现的顺序同他们在网络中出现的顺序一致。从A到B的通信标记为红色，从B到A的通信标记为蓝色。当然，如果你喜欢的话你可以从"Edit/Preferences"菜单项的"Colores"修改颜色。
非打印字符将会被显示为圆点。XXX - What about line wrapping (maximum line length) and CRNL conversions?
在捕捉过程中，TCP流不能实时更新。想得到最近的内容需要重新打开对话框。
你可以在此对话框执行如下操作：
- 
Save As 以当前选择格式保存流数据。
- 
Print 以当前选择格式打印流数据。
- 
Direction 选择流的显示方向("Entire conversation", "data from A to B only" or "data from B to A only").
- 
Filter out this stream 应用一个显示过滤，在显示中排除当前选择的TCP流。
- 
Close 关闭当前对话框。移除对当前显示过滤的影响。
你可以用以下格式浏览流数据。
- 
AsCII。在此视图下你可以以ASCII凡是查看数据。当然最适合基于ASCII的协议用，例如HTTP.
- 
EBCDIC。For the big-iron freaks out there.（不知道这句是什么意思， EBCDIC 是IBM公司的字符二进制编码标准。）
- 
HEX Dump. 允许你查看所有数据，可能会占用大量屏幕空间。适合显示二进制协议。
- 
C Arrays. 允许你将流数据导入你自己的C语言程序。
- 
RAW。 允许你载入原始数据到其他应用程序做进一步分析。显示类似与ASCII设置。但“save As”将会保存为二进制文件
