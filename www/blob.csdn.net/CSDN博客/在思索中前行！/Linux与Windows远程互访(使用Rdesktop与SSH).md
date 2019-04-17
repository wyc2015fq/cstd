# Linux与Windows远程互访(使用Rdesktop与SSH) - 在思索中前行！ - CSDN博客





2014年12月08日 16:55:12[_Tham](https://me.csdn.net/txl16211)阅读数：1454









工作的时候经常使用Redhat系列系统，而平常娱乐文档都是在windows平台上进行。因此实现linux与windows远程互访也是很有必要的事情。


本文将介绍如何实现Linux与Windows的远程互访。正如其名，Windows是“视窗”操作系统，Windows的操作基本上都需要在图形界面下完成，Linux则所有的工作基本都可以通过文本界面完成（玩游戏看电影等娱乐活动除外）。因此，以下将介绍如何使用Linux中的Rdesktop远程访问Windows图形界面以及如何通过SSH远程连接Linux.


环境: Windows 7 + ssh --> Fedora Linux 9


Fedora Linux + Rdesktop --> Windows 7


1)Linux连接Windows


前提是Winodws允许用户进行远程访问，Fedora默认安装了Rdesktop软件包，因此可以直接使用Rdesktop命令，如果要连接ip为192.168.0.0的Windows主机，可以通过以下命令完成:

# rdesktop -u yournape -p password -g 1024*720 192.168.0.2



如果没有这个命令，表明没有安装Rdesktop软件包，可以通过yum 安装，具体方法是：


#su -


输入密码


#yum install rdesktop*


也可以通过编译源码安装，方式如下：


下载源码包：

[http://jaist.dl.sourceforge.net/sourceforge/rdesktop/rdesktop-1.5.0.tar.gz](http://jaist.dl.sourceforge.net/sourceforge/rdesktop/rdesktop-1.5.0.tar.gz)


　　编译安装如下：


　　[root@host12cx]# tar xvzfrdesktop-1.7.0.tar.gz


　　[root@host12 cx]# cdrdesktop-1.7.0


　　[root@host12 cx]# ./configure


　　[root@host12 cx]# make


　　[root@host12 cx]# make install


　　[root@host12 cx]#


　　安装成功后，在/usr/local/bin下生成了可执行的rdesktop 程序。


rdesktop 的使用很简单，可通过#rdesktop -h得到使用的帮助。



 rdesktop是一个开放源码的Window   NT中断服务器的客户端，它实现了远程桌面协议（RDP）。不像Citrix   ICA，它对于服务器并无扩展要求。 

![](http://www.rdesktop.org/rdesktop.gif)

rdesktop是linux下支持Windows远程桌面连接的客户端程序，在linux系统下可通过它远程访问Windows桌面，支持多种版本。rdesktop是sourceforge下支持GPL协议的一个开源项目，采用RDP（Remote Desktop Protocol,远程桌面协议），几乎可以连接windows的所有版本，诸如NT 4 Terminal Server,
 2000, XP, 2003, 2003 R2, Vista, 2008, 7, and 2008 R2等。目前，rdesktop可运行于所有的基于X window平台的Unix系统中,当前稳定版本是1.7.0.

具体使用方法要先打开终端，然后输入以下命令：

　　rdesktop -u yournape -p password -g 1024*720 192.168.0.2

rdesktop为使用远程桌面连接的命令；

　　-u 用户名，yourname处为目标客户端的用户名；

　　-p 客户端用户的密码；
　-g 分辨率， 中间用“x”连接，可省略，省略后默认为全屏显示；

　　192.168.0.1 目标客户端的IP地址


2)Windows连接Linux


此处的Linux指的是开启了ftpd,sshd等远程登录服务程序的主机，是否开启这些服务可以通过 pgrep ftpd, pgrep sshd查看，服务开启的命令为/etc/init.d/sshd start和/etc/init.d/ftpd start，同时/etc/ssh/sshd_config文件需要进行配置以允许远程登录。


连接使用的是SSH协议，支持SSH协议的软件有很多，如Putty,SercureCRT等，此处使用SecureCRT作为连接工具。


打开SecureCRT，生成RSA密钥，默认生成Identity和Identity.pub密钥对，其中Identity.pub要上传至Linux服务器，上传可以通过ftp完成，也可以通过SecureCRT的cz工具完成，上传完成以后需要在Linux服务器端用户的home目录中建立.ssh目录，将上传的密钥存放到此目录中，然后使用ssh-add将此密钥加入密钥认证中来，具体命令如下：


#ssh-add Identity.pub


它会提示你是否输入认证字符串，此处可以选择不输入，此时服务端的配置完成。


客户端建立快速连接，公钥选择使用个人存储证书完成，注意此处不能新生成公钥然后上传，它会提示文件系统不兼容的错误，输入ip地址，用户名密码之后，就可以通过ssh连接Linux主机了。


附：ssh连接乱码解决方法，在选项--》会话选项的外观中选择默认字符编码为UTF-8，同时选择字体为隶书等支持GB2312的字体


参考China Unix http://download.chinaunix.net/download/0009000/8874.shtml



