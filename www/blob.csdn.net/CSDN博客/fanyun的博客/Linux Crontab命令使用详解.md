# Linux  Crontab命令使用详解 - fanyun的博客 - CSDN博客
2016年06月02日 09:16:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1041
所属专栏：[Linux系统开发维护实战](https://blog.csdn.net/column/details/linux-01.html)
在我们的日常开发工作中，经常用到，如互联网金融行业的定时批处理业务，核心系统定时状态检测及状态日志生成，数据库文件的定时入库，定时生成批量付息，交易系统文件定时升级更新，日终交易文件上传备份至文件服务器等等，都会用到这个命令，从而大大减少工作量，提高工作效率，现在总结一下它的常用方法和参数配置，与大家分享。(也不是最全的，如有补充和建议，欢迎留言,也可以加入我们的IT技术交流群
 62775887).
**一.  Crontab介绍**
      crontab命令的功能就是让系统在指定的时间，去执行某个指定的工作，我们可以使用crontab指令来管理cron机制。
**1.1 /etc/crontab 文件**
在/etc目录下有一个crontab文件，这里存放有系统运行的一些调度程序。每个用户可以建立自己的调度crontab。
如：
[root@dave ~]# cat /etc/crontab
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
HOME=/
# run-parts
01 * * * * root run-parts /etc/cron.hourly
02 4 * * * root run-parts /etc/cron.daily
22 4 * * 0 root run-parts /etc/cron.weekly
42 4 1 * * root run-parts /etc/cron.monthly
通过crontab  -e命令给当前系统添加定时任务，如下图所示，增加一个定时文件采集任务：
![](https://img-blog.csdn.net/20160602094650261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
通过crontab  -l命令查看当前系统的定时任务，如下图所示，系统有一个定时文件采集任务：
![](https://img-blog.csdn.net/20160602094732033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果crontab  -l出现如上命令行，则表示添加任务成功。  
**1.2/etc/cron.deny 和/etc/cron.allow文件**
/etc/cron.deny 表示不能使用crontab命令的用户
/etc/cron.allow 表示能使用crontab的用户。
如果两个文件同时存在，那么/etc/cron.allow优先。
如果两个文件都不存在，那么只有超级用户可以安排作业。
每个用户都会生成一个自己的crontab文件。这些文件在/var/spool/cron目录下：
如：
[root@dave ~]# cd /var/spool/cron
[root@dave cron]# ls
oracle  root 
我们直接查看这个文件，里面的内容和对应用户显示的crontab -l一致。
[root@dave cron]# cat oracle
00 6 * * * /u02/scripts/del_st_archive.sh>/u02/scripts/del_st_arch.log 2>&1
[root@dave cron]# cat root
0 12 * * * /root/bin/sync-clock.sh
[root@dave cron]#  
**二.  Crontab使用说明**
**2.1  Crontab语法**
usage:  crontab[-u user] file
        crontab[-u user] [ -e | -l | -r ]
               (default operation is replace, per 1003.2)
        -e      (edit user's crontab)
        -l      (list user's crontab)
        -r      (delete user's crontab)
        -i      (prompt before deleting user's crontab)
        -s     (selinux context)
其中，file是命令文件的名字。如果在命令行中指定了这个文件，那么执行crontab命令，则将这个文件拷贝到crontabs目录下；如果在命令行中没有制定这个文件，crontab命令将接受标准输入（键盘）上键入的命令，并将他们也存放在crontab目录下。
“内事不决问百度，外事不决问谷歌，在Linux/Uinux系统里，我们就要问man了”.
帮助：
[root@dave ~]# man crontab
CRONTAB(1)                                                          CRONTAB(1)
NAME
       crontab -maintain crontab files for individual users (ISC Cron V4.1)
SYNOPSIS
       crontab[-u user] file
       crontab[-u user] [-l | -r | -e] [-i] [-s] 
DESCRIPTION
      Crontab  is the program used toinstall, deinstall or list the tables used to drive the cron(8) daemon in ISCCron.  Each user can have their owncrontab,  and  though these are  files in /var/spool/ , theyare not intended
 to be edited directly. For SELinux in mls mode can be evenmore crontabs  -  for each  range.  For more  see selinux(8).
       If  the cron.allow  file  exists, then  you must be listed thereinin order to be allowed to use this command. If  the  cron.allow file  does  not exist  but  the cron.deny file  does  exist, then you must not be listed
 in the cron.deny file in order to use thiscommand.  If neither of these filesexists, only the  super  user will be allowed to use this command.
OPTIONS
       -u     It specifies  the name of the userwhose crontab is to be tweaked.  Ifthis  option is not given, crontabexamines "your" crontab, i.e., the crontab  of the  person executing the command.  Note thatsu(8) can confuse
 crontab and               that if you are running inside of su(8)you should always use the -u  option               for  safety¡¯s sake.  The first form of this command is used toinstall a new               crontab fromsome named file or standard input if
 the pseudo-filename "-" is               given.
       -l     The current crontab will be displayed onstandard output.
       -r     The current crontab will be be removed.
       -e     This option  is used to edit thecurrent crontab using the editor specified by the VISUAL or EDITOR environmentvariables.  After you exit from theedi-tor, the modified crontab will be installed automatically.
       -i     This option  modifies the -r option toprompt the user for a ¡¯y/Y¡¯ response before actually removing the crontab.
       -s     It will append the current SELinuxsecurity context string as  an  MLS_LEVEL setting  to the  crontab file before editing /replacement occurs - see the documentation of MLS_LEVEL in crontab(5).
SEE ALSO
      crontab(5), cron(8)
FILES
      /etc/cron.allow
      /etc/cron.deny
STANDARDS
       Thecrontab command conforms to IEEE Std1003.2-1992 (¡®¡®POSIX¡¯¡¯).  This new command syntax  differs  from previous versions of Vixie Cron, as well as from the classic
       SVR3syntax.
DIAGNOSTICS
       A fairlyinformative usage message appears if you run it with a bad command line.
AUTHOR
       Paul Vixie<vixie@isc.org>
4th Berkeley Distribution       16 Januar 2007                      CRONTAB(1)
**2.2  Crontab 格式说明**
我们可以用crontab -e添加要执行的命令。命令执行的结果，无论是标准输出还是错误输出，都将以邮件形式发给用户。
添加的命令必须以如下格式：
   * * * * */command path
前五个字段可以取整数值，指定何时开始工作，第六个域是字符串，即命令字段，其中包括了crontab调度执行的命令。各个字段之间用spaces和tabs分割。
前5个字段分别表示：
分钟：0-59
小时：1-23
日期：1-31
月份：1-12
星期：0-6（0表示周日）
还可以用一些特殊符号：
       *：表示任何时刻
       ,：　表示分割
　　－：表示一个段，如第二端里： 1-5，就表示1到5点
       /n : 表示每个n的单位执行一次，如第二段里，*/1,就表示每隔1个小时执行一次命令。也可以写成1-23/1.
一些示例：
00 8,12,16 * * * /data/app/scripts/monitor/df.sh
30 2 * * */data/app/scripts/hotbackup/hot_database_backup.sh
10 8,12,16 * * */data/app/scripts/monitor/check_ind_unusable.sh
10 8,12,16 * * */data/app/scripts/monitor/check_maxfilesize.sh
10 8,12,16 * * * /data/app/scripts/monitor/check_objectsize.sh
43 21 * * * 21:43 执行
15 05 * * * 05:15执行
0 17 * * *       17:00 执行
0 17 * * 1       每周一的 17:00执行
0,10 17 * * 0,2,3 每周日,周二,周三的
 17:00和 17:10执行
0-10 17 1 * *    毎月1日从 17:00到7:10毎隔1分钟执行
0 0 1,15 * 1     毎月1日和 15日和一日的
 0:00 执行
42 4 1 * * 毎月1日的 4:42分执行
0 21 * * 1-6周一到周六 21:00执行
0,10,20,30,40,50 * * * *　每隔10分执行
*/10 * * * *     每隔10分执行
* 1 * * *        从1:0到1:59每隔1分钟执行
0 1 * * *        1:00 执行
0 */1 * * *      毎时0分每隔1小时执行
0 * * * *        毎时0分每隔1小时执行
2 8-20/3 * * *    8:02,11:02,14:02,17:02,20:02执行
30 5 1,15 * *     1日和 15日的 5:30执行
**2.3  & 后台执行命令**
当在前台运行某个作业时，终端被该作业占据；而在后台运行作业时，它不会占据终端。可以使用&命令把作业放到后台执行。
如：
    30 2 * * */data/app/scripts/hotbackup/hot_database_backup.sh &
在后台运行作业时要当心：需要用户交互的命令不要放在后台执行，因为这样你的机器就会在那里傻等。
不过，作业在后台运行一样会将结果输出到屏幕上，干扰你的工作。如果放在后台运行的作业会产生大量的输出，最好使用下面的方法把它的输出重定向到某个文件中：
如：
              command >out.file 2>&1 &
在这个例子中，2>&1表示所有的标准输出和错误输出都将被重定向到一个叫做out.file的文件中。
**2.4  2>&1 含义**
先看一个例子：
0 2 * * * /u01/test.sh >/dev/null 2>&1 &
      这句话的意思就是在后台执行这条命令，并将错误输出2重定向到标准输出1，然后将标准输出1全部放到/dev/null文件，也就是清空。
在这里有有几个数字的意思：
       0表示键盘输入
       1表示标准输出
       2表示错误输出.
我们也可以这样写：
0 2 * * * /u01/test.sh >/u01/out.file &  --这里没写，默认是1
0 2 * * * /u01/test.sh 1>/u01/out.file &
0 2 * * * /u01/test.sh 2>/u01/out.file &
0 2 * * * /u01/test.sh 2>/u01/out.file  2>&1&
将tesh.sh命令输出重定向到out.file,即输出内容不打印到屏幕上，而是输出到out.file文件中。
     2>&1 是将错误输出重定向到标准输出。然后将标准输入重定向到文件out.file。
&1 表示的是文件描述1，表示标准输出，如果这里少了&就成了数字1，就表示重定向到文件1。
  & ：后台执行
测试：
ls 2>1 ：不会报没有2文件的错误，但会输出一个空的文件1；
ls xxx 2>1：没有xxx这个文件的错误输出到了1中；
ls xxx 2>&1：不会生成1这个文件了，不过错误跑到标准输出了；
ls xxx >out.txt 2>&1 == ls xxx 1>out.txt2>&1；因为重定向符号>默认是1，这句就把错误输出和标准输出都传到out.txt文件中。
**2.5  2>&1写在后面的原因**
格式：command >file 2>&1   == command  1> file 2>&1
首先是command >file将标准输出重定向到file中，
 2>&1 是标准错误拷贝了标准输出，也就是同样被重定向到file中，最终结果就是标准输出和错误都被重定向到file中。
如果改成： command2>&1 >file
     2>&1 标准错误拷贝了标准输出的行为，但此时标准输出还是在终端。>file后输出才被重定向到file，但标准错误仍然保持在终端。
其它功能添加：
实现linux定时任务有:cron、anacron、at等，这里主要介绍cron服务。
名词解释：
cron是服务名称，crond是后台进程，crontab则是定制好的计划任务表。
软件包安装：
要使用cron服务，先要安装vixie-cron软件包和crontabs软件包，两个软件包作用如下：
vixie-cron软件包是cron的主程序。
crontabs软件包是用来安装、卸装、或列举用来驱动 cron 守护进程的表格的程序。
查看是否安装了cron软件包: rpm -qa|grep vixie-cron
查看是否安装了crontabs软件包:rpm -qa|grep crontabs
如果没有安装，则执行如下命令安装软件包(软件包必须存在)
rpm -ivh vixie-cron-4.1-54.FC5*
rpm -ivh crontabs*
如果本地没有安装包，在能够连网的情况下可以在线安装
yum install vixie-cron
yum install crontabs
查看crond服务是否运行：
pgrep crond
或
/sbin/service crond status
或
ps -elf|grep crond|grep -v "grep"
crond服务操作命令:
/sbin/service crond start //启动服务  
/sbin/service crond stop //关闭服务  
/sbin/service crond restart //重启服务  
/sbin/service crond reload //重新载入配置
cron是一个linux下的定时执行工具，可以在无需人工干预的情况下运行作业。由于Cron 是Linux的内置服务，但它不自动起来，可以用以下的方法启动、关闭这个服务：
/sbin/service crond start //启动服务
/sbin/service crond stop //关闭服务
/sbin/service crond restart //重启服务
/sbin/service crond reload //重新载入配置
 你也可以将这个服务在系统启动的时候自动启动：
在/etc/rc.d/rc.local这个脚本的末尾加上：
/sbin/service crond start
现在Cron这个服务已经在进程里面了，我们就可以用这个服务了
-------------------------------------
 以Linux下定时备份mysql为例说明下
写一个简单的mysql备份shell脚本
vi
#!/bin/sh
da=`date +%Y%m%d%H%M%S`
mysqldump -u root -pdongjj --all-database>/root/mysqlbakup/$da
保存为 mysqlbak.sh
然后crontab-e
 0 3 * * * /root/mysqlbak.sh 
保存退出
