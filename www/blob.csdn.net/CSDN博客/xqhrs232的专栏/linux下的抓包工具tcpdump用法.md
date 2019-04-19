# linux下的抓包工具tcpdump用法 - xqhrs232的专栏 - CSDN博客
2016年04月01日 17:24:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：765
原文地址::[http://blog.chinaunix.net/uid-11140746-id-2903933.html](http://blog.chinaunix.net/uid-11140746-id-2903933.html)
相关文章
1、[Linux下抓包工具tcpdump应用详解](http://blog.csdn.net/tanqiuwei/article/details/7273402)----[http://blog.csdn.net/tanqiuwei/article/details/7273402](http://blog.csdn.net/tanqiuwei/article/details/7273402)
2、[Linux tcpdump命令详解](http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html)----[http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html](http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html)
3、Linux命令行下抓包工具tcpdump的使用----[http://jingyan.baidu.com/article/295430f1d66e800c7e00503f.html](http://jingyan.baidu.com/article/295430f1d66e800c7e00503f.html)
**linux下的抓包工具tcpdump用法**
网络数据采集分析工具TcpDump的简介
顾名思义，TcpDump可以将网络中传送的数据包的“头”完全截获下来提供分析。它支持针对网络层、协议、主机、网络或端口的过滤，并提供and、or、not等逻辑语句来帮助你去掉无用的信息。tcpdump就是一种免费的网络分析工具，尤其其提供了源代码，公开了接口，因此具备很强的可扩展性，对于网络维护和入侵者都是非常有用的工具。tcpdump存在于基本的FreeBSD系统中，由于它需要将网络界面设置为混杂模式，普通用户不能正常执行，但具备root权限的用户可以直接执行它来获取网络上的信息。因此系统中存在网络分析工具主要不是对本机安全的威胁，而是对网络上的其他计算机的安全存在威胁。
我们用尽量简单的话来定义tcpdump，就是：dump
 the traffice on a network.，根据使用者的定义对网络上的数据包进行截获的包分析工具。作为互联网上经典的的系统管理员必备工具，tcpdump以其强大的功能，灵活的截取策略，成为每个高级的系统管理员分析网络，排查问题等所必备的东西之一。tcpdump提供了源代码，公开了接口，因此具备很强的可扩展性，对于网络维护和入侵者都是非常有用的工具。tcpdump存在于基本的FreeBSD系统中，由于它需要将网络界面设置为混杂模式，普通用户不能正常执行，但具备root权限的用户可以直接执行它来获取网络上的信息。因此系统中存在网络分析工具主要不是对本机安全的威胁，而是对网络上的其他计算机的安全存在威胁。
网络数据采集分析工具TcpDump的安装
在linux下tcpdump的安装十分简单，一般由两种安装方式。一种是以rpm包的形式来进行安装。另外一种是以源程序的形式安装。
rpm包的形式安装：这种形式的安装是最简单的安装方法，rpm包是将软件编译后打包成二进制的格式，通过rpm命令可以直接安装，不需要修改任何东西。以超级用户登录，使用命令如下：
#rpm -ivh tcpdump-3_4a5.rpm
这样tcpdump就顺利地安装到你的linux系统中。怎么样，很简单吧。
源程序的安装：既然rpm包的安装很简单,为什么还要采用比较复杂的源程序安装呢?其实，linux一个最大的诱人之处就是在她上面有很多软件是提供源程序的，人们可以修改源程序来满足自己的特殊的需要。所以我特别建议朋友们都采取这种源程序的安装方法。
第一步取得源程序在源程序的安装方式中，我们首先要取得tcpdump的源程序分发包，这种分发包有两种形式，一种是tar压缩包(tcpdump-3_4a5.tar.Z),另一种是rpm的分发包(tcpdump-3_4a5.src.rpm)。这两种形式的内容都是一样的，不同的仅仅是压缩的方式.tar的压缩包可以使用如下命令解开：
#tar xvfz tcpdump-3_4a5.tar.Z
rpm的包可以使用如下命令安装:
#rpm -ivh tcpdump-3_4a5.src.rpm
这样就把tcpdump的源代码解压到/usr/src/redhat/SOURCES目录下.
第二步做好编译源程序前的准备活动
在编译源程序之前，最好已经确定库文件libpcap已经安装完毕，这个库文件是tcpdump软件所需的库文件。同样，你同时还要有一个标准的c语言编译器。在linux下标准的c 语言编译器一般是gcc。在tcpdump的源程序目录中。有一个文件是Makefile.in，configure命令就是从Makefile.in文件中自动产生Makefile文件。在Makefile.in文件中，可以根据系统的配置来修改BINDEST 和 MANDEST 这两个宏定义，缺省值是
BINDEST = @sbindir@
MANDEST = @mandir@
第一个宏值表明安装tcpdump的二进制文件的路径名，第二个表明tcpdump的man 帮助页的路径名,你可以修改它们来满足系统的需求。
第三步编译源程序
使用源程序目录中的configure脚本，它从系统中读出各种所需的属性。并且根据Makefile.in文件自动生成Makefile文件，以便编译使用.make 命令则根据Makefile文件中的规则编译tcpdump的源程序。使用make
 install命令安装编译好的tcpdump的二进制文件。
总结一下就是:
# tar xvfz tcpdump-3_4a5.tar.Z
# vi Makefile.in
# . /configure
# make
# make install
3 网络数据采集分析工具TcpDump的使用
普通情况下，直接启动tcpdump将监视第一个网络界面上所有流过的数据包。
# tcpdump
tcpdump: listening on fxp0
11:58:47.873028 202.102.245.40.netbios-ns > 202.102.245.127.netbios-ns: udp 50
11:58:47.974331 0:10:7b:8:3a:56 > 1:80:c2:0:0:0 802.1d ui/C len=43
0000 0000 0080 0000 1007 cf08 0900 0000
0e80 0000 902b 4695 0980 8701 0014 0002
000f 0000 902b 4695 0008 00
11:58:48.373134 0:0:e8:5b:6d:85 > Broadcast sap e0 ui/C len=97
ffff 0060 0004 ffff ffff ffff ffff ffff
0452 ffff ffff 0000 e85b 6d85 4008 0002
0640 4d41 5354 4552 5f57 4542 0000 0000
0000 00
^C
tcpdump支持相当多的不同参数，如使用-i参数指定tcpdump监听的网络界面，这在计算机具有多个网络界面时非常有用，使用-c参数指定要监听的数据包数量，使用-w参数指定将监听到的数据包写入文件中保存，等等。
然而更复杂的tcpdump参数是用于过滤目的，这是因为网络中流量很大，如果不加分辨将所有的数据包都截留下来，数据量太大，反而不容易发现需要的数据包。使用这些参数定义的过滤规则可以截留特定的数据包，以缩小目标，才能更好的分析网络中存在的问题。tcpdump使用参数指定要监视数据包的类型、地址、端口等，根据具体的网络问题，充分利用这些过滤规则就能达到迅速定位故障的目的。请使用man
 tcpdump查看这些过滤规则的具体用法。
显然为了安全起见，不用作网络管理用途的计算机上不应该运行这一类的网络分析软件，为了屏蔽它们，可以屏蔽内核中的bpfilter伪设备。一般情况下网络硬件和TCP/IP堆栈不支持接收或发送与本计算机无关的数据包，为了接收这些数据包，就必须使用网卡的混杂模式，并绕过标准的TCP/IP堆栈才行。在FreeBSD下，这就需要内核支持伪设备bpfilter。因此，在内核中取消bpfilter支持，就能屏蔽tcpdump之类的网络分析工具。
并且当网卡被设置为混杂模式时，系统会在控制台和日志文件中留下记录，提醒管理员留意这台系统是否被用作攻击同网络的其他计算机的跳板。
May 15 16:27:20 host1 /kernel: fxp0: promiscuous mode enabled
虽然网络分析工具能将网络中传送的数据记录下来，但是网络中的数据流量相当大，如何对这些数据进行分析、分类统计、发现并报告错误却是更关键的问题。网络中的数据包属于不同的协议，而不同协议数据包的格式也不同。因此对捕获的数据进行解码，将包中的信息尽可能的展示出来，对于协议分析工具来讲更为重要。昂贵的商业分析工具的优势就在于它们能支持很多种类的应用层协议，而不仅仅只支持tcp、udp等低层协议。
从上面tcpdump的输出可以看出，tcpdump对截获的数据并没有进行彻底解码，数据包内的大部分内容是使用十六进制的形式直接打印输出的。显然这不利于分析网络故障，通常的解决办法是先使用带-w参数的tcpdump 截获数据并保存到文件中，然后再使用其他程序进行解码分析。当然也应该定义过滤规则，以避免捕获的数据包填满整个硬盘。FreeBSD提供的一个有效的解码程序为tcpshow，它可以通过Packages
 Collection来安装。
# pkg_add /cdrom/packages/security/tcpshow*
# tcpdump -c 3 -w tcpdump.out
tcpdump: listening on fxp0
# tcpshow < tcpdump.out
---------------------------------------------------------------------------
Packet 1
TIME:12:00:59.984829
LINK:00:10:7B:08:3A:56 -> 01:80:C2:00:00:00 type=0026
<*** No decode support for encapsulated protocol ***>
---------------------------------------------------------------------------
Packet 2
TIME:12:01:01.074513 (1.089684)
LINK:00:A0:C9:AB:3C:DF -> FF:FF:FF:FF:FF:FF type=ARP
ARP:htype=Ethernet ptype=IP hlen=6 plen=4 op=request
sender-MAC-addr=00:A0:C9:AB:3C:DF sender-IP-address=202.102.245.3
target-MAC-addr=00:00:00:00:00:00 target-IP-address=202.102.245.3
---------------------------------------------------------------------------
Packet 3
TIME:12:01:01.985023 (0.910510)
LINK:00:10:7B:08:3A:56 -> 01:80:C2:00:00:00 type=0026
<*** No decode support for encapsulated protocol ***>
tcpshow能以不同方式对数据包进行解码，并以不同的方式显示解码数据，使用者可以根据其手册来选择最合适的参数对截获的数据包进行分析。从上面的例子中可以看出，tcpshow支持的协议也并不丰富，对于它不支持的协议就无法进行解码。
除了tcpdump之外，FreeBSD的Packages
 Collecion中还提供了Ethereal和Sniffit两个网络分析工具，以及其他一些基于网络分析方式的安全工具。其中Ethereal运行在X
 Window 下，具有不错的图形界面，Sniffit使用字符窗口形式，同样也易于操作。然而由于tcpdump对过滤规则的支持能力更强大，因此系统管理员仍然更喜欢使用它。对于有经验的网络管理员，使用这些网络分析工具不但能用来了解网络到底是如何运行的，故障出现在何处，还能进行有效的统计工作，如那种协议产生的通信量占主要地位，那个主机最繁忙，网络瓶颈位于何处等等问题。因此网络分析工具是用于网络管理的宝贵系统工具。为了防止数据被滥用的网络分析工具截获，关键还是要在网络的物理结构上解决。常用的方法是使用交换机或网桥将信任网络和不信任网络分隔开，可以防止外部网段窃听内部数据传输，但仍然不能解决内部网络与外部网络相互通信时的数据安全问题。如果没有足够的经费将网络上的共享集线器升级为以太网交换机，可以使用FreeBSD系统执行网桥任务。这需要使用option
 BRIDGE编译选项重新定制内核，此后使用bridge命令启动网桥功能。
tcpdump采用命令行方式，它的命令格式为：
tcpdump [ -adeflnNOpqStvx ] [ -c 数量 ]
 [ -F 文件名 ]
[ -i 网络接口 ]
 [ -r 文件名] [ -s snaplen ]
[ -T 类型 ] [
 -w 文件名 ] [表达式 ]
(1). tcpdump的选项介绍
-a 　　　将网络地址和广播地址转变成名字；
-d 　　　将匹配信息包的代码以人们能够理解的汇编格式给出；
-dd 将匹配信息包的代码以c语言程序段的格式给出；
-ddd 　　将匹配信息包的代码以十进制的形式给出；
-e 　　　在输出行打印出数据链路层的头部信息；
-f 　　　将外部的Internet地址以数字的形式打印出来；
-l 　　　使标准输出变为缓冲行形式；
-n 　　　不把网络地址转换成名字；
-t 　　　在输出的每一行不打印时间戳；
-v 　　　输出一个稍微详细的信息，例如在ip包中可以包括ttl和服务类型的信息；
-vv 输出详细的报文信息；
-c 　　　在收到指定的包的数目后，tcpdump就会停止；
-F 　　　从指定的文件中读取表达式,忽略其它的表达式；
-i 　　　指定监听的网络接口；
-r 　　　从指定的文件中读取包(这些包一般通过-w选项产生)；
-w 　　　直接将包写入文件中，并不分析和打印出来；
-T 　　　将监听到的包直接解释为指定的类型的报文，常见的类型有rpc （远程过程调用）和snmp（简单网络管理协议；）
(2). tcpdump的表达式介绍
表达式是一个正则表达式，tcpdump利用它作为过滤报文的条件，如果一个报文满足表达式的条件，则这个报文将会被捕获。如果没有给出任何条件，则网络上所有的信息包将会被截获。在表达式中一般如下几种类型的关键字。
第一种是关于类型的关键字，主要包括host，net，port,例如 host
 210.27.48.2，指明 210.27.48.2是一台主机，net
 202.0.0.0 指明 202.0.0.0是一个网络地址，port
 23 指明端口号是23。如果没有指定类型，缺省的类型是host.
第二种是确定传输方向的关键字，主要包括src
 , dst ,dst or src, dst and src ,这些关键字指明了传输的方向。举例说明，src 210.27.48.2 ,指明ip包中源地址是210.27.48.2
 , dst net 202.0.0.0 指明目的网络地址是202.0.0.0 。如果没有指明方向关键字，则缺省是src
 or dst关键字。
第三种是协议的关键字，主要包括fddi,ip,arp,rarp,tcp,udp等类型。Fddi指明是在FDDI(分布式光纤数据接口网络)上的特定的网络协议，实际上它是"ether"的别名，fddi和ether具有类似的源地址和目的地址，所以可以将fddi协议包当作ether的包进行处理和分析。其他的几个关键字就是指明了监听的包的协议内容。如果没有指定任何协议，则tcpdump将会监听所有协议的信息包。
除了这三种类型的关键字之外，其他重要的关键字如下：gateway,
 broadcast,less,greater,还有三种逻辑运算，取非运算是 'not ' '! ', 与运算是'and','&&';或运算是'or'
 ,'││'；这些关键字可以组合起来构成强大的组合条件来满足人们的需要，下面举几个例子来说明。
A想要截获所有210.27.48.1 的主机收到的和发出的所有的数据包：
#tcpdump host 210.27.48.1
B想要截获主机210.27.48.1 和主机210.27.48.2 或210.27.48.3的通信，使用命令：（在命令行中适用　　　括号时，一定要
#tcpdump host 210.27.48.1 and \ (210.27.48.2 or 210.27.48.3 \)
C如果想要获取主机210.27.48.1除了和主机210.27.48.2之外所有主机通信的ip包，使用命令：
#tcpdump ip host 210.27.48.1 and ! 210.27.48.2
D如果想要获取主机210.27.48.1接收或发出的telnet包，使用如下命令：
#tcpdump tcp port 23 host 210.27.48.1
(3). tcpdump的输出结果介绍
下面我们介绍几种典型的tcpdump命令的输出信息
**A）数据链路层头信息**
使用命令
#tcpdump --e host ice
ice 是一台装有linux的主机，她的MAC地址是0：90：27：58：AF：1A
H219是一台装有SOLARIC的SUN工作站，它的MAC地址是8：0：20：79：5B：46；上一条命令的输出结果如下所示：
21:50:12.847509 eth0 < 8:0:20:79:5b:460:90:27:58:af:1a ip
 60: h219.33357 > ice.telnet 0:0(0) ack 22535 win 8760 (DF)
分析：21：50：12是显示的时间， 847509是ID号，eth0
 <表示从网络接口eth0 接受该数据包，eth0
 >表示从网络接口设备发送数据包, 8:0:20:79:5b:46是主机H219的MAC地址,它表明是从源地址H219发来的数据包.
 0:90:27:58:af:1a是主机ICE的MAC地址,表示该数据包的目的地址是ICE
 . ip 是表明该数据包是IP数据包,60 是数据包的长度,
 h219.33357 > ice.telnet 表明该数据包是从主机H219的33357端口发往主机ICE的TELNET(23)端口.
 ack 22535 表明对序列号是222535的包进行响应.
 win 8760表明发送窗口的大小是8760.
**B）ARP包的TCPDUMP输出信息**
http://anheng.com.cn/news/24/586.html
使用命令
#tcpdump arp
得到的输出结果是：
22:32:42.802509 eth0 > arp who-has route tell ice (0:90:27:58:af:1a)
22:32:42.802902 eth0 < arp reply route is-at 0:90:27:12:10:66 (0:90:27:58:af:1a)
分析: 22:32:42是时间戳,
 802509是ID号, eth0 >表明从主机发出该数据包,
 arp表明是ARP请求包, who-has route
 tell ice表明是主机ICE请求主机ROUTE的MAC地址。 0:90:27:58:af:1a是主机ICE的MAC地址。
**C）TCP包的输出信息**
用TCPDUMP捕获的TCP包的一般输出信息是：
src > dst: flags data-seqno ack window urgent options
src > dst:表明从源地址到目的地址,
 flags是TCP包中的标志信息,S 是SYN标志,
 F (FIN), P (PUSH) , R (RST) "." (没有标记); data-seqno是数据包中的数据的顺序号,
 ack是下次期望的顺序号, window是接收缓存的窗口大小,
 urgent表明数据包中是否有紧急指针. Options是选项.
**D）UDP包的输出信息**
用TCPDUMP捕获的UDP包的一般输出信息是：
route.port1 > ice.port2: udp lenth
UDP十分简单，上面的输出行表明从主机ROUTE的port1端口发出的一个UDP数据包到主机ICE的port2端口，类型是UDP，包的长度是lenth
由于ethereal改名为wirshark项目，所以两个版本的用法是一致的。
wireshark工具非常强大，它除了GUI工具外还有对应的命令行工具──tshark。在远程环境下，它还是相当实用的。下面就简单的说说常用的一些功能。
以下就以tshark为例。
tshark 平常远程登陆时抓包很好用，而且可以直接解析，但总是不很记得如何打印出原本16进制的数据。
抓包命令
tshark -wfilename -i ethx -q
-w 将抓包的数据写入文件filename中。
-i 指定要抓包的接口名称
-q 安静，在远程时最有用，否则会抓到你自己SSH的报文
读包命令
tshark -rfilename -x -V
-r 指定要读取的包文件
-x 将16进制原始包数据打印出来
-V 将包尽可能的解析（这个有时在包数量很多的情况下可以不使用，这样它会给出一个很简洁的报文解释）
然后用windows下的winshark慢慢看
**linux下的抓包工具****tcpdump**
[上一篇](http://www.51testing.com/batch.common.php?action=viewspace&op=up&itemid=142353&uid=22578) / [下一篇](http://www.51testing.com/batch.common.php?action=viewspace&op=next&itemid=142353&uid=22578)  2009-07-30
 17:27:18 / 个人分类：[Linux](http://www.51testing.com/?uid-22578-action-spacelist-type-blog-itemtypeid-11912)
[查看](http://www.51testing.com/?uid-22578-action-viewspace-itemid-142353#xspace-tracks%23xspace-tracks)(
 86 ) / [评论(
 0 )](http://www.51testing.com/?uid-22578-action-viewspace-itemid-142353#xspace-itemreply%23xspace-itemreply) / [评分(
 0 / 0 )](http://www.51testing.com/?uid-22578-action-viewspace-itemid-142353#xspace-itemform%23xspace-itemform)
由于有个项目涉及到TCP、HTTP、Soap的一些东西，于是回顾了一下以前曾经学过的东西，并学到了一些新东西，以此记录。根据我的逐步[]()学习过程，分了以下5个部分。
1.由netstat查看网络情况，引出的TCP建立连接、终止连接过程，以及TCP状态分析；
2.Soap=XML+HTTP引出的HTTP协议分析；
3.Soap(Simple Object Access Protocol)简单对象访问协议分析及应用；
4.番外一：awk的使用
5.番外二：[]()Linux下抓包工具
--------------------------篇四-------------------------------------
这篇讲linux下的抓包工具。
抓包工具比较好用的有两个，一个是snort，一个是tcpdump，这次不说snort了，觉得这个工具虽然很强大，但是比较复杂，还是tcpdump比较简单。tcpdump
 windows、linux版本均有。linux版本可以在以下网站下载：[www.tcpdump.org](http://www.tcpdump.org/).
安装好tcpdump之后，运行tcpdump：
1. tcpdump -D 获取网络适配器列表，以下是在windows上获取到的结果：
1.\Device\PssdkLoopback (PSSDK Loopback Ethernet Emulation Adapter)
2.\Device\{CF587901-C85F-4FD6-896F-D977DEFE76EC} (Intel(R) PRO/100 VE Network Co
nnection)
2. tcpdump -i <需要监控的网络适配器编号>，例如我想监控lo(127.0.0.1)，即上面列表中的1.\Device\PssdkLoopback: （windows上特有的，linux不适用）
tcpdump -i 1
如果不使用-i来定义监控适配器的话，默认使用列表中的第一个；
3.监控主机为192.9.200.59上8000端口的tcp协议：
tcpdump host 192.9.200.59 and tcp port 8000
4. 如果想要显示数据包的内容，需要使用-X参数，如，我想要显示捕获的http数据包http
 header的内容：
tcpdump -X host 192.9.200.59 and tcp port 8000
显示结果如下：
22:13:19.717472 IP testhost59.12535 > liujuan59.8000: . 1:330(329) ack 1 win 327
8
        0x0000:  4500 0171 e616 0000 8006 cb2b 0000 0000  E..q.......+....
        0x0010:  c009 c83b 30f7 1f40 0000 0002 0000 0002  ...;0..@........
        0x0020:  5010 8000 b066 0000 504f 5354 202f 2048  P....f..POST./.H
        0x0030:  5454 502f 312e 310d 0a43 6f6e 7465 6e74  TTP/1.1..Content
        0x0040:  2d54 7970 653a 2074 6578 742f 786d 6c3b  -Type:.text/xml;
        0x0050:  2063                                     .c
可以看到该结果只显示了http头的一部分，没有显示全，是因为tcpdump默认将显示的数据长度截断了，可以使用-s后面加数据长度，来设置数据显示长度：
tcpdump -X -s 0 host 192.9.200.59 and tcp port 8000
以上的例子中，-s 0 表示自动设置长度使其能够显示所有数据。
5. 捕获的数据太多，不断刷屏，可能需要将数据内容记录到文件里，需要使用-w参数：
tcpdump -X -s 0 -w aaa host 192.9.200.59 and tcp port 8000
则将之前显示在屏幕中的内容，写入tcpdump可执行文件同级目录下的aaa文件中。
文件查看方式如下，需要使用-r参数：
tcpdump -X -s 0 -r aaa host 192.9.200.59 and tcp port 8000
如果这样写：
tcpdump -r aaa
则只能看到最简单的数据传输交互过程，看不到数据包内容，查看时也需要使用相应的参数。
6.总结
总结一下，tcpdump的参数分两个部分，选项（Options）和表达式（expression）：
tcpdump[-adeflnNOpqRStuvxX] [-ccount]
               [-Cfile_size] [-Ffile]
               [-iinterface] [-mmodule] [-rfile]
               [-ssnaplen] [-Ttype] [-wfile]
               [-Ealgo:secret] [expression]
