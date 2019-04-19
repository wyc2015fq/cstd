# Linux使用tcpdump抓取网络数据包示例 - xqhrs232的专栏 - CSDN博客
2017年08月08日 14:52:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：269
原文地址::[http://blog.csdn.net/kobejayandy/article/details/17208137/](http://blog.csdn.net/kobejayandy/article/details/17208137/)
相关文章
1、[Linux tcpdump命令详解](http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html)----[http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html](http://www.cnblogs.com/ggjucheng/archive/2012/01/14/2322659.html)
2、tcpdump基于mac地址抓取数据包----[http://www.300168.com/yunwei/show-7728.html](http://www.300168.com/yunwei/show-7728.html)
3、tcpdump抓取TCP/IP数据包分析 
 ----[http://yhj1065.blog.163.com/blog/static/198002172013111911052930](http://yhj1065.blog.163.com/blog/static/198002172013111911052930)
tcpdump是[Linux](http://lib.csdn.net/base/linux)命令行下常用的的一个抓包工具，记录一下平时常用的方式，[测试](http://lib.csdn.net/base/softwaretest)机器系统是ubuntu
 12.04。
tcpdump的命令格式
tcpdump的参数众多，通过man tcpdump可以查看tcpdump的详细说明，这边只列一些笔者自己常用的参数：
tcpdump [-i 网卡] -nnAX '表达式'
各参数说明如下：
- -i：interface 监听的网卡。
- -nn：表示以ip和port的方式显示来源主机和目的主机，而不是用主机名和服务。
- -A：以ascii的方式显示数据包，抓取web数据时很有用。
- -X：数据包将会以16进制和ascii的方式显示。
- 表达式：表达式有很多种，常见的有：host 主机；port 端口；src host 发包主机；dst host 收包主机。多个条件可以用and、or组合，取反可以使用!，更多的使用可以查看man 7 pcap-filter。
下面进行一些命令测试，如果没有权限，可以先切换成root用户。
监听网卡eth0
$ tcpdump -i eth0
这个方式最简单了，但是用处不多，因为基本上只能看到数据包的信息刷屏，压根看不清，可以使用ctrl+c中断退出，如果真有需求，可以将输出内容重定向到一个文件，这样也更方便查看。
监听指定协议的数据
$ tcpdump -i eth0 -nn 'icmp'
这个是用来监听icmp协议的数据，就是ping命令使用的协议。类似的，如果要监听tcp或者是udp协议，只需要修改上例的icmp就可以了。ping下监听的机器，输出如下：
![linux使用tcpdump抓包示例](http://www.01happy.com/wp-content/uploads/2012/12/linux%E4%BD%BF%E7%94%A8tcpdump%E6%8A%93%E5%8C%85%E7%A4%BA%E4%BE%8B.png)
[linux](http://lib.csdn.net/base/linux)使用tcpdump抓包示例
每一行的各个数据表示的含义：
抓到包的时间 IP 发包的主机和端口 > 接收的主机和端口 数据包内容
监听指定的主机
$ tcpdump -i eth0 -nn 'host 192.168.1.231'
这样的话，192.168.1.231这台主机接收到的包和发送的包都会被抓取。
$ tcpdump -i eth0 -nn 'src host 192.168.1.231'
这样只有192.168.1.231这台主机发送的包才会被抓取。
$ tcpdump -i eth0 -nn 'dst host 192.168.1.231'
这样只有192.168.1.231这台主机接收到的包才会被抓取。
监听指定端口
$ tcpdump -i eth0 -nnA 'port 80'
上例是用来监听主机的80端口收到和发送的所有数据包，结合-A参数，在web开发中，真是非常有用。
监听指定主机和端口
$ tcpdump -i eth0 -nnA 'port 80 and src host 192.168.1.231'
多个条件可以用and，or连接。上例表示监听192.168.1.231主机通过80端口发送的数据包。
监听除某个端口外的其它端口
$ tcpdump -i eth0 -nnA '!port 22'
如果需要排除某个端口或者主机，可以使用“!”符号，上例表示监听非22端口的数据包。
小结：
tcpdump这个功能参数很多，表达式的选项也非常多，非常强大，不过常用的功能确实不多。详情可以通过man查看系统手册。
另外在抓取web包的时候，发送网页内容都是很奇怪的字符，发现是apache开启了gzip压缩的缘故，关闭掉gzip压缩就可以了。在ubuntu 12.04下，编辑vim /etc/apache2/mods-enabled/deflate.load文件，将加载模块deflate_module的语句注释掉，然后重启apache就OK了

