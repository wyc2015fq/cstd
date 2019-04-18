# MySQL之GTID简介 - z69183787的专栏 - CSDN博客
2017年04月15日 11:41:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：368
个人分类：[数据库-主从同步](https://blog.csdn.net/z69183787/article/category/6863121)
官网：[http://dev.mysql.com/doc/refman/5.6/en/replication-gtids-concepts.html](http://dev.mysql.com/doc/refman/5.6/en/replication-gtids-concepts.html)
## 简介
MySQL5.6 以后，提供了基于 GTID 多开启多线程同步复制的方案，即每个库有一个单独的(sql thread)，进行同步复制，这将大大改善 MySQL 主从同步的数据延迟问题，此外，用 GTID 避免了在传送 binlog 逻辑上依赖文件名和物理偏移量，能够更好的支持自动容灾切换，传统的方式里，你需要找到 binlog和 POS 点，然后 change master to 指向，而不是很有经验的运维，往往会将其找错，造成主从同步复制报错，在 mysql5.6 里，无须再知道 binlog 和 POS
 点，需要知道 master 的 IP、端口，账号密码即可，因为同步复制是自动的，mysql 通过内部机制 GTID 自动找点同步。
## 概念
GTID（global transaction identifier）：mysql服务提交事务时会创建一个全局标识符，它不仅在主（master）服务器上唯一标识，而且在从（slave）服务上也是唯一标识的。
## 格式
```bash
GTID = source_id:transaction_id
```
- 1
source_id：mysql的server_id； 
transaction_id：事务提交时，会生成一个序列号，第一个提交的事务值为1，第十个提交的事务值为10。
```
//第23个事务被提交时生成的GTID
3E11FA47-71CA-11E1-9E33-C80AA9429562:23
```
- 1
- 2
同一mysql服务上的多个GTID可以缩写：
`3E11FA47-71CA-11E1-9E33-C80AA9429562:1-5`- 1
## 工作原理
- 主服务执行并提交事务时生成GTID，写入binary log文件。
- binary log传输到从服务上后，被存储在relay log中，从服务读取GTID并将值设置到gtid_next系统变量中，gtid_next代表下个事务要执行的GTID。
- 从服务会去读取自己的binary log来判断是否执行过此GTID，如果有记录，则会忽略；如果没有执行过，将GTID写进binary log，执行事务之前，确保其他的session客户端没有执行提交过该GTID事务，保证数据的一致性。
