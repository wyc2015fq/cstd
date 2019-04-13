
# Ethereal使用入门(数据流分析工具) - .NET博文收藏 - CSDN博客


2008年06月10日 12:44:00[hejishan](https://me.csdn.net/hejishan)阅读数：681标签：[工具																](https://so.csdn.net/so/search/s.do?q=工具&t=blog)[primitive																](https://so.csdn.net/so/search/s.do?q=primitive&t=blog)[protocols																](https://so.csdn.net/so/search/s.do?q=protocols&t=blog)[filter																](https://so.csdn.net/so/search/s.do?q=filter&t=blog)[tcp																](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=filter&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=protocols&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=protocols&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=primitive&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
ethereal 可以用来从网络上抓包，并能对包进行分析。下面介绍windows 下面ethereal 的使用方法
**安装**
1）安装winpcap，下载地址[http://netgroup-serv.polito.it/winpcap/install/Default.htm](http://netgroup-serv.polito.it/winpcap/install/Default.htm)2）安装ethereal ，下载地址[http://www.ethereal.com/](http://www.ethereal.com/)
**使用**
windows 程序，使用很简单。
启动ethereal 以后，选择菜单Capature->Start ，就OK 了。当你不想抓的时候，按一下stop， 抓的包就会显示在面板中，并且已经分析好了。
下面是一个截图：
![Ethereal](http://www.cnpaf.net/Files/UpotherFiles/ethreal_img_0.jpg)
**ethereal使用－capture选项**
![Ethereal](http://www.cnpaf.net/Files/UpotherFiles/ethreal_img_1.jpg)
nterface: 指定在哪个接口（网卡）上抓包。一般情况下都是单网卡，所以使用缺省的就可以了Limit each packet: 限制每个包的大小，缺省情况不限制
Capture packets in promiscuous mode: 是否打开混杂模式。如果打开，抓取所有的数据包。一般情况下只需要监听本机收到或者发出的包，因此应该关闭这个选项。Filter：过滤器。只抓取满足过滤规则的包（可暂时略过） File：如果需要将抓到的包写到文件中，在这里输入文件名称。use ring buffer： 是否使用循环缓冲。缺省情况下不使用，即一直抓包。注意，循环缓冲只有在写文件的时候才有效。如果使用了循环缓冲，还需要设置文件的数目，文件多大时回卷
其他的项选择缺省的就可以了
**ethereal的抓包****过滤****器**
抓包过滤器用来抓取感兴趣的包，用在抓包过程中。 抓包过滤器使用的是libcap过滤器语言，在tcpdump 的手册中有详细的解释，基本结构是： [not] primitive [and|or [not] primitive ...]
个人观点，如果你想抓取某些特定的数据包时，可以有以下两种方法，你可以任选一种， 个人比较偏好第二种方式：
1、在抓包的时候，就先定义好抓包过滤器，这样结果就是只抓到你设定好的那些类型的数 据包；
2、先不管三七二十一，把本机收到或者发出的包一股脑的抓下来，然后使用下节介绍的显 示过滤器，只让Ethereal 显示那些你想要的那些类型的数据包；
**etheral的显示****过滤****器（重点内容）**
在抓包完成以后，显示过滤器可以用来找到你感兴趣的包，可以根据1)协议2)是否存在某个域3)域值4)域值之间的比较来查找你感兴趣的包。
举个例子，如果你只想查看使用tcp 协议的包，在ethereal 窗口的左下角的Filter 中输入tcp， 然后回车，ethereal 就会只显示tcp 协议的包。如下图所示：
![Ethereal](http://www.cnpaf.net/Files/UpotherFiles/ethreal_img_2.jpg)
值比较表达式可以使用下面的操作符来构造显示过滤器自然语言类c 表示举例eq == ip.addr==10.1.10.20 ne != ip.addr!=10.1.10.20 gt > frame.pkt_len>10 lt < frame.pkt_len<10 ge >= frame.pkt_len>=10 le <= frame.pkt_len<=10
表达式组合可以使用下面的逻辑操作符将表达式组合起来自然语言类c 表示举例and && 逻辑与，比如ip.addr=10.1.10.20&&tcp.flag.fin or || 逻辑或，比如ip.addr=10.1.10.20||ip.addr=10.1.10.21 xor ^^ 异或，如tr.dst[0:3] == 0.6.29 xor tr.src[0:3] == not ! 逻辑非，如 !llc
例如：
我想抓取IP 地址是192.168.2.10 的主机，它所接收收或发送的所有的HTTP 报文，那么合适的显示Filter （过滤器）就是：
![Ethereal](http://www.cnpaf.net/Files/UpotherFiles/ethreal_img_3.jpg)
**在ethereal 使用协议插件**
ethereal 能够支持许多协议，但有些协议需要安装插件以后才能解，比如H.323，以H.323 协议为例，首先下载ethereal 的H.323 插件，下载地址[http://www.voice2sniff.org/](http://www.voice2sniff.org/)下载完了以后将文件(h323.dll) 解压到ethereal 安装目录的plugin/0.9.x 目录下面，比如我的是0.9.11 ，然后，需要进行一下设置1)启动ethereal 2)菜单Edit->Preference 3)单击Protocols 前面的"+"号，展开Protocols 4)找到Q931 ，并单击5)确保"Desegment.... TCP segments" 是选中的（即方框被按下去）6)单击TCP 7)确保"Allow....TCP streams" 是选中的8)确保没有选中"Check....TCP checksum" 和"Use....sequence numbers" 9)单击TPKT 10)确保"Desegment....TCP segments" 是选中的11)点击Save，然后点击Apply ，然后点击OK 你也完全可以不断地重新安装新版本winpcap 和ethreal， 这样就可以不需在旧的ethreal 的版本中安装新的插件来支持新的协议插件。这也是懒人的一种做法



