# wireshark过滤使用 - V__KING__的专栏 - CSDN博客





2017年06月21日 14:21:09[v__king__](https://me.csdn.net/V__KING__)阅读数：239标签：[wireshark																[通信](https://so.csdn.net/so/search/s.do?q=通信&t=blog)](https://so.csdn.net/so/search/s.do?q=wireshark&t=blog)
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)








# 一.  wireshark能干啥？

wireshark是什么，我这里就不介绍了。总之就是功能很变态的抓包工具，能抓取和你本机通信的各种协议的信息，例如：tcp，udp，http，ftp，ssh等等等。

# 二.  wireshark使用

在使用的过程中，可以点击”表达式”，找到IPV4，可以查看相关的过滤规则。同理，其他协议的也是如此。
- 
抓取两个ip间的通信数据：

> 
(ip.src==192.168.1.108&&ip.dst==192.168.1.73)||(ip.src==192.168.1.73&&ip.dst==192.168.1.108)


- 
抓某个tcp ip端口的数据

> 
(ip.src==192.168.1.222 and tcp.port==8084)



# 三、 如何自定义协议让wireshark识别

参考： 

[1]. [http://www.cnblogs.com/zzqcn/p/4827251.html](http://www.cnblogs.com/zzqcn/p/4827251.html)   （有很多关于lua脚本和wireshark的教程）

[2]. [http://www.cnblogs.com/wendellyi/p/3475461.html](http://www.cnblogs.com/wendellyi/p/3475461.html)




