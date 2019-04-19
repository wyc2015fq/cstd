# CentOS 6.5下快速搭建ftp服务器 - sxf_123456的博客 - CSDN博客
2018年01月16日 13:04:33[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：181
**[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) 6.5下快速搭建ftp服务器**
1、用root 进入系统
2、使用命令 rpm  -qa|grep vsftpd 查看系统是否安装了ftp，若安装了vsftp，使用这个命令会在屏幕上显示vsftpd的版本 
3、使用命令rpm -e vsftpd 即可卸载ftp
4、再使用rpm  -qa|grep vsftpd 查看系统是否已删除ftp，若删除成功，屏幕上显示vsftpd的版本
一：安装vsftpd
查看是否已经安装vsftpd
rpm -qa | grep vsftpd
如果没有，就安装，并设置开机启动
yum -y install vsftpd
chkconfig vsftpd on
安装时发现错误：
Loaded plugins: fastestmirror, refresh-packagekit, security
Loading mirror speeds from cached hostfile
Could not retrieve mirrorlist http://mirrorlist.centos.org/?release=6&arch=x86_64&repo=os error was
14: PYCURL ERROR 6 - "Couldn't resolve host 'mirrorlist.centos.org'"
Error: Cannot find a valid baseurl for repo: base
是因为缺少DNS，解决如下：到/etc目录下配置resolv.conf加入nameserver IP：
[root@localhost ~]# vi /etc/resolv.conf
#下面地址是福建电信DNS
nameserver 218.85.157.99
管理vsftpd相关命令：
启动vsftpd:  service vsftpd start
停止vsftpd:  service vsftpd stop
重启vsftpd:  service vsftpd restart
二、配置防火墙
打开/etc/sysconfig/iptables文件
vi /etc/sysconfig/iptables
在REJECT行之前添加如下代码
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 21 -j ACCEPT
保存和关闭文件，重启防火墙
service iptables start
三、配置vsftpd服务器
1.默认的配置文件是/etc/vsftpd/vsftpd.conf，你可以用文本编辑器打开。
vi /etc/vsftpd/vsftpd.conf
2.添加ftp用户
下面是添加ftpuser用户，设置根目录为/home/wwwroot/ftpuser,禁止此用户登录SSH的权限，并限制其访问其它目录。
#chroot_list_enable=YES
# (default follows)
#chroot_list_file=/etc/vsftpd.chroot_list
改为
chroot_list_enable=YES
# (default follows)
chroot_list_file=/etc/vsftpd/chroot_list
3.增加用户ftpuser，指向目录/home/wwwroot/ftpuser,禁止登录SSH权限。
useradd -d /home/wwwroot/ftpuser -g ftp -s /sbin/nologin ftpuser
4.设置用户口令
passwd ftpuser
5、编辑文件chroot_list:
vi /etc/vsftpd/chroot_list
内容为ftp用户名,每个用户占一行,如：
peter
john
6、重新启动vsftpd
service vsftpd restart
另外，如果觉得以后管理ftp用户名嫌麻烦，可以使用centos官方发布的脚本管理。地址如下：（未用过）
http://wiki.centos.org/HowTos/Chroot_Vsftpd_with_non-system_users
----------------------------------
出现的错误
1、500 OOPS: cannot change directory
解决方法：
在终端输入命令：
1.setsebool -P ftpd_disable_trans 1
2.service vsftpd restart
就OK了！
原因：这是因为服务器开启了selinux，这限制了FTP的登录。
来源:http://www.linuxidc.com/Linux/2015-10/123848.htm
