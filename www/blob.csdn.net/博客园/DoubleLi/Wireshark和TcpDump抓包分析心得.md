# Wireshark和TcpDump抓包分析心得 - DoubleLi - 博客园






**Wireshark和 TcpDump抓包分析心得 **



1. Wireshark与tcpdump介绍

 Wireshark是一个网络协议检测工具，支持Windows平台和Unix平台，我一般只在Windows平台下使用Wireshark，如果是Linux的话，我直接用tcpdump了，因为我工作环境中的Linux一般只有字符界面，且一般而言Linux都自带的tcpdump，或者用tcpdump抓包以后用Wireshark打开分析。

在Windows平台下，Wireshark通过WinPcap进行抓包，封装的很好，使用起来很方便，可以很容易的制定抓包过滤器或者显示过滤器，具体在下面介绍。Wireshark是一个免费的工具，只要google一下就能很容易找到下载的地方。

tcpdump是基于Unix系统的命令行式的数据包嗅探工具。如果要使用tcpdump抓取其他主机MAC地址的数据包，必须开启网卡混杂模式，所谓混杂模式，用最简单的语言就是让网卡抓取任何经过它的数据包，不管这个数据包是不是发给它或者是它发出的，点击【[http://en.wikipedia.org/wiki/Promiscuous_mode](http://en.wikipedia.org/wiki/Promiscuous_mode)】获取更多有关混杂模式的资料。一般而言，Unix不会让普通用户设置混杂模式，因为这样可以看到别人的信息，比如telnet的用户名和密码，这样会引起一些安全上的问题，所以只有root用户可以开启混杂模式，开启混杂模式的命令是：ifconfigeth0 promisc, eth0是你要打开混杂模式的网卡。肯定有人要问如果在windows下要不要打开混杂模式，windows下网卡没有什么混杂模式不混杂模式，在于应用程序本身，如使用Wireshark抓包的时候可以通过设置为在混杂模式下抓包（这就是为什么该死的ARP欺骗病毒可以猖狂的原因）。tcpdump当然也可以指定抓包过滤器，而且其过滤器语言非常著名，叫做Berkeley包过滤，简称BPF语言。

2. 简单的例子

我们通过访问[www.google.com.hk](http://www.google.com.hk/)这个网址来看看抓包结果。

2.1  tcpdump

前面说过一般情况下Linux都自带了tcpdump，但是如果发生了小概率事件，发现没有tcpdump的话，可以到[http://www.tcpdump.org](http://www.tcpdump.org/)下载源代码，编译安装。

使用root用户登录，运行tcpdump命令就可以开始抓包。这里说明一下，如果使用SSH登录到远程Linux，然后直接运行tcpdump，会发现抓到大量的数据包，速度快的都看不清楚，这是因为tcpdump抓到的包发送给远程的终端显示，同时又抓了这个包，再显示，再抓取，造成了循环抓取。当然，这样抓包没有任何意义，除了证明你的网络是通的。

因为没有打开网卡的混杂模式，所以如果本机没有任何进程访问网络，是抓不到包的，如果在字符界面下，用wget[http://www.google.com.hk](http://www.google.com.hk/) 访问网址，如果有GUI，可以打开firefox浏览器访问[http://www.google.com.hk](http://www.google.com.hk/)。

默认情况下，tcpdump会选择第一块网卡，也就是eth0，进行抓包，每行显示一个抓取的数据包，如：

0.003183              192.168.21.137  72.14.203.147     TCP        38039 > http [SYN] Seq=0 Win=5840Len=0 MSS=1460 SACK_PERM=1 TSV=36941509 TSER=0 WS=6

0.011707              72.14.203.147     192.168.21.137  TCP        http > 38039[SYN, ACK] Seq=0 Ack=1 Win=64240 Len=0 MSS=1460

0.011770              192.168.21.137  72.14.203.147     TCP        38039 > http [ACK] Seq=1 Ack=1Win=5840 Len=0

以上三个数据包就是著名的TCP三次握手的数据包，其中38039是客户端的TCP端口，http的默认端口是80，如果tcpdump在/etc/services中发现端口对应的服务名称，那么会自动的转为名字，所以这里会显示为http。表示客户端的38039端口和服务器端的http端口进行TCP三次握手。

前面提到tcpdump默认选择第一块网卡进行抓包，我们可以使用-i参数指定通过哪一个网卡抓包，如（#表示我输入的命令，Linux下root用户的提示符就是#）：

# tcpdump –i eth1

或者

#tcpdump –i any

如果想知道我们可以通过哪几个网卡抓包，可以使用-D参数，如：

# tcpdump –D

1.eth0

2.any

3.lo

因为我的机器上只有一个网卡，因此只有eth0，如果有多块网卡活动的话，会有eth1，eth2依次下去。any的意思是通过任意一块网卡进行抓包，lo是回环接口。（关于TCP三次握手和回环接口等网络问题，请参考《TCP/IP协议详解》）。

默认情况下，tcpdump抓包结果显示在屏幕（严格点，专业点应该说是标准输出）上，显然这不利于进一步的数据分析，因此我们需要将抓包结果存放在文件中。可是使用-w命令将结果保存在文件中，如：

#tcpdump –w google.cap

这句命令将抓包结果存放在google.cap文件中，结束以后可以用Wireshark打开查看。同事，tcpdump出了抓包，还可以使用-r参数制定抓包数据文件，结合过滤器对抓包数据分析，如：

#tcpdump –r google.cap http

这句命令的意思是让tcpdump读取google.cap文件，把其中http协议的数据包都给过滤出来。关于过滤器在下面详细介绍。

2.2  Wireshark

我在windows系统中使用Wireshark的，首先熟悉一下界面，图1是使用Wireshark打开google.cap文件的界面，

![](http://hi.csdn.net/attachment/201012/14/0_1292328766j23l.gif)

图1   Wireshark界面

图1中标注出三快区域，R1区域用来显示简单的数据包信息，我们用tcpdump抓包的时候，默认情况下也是显示成这样的；R2区域用来显示选中的数据包的详细信息，细心一点会发现他是按照TCP/IP四层结构显示的，第一行是数据链路层的信息，第二行是网络层信息（IP协议），第三行是传输层信息（TCP协议），第四行是应用层信息（HTTP协议），可以展开每一行用来观察具体的内容；R3区域是用来显示此数据包的真实面目。我们在R1和R2区域看到的信息都是Wireshark整理以后给我们看的，抓包的真实数据实际上是一堆二进制序列，用ultraedit打开google.cap文件可以看到就是一些数字，如图2所示。

![](http://hi.csdn.net/attachment/201012/14/0_12923288875Lvb.gif)

图2  抓包文件长的样子

使用Wireshark抓包非常容易，直接点击按钮![](http://hi.csdn.net/attachment/201012/14/0_1292328956uQUE.gif)（工具栏第三个按钮）![](http://hi.csdn.net/attachment/201012/14/0_1292328988L8kk.gif)（工具栏第一个按钮）就开始抓包了，会发现只要一点击这个按钮，立刻就显示抓到包了，这是因为Wireshark默认在混杂模式下抓包，只要经过网卡的数据包都抓取下来（当然这台机器要连在网络中，如果没有数据流过当然没有包可抓），点击按钮![](http://hi.csdn.net/attachment/201012/14/0_1292329027Q6t8.gif)停止此次抓包。

如果机器上安装了多块网卡，Wireshark默认选择第一张网卡抓包，如果等抓包完成了，这是发现选错了网卡是一件极度郁闷的事情。点击按钮可以在抓包之前选择抓哪张网卡。

![](http://hi.csdn.net/attachment/201012/14/0_1292329083Gn6e.gif)

图3  选择网卡

我机器上只有一张网卡，另外两个是安装Vmware时的虚拟网卡，可以看到虽然Packets上面已经有数据了，实际上需要点击Start才开始抓包。

解决了选择网卡的问题以后，考虑如果过滤抓包内容，点击菜单栏上的” Capture” > “Options”可以看到制定抓包规则的界面，如图4所示。

![](http://hi.csdn.net/attachment/201012/14/0_129232912319i1.gif)

图4  制定抓包规则

图4可以看到Caputre packets inpromiscuous mode，默认是选中的，表示Wireshark默认在混杂模式下抓包。同样可以选择通过哪张网卡抓包，不过这些都不是重点，最重要的是Caupture Fileter这里，点击该按钮，可以看到弹出一些预定义好的过滤器。比如选择“HTTP TCP port(80)”，下面Filter string: tcp port http就是过滤器的表示。表示抓tcp协议的，端口为80的数据包（http协议的默认端口是80）。

3. 过滤器（BPF语言）的使用

         主要介绍一下在tcpdump中的过滤器使用，因为懂了这个就可以得心应手的使用wireshark了。

         从最简单的开始，BPF语言主要有一个标志或者数字和限定词组成，限定词有三种：

          第一种：指定类型

          host, 定义抓取哪个IP地址（也可以给它mac地址，格式是00:00:00:00:00:00）的数据包，比如我想抓有关192.168.0.148这个IP地址的数据包，那么就写成tcpdump host 192.168.0.148, host是限定词，192.168.0.148就是标志。这条命令会抓取从发出或者向192.168.0.148发送的数据包。

          net, 定义抓取某个网络的数据包，给出网络号就行了，它根据给的网络号字节数是1,2,3来判断A类地址，B类地址或者C类地址，比如tcpdump net 10.1.1 ，它就认为这是一个C类地址。

port，指定端口，比如tcpdumphost and port 22, 这是抓端口为22的数据包，不管是TCP还是UDP的，这里我稍微早一点的给出了逻辑操作，andJ，如果只想抓TCP的，那么可以写tcpdump host 192.168.0.148and tcp port 22。

portrange，顾名思义，这个是指定端口范围的，用连字符”-”指定范围，比如tcpdump port 1025-8080

         第二种：指定方向

我们之前的命令都是说“这条命令会抓取从192.168.0.148发出或者向192.168.0.148发送”，所以，如果指向抓从发出的数据包可以使用限定词src, 命令：tcpdumpsrc host 192.168.0.148，反过来，想抓发向192.168.0.148的数据包，使用限定词dst,命令：tcpdumpdst host 192.168.0.148。

        第三种：指定协议

我们知道网络协议有N种。。。我列一下常用的几种，其他的可以去google一下J

ether和fddi,以太网协议

tr, TR协议

ip, IP协议

ip6，IPv6协议

arp,  ARP协议

好了，最后还需要注意的是逻辑运算，and,or, not（与，或，非），上面已经有一个例子了， 这里就不再罗嗦了，和普通的编程语言没有什么不同。



除此之外，还有更加牛X的功能，比如指定TCP中的某个标识位是什么，这种应用我一般很少用，不再罗嗦了。









