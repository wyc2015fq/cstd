# mysql中kill掉所有锁表的进程 - weixin_33985507的博客 - CSDN博客
2017年10月21日 18:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
很多时候由于异常或程序错误会导致个别进程占用大量系统资源，需要结束这些进程，通常可以使用以下命令Kill进程: mysql中kill掉所有锁表的进程
转载
作者 : jesse
博客 :[http://hi.baidu.com/leechl](https://link.jianshu.com?t=http://hi.baidu.com/leechl)
3点钟刚睡下, 4点多, 同事打电话告诉我用户数据库挂掉了. 我起床看一下进程列表.
mysql>showprocesslist;
出来哗啦啦好几屏幕的, 没有一千也有几百条, 查询语句把表锁住了, 赶紧找出第一个Locked的thread_id, 在mysql的shell里面执行.
mysql>killthread_id;
kill掉第一个锁表的进程, 依然没有改善. 既然不改善, 咱们就想办法将所有锁表的进程kill掉吧, 简单的脚本如下.
#!/bin/bash
mysql-uroot-e"show processlist"|grep-i"Locked">>locked_log.txt
forlinein`cat locked_log.txt | awk '{print$1}'`
do
echo"kill$line;">>kill_thread_id.sql
done
现在kill_thread_id.sql的内容像这个样子
kill66402982;
kill66402983;
kill66402986;
kill66402991;
.....
好了, 我们在mysql的shell中执行, 就可以把所有锁表的进程杀死了.
mysql>sourcekill_thread_id.sql
当然了, 也可以一行搞定
foridin`mysqladmin processlist | grep -i locked | awk '{print$1}'`
do
mysqladminkill${id}
done
