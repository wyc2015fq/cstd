# ntpd时间同步 安装与配置 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月14日 10:40:04[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：606
1，安装
yum -y install ntp
vim /etc/ntp.conf
默认配置：
driftfile /var/lib/ntp/drift
restrict default kod nomodify notrap nopeer noquery
restrict -6 default kod nomodify notrap nopeer noquery
restrict 127.0.0.1
restrict -6 ::1
server 0.centos.pool.ntp.org iburst
server 1.centos.pool.ntp.org iburst
server 2.centos.pool.ntp.org iburst
server 3.centos.pool.ntp.org iburst
includefile /etc/ntp/crypto/pw
keys /etc/ntp/keys
修改：
允许任何IP的客户机都可以进行时间同步
将“restrict default kod nomodify notrap nopeer noquery”这行修改成：
restrict default nomodify
启动前先关闭防火墙
service ntpd start
客服端测试
ntpdate  192.168.1.101
 最好客户端安装ntpd，指定时间server为上面配置的服务器地址，推荐
server 192.168.1.101
ntpd是步进式的逐渐调整时间，测试时如果你的ntpd服务器时间是准的，客户端不一定要ntpd服务启动时就自动对准，但过一段时间，就同步了。
TODO
加上防火墙后测试：
