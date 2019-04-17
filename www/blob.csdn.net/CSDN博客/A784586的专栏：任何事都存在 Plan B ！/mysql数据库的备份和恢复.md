# mysql数据库的备份和恢复 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月31日 15:03:47[QuJack](https://me.csdn.net/A784586)阅读数：268标签：[mysql数据库的备份和恢复](https://so.csdn.net/so/search/s.do?q=mysql数据库的备份和恢复&t=blog)
个人分类：[数据库](https://blog.csdn.net/A784586/article/category/6777843)









**mysql数据库的备份和恢复**

写太详细了，估计很浪费时间，对MySQL数据库的备份和恢复做个宏观的总结，想初步了解下的可以花两分钟看下**。**

1.根据是否需要数据库离线

**冷备（cold backup）**：需要关mysql服务，读写请求均不允许状态下进行；

**温备（warm backup）**： 服务在线，但仅支持读请求，不允许写请求；

**热备（hot backup）**：备份的同时，业务不受影响。




2、根据要备份的数据集合的范围

**完全备份**：full backup，备份全部字符集。

**增量备份**: incremental backup 上次完全备份或增量备份以来改变了的数据，不能单独使用，要借助完全备份，备份的频率取决于数据的更新频率。

**差异备份**：differential backup 上次完全备份以来改变了的数据。

建议的恢复策略：完全+增量+二进制日志 || 完全+差异+二进制日志

3、根据备份数据或文件

**物理备份**：直接备份数据文件

优点：备份和恢复操作都比较简单，能够跨mysql的版本，恢复速度快，属于文件系统级别的

建议：不要假设备份一定可用，要测试 mysql>check tables；检测表是否可用

**逻辑备份**: 备份表中的数据和代码

优点：恢复简单、备份的结果为ASCII文件，可以编辑，与存储引擎无关，可以通过网络备份和恢复

缺点：备份或恢复都需要mysql服务器进程参与，备份结果占据更多的空间，浮点数可能会丢失精度，还原之后，索引需要重建




4.需要备份的内容：

1、 数据；

2、配置文件；

3、代码：存储过程、存储函数、触发器

4、os相关的配置文件

5、复制相关的配置

6、二进制日志




5.备份与恢复:

**5.1 利用select into outfile实现****数据****的备份与还原**

mysql> use testdb;     //打开testdb库 

mysql> select * from students;  查看students的属性 

mysql>**select*** from students where Age
 > 30**into outfile** ‘/tmp/stud.txt' ;   //将年龄大于三十的同学的信息备份出来 

**恢复：使用load data infile 恢复**

[root@www ~]# cd /tmp

[root@www tmp]# cat stud.txt

mysql>**load data infile**
 '/tmp/stud.txt'**into table** students;




6.逻辑备份和恢复：

6.1 备份为**sql文件**：

shell>mysqldump -uroot -p --all-database > all.sql

shell>mysqldump -uroot -p database01 > database01.sql

shell>mysqldump -uroot -p database01 tablse01 > table01.sql

shell>mysqldump -uroot -p database01 tablse01 tablse02 > table01_tablse02.sql

6.2 备份为文本文件：所有表逗号分隔的**文本**

shell>**mysqldump**-uroot
 -T/tmp database01 txt01--fields-terminated-by ','

shell>more txt01.txt




7.备份和恢复流程：

7.1 上午九点备份数据库：

shell > mysqldump -uroot -p -l -F database01 > database001.dmp

-l表示给所有表加锁，-F表示生成一个新的日志文件；

7.2 上午九点半备份完毕，插入数据：

shell>insert into tablse01 values(5,'z5');

shell>insert into tablse01 values(6,'z6');

7.3 上午10点出现故障，需要恢复

shell>mysql -uroot -p database01 < database001.dmp

shell>**mysqlbinlog**localhost-bin.000015
 | mysql -uroot -p database01




8.基于时间点的恢复：

10点误删除，发生故障,**恢复到之前**：

shell>mysqlbinlog --stop-date="2017-03-31 9:59:59" /var/log/mysql/bin.123456 | mysql -uroot -pmypwd

**跳过故障**继续执行后面的语句：

shell>mysqlbinlog --start-date="2017-03-31 10:01:00" /var/log/mysql/bin.123456 | mysql -uroot -pmypwd

在同一个时间点可能执行多条语句，有时候根据位置恢复更好。




9.基于位置的恢复：

shell>mysqlbinlog --start-date="2017-03-31 9:55:00" --stop-date="2017-03-31 10:05:00" /var/log/mysql/bin.123456 > /tmp/mysql_restore.sql

从上面的文件里面找出错误语句前后的**位置号，**假设是3683212----3683215

shell>mysqlbinlog --stop-position="3683212" /var/log/mysql/bin.123456 | mysql -uroot -pmypwd

**跳过故障**继续执行后面的语句：

shell>mysqlbinlog --start-position="3683215" /var/log/mysql/bin.123456 | mysql -uroot -pmypwd







10.MyISAM存储引擎的热备份

10.1 使用mysqlhotcopy 工具

shell>mysqlhotcopydb_name
 [/path/to/new_directory]

10.2手工锁表 copy

mysql>flush tables for read;




**11.InnoDB存储引擎的热备份**

编辑启动配置文件my.cnf和用于配置的文件backup-my.cnf

shell>**ibbackup**/home/peppka/mycnf
 /home/pekka/backup-my.cnf

日志重做：

shell>**ibbackup****--apply-log /home/pekka/backup-my.cnf**

重启数据库服务：

shell>./bin/mysql_saft --defaults-file=/home/pekka/backup-my.cnf &

利用binlog日志将备份点和故障点之间的剩余数据恢复：

mysqlbinlog binlog-file | mysql -uroot -p***




12.表的导入和导出：

12.1

mysql>**select*** from students where Age
 > 30**into outfile** ‘/tmp/stud.txt'

mysql>**load data infile**
 '/tmp/stud.txt'**into table** students;




12.2

shell>mysqldump-uroot
 -p database01 tablse01 > table01.sql




shell > mysqldump -uroot -p -l -F database01 > database001.dmp

shell>mysql -uroot -p database01 < database001.dmp

shell>mysqlimport-uroot
 database01 /tmp/emp.txt --fields-terminated-by=',' --fields-enclosed-by ' " '

--fields-terminated-by 字段分割符号；

--fields-enclosed-by 字段引用符号



