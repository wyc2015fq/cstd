# Linux下的应用程序开机自启动 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月23日 15:46:46[左直拳](https://me.csdn.net/leftfist)阅读数：1652
Linux下的应用程序开机自启动，目前我知道的有2种：
1、将启动语句写到/etc/rc.local
2、在init.d放置启动脚本
第一种，很简单，直接写进去就行，比如说：
/db/mongodb/265/bin/mongod --config /db/conf/mongodb/mongod.conf
这是一句启动mongodb的命令
第二种：
1、在/etc/init.d/放置启动脚本文件。文件内容是一些脚本，具体看不懂。这些脚本当然可以手写，但不清楚是不是安装的时候系统自己写上去的。就算不是系统自己提供，好像也有地方可以下载
2、脚本文件写好以后，需要赋予执行权限
chmod +x /etc/init.d/redis
3、试一下是否可以启动、关闭
service redis start
service redis stop
4、设为开机启动：
chkconfig redis on

