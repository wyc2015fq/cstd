# crontab用法（例子） - z69183787的专栏 - CSDN博客
2018年07月09日 17:10:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：241
[https://www.cnblogs.com/lsmsky/archive/2012/03/08/2385405.html](https://www.cnblogs.com/lsmsky/archive/2012/03/08/2385405.html)
每个操作系统都有它的自动定时启动程序的功能，Windows有它的任务计划，而[Linux](http://www.centos.bz/)对应的功能是[crontab](http://www.centos.bz/tag/crontab/)。
### crontab简介
crontab命令常见于Unix和类Unix的操作系统之中，用于设置周期性被执行的指令。该命令从标准输入设备读取指令，并将其存放于“crontab”文件中，以供之后读取和执行。该词来源于希腊语 chronos(χρόνος)，原意是时间。　　通常，crontab储存的指令被守护进程激活， crond常常在后台运行，每一分钟检查是否有预定的作业需要执行。这类作业一般称为cron jobs。
### crontab用法
crontab的格式如下面：
f1 f2 f3 f4 f5 program
其中 f1 是表示分钟，f2 表示小时，f3 表示一个月份中的第几日，f4 表示月份，f5 表示一个星期中的第几天。program 表示要执行程式的路径。
- 当 f1 为 * 时表示每分钟都要执行 program，f2 为 * 时表示每小时都要执行程式，其余类推
- 当 f1 为 a-b 时表示从第 a 分钟到第 b 分钟这段时间内要执行，f2 为 a-b 时表示从第 a 到第 b 小时都要执行，其余类推
- 当 f1 为 */n 时表示每 n 分钟个时间间隔执行一次，f2 为 */n 表示每 n 小时个时间间隔执行一次，其余类推
- 当 f1 为 a, b, c,… 时表示第 a, b, c,… 分钟要执行，f2 为 a, b, c,… 时表示第 a, b, c…个小时要执行，其余类推
管理员登录SSH,输入命令crontab -e编辑crontab文件，根据上面的格式输入并保存。
### crontab例子
每月每天每小时的第 0 分钟执行一次 /bin/ls :
- 0 * * * * /bin/ls
在 12 月内, 每天的早上 6 点到 12 点中，每隔 20 分钟执行一次 /usr/bin/backup :
- */20 6-12 * 12 * /usr/bin/backup
周一到周五每天下午 5:00 寄一封信给 alex@domain.name :
- 0 17 * * 1-5 mail -s "hi" alex@domain.name < /tmp/maildata
每月每天的午夜 0 点 20 分, 2 点 20 分, 4 点 20 分….执行 echo “haha”
- 20 0-23/2 * * * echo "haha"
晚上11点到早上8点之间每两个小时，早上8点
- 0 23-7/2，8 * * * date
在hp unix,中，每20分钟执行一次，表示为：0,20,40 * * * * 而不能采用*/n方式，否则出现语法错误
crontab用法其实很容易掌握，懂得使用crontab，对网站和服务器维护起到很大的帮助，比如定时备份，定时优化服务器等。
