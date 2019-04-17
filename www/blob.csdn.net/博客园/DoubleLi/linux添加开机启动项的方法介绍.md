# linux添加开机启动项的方法介绍 - DoubleLi - 博客园






使用chkconfig命令可以查看在不同启动级别下课自动启动的服务（或是程序），命令格式如下：
chkconfig 
--list
可能输出如下：
openvpn 0：关闭 1：开启 ...... 6：关闭 （0-6 为启动级别 ; 
关闭/开启为相应级别下该服务的自动启动选项）
如果希望对自动启动选项做出改变，命令格式为：
chkconfig --level x name 
on/off
z.B. chkconfig --level 5 openvpn 
off
以上的命令可以查询系统可提供的服务，如果希望开机时启动某一程序，可采用以下方法：
在~/.bash_profile文件最后一行添加所希望启动的软件名。如：“synergyc 
192.168.0.154” 
则在开机时自动运行synergyc并与192.168.0.154连接。
以上内容为我个人配置，但是发现了一个问题：FC12在登陆后才完成系统启动，也就是说在进入用户登陆界面的时候synergyc还未能启动。所以，（也许）synergyc不适合被安装在用作无键鼠客户机的linux系统中。
在Red 
Hat Linux中自动运行程序
1．开机启动时自动运行程序
Linux加载后, 它将初始化硬件和设备驱动, 
然后运行第一个进程init。init根据配置文件继续引导过程，启动其它进程。通常情况下，修改放置在 /etc/rc或 /etc/rc.d 或 
/etc/rc?.d 目录下的脚本文件，可以使init自动启动其它程序。例如：编辑 /etc/rc.d/rc.local 
文件，在文件最末加上一行"xinit"或"startx"，可以在开机启动后直接进入X－Window。
2．登录时自动运行程序
用户登录时，bash首先自动执行系统管理员建立的全局登录script 
：/etc/profile。然后bash在用户起始目录下按顺序查找三个特殊文件中的一个：/.bash_profile、/.bash_login、 
/.profile，但只执行最先找到的一个。
因此，只需根据实际需要在上述文件中加入命令就可以实现用户登录时自动运行某些程序（类似于DOS下的Autoexec.bat）。
3．退出登录时自动运行程序
退出登录时，bash自动执行个人的退出登录脚本/.bash_logout。例如，在/.bash_logout中加入命令"tar 
－cvzf c.source.tgz ＊.c"，则在每次退出登录时自动执行 "tar" 命令备份 ＊.c 
文件。
4．定期自动运行程序
Linux有一个称为crond的守护程序，主要功能是周期性地检查 
/var/spool/cron目录下的一组命令文件的内容，并在设定的时间执行这些文件中的命令。用户可以通过crontab 
命令来建立、修改、删除这些命令文件。
例如，建立文件crondFile，内容为"00 9 23 Jan ＊ 
HappyBirthday"，运行"crontab 
cronFile"命令后，每当元月23日上午9:00系统自动执行"HappyBirthday"的程序（"＊"表示不管当天是星期几）。
5．定时自动运行程序一次
定时执行命令at 
与crond 类似（但它只执行一次）：命令在给定的时间执行，但不自动重复。at命令的一般格式为：at [ －f file ] time 
，在指定的时间执行file文件中所给出的所有命令。也可直接从键盘输入命令：




复制代码


代码如下:


＄ at 12:00
at>mailto Roger －s ″Have a 
lunch″ < plan.txt
at>Ctr－D
Job 1 at 2000－11－09 12:00
2000－11－09 
12:00时候自动发一标题为"Have a 
lunch"，内容为plan.txt文件内容的邮件给Roger。
#!/bin/bash
RESTART="........" 
#里面写相应服务代码
START="......."
STOP=".........."
case "$1" 
in
restart)
$RESTART
echo "......"
;;
start)
$START
echo 
"......"
;;
STOP)
$STOP
echo "......"
;;
*)
echo "Usage: $0 
{restart ¦ start ¦ stop}"
exit 1
esac
exit 1


脚本写完要修改一下权限 
chmod u+x 
test.sh
首先，linux随机启动的服务程序都在/etc/init.d这个文件夹里，里面的文件全部都是脚本文件（脚本程序简单的说就是把要运行的程序写到一个文件里让系统能够按顺序执行，类似windows下的autorun.dat文件），另外在/etc这个文件夹里还有诸如名为rc1.d, 
rc2.d一直到rc6.d的文件夹，这些都是linux不同的runlevel，我们一般进入的X 
windows多用户的运行级别是第5级，也就是rc5.d，在这个文件夹下的脚本文件就是运行第5级时要随机启动的服务程序。需要注意的是，在每个rc 
(1-6).d文件夹下的文件其实都是/etc/init.d文件夹下的文件的一个软连接（类似windows中的快捷方式），也就是说，在 
/etc/init.d文件夹下是全部的服务程序，而每个rc(1-6).d只链接它自己启动需要的相应的服务程序！
要启动scim(某一程序)，我们首先要知道scim程序在哪里，用locate命令可以找到，scim在/usr/bin/scim这里，其中usr表示是属于用户的，bin在linux里表示可以执行的程序。这样，我就可以编写一个脚本程序，把它放到/etc/init.d里，然后在rc5.d里做一个相应的软链接就可以了。
这个脚本其实很简单，就两行：
#!/bin/bash
/usr/bin/scim
第一行是声明用什么终端运行这个脚本，第二行就是要运行的命令。
还需要注意的一点是，在rc5.d里，每个链接的名字都是以S或者K开头的，S开头的表示是系统启动是要随机启动的，K开头的是不随机启动的。这样，你就可以知道，如果我要哪个服务随机启动，就把它名字第一个字母K改成S就可以了，当然，把S改成K后，这个服务就不能随机启动了。因此，我这个链接还要起名为SXXX，这样系统才能让它随机启动
添加自启动脚本
首先把自己的脚本放到/etc/init.d中,，然后执行如下指令：
update-rc.d 
a start 90 2 3 4 5 . stop 90 0 1 6 .
其中a就是你的脚本，注意有两个点。
a脚本范例。




复制代码


代码如下:


#!/bin/sh
# Source function 
library.
if [ -f /etc/init.d/functions ]; then
. 
/etc/init.d/functions
else
. 
/lib/lsb/init-functions
fi
MOD=/a.ko
start()
{
echo -n $"insert a 
kernel module: "
/sbin/insmod $MOD
echo
}
stop()
{
echo -n 
$"remove a kernel module: "
/sbin/rmmod a -f
echo
}
[ -f $MOD ] || 
exit 0
# See how we were called.
case "$1" 
in
start)
start
;;
stop)
stop
;;
restart|reload)
stop
start
;;
*)
echo 
$"Usage: $0 
{start|stop|restart|reload}"


update-rc.d命令，是用来自动的升级System 
V类型初始化脚本，简单的讲就是，哪些东西是你想要系统在引导初始化的时候运行的，哪些是希望在关机或重启时停止的，可以用它来帮你设置。这些脚本的连接位于/etc/rcn.d/LnName,对应脚本位于/etc/init.d/Script-name.
1、设置指定启动顺序、指定运行级别的启动项：
update-rc.d 
＜service＞ start ＜order＞ ＜runlevels＞
2、设置在指定运行级中，按指定顺序停止：
update-rc.d 
＜service＞ stop ＜order＞ ＜runlevels＞
3、从所有的运行级别中删除指定的启动项：
update-rc.d -f 
＜script-name＞ remove
例如：
update-rc.d script-name start 90 1 2 3 4 5 . stop 
52 0 6 .
start 90 1 2 3 4 5 . : 
表示在1、2、3、4、5这五个运行级别中，按先后顺序，由小到大，第90个开始运行这个脚本。
stop 52 0 6 . 
：表示在0、6这两个运行级别中，按照先后顺序，由小到大，第52个停止这个脚本的运行。
如果在 /etc/init.d 中加入一个 
script，还须要制作相关的 link
在 /etc/rc*.d 中。K 开头是 kill , S 开头是 start , 
数字顺序代表启动的顺序。(SysV)
update-rc.d 可以帮你的忙。
例：
在 /etc/init.d 中建立一个叫作 zope 的 
script , 然后
update-rc.d zope defaults
就会产生以下链結::




复制代码


代码如下:


Adding system startup for /etc/init.d/zope 
...
/etc/rc0.d/K20zope -> ../init.d/zope
/etc/rc1.d/K20zope -> 
../init.d/zope
/etc/rc6.d/K20zope -> ../init.d/zope
/etc/rc2.d/S20zope 
-> ../init.d/zope
/etc/rc3.d/S20zope -> 
../init.d/zope
/etc/rc4.d/S20zope -> ../init.d/zope
/etc/rc5.d/S20zope 
-> ../init.d/zope
其他进阶使用方式请 man update-rc.d









