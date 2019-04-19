# 如何实时查看linux下的日志 - xqhrs232的专栏 - CSDN博客
2017年06月02日 10:19:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3068
原文地址::[http://www.cnblogs.com/lcword/p/5917395.html](http://www.cnblogs.com/lcword/p/5917395.html)
相关文章
1、Ubuntu系统日志配置 /var/log/messages----[http://www.linuxidc.com/Linux/2015-04/116532.htm](http://www.linuxidc.com/Linux/2015-04/116532.htm)
2、linux输出日志，怎么查看Linux系统日志less/var/log/mess----[https://zhidao.baidu.com/question/755538762334357484.html](https://zhidao.baidu.com/question/755538762334357484.html)
cat /var/log/*.log
如果日志在更新，如何实时查看 tail -f /var/log/messages
还可以使用 watch -d -n 1 cat /var/log/messages
-d表示高亮不同的地方，-n表示多少秒刷新一次。
该指令，不会直接返回命令行，而是实时打印日志文件中新增加的内容，这一特性，对于查看日志是非常有效的。如果想终止输出，按 Ctrl+C 即可。
在Linux系统中，有三个主要的日志子系统：
　　连接时间日志--由多个程序执行，把纪录写入到/var/log/wtmp和/var/run/utmp，login等程序更新wtmp和utmp文件，使系统管理员能够跟踪谁在何时登录到系统。
　　进程统计--由系统内核执行。当一个进程终止时，为每个进程往进程统计文件（pacct或acct）中写一个纪录。进程统计的目的是为系统中的基本服务提供命令使用统计。
　　错误日志--由syslogd（8）执行。各种系统守护进程、用户程序和内核通过syslog（3）向文件/var/log/messages报告值得注意的事件。另外有许多UNIX程序创建日志。像HTTP和FTP这样提供网络服务的服务器也保持详细的日志。
　　常用的日志文件如下：
　　access-log　　　　　　　　 纪录HTTP/web的传输
　　acct/pacct　　　　　　　　 纪录用户命令
　　aculog　　　　　　　　　　 纪录MODEM的活动
　　btmp　　　　　　　　　　　　纪录失败的纪录
　　lastlog　　纪录最近几次成功登录的事件和最后一次不成功的登录
　　messages　　　　从syslog中记录信息（有的链接到syslog文件）系统启动后的信息和错误日志，是Red Hat Linux中最常用的日志之一 
　　sudolog　　　　　　　　　　 纪录使用sudo发出的命令
　　sulog　　　　　　　　　　 纪录使用su命令的使用
　　syslog　　　　 从syslog中记录信息（通常链接到messages文件）
　　utmp　　　　　　　　　　　　纪录当前登录的每个用户
　　wtmp　　　　　　　　一个用户每次登录进入和退出时间的永久纪录
　　xferlog　　　　　　　　　　 纪录FTP会话
/var/log/secure与安全相关的日志信息 
/var/log/maillog 与邮件相关的日志信息 
/var/log/cron 与定时任务相关的日志信息 
/var/log/spooler 与UUCP和news设备相关的日志信息 
/var/log/boot.log 守护进程启动和停止相关的日志消息 
　　utmp、wtmp和lastlog日志文件是多数重用UNIX日志子系统的关键--保持用户登录进入和退出的纪录。有关当前登录用户的信息记 录在文件utmp中；登录进入和退出纪录在文件wtmp中；最后一次登录文件可以用lastlog命令察看。数据交换、关机和重起也记录在wtmp文件 中。所有的纪录都包含时间戳。这些文件（lastlog通常不大）在具有大量用户的系统中增长十分迅速。例如wtmp文件可以无限增长，除非定期截取。许 多系统以一天或者一周为单位把wtmp配置成循环使用。它通常由cron运行的脚本来修改。这些脚本重新命名并循环使用wtmp文件。通常，wtmp在第
 一天结束后命名为wtmp.1；第二天后wtmp.1变为wtmp.2等等，直到wtmp.7。
　　每次有一个用户登录时，login程序在文件lastlog中察看用户的UID。如果找到了，则把用户上次登录、退出时间和主机名写到标准输出中，然 后login程序在lastlog中纪录新的登录时间。在新的lastlog纪录写入后，utmp文件打开并插入用户的utmp纪录。该纪录一直用到用户 登录退出时删除。utmp文件被各种命令文件使用，包括who、w、users和finger。
　　下一步，login程序打开文件wtmp附加用户的utmp纪录。当用户登录退出时，具有更新时间戳的同一utmp纪录附加到文件中。wtmp文件被程序last和ac使用。
　　具体命令
　　wtmp和utmp文件都是二进制文件，他们不能被诸如tail命令剪贴或合并（使用cat命令）。用户需要使用who、w、users、last和ac来使用这两个文件包含的信息。
　　who：who命令查询utmp文件并报告当前登录的每个用户。Who的缺省输出包括用户名、终端类型、登录日期及远程主机。例如：who（回车）显示
　　chyang　　　　 pts/0 Aug　　　　 18 15:06
　　ynguo　　　　 pts/2 Aug　　　　 18 15:32
　　ynguo　　　　 pts/3 Aug　　　　 18 13:55
　　lewis　　　　 pts/4 Aug　　　　 18 13:35
　　ynguo　　　　 pts/7 Aug　　　　 18 14:12
　　ylou　　　　 pts/8 Aug　　　　 18 14:15
　　如果指明了wtmp文件名，则who命令查询所有以前的纪录。命令who /var/log/wtmp将报告自从wtmp文件创建或删改以来的每一次登录。
　　w：w命令查询utmp文件并显示当前系统中每个用户和它所运行的进程信息。例如：w（回车）显示：3:36pm up 1 day, 22:34, 6 users, load average: 0.23, 0.29, 0.27。
　　USER　　 TTY　　　　FROM　　　　 LOGIN@ IDLE JCPU PCPU　　WHAT
　　chyang pts/0 202.38.68.242　　3:06pm 2:04 0.08s 0.04s -bash
        ynguo pts/2 202.38.79.47　　 3:32pm 0.00s 0.14s 0.05　　 w
　　lewis pts/3 202.38.64.233　　1:55pm 30:39 0.27s 0.22s -bash
　　lewis pts/4 202.38.64.233　　1:35pm 6.00s 4.03s 0.01s sh /home/users/
　　ynguo pts/7 simba.nic.ustc.e 2:12pm 0.00s 0.47s 0.24s telnet mail
　　ylou　　pts/8 202.38.64.235　　2:15pm 1:09m 0.10s 0.04s　　-bash
　　users：users用单独的一行打印出当前登录的用户，每个显示的用户名对应一个登录会话。如果一个用户有不止一个登录会话，那他的用户名将显示相同的次数。例如：users（回车）显示：chyang lewis lewis ylou ynguo ynguo
 　last：last命令往回搜索wtmp来显示自从文件第一次创建以来登录过的用户。例如：
　　chyang pts/9　　202.38.68.242 Tue Aug 1 08:34 - 11:23 (02:49)
　　cfan　　pts/6　　202.38.64.224 Tue Aug 1 08:33 - 08:48 (00:14)
　　chyang pts/4　　202.38.68.242 Tue Aug 1 08:32 - 12:13 (03:40)
　　lewis pts/3　　202.38.64.233 Tue Aug 1 08:06 - 11:09 (03:03)
　　lewis pts/2　　202.38.64.233 Tue Aug 1 07:56 - 11:09 (03:12)
　　如果指明了用户，那么last只报告该用户的近期活动，例如：last ynguo（回车）显示：
　　ynguo　　pts/4 simba.nic.ustc.e Fri Aug 4 16:50 - 08:20 (15:30)
　　ynguo　　pts/4 simba.nic.ustc.e Thu Aug 3 23:55 - 04:40 (04:44)
　　ynguo　　pts/11 simba.nic.ustc.e Thu Aug 3 20:45 - 22:02 (01:16)
　　ynguo　　pts/0 simba.nic.ustc.e Thu Aug 3 03:17 - 05:42 (02:25)
　　ynguo　　pts/0 simba.nic.ustc.e Wed Aug 2 01:04 - 03:16 1+02:12)
　　ynguo　　pts/0 simba.nic.ustc.e Wed Aug 2 00:43 - 00:54 (00:11)
　　ynguo　　pts/9 simba.nic.ustc.e Thu Aug 1 20:30 - 21:26 (00:55)
　　ac：ac命令根据当前的/var/log/wtmp文件中的登录进入和退出来报告用户连结的时间（小时），如果不使用标志，则报告总的时间。例如：ac（回车）显示：total 5177.47
　　ac -d（回车）显示每天的总的连结时间
　　Aug 12 total 261.87
　　Aug 13 total 351.39
　　Aug 14 total 396.09
　　Aug 15 total 462.63
　　Aug 16 total 270.45
　　Aug 17 total 104.29
　　Today total 179.00
　　ac -p （回车）显示每个用户的总的连接时间
　　ynguo 193.23
　　yucao 3.35
　　rong 133.40
　　hdai 10.52
　　zjzhu 52.87
　　zqzhou 13.14
　　liangliu 24.34
　　total 5178.22
　　lastlog：lastlog文件在每次有用户登录时被查询。可以使用lastlog命令来检查某特定用户上次登录的时间，并格式化输出上次登录日 志/var/log/lastlog的内容。它根据UID排序显示登录名、端口号（tty）和上次登录时间。如果一个用户从未登录过，lastlog显 示"**Never logged**。注意需要以root运行该命令，例如：
　　rong　　　　　　5　　 202.38.64.187　　　　　　　　 Fri Aug 18 15:57:01 +0800 2000
　　dbb　　　　　　　　　　　　　　　　　　　　　　　　　　 **Never logged in**
　　xinchen　　　　　　　　　　　　　　　　　　　　　　　　 **Never logged in**
　　pb9511　　　　　　　　　　　　　　　　　　　　　　　　 **Never logged in**
　　xchen　　　　 0　　 202.38.64.190　　　　　　　　 Sun Aug 13 10:01:22 +0800 2000
　　另外，可一加一些参数，例如，lastlog -u 102将报告UID为102的用户；lastlog -t 7表示限制上一周的报告。
# linux输出日志，怎么查看Linux系统日志less/var/log/mess
