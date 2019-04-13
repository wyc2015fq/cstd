
# Linux crond 不执行原因分析 - 阳光岛主 - CSDN博客

2018年02月11日 21:46:20[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2472


为了定时监控Linux系统CPU、内存、负载的使用情况，写了Linux Shell脚本，当达到一定值得时候，定时发送邮件通知。
但是，让crond来周期性执行脚本发送邮件通知时，遇到了问题，在crontab -e里面加入了执行脚本之后，发现脚本并没有执行。
可是，通过手动执行Shell脚本命令（./mimvp-email.sh)是正常的，因为手动执行脚本可以默认获取Linux的环境变量，但通过Crontab做的定时任务，则无法获取环境变量。
分析了原因，crond不执行的原因主要有以下几个方面：
**1、crond服务没启动**
ps -ef | grep -v grep | grep crond         // 查看crond服务是否运行
service crond start           //关闭服务
service crond stop           //关闭服务
service crond restart       //重启服务
service crond reload       //重新载入配置
**2、用户没有执行crond的权限**
vim  /etc/cron.deny文件用来控制哪些用户不能执行crond服务的功能。
可以将自己从文件中删去，或者联系root
**3、crontab不提供所执行用户的环境变量**
解决方法：在脚本中加入下面这一行：
. /etc/profile
. ~/.bash_profile
**4、没有使用绝对路径**
这里的绝对路径包括脚本中的路径和crond命令中的路径两个方面，例如：
*/10 * * * * sh/root/script/mysql_files_monitor.sh&
**5、如果上面都没有解决问题的话可以再找找问题：**
1）去邮件看看，在这个过程中用户应该会收到邮件，比如收到这样的提示：
vim /var/spool/mail/root
You have mail in /var/spool/mail/root
去看看里面就有crond的内容
文件太大打不开，可以截取最后1000行查看
tail -n 1000 /var/spool/mail/root > aaa.txt  &&  vim aaa.txt
2）在脚本里面加入output用来调试
可以在crontab的脚本里面添加个
echo $PATH > /tmp/test.log
对比和终端下执行脚本的echo $PATH
**6、crond进程太多，全部杀死重启crond服务**
\#!/bin/bash
for i in $(ps -elf | grep -v grep | grep crond | awk -F " " '{print $4}'); do
kill -9  $i
done
使用root执行重启,后问题解决:
service crond restart
**7、crond防止脚本周期内未执行完重复执行**
个人体会:flock -xn my.lock cmd
my.lock是一个文件，可以是任意文件，可以新建一个空文件
当flock 获得锁后就会执行后面的 cmd
**测试过程:**
$1: flock -xn my.lock sleep 20
$2: flock -xn my.lock ls
只有当1返回后， 2的ls才会成功
如果某脚本要运行30分钟，可以在Crontab里把脚本间隔设为至少一小时来避免冲突。而比较糟的情况是可能该脚本在执行周期内没有完成，接着第 二个脚本又开始运行了。如何确保只有一个脚本实例运行呢？一个好用的方法是利用lockf（FreeBSD 8.1下为lockf，CentOS  5.5下为flock），在脚本执行前先检测能否获取某个文件锁，以防止脚本运行冲突。
**lockf 参数如下**
-k：一直等待获取文件锁。
-s：silent，不发出任何信息，即使拿不到文件锁。
-t seconds：设定timeout的时间是seconds秒，如果超过时间，则自动放弃。
以下crontab计划任务执行前，需获取临时文件create.lock 文件锁，crontab计划任务的内容如下：
1 */10 * * * * (lockf -s -t 0 /tmp/create.lock/usr/bin/python /home/project/cron/create_tab.py  >> /home/project/logs/create.log 2>&1)
若第一个实例在10分钟内没有运行完，第2个实例不会运行。我以前是通过Shell脚本来解决这个问题的，比如用while...do循环，然后放在后台执行。但后来发现其实用flock或lockf方法更为简单。
**附上linux下的flock的用法：**
flock (util-linux 2.13-pre7)
Usage: flock [-sxun][-w \#] fd\#
flock [-sxon][-w \#] file [-c] command...
-s  --shared     Get a shared lock
\#共享锁,在定向为某文件的FD上设置共享锁而未释放锁的时间内,其他进程试图在定向为此文件的FD上设置独占锁的请求失败,而其他进程试图在定向为此文件的FD上设置共享锁的请求会成功
-x  --exclusive  Get an exclusive lock
\#独占或排他锁,在定向为某文件的FD上设置独占锁而未释放锁的时间内,其他进程试图在定向为此文件的FD上设置共享锁或独占锁都会失败。只要未设置-s参数,此参数默认被设置
-u  --unlock     Remove a lock
\#手动解锁,一般情况不必须,当FD关闭时,系统会自动解锁,此参数用于脚本命令一部分需要异步执行,一部分可以同步执行的情况
-n  --nonblock   Fail rather than wait
\#为非阻塞模式,当试图设置锁失败,采用非阻塞模式,直接返回1,
-w  --timeout    Wait for a limited amount of time
\#设置阻塞超时,当超过设置的秒数,就跳出阻塞,返回1
-o  --close      Close file descriptor before running command
-c  --command    Run a single command string through the shell 执行其后的comand
-h  --help       Display this text
-V  --version    Display version
举个例子执行如下脚本：
每天23:30的时候执行一个脚本，但是执行前必须要获得排他文件锁，否则无法执行命令
1 30 23 * * *flock -xn /tmp/test.lock-c '/usr/local/php test.php'
**8、; 和 && 区别**
“;” 和 “&&”是有区别的
**“;”：**不管cmd1执行的结果如何，都执行cmd2
“&&”：只有cmd1执行返回的结果是成功的，才执行cmd2
cmd1 && cmd2; cmd3
- cmd1 is executed, if it succeeds, then execute cmd2. and then cmd3 (regardless of cmd2 success or not)
- cmd1 is executed, if it fails, then cmd3 (cmd2 won't be executed)
**9、如果遇到shell语法错误**
`Syntax error: "(" unexpected`**解决方法：**
需指定shell解释器命令：SHELL=/bin/bash（请参见上面 crontab编辑示例**SHELL=/bin/bash**）
或者参见：[LINUX - BASH Syntax Error](http://efreedom.com/Question/1-6347119/BASH-Syntax-Error)
**如果遇到路径错误**
在 /var/spool/crontab/yanggang 中，添加了如下命令，在日志文件 /var/spool/mail/yanggang 中提示找不到 xxx.sh 路径
30 * * * *  /home/barry/top800/top10/top10_fruits/top10_all.sh
或
30 * * * * bash /home/barry/top800/top10/top10_fruits/top10_all.sh
这是因为你在crontab中使用了绝对路径执行脚本 top10_all.sh，因此在脚本 top10_all.sh 中引用的其它脚本也都需要使用绝对路径，才能被crontab找到并执行。
**那么该如何避免绝对路径呢，推荐采用如下格式：**
30 * * * * cd /home/barry/top800/top10/top10_fruits/ && ./top10_all.sh（**推荐用此方式**）
先进入该目录，然后在执行脚本；否则，执行脚本中的其它脚本都需要加绝对路径

**参考推荐：**
[CentOS 7.2上 crontab 计划任务](https://blog.mimvp.com/article/14634.html)
[linux定时运行命令脚本——crontab](https://blog.mimvp.com/article/11820.html)
[CentOS crontab 定时任务不执行的解决](https://blog.mimvp.com/article/7470.html)
[WordPress定时任务(wp-cron.php)造成主机CPU超标解决办法](https://blog.mimvp.com/article/10131.html)


