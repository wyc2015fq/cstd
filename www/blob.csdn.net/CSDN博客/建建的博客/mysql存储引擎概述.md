# mysql存储引擎概述 - 建建的博客 - CSDN博客
2017年03月15日 11:03:35[纪建](https://me.csdn.net/u013898698)阅读数：123
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

Mysql存储引擎特点
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160720110201310-1367097575.png)
- 有多种可选方案，可插拔，可修改存储引擎
- 基于表选择使用何种存储引擎
为表选择存储引擎：
```
mysql>create table test （a int） engine = innodb;
```
查看有多少种存储引擎：
```
mysql>show engines
```
更改表的存储引擎：
```
mysql>alter table tbl engine = innodb；
```
主要存储引擎
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160720110246357-1946464948.png)
innodb存储引擎
- 索引组织表
- 支持事务
- 支持行级锁
- 数据块缓存
- 日志持久化（事务日志持久化使得数据恢复迅速）
- 稳定可靠，性能好，线上请尽量使用innodb；
主键：primary key（a）
自增：auto_increment
此时，就算不给a 值或者null ,a依然能赋值正确；
对于使用auto_increment的列必须是索引
mysql中只有innodb支持外键约束；且在创建外键的时候，要求父表必须有对应的索引，子表在创建外键的时候也会自动创建对应的索引；
MyISAM
- 堆表
- 不支持事务
- 只维护索引缓存池，表数据缓存交给操作系统
- 锁粒度较大（写并发性差）
- 数据文件可以直接拷贝，偶尔可能会使用
- 不建议线上业务数据使用
每个myisam在磁盘上存成3个文件：
.frm (存储表定义)
MYD(存储数据)
MYI (存储索引)
频繁地更新删除记录会产生 碎片，需要定期执行：
OPTIMIZE TABLE;
myisam,锁粒度非常大，同时只能一个在写锁；并发性能会差很多；
myisam 表数据可以直接从一个数据库拷贝到另一个数据库；
MEMORY存储引擎
- 数据全内存存放，无法持久化
- 性能较高
- 不支持事务
- 适合偶尔作为临时表使用
- create temporary table tmp（id int ） engine = memory；
创建临时表：
```
mysql> create temporary table tmp（id int ） engine = memory；
```
临时表，只在每一连接中生效；
BLACKHOLE存储引擎
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160720110304982-1813104244.png)
- 数据不作任何存储
- 利用mysql replicate,充当日志服务器
- 在mysql replicate 环境中充当代理主
TokuDB
- 分形树存储结构
- 支持事务
- 行锁
- 压缩效率较高
- 适合大批量insert的场景
Mysql Cluster
- 多主分布式集群
- 数据节点间冗余，高可用
- 支持事务
- 设计上易于扩展
- 面向未来，线上慎用
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160720110319372-985730930.png)
 总结
-   mysql存储引擎可插拔，可修改，基于表；
-   innodb：索引表，支持事务，行锁，数据库缓存，日志持久化，稳定性能好
-   myisam: 堆表，不支持事务，表锁，只维护索引缓冲池表数据缓存交给操作系统；
-   memory：创建零时表
-   blackhole:充当日志服务器，
-   TokuDB:树型存储结构，支持事务，行锁，压缩效率高，适合大批量insert场景；
