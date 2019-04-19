# 数据切分-Mysql分区表的建立及性能分析 - 零度的博客专栏 - CSDN博客
2017年03月20日 16:50:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：465
### [MySQL](http://lib.csdn.net/base/14)的安装方法参考
[http://blog.csdn.net/jhq0113/article/details/43812895](http://blog.csdn.net/jhq0113/article/details/43812895)
### Mysql分区表的介绍
定义：
**        表的分区指根据可以设置为任意大小的规则，跨文件系统分配单个表的多个部分。实际上，表的不同部分在不同的位置被存储为单独的表。用户所选择的、实现数据分割的规则被称为分区函数，这在[MySQL](http://lib.csdn.net/base/14)中它可以是模数，或者是简单的匹配一个连续的数值区间或数值列表，或者是一个内部HASH函数，或一个线性HASH函数。**
        使用场景:
** 1****.某张表的数据量非常大，通过索引已经不能很好的解决查询性能的问题**
**       2.表的数据可以按照某种条件进行分类，以致于在查询的时候性能得到很大的提升**
   优点:
**   1）对于那些已经失去保存意义的数据，通常可以通过删除与那些数据有关的分区，很容易地删除那些数据。相反地，在某些情况下，添加新数据的过程又可以通过为那些新数据专门增加一个新的分区，来很方便地实现。**
**       2）一些查询可以得到极大的优化，这主要是借助于满足一个给定WHERE语句的数据可以只保存在一个或多个分区内，这样在查找时就不用查找其他剩余的分区。因为分区可以在创建了分区表后进行修改，所以在第一次配置分区方案时还不曾这么做时，可以重新组织数据，来提高那些常用查询的效率。**
**       3）涉及到例如SUM()和COUNT()这样聚合函数的查询，可以很容易地进行并行处理。这意味着查询可以在每个分区上同时进行，最终结果只需通过总计所有分区得到的结果。**
**       4）通过跨多个磁盘来分散数据查询，来获得更大的查询吞吐量。**
**分类：**
### ![](https://img-blog.csdn.net/20150324160619897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamhxMDExMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)Mysql分区表使用实例
       1.检查你的Mysql是否支持分区
        mysql> SHOW VARIABLES LIKE '%partition%';
        若结果如下，表示你的Mysql支持表分区:
+-----------------------+-------+
       | Variable_name         | Value |
       +-----------------------+-------+
       | have_partition_engine | YES   |
       +-----------------------+-------+
       1 row in set (0.00 sec)
       
               RANGE分区表创建方式：
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- DROPTABLE IF EXISTS `my_orders`;  
- CREATETABLE `my_orders` (  
-   `id` int(10) unsigned NOTNULL AUTO_INCREMENT COMMENT '表主键',  
-   `pid` int(10) unsigned NOTNULL COMMENT '产品ID',  
-   `price` decimal(15,2) NOTNULL COMMENT '单价',  
-   `num` int(11) NOTNULL COMMENT '购买数量',  
-   `uid` int(10) unsigned NOTNULL COMMENT '客户ID',  
-   `atime` datetime NOTNULL COMMENT '下单时间',  
-   `utime` int(10) unsigned NOTNULLDEFAULT 0 COMMENT '修改时间',  
-   `isdel` tinyint(4) NOTNULLDEFAULT'0' COMMENT '软删除标识',  
- PRIMARYKEY (`id`,`atime`)  
- ) ENGINE=InnoDB DEFAULT CHARSET=utf8  
- 
- /*********分区信息**************/  
- PARTITION BY RANGE (YEAR(atime))  
- (  
-    PARTITION p0 VALUES LESS THAN (2016),  
-    PARTITION p1 VALUES LESS THAN (2017),  
-    PARTITION p2 VALUES LESS THAN MAXVALUE  
- );  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
DROP TABLE IF EXISTS `my_orders`;
CREATE TABLE `my_orders` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '表主键',
  `pid` int(10) unsigned NOT NULL COMMENT '产品ID',
  `price` decimal(15,2) NOT NULL COMMENT '单价',
  `num` int(11) NOT NULL COMMENT '购买数量',
  `uid` int(10) unsigned NOT NULL COMMENT '客户ID',
  `atime` datetime NOT NULL COMMENT '下单时间',
  `utime` int(10) unsigned NOT NULL DEFAULT 0 COMMENT '修改时间',
  `isdel` tinyint(4) NOT NULL DEFAULT '0' COMMENT '软删除标识',
  PRIMARY KEY (`id`,`atime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
/*********分区信息**************/
PARTITION BY RANGE (YEAR(atime))
(
   PARTITION p0 VALUES LESS THAN (2016),
   PARTITION p1 VALUES LESS THAN (2017),
   PARTITION p2 VALUES LESS THAN MAXVALUE
);
```
        以上是一个简单的订单表，分区字段是atime,根据RANGE分区，这样当你向该表中插入数据的时候，Mysql会根据YEAR(atime)的值进行分区存储。
        检查分区是否创建成功，执行查询语句：
         EXPLAIN PARTITIONS SELECT * FROM `my_orders`
         若成功，结果如下：
![](https://img-blog.csdn.net/20150324162214992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamhxMDExMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        性能分析：
        1）创建同样表结构，但没有进行分区的表     
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- DROPTABLE IF EXISTS `my_order`;  
- CREATETABLE `my_order` (  
-   `id` int(10) unsigned NOTNULL AUTO_INCREMENT COMMENT '表主键',  
-   `pid` int(10) unsigned NOTNULL COMMENT '产品ID',  
-   `price` decimal(15,2) NOTNULL COMMENT '单价',  
-   `num` int(11) NOTNULL COMMENT '购买数量',  
-   `uid` int(10) unsigned NOTNULL COMMENT '客户ID',  
-   `atime` datetime NOTNULL COMMENT '下单时间',  
-   `utime` int(10) unsigned NOTNULLDEFAULT 0 COMMENT '修改时间',  
-   `isdel` tinyint(4) NOTNULLDEFAULT'0' COMMENT '软删除标识',  
- PRIMARYKEY (`id`,`atime`)  
- ) ENGINE=InnoDB DEFAULT CHARSET=utf8;  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
DROP TABLE IF EXISTS `my_order`;
CREATE TABLE `my_order` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '表主键',
  `pid` int(10) unsigned NOT NULL COMMENT '产品ID',
  `price` decimal(15,2) NOT NULL COMMENT '单价',
  `num` int(11) NOT NULL COMMENT '购买数量',
  `uid` int(10) unsigned NOT NULL COMMENT '客户ID',
  `atime` datetime NOT NULL COMMENT '下单时间',
  `utime` int(10) unsigned NOT NULL DEFAULT 0 COMMENT '修改时间',
  `isdel` tinyint(4) NOT NULL DEFAULT '0' COMMENT '软删除标识',
  PRIMARY KEY (`id`,`atime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```
        2）向两张表中插入相同的数据
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- /**************************向分区表插入数据****************************/  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,CURRENT_TIMESTAMP());  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2016-05-01 00:00:00');  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2017-05-01 00:00:00');  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2018-05-01 00:00:00');  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2015-05-01 00:00:00');  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2016-05-01 00:00:00');  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2017-05-01 00:00:00');  
- INSERTINTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2018-05-01 00:00:00');  
- 
- /**************************向未分区表插入数据****************************/  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,CURRENT_TIMESTAMP());  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2016-05-01 00:00:00');  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2017-05-01 00:00:00');  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2018-05-01 00:00:00');  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2015-05-01 00:00:00');  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2016-05-01 00:00:00');  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2017-05-01 00:00:00');  
- INSERTINTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2018-05-01 00:00:00');  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
/**************************向分区表插入数据****************************/
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,CURRENT_TIMESTAMP());
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2016-05-01 00:00:00');
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2017-05-01 00:00:00');
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2018-05-01 00:00:00');
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2015-05-01 00:00:00');
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2016-05-01 00:00:00');
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2017-05-01 00:00:00');
INSERT INTO my_orders(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2018-05-01 00:00:00');
/**************************向未分区表插入数据****************************/
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,CURRENT_TIMESTAMP());
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2016-05-01 00:00:00');
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2017-05-01 00:00:00');
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89757,'2018-05-01 00:00:00');
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2015-05-01 00:00:00');
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2016-05-01 00:00:00');
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2017-05-01 00:00:00');
INSERT INTO my_order(`pid`,`price`,`num`,`uid`,`atime`) VALUES(1,12.23,1,89756,'2018-05-01 00:00:00');
```
        3）主从复制，大约20万条左右（主从复制的数据和真实环境有差距，但是能体现出表分区查询的性能优劣）
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- /**********************************主从复制大量数据******************************/  
- INSERTINTO `my_orders`(`pid`,`price`,`num`,`uid`,`atime`) SELECT `pid`,`price`,`num`,`uid`,`atime` FROM `my_orders`;  
- INSERTINTO `my_order`(`pid`,`price`,`num`,`uid`,`atime`) SELECT `pid`,`price`,`num`,`uid`,`atime` FROM `my_order`;  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
/**********************************主从复制大量数据******************************/
INSERT INTO `my_orders`(`pid`,`price`,`num`,`uid`,`atime`) SELECT `pid`,`price`,`num`,`uid`,`atime` FROM `my_orders`;
INSERT INTO `my_order`(`pid`,`price`,`num`,`uid`,`atime`) SELECT `pid`,`price`,`num`,`uid`,`atime` FROM `my_order`;
```
       4）查询测试 
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- /***************************查询性能分析**************************************/  
- SELECT * FROM `my_orders` WHERE `uid`=89757 AND `atime`< CURRENT_TIMESTAMP();  
- /****用时0.084s****/  
- 
- SELECT * FROM `my_order` WHERE `uid`=89757 AND `atime`< CURRENT_TIMESTAMP();  
- /****用时0.284s****/  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
/***************************查询性能分析**************************************/
SELECT * FROM `my_orders` WHERE `uid`=89757 AND `atime`< CURRENT_TIMESTAMP();
/****用时0.084s****/
SELECT * FROM `my_order` WHERE `uid`=89757 AND `atime`< CURRENT_TIMESTAMP();
/****用时0.284s****/
```
      通过以上查询可以明显看出进行表分区的查询性能更好，查询所花费的时间更短。 
      分析查询过程：
      EXPLAIN PARTITIONS SELECT * FROM `my_orders` WHERE `uid`=89757 AND `atime`< CURRENT_TIMESTAMP();
![](https://img-blog.csdn.net/20150324163116189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamhxMDExMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      EXPLAIN PARTITIONS SELECT * FROM `my_order` WHERE `uid`=89757 AND `atime`< CURRENT_TIMESTAMP();
![](https://img-blog.csdn.net/20150324163128435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamhxMDExMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       通过以上结果可以看出，my_orders表查询直接经过p0分区，只扫描了49386行，而my_order表没有进行分区，扫描了196983行，这也是性能得到提升的关键所在。
       当然，表的分区并不是分的越多越好，当表的分区太多时找分区又是一个性能的瓶颈了，建议在200个分区以内。
      LIST分区表创建方式：
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- /*****************创建分区表*********************/  
- CREATETABLE `products` (  
- `id`  bigint UNSIGNED NOTNULL AUTO_INCREMENT COMMENT '表主键' ,  
- `name`  varchar(64) CHARACTERSET utf8 COLLATE utf8_general_ci NOTNULL COMMENT '产品名称' ,  
- `metrial`  tinyint UNSIGNED NOTNULL COMMENT '材质' ,  
- `weight`  double UNSIGNED NOTNULLDEFAULT 0 COMMENT '重量' ,  
- `vol`  double UNSIGNED NOTNULLDEFAULT 0 COMMENT '容积' ,  
- `c_id`  tinyint UNSIGNED NOTNULL COMMENT '供货公司ID' ,  
- PRIMARYKEY (`id`,`c_id`)  
- )ENGINE=InnoDB DEFAULT CHARSET=utf8  
- 
- /*********分区信息**************/  
- PARTITION BY LIST(c_id)  
- (  
-     PARTITION pA VALUESIN (1,3,11,13),  
-     PARTITION pB VALUESIN (2,4,12,14),  
-     PARTITION pC VALUESIN (5,7,15,17),  
-     PARTITION pD VALUESIN (6,8,16,18),  
-     PARTITION pE VALUESIN (9,10,19,20)  
- );  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
/*****************创建分区表*********************/
CREATE TABLE `products` (
`id`  bigint UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '表主键' ,
`name`  varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '产品名称' ,
`metrial`  tinyint UNSIGNED NOT NULL COMMENT '材质' ,
`weight`  double UNSIGNED NOT NULL DEFAULT 0 COMMENT '重量' ,
`vol`  double UNSIGNED NOT NULL DEFAULT 0 COMMENT '容积' ,
`c_id`  tinyint UNSIGNED NOT NULL COMMENT '供货公司ID' ,
PRIMARY KEY (`id`,`c_id`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8
/*********分区信息**************/
PARTITION BY LIST(c_id)
(
    PARTITION pA VALUES IN (1,3,11,13),
    PARTITION pB VALUES IN (2,4,12,14),
    PARTITION pC VALUES IN (5,7,15,17),
    PARTITION pD VALUES IN (6,8,16,18),
    PARTITION pE VALUES IN (9,10,19,20)
);
```
       可以看出，LIST分区和RANGE分区很类似，这里就不做性能分析了，和RANGE很类似。 
       HASH分区表的创建方式：
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- /*****************分区表*****************/  
- CREATETABLE `msgs` (  
-   `id` bigint(20) unsigned NOTNULL AUTO_INCREMENT COMMENT '表主键',  
-   `sender` int(10) unsigned NOTNULL COMMENT '发送者ID',  
-   `reciver` int(10) unsigned NOTNULL COMMENT '接收者ID',  
-   `msg_type` tinyint(3) unsigned NOTNULL COMMENT '消息类型',  
-   `msg` varchar(225) NOTNULL COMMENT '消息内容',  
-   `atime` int(10) unsigned NOTNULL COMMENT '发送时间',  
-   `sub_id` tinyint(3) unsigned NOTNULL COMMENT '部门ID',  
- PRIMARYKEY (`id`,`sub_id`)  
- ) ENGINE=InnoDB DEFAULT CHARSET=utf8  
- /*********分区信息**************/  
- PARTITION BY HASH(sub_id)  
- PARTITIONS 10;  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
/*****************分区表*****************/
CREATE TABLE `msgs` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '表主键',
  `sender` int(10) unsigned NOT NULL COMMENT '发送者ID',
  `reciver` int(10) unsigned NOT NULL COMMENT '接收者ID',
  `msg_type` tinyint(3) unsigned NOT NULL COMMENT '消息类型',
  `msg` varchar(225) NOT NULL COMMENT '消息内容',
  `atime` int(10) unsigned NOT NULL COMMENT '发送时间',
  `sub_id` tinyint(3) unsigned NOT NULL COMMENT '部门ID',
  PRIMARY KEY (`id`,`sub_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
/*********分区信息**************/
PARTITION BY HASH(sub_id)
PARTITIONS 10;
```
          以上语句代表，msgs表按照sub_id进行HASH分区，一共分了十个区。
          Key分区和HASH分区很类似，不再介绍，若想了解可以参考Mysql官方文档进行详细了解。
         子分区的创建方式:
**[sql]**[view plain](http://blog.csdn.net/jhq0113/article/details/44593511#)[copy](http://blog.csdn.net/jhq0113/article/details/44593511#)[print](http://blog.csdn.net/jhq0113/article/details/44593511#)[?](http://blog.csdn.net/jhq0113/article/details/44593511#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/626779/fork)
- CREATETABLE `msgss` (  
-   `id` bigint(20) unsigned NOTNULL AUTO_INCREMENT COMMENT '表主键',  
-   `sender` int(10) unsigned NOTNULL COMMENT '发送者ID',  
-   `reciver` int(10) unsigned NOTNULL COMMENT '接收者ID',  
-   `msg_type` tinyint(3) unsigned NOTNULL COMMENT '消息类型',  
-   `msg` varchar(225) NOTNULL COMMENT '消息内容',  
-   `atime` int(10) unsigned NOTNULL COMMENT '发送时间',  
-   `sub_id` tinyint(3) unsigned NOTNULL COMMENT '部门ID',  
- PRIMARYKEY (`id`,`atime`,`sub_id`)  
- ) ENGINE=InnoDB DEFAULT CHARSET=utf8  
- /*********分区信息**************/  
- PARTITION BY RANGE (atime) SUBPARTITION BY HASH (sub_id)   
- (  
-         PARTITION t0 VALUES LESS THAN(1451577600)  
-         (  
-             SUBPARTITION s0,  
-             SUBPARTITION s1,  
-             SUBPARTITION s2,  
-             SUBPARTITION s3,  
-             SUBPARTITION s4,  
-             SUBPARTITION s5  
-         ),  
-         PARTITION t1 VALUES LESS THAN(1483200000)  
-         (  
-             SUBPARTITION s6,  
-             SUBPARTITION s7,  
-             SUBPARTITION s8,  
-             SUBPARTITION s9,  
-             SUBPARTITION s10,  
-             SUBPARTITION s11  
-         ),  
-         PARTITION t2 VALUES LESS THAN MAXVALUE  
-         (  
-             SUBPARTITION s12,  
-             SUBPARTITION s13,  
-             SUBPARTITION s14,  
-             SUBPARTITION s15,  
-             SUBPARTITION s16,  
-             SUBPARTITION s17  
-         )  
- );  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
CREATE TABLE `msgss` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '表主键',
  `sender` int(10) unsigned NOT NULL COMMENT '发送者ID',
  `reciver` int(10) unsigned NOT NULL COMMENT '接收者ID',
  `msg_type` tinyint(3) unsigned NOT NULL COMMENT '消息类型',
  `msg` varchar(225) NOT NULL COMMENT '消息内容',
  `atime` int(10) unsigned NOT NULL COMMENT '发送时间',
  `sub_id` tinyint(3) unsigned NOT NULL COMMENT '部门ID',
  PRIMARY KEY (`id`,`atime`,`sub_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
/*********分区信息**************/
PARTITION BY RANGE (atime) SUBPARTITION BY HASH (sub_id) 
(
		PARTITION t0 VALUES LESS THAN(1451577600)
		(
			SUBPARTITION s0,
			SUBPARTITION s1,
			SUBPARTITION s2,
			SUBPARTITION s3,
			SUBPARTITION s4,
			SUBPARTITION s5
		),
		PARTITION t1 VALUES LESS THAN(1483200000)
		(
			SUBPARTITION s6,
			SUBPARTITION s7,
			SUBPARTITION s8,
			SUBPARTITION s9,
			SUBPARTITION s10,
			SUBPARTITION s11
		),
		PARTITION t2 VALUES LESS THAN MAXVALUE
		(
			SUBPARTITION s12,
			SUBPARTITION s13,
			SUBPARTITION s14,
			SUBPARTITION s15,
			SUBPARTITION s16,
			SUBPARTITION s17
		)
);
```
         检查子分区是否创建成功： 
         EXPLAIN PARTITIONS SELECT * FROM msgss;
         结果如下图：
![](https://img-blog.csdn.net/20150324164154605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamhxMDExMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
