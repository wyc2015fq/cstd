# 一站式学习Wireshark（九）：应用Wireshark显示过滤器分析特定数据流（上） - 文章 - 伯乐在线
原文出处： [EMC中文支持论坛](https://community.emc.com/message/828296#828296)
**介绍**
掌握显示过滤器对于网络分析者来说是一项必备的技能。这是一项大海捞针的技巧。学会构建，编辑，保存关键的显示过滤器能够节省数小时的时间。
与捕捉过滤器使用的BPF语法不同，显示过滤器使用的是Wireshark特定的格式。除了某些特例之外，Wireshark显示过滤器和捕捉过滤器有很大的区别。
** 更多信息**
**最简单的过滤器语法:**
最简单的显示过滤器可基于协议，应用，域名，或字符，对大小写敏感。绝大多数简单的显示过滤器由小写字母组成。
**协议过滤器：**
- arp：显示所有包括ARP请求和回复在内的所有ARP数据流。
-  ip：显示内含IPv4头在内的（如ICMP目的地址不可达报文，在ICMP报文头之后返回到来方向的IPv4头）IP数据流。
- ipv6：显示所有IPv6数据流，包括内含IPv6报文头的IPv4报文，如6to4，Teredo，以及ISATAP数据流。
- tcp：显示所有基于TCP的数据流。
**应用过滤器：**
- bootp：显示所有DHCP数据流（基于BOOTP）。
- dns：显示包括TCP区域传输以及基于标准UDP的DNS请求和回复在内的所有DNS数据流。
- tftp：显示所有TFTP（Trivial File Transfer Protocol）数据流。
- http：显示所有HTTP命令，回复以及数据传输报文，但不显示TCP握手报文，TCP ACK报文以及TCP结束报文。
- icmp：显示所有ICMP报文。
**域过滤器：**
- boot.option.hostname：显示所有包含主机名的DHCP数据流（DHCP基于BOOTP）。
- http:host：显示所有包含HTTP主机名字段的所有HTTP报文。此报文是客户端向网络服务器发送请求时发出的。
- ftp.request.command：显示所有包含命令的FTP数据流，比如USER，PASS，或RETR命令。
**字符过滤器：**
- tcp.analysis.flags：显示所有包含TCP分析标识的所有报文，包括报文丢失，重传，或零窗口标识。
- tcp.analysis,zero_window：显示含有表明发送方的接收缓存用完标识的报文。
**了解字段名:**
很多显示过滤器都是基于字段名（例如http.host）。要了解字段名，在Packet list面板选中字段查看Status条，如下图所示。
![image002](http://jbcdn2.b0.upaiyun.com/2015/12/c037af8fcc414f81879d248936427de5.png)
本例中，我们在Packet Display面板中选中第10帧，然后在Packet Details面板中展开HTTP报文头。之后在报文的HTTP段点击Request Method行，状态条表明这一字段称为http.request.method。
我们在显示过滤器区域输入http.request.method以显示所有包含这一字段的报文。如下图所示，注意到Status条表明此抓包文件包含2011个报文只有101个报文符合过滤条件。
![image003](http://jbcdn2.b0.upaiyun.com/2015/12/1488ffe8dd392a8caf548028f6ee49ff.png)
这是一个很好用的过滤器，以确定HTTP客户端请求了哪些内容。网络服务器不发送HTTP request method，它们发送HTTP响应代码。
**使用自动完成功能:**
当你在过滤器区域输入http.request.method的时候，Wireshark打开一个窗口让你遍历过滤器选项。当你输入http.（包括点号），你会看到所有可能的以此为开头的显示过滤条件列表。继续输入http.request. ，会看到以这一短语为开头的过滤条件，如下图所示：
![image004](http://jbcdn2.b0.upaiyun.com/2015/12/6d50427c3cbc7d823ca249e0db8f6af3.png)
可以使用这一自动完成功能查看所有可用的过滤条件。例如，输入tcp. ，Wireshark会列出所有可用的TCP过滤条件。如果输入tcp.analysis. ，Wireshark会列出所有处理TCP问题与性能的TCP analysis过滤条件，如下图：
![image005](http://jbcdn2.b0.upaiyun.com/2015/12/c62874c0b0fb36caec0c9c26d00f6301.png)
**显示过滤器比较运算符:**
通过扩展过滤条件可查找某一域值，Wireshark针对此功能支持数字比较运算符。
1. ==或eq
例如：ip.src == 10.2.2.2
显示所有源地址为10.2.2.2的IPv4数据流
2. ！=或ne
例如：tcp.srcport != 80
显示源端口除了80以外的所有TCP数据流
3. >或gt
例如：frame.time_relative > 1
显示距前一个报文到达时间相差1秒的报文
4. <或lt
例如：tcp.window_size < 1460
显示当TCP接收窗口小于1460字节时的报文
5. >=或ge
例如：dns.count.answers >= 10
显示包含10个以上answer的DNS响应报文
6. <=或le
例如：ip.ttl <= 10
显示IP报文中Time to Live字段小于等于10的报文
7. contains
例如：http contains “GET”
显示所有HTTP客户端发送给HTTP服务器的GET请求
对于基于TCP应用的过滤条件采用比较运算符。例如，如果想看端口80上面的HTTP数据流，使用HTTP.port==80。
**小贴士：**
运算符两边不用留空格。ip.src == 10.2.2.2与ip.src==10.2.2.2的效果是相同的。
使用Expressions:
当你实在不知道该过滤些什么的时候，使用显示过滤器工具栏的Expression按钮。在Filter Expression窗口，输入感兴趣的应用或协议名，跳转到Field Name列表中相应条目。Relation选项用来添加一个比较运算符，窗口右端是预先定义的值。点击Apply按钮完成。
![image006](http://jbcdn2.b0.upaiyun.com/2015/12/bee5464f1399de9494126b801d600d0f.png)
**编辑和使用默认过滤器:**
Wireshark提供15个缺省显示过滤器供创建新的显示过滤器时参考。点击Filter按钮或点击Display Filter按钮打开显示过滤器窗口，如下图所示：
![image007](http://jbcdn2.b0.upaiyun.com/2015/12/6df787e911e282b998946f932177ffaa.png)
下图显示了缺省过滤器列表，选中之后点击OK。
![image008](http://jbcdn2.b0.upaiyun.com/2015/12/6c6fab5277846ee6a18229f8163999d6.png)
小心使用缺省过滤器。以太网和IP主机过滤器可能与实际网络不符。使用之前必须编辑或作为创建参考。
**小贴士：**
显示过滤器保存在一个名为dfilters的文件中，可以使用任何文本编辑器来进行编辑。要找到你的dfilters文件，首先在Status条的右端查找当前profile名。如果这一区域表明你在默认profile，选择Help\About Wireshark\Folders并双击Personal Configuration文件夹链接，dfilters文件就在这个目录里。
