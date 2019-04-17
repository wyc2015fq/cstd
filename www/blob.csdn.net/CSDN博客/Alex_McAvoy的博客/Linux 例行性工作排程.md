# Linux 例行性工作排程 - Alex_McAvoy的博客 - CSDN博客





2018年07月08日 01:18:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：104








# 【概述】





工作排程，就是为工作安排执行流程，其分为两种形式，一种是突发性的，一种是例行性的。

对于突发性的工作（工作完成后就不再出现），要使用 at 命令。

对于例行性的工作（每隔一定周期就要执行），要使用 crontab 命令。

常见的例行性工作排程有：周期性的清理／tmp，周期性的备份数据库，周期性的分析日志等等。

# 【crontab 的使用】

## 1.简介

crontab 来源于希腊语 chronos(χρνο)，原意是时间，该命令用于设置周期性被执行的指令。

该命令从标准输入设备读取指令，并将其存放于“crontab”文件中，以供之后读取和执行。



crontab 文件包含送交 cron 守护进程的一系列作业和指令。通常，crontab 储存的指令被守护进程激活，crond 常常在后台运行，每一分钟检查是否有预定的作业需要执行。这类作业一般称为cron jobs。

每个用户可以拥有自己的 crontab 文件，同时，操作系统保存一个针对整个系统的 crontab 文件，该文件通常存放于 /etc 或者 /etc 之下的子目录中，而这个文件只能由系统管理员来修改，即：**使用权限是系统管理员与 crontab 文件所有者，修改权限是系统管理员**。

通过 crontab 命令，可在固定的间隔时间执行指定的系统指令或 shell script 脚本，时间间隔的单位可以是分钟、小时、日、月、周的任意组合。



## 2.语法介绍

crontab 是用来让使用者在固定时间或固定间隔执行程序之用，换句话说，也就是类似使用者的时程表。

1）语法格式为：**crontab [-e [UserName]|-l [UserName]|-r [UserName]|-v [UserName]|File ]**

2）参数：

**    -e [UserName]**：执行文字编辑器来设定时程表

**    -r [UserName]**：删除目前的时程表

**    -l [UserName]**：列出目前的时程表

**    -v [UserName]**：列出用户 cron 作业的状态

**    File**：存放时程表的档案

3）使用方法：除了直接执行文字编辑器来设定时程表外，使用者还可以将用 VI 编辑一个文件 cronfile，将所有设定存放在文件 cronfile 中，编辑完成后，保存退出。在命令行中输入 **crontab cronfile** 命令来设定，这样就将文件 cronfile 提交给 cron 进程，同时，新创建 cronfile 的一个副本已经被放在 /var/spool/cron 目录中，文件名就是用户名。

## 3.时程表格式

crontab 文件的每一行均遵守特定的格式，由空格或 tab 分隔为数个领域，每个领域可以放置单一或多个数值。



其基本格式如下图

![](https://img-blog.csdn.net/20180707224210789?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以：f1 f2 f3 f4 f5 program 为例

1）f1 是表示分钟，f2 表示小时，f3 表示一个月份中的第几日，f4 表示月份，f5 表示一个星期中的第几天，program 表示要执行的程式。

2）当 f1 为 * 时，表示每分钟都要执行 program ；f2 为 * 时，表示每小时都要执行程式，其余类推。

3）当 f1 为 a-b 时，表示从第 a 分钟到第 b 分钟这段时间内要执行；f2 为 a-b 时，表示从第 a 到第 b 小时都要执行，其余类推。

4）当 f1 为 */n 时，表示每 n 分钟个时间间隔执行一次；f2 为 */n 时，表示每 n 小时个时间间隔执行一次，其余类推。

5）当 f1 为 a, b, c,... 时，表示第 a, b, c,... 分钟要执行；f2 为 a, b, c,... 时，表示第 a, b, c...个小时要执行，其余类推。

## **4.使用实例**

1）使用 crontab -e 命令，添加任务计划

注意，第一次启动时，会让你选择编辑的工具，crontab 默认的编辑工具是 nano，如果对 nano 不适应，可以根据需要选择合适的编辑工具。

在这里，选择了 vim 编辑工具。

![](https://img-blog.csdn.net/20180707232625667?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：如果以后对选择的编辑工具不满意，可以执行 select-editor 命令，重新选择编辑工具**

![](https://img-blog.csdn.net/20180708004919605?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2）选择后会进入如下画面，就是添加任务计划的地方，与一般的配置文档相同，以#号开头的都是注释

![](https://img-blog.csdn.net/20180707234956715?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3）添加任务计划：在 /home/test 目录下，每2分钟新建一个以当前时间为名的文件

命令：***/2 * * * * touch /home/shiyanlou/$(date +\%Y\%m\%d\%H\%M\%S)**

![](https://img-blog.csdn.net/20180708005413517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：

**① 在 crontab 文件中，% 有结束命令行、换行、重定向的作用，需要在前面加  \ 转义，否则，% 符号将执行其结束命令行或者换行的作用，并且其后的内容会被做为标准输入发送给前面的命令。**

**② 使用 vim 编辑器时，写入成功后会给出提示，所以退出前一定要记得保存。**

4）任务计划成功添加进 crontab 中后，会给出成功提示

![](https://img-blog.csdn.net/2018070800484753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5）可以通过 crontab -l 命令，查看已添加的任务计划

![](https://img-blog.csdn.net/20180708004828416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



6）确定 cron 守护进程是否成功在后台启动，执行 crontab 中的任务计划

可通过以下两种方法，来判断 cron 守护进程是否运行。

① 通过 **ps aux | grep cron** 命令检测 cron 守护进程当前情况

![](https://img-blog.csdn.net/20180708004756322?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

② 通过 pgrep cron 命令查询 cron 守护进程是否运行

![](https://img-blog.csdn.net/20180708004802953?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

7）可以查看执行任务命令之后在日志中的信息反馈来确定执行情况



命令：**tail -f /var/log/syslog**

![](https://img-blog.csdn.net/20180708010752667?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由上图可以看到，每间隔两分钟，就在 /home/test下创建了一个文件名为当时时间的文件

8）转入 /home/test 目录下，可以看到自动创建的文件

![](https://img-blog.csdn.net/20180708011015517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

9）当不再需要这个任务计划时，通过 **crontab -r** 命令删除，再次查看任务列表，会显示没有任务

![](https://img-blog.csdn.net/20180708011128944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



