# 转载和积累系列 -  Linux服务器安全教程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月14日 13:26:15[initphp](https://me.csdn.net/initphp)阅读数：644







**　一、系统安全记录文件**
　　操作系统内部的记录文件是检测是否有网络入侵的重要线索。如果您的系统是直接连到Internet，您发现有很多人对您的系统做Telnet/FTP登录尝试，可以运行“#more /var/log/secure grep refused”来检查系统所受到的攻击，以便采取相应的对策，如使用SSH来替换Telnet/rlogin等。

**　　二、启动和登录安全性**

　　1.BIOS安全IXPUB

　　设置BIOS密码且修改引导次序禁止从软盘启动系统。

　　2.用户口令

　　用户口令是Linux安全的一个基本起点，很多人使用的用户口令过于简单，这等于给侵入者敞开了大门，虽然从理论上说，只要有足够的时间和资源可以利用，就没有不能破解的用户口令，但选取得当的口令是难于破解的。较好的用户口令是那些只有他自己容易记得并理解的一串字符，并且绝对不要在任何地方写出来。

**　　3.默认账号**

　　应该禁止所有默认的被操作系统本身启动的并且不必要的账号，当您第一次安装系统时就应该这么做，Linux提供了很多默认账号，而账号越多，系统就越容易受到攻击。

　　可以用下面的命令删除账号。

　　[root@server /]# userdel 用户名

　　或者用以下的命令删除组用户账号。

　　[root@server /]# groupdel username

**　　4.口令文件**

　　chattr命令给下面的文件加上不可更改属性，从而防止非授权用户获得权限。

　　[root@server /]# chattr +i /etc/passwd

　　[root@server /]# chattr +i /etc/shadow

　　[root@server /]# chattr +i /etc/group

　　[root@server /]# chattr +i /etc/gshadow

**　5.禁止Ctrl+Alt+Delete重新启动机器命令**

　　修改/etc/inittab文件，将“ca：：ctrlaltdel:/sbin/shutdown -t3 -r now”一行注释掉。然后重新设置/etc/rc.d/init.d/目录下所有文件的许可权限，运行如下命令：

　　[root@server /]# chmod -R 700 /etc/rc.d/init.d/*

　　这样便仅有root可以读、写或执行上述所有脚本文件。

**　　6.限制su命令**

　　如果您不想任何人能够su作为root，可以编辑/etc/pam.d/su文件，增加如下两行：

　　auth sufficient /lib/security/pam_rootok.so debug

　　auth required /lib/security/pam_wheel.so group=isd

　　这时，仅isd组的用户可以su作为root。此后，如果您希望用户admin能够su作为root，可以运行如下命令：

　　[root@server /]# usermod -G10 admin

**　　7.删减登录信息**

　　默认情况下，登录提示信息包括Linux发行版、内核版本名和服务器主机名等。对于一台安全性要求较高的机器来说这样泄漏了过多的信息。可以编辑/etc/rc.d/rc.local将输出系统信息的如下行注释掉。

　　# This will overwrite /etc/issue at every boot. So， make any changes you

　　# want to make to /etc/issue here or you will lose them when you reboot.

　　# echo “” 》 /etc/issue

　　# echo “$R” 》》 /etc/issue

　　# echo “Kernel $(uname -r) on $a $(uname -m)” 》》 /etc/issue

　　# cp -f /etc/issue /etc/issue.net

　　# echo 》》 /etc/issue

　　然后，进行如下操作：

　　[root@server /]# rm -f /etc/issue

　　[root@server /]# rm -f /etc/issue.net

　　[root@server /]# touch /etc/issue

　　[root@server /]# touch /etc/issue.net

**　　8.设置Grub密码**

　　[root@server share]# grub-md5-crypt

　　Password： //输入密码

　　Retype password： //输入确认密码

　　$1$LlZDJ/$VxlsjlcR9vvWcI/YRHPpm0

　　vim /boot/grub/grub.conf

　　添加一行：password $1$LlZDJ/$VxlsjlcR9vvWcI/YRHPpm0

**　　三、限制网络访问**

　　1.NFS访问如果您使用NFS网络文件系统服务，应该确保您的/etc/exports具有最严格的访问权限设置，也就是意味着不要使用任何通配符、不允许root写权限并且只能安装为只读文件系统。编辑文件/etc/exports并加入如下两行。

　　/dir/to/export host1.mydomain.com(ro，root_squash)

　　/dir/to/export host2.mydomain.com(ro，root_squash)

　　/dir/to/export 是您想输出的目录，host.mydomain.com是登录这个目录的机器名，ro意味着mount成只读系统，root_squash禁止root写入该目录。为了使改动生效，运行如下命令。

　　[root@server /]# /usr/sbin/exportfs -a

　　2.Inetd设置

　　首先要确认/etc/inetd.conf的所有者是root，且文件权限设置为600。设置完成后，可以使用“stat”命令进行检查。

　　[root@server /]# chmod 600 /etc/inetd.conf

　　然后，编辑/etc/inetd.conf禁止以下服务。

　　ftp telnet shell login exec talk ntalk imap pop-2 pop-3 finger auth

　　如果您安装了ssh/scp，也可以禁止掉Telnet/FTP。为了使改变生效，运行如下命令：

　　[root@server /]# killall -HUP inetd

　　默认情况下，多数Linux系统允许所有的请求，而用TCP_WRAPPERS增强系统安全性是举手之劳，您可以修改/etc/hosts.deny和 /etc/hosts.allow来增加访问限制。例如，将/etc/hosts.deny设为“ALL： ALL”可以默认拒绝所有访问。然后在/etc/hosts.allow文件中添加允许的访问。例如，“sshd： 192.168.1.10/255.255.255.0 gate.openarch.com”表示允许IP地址192.168.1.10和主机名gate.openarch.com允许通过SSH连接。

　　配置完成后，可以用tcpdchk检查：

　　[root@server /]# tcpdchk

　　tcpchk是TCP_Wrapper配置检查工具，它检查您的tcp wrapper配置并报告所有发现的潜在/存在的问题。

　　3.登录终端设置

　　/etc/securetty文件指定了允许root登录的tty设备，由/bin/login程序读取，其格式是一个被允许的名字列表，您可以编辑/etc/securetty且注释掉如下的行。

　　# tty1

　　# tty2

　　# tty3

　　# tty4

　　# tty5

　　# tty6

　　这时，root仅可在tty1终端登录。

　　4.避免显示系统和版本信息。

　　如果您希望远程登录用户看不到系统和版本信息，可以通过一下操作改变/etc/inetd.conf文件：

　　telnet stream tcp nowait root /usr/sbin/tcpd in.telnetd -

　　加-h表示telnet不显示系统信息，而仅仅显示“login：”。

　　5.不允许root用户登录服务器

　　[root@server /]# vim /etc/ssh/sshd_config

　　修改：PermitRootLogin no

　　四、防止攻击

　　1.阻止ping 如果没人能ping通您的系统，安全性自然增加了。

　　[root@server /]# vim /etc/sysctl.conf

　　添加：net.ipv4.icmp_echo_ignore_all = 1

　　[root@server /]# sysctl -p

　　2.防止IP欺骗

　　编辑host.conf文件并增加如下几行来防止IP欺骗攻击。

　　order bind，hosts

　　multi off

　　nospoof on

　　3.防止DoS攻击

　　对系统所有的用户设置资源限制可以防止DoS类型攻击。如最大进程数和内存使用数量等。例如，可以在

　　[root@server /]# vim /etc/security/limits.conf中添加如下几行：

　　* hard core 0

　　* hard rss 5000

　　* hard nproc 20

　　然后必须编辑/etc/pam.d/login文件检查下面一行是否存在。

　　session required /lib/security/pam_limits.so

　　上面的命令禁止调试文件，限制进程数为50并且限制内存使用为5MB。




