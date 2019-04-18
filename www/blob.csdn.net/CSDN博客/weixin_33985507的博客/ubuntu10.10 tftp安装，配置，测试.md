# ubuntu10.10 tftp安装，配置，测试 - weixin_33985507的博客 - CSDN博客
2013年06月21日 14:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
# ubuntu10.10 tftp安装，配置，测试
**成于坚持，败于止步**
虽然ubuntu/centos/redhat都是linux，但是内核其中存在一定的修改，所以对于tftp服务器的安装存在不同的命令。
在这里由于自己安装的是ubuntu （其中redhat，采用rpm方式安装）
**安装tftp-hpa tftpd-hpa xinetd**
**sudo apt-get install tftp-hpa tftpd-hpa xinetd**
在根目录下创建文件夹/tftpboot，我的根目录是/root
修改目录权限chmod -R 777 /root/tftpboot
1、修改tftp配置文件，如果没有就创建
**vim /etc/xinetd.d/tftp**
```
service tftp
{
	disable = no
	socket_type = dgram
	protocol = udp
	wait = yes
	user = root
	server = /usr/sbin/in.tftpd
	server_args = -s /root/tftpboot
	source = 11
	cps = 100 2
	flags =IPv4
}
```
2、修改inetd.conf文件
vim /etc/inetd.conf
```
tftp dgram udp wait nobody /usr/sbin/tcpd
/usr/sbin/in.tftpd /root/tftpboot
```
其中/root/tftpboot为 tftp共享目录
3、修改tftpd-hpa文件
**vim /etc/default/tftpd-hpa**
```
#RUN_DAEMON="no"
#OPTIONS="-s /tftpboot/root -c -p -U tftpd"
TFTP_USERNAME="tftp"
TFTP_DIRECTORY="/root/tftpboot"
TFTP_ADDRESS="0.0.0.0:69"
TFTP_OPTIONS="-l -c -s"
```
4.在/tftpboot/root下创建测试文件file
在测试之前要先启动服务器：
方法有两种：
1.service xinetd restart
2./etc/init.d/xinetd restart
#cd /tftpboot/root
#touch file
#chmod 777 file
测试一下 tftp服务：
#cd /
#tftp 127.0.0.1
tftp>get file
tftp>q
#ls
查看当前目录，发现xxx文件已在当前目录
**如果上述设置还不行的话，那么就要把selinux禁用掉：**
**vim /etc/selinux/config //如果没有selinux/config这个文件，则创建**
```
# This file controls the state of SELinux on the system.
# SELINUX= can take one of these three values:
# enforcing - SELinux security policy is enforced.
# permissive - SELinux prints warnings instead of enforcing.
# disabled - SELinux is fully disabled.
SELINUX=enforcing
# SELINUXTYPE= type of policy in use. Possible values are:
# targeted - Only targeted network daemons are protected.
# strict - Full SELinux protection.
SELINUXTYPE=targeted
```
把上面的SELINUX=enforcing 改为：SELINUX=disable 禁用SeLinux,然后reboot重启PC
重启后再进行tftp测试。
5、开发板测试：
确保/tftpboot目录存在，而且有访问权限（至少应该"dr-xr-xr-x"）。
或者把disable = yes 改为 disable = no
这样就已经打开了主机linux中的tftp服务。下面我们可以用uboot的tftp命令来传输文件到目标板的ram中运行。
我们在目标板用命令printenv查看ip地址。
serverip 192.168.2.1 //主机ip，也就是eth0，因为pc和开发板连接默认是以太网
ipaddr 192.168.2.10 //目标机也就是开发板的ip
setenv命令可以修改主机和目标机的ip地址。
#setenv serverip 192.168.2.1 //该命令可设置主机ip，该命令只是把设置保存到ram中，如果重启的话设置会重新回到原来的设置。这时我们用saveenv命令把设置保存到flash中。
#saveenv
配置好各个参数后，我们在uboot命令窗口中使用tftp命令把内核，文件系统拷贝到ram中。
首先 ping 192.168.2.1如果成功就进行下载，不过在ping以前需要在终端设置号eth0：sudo ifconfig eth0 192.168.2.1 netmask 255.255.255.0 up
#tftp 30008000 zImage_W35  //在主机 /tftpboot目录中的zImage文件下载到目标板内存的30008000地址中。
待续。。。。。
