# mysql or条件可以使用索引而避免全表 - z69183787的专栏 - CSDN博客
2015年07月02日 21:49:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4088
在某些情况下，or条件可以避免全表扫描的。
## 1 .where 语句里面如果带有or条件, myisam表能用到索引， innodb不行。
1)myisam表：
 CREATE TABLE IF NOT EXISTS `a` (
  `id` int(1) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `aNum` char(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;
mysql> explain select * from a where id=1 or uid =2;
+----+-------------+-------+-------------+---------------+-------------+---------+------+------+---------------------------------------+
| id | select_type | table | type        | possible_keys | key         | key_len | ref  | rows | Extra                                 |
+----+-------------+-------+-------------+---------------+-------------+---------+------+------+---------------------------------------+
|  1 | SIMPLE      | a     | index_merge | PRIMARY,uid   | PRIMARY,uid | 4,4     | NULL |    2 | Using union(PRIMARY,uid); Using where |
+----+-------------+-------+-------------+---------------+-------------+---------+------+------+---------------------------------------+
1 row in set (0.00 sec)
2)innodb表：
CREATE TABLE IF NOT EXISTS `a` (
  `id` int(1) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `aNum` char(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;
mysql>  explain select * from a where id=1 or uid =2;
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
|  1 | SIMPLE      | a     | ALL  | PRIMARY,uid   | NULL | NULL    | NULL |    5 | Using where |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
1 row in set (0.00 sec)
## **2 .必须所有的or条件都必须是独立索引：**
+-------+----------------------------------------------------------------------------------------------------------------------
| Table | Create Table
+-------+----------------------------------------------------------------------------------------------------------------------
| a     | CREATE TABLE `a` (
  `id` int(1) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `aNum` char(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=latin1 |
+-------+----------------------------------------------------------------------------------------------------------------------
1 row in set (0.00 sec)
explain查看：
mysql> explain select * from a where id=1 or uid =2;
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
|  1 | SIMPLE      | a     | ALL  | PRIMARY       | NULL | NULL    | NULL |    5 | Using where |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
1 row in set (0.00 sec)
全表扫描了。
## **3. 用UNION替换OR (适用于索引列)**
       通常情况下, 用UNION替换WHERE子句中的OR将会起到较好的效果. 对索引列使用OR将造成全表扫描. 
注意, 以上规则只针对多个索引列有效. 如果有column没有被索引, 查询效率可能会因为你没有选择OR而降低. 
       在下面的例子中, LOC_ID 和REGION上都建有索引.
       高效: 
- select loc_id , loc_desc , region from location where loc_id = 10   
- union
- select loc_id , loc_desc , region  from location where region = "melbourne"
     低效: 
- select loc_id , loc desc , region from location where loc_id = 10 or region = "melbourne"
如果你坚持要用OR, 那就需要返回记录最少的索引列写在最前面.
## 4. 用in来替换or  
     这是一条简单易记的规则，但是实际的执行效果还须检验，在oracle8i下，两者的执行路径似乎是相同的．　
低效: 
select…. from location where loc_id = 10 or loc_id = 20 or loc_id = 30 
高效 
select… from location where loc_in  in (10,20,30);
