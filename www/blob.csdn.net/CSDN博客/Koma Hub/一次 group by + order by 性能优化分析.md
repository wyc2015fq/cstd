# 一次 group by + order by 性能优化分析 - Koma Hub - CSDN博客
2019年03月21日 21:06:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：45
个人分类：[MySQL](https://blog.csdn.net/Rong_Toa/article/category/7349084)
> 
原文：我的个人博客 https://mengkang.net/1302.html
**Table of Contents**
[背景](#%E8%83%8C%E6%99%AF)
[准备工作](#%E5%87%86%E5%A4%87%E5%B7%A5%E4%BD%9C)
[开启慢日志](#%E5%BC%80%E5%90%AF%E6%85%A2%E6%97%A5%E5%BF%97)
[性能分析](#%E6%80%A7%E8%83%BD%E5%88%86%E6%9E%90)
[发现问题](#%E5%8F%91%E7%8E%B0%E9%97%AE%E9%A2%98)
[索引示例](#%E7%B4%A2%E5%BC%95%E7%A4%BA%E4%BE%8B)
[查看 optimizer trace 信息](#%E6%9F%A5%E7%9C%8B%20optimizer%20trace%20%E4%BF%A1%E6%81%AF)
[分析临时表字段](#%E5%88%86%E6%9E%90%E4%B8%B4%E6%97%B6%E8%A1%A8%E5%AD%97%E6%AE%B5)
[总结执行流程](#%E6%80%BB%E7%BB%93%E6%89%A7%E8%A1%8C%E6%B5%81%E7%A8%8B)
[优化](#%E4%BC%98%E5%8C%96)
[索引示例](#%E7%B4%A2%E5%BC%95%E7%A4%BA%E4%BE%8B)
[group by 不需要临时表的情况](#group%20by%20%E4%B8%8D%E9%9C%80%E8%A6%81%E4%B8%B4%E6%97%B6%E8%A1%A8%E7%9A%84%E6%83%85%E5%86%B5)
[查看 optimizer trace 信息](#%E6%9F%A5%E7%9C%8B%20optimizer%20trace%20%E4%BF%A1%E6%81%AF)
[该方案可行性](#%E8%AF%A5%E6%96%B9%E6%A1%88%E5%8F%AF%E8%A1%8C%E6%80%A7)
[总结](#%E6%80%BB%E7%BB%93)
[问题与困惑](#%E9%97%AE%E9%A2%98%E4%B8%8E%E5%9B%B0%E6%83%91)
[附录](#%E9%99%84%E5%BD%95)
最近通过一个日志表做排行的时候发现特别卡，最后问题得到了解决，梳理一些索引和 MySQL 执行过程的经验，但是最后还是有 **5 个谜题没解开**，希望大家帮忙解答下。
主要包含如下知识点：
- 
用数据说话证明慢日志的扫描行数到底是如何统计出来的
- 
从 group by 执行原理找出优化方案
- 
排序的实现细节
- 
gdb 源码调试
# 背景
需要分别统计本月、本周被访问的文章的 TOP10。日志表如下：
> - 
`CREATE TABLE `article_rank` (`
- 
``id` int(11) unsigned NOT NULL AUTO_INCREMENT,`
- 
``aid` int(11) unsigned NOT NULL,`
- 
``pv` int(11) unsigned NOT NULL DEFAULT '1',`
- 
``day` int(11) NOT NULL COMMENT '日期 例如 20171016',`
- 
`PRIMARY KEY (`id`),`
- 
`KEY `idx_day_aid_pv` (`day`,`aid`,`pv`),`
- 
`KEY `idx_aid_day_pv` (`aid`,`day`,`pv`)`
- 
`) ENGINE=InnoDB DEFAULT CHARSET=utf8`
## 准备工作
为了能够清晰的验证自己的一些猜想，在虚拟机里安装了一个 debug 版的 mysql，然后开启了慢日志收集，用于统计扫描行数。
安装
- 
下载源码
- 
编译安装
- 
创建 mysql 用户
- 
初始化数据库
- 
初始化 mysql 配置文件
- 
修改密码
> 
如果你兴趣，具体可以参考我的博客，一步步安装 https://mengkang.net/1335.html。
## 开启慢日志
编辑配置文件，在 `[mysqld]` 块下添加：
> - 
`slow_query_log=1`
- 
`slow_query_log_file=xxx`
- 
`long_query_time=0`
- 
`log_queries_not_using_indexes=1`
# 性能分析
## 发现问题
假如我需要查询 `2018-12-20` ~ `2018-12-24` 这 5 天浏览量最大的 10 篇文章的 sql 如下，首先使用 `explain` 看下分析结果：
> - 
`mysql> explain select aid,sum(pv) as num from article_rank where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+--------+----------+-----------------------------------------------------------+`
- 
`| id | select_type | table | partitions | type | possible_keys | key | key_len | ref | rows | filtered | Extra |`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+--------+----------+-----------------------------------------------------------+`
- 
`| 1 | SIMPLE | article_rank | NULL | range | idx_day_aid_pv,idx_aid_day_pv | idx_day_aid_pv | 4 | NULL | 404607 | 100.00 | Using where; Using index; Using temporary; Using filesort |`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+--------+----------+-----------------------------------------------------------+`
系统默认会走的索引是 `idx_day_aid_pv`，根据 `Extra` 信息我们可以看到，使用 `idx_day_aid_pv` 索引的时候，会走覆盖索引，但是会使用临时表，会有排序。
我们查看下慢日志里的记录信息：
> - 
`# Time: 2019-03-17T03:02:27.984091Z`
- 
`# User@Host: root[root] @ localhost [] Id: 6`
- 
`# Query_time: 56.959484 Lock_time: 0.000195 Rows_sent: 10 Rows_examined: 1337315`
- 
`SET timestamp=1552791747;`
- 
`select aid,sum(pv) as num from article_rank where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
为什么扫描行数是 1337315
我们查询两个数据，一个是满足条件的行数，一个是 `groupby` 统计之后的行数。
> - 
`mysql> select count(*) from article_rank where day>=20181220 and day<=20181224;`
- 
`+----------+`
- 
`| count(*) |`
- 
`+----------+`
- 
`| 785102 |`
- 
`+----------+`
- 
- 
`mysql> select count(distinct aid) from article_rank where day>=20181220 and day<=20181224;`
- 
`+---------------------+`
- 
`| count(distinct aid) |`
- 
`+---------------------+`
- 
`| 552203 |`
- 
`+---------------------+`
发现：满足条件的总行数（785102） + `groupby` 之后的总行数（552203）+ `limit` 的值 = 慢日志里统计的 `Rows_examined`。
要解答这个问题，就必须搞清楚上面这个 sql 到底分别都是如何运行的。
执行流程分析
## **索引示例**
为了便于理解，我按照索引的规则先模拟 `idx_day_aid_pv` 索引的一小部分数据：
> - 
`day | aid | pv | id`
- 
`-------- | -------- | ------- | ------- `
- 
`20181220 | 1 | 23 | 1234 `
- 
`20181220 | 3 | 2 | 1231`
- 
`20181220 | 4 | 1 | 1212 `
- 
`20181220 | 7 | 2 | 1221 `
- 
`20181221 | 1 | 5 | 1257`
- 
`20181221 | 10 | 1 | 1251 `
- 
`20181221 | 11 | 8 | 1258 `
因为索引 `idx_day_aid_pv` 最左列是 `day`，所以当我们需要查找 `20181220` ~ `20181224` 之间的文章的 pv 总和的时候，我们需要遍历 `20181220` ~ `20181224` 这段数据的索引。
## **查看 optimizer trace 信息**
> - 
`# 开启 optimizer_trace`
- 
`set optimizer_trace='enabled=on';`
- 
`# 执行 sql `
- 
`select aid,sum(pv) as num from article_rank where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
- 
`# 查看 trace 信息`
- 
`select trace from `information_schema`.`optimizer_trace`\G;`
摘取里面最后的执行结果如下：
> - 
`{`
- 
`"join_execution": {`
- 
`"select#": 1,`
- 
`"steps": [`
- 
`{`
- 
`"creating_tmp_table": {`
- 
`"tmp_table_info": {`
- 
`"table": "intermediate_tmp_table",`
- 
`"row_length": 20,`
- 
`"key_length": 4,`
- 
`"unique_constraint": false,`
- 
`"location": "memory (heap)",`
- 
`"row_limit_estimate": 838860`
- 
`}`
- 
`}`
- 
`},`
- 
`{`
- 
`"converting_tmp_table_to_ondisk": {`
- 
`"cause": "memory_table_size_exceeded",`
- 
`"tmp_table_info": {`
- 
`"table": "intermediate_tmp_table",`
- 
`"row_length": 20,`
- 
`"key_length": 4,`
- 
`"unique_constraint": false,`
- 
`"location": "disk (InnoDB)",`
- 
`"record_format": "fixed"`
- 
`}`
- 
`}`
- 
`},`
- 
`{`
- 
`"filesort_information": [`
- 
`{`
- 
`"direction": "desc",`
- 
`"table": "intermediate_tmp_table",`
- 
`"field": "num"`
- 
`}`
- 
`],`
- 
`"filesort_priority_queue_optimization": {`
- 
`"limit": 10,`
- 
`"rows_estimate": 1057,`
- 
`"row_size": 36,`
- 
`"memory_available": 262144,`
- 
`"chosen": true`
- 
`},`
- 
`"filesort_execution": [`
- 
`],`
- 
`"filesort_summary": {`
- 
`"rows": 11,`
- 
`"examined_rows": 552203,`
- 
`"number_of_tmp_files": 0,`
- 
`"sort_buffer_size": 488,`
- 
`"sort_mode": "<sort_key, additional_fields>"`
- 
`}`
- 
`}`
- 
`]`
- 
`}`
- 
`}`
## **分析临时表字段**
> 
mysql gdb 调试更多细节：https://mengkang.net/1336.html。
通过 `gdb` 调试确认临时表上的字段是 `aid` 和 `num`：
> - 
`Breakpoint 1, trace_tmp_table (trace=0x7eff94003088, table=0x7eff94937200) at /root/newdb/mysql-server/sql/sql_tmp_table.cc:2306`
- 
`warning: Source file is more recent than executable.`
- 
`2306 trace_tmp.add("row_length",table->s->reclength).`
- 
`(gdb) p table->s->reclength`
- 
`$1 = 20`
- 
`(gdb) p table->s->fields`
- 
`$2 = 2`
- 
`(gdb) p (*(table->field+0))->field_name`
- 
`$3 = 0x7eff94010b0c "aid"`
- 
`(gdb) p (*(table->field+1))->field_name`
- 
`$4 = 0x7eff94007518 "num"`
- 
`(gdb) p (*(table->field+0))->row_pack_length()`
- 
`$5 = 4`
- 
`(gdb) p (*(table->field+1))->row_pack_length()`
- 
`$6 = 15`
- 
`(gdb) p (*(table->field+0))->type()`
- 
`$7 = MYSQL_TYPE_LONG`
- 
`(gdb) p (*(table->field+1))->type()`
- 
`$8 = MYSQL_TYPE_NEWDECIMAL`
- 
`(gdb)`
通过上面的打印，确认了字段类型，一个 `aid` 是 `MYSQL_TYPE_LONG`，占 4 字节， `num` 是 `MYSQL_TYPE_NEWDECIMAL`，占 15 字节。
> 
The SUM() and AVG() functions return a DECIMAL value for exact-value arguments (integer or DECIMAL), and a DOUBLE value for approximate-value arguments (FLOAT or DOUBLE). (Before MySQL 5.0.3, SUM() and AVG() return DOUBLE for all numeric arguments.)
但是通过我们上面打印信息可以看到两个字段的长度加起来是 19，而 `optimizer_trace` 里的 `tmp_table_info.reclength` 是 20。通过其他实验也发现 `table->s->reclength` 的长度就是 `table->field` 数组里面所有字段的字段长度和再加 1。
## **总结执行流程**
- 
尝试在堆上使用 `memory` 的内存临时表来存放 `groupby` 的数据，发现内存不够；
- 
创建一张临时表，临时表上有两个字段， `aid` 和 `num` 字段（ `sum(pv)asnum`）；
- 
从索引 `idx_day_aid_pv` 中取出1行，插入临时表。插入规则是如果 `aid` 不存在则直接插入，如果存在，则把 `pv`的值累加在 `num` 上；
- 
循环遍历索引 `idx_day_aid_pv` 上 `20181220` ~ `20181224` 之间的所有行，执行步骤 3；
- 
对临时表根据 `num` 的值做优先队列排序；
- 
取出最后留在堆（优先队列的堆）里面的 10 行数据，作为结果集直接返回，不需要再回表；
补充说明优先队列排序执行步骤分析：
- 
在临时表（未排序）中取出前 10 行，把其中的 `num` 和 `aid` 作为 10 个元素构成一个小顶堆，也就是最小的 num 在堆顶。
- 
取下一行，根据 num 的值和堆顶值作比较，如果该字大于堆顶的值，则替换掉。然后将新的堆做堆排序。
- 
重复步骤 2 直到第 552203 行比较完成。
# 优化
方案1 使用 idxaidday_pv 索引
> - 
`# Query_time: 4.406927 Lock_time: 0.000200 Rows_sent: 10 Rows_examined: 1337315`
- 
`SET timestamp=1552791804;`
- 
`select aid,sum(pv) as num from article_rank force index(idx_aid_day_pv) where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
扫描行数都是 `1337315`，为什么执行消耗的时间上快了 `12` 倍呢？
## **索引示例**
为了便于理解，同样我也按照索引的规则先模拟 `idx_aid_day_pv` 索引的一小部分数据：
> - 
`aid | day | pv | id`
- 
`-------- | -------- | ------- | ------- `
- 
`1 | 20181220 | 23 | 1234`
- 
`1 | 20181221 | 5 | 1257`
- 
`3 | 20181220 | 2 | 1231`
- 
`3 | 20181222 | 22 | 1331`
- 
`3 | 20181224 | 13 | 1431`
- 
`4 | 20181220 | 1 | 1212 `
- 
`7 | 20181220 | 2 | 1221 `
- 
`10 | 20181221 | 1 | 1251 `
- 
`11 | 20181221 | 8 | 1258 `
## **group by 不需要临时表的情况**
为什么性能上比 SQL1 高了，很多呢，原因之一是 `idx_aid_day_pv` 索引上 `aid` 是确定有序的，那么执行 `groupby` 的时候，则不会创建临时表，排序的时候才需要临时表。如果印证这一点呢，我们通过下面的执行计划就能看到。
使用 `idx_day_aid_pv` 索引的效果：
> - 
`mysql> explain select aid,sum(pv) as num from article_rank force index(idx_day_aid_pv) where day>=20181220 and day<=20181224 group by aid order by null limit 10;`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+--------+----------+-------------------------------------------+`
- 
`| id | select_type | table | partitions | type | possible_keys | key | key_len | ref | rows | filtered | Extra |`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+--------+----------+-------------------------------------------+`
- 
`| 1 | SIMPLE | article_rank | NULL | range | idx_day_aid_pv,idx_aid_day_pv | idx_day_aid_pv | 4 | NULL | 404607 | 100.00 | Using where; Using index; Using temporary |`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+--------+----------+-------------------------------------------+`
注意我上面使用了 `orderbynull` 表示强制对 `groupby` 的结果不做排序。如果不加 `orderbynull`，上面的 sql 则会出现 `Usingfilesort`。
使用 `idx_aid_day_pv` 索引的效果：
> - 
`mysql> explain select aid,sum(pv) as num from article_rank force index(idx_aid_day_pv) where day>=20181220 and day<=20181224 group by aid order by null limit 10;`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+------+----------+--------------------------+`
- 
`| id | select_type | table | partitions | type | possible_keys | key | key_len | ref | rows | filtered | Extra |`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+------+----------+--------------------------+`
- 
`| 1 | SIMPLE | article_rank | NULL | index | idx_day_aid_pv,idx_aid_day_pv | idx_aid_day_pv | 12 | NULL | 10 | 11.11 | Using where; Using index |`
- 
`+----+-------------+--------------+------------+-------+-------------------------------+----------------+---------+------+------+----------+--------------------------+`
## **查看 optimizer trace 信息**
> - 
`# 开启optimizer_trace`
- 
`set optimizer_trace='enabled=on';`
- 
`# 执行 sql `
- 
`select aid,sum(pv) as num from article_rank force index(idx_aid_day_pv) where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
- 
`# 查看 trace 信息`
- 
`select trace from `information_schema`.`optimizer_trace`\G;`
摘取里面最后的执行结果如下：
> - 
`{`
- 
`"join_execution": {`
- 
`"select#": 1,`
- 
`"steps": [`
- 
`{`
- 
`"creating_tmp_table": {`
- 
`"tmp_table_info": {`
- 
`"table": "intermediate_tmp_table",`
- 
`"row_length": 20,`
- 
`"key_length": 0,`
- 
`"unique_constraint": false,`
- 
`"location": "memory (heap)",`
- 
`"row_limit_estimate": 838860`
- 
`}`
- 
`}`
- 
`},`
- 
`{`
- 
`"filesort_information": [`
- 
`{`
- 
`"direction": "desc",`
- 
`"table": "intermediate_tmp_table",`
- 
`"field": "num"`
- 
`}`
- 
`],`
- 
`"filesort_priority_queue_optimization": {`
- 
`"limit": 10,`
- 
`"rows_estimate": 552213,`
- 
`"row_size": 24,`
- 
`"memory_available": 262144,`
- 
`"chosen": true`
- 
`},`
- 
`"filesort_execution": [`
- 
`],`
- 
`"filesort_summary": {`
- 
`"rows": 11,`
- 
`"examined_rows": 552203,`
- 
`"number_of_tmp_files": 0,`
- 
`"sort_buffer_size": 352,`
- 
`"sort_mode": "<sort_key, rowid>"`
- 
`}`
- 
`}`
- 
`]`
- 
`}`
- 
`}`
**执行流程如下**
- 
创建一张临时表，临时表上有两个字段， `aid` 和 `num` 字段（ `sum(pv)asnum`）；
- 
读取索引 `idx_aid_day_pv` 中的一行，然后查看是否满足条件，如果 `day` 字段不在条件范围内（ `20181220`~ `20181224`之间），则读取下一行；如果 `day` 字段在条件范围内，则把 `pv` 值累加（不是在临时表中操作）；
- 
读取索引 `idx_aid_day_pv` 中的下一行，如果 `aid` 与步骤1中一致且满足条件，则 `pv` 值累加（不是在临时表中操作）。如果 `aid` 与步骤1中不一致，则把之前的结果集写入临时表；
- 
循环执行步骤 2、3，直到扫描完整个 `idx_aid_day_pv` 索引；
- 
对临时表根据 `num` 的值做优先队列排序；
- 
根据查询到的前 10 条的 `rowid` 回表（临时表）返回结果集。
补充说明优先队列排序执行步骤分析：
- 
在临时表（未排序）中取出前 10 行，把其中的 `num` 和 `rowid` 作为 10 个元素构成一个小顶堆，也就是最小的 num 在堆顶。
- 
取下一行，根据 num 的值和堆顶值作比较，如果该字大于堆顶的值，则替换掉。然后将新的堆做堆排序。
- 
重复步骤 2 直到第 552203 行比较完成。
## **该方案可行性**
实验发现，当我增加一行 `20181219` 的数据时，虽然这行记录不满足我们的需求，但是扫描索引的也会读取这行。因为我做这个实验，只弄了 `20181220` ~ `20181224` 5 天的数据，所以需要扫描的行数正好是全表数据行数。
那么如果该表的数据存储的不是5天的数据，而是 10 天的数据呢，更或者是 365 天的数据呢？这个方案是否还可行呢？先模拟 10 天的数据，在现有时间基础上往后加 5 天，行数与现在一样 `785102` 行。
> - 
`drop procedure if exists idata;`
- 
`delimiter ;;`
- 
`create procedure idata()`
- 
`begin`
- 
`declare i int;`
- 
`declare aid int;`
- 
`declare pv int;`
- 
`declare post_day int;`
- 
`set i=1;`
- 
`while(i<=785102)do`
- 
`set aid = round(rand()*500000);`
- 
`set pv = round(rand()*100);`
- 
`set post_day = 20181225 + i%5;`
- 
`insert into article_rank (`aid`,`pv`,`day`) values(aid, pv, post_day);`
- 
`set i=i+1;`
- 
`end while;`
- 
`end;;`
- 
`delimiter ;`
- 
`call idata();`
> - 
`# Query_time: 9.151270 Lock_time: 0.000508 Rows_sent: 10 Rows_examined: 2122417`
- 
`SET timestamp=1552889936;`
- 
`select aid,sum(pv) as num from article_rank force index(idx_aid_day_pv) where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
这里扫描行数 `2122417` 是因为扫描索引的时候需要遍历整个索引，整个索引的行数就是全表行数，因为我刚刚又插入了 `785102` 行。
当我数据量翻倍之后，这里查询时间明显已经翻倍。所以这个优化方式不稳定。
方案2 扩充临时表空间上限大小
默认的临时表空间大小是 16 MB。
> - 
`mysql> show global variables like '%table_size';`
- 
`+---------------------+----------+`
- 
`| Variable_name | Value |`
- 
`+---------------------+----------+`
- 
`| max_heap_table_size | 16777216 |`
- 
`| tmp_table_size | 16777216 |`
- 
`+---------------------+----------+`
> 
https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html#sysvarmaxheaptablesize https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html#sysvartmptable_size
maxheaptablesize This variable sets the maximum size to which user-created MEMORY tables are permitted to grow. The value of the variable is used to calculate MEMORY table MAXROWS values. Setting this variable has no effect on any existing MEMORY table, unless the table is re-created with a statement such as CREATE TABLE or altered with ALTER TABLE or TRUNCATE TABLE. A server restart also sets the maximum size of existing MEMORY tables to the global maxheaptable_size value.
tmptablesize The maximum size of internal in-memory temporary tables. This variable does not apply to user-created MEMORY tables. The actual limit is determined from whichever of the values of tmptablesize and maxheaptablesize is smaller. If an in-memory temporary table exceeds the limit, MySQL automatically converts it to an on-disk temporary table. The internaltmpdiskstorage_engine option defines the storage engine used for on-disk temporary tables.
也就是说这里临时表的限制是 16 MB， `max_heap_table_size` 大小也受 `tmp_table_size` 大小的限制。
所以我们这里调整为 32 MB，然后执行原始的 SQL。
> - 
`set tmp_table_size=33554432;`
- 
`set max_heap_table_size=33554432;`
> - 
`# Query_time: 5.910553 Lock_time: 0.000210 Rows_sent: 10 Rows_examined: 1337315`
- 
`SET timestamp=1552803869;`
- 
`select aid,sum(pv) as num from article_rank where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
方案3 使用 SQLBIGRESULT 优化
告诉优化器，查询结果比较多，临时表直接走磁盘存储。
> - 
`# Query_time: 6.144315 Lock_time: 0.000183 Rows_sent: 10 Rows_examined: 2122417`
- 
`SET timestamp=1552802804;`
- 
`select SQL_BIG_RESULT aid,sum(pv) as num from article_rank where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
扫描行数是：2 X `满足条件的总行数（785102）` + `groupby` 之后的总行数（552203）+ `limit` 的值。
**顺便值得一提的是：** 当我把数据量翻倍之后，使用该方式，查询时间基本没变。因为扫描的行数还是不变的。实际测试耗时 `6.197484`。
# 总结
方案 1 优化效果不稳定，当总表数据量与查询范围的总数相同时，且不超出内存临时表大小限制时，性能达到最佳。当查询数据量占据总表数据量越大，优化效果越不明显；
方案 2 需要调整临时表内存的大小，可行；不过当数据库超过 32 MB 时，如果使用该方式，还需要继续提升临时表大小；
方案 3 直接声明使用磁盘来放临时表，虽然扫描行数多了一次符合条件的总行数的扫描。但是整体响应时间比方案 2 就慢了 `0.1` 秒。因为我们这里数据量比较，我觉得这个时间差还能接受。
所以最后对比，选择方案 3 比较合适。
## 问题与困惑
> - 
`# SQL1`
- 
`select aid,sum(pv) as num from article_rank where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
- 
`# SQL2`
- 
`select aid,sum(pv) as num from article_rank force index(idx_aid_day_pv) where day>=20181220 and day<=20181224 group by aid order by num desc limit 10;`
问题：
- 
SQL1 执行过程中，使用的是全字段排序最后不需要回表为什么总扫描行数还要加上 10 才对得上？
- 
SQL1 与 SQL2 `groupby` 之后得到的行数都是 `552203`，为什么会出现 SQL1 内存不够，里面还有哪些细节呢？
- 
trace 信息里的 `creating_tmp_table.tmp_table_info.row_limit_estimate` 都是 `838860`；计算由来是临时表的内存限制大小 16 MB，而一行需要占的空间是 20 字节，那么最多只能容纳 `floor(16777216/20)=838860` 行，而实际我们需要放入临时表的行数是 `785102`。为什么呢？
- 
SQL1 使用 `SQL_BIG_RESULT` 优化之后，原始表需要扫描的行数会乘以 2，背后逻辑是什么呢？为什么仅仅是不再尝试往内存临时表里写入这一步会相差 10 多倍的性能？
- 
通过源码看到 trace 信息里面很多扫描行数都不是实际的行数，既然是实际执行，为什么 trace 信息里不输出真实的扫描行数和容量等呢？比如 `filesort_priority_queue_optimization.rows_estimate` 在 SQL1 中的扫描行数我通过 gdb 看到计算规则如附录图 1。
- 
有没有工具能够统计 SQL 执行过程中的 I/O 次数？
# 附录
![](https://img-blog.csdnimg.cn/20190321210201321.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
