# linux log日志解析 - DoubleLi - 博客园






linux log日志解析



其实，可以说成是监控[系统](http://www.2cto.com/os/)的记录，系统一举一动基本会记录下来。这样由于信息非常全面很重要，通常只有 root 可以进行视察！通过登录文件（日志文件）可以根据屏幕上面的错误讯息与再配合登录文件的错误信息，几乎就可以解决大部分的 [Linux](http://www.2cto.com/os/linux/) 问题！ 所以日志文件异常重要，作为一个合格的linux 系统工程师，日志文件是必要熟练掌握的部分。



常见的几个登录文件有：

/var/log/secure：记录登入系统存取数据的文件，例如 pop3, ssh, telnet, ftp 等都会被记录；

/var/log/wtmp：记录登入者的讯息数据，由于本文件已经被编码过，所以必须使用 last指令来取出文件的内容；

/var/log/messages：尤为重要，几乎发生的错误讯息（或是重要信息）都会被记录在此；

/var/log/boot.log：记录开机或者是一些服务启动的时候，所显示的启动或关闭讯息；

/var/log/maillog 或 /var/log/mail/*：纪录邮件存取或往来( sendmail 与 pop3 )的使用者记录；

/var/log/cron：记录 crontab 这个例行性服务的内容的。

/var/log/httpd, /var/log/news, /var/log/[mysql](http://www.2cto.com/database/MySQL/)d.log, /var/log/samba, /var/log/procmail.log：分别是几个不同的网络服务的记录文件！





登录文件的纪录程序之一： syslogd

通常经过 syslog 而记录下来的数据主要有：

事件发生的日期与时间；

发生此事件的主机名称；

启动此事件的服务名称 (如 samba, xinetd 等) 或函式名称 (如 libpam ..)；

该讯息数据内容



syslogd的daemon配置文件：/etc/syslog.conf

内容语法是这样的：

服务名称[.=!]讯息等级          讯息记录的文件名或装置或主机

# 例如底下：

mail.info                      /var/log/maillog_info



服务名称：该服务产生的讯息会被纪录的意思。syslog 认识的服务主要有底下这些：

auth, authpriv：主要与认证有关的机制，例如telnet, login, ssh 等需要认证的服务都是使用此一机制；

cron：例行性命令 cron/at 等产生讯息记录的地方；

daemon：与各个 daemon 有关的讯息；

kern：核心 (kernel) 产生讯息的地方；

lpr：打印相关的讯息！

mail：只要与邮件收发有关的讯息纪录都属于这个；

news：与新闻群组服务器有关的东西；

syslog：syslogd 这支程序本身产生的信息啊！

user, uucp, local0 ~ local7：与 Unix like 机器本身有关的一些讯息。



讯息等级

系统将讯息分为七个主要的等级，依序是由不重要排列到重要讯息等级：

info：仅是一些基本的讯息说明而已；

notice：比 info 还需要被注意到的一些信息内容；

warning 或 warn：警示讯息，可能有问题，但是还不至于影响到某个 daemon 运作。

err 或 error ：一些重大的错误讯息，这就要去找原因了。

crit：比 error 还要严重的错误信息，crit 是临界点 (critical) 的缩写，已经很严重了！

alert：警告警告，已经很有问题的等级，比 crit 还要严重！

emerg 或 panic：疼痛等级，意指系统已经几乎要当机的状态！ 很严重的错误信息了。

  除了这些有等级的讯息外，还有两个特殊的等级，那就是 debug(错误侦测等级) 与 none (不需登录等级) 两个，当要作一些错误侦测，或者是忽略掉某些服务的信息时，就用这俩！



在讯息等级之前还有 [.=!] 的连结符号！他代表的意思是：

. ：代表比后面还要高的等级(含该等级)都被记录下来的意思， 例如：mail.info 代表只要是 mail 的信息，而且该信息等级高于 info (含info )时，就会被记录下来。

.=：代表所需要的等级就是后面接的等级而已！

.!：代表不等于。



日志文件记录的文件名或装置或主机常见的放置处：

文件的绝对路径：通常就是放在 /var/log 里头的文件！

打印机或其它：例如 /dev/lp0 这个打印机装置  （即使被[黑客](http://www.2cto.com/)可以删除掉日志文件，但是最终删除不了打印出来的日志信息）

使用者名称：显示给使用者！

远程主机：例如 @test.adsldns.org，要对方主机也能支持才行！

*：代表目前在线的所有人，类似 wall 这个指令的意义！



看看在尚未开启网络服务的情况下来自 Fedora Core Release 4 的相关资料

[root@linux ~]# vi /etc/syslog.conf

#kern.*                                    /dev/console

# 只要是 kernel 产生的讯息，全部都送到 console 去！默认是关闭的。

*.info;mail.none;authpriv.none;cron.none   /var/log/messages

# 在已知各服务的讯息中，不要记录到这个文件中，把已知的服务记录到单独的日志文件中去，方便日后查询，否则messages这个文件就太混乱了。这个文件非常重要，所有未知的信息都会被记录在这个文件中，所以有问题，找这个文件就八九不离十了。

authpriv.*                                 /var/log/secure

# 这个就是经过一些身份确认的行为之后，需要记录身份的文件。

mail.*                                     -/var/log/maillog

# 只要跟 mail 有关的（不论是 pop3 还是 sendmail ）都会被纪录到这个文件！

cron.*                                     /var/log/cron

#例行性命令相关的。

*.emerg                                    *

# 任何时候发生的警告讯息都会显示给在线的所有人！那个 ＊ 就是目前在线的所有人。

uucp,news.crit                             /var/log/spooler

# 记录新闻错误高于 crit 的等级的信息，写入 spooler 当中！

local7.*                                   /var/log/boot.log

# 将开机的当中的讯息写入 /var/log/boot.log 中！

  每个版本的 syslog.conf 差异是很大的，所以，每个登录文件记录的数据其实不很固定。



例：让所有的信息都额外写入到 /var/log/admin.log！

[root@linux ~]# vi /etc/syslog.conf

*.info          /var/log/admin.log  》》如果服务器硬盘容量够大，这么做也不失为一个良策。

[root@linux ~]# /etc/init.d/syslog restart

[root@linux ~]# ll /var/log/admin.log

-rw-------  1 root root 122 Oct 23 22:21 /var/log/admin.log  》》注意权限





分析登录文件 

dmesg

[root@linux ~]# dmesg | more

  由于系统在开机的过程当中尚未将硬盘 mount 上来，所以无法直接将数据直接读到 log file 当中去，但是为了除错上面的方便， 所以在开机的过程当中的讯息还是要记录下来，这个时候系统就将 ram 开了一个小区块来储存这个数据！这个开机记录文件就是：/proc/kmsg！

  几乎所有的核心信息都可以使用 dmesg 来查阅得到的，例 想要知道开机有没有捉到网卡，dmesg | grep 'eth' ！



last

[root@linux ~]# last -n number

[root@linux ~]# last -f filename

参数：

-n  ：last 会读出这个月的数据，若数据量太大，可使用 -n 来严格要求显示的笔数。例如 20 笔数据： last -n 20 或 last -20 均可。

-f  ：last预设读出 /var/log/wtmp，但是我们可以透过 -f 读取不同的登录文件信息！



例,将上个月的资料读出，仅读出5笔资料

[root@linux ~]# last -n 5 -f /var/log/wtmp.1

dmtsai2  pts/2             Mon Oct 24 14:18 - 14:18  (00:00)

dmtsai2  work:0 work       Mon Oct 24 14:18    gone - no logout

dmtsai2  work:0 work       Mon Oct 24 14:18 - 14:18  (00:00)

dmtsai2  pts/2             Mon Oct 24 14:18 - 14:18  (00:00)

dmtsai2  work:0 work       Mon Oct 24 14:18 - 14:18  (00:00)



[root@linux ~]# lastlog

lastlog 只是读出 /var/log/lastlog 内的信息. 他会显示目前系统上面的所有账号当中，每个账号最近一次登入的时间！





Linux系统的LOG日志文件介绍！

UNIX网管员主要是靠系统的LOG,来获得入侵的痕迹.当然也有第三方工具记录入侵系统的 痕迹,UNIX系统存放LOG文件,普通位置如下:

/usr/adm - 早期版本的UNIX

/var/adm - 新一点的版本使用这个位置

/var/log - 一些版本的Solaris,linux BSD,Free BSD使用这个位置

/etc - 多数UNIX版本把utmp放在这里，有些也把wtmp放在这里，syslog.conf在这里



下面的一些文件根据你所在的目录不同而不同：

acct 或 pacct -- 记录每个用户使用的命令记录

access_log -- 主要当服务器运行NCSA HTTPD时, 记录什么站点连接过你的服务器

aculog -- 保存着你拨出去的MODEMS记录

lastlog -- 记录了用户最近的LOGIN记录和每个用户的最初目的地，有时是最后不

成功LOGIN的记录,当一个用户登陆到unix系统，注册程序在lastlog文件中查找该用户的uid，如果该程序找到了

该用户的uid，unix就会显示最后一次登陆的时间和tty（终端号）

loginlog -- 记录一些不正常的LOGIN记录

messages -- 记录输出到系统控制台的记录，另外的信息由syslog来生成

security -- 记录一些使用UUCP系统企图进入限制范围的事例

sulog -- 记录使用su命令的记录．它通常在/var/adm/sulog．如果你在机器上使用了su命令，别忘了清除哦．

utmp -- 记录当前登录到系统中的所有用户，这个文件伴随着用户进入和离开系统而不断变化.它还会为系统中的用户保持很长的历史记录，utmp日志通常存放在/var/adm/utmp目录下．可以用w和who命令查看，其他命令也可以访问这个文件．如：finger　root就可以．现在的utmp一般都有utmpx文件作为日志记录的补充．

utmpx -- UTMP的扩展

wtmp -- 记录用户登录和退出事件．它和utmp日志文件相似，但它随着登陆次数的增加，它会变的越来越大，有些系统的ftp访问也在这个文件里记录，同时它也记录正常的系统退出时间,可以用ac和last命令访问．

syslog -- 最重要的日志文件，使用syslogd守护程序来获得日志信息，通常情况下通过查看/etc/syslog.conf．我们可以知道syslog记录些什么．缺省时，它把大多的消息传给/var/adm/message．

/dev/log -- 一个UNIX域套接字，接受在本地机器上运行的进程所产生的消息

/dev/klog -- 一个从UNIX内核接受消息的设备

514端口 -- 一个INTERNET套接字，接受其他机器通过UDP产生的syslog消息。

uucp -- 记录的UUCP的信息，可以被本地UUCP活动更新， 也可有远程站点发起

的动作修改，信息包括发出和接受的呼叫，发出的请求，发送者， 发

送时间和发送主机

lpd-errs -- 处理打印机故障信息的日志

ftp日志 -- 执行带-l选项的ftpd能够获得记录功能

httpd日志 -- HTTPD服务器在日志中记录每一个WEB访问记录

history日志 -- 这个文件保存了用户最近输入命令的记录

vold.log -- 记录使用外接媒介时遇到的错误记录

======================

其他类型的日志文件-

======================

有些类型的LOG文件没有特定的标题，但开始于一个特定的标志， 你可以在前面头发现如

下的标志，这就一般表示此是个LOG日志文件，你就可以编辑它了：

xfer -- 表明试图一个禁止的文件传输.

rexe -- 表明试图执行一个不允许的命令

还有许多其他其他类型的LOG文件存在，主要是第三方软件引起的， 或者甚至他妈的网管

自己有设置了一只"眼睛"在他的系统上，所以你要对你认为可能是LOG文件的文件多一份心眼。

许多管理员喜欢把日志文件放在同一个目录中以便管理， 所以你要检查你发现的LOG文件所在

的目录中，是否有其他日志文件放在这里，如果有,咯，你知道怎么做。

另一个你要注意的是有关LOG用户MAIL的文件，此文件名可以多种多样，或则有时是

syslog文件的一部分。你要知道syslog记录那些信息， 你可以查看syslog.conf中的信息此文

件的目录是在/etc中

一般我们都是查看syslog.conf文件来查看日志的配置情况.例如:cat /etc/syslog.conf

其中sunos操作系统的在/var/log和/var/adm下，还有/usr/adm为/var/adm的的链接．

redhat的在/var/log 和 /var/run下

下面的是sun os5.7中的日志样本． 此外，各种shell还会记录用户使用的命令历史，它使用用户主目录下的文件来记录这些命令历史，通常这个文件的名字为.sh_history(ksh)，.history(csh)，或.bash_history(bash)等。

# ls /var/adm

acct 　　　log 　　　　 messages.1 　　　passwd 　　　sulog　　 vold.log

aculog　　 messages 　　messages.2　　　 sa 　　　　　utmp 　　wtmp

lastlog 　 messages.0 　messages.3 　　　spellhist 　 utmpx　　 wtmpx

# ls /var/log

authlog　　　　　 syslog 　　　syslog.1 　　syslog.3

sysidconfig.log 　syslog.0　　 syslog.2 　　syslog.4



下面的是redhat9.0中的日志样本．

# ls /var/log

boot.log 　　dmesg 　　　　　　messages.2 　　　secure 　　　　uucp

boot.log.1 　htmlaccess.log　　messages.3 　　　secure.1 　　　wtmp

boot.log.2 　httpd 　　　　　　messages.4　　 　secure.2 　　　wtmp.1

boot.log.3 　lastlog 　　　　　netconf.log　　　secure.3 　　　xferlog

boot.log.4 　mailllog 　　　　 netconf.log.1　　secure.4 　　　xferlog.1

cron 　　　　maillog 　　　　　netconf.log.2　　sendmail.st 　 xferlog.2

cron.1　　　 maillog.1 　　　　netconf.log.3　　spooler 　　　 xferlog.3

cron.2　　　 maillog.2 　　　　netconf.log.4　　spooler.1 　　 xferlog.4

cron.3　　　 maillog.3 　　　　news 　　　　　　spooler.2

cron.4　　　 maillog.4 　　　　normal.log 　　　spooler.3

daily.log 　 messages　　　　　realtime.log　　spooler.4

daily.sh 　　messages.1 　　　 samba 　　　　　transfer.log



# ls /var/run

atd.pid　　 　gpm.pid 　　　klogd.pid 　　random-seed 　　 treemenu.cache

crond.pid 　　identd.pid 　 netreport 　　runlevel.dir 　　utmp

ftp.pids-all　inetd.pid 　　news 　　　　 syslogd.pid

一般我们要清除的日志有

lastlog

utmp(utmpx)

wtmp(wtmpx)

messages

syslog

sulog

一般把以上说的日志给擦一下，就可以了．

下面我来说说上面这些我们要清除的日志的相关资料和清除方法．更详细的资料和其他的日志请你查看相关资料．

上面已经对日志的功能做了简单陈述，那么这些日志文件到底记录的是什么呢? follow　me

下面是一个例子：

SunOS 5.7

login: gao

Password:

No directory! Logging in with home=/

Last login: Sun Feb 4 22:18:25 from 219.31.36.7

Sun Microsystems Inc. SunOS 5.7 Generic October 1998 $ 

然后注册程序用新的登陆时间和TTY信息更新lastlog文件，而且该程序带更新utmp wtmp.文件．



shell记录：



.sh_history(ksh)，.history(csh)，或.bash_history(bash)等，是shell执行时的历史记录．记录用户执行的命令．它一般存在于用户的主目录．别忘了去根目录看看.



1.日志都是一些文本形式的文件．最笨的方法是用文本编辑器来编辑日志文件．删除相关的记录．来达到擦拭脚印和隐藏自己的效果.

比如用vi等,但这样做是很笨的．太麻烦，工作量太大．



2.用rm -f 来删掉日志．比如rm -f /usr/adm/lastlog

这样做是很蠢的．更容易被管理员发现有人入侵．但是，相对来说自己还是保护好了. 可以用在一些不太重要的机器上．

3．用>定向符清除．

比如：

cat > /usr/log/lastlog

－>这里输入你要的写的东西．最好伪装得像一些，也可以不输入哦．

^d　－>这里的^d是按键 ctrl + d.

# ．









