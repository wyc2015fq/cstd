
# 网络协议：NTP：2：使用NTPD搭建NTP服务器 - 知行合一 止于至善 - CSDN博客

2019年03月14日 07:20:08[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：315标签：[ntpd																](https://so.csdn.net/so/search/s.do?q=ntpd&t=blog)[ntp																](https://so.csdn.net/so/search/s.do?q=ntp&t=blog)[时间同步																](https://so.csdn.net/so/search/s.do?q=时间同步&t=blog)[网络时间协议																](https://so.csdn.net/so/search/s.do?q=网络时间协议&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=时间同步&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=ntp&t=blog)个人分类：[Unix/Linux																](https://blog.csdn.net/liumiaocn/article/category/6328278)
[
																								](https://so.csdn.net/so/search/s.do?q=ntp&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=ntpd&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=ntpd&t=blog)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
虽然NTPD已经不再是CentOS缺省的时间同步软件，由于其长时间占据这个地位，还是将其作为网络时间同步搭建的工具之一来进行介绍。
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# NTP基础知识
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)基础知识请参看如下文章，此处不再赘述。
[
](https://img-blog.csdnimg.cn/20190312094832313.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)[https://liumiaocn.blog.csdn.net/article/details/88414432](https://liumiaocn.blog.csdn.net/article/details/88414432)
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
> 安装命令：yum install -y ntp

> 注：包含了ntp和ntpdate，在本文的示例中客户端和服务器端均采用同样的安装包

> [
> root@host131 ~
> ]
> \# rpm -qa |grep ntp
> ntp-4.2.6p5-28.el7.centos.x86_64
ntpdate-4.2.6p5-28.el7.centos.x86_64
> [
> root@host131 ~
> ]
> \# ntpd --version
> ntpd 4.2.6p5
> exit
> 0
> [
> root@host131 ~
> ]
> \#
> 端口开放：如果有防火墙的话，需要注意ntpd的服务是使用UDP方式在123端口进行服务。要么关闭，要么加入规则对端口放行。

> firewall设定：

> firewall-cmd --add-service=ntp --permanent

> firewall-cmd --reload

> 安装设定

> 设定文件：/etc/ntp.conf

> 设定文件ntp.conf是ntpd服务的重中之重，设定格式如下所示：

> 设定格式：

> 设定指令 指令参数

> 设定指令：server

> 使用介绍：

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

> 设定指令：fudge

> 说明：fudge英文含义具有“欺骗”的意思。这条指令必须紧跟server指令，为server指令提供附加信息。

> 使用示例：

> server 127.127.1.0

> fudge  127.127.1.0 stratum 5

> 场景说明：指定本地时钟的层为5，在无法取到外部服务的时候使用本地时钟，在无法连接外网的局域网环境中运用较多。此时一般将其他的server设定全部删除或注释即可。

> 设定指令：logfile

> 参数介绍：日志的FULL PATH

> 说明：默认情况下，NTP服务器的日志保存在/var/log/messages中，但也可自行指定

> 使用例：logfile /var/log/ntpd.log

> 设定指令：driftfile

> 参数说明：/var/lib/ntp/drift

> 说明：根据实际时间计算出服务器增减时间的比率，通过指定文件进行记录，调整时间时会根据此文件进行补偿。此指令一般无需修改

> 设定指令：restrict

> 使用介绍

> restrict [地址] mask [子网掩码] [参数]

> 参数说明：ignore

> 说明: 拒绝所有类型的NTP连接

> 参数说明：nomodify

> 说明: 客户端无法修改服务器端时间

> 参数说明：noquery

> 说明: 客户端无法查询服务器端时间

> 参数说明：notrap

> 说明: 不提供远程日志功能

> 参数说明：notrust

> 说明: 不允许接入没有认证的客户端

> 参数说明：nopeer

> 说明: 不允许和同一层的其他服务器进行时间同步

> 使用示例：

> restrict 192.168.163.131 mask 255.255.255.0 nomodify notrap

> 说明：这也是缺省的ntp.conf被注释的使用例，允许192.168.163.*的机器进行连接，但是不允许修改主机时间，不提供远程日志功能

> 设定文件：/etc/ntp/step-tickers

> 用于记录使用ntpdate同步时间时用到的NTP服务器信息

> [
> root@host131 ~
> ]
> \# cat /etc/ntp/step-tickers
> \# List of NTP servers used by the ntpdate service.
> 0.centos.pool.ntp.org
> [
> root@host131 ~
> ]
> \#
> 设定示例(服务器侧)

> 在本文中，进行最小的修改启动ntpd，场景为无法连接外网的NTP服务器和NTP客户端。承担NTP服务器的host131通过server和fudge指令进行设定。内容的修改仅限于如下三点：

> restrict的IP修改

> 外网的NTP源注释掉

> 加入支持本地时钟的server与fudge

> [
> root@host131 ~
> ]
> \# cp /etc/ntp.conf /etc/ntp.conf.org
> [
> root@host131 ~
> ]
> \# vi /etc/ntp.conf
> [
> root@host131 ~
> ]
> \# diff /etc/ntp.conf /etc/ntp.conf.org
> 17c17
> <
> restrict 192.168.163.0 mask 255.255.255.0 nomodify notrap
---
> >
> \#restrict 192.168.1.0 mask 255.255.255.0 nomodify notrap
> 21,27c21,24
> <
> \#server 0.centos.pool.ntp.org iburst
> <
> \#server 1.centos.pool.ntp.org iburst
> <
> \#server 2.centos.pool.ntp.org iburst
> <
> \#server 3.centos.pool.ntp.org iburst
> <
> <
> server 127.127.1.0
> <
> fudge  127.127.1.0 stratum 5
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
> root@host131 ~
> ]
> \#
> 启动ntpd服务

> [
> root@host131 ~
> ]
> \# systemctl start ntpd
> [
> root@host131 ~
> ]
> \#
> 结果确认

> [
> root@host131 ~
> ]
> \# systemctl status ntpd
> ● ntpd.service - Network Time Service
   Loaded: loaded
> (
> /usr/lib/systemd/system/ntpd.service
> ;
> disabled
> ;
> vendor preset: disabled
> )
> Active: active
> (
> running
> )
> since Thu 2019-02-14 22:59:15 EST
> ;
> 10s ago
  Process: 11775 ExecStart
> =
> /usr/sbin/ntpd -u ntp:ntp
> $OPTIONS
> (
> code
> =
> exited, status
> =
> 0/SUCCESS
> )
> Main PID: 11776
> (
> ntpd
> )
> CGroup: /system.slice/ntpd.service
           └─11776 /usr/sbin/ntpd -u ntp:ntp -g
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : Listen and drop on 1 v6wildcard :: UDP 123
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : Listen normally on 2 lo 127.0.0.1 UDP 123
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : Listen normally on 3 enp0s3 192.168.163.131 UDP 123
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : Listen normally on 4 lo ::1 UDP 123
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : Listen normally on 5 enp0s3 fe80::853e:84d1:5e1:6303 UDP 123
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : Listening on routing socket on fd
> \#22 for interface updates
> Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : 0.0.0.0 c016 06 restart
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : 0.0.0.0 c012 02 freq_set kernel 0.000 PPM
Feb 14 22:59:15 host131 ntpd
> [
> 11776
> ]
> : 0.0.0.0 c011 01 freq_not_set
Feb 14 22:59:16 host131 ntpd
> [
> 11776
> ]
> : 0.0.0.0 c514 04 freq_mode
> [
> root@host131 ~
> ]
> \#
> 添加到自启动列表

> [
> root@host131 ~
> ]
> \# systemctl enable ntpd
> Created symlink from /etc/systemd/system/multi-user.target.wants/ntpd.service to /usr/lib/systemd/system/ntpd.service.
> [
> root@host131 ~
> ]
> \#
> 结果确认

> 使用ntpq进行结果确认，可以看到确实是本地同步（LOCAL），同时stratum也被设定为5。 因为是本地，所以offset也都是0。

> [
> root@host131 ~
> ]
> \# ntpq -p
> remote           refid      st t when poll reach   delay   offset  jitter
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
> *LOCAL
> (
> 0
> )
> .LOCL.           5 l   41   64    7    0.000    0.000   0.000
> [
> root@host131 ~
> ]
> \#
> 设定示例（客户端）

> 客户端的时间同步一般来说有两种方式：

> *方式1: 结合crontab的定时任务，使用ntpdate连接NTP服务器进行时间同步

> *方式2: 设定ntpd，将server设定为内网的NTP服务器，可自动同步

> 此处采用方式2进行设定。

> 事前准备

> 安装命令：yum install -y ntp

> 注：包含了ntp和ntpdate，在本文的示例中客户端和服务器端均采用同样的安装包

> [
> root@host131 ~
> ]
> \# rpm -qa |grep ntp
> ntp-4.2.6p5-28.el7.centos.x86_64
ntpdate-4.2.6p5-28.el7.centos.x86_64
> [
> root@host131 ~
> ]
> \# ntpd --version
> ntpd 4.2.6p5
> exit
> 0
> [
> root@host131 ~
> ]
> \#
> 设定文件修改

> 修改内容如下：

> [
> root@host132 ~
> ]
> \# cp /etc/ntp.conf /etc/ntp.conf.org
> [
> root@host132 ~
> ]
> \# vi /etc/ntp.conf
> [
> root@host132 ~
> ]
> \# diff /etc/ntp.conf /etc/ntp.conf.org
> 21,25c21,24
> <
> \#server 0.centos.pool.ntp.org iburst
> <
> \#server 1.centos.pool.ntp.org iburst
> <
> \#server 2.centos.pool.ntp.org iburst
> <
> \#server 3.centos.pool.ntp.org iburst
> <
> server 192.168.163.131
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
> 启动并确认ntpd服务

> [
> root@host132 ~
> ]
> \# systemctl start ntpd
> [
> root@host132 ~
> ]
> \# systemctl status ntpd
> ● ntpd.service - Network Time Service
   Loaded: loaded
> (
> /usr/lib/systemd/system/ntpd.service
> ;
> disabled
> ;
> vendor preset: disabled
> )
> Active: active
> (
> running
> )
> since Thu 2019-02-14 23:15:13 EST
> ;
> 5s ago
  Process: 11592 ExecStart
> =
> /usr/sbin/ntpd -u ntp:ntp
> $OPTIONS
> (
> code
> =
> exited, status
> =
> 0/SUCCESS
> )
> Main PID: 11593
> (
> ntpd
> )
> CGroup: /system.slice/ntpd.service
           └─11593 /usr/sbin/ntpd -u ntp:ntp -g
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listen and drop on 0 v4wildcard 0.0.0.0 UDP 123
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listen and drop on 1 v6wildcard :: UDP 123
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listen normally on 2 lo 127.0.0.1 UDP 123
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listen normally on 3 enp0s3 192.168.163.132 UDP 123
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listen normally on 4 lo ::1 UDP 123
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listen normally on 5 enp0s3 fe80::310b:d919:625d:a6f4 UDP 123
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : Listening on routing socket on fd
> \#22 for interface updates
> Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : 0.0.0.0 c016 06 restart
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : 0.0.0.0 c012 02 freq_set kernel 0.000 PPM
Feb 14 23:15:13 host132 ntpd
> [
> 11593
> ]
> : 0.0.0.0 c011 01 freq_not_set
> [
> root@host132 ~
> ]
> \#
> 确认同步信息

> 使用ntpq命令，可以看到连接的NTP服务器为192.168.163.131

> [
> root@host132 ~
> ]
> \# ntpq -p
> remote           refid      st t when poll reach   delay   offset  jitter
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
> host131         .INIT.          16 u    -   64    0    0.000    0.000   0.000
> [
> root@host132 ~
> ]
> \#
> 但是继续确认，发现其实时间并未同步, 原因在于本机时间比host131更新。按照ntp的机制，这种情况是不会同步的。

> [
> root@host132 ~
> ]
> \# date; ssh host131 date
> Thu Feb 14 23:20:04 EST 2019
Thu Feb 14 23:19:36 EST 2019
> [
> root@host132 ~
> ]
> \#
> 手动使用ntpdate进行时间同步，收到如下错误提示：

> [
> root@host132 ~
> ]
> \# ntpdate host131
> 14 Feb 23:21:39 ntpdate
> [
> 11723
> ]
> : the NTP socket is
> in
> use, exiting
> [
> root@host132 ~
> ]
> \#
> 这也是相较于chrony被诟病的原因之一，这里先停下服务，然后再进行手动时间同步。

> [
> root@host132 ~
> ]
> \# systemctl stop ntpd
> [
> root@host132 ~
> ]
> \# ntpdate host131
> 14 Feb 23:26:05 ntpdate
> [
> 11804
> ]
> : step
> time
> server 192.168.163.131 offset -28.203650 sec
> [
> root@host132 ~
> ]
> \#
> 再次重启，然后使用ntpd进行确认

> [
> root@host132 ~
> ]
> \# ntpq -p
> remote           refid      st t when poll reach   delay   offset  jitter
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
> host131         LOCAL
> (
> 0
> )
> 6 u   13   64    1    0.384   -0.006   0.000
> [
> root@host132 ~
> ]
> \#
> 注意：offset是显示时间同步效果的重要标尺，单位为毫秒，需要多多观测。

> 再次确认两台机器的时间信息，发现同步的比较不错

> [
> root@host132 ~
> ]
> \# date; ssh host131 date
> Thu Feb 14 23:36:37 EST 2019
Thu Feb 14 23:36:37 EST 2019
> [
> root@host132 ~
> ]
> \#
> 常见问题

> 现象： no server suitable for synchronization found

> 可能原因：检查NTP服务器和NTP客户端两侧的firewalld是否正确设定，只是验证原因的情况下可以先systemctl stop firewalld将其关闭。

> 参考文章

> [http://doc.ntp.org/](http://doc.ntp.org/)


