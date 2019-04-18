# 5款监控利器介绍(smokeping、cacti、nagios、zabbix、ntop) - 深之JohnChen的专栏 - CSDN博客

2017年02月17日 09:13:27[byxdaz](https://me.csdn.net/byxdaz)阅读数：3178


### **转载：**[5款监控利器介绍(smokeping、cacti、nagios、zabbix、ntop) | 运维生存时间](http://www.ttlsa.com/monitor-safe/monitor-software-5/)[](http://www.ttlsa.com/monitor-safe/monitor-software-5/#)

### 1. smokeping

Smokeping 是rrdtool 的作者Tobi Oetiker 的作品，是用[Perl](http://www.ttlsa.com/perl/) 写的，主要是监视网络性能，包括常规的ping，用echoping监控www 服务器性能，监视dns 查询性能，监视ssh 性能等。底层也是rrdtool做支持，特点是画的图非常漂亮，网络丢包和延迟用颜色和阴影来表示。在企业中运用较少。

### 2 cacti

Cacti是一套基于[PHP](http://www.ttlsa.com/php/),MySQL,SNMP及RRDTool开发的网络流量监测图形分析工具。

简单的说Cacti 就是一个PHP 程序。它通过使用SNMP 协议获取远端网络设备和相关信息，（其实就是使用Net-SNMP 软件包的snmpget 和snmpwalk 命令获取）并通过RRDTOOL 工具绘图，通过PHP 程序展现出来。我们使用它可以展现出监控对象一段时间内的状态或者性能趋势图。

Cacti是通过 snmpget来获取数据，使用 RRDtool绘画图形，而且你完全可以不需要了解RRDtool复杂的参数。它提供了非常强大的数据和用户管理功能，可以指定每一个用户能查看树状结构、host以及任何一张图，还可以与LDAP结合进行用户验证，同时也能自己增加模板，功能非常强大完善。界面友好。 CACTI

软件 Cacti 的发展是基于让 RRDTool 使用者更方便使用该软件，除了基本的 Snmp 流量跟系统资讯监控外，Cacti 也可外挂 Scripts 及加上 Templates 来作出各式各样的监控图。

### 3 [nagios](http://www.ttlsa.com/monitor/nagios/)

Nagios是一款开源的免费网络监视工具，能有效监控[Windows](http://www.ttlsa.com/windows/)、[Linux](http://www.ttlsa.com/linux/)和Unix的主机状态，交换机路由器等网络设置，打印机等。在系统或服务状态异常时发出邮件或短信报警第一时间通知网站运维人员，在状态恢复后发出正常的邮件或短信通知。

Nagios 可以监控的功能有：
- 监控网络服务（SMTP、POP3、HTTP、NNTP、PING等）；
- 监控主机资源（处理器负荷、磁盘利用率等）；
- 简单地插件设计使得用户可以方便地扩展自己服务的检测方法；
- 并行服务检查机制；
- 具备定义网络分层结构的能力，用"parent"主机定义来表达网络主机间的关系，这种关系可被用来发现和明晰主机宕机或不可达状态；
- 当服务或主机问题产生与解决时将告警发送给联系人（通过EMail、短信、用户定义方式）；
- 可以定义一些处理程序，使之能够在服务或者主机发生故障时起到预防作用；
- 自动的日志滚动功能；
- 可以支持并实现对主机的冗余监控；
- 可选的WEB界面用于查看当前的网络状态、通知和故障历史、日志文件等；

### 4. ntop

Ntop是一种监控网络流量工具，用ntop显示网络的使用情况比其他一些网络管理软件更加直观、详细。Ntop甚至可以列出每个节点计算机的网络带宽利用率。他是一个灵活的、功能齐全的，用来监控和解决局域网问题的工具；尤其当ntop与nprobe配合使用，其功能更加显著。它同时提供命令行输入和web页面，可应用于嵌入式web服务

Ntop主要提供以下一些功能:
- 自动从网络中识别有用的信息；
- 将截获的数据包转换成易于识别的格式；
- 对网络环境中通信失败的情况进行分析；
- 探测网络通信的时间和过程

### 5. [zabbix](http://www.ttlsa.com/monitor/zabbix/)

zabbix是一个基于WEB界面的提供分布式系统监视以及网络监视功能的企业级的开源解决方案。zabbix能监视各种网络参数，保证服务器系统的安全运营；并提供柔软的通知机制以让系统管理员快速定位/解决存在的各种问题。zabbix由2部分构成，zabbix server与可选组件zabbix agent。zabbix server可以通过SNMP，zabbix agent，ping，端口监视等方法提供对远程服务器/网络状态的监视，数据收集等功能，它可以运行在Linux, Solaris, HP-UX, AIX,
 Free BSD, Open BSD, OS X等平台上。

