# RedHat 光盘RPM安装SSH等软件包 - xqhrs232的专栏 - CSDN博客
2016年04月28日 13:39:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1831
原文安装::[http://blog.csdn.net/zhifeiyu2008/article/details/8153693](http://blog.csdn.net/zhifeiyu2008/article/details/8153693)
相关文章
1、[Linux ssh服务安装与启动](http://blog.csdn.net/rumenxinshou/article/details/31732743)----[http://blog.csdn.net/rumenxinshou/article/details/31732743](http://blog.csdn.net/rumenxinshou/article/details/31732743)
2、Linux系统下如何配置安装SSH服务?如何开启SSH服务?----[http://www.jb51.net/LINUXjishu/308897.html](http://www.jb51.net/LINUXjishu/308897.html)
3、Linux下用来查询安装包信息的RPM选项----[http://os.51cto.com/art/201311/418865.htm](http://os.51cto.com/art/201311/418865.htm)
安装完RedHat 5.3(32bit)后，由于最简安装，发现SSH没有安装。
#ssh
ssh:command not found
即SSH软件未安装（至少客户端未安装）。刚装完系统，光驱还接在服务器上，用光盘安装吧。
登入系统，挂载光盘：
#mkdir /mnt/cdrom    [先建一个供挂载的目录，名字"cdrom"随便取一个]
#mount -t iso9660 /dev/cdrom /mnt/cdrom
会提示cdrom为只读，正常情况。接着找要安装软件包，不知道在哪个目录的话可以用find命令：
#find /mnt/cdrom | grep openssh
会弹出几条找到的软件包名字的列表，redhat5的RPM包在./Server目录下:
#cd /mnt/cdrom/Server
#ls openssh*
openssh-4.3p2-36.el5.i386.rpm
openssh-askpass-4.3p2-36.el5.i386.rpm
openssh-clients-4.3p2-36.el5.i386.rpm
openssh-server-4.3p2-36.el5.i386.rpm
列出了Server目录下所有以openssh开头的RPM包列表，下面就是选择安装了：
#rpm -ivh openssh-4.3p2-36.el5.i386.rpm
#rpm -ivh openssh-clients-4.3p2-36.el5.i386.rpm    [客户端]
#rpm -ivh openssh-server-4.3p2-36.el5.i386.rpm    [服务端]
按照ls出来的RPM包具体版本安装上述3个包，会显示进度条100%安装完成。然后就可以使用SSH远程管理了。
如果还不行尝试一下：
1、重启ssh服务 /etc/init.d/sshd restart
2、重新配置防火墙或关闭防火墙 /etc/init.d/iptables stop
OK!
同上，RPM安装其他软件包，方法类似。
