# 【Linux】crontab使用 - zkq_1986的博客 - CSDN博客





2018年01月26日 16:11:16[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：87
个人分类：[Linux](https://blog.csdn.net/zkq_1986/article/category/6407840)









用户所建立的crontab文件中，每一行都代表一项任务，每行的每个字段代表一项设置，它的格式共分为六个字段，前五段是时间设定段，第六段是要执行的命令段，格式如下：

minute hour day month week command

其中：

minute： 表示分钟，可以是从0到59之间的任何整数。

hour：表示小时，可以是从0到23之间的任何整数。

day：表示日期，可以是从1到31之间的任何整数。

month：表示月份，可以是从1到12之间的任何整数。

week：表示星期几，可以是从0到7之间的任何整数，这里的0或7代表星期日。

command：要执行的命令，可以是系统命令，也可以是自己编写的脚本文件。

![](https://images2015.cnblogs.com/blog/513841/201608/513841-20160812102124078-171184924.png)

在以上各个字段中，还可以使用以下特殊字符：

星号（*）：代表所有可能的值，例如month字段如果是星号，则表示在满足其它字段的制约条件后每月都执行该命令操作。

逗号（,）：可以用逗号隔开的值指定一个列表范围，例如，“1,2,5,7,8,9”

中杠（-）：可以用整数之间的中杠表示一个整数范围，例如“2-6”表示“2,3,4,5,6”

正斜线（/）：可以用正斜线指定时间的间隔频率，例如“0-23/2”表示每两小时执行一次。同时正斜线可以和星号一起使用，例如*/10，如果用在minute字段，表示每十分钟执行一次。

二、crond服务

安装crontab：

yum install crontabs

服务操作说明：

/sbin/service crond start //启动服务

/sbin/service crond stop //关闭服务

/sbin/service crond restart //重启服务

/sbin/service crond reload //重新载入配置

/sbin/service crond status //启动服务



查看crontab服务是否已设置为开机启动，执行命令：

ntsysv

加入开机自动启动：

chkconfig –level 35 crond on

三、crontab命令详解

1．命令格式：

crontab [-u user] file

crontab [-u user] [ -e | -l | -r ]




转载自：https://www.cnblogs.com/intval/p/5763929.html



