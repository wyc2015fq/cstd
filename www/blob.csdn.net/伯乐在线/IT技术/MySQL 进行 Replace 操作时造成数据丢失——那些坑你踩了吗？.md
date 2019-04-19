# MySQL 进行 Replace 操作时造成数据丢失——那些坑你踩了吗？ - 文章 - 伯乐在线
原文出处： [o翡翠谷o](https://my.oschina.net/feicuigu/blog/1512275)
## 一、问题说明
公司开发人员在更新数据时使用了 replace into 语句，由于使用不当导致了数据的大量丢失，到底是如何导致的数据丢失？现分析如下。
## 二、问题分析
### a. REPLACE 原理
REPLACE INTO 原理的官方解释为：
> 
REPLACE works exactly like INSERT, except that if an old row in the table has the same value as a new row for a PRIMARY KEY or a UNIQUE index, the old row is deleted before the new row is inserted.
> 
如果新插入行的主键或唯一键在表中已经存在，则会删除原有记录并插入新行；如果在表中不存在，则直接插入
地址：https://dev.mysql.com/doc/refman/5.6/en/replace.html
### b. 问题现象
丢失数据的表结构如下：


```
CREATE TABLE `active_items` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `ad_id` char(32) NOT NULL DEFAULT '' COMMENT 'XXX',
  `ap_id` tinyint(4) NOT NULL DEFAULT '1' COMMENT 'XXX',
  `price` bigint(20) NOT NULL DEFAULT '0' COMMENT 'xxx',
  `rate` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `cc_price` bigint(20) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `sale_count` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `prom_count` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `c1` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `order_num` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `score` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `audit_time` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `prom_modify_time` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `sales_modify_time` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后修改时间',
  `score_modify_time` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `brand_id` int(11) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `freight_status` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'XXX',
  `flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'XXX',
  PRIMARY KEY (`id`),
  UNIQUE KEY `idx_ad_id` (`ad_id`) USING BTREE,
  ...
) ENGINE=InnoDB AUTO_INCREMENT=2699 DEFAULT CHARSET=utf8 COMMENT='XXXXX'
```
执行的replace语句如下（多条）：


```
REPLACE INTO active_items(ad_id,score) VALUES('XXXXXXX', 1800);
```
通过查询binlog找到执行记录，部分如下：


```
### UPDATE `items`.`active_items`
### WHERE
###   @1=21926 /* LONGINT meta=0 nullable=0 is_null=0 */
###   @2='XXXXXXX' /* STRING(96) meta=65120 nullable=0 is_null=0 */
###   @3=1 /* TINYINT meta=0 nullable=0 is_null=0 */
###   @4=3900 /* LONGINT meta=0 nullable=0 is_null=0 */
###   @5=315 /* INT meta=0 nullable=0 is_null=0 */
###   @6=1228 /* LONGINT meta=0 nullable=0 is_null=0 */
###   @7=19 /* INT meta=0 nullable=0 is_null=0 */
###   @8=0 /* INT meta=0 nullable=0 is_null=0 */
###   @9=-6 (4294967290) /* INT meta=0 nullable=0 is_null=0 */
###   @10=0 /* TINYINT meta=0 nullable=0 is_null=0 */
###   @11=0 /* INT meta=0 nullable=0 is_null=0 */
###   @12=8 /* INT meta=0 nullable=0 is_null=0 */
###   @13=1489997272 /* INT meta=0 nullable=0 is_null=0 */
###   @14=1495765800 /* INT meta=0 nullable=0 is_null=0 */
###   @15=1495728000 /* INT meta=0 nullable=0 is_null=0 */
###   @16=1489996426 /* INT meta=0 nullable=0 is_null=0 */
###   @17=1489997272 /* INT meta=0 nullable=0 is_null=0 */
###   @18=1495728725 /* INT meta=0 nullable=0 is_null=0 */
###   @19=0 /* INT meta=0 nullable=0 is_null=0 */
###   @20=1 /* TINYINT meta=0 nullable=0 is_null=0 */
###   @21=0 /* TINYINT meta=0 nullable=0 is_null=0 */
### SET
###   @1=35313 /* LONGINT meta=0 nullable=0 is_null=0 */
###   @2='XXXXXXX' /* STRING(96) meta=65120 nullable=0 is_null=0 */
###   @3=1 /* TINYINT meta=0 nullable=0 is_null=0 */
###   @4=0 /* LONGINT meta=0 nullable=0 is_null=0 */
###   @5=0 /* INT meta=0 nullable=0 is_null=0 */
###   @6=0 /* LONGINT meta=0 nullable=0 is_null=0 */
###   @7=0 /* INT meta=0 nullable=0 is_null=0 */
###   @8=0 /* INT meta=0 nullable=0 is_null=0 */
###   @9=0 /* INT meta=0 nullable=0 is_null=0 */
###   @10=0 /* TINYINT meta=0 nullable=0 is_null=0 */
###   @11=0 /* INT meta=0 nullable=0 is_null=0 */
###   @12=3150 /* INT meta=0 nullable=0 is_null=0 */
###   @13=0 /* INT meta=0 nullable=0 is_null=0 */
###   @14=0 /* INT meta=0 nullable=0 is_null=0 */
###   @15=0 /* INT meta=0 nullable=0 is_null=0 */
###   @16=0 /* INT meta=0 nullable=0 is_null=0 */
###   @17=0 /* INT meta=0 nullable=0 is_null=0 */
###   @18=0 /* INT meta=0 nullable=0 is_null=0 */
###   @19=0 /* INT meta=0 nullable=0 is_null=0 */
###   @20=0 /* TINYINT meta=0 nullable=0 is_null=0 */
###   @21=0 /* TINYINT meta=0 nullable=0 is_null=0 */
```
- 操作的ad_id已经存在，因此先删除后插入，可以看到除了指定的 ad_id,score，其他字段都变为默认值，导致原有数据丢失（虽然在日志中转为了update）
### c. 对比测试
接下来我进行了如下测试：
![测试对比](http://jbcdn2.b0.upaiyun.com/2017/09/bc1c5405e9f04bdf942d13b7e34928ef.png)
- 左侧使用 `REPLACE` 语句，右侧使用 `DELETE` + `INSERT` 语句，最后结果完全相同
- 原主键id为1的行被删除，新插入行主键id更新为4，**没有指定内容的字段c则插入了默认值**
- 使用 `REPLACE` 更新了一行数据，MySQL提示受影响行数为2行
- 综上所述，说明确实是删除一行，插入一行
## 三、数据恢复
数据丢失或数据错误后，可以有如下几种方式恢复：
- 业务方自己写脚本恢复
- 通过MySQL的binlog查出误操作sql，生成反向sql进行数据恢复（适合sql数据量较小的情况）
- 通过历史备份文件+增量binlog将数据状态恢复到误操作的前一刻
## 四、问题扩展
通过上述分析可以发现，`REPLACE` 会删除旧行并插入新行，但是binlog中是以update形式记录，这样就带来另一个问题：
> 
**从库自增长值小于主库**
### 1. 测试
**a. 主从一致：**


```
主库：
mysql> show create table t\G
*************************** 1. row ***************************
       Table: t
Create Table: CREATE TABLE `t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `a` int(11) DEFAULT '0',
  `b` int(11) DEFAULT '0',
  `c` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`a`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
从库：
mysql> show create table t\G
*************************** 1. row ***************************
       Table: t
Create Table: CREATE TABLE `t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `a` int(11) DEFAULT '0',
  `b` int(11) DEFAULT '0',
  `c` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`a`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
```
**b. 主库REPLACE：**


```
主库：
mysql> replace into t (a,b)values(1,7);
Query OK, 2 rows affected (0.01 sec)
mysql> show create table t\G           
*************************** 1. row ***************************
       Table: t
Create Table: CREATE TABLE `t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `a` int(11) DEFAULT '0',
  `b` int(11) DEFAULT '0',
  `c` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`a`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
从库：
mysql> show create table t\G
*************************** 1. row ***************************
       Table: t
Create Table: CREATE TABLE `t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `a` int(11) DEFAULT '0',
  `b` int(11) DEFAULT '0',
  `c` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`a`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
```
- 注意此时主从两个表的AUTO_INCREMENT值已经不同了
**c. 模拟从升主，在从库进行INSERT：**


```
mysql> insert into t (a,b,c)values(4,4,4);
ERROR 1062 (23000): Duplicate entry '4' for key 'PRIMARY'
mysql> show create table t\G
*************************** 1. row ***************************
       Table: t
Create Table: CREATE TABLE `t` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `a` int(11) DEFAULT '0',
  `b` int(11) DEFAULT '0',
  `c` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `a` (`a`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
mysql> insert into t (a,b,c)values(4,4,4);
Query OK, 1 row affected (0.00 sec)
```
- 从库插入时会报错，主键重复，报错后AUTO_INCREMENT会 +1，因此再次执行就可以成功插入
### 2. 结论
这个问题在平时不会有丝毫影响，但是：
> 
**如果主库平时大量使用 `REPLACE` 语句，造成从库 `AUTO_INCREMENT` 值落后主库太大，当主从发生切换后，再次插入数据时新的主库就会出现大量主键重复报错，导致数据无法插入。**
### 3. 参考文章
http://www.cnblogs.com/monian/archive/2014/10/09/4013784.html
