# flickr的全局主键生成方案 - 零度的博客专栏 - CSDN博客
2017年09月08日 09:51:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：327

### flickr 的全局主键生成方案原理
        假设我们用户分库有 Shop_1/2/3/4 … 那么全局uid怎样生成？我们一般的做法是用一张索引表 Shop_share.user_index 取其自增主键，insert_id 便是uid。但缺点是，有单点负载的风险。
         flickr提供了一个扩展的更好的方案： 他们把 user_index 抽出一个专门用作生成 uid 的表，例如取名叫 uid_sequence，并拆成若干的字表，自增步长设置为2（机器数目），这两张表可以放在不同的物理机器上。初始值一个结点设为1， 另一个结点初始值为2，这样其中一个表负责生成奇数uid，另一个负责生成偶数uid。

![](http://ww1.sinaimg.cn/large/67a6a651gw1dujgqcx9ncj.jpg)
## uid_sequence 表的设计（如创建64位的自增id）：
```
CREATE TABLE `uid_sequence` (  
  `id` bigint(20) unsigned NOT NULL auto_increment,  
  `stub` char(1) NOT NULL default '',  
  PRIMARY KEY  (`id`),  
  UNIQUE KEY `stub` (`stub`)  
) ENGINE=MyISAM;
```
SELECT * from uid_sequence 输出：
```
+----------------------------+-------+  
| id                                | stub |  
+----------------------------+------+  
| 72157623227190423 |    a |
```
如果我需要一个全局的唯一的64位uid，则执行：
```
REPLACE INTO uid_sequence (stub) VALUES ('a');  
SELECT LAST_INSERT_ID();
```
- 用 REPLACE INTO 代替 INSERT INTO 的好处是避免表行数太大，还要另外定期清理。
- stub 字段要设为唯一索引，这个 sequence 表只有一条纪录，但也可以同时为多张表生成全局主键，例如 user_ship_id。除非你需要表的主键是连续的，那么就另建一个 user_ship_id_sequence 表。
- 经过实际对比测试，使用 MyISAM 比 Innodb 有更高的性能。
这里flickr使用两台数据库作为自增序列生成，通过这两台机器做主备和负载均衡。
```cpp
TicketServer1: 
auto-increment-increment = 2     //自增步长
auto-increment-offset = 1    //初始值
  
TicketServer2:
auto-increment-increment = 2  
auto-increment-offset = 2
```
## MySQL 中 last_insert_id() 的并发问题
      因为是两条SQL语句，所以这两条语句之间会不会有并发问题？答案是不会，因为 last_insert_id() 是 Connection 级别的（数据库级别），是单个连接客户端里执行的insert语句最近一条，客户端之间是不会影响，没必要锁定和事务处理。
