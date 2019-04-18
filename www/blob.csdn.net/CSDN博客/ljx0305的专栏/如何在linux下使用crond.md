# 如何在linux下使用crond - ljx0305的专栏 - CSDN博客
2009年08月19日 17:26:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3344
下文内容大部分是根据[《Cron Help Guide》](http://www.linuxhelp.net/guides/cron/)翻译而来，有些部分是自己加上的。
全文如下：
cron来源于希腊单词chronos（意为“时间”），是linux系统下一个自动执行指定任务的程序。例如，你想在每晚睡觉期间创建某些文件或文件夹的备份，就可以用cron来自动执行。
服务的启动和停止
cron服务是linux的内置服务，但它不会开机自动启动。可**以用以下命令启动和停止服务：**
**/sbin/service crond start**
**/sbin/service crond stop**
**/sbin/service crond restart**
**/sbin/service crond reload**
以上1-4行分别为启动、停止、重启服务和重新加载配置。
要把cron设为在开机的时候自动启动，在 /etc/rc.d/rc.local 脚本中加入 /sbin/service crond start 即可。
查看、编辑和删除
cron把命令行保存在crontab（cron table）文件里，这个文件通常在 /etc 目录下。每个系统用户都可以有自己的crontab（在 /var/spool/cron/ 下）。要查看当前用户的crontab，输入 crontab -l；要编辑crontab，输入 crontab -e；要删除crontab，输入 crontab -r。如当前是root身份，要查看/编辑/删除/某用户的crontab，只需在相应的命令后加上 -u USERNAME（如 crontab -e -u USERNAME）即可。crontab文件的默认编辑器是vi，可以输入 export VISUAL=’editor’ 更改默认编辑器。
cron服务每分钟不仅要读一次 /var/spool/cron 目录内的所有文件，还需要读一次 /etc/crontab 文件。配置这个文件也能让cron执行任务。使用crontab命令是对用户级任务的配置，而编辑 /etc/crontab 文件是对系统级任务的配置。
语法说明
以下是两个cron语句的例子（在 /etc/crontab 文件里）。前者用来晚间备份 /etc 目录，后者运行Analog程序处理服务器的统计信息。
12 3 * * * root tar czf /usr/local/backups/daily/etc.tar.gz /etc >> /dev/null 2>&1
52 5 * * * root /usr/local/src/analog-5.32-lh/analog >> /dev/null 2>&1
以下是cron语句中的字段与字段说明：
|字段|说明|
|----|----|
|1|分钟（0-59）|
|2|小时（2-24）|
|3|日期（1-31）|
|4|月份（1-12；或英文缩写Jan、Feb等）|
|5|周几（0-6，0为周日；或单词缩写Sun、Mon等）|
|6|用户名（执行命令时以此用户的身份）|
|7|要执行的命令（路径）|
|||
现在来看第一行：
12 3 * * * root tar czf /usr/local/backups/daily/etc.tar.gz /etc >> /dev/null 2>&1
这条语句将在*每天的凌晨3点12分*（03:12）运行 tar czf /usr/local/backups/daily/etc.tar.gz /etc 命令。>> /dev/null 2>&1 表示把所有标准输出发送到 /dev/null（linux的回收站），把标准错误输出（2）发送到和标准输出（1）同样的地方（即 /dev/null）。运行这行命令将不会产生任何输出。
这条语句可以变得稍微复杂一点：
30 15 13 6 1 * root tar czf /usr/local/backups/daily/etc.tar.gz /etc >> /dev/null 2>&1
它将在*6月13日周一的15:30*运行 tar czf /usr/local/backups/daily/etc.tar.gz /etc 命令。
以下语句可以达到同样的效果：
30 15 13 Jun Mon * root tar czf /usr/local/backups/daily/etc.tar.gz /etc >> /dev/null 2>&1
如果你想以用户joey的身份*每小时的第15分钟*运行某个程序，可以使用：
15 * * * * joey /usr/bin/somecommand >> /dev/null 2>&1
其中的星号（*）是通配符，表示cron将忽略这个字段。
如果你想*每两小时*就运行某个程序，可以在小时字段里使用 */2。它将会在2点，4点，6点……22点，24点运行。具体语句如下：
0 */2 * * * joey /usr/bin/somecommand >> /dev/null 2>&1
cron语句中还可以使用逗号（,）来指定多个时间。例如你想在*每小时的15分和30分*运行某个程序，可以在分钟字段使用 15,30：
15,30 * * * * joey /usr/bin/somecommand >> /dev/null 2>&1
如果你想在*每月的第一周（即1号到7号）每天的指定时间*运行某个程序，可以在日期字段使用 1-7：
15,30 */2 1-7 * * joey /usr/bin/somecommand >> /dev/null 2>&1
这条语句将在*每月的第1-7日每两小时的15分和30分*（02:15，02:30……22: 15，22:30等）运行 /usr/bin/somecommand 命令。
如果你想在*每天的16:18*执行一个脚本集合，可以把所有要执行的脚本放到一个目录中（如 /home/username/cron），可以使用：
18 16 * * * root run-parts /home/username/cron >> /dev/null 2>&1
如果你想保存某个程序的输出结果， 可以把 >> /dev/null 2>&1 替换为 >> /home/user/somecommand.log 2>&1 。
以下举几个例子说明问题： 
每天早上6点 
0 6 * * * echo "Good morning." >> /tmp/test.txt //注意单纯echo，从屏幕上看不到任何输出，因为cron把任何输出都email到root的信箱了。 
每两个小时 
0 */2 * * * echo "Have a break now." >> /tmp/test.txt 
晚上11点到早上8点之间每两个小时，早上八点 
0 23-7/2，8 * * * echo "Have a good dream：）" >> /tmp/test.txt 
每个月的4号和每个礼拜的礼拜一到礼拜三的早上11点 
0 11 4 * 1-3 command line 
1月1日早上4点 
0 4 1 1 * command line
**总结**
- **查看当前用户的cron配置，使用 crontab -l **
- **编辑当前用户的cron配置，使用 crontab -e **
- **删除当前用户的cron配置，使用 crontab -r **
- **以root身份查看/编辑/删除某用户的cron配置，在命令后加上 -u USERNAME **
- **配置系统级的任务，编辑 /etc/crontab 文件 **
cron 是一个可以用来根据时间、日期、月份、星期的组合来调度对重复任务的执行的守护进程。 
cron 假定系统持续运行。如果当某任务被调度时系统不在运行，该任务就不会被执行。 
要使用 cron 服务，你必须安装了 vixie-cron RPM 软件包，而且必须在运行 crond 服务。要判定该软件包是否已安装，使用 rpm -q vixie-cron 命令。要判定该服务是否在运行，使用 /sbin/service crond status 命令。 
**cron 的主配置文件是 /etc/crontab，它包括下面几行：**
**SHELL=/bin/bash **
**PATH=/sbin:/bin:/usr/sbin:/usr/bin **
**MAILTO=root**    #如果出现错误，或者有数据输出，数据作为邮件发给这个帐号
**HOME=/**    #使用者运行的路径,这里是根目录
**# run-parts **
**01 * * * * root run-parts /etc/cron.hourly **
**02 4 * * * root run-parts /etc/cron.daily **
**22 4 * * 0 root run-parts /etc/cron.weekly **
**42 4 1 * * root run-parts /etc/cron.monthly**
前四行是用来配置 cron 任务运行环境的变量。 SHELL 变量的值告诉系统要使用哪个 shell 环境（在这个例子里是 bash shell）；PATH 变量定义用来执行命令的路径。cron 任务的输出被邮寄给 MAILTO 变量定义的用户名。如果 MAILTO 变量被定义为空白字符串（MAILTO=""），电子邮件就不会被寄出。HOME 变量可以用来设置在执行命令或脚本时使用的主目录。 
**/etc/crontab 文件中的每一行都代表一项任务，它的格式是**：
**minute   hour   day    month    dayofweek    command**
minute — 分钟，从 0 到 59 之间的任何整数  
hour — 小时，从 0 到 23 之间的任何整数  
day — 日期，从 1 到 31 之间的任何整数（如果指定了月份，必须是该月份的有效日期）  
month — 月份，从 1 到 12 之间的任何整数（或使用月份的英文简写如 jan、feb 等等） 
dayofweek — 星期，从 0 到 7 之间的任何整数，这里的 0 或 7 代表星期日（或使用星期的英文简写如 sun、mon 等等） 
command — 要执行的命令（命令可以是 ls /proc >> /tmp/proc 之类的命令，也可以是执行你自行编写的脚本的命令。） 
在以上任何值中，星号（*）可以用来代表所有有效的值。譬如，月份值中的星号意味着在满足其它制约条件后每月都执行该命令。 
整数间的短线（-）指定一个整数范围。譬如，1-4 意味着整数 1、2、3、4。 
用逗号（,）隔开的一系列值指定一个列表。譬如，3, 4, 6, 8 标明这四个指定的整数。 
正斜线（/）可以用来指定间隔频率。在范围后加上 / 意味着在范围内可以跳过 integer。譬如，0-59/2 可以用来在分钟字段定义每两分钟。间隔频率值还可以和星号一起使用。例如，*/3 的值可以用在月份字段中表示每三个月运行一次任务。 
开头为井号（#）的行是注释，不会被处理。 
如你在 /etc/crontab 文件中所见，它使用 run-parts 脚本来执行 /etc/cron.hourly、/etc/cron.daily、/etc/cron.weekly 和 /etc/cron.monthly 目录中的脚本，这些脚本被相应地每小时、每日、每周、或每月执行。这些目录中的文件应该是 shell 脚本。如果你想要增加一个任务到cron里，把运行任务的脚本（或者脚本的连接）放在对应的目录里;如果去掉run-parts这个参数的话，后面就可以写要运行的某个脚本的完整路径名，而不是文件夹名了
以下是/etc/crontab的一个例子，该任务每周运行三次(星期一，星期三，星期五)： 
　　02 4 * * 1,3,5 root run-parts  /etc/cron.weekly 
　　以下是一个新闻组上的例子，显示如何自动用电子邮件发送一个日记文件 
　　回复：help in crontab 
　　发信：Dean Thompson <Dean.Thompson@csse.monash.edu.au> 日期： 2001-03-03 16:35 
　　Newsgroups: comp.os.linux.admin,comp.os.linux.networking,comp.os.linux.security 
　　> 我该如何在/etc/crontab文件里设置，让每天的工作记录文件自动发送到信箱abc@abc.com < /var/log？ 
你可以试试看以下的例子 
0 0 * * * (/bin/mail abc@abc.com < /var/log/messages) > /dev/null 2>&1
如果某 cron 任务需要根据调度来执行，而不是每小时、每日、每周、或每月地执行，它可以被添加到 /etc/cron.d 目录中。该目录中的所有文件使用和 /etc/crontab 中一样的语法。如下：
**# record the memory usage of the system every monday **
**# at 3:30AM in the file /tmp/meminfo **
**30 3 * * mon cat /proc/meminfo >> /tmp/meminfo **
**# run custom script the first day of every month at 4:10AM **
**10 4 1 * * /root/scripts/backup.sh**
根用户以外的用户可以使用 crontab 工具来配置 cron 任务。所有用户定义的 crontab 都被保存在 /var/spool/cron 目录中，并使用创建它们的用户身份来执行。要以某用户身份创建一个 crontab 项目，登录为该用户，然后键入 crontab -e 命令，使用由 VISUAL 或 EDITOR 环境变量指定的编辑器来编辑该用户的 crontab。该文件使用的格式和 /etc/crontab 相同。当对 crontab 所做的改变被保存后，该 crontab 文件就会根据该用户名被保存，并写入文件 /var/spool/cron/username 中。 
cron 守护进程每分钟都检查 /etc/crontab 文件、etc/cron.d/ 目录、以及 /var/spool/cron 目录中的改变。如果发现了改变，它们就会被载入内存。这样，当某个 crontab 文件改变后就不必重新启动守护进程了。 
/etc/cron.allow 和 /etc/cron.deny 文件被用来限制对 cron 的使用。这两个使用控制文件的格式都是每行一个用户。两个文件都不允许空格。如果使用控制文件被修改了，cron 守护进程（crond）不必被重启。使用控制文件在每次用户添加或删除一项 cron 任务时都会被读取。 
无论使用控制文件中的规定如何，根用户都总是可以使用 cron。 
如果 cron.allow 文件存在，只有其中列出的用户才被允许使用 cron，并且 cron.deny 文件会被忽略。 
如果 cron.allow 文件不存在，所有在 cron.deny 中列出的用户都被禁止使用 cron。 
要启动 cron 服务，使用 /sbin/service crond start 命令。要停止该服务，使用 /sbin/service crond stop 命令。推荐你在引导时启动该服务。 
增加cron的工作 
当我们决定要在系统上定期执行某个命令，我们必须将这个要执行的命令定义成一个cron工作，可以直接将要建立的cron工作定义在crontab设置文件中，设置该命令执行的时机；或者也可以利用cron会定时执行/etc/cron.*目录SCRIPT的特性，将要执行的命令建立成为一个脚本文档，放置在该目录下，并且注意它的权限--必须可以执行！
参考资料：[http://www.cnblogs.com/youyou/archive/2007/01/20/625365.aspx](http://www.cnblogs.com/youyou/archive/2007/01/20/625365.aspx)
引用:http://www.cnblogs.com/huqingyu/archive/2009/04/15/1436664.html
