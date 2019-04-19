# linux下提示bash:command not found_带目录下命令列表介绍 - xqhrs232的专栏 - CSDN博客
2016年04月15日 09:46:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：661
原文地址::[http://www.jb51.net/LINUXjishu/32192.html](http://www.jb51.net/LINUXjishu/32192.html)
如果新装的系统，运行一些很正常的诸如：shutdown，fdisk的命令时，悍然提示：bash:command not found。那么 
首先就要考虑root 的$PATH里是否已经包含了这些环境变量。 
主要是这四个：/bin ,/usr/bin,/sbin,/usr/sbin。 
四个主要存放的东东： 
./bin:
bin为binary的简写主要放置一些系统的必备执行档例如:cat、cp、chmod df、dmesg、gzip、kill、ls、mkdir、more、mount、rm、su、tar等。 
/usr/bin:
主要放置一些应用软体工具的必备执行档例如c++、g++、gcc、chdrv、diff、dig、du、eject、elm、free、gnome*、 gzip、htpasswd、kfm、ktop、last、less、locale、m4、make、man、mcopy、ncftp、 newaliases、nslookup passwd、quota、smb*、wget等。 
/sbin:
主要放置一些系统管理的必备程式例如:cfdisk、dhcpcd、dump、e2fsck、fdisk、halt、ifconfig、ifup、 ifdown、init、insmod、lilo、lsmod、mke2fs、modprobe、quotacheck、reboot、rmmod、 runlevel、shutdown等。 
/usr/sbin:
放置一些网路管理的必备程式例如:dhcpd、httpd、imap、in.*d、inetd、lpd、named、netconfig、nmbd、samba、sendmail、squid、swap、tcpd、tcpdump等。 
可以在家目录下查看.bash_profile里的PATH。如果是：PATH=$PATH:$HOME/bin则需要添加成如下： 
PATH=$PATH:$HOME/bin:/sbin:/usr/bin:/usr/sbin
今天在虚拟机上安装了CentOS5.5，发现运行一些很正常的诸如：init，shutdown，fdisk 等命令时，悍然提示： 
bash: xx command not found。 
那么，首先就要考虑root 的$PATH里是否已经包含了这些环境变量（可能是我安装的时候选择了“最小安装”，导致一些功能没开启）。 
主要是这四个：/bin , /usr/bin, /sbin, /usr/sbin。
