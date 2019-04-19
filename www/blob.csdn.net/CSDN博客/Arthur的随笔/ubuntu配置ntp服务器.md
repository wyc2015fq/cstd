# ubuntu配置ntp服务器 - Arthur的随笔 - CSDN博客
2012年01月30日 21:55:36[largetalk](https://me.csdn.net/largetalk)阅读数：13401
一个多台机器构成系统，有时候会有时间同步的需求，比如应用服务器和数据库服务器，如果两则不一致的话，有时就会产生问题，所以需要一个时间同步的机制来保证每台机器的时间一致。
Network Time Protocol([NTP]()，网络时间协议)用于同步它所有客户端时钟的服务。NTP服务器将本地系统的时钟与一个公共的NTP服务器同步然后作为时间主机提供服务，使本地网络的所有客户端能同步时钟。
在ubuntu上只要sudo apt-get install ntp就可以安装好了，安装好之后有有一个ntpd的后台进程，因为ntp既是服务器又是客户端，如果只想要客户端功能可以把这个deamon停掉，使用crontab执行19
 * * * * /usr/sbin/ntpdate ntp.api.bz 就可以了（ntp.api.bz是ntp服务器地址，如局域网中有ntp服务器，也可以用该服务器的ip）。
一般一个局域网中需要建一个ntp服务器，其他机器同步该机器就可以了，配置ntp服务器也比较简单，编辑/etc/ntp.conf文件，如下：
**server ntp.api.bz**
server 0.ubuntu.pool.ntp.org
server 1.ubuntu.pool.ntp.org
server 2.ubuntu.pool.ntp.org
server 3.ubuntu.pool.ntp.org
# By default, exchange time with everybody, but don't allow configuration.
restrict -4 default kod notrap nomodify nopeer noquery
restrict -6 default kod notrap nomodify nopeer noquery
# Local users may interrogate the ntp server more closely.
restrict 127.0.0.1
restrict ::1
# Clients from this (example!) subnet have unlimited access, but only if
# cryptographically authenticated.
#restrict 192.168.123.0 mask 255.255.255.0 notrust
# If you want to provide time to your local subnet, change the next line.
# (Again, the address is an example only.)
**broadcast 172.16.120.255**
# If you want to listen to time broadcasts on your local subnet, de-comment the
# next lines.  Please do this only if you trust everybody on the network!
**disable authbroadcastclient**
上面加粗行就是文件中我所改动的，server是指定外部ntp server， broadcast是指对哪个局域网广播时间， 最后两行是指客户端不需要加密验证就可以同步改服务器。
需要注意的是ntp server重启之后需要比较长的一段时间客户端才能同步服务器，这段时间内客户端ntpdate会抛个no
 server suitable for synchronization found， 蛋疼
ps. [国内可用的NTP服务器地址列表
 网络时间 服务器](http://www.cnblogs.com/sys1263/archive/2008/08/06/1261658.html)
