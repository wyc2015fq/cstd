
# 网络协议：NTP：3：使用Chrony搭建NTP服务器 - 知行合一 止于至善 - CSDN博客

2019年03月15日 06:28:27[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：287


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
作为CentOS上缺省的NTPD的同步服务，Chrony俨然已经成为了NTPD的继任者。这篇文章将介绍一下如何使用Chrony搭建NTPD服务器和客户端。
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# NTP基础知识
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)基础知识请参看如下文章，此处不再赘述。
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)[https://liumiaocn.blog.csdn.net/article/details/88414432](https://liumiaocn.blog.csdn.net/article/details/88414432)
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Why Chrony
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)已经有NTPD了为什么还需要chrony呢，自然是有一定原因的，比如以下原因
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)在NTP服务器因为种种原因断断续续可连接的情况，NTPD效果不好
时间相差过大，需要手工执行ntpdate同步时间时需要先停下客户端的ntpd，手工操作之后再重启
参数设定较为复杂，不易理解
…
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 事前准备
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|用途|IP|OS版本|
|---|---|---|
|NTP服务器|192.168.163.131|CentOS7.6|
|NTP客户端|192.168.163.132|CentOS7.6|
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
# cat /etc/redhat-release
```
```python
CentOS Linux release 7.6.1810
```
```python
(
```
```python
Core
```
```python
)
```
```python
[
```
```python
root@host131 ~
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 安装准备
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 安装命令：yum install -y chrony

> 注：chrony缺省已被安装和设定，一般此命令不必执行

> [
> root@host131 ~
> ]
> \# rpm -qa |grep chrony
> chrony-3.2-2.el7.x86_64
> [
> root@host131 ~
> ]
> \# chronyd --version
> chronyd
> (
> chrony
> )
> version 3.2
> (
> +CMDMON +NTP +REFCLOCK +RTC +PRIVDROP +SCFILTER +SECHASH +SIGND +ASYNCDNS +IPV6 +DEBUG
> )
> [
> root@host131 ~
> ]
> \#
> 端口开放：如果有防火墙的话，需要注意chrony的服务是使用UDP方式在123端口进行服务。要么关闭，要么加入规则对端口放行。

> firewall设定：

> firewall-cmd --add-service=ntp --permanent

> firewall-cmd --reload

> 安装设定

> 设定文件：/etc/chrony.conf

> 设定文件chrony.conf是chrony服务的重中之重，设定格式如下所示：

> 设定格式：

> 设定指令 指令参数

> 设定指令：server

> 使用介绍：

> VS ntpd: 常用方式基本一致

> 使用 server ip或者server hostname方式进行设定

> 设定例：server 192.168.163.131

> 设定例：server host131

> 参数介绍：prefer

> perfer参数用于设置优先使用的NTP服务器

> 设定例：

> server 192.168.163.131 prefer

> server 192.168.163.132

> 设定说明：默认使用192.168.163.131进行同步，192.168.163.132为备用NTP服务器

> 参数介绍：burst

> 当NTP服务器可用时，向其发送一些列并发包进行检测

> 设定例：server 192.168.163.131 burst

> 参数介绍：iburst

> 当NTP服务器不可用时，向其发送一些列并发包进行检测

> 设定例：server 192.168.163.131 iburst

> 设定指令：logdir

> 参数介绍：日志的FULL PATH

> VS ntpd: logfile

> 说明：默认情况下，NTP服务器的日志保存在/var/log/messages中，但也可自行指定

> 使用例：logdir /var/log/chrony

> 设定指令：local

> VS ntpd: fudge + server

> 说明：简化本地时钟作为NTP源

> 使用示例：

> local stratum 5

> 场景说明：指定本地时钟的层为5，在无法取到外部服务的时候使用本地时钟，在无法连接外网的局域网环境中运用较多。此时一般将其他的server设定全部删除或注释即可。

> 设定指令：driftfile

> VS ntpd: 使用方式基本一致，一般同样无需修改

> 参数说明：/var/lib/chrony/drift

> 说明：根据实际时间计算出服务器增减时间的比率，通过指定文件进行记录，调整时间时会根据此文件进行补偿。此指令一般无需修改

> 设定指令：allow/deny

> VS ntpd: restrict

> 使用介绍

> 黑白名单，简化restrict常用的部分主要功能

> 使用示例：

> allow 192.168.0.0/16

> 说明：这也是缺省的ntp.conf被注释的使用例，允许192.168.*.*的机器进行连接,只需打开注释即可。

> 设定指令： makestep

> 使用介绍

> 根据需求偏移量的阈值进行调整同步速度的步伐

> 使用示例：

> makestep 1.0 3

> 说明：当偏移量offset大于1秒时，更新的步数为3

> 设定指令：rtcsync

> 使用介绍

> 开启内核模式，将定期同步到RTC硬件时钟

> 设定示例(服务器侧)

> 在本文中，进行最小的修改启动ntpd，场景为无法连接外网的NTP服务器和NTP客户端。承担NTP服务器的host131通过server和fudge指令进行设定。内容的修改仅限于如下三点：

> allow的白名单注释直接打开

> 外网的NTP源注释掉

> 本地设定方式直接打开，并调整stratum

> [
> root@host131 chrony
> ]
> \# cp /etc/chrony.conf /etc/chrony.conf.org
> [
> root@host131 chrony
> ]
> \# vi /etc/chrony.conf
> [
> root@host131 chrony
> ]
> \# diff /etc/chrony.conf /etc/chrony.conf.org
> 3,6c3,6
> <
> \#server 0.centos.pool.ntp.org iburst
> <
> \#server 1.centos.pool.ntp.org iburst
> <
> \#server 2.centos.pool.ntp.org iburst
> <
> \#server 3.centos.pool.ntp.org iburst
> ---
> >
> server 0.centos.pool.ntp.org iburst
> >
> server 1.centos.pool.ntp.org iburst
> >
> server 2.centos.pool.ntp.org iburst
> >
> server 3.centos.pool.ntp.org iburst
26c26
> <
> allow 192.168.0.0/16
---
> >
> \#allow 192.168.0.0/16
> 29c29
> <
> local stratum 5
---
> >
> \#local stratum 10
> [
> root@host131 chrony
> ]
> \#
> 启动chronyd服务

> [
> root@host131 ~
> ]
> \# systemctl restart chronyd
> [
> root@host131 ~
> ]
> \#
> 结果确认

> [
> root@host131 chrony
> ]
> \# systemctl status chronyd
> ● chronyd.service - NTP client/server
   Loaded: loaded
> (
> /usr/lib/systemd/system/chronyd.service
> ;
> enabled
> ;
> vendor preset: enabled
> )
> Active: active
> (
> running
> )
> since Fri 2019-02-15 00:45:27 EST
> ;
> 48s ago
     Docs: man:chronyd
> (
> 8
> )
> man:chrony.conf
> (
> 5
> )
> Process: 12116 ExecStartPost
> =
> /usr/libexec/chrony-helper update-daemon
> (
> code
> =
> exited, status
> =
> 0/SUCCESS
> )
> Process: 12113 ExecStart
> =
> /usr/sbin/chronyd
> $OPTIONS
> (
> code
> =
> exited, status
> =
> 0/SUCCESS
> )
> Main PID: 12115
> (
> chronyd
> )
> CGroup: /system.slice/chronyd.service
           └─12115 /usr/sbin/chronyd
Feb 15 00:45:27 host131 systemd
> [
> 1
> ]
> : Starting NTP client/server
> ..
> .
Feb 15 00:45:27 host131 chronyd
> [
> 12115
> ]
> : chronyd version 3.2 starting
> (
> +CMDMON +NTP +REFCLOCK +RTC +PRIVDROP +SCFILTER +SECHASH
> ..
> .+DEBUG
> )
> Feb 15 00:45:27 host131 systemd
> [
> 1
> ]
> : Started NTP client/server.
Hint: Some lines were ellipsized, use -l to show
> in
> full.
> [
> root@host131 chrony
> ]
> \#
> 结果确认

> 使用命令chronyc sources -v进行同步信息的确认。

> [
> root@host131 chrony
> ]
> \# chronyc sources -v
> 210 Number of sources
> =
> 0
  .-- Source mode
> '^'
> =
> server,
> '='
> =
> peer,
> '\#'
> =
> local clock.
 / .- Source state
> '*'
> =
> current synced,
> '+'
> =
> combined ,
> '-'
> =
> not combined,
> |
> /
> '?'
> =
> unreachable,
> 'x'
> =
> time
> may be
> in
> error,
> '~'
> =
> time
> too variable.
> ||
> .- xxxx
> [
> yyyy
> ]
> +/- zzzz
> ||
> Reachability register
> (
> octal
> )
> -.
> |
> xxxx
> =
> adjusted offset,
> ||
> Log2
> (
> Polling interval
> )
> --.
> |
> |
> yyyy
> =
> measured offset,
> ||
> \
> |
> |
> zzzz
> =
> estimated error.
> ||
> |
> |
> \
MS Name/IP address         Stratum Poll Reach LastRx Last sample
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> =
> [
> root@host131 chrony
> ]
> \#
> 设定示例（客户端）

> 客户端的时间同步一般来说有两种方式：

> *方式1: 结合crontab的定时任务，使用ntpdate连接NTP服务器进行时间同步

> *方式2: 设定chronyd，将server设定为内网的NTP服务器，可自动同步

> 注意：chrony也是支持ntpdate进行使用的

> 此处采用方式2进行设定。

> 事前准备

> 安装命令：yum install -y chrony

> 注：缺省无需安装

> [
> root@host132 ~
> ]
> \# rpm -qa |grep chrony
> chrony-3.2-2.el7.x86_64
> [
> root@host132 ~
> ]
> \# chronyd --version
> chronyd
> (
> chrony
> )
> version 3.2
> (
> +CMDMON +NTP +REFCLOCK +RTC +PRIVDROP +SCFILTER +SECHASH +SIGND +ASYNCDNS +IPV6 +DEBUG
> )
> [
> root@host132 ~
> ]
> \#
> 设定文件修改

> 修改内容如下：

> [
> root@host132 ~
> ]
> \# cp /etc/chrony.conf /etc/chrony.conf.org
> [
> root@host132 ~
> ]
> \# vi /etc/chrony.conf
> [
> root@host132 ~
> ]
> \# diff /etc/chrony.conf /etc/chrony.conf.org
> 3,7c3,6
> <
> \#server 0.centos.pool.ntp.org iburst
> <
> \#server 1.centos.pool.ntp.org iburst
> <
> \#server 2.centos.pool.ntp.org iburst
> <
> \#server 3.centos.pool.ntp.org iburst
> <
> server 192.168.163.131 iburst
---
> >
> server 0.centos.pool.ntp.org iburst
> >
> server 1.centos.pool.ntp.org iburst
> >
> server 2.centos.pool.ntp.org iburst
> >
> server 3.centos.pool.ntp.org iburst
> [
> root@host132 ~
> ]
> \#
> 启动并确认chronyd服务

> [
> root@host132 ~
> ]
> \# systemctl restart chronyd
> [
> root@host132 ~
> ]
> \# systemctl status chronyd
> ● chronyd.service - NTP client/server
   Loaded: loaded
> (
> /usr/lib/systemd/system/chronyd.service
> ;
> enabled
> ;
> vendor preset: enabled
> )
> Active: active
> (
> running
> )
> since Fri 2019-02-15 00:57:10 EST
> ;
> 4s ago
     Docs: man:chronyd
> (
> 8
> )
> man:chrony.conf
> (
> 5
> )
> Process: 11988 ExecStartPost
> =
> /usr/libexec/chrony-helper update-daemon
> (
> code
> =
> exited, status
> =
> 0/SUCCESS
> )
> Process: 11985 ExecStart
> =
> /usr/sbin/chronyd
> $OPTIONS
> (
> code
> =
> exited, status
> =
> 0/SUCCESS
> )
> Main PID: 11987
> (
> chronyd
> )
> CGroup: /system.slice/chronyd.service
           └─11987 /usr/sbin/chronyd
Feb 15 00:57:10 host132 systemd
> [
> 1
> ]
> : Starting NTP client/server
> ..
> .
Feb 15 00:57:10 host132 chronyd
> [
> 11987
> ]
> : chronyd version 3.2 starting
> (
> +CMDMON +NTP +REFCLOCK +RTC +PRIVDROP +SCFILTER +SECHASH
> ..
> .+DEBUG
> )
> Feb 15 00:57:10 host132 chronyd
> [
> 11987
> ]
> : Initial frequency -0.101 ppm
Feb 15 00:57:10 host132 systemd
> [
> 1
> ]
> : Started NTP client/server.
Feb 15 00:57:14 host132 chronyd
> [
> 11987
> ]
> : Selected
> source
> 192.168.163.131
Hint: Some lines were ellipsized, use -l to show
> in
> full.
> [
> root@host132 ~
> ]
> \#
> 确认同步信息

> 使用chronyc命令，可以看到连接的NTP服务器为192.168.163.131

> [
> root@host132 ~
> ]
> \# chronyc sources -v
> 210 Number of sources
> =
> 1
  .-- Source mode
> '^'
> =
> server,
> '='
> =
> peer,
> '\#'
> =
> local clock.
 / .- Source state
> '*'
> =
> current synced,
> '+'
> =
> combined ,
> '-'
> =
> not combined,
> |
> /
> '?'
> =
> unreachable,
> 'x'
> =
> time
> may be
> in
> error,
> '~'
> =
> time
> too variable.
> ||
> .- xxxx
> [
> yyyy
> ]
> +/- zzzz
> ||
> Reachability register
> (
> octal
> )
> -.
> |
> xxxx
> =
> adjusted offset,
> ||
> Log2
> (
> Polling interval
> )
> --.
> |
> |
> yyyy
> =
> measured offset,
> ||
> \
> |
> |
> zzzz
> =
> estimated error.
> ||
> |
> |
> \
MS Name/IP address         Stratum Poll Reach LastRx Last sample
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> ==
> =
> ^? host131                       5   6     3     0    -53us
> [
> -53us
> ]
> +/-  195us
> [
> root@host132 ~
> ]
> \#
> 再次确认两台机器的时间信息，发现同步的比较不错

> [
> root@host132 ~
> ]
> \# date; ssh host131 date
> Fri Feb 15 00:59:46 EST 2019
Fri Feb 15 00:59:46 EST 2019
> [
> root@host132 ~
> ]
> \#
> chronyc提供了更多功能，比如通过tracking来确认详细信息

> [
> root@host132 ~
> ]
> \# chronyc tracking
> Reference ID
> :
> C0A8A383
> (
> host131
> )
> Stratum
> :
> 6
Ref
> time
> (
> UTC
> )
> :
> Fri Feb 15 06:07:17 2019
System
> time
> :
> 0.000008046 seconds fast of NTP
> time
> Last offset
> :
> +0.000006541 seconds
RMS offset
> :
> 0.000018217 seconds
Frequency
> :
> 0.037 ppm fast
Residual freq
> :
> +0.002 ppm
Skew
> :
> 0.140 ppm
Root delay
> :
> 0.000354680 seconds
Root dispersion
> :
> 0.000007274 seconds
Update interval
> :
> 64.6 seconds
Leap status
> :
> Normal
> [
> root@host132 ~
> ]
> \#
> 使用ntpdate进行手动同步

> 使用ntpdate可以连接chrony进行手动同步，也不需要像ntpd那样要先停下本机的daemon进程了。

> [
> root@host132 ~
> ]
> \# ntpdate host131
> 15 Feb 01:02:03 ntpdate
> [
> 12040
> ]
> : adjust
> time
> server 192.168.163.131 offset 0.000003 sec
> [
> root@host132 ~
> ]
> \#
> 总结

> chrony使用起来比ntpd确实更为简单，而且考虑到是ntpd的改进以及成为了缺省的选项，比如makestep和allow/deny等理解简单，使用方便的功能，在使用CentOS或者RHEL发行版的Linux可考虑将其作为NTP服务的首选了。

> 参考文章

> [https://chrony.tuxfamily.org/doc/devel/chrony.conf.html](https://chrony.tuxfamily.org/doc/devel/chrony.conf.html)


