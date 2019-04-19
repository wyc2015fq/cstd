# centos 6 7 differences 区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月23日 17:09:26[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：404

|命令|centos6|centos7|
|ifconfig|有|有 yum install -y net-tools|
|服务管理|chkconfig /etc/init.d/服务|systemctl|
systemctl start service_name

自启动
[CentOS6]
$ chkconfig service_name on/off
[CentOS7]
$ systemctl 
enable service_name
$ systemctl 
disable service_name
而对于启动脚本的存放位置，也不再是/etc/init.d/（这个目录也是存在的），而是/usr/lib/systemd/system/.
|kernel|2.6|3.10|
|网卡|eth0|成为了可预见性的命名规则|
|文件系统|ext4|xfs|
强制停止
[CentOS6]
$ kill -9 <PID>
[CentOS7]
$ systemctl kill --signal=9 sshd
|主机名|cat /etc/sysconfig/network|cat /etc/hostname|
|防火墙|iptables|被firewalld取代|
|开机执行文件/etc/rc.local|可放置开机要执行的脚本文件|默认这个文件没有执行权限 x官方推荐不使用这个文件(自己写服务)，但是你可以加+x继续使用。|
|内核参数配置文件|/etc/sysctl.conf|/usr/lib/sysctl.d/00-system.conf和/etc/sysctl.d/<name>.conf|
文件结构
[CentOS6] /bin, /sbin, /lib, and /lib64在/下
[CentOS7] /bin, /sbin, /lib, and /lib64移到/usr下
IP地址MAC地址
[CentOS6]
$ ifconfig -a
[CentOS7]
$ ip address show
关闭
[CentOS6]$ shutdown -h now
[CentOS7]
$ poweroff
$ systemctl poweroff
防火墙
systemctl status firewalld
systemctl stop firewalld
systemctl disable firewalld
继续使用老版防火墙：
systemctl stop firewalld
systemctl disable firewalld
yum installed -y iptables-services
systemctl enable iptables
systemctl start iptables
参考：
[https://putty.biz/760](https://putty.biz/760)
[http://cuidehua.blog.51cto.com/5449828/1858374](http://cuidehua.blog.51cto.com/5449828/1858374)
[http://pengjiezhang.blog.51cto.com/1506442/1868140](http://pengjiezhang.blog.51cto.com/1506442/1868140)

