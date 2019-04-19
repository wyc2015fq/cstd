# Can’t connect to MySQL server有关linux tcp连接优化 - 三少GG - CSDN博客
2014年09月19日 15:21:47[三少GG](https://me.csdn.net/scut1135)阅读数：1376
最近有2台web机器链接mysql有时会出现Can’t connect to MySQL server的情况，查看exception日志：
如下：
[message:protected] => SQLSTATE[HY000] [2003] Can’t connect to MySQL server on ‘192.168.1.248′(99)
1）：问题描述：
2台web机器，只有其中一台出现该exception，另外一台无此问题；
exception logs: Can’t connect to MySQL server on ‘*.*.*.*’ (99)
2）：问题排查：
1、查看mysql的variables,status:
max_connect_errors = 50000
max_connections = 1000
max_user_connections = 200
max_connect_errors = 50000
max_connections = 1000
max_user_connections = 200
2、ulimit -a:
3、查看netstat -anp
查看问题机器的netstat -anp，发现大部分都是TIME_WAIT，怀疑是tcp的问题；
将近有3万的网络套接字，此时CPU也耗尽，导致这么多的TCP/IP连接CPU处理不过来，而使部分TCP/IP连接超时或处理失败
3、OS：
more /etc/sysctl.conf:
net.ipv4.tcp_syncookies = 1
新的连接可以重新使用TIME-WAIT套接字
net.ipv4.tcp_tw_reuse=1
启动TIME-WAIT套接字状态的快速循环功能
net.ipv4.tcp_tw_recycle=1
套接字关闭时，保持FIN-WAIT-2状态的时间
net.ipv4.tcp_fin_timeout=30
对于所有协议的队列，设置最大系统发送缓存(wmen)和接收缓存(rmem)到8M
net.core.wmem_max=8388608
net.core.rmem_max=8388608
让参数生效。
3）：问题原因：
报错”Can’t connect to MySQL server on ‘*.*.*.*’ (99) ” 参考MySQL Client端错误代码说明：错误代码为99，99的含义：$perror 99 OS error code  99:  Cannot assign requested address 这是一个本地OS的抛错，表示无法分配本地地址资源（应该是端口），socket无法创建
” Cannot assign requested address”，多半是由于客户端请求过于频繁，而Server端练级关闭后本地暂时处于TIME_WAIT，所以暂时端口都不可用导致。因此修改下OS参数就ok了

有个应用就是每次都会去查一个接口，接口返回用户的信息数据，从而展现不同的页面效果。大致流程如下
应用APP（电信）-> memcache ->电信custom接口 ->master-db
应用APP（网通）-> 网通custom接口 -> slave-db
接口环境是php（cgi） + nginx，接口已经运行很久，未出过异常
应用访问custom接口，然后接口去查数据库（数据库是主从复制，数据同步，各自机房读各自的数据库，写的话都写master-db）
有一点，就是电信机房是有memcache层的，而网通机房一直没有（考虑到网通机房流量不高，并且机房cache不同步，从上线起就网通机房一直未使用cache）
有一次上线，这个上线的版本有个改动就是把电信机房的memcache也取消了，然后 电信机房流量暴增
看pv统计：
$ “find /path -name ‘access*’|xargs wc -l|awk ‘END{print$1}’” fe
cmd :find /path ‘access*’|xargs wc -l|awk ‘END{print }’
type:fe
server1
2倍A total（28号是Atotal）
———–
server2
2倍B（28号是B
 total）
———–
server3
C 总计
———–
server4
D total
….
other servers ….
网通机房流量一直比较稳定左右，从未出任何问题
就是昨天电信custom接口流量暴增后，出现了异常，电信机房机器负载涨了40多倍，QPS涨了15倍，直到凌晨0：24分才降到1以下
应用也报了短暂的超时警报，不过php和nginx运行还是比较蛋定，重启依然非常快，终端也没有出现很卡的情况
流量是前一天的9倍！
异常就是error.log在上线后飙到3个G！！！
而且错误全都是Can’t connect to MySQL server on ’1.1.1.1′ (99)
即便在命令行下用mysql -hx.x.x.x -u -p
也间歇性地连接不上，但是据dba描述，数据库监控无任何异常，数据库上其他部门的应用也无异常
不知是否机器负载过高导致大量time wait，导致mysql连接超时或连接不上
以下是晚上0点13分的监控：
TIME_WAIT 涨了300倍（不知是否和他有关）
ESTABLISHED 涨了10倍
按理说，custom的网通接口流量非常稳定，从未出现过异常，电信机房接口飙了2倍就抗不住了，load直线上升，
为了排除cache引起的流量导致接口异常，22：30左右重新上了2个文件，把电信机房的memcache重新开启，
开启后慢慢load是降了，但是mysql错误依旧只是没那么多了
现在去机器上看，还是大量错误，提取日志如下
FastCGI sent in stderr: “PHPWarning:  mysql_connect()Can’t connect to MySQL server on ’1.1.1.1′ (99)in
 XXXXXX on line xxx
后来跟dba不断沟通排查，发现电信机房和网通机房的/etc/sysctl.conf配置有所区别
网通机房多了下面几行
net.ipv4.tcp_syncookies =1
net.ipv4.tcp_tw_reuse = 1
net.ipv4.tcp_tw_recycle =1
net.ipv4.tcp_fin_timeout= 5
原因就在这，把配置同步到杭电信机房后，问题就解决了，总结如下：
- 问题描述- 上线异常导致qps：五倍+，负载：四十倍+，虽然nginx+php表示很淡定没挂，但error.log飙到了3G/天，全是Can’t connect to MySQL server on ‘*.*.*.*’ (99)
- 解决异常后，error.log日志少了，但TIME_WAIT依旧减不下去，数据库依旧连接是失败
- 问题排查- Mysql Config ? (no problem)- max_connect_errors = 50000 (no problem)
- max_connections = 1000 (no problem)
- max_user_connections = 950 (no problem)
- OS Config ? (problem, 按以下修改问题就解决了)- vi /etc/sysctl.conf
// 编辑
net.ipv4.tcp_syncookies = 1
**net.ipv4.tcp_tw_reuse = 1**
**net.ipv4.tcp_tw_recycle = 1**
net.ipv4.tcp_fin_timeout = 5
// 让参数生效
**/sbin/sysctl -p**
- 问题原因- 报错”Can’t connect to MySQL server on ‘*.*.*.*’ (99) ” 参考MySQL
 Client端[错误代码](http://dev.mysql.com/doc/refman/5.1/en/error-messages-client.html#error_cr_conn_host_error)说明：错误代码为99，99的含义：$perror 99 OS
 error code  99:  Cannot assign requested address 这是一个本地OS的抛错，表示无法分配本地地址资源（应该是端口），socket无法创建
- Google一下” Cannot assign requested address”，多半是由于客户端请求过于频繁，而Server端练级关闭后本地暂时处于TIME_WAIT，所以暂时端口都不可用导致。因此修改下OS参数就ok了
- 问题反思- 这个问题非常紧急么？紧急！- 参考文章《nginx+php产生大量TIME_WAIT》：[http://leven.blog.51cto.com/1675811/382097](http://leven.blog.51cto.com/1675811/382097)，遇到这样的问题，我们应该第一时间想到端口不可用，首先会导致nginx连不上php-cgi导致服务不可用，其次才是php-cgi连不上mysql，因此非常重要！
- 为什么mysql不使用长连接pconnection呢？- pconnection mysql占用大量资源，并且在大并发情况下，例如个性化，活动促销等，连接过多导致无数的连接失败error，并牵制apache（nginx）ThreadsPerChild的参数
- 高并发下的最佳实践？- apache短连接，nginx短连接，mysql短连接，虽然TIME_WAIT多了，但可通过修改OS内核加速TIME_WAIT的复用，经验之谈啊！
