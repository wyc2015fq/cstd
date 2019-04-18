# MySQL优化order by导致的 using filesort - z69183787的专栏 - CSDN博客
2018年10月12日 12:30:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：233
# http://dev.mysql.com/doc/refman/5.7/en/order-by-optimization.html
      这段时间mysql 数据库的性能明显降低,iowait达到了30, 响应时间明显变长.  通过show processlist 查看,发现有很多session在处理sort 操作, 跟DBA一起调试优化,增大sort_buffer_size 好象效果也不大, 通过查看监控,也没发现有硬盘排序. 我怀疑是sort导致性能下降,固让开发修改程序, sort由程序来处理. 星期五发布后,今天发现压力固然好了很多.
      因此基本上能确定是sort引起的问题. 今天仔细分析问题,查看mysql的参数时,看到一个叫做max_length_for_sort_data 的参数, 值是1024 仔细查看mysql 的filesort算法时, 发现mysql的filesort有两个方法,MySQL 4.1之前是使用方法A, 之后版本会使用改进的算法B, 但使用方法B的前提是列长度的值小于max_length_for_sort_data, 但我们系统中的列的长度的值会大于1024. 因此也就是说在sort的时候, 是在使用方法A, 而方法A的性能比较差, 也就解释了我们的mysql系统在有sort时,性能差,去掉之后性能马上提高很多的原因.
马上修改max_length_for_sort_data这个值，增大到8096, 果然性能就提高了.
  总结:
  mysql对于排序,使用了两个变量来控制sort_buffer_size和  max_length_for_sort_data, 不象oracle使用SGA控制. 这种方式的缺点是要单独控制,容易出现排序性能问题.
using filesort 一般出现在 使用了 order by 语句当中。
using filesort不一定引起mysql的性能问题。但是如果查询次数非常多，那么每次在mysql中进行排序，还是会有影响的。
这里的优化方式是在order by 的字段建立索引，例如 语句：
SELECT * FROM yw_syjgb  ORDER BY result_date desc LIMIT 0,1000;
 查看执行计划：
+----+-------------+----------+------+---------------+------+---------+------+---------+----------------+
| id | select_type | table | type | possible_keys | key | key_len | ref | rows | Extra |
+----+-------------+----------+------+---------------+------+---------+------+---------+----------------+
| 1 | SIMPLE | yw_syjgb | ALL | NULL | NULL | NULL | NULL | 1312418 | Using filesort |
+----+-------------+----------+------+---------------+------+---------+------+---------+----------------+
则需要在result_date  建立索引：
此时查看执行计划：
+----+-------------+----------+-------+---------------+-------------+---------+------+------+-------+
| id | select_type | table | type | possible_keys | key | key_len | ref | rows | Extra |
+----+-------------+----------+-------+---------------+-------------+---------+------+------+-------+
| 1 | SIMPLE | yw_syjgb | index | NULL | result_date | 6 | NULL | 1000 | NULL |
+----+-------------+----------+-------+---------------+-------------+---------+------+------+-------+
可以看到执行计划中使用索引后没有 Using filesort
需要注意的是：由于 Using filesort是使用算法在 内存中进行排序，MySQL对于排序的记录的大小也是有做限制：max_length_for_sort_data，默认为1024
show variables like '%max_length_for_sort_data%';
+--------------------------+-------+
| Variable_name | Value |
+--------------------------+-------+
| max_length_for_sort_data | 1024 |
+--------------------------+-------+
经过笔者测试，如果排序查询的数据两大于这个默认值的话，还是会使用Using filesort。
总结一句，当排序查询的数据量在默认值的范围内是，在排序的字段上加上索引可以提升MySQL查询的速度。
