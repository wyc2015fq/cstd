# linux日志文件 - nosmatch的专栏 - CSDN博客
2011年07月19日 15:03:59[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：575标签：[linux																[login																[unix																[网络																[cron																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=cron&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=login&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
                 1. 日志简介 
    日志对于[安全](http://security.chinaitlab.com/)来说，非常重要，他记录了系统每天发生的各种各样的事情，你可以通过他来检查错误发生的原因，或者受到攻击时攻击者留下的痕迹。日志主要的功能有：审计和监测。他还可以实时的监测系统状态，监测和追踪侵入者等等。 
在Linux系统中，有三个主要的日志子系统： 
连接时间日志--由多个程序执行，把纪录写入到/var/log/wtmp和/var/run/utmp，login等程序更新wtmp和utmp文件，使系统管理员能够跟踪谁在何时登录到系统。 
进程统计--由系统内核执行。当一个进程终止时，为每个进程往进程统计文件（pacct或acct）中写一个纪录。进程统计的目的是为系统中的基本服务提供命令使用统计。 
错误日志--由syslogd（8）执行。各种系统守护进程、用户程序和内核通过syslog（3）向文件/var/log/messages报告值得注意的事件。另外有许多UNIX程序创建日志。像HTTP和FTP这样提供网络服务的[服务器](http://server.chinaitlab.com/)也保持详细的日志。 
常用的日志文件如下： 
access-log 纪录HTTP/web的传输 
acct/pacct 纪录用户命令 
aculog 纪录MODEM的活动 
btmp 纪录失败的纪录 
lastlog 纪录最近几次成功登录的事件和最后一次不成功的登录 
messages 从syslog中记录信息（有的链接到syslog文件） 
sudolog 纪录使用sudo发出的命令 
sulog 纪录使用su命令的使用 
syslog 从syslog中记录信息（通常链接到messages文件） 
utmp 纪录当前登录的每个用户 
wtmp 一个用户每次登录进入和退出时间的永久纪录 
xferlog 纪录FTP会话 
utmp、wtmp和lastlog日志文件是多数重用UNIX日志子系统的关键--保持用户登录进入和 退出的纪录。有关当前登录用户的信息记录在文件utmp中；登录进入和退出纪录在文件 wtmp中；最后一次登录文件可以用lastlog命令察看。数据[交换](http://cisco.chinaitlab.com/List_7.html)、关机和重起也记录在w 
tmp文件中。所有的纪录都包含时间戳。这些文件（lastlog通常不大）在具有大量用户 的系统中增长十分迅速。例如wtmp文件可以无限增长，除非定期截取。许多系统以一天 或者一周为单位把wtmp配置成循环使用。它通常由cron运行的脚本来修改。这些脚本重 
新命名并循环使用wtmp文件。通常，wtmp在第一天结束后命名为wtmp.1；第二天后wtmp .1变为wtmp.2等等，直到wtmp.7。 
每次有一个用户登录时，login程序在文件lastlog中察看用户的UID。如果找到了，则把 用户上次登录、退出时间和主机名写到标准输出中，然后login程序在lastlog中纪录新 的登录时间。在新的lastlog纪录写入后，utmp文件打开并插入用户的utmp纪录。该纪录 
一直用到用户登录退出时删除。utmp文件被各种命令文件使用，包括who、w、users和finger。 
下一步，login程序打开文件wtmp附加用户的utmp纪录。当用户登录退出时，具有更新时 间戳的同一utmp纪录附加到文件中。wtmp文件被程序last和ac使用。 
2. 具体命令 
    wtmp和utmp文件都是二进制文件，他们不能被诸如tail命令剪贴或合并（使用cat命令）。用户需要使用who、w、users、last和ac来使用这两个文件包含的信息。 who：who命令查询utmp文件并报告当前登录的每个用户。Who的缺省输出包括用户名、终端类型、登录日期及远程主机。例如：who（回车）显示 
chyang pts/0 Aug 18 15:06 
ynguo pts/2 Aug 18 15:32 
ynguo pts/3 Aug 18 13:55 
lewis pts/4 Aug 18 13:35 
ynguo pts/7 Aug 18 14:12 
ylou pts/8 Aug 18 14:15 
如果指明了wtmp文件名，则who命令查询所有以前的纪录。命令who /var/log/wtmp将报告自从wtmp文件创建或删改以来的每一次登录。 
w：w命令查询utmp文件并显示当前系统中每个用户和它所运行的进程信息。 
例如：w（回车）显示：3:36pm up 1 day, 22:34, 6 users, load average: 0.23, 0.29, 0.27 
USER TTY FROM LOGIN@ IDLE JCPU PCPU WHAT 
chyang pts/0 202.38.68.242 3:06pm 2:04 0.08s 0.04s -bash 
ynguo pts/2 202.38.79.47 3:32pm 0.00s 0.14s 0.05s w 
lewis pts/3 202.38.64.233 1:55pm 30:39 0.27s 0.22s -bash 
lewis pts/4 202.38.64.233 1:35pm 6.00s 4.03s 0.01s sh /home/users/ 
ynguo pts/7 simba.nic.ustc.e 2:12pm 0.00s 0.47s 0.24s telnet mail 
ylou pts/8 202.38.64.235 2:15pm 1:09m 0.10s 0.04s -bash 
users：users用单独的一行打印出当前登录的用户，每个显示的用户名对应一个登录会话。如果一个用户有不止一个登录会话，那他的用户名将显示相同的次数。
例如：users（回车）显示：chyang lewis lewis ylou ynguo ynguo 
last：last命令往回搜索wtmp来显示自从文件第一次创建以来登录过的用户。例如： 
chyang pts/9 202.38.68.242 Tue Aug 1 08:34 - 11:23 (02:49) 
cfan pts/6 202.38.64.224 Tue Aug 1 08:33 - 08:48 (00:14) 
chyang pts/4 202.38.68.242 Tue Aug 1 08:32 - 12:13 (03:40) 
lewis pts/3 202.38.64.233 Tue Aug 1 08:06 - 11:09 (03:03) 
lewis pts/2 202.38.64.233 Tue Aug 1 07:56 - 11:09 (03:12) 
如果指明了用户，那么last只报告该用户的近期活动，例如：last ynguo（回车）显示 
： 
ynguo pts/4 simba.nic.ustc.e Fri Aug 4 16:50 - 08:20 (15:30) 
ynguo pts/4 simba.nic.ustc.e Thu Aug 3 23:55 - 04:40 (04:44) 
ynguo pts/11 simba.nic.ustc.e Thu Aug 3 20:45 - 22:02 (01:16) 
ynguo pts/0 simba.nic.ustc.e Thu Aug 3 03:17 - 05:42 (02:25) 
ynguo pts/0 simba.nic.ustc.e Wed Aug 2 01:04 - 03:16 1+02:12) 
ynguo pts/0 simba.nic.ustc.e Wed Aug 2 00:43 - 00:54 (00:11) 
ynguo pts/9 simba.nic.ustc.e Thu Aug 1 20:30 - 21:26 (00:55) 
ac：ac命令根据当前的/var/log/wtmp文件中的登录进入和退出来报告用户连结的时间（小时），如果不使用标志，则报告总的时间。例如：ac（回车）显示：total 5177.47 
ac -d（回车）显示每天的总的连结时间 
Aug 12 total 261.87 
Aug 13 total 351.39 
Aug 14 total 396.09 
Aug 15 total 462.63 
Aug 16 total 270.45 
Aug 17 total 104.29 
Today total 179.02 
ac -p （回车）显示每个用户的总的连接时间 
ynguo 193.23 
yucao 3.35 
rong 133.40 
hdai 10.52 
zjzhu 52.87 
zqzhou 13.14 
liangliu 24.34 
total 5178.24 
lastlog：lastlog文件在每次有用户登录时被查询。可以使用lastlog命令来检查某特定用户上次登录的时间，并格式化输出上次登录日志 /var/log/lastlog的内容。它根据UID排序显示登录名、端口号（tty）和上次登录时间。如果一个用户从未登录过，lastlo 
g显示"**Never logged**。注意需要以root运行该命令，例如： 
rong 5 202.38.64.187 Fri Aug 18 15:57:01 +0800 2000 
dbb **Never logged in** 
xinchen **Never logged in** 
pb9511 **Never logged in** 
xchen 0 202.38.64.190 Sun Aug 13 10:01:22 +0800 2000 
另外，可一加一些参数，例如，last -u 102将报告UID为102的用户；last -t 7表示限 
制上一周的报告。 
3. 进程统计 
    UNIX可以跟踪每个用户运行的每条命令，如果想知道昨晚弄乱了哪些重要的文件，进程统计子系统可以告诉你。它对还跟踪一个侵入者有帮助。与连接时间日志不同，进程统计子系统缺省不激活，它必须启动。在Linux系统中启动进程统计使用accton命令，必须用root身份来运行。Accton命令的形式 accton file，file必须先存在。先使用touch命令来创建pacct文件：touch /var/log/pacct，然后运行accton： accton /var/log/pacct。一旦accton被激活，就可以使用lastcomm命令监测系统中任何时候执行的命令。若要关闭统计，可以使用不带任何参数的accton命令。
lastcomm命令报告以前执行的文件。不带参数时，lastcomm命令显示当前统计文件生命周期内纪录的所有命令的有关信息。包括命令名、用户、 tty、命令花费的CPU时间和一个时间戳。如果系统有许多用户，输入则可能很长。下面的例子： 
crond F root ?? 0.00 secs Sun Aug 20 00:16 
promisc_check.s S root ?? 0.04 secs Sun Aug 20 00:16 
promisc_check root ?? 0.01 secs Sun Aug 20 00:16 
grep root ?? 0.02 secs Sun Aug 20 00:16 
tail root ?? 0.01 secs Sun Aug 20 00:16 
sh root ?? 0.01 secs Sun Aug 20 00:15 
ping S root ?? 0.01 secs Sun Aug 20 00:15 
ping6.pl F root ?? 0.01 secs Sun Aug 20 00:15 
sh root ?? 0.01 secs Sun Aug 20 00:15 
ping S root ?? 0.02 secs Sun Aug 20 00:15 
ping6.pl F root ?? 0.02 secs Sun Aug 20 00:15 
sh root ?? 0.02 secs Sun Aug 20 00:15 
ping S root ?? 0.00 secs Sun Aug 20 00:15 
ping6.pl F root ?? 0.01 secs Sun Aug 20 00:15 
sh root ?? 0.01 secs Sun Aug 20 00:15 
ping S root ?? 0.01 secs Sun Aug 20 00:15 
sh root ?? 0.02 secs Sun Aug 20 00:15 
ping S root ?? 1.34 secs Sun Aug 20 00:15 
locate root ttyp0 1.34 secs Sun Aug 20 00:15 
accton S root ttyp0 0.00 secs Sun Aug 20 00:15 
进程统计的一个问题是pacct文件可能增长的十分迅速。这时需要交互式的或经过cron机制运行sa命令来保持日志数据在系统控制内。sa命令报告、清理并维护进程统计文件。它能把/var/log/pacct中的信息压缩到摘要文件/var/log/savacct和 /var/log/usracct 
中。这些摘要包含按命令名和用户名分类的系统统计数据。sa缺省情况下先读它们，然后读pacct文件，使报告能包含所有的可用信息。sa的输出有下面一些标记项： 
avio--每次执行的平均I/O操作次数 
cp--用户和系统时间总和，以分钟计 
cpu--和cp一样 
k--内核使用的平均CPU时间，以1k为单位 
k*sec--CPU[存储](http://www.storworld.com/)完整性，以1k-core秒 
re--实时时间，以分钟计 
s--系统时间，以分钟计 
tio--I/O操作的总数 
u--用户时间，以分钟计 
例如： 
842 173.26re 4.30cp 0avio 358k 
2 10.98re 4.06cp 0avio 299k find 
9 24.80re 0.05cp 0avio 291k ***other 
105 30.44re 0.03cp 0avio 302k ping 
104 30.55re 0.03cp 0avio 394k sh 
162 0.11re 0.03cp 0avio 413k security.sh* 
154 0.03re 0.02cp 0avio 273k ls 
56 31.61re 0.02cp 0avio 823k ping6.pl* 
2 3.23re 0.02cp 0avio 822k ping6.pl 
35 0.02re 0.01cp 0avio 257k md5sum 
97 0.02re 0.01cp 0avio 263k initlog 
12 0.19re 0.01cp 0avio 399k promisc_check.s 
15 0.09re 0.00cp 0avio 288k grep 
11 0.08re 0.00cp 0avio 332k awk 
用户还可以根据用户而不是命令来提供一个摘要报告。例如sa -m显示如下： 
885 173.28re 4.31cp 0avk 
root 879 173.23re 4.31cp 0avk 
alias 3 0.05re 0.00cp 0avk 
qmailp 3 0.01re 0.00cp 0avk 
4. Syslog设备 
    Syslog已被许多日志函数采纳，它用在许多保护措施中--任何程序都可以通过syslog 纪录事件。Syslog可以纪录系统事件，可以写到一个文件或设备中，或给用户发送一个信息。它能纪录本地事件或通过网络纪录另一个主机上的事件。 
Syslog设备依据两个重要的文件：/etc/syslogd（守护进程）和/etc/syslog.conf配置文件，习惯上，多数syslog信息被写到/var/adm或/var/log目录下的信息文件中（messages.*）。一个典型的syslog纪录包括生成程序的名字和一个文本信息。它还包括一个设备和一个优先级范围（但不在日之中出现）。 
每个syslog消息被赋予下面的主要设备之一： 
LOG_AUTH--认证系统：login、su、getty等 
LOG_AUTHPRIV--同LOG_AUTH，但只登录到所选择的单个用户可读的文件中 
LOG_CRON--cron守护进程 
LOG_DAEMON--其他系统守护进程，如routed 
LOG_FTP--文件传输[协议](http://cisco.chinaitlab.com/List_11.html)：ftpd、tftpd 
LOG_KERN--内核产生的消息 
LOG_LPR--系统打印机缓冲池：lpr、lpd 
LOG_MAIL--电子邮件系统 
LOG_NEWS--网络新闻系统 
LOG_SYSLOG--由syslogd（8）产生的内部消息 
LOG_USER--随机用户进程产生的消息 
LOG_UUCP--UUCP子系统 
LOG_LOCAL0~LOG_LOCAL7--为本地使用保留 
Syslog为每个事件赋予几个不同的优先级： 
LOG_EMERG--紧急情况 
LOG_ALERT--应该被立即改正的问题，如系统数据库破坏 
LOG_CRIT--重要情况，如硬盘错误 
LOG_ERR--错误 
LOG_WARNING--警告信息 
LOG_NOTICE--不是错误情况，但是可能需要处理 
LOG_INFO--情报信息 
LOG_DEBUG--包含情报的信息，通常旨在调试一个程序时使用 
syslog.conf文件指明syslogd程序纪录日志的行为，该程序在启动时查询配置文件。该文件由不同程序或消息分类的单个条目组成，每个占一行。对每类消息提供一个选择域和一个动作域。这些域由tab隔开：选择域指明消息的类型和优先级；动作域指明syslogd接收到一个与选择标准相匹配的消息时所执行的动作。每个选项是由设备和优先级组成。当指明一个优先级时，syslogd将纪录一个拥有相同或更高优先级的消息。所以如果指明 "crit"，那所有标为crit、alert和emerg的消息将被纪录。每行的行动域指明当选择域选择了一个给定消息后应该把他发送到哪儿。例如，如果想把所有邮件消息纪录到一个文件中，如下： 
#Log all the mail messages in one place 
mail.* /var/log/maillog 
其他设备也有自己的日志。UUCP和news设备能产生许多外部消息。它把这些消息存到自己的日志（/var/log/spooler）中并把级别限为"err"或更高。例如： 
# Save mail and news errors of level err and higher in aspecial file. 
uucp,news.crit /var/log/spooler 
当一个紧急消息到来时，可能想让所有的用户都得到。也可能想让自己的日志接收并保存。 
#Everybody gets emergency messages， plus log them on anther machine 
*.emerg * 
*.emerg @[linux](http://linux.chinaitlab.com/)aid.com.cn 
alert消息应该写到root和tiger的个人账号中： 
#Root and Tiger get alert and higher messages 
*.alert root,tiger 
有时syslogd将产生大量的消息。例如内核（"kern"设备）可能很冗长。用户可能想把内核消息纪录到/dev/console中。下面的例子表明内核日志纪录被注释掉了： 
#Log all kernel messages to the console 
#Logging much else clutters up the screen 
#kern.* /dev/console 
用户可以在一行中指明所有的设备。下面的例子把info或更高级别的消息送到/var/log 
/messages，除了mail以外。级别"none"禁止一个设备： 
#Log anything（except mail）of level info or higher 
#Don log private authentication messages! 
*.info:mail.none;authpriv.none /var/log/messages 
在有些情况下，可以把日志送到打印机，这样网络入侵者怎么修改日志都没有用了。通常要广泛纪录日志。Syslog设备是一个攻击者的显著目标。一个为其他主机维护日志的系统对于防范[服务器](http://server.chinaitlab.com/)攻击特别脆弱，因此要特别注意。 
有个小命令logger为syslog（3）系统日志文件提供一个shell命令接口，使用户能创建日志文件中的条目。
用法：logger 
例如：logger This is a test！ 
它将产生一个如下的syslog纪录：Aug 19 22:22:34 tiger: This is a test! 
注意不要完全相信日志，因为攻击者很容易修改它的。 
5. 程序日志 
    许多程序通过维护日志来反映系统的[安全](http://security.chinaitlab.com/)状态。su命令允许用户获得另一个用户的权限，所以它的安全很重要，它的文件为sulog。同样的还有 sudolog。另外，想Apache有两个日志：access_log和error_log
