
# Linux -bin, -sbin, -usr-bin, -usr-sbin 区别 - 阳光岛主 - CSDN博客

2013年08月10日 04:05:57[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：15156



在linux下我们经常用到的四个应用程序的目录是：/bin、/sbin、/usr/bin、/usr/sbin

**bin:**
bin为binary的简写主要放置一些[系统](http://www.2cto.com/os/)的必备执行档例如:cat、cp、chmod df、dmesg、gzip、kill、ls、mkdir、more、mount、rm、su、tar等。
![](https://img-blog.csdn.net/20130808194725687)

**/usr/bin:**
主要放置一些应用软件工具的必备执行档例如c++、g++、gcc、chdrv、diff、dig、du、eject、elm、free、gnome*、 zip、htpasswd、kfm、ktop、last、less、locale、m4、make、man、mcopy、ncftp、 newaliases、nslookup passwd、quota、smb*、wget等。
![](https://img-blog.csdn.net/20130808194740437)

**/sbin:**
主要放置一些系统管理的必备程序例如:cfdisk、dhcpcd、dump、e2fsck、fdisk、halt、ifconfig、ifup、 ifdown、init、insmod、lilo、lsmod、mke2fs、modprobe、quotacheck、reboot、rmmod、 runlevel、shutdown等。
![](https://img-blog.csdn.net/20130808194754562)

**/usr/sbin:**
放置一些网路管理的必备程序例如:dhcpd、httpd、imap、in.*d、inetd、lpd、named、netconfig、nmbd、samba、sendmail、squid、swap、tcpd、tcpdump等
![](https://img-blog.csdn.net/20130808194809625)

/  : this is root directory
/bin : commands in this dir are all system installed user commands
/sbin:  commands in this dir are all system installed super user commands
/usr/bin: user commands for applications
/usr/sbin: super user commands for applications
/usr/local/bin : application user commands
/usr/local/sbin: application super user commands
![](https://img-blog.csdn.net/20130808194825562)

/bin:是系统的一些指令.
/sbin:一般是指超级用户指令.
/usr/bin:是你在后期安装的一些软件的运行脚本.

**综述：**
如果是用户和管理员必备的二进制文件，就会放在/bin；
如果是系统管理员必备，但是一般用户根本不会用到的二进制文件，就会放在 /sbin。
如果不是用户必备的二进制文件，多半会放在/usr/bin；
如果不是系统管理员必备的工具，如网络管理命令，多半会放在/usr/sbin。



**参考推荐：**
[/bin、/sbin、/usr/bin、/usr/sbin目录的区别](http://www.2cto.com/os/201211/170994.html)
[Linux 之 /etc/profile、~/.bash_profile 等几个文件的执行过程](http://blog.csdn.net/ithomer/article/details/6322892)



