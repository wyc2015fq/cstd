# 一站式学习Wireshark（十）：应用Wireshark显示过滤器分析特定数据流（下） - 文章 - 伯乐在线
原文出处： [EMC中文支持论坛](https://community.emc.com/message/830287#830287)
# 介绍
掌握显示过滤器对于网络分析者来说是一项必备的技能。这是一项大海捞针的技巧。学会构建，编辑，保存关键的显示过滤器能够节省数小时的时间。
与捕捉过滤器使用的BPF语法不同，显示过滤器使用的是Wireshark特定的格式。除了某些特例之外，Wireshark显示过滤器和捕捉过滤器有很大的区别。
# 更多信息
**过滤****HTTP数据流:**
在排查网页浏览器会话或检查网速过慢问题时，对浏览器会话进行过滤就显得尤为重要。过滤HTTP数据流有两种方式：
http
tcp.port==xx(xx表示所使用的HTTP端口)
第二种过滤方法更加有效。让我们通过分别对网页浏览会话应用两个过滤条件来比较一下：
**基于****TCP端口号的应用过滤**：
本例中的抓包文件包含与网站[www.wireshark.org的链接以及请求下载Wireshark](http://www.wireshark.org%E7%9A%84%E9%93%BE%E6%8E%A5%E4%BB%A5%E5%8F%8A%E8%AF%B7%E6%B1%82%E4%B8%8B%E8%BD%BDWireshark/)的请求。我们使用tcp.port==80的显示过滤器并且发现，确实，所有报文都符合条件，如下图所示。很好这就是我们想要的结果。
![image002](http://jbcdn2.b0.upaiyun.com/2015/12/c037af8fcc414f81879d248936427de52.png)
我们仔细看报文20的Protocol列，Wireshark并没有在报文中看到任何HTTP命令或回复，因此HTTP协议分析器没有被应用于此报文。它只是一个TCP报文（TCP ACK, FIN, RST,以及三路TCP握手信号都列成TCP）。
如果你想要查看TCP连接建立，维护以及关闭报文，就应该使用上述过滤条件（并且你会一直看到这些TCP报文）。
**谨慎使用基于****TCP的应用名过滤**：
现在来看一下对数据流应用http过滤条件的情况。下图中，你会看到Wireshark显示了13，353个报文。这些是**Protocol**列中包含HTTP的报文。
![image003](http://jbcdn2.b0.upaiyun.com/2015/12/1488ffe8dd392a8caf548028f6ee49ff2.png)
这是整个网站浏览会话的一部分，使用HTTP过滤条件我们无法检测出TCP错误，对于应用永远是使用端口号过滤器优于TCP。
小贴士：
不幸的是，Wireshark对于HTTP数据流的缺省过滤是http。可以考虑将缺省对HTTP数据流的过滤改为基于端口号。
**按照某一****IP地址或主机过滤报文**：
对于IPv4数据流，我们使用字段名ip.src，ip.dst，ip.addr；对于IPv6数据流，使用ipv6.src，ipv6.dst，ipv6.host以及ipv6.addr。注意当你在Packet Details面板中点击IP地址时，将会看到它们被称为：ip.src，ip.dst, ipv6.src或ipv6.dst。字段名ip.host和ipv6.host，ip.addr以及ipv6.addr不在报文里。
ip.host和ipv6.host过滤条件在IPv4/IPv6源和目的地址字段查找解析为指定主机名的IPv4或IPv6地址。ip.addr==[address]和ipv6.addr==[address]过滤条件在IPv4/IPv6源和目的地址字段查找指定IPv4/IPv6地址。
- 例如：ip.addr==10.3.1.1
显示在IP源地址字段或IP目的地址字段包含10.3.1.1的帧。
- 例如：！ip.addr==10.3.1.1
显示除了在IP源地址字段或IP目的地址字段包含10.3.1.1以外的帧。
- 例如：ipv6.addr==2406:da00:ff00::6b16:f02d
显示以2406:da00:ff00::6b16:f02d为源地址或目的地址的帧
- 例如：ip.src==10.3.1.1
显示所有来自10.3.1.1的数据流。
- 例如：ip.dst==10.3.1.1
显示所有发往10.3.1.1的数据流
- 例如：ip.host==www.wireshark.org
显示所有解析为[www.wireshark.org的IP](http://www.wireshark.org%E7%9A%84IP/)地址。
**按照某一****IP地址范围过滤报文**：
可以使用>或<比较运算符或逻辑运算符&&查找某一地址范围内的报文。
- 例如：ip.addr>10.3.0.1&&ip.addr<10.3.0.5
显示来自或发往10.3.0.2，10.3.0.3，10.3.0.4的数据流。
- 例如：(ip.addr>=10.3.0.1&&ip.addr<=10.3.0.6)&&!ip.addr==10.3.0.3
显示来自或发往10.3.0.1，10.3.0.2，10.3.0.4，10.3.0.5，10.3.0.6的数据流，10.3.0.3排除在外。
- ipv6.addr>=fe80::&&ipv6.addr
显示IPv6地址从0xfe80到0xfec0开头的数据流。
**按照某一****IP子网过滤报文**：
可以通过ip.addr字段名定义一个子网。这种格式使用IP地址后跟斜杠以及一个后缀，表明IP地址中定义的网络部分的比特数。
- 例如：ip.addr==10.3.0.0/16
显示在IP源地址或目的地址字段以10.3开头的数据流。
- 例如：ip.addr==10.3.0.0/16 && ！ip.addr==10.3.1.1
显示除了10.3.1.1以外，在IP源地址或目的地址字段以10.3开头的数据流。
- 例如：!ip.addr==10.3.0.0/16 && !ip.addr==10.2.0.0/16
显示所有数据流，除了在IP源地址或目的地址字段以10.3和10.2开头的数据流.
**使用右键****|作为过滤条件**：
如下图所示：在某一帧的Packet Details面板，扩展HTTP部分，右键Request URI一行，选择**Apply as Filter | Selected**。
![image004](http://jbcdn2.b0.upaiyun.com/2015/12/6d50427c3cbc7d823ca249e0db8f6af32.png)
Wireshark会创建合适的显示过滤条件（http.request.rui==”/”）并应用于抓包文件。之后过滤得到2个报文，显示用户从两个不同的IP地址访问主页内容，如下图所示。
![image005](http://jbcdn2.b0.upaiyun.com/2015/12/c62874c0b0fb36caec0c9c26d00f63012.png)
如果你想排除这类HTTP请求，只要在过滤条件前加！或not。你可以通过右键GET请求并选择**Apply as Filter | Not Selected。**
not http.request.uri==”/”
如果你感兴趣的是其余HTTP GET请求，可对上述过滤条件扩展：定位在一个HTTP GET请求报文，扩展HTTP部分，右键GET并选择**Apply as Filter**，这一次选择**and Selected**选项。之后，显示过滤器显示如下：
(not http.request.uri==”/”)&&(http.request.method==“GET”)。
现在可以看到除了default page request(/)之外的所有HTTP GET请求。
**过滤单个****TCP或UDP会话**：
当你想要观察客户端应用于服务器进程之间的通讯，你需要查找的是一个“会话“。会话是基于客户端应用和服务器进程的IP地址和端口号。通常抓包文件中会包含数百个会话，了解如何快速定位及过滤无疑是很有帮助的。
以下两种常用方式可从抓包文件中提取单一TCP或UDP会话：
- 通过在**Packet List**面板右键一个TCP或UDP报文并选择**Conversation Filter | [TCP|UDP].**
- 通过在**Packet List**面板右键一个TCP或UDP报文并选择**Follow [TCP|UDP] Stream**。
**右键选择****Conversation Filter**：
通过在**Packet List**面板右键一个TCP或UDP报文并选择**Conversation Filter | TCP，**如下图所示：
![image006](http://jbcdn2.b0.upaiyun.com/2015/12/bee5464f1399de9494126b801d600d0f2.png)
Wireshark对数据流创建并应用以下过滤条件：
(ip.addr eq 24.6.173.220 and ip.addr eq 184.84.222.48) and (tcp.port eq 19953 and tcp.port eq 80)。
同样的方法可应用于基于IP地址，以太网地址，UDP地址/端口号结合的会话。
**右键选择****Follow a Stream**：
要查看应用命令以及会话中的交换数据，通过在**Packet List**面板右键一个TCP或UDP报文并选择**Follow [TCP|UDP] Stream**，如下图所示。如果选择**Follow UDP Stream**，显示过滤条件会基于IP地址和端口号。如果选择**Follow TCP Stream**, 显示过滤条件会基于TCP Stream Index number。
![image007](http://jbcdn2.b0.upaiyun.com/2015/12/6df787e911e282b998946f932177ffaa2.png)
**错误的用法导致不****work**：
**错误**：ip.addr != 10.2.2.2
显示在IP源地址*或*IP目的地址不包含10.2.2.2的报文。只要在源或目的IP地址不为10.2.2.2，报文就会被显示出来。这时隐含的*或*会导致实际上并未过滤任何报文。
**正确**：！ip.addr == 10.2.2.2
显示IP源地址和IP目的地址都不包含10.2.2.2的报文。
**错误**：!tcp.flags.syn==1
显示所有不含TCP SYN bit设置为1的报文。其他协议报文，必须UDP和ARP报文也符合这一过滤条件，因为它们的TCP SYN bit没有设置为1。
**正确**：tcp.flags.syn！=1
只显示包含SYN设置为0的TCP报文。
小贴士：
当你知道只有一个字段符合你的过滤字段名的时候，不要害怕使用!=运算符。有时，这是最好用的过滤运算符。
