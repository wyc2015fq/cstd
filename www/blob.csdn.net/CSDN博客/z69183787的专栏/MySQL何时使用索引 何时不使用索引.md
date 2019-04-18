# MySQL何时使用索引 何时不使用索引 - z69183787的专栏 - CSDN博客
2015年07月02日 21:47:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1552
**MySQL何时使用索引**
   对一个键码使用>, >=, =, <, <=, IF NULL和BETWEEN
　　SELECT * FROM table_name WHERE key_part1=1 and key_part2 > 5;
　　SELECT * FROM table_name WHERE key_part1 IS NULL;
　　当使用不以通配符开始的LIKE
　　SELECT * FROM table_name WHERE key_part1 LIKE 'jani%'
　　在进行联结时从另一个表中提取行时
　　SELECT * from t1,t2 where t1.col=t2.key_part
　　找出指定索引的MAX()或MIN()值
　　SELECT MIN(key_part2),MAX(key_part2) FROM table_name where key_part1=10
　　一个键码的前缀使用ORDER BY或GROUP BY
　　SELECT * FROM foo ORDER BY key_part1,key_part2,key_part3
　　在所有用在查询中的列是键码的一部分时间
　　SELECT key_part3 FROM table_name WHERE key_part1=1
**   MySQL何时不使用索引 **
    如果MySQL能估计出它将可能比扫描整张表还要快时，则不使用索引。例如如果key_part1均匀分布在1和100之间，下列查询中使用索引就不是很好：
　　SELECT * FROM table_name where key_part1 > 1 and key_part1 < 90
　　如果使用HEAP表且不用=搜索所有键码部分。
　　在HEAP表上使用ORDER BY。
　　如果不是用键码第一部分
　　SELECT * FROM table_name WHERE key_part2=1
　　如果使用以一个通配符开始的LIKE
　　SELECT * FROM table_name WHERE key_part1 LIKE '%jani%'
　　搜索一个索引而在另一个索引上做ORDER BY
　　SELECT * from table_name WHERE key_part1 = # ORDER BY key2
