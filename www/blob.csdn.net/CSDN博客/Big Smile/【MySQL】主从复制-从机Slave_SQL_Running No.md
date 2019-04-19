# 【MySQL】主从复制-从机Slave_SQL_Running: No - Big Smile - CSDN博客
2018年07月31日 16:47:51[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1421
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
**问题现象：**
![](https://img-blog.csdn.net/20180731163517502?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 我们可以明显的发现，我们的sql线程已经挂掉了。
         导致这个问题的原因是因为由于数据的不同步，导致我们在执行主从同步的时候出现了执行不了的sql，导致从机事务回滚了，所以sql的进程就被关闭了，常见的这个问题的出现方式：
- 在从机上进行了写操作（非双机热备情况下）。
- slave机器重启，事务回滚。
- 各种原因导致的数据不同步。
**解决办法1：**
- mysql> stop slave;
- mysql> set GLOBAL SQL_SLAVE_SKIP_COUNTER=1;
- mysql> start slave;
这个解决的是因为事务的原因导致的sql进程停止。
**解决办法2：**
停止从机服务：stop slave;
然后在主机上查看binlog文件的位置：show master status;
+----------------------+--------------+---------------+------------------+
| File                      | Position   | Binlog_Do_DB | Binlog_Ignore_DB |
+----------------------+--------------+---------------+-------------------+
| XXX-bin.000004 | 33622483 |                   |                      | 
+----------------------+---------------+--------------+------------------+
然后重新手动同步：
mysql> change master to 
> master_host='master_ip',
> master_user='user', 
> master_password='pwd', 
> master_port=3306, 
> master_log_file=‘XXX-bin.000004’, 
> master_log_pos=33622483 ;
然后启动从服务：start slave;
**总结**
要不就查看是否和主服务器断开了binlog的同步，要不就看是否是因为事务问题导致的数据不一致。
