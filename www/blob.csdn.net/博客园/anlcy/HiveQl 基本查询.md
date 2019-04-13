
# HiveQl 基本查询 - anlcy - 博客园






# [HiveQl 基本查询](https://www.cnblogs.com/camilla/p/8986460.html)
1 基本的Select 操作
SELECT [ALL | DISTINCT] select_expr, select_expr, ...
FROM table_reference
[WHERE where_condition]
[GROUP BY col_list [HAVING condition]]
[   CLUSTER BY col_list
| [DISTRIBUTE BY col_list] [SORT BY| ORDER BY col_list]
[LIMIT number]
•使用ALL和DISTINCT选项区分对重复记录的处理。默认是ALL，表示查询所有记录。DISTINCT表示去掉重复的记录
•Where 条件
•类似我们传统SQL的where 条件
•目前支持 AND,OR ,0.9版本支持between
•IN, NOT IN
•不支持EXIST ,NOT EXIST
ORDER BY与SORT BY的不同
•ORDER BY 全局排序，只有一个Reduce任务
•SORT BY 只在本机做排序
Limit
•Limit 可以限制查询的记录数
SELECT * FROM t1 LIMIT 5
•实现Top k 查询
•下面的查询语句查询销售记录最大的 5 个销售代表。
SET mapred.reduce.tasks = 1
SELECT * FROM test SORT BY amount DESC LIMIT 5
•REGEX Column Specification
SELECT 语句可以使用正则表达式做列选择，下面的语句查询除了 ds 和 hr 之外的所有列：
SELECT `(ds|hr)?+.+` FROM test
例如
按先件查询
hive> SELECT a.foo FROM invites a WHERE a.ds='<DATE>';
将查询数据输出至目录：
hive> INSERT OVERWRITE DIRECTORY '/tmp/hdfs_out' SELECT a.* FROM invites a WHERE a.ds='<DATE>';
将查询结果输出至本地目录：
hive> INSERT OVERWRITE LOCAL DIRECTORY '/tmp/local_out' SELECT a.* FROM pokes a;
选择所有列到本地目录 ：
hive> INSERT OVERWRITE TABLE events SELECT a.* FROM profiles a;
hive> INSERT OVERWRITE TABLE events SELECT a.* FROM profiles a WHERE a.key < 100;
hive> INSERT OVERWRITE LOCAL DIRECTORY '/tmp/reg_3' SELECT a.* FROM events a;
hive> INSERT OVERWRITE DIRECTORY '/tmp/reg_4' select a.invites, a.pokes FROM profiles a;
hive> INSERT OVERWRITE DIRECTORY '/tmp/reg_5' SELECT COUNT(1) FROM invites a WHERE a.ds='<DATE>';
hive> INSERT OVERWRITE DIRECTORY '/tmp/reg_5' SELECT a.foo, a.bar FROM invites a;
hive> INSERT OVERWRITE LOCAL DIRECTORY '/tmp/sum' SELECT SUM(a.pc) FROM pc1 a;
将一个表的统计结果插入另一个表中：
hive> FROM invites a INSERT OVERWRITE TABLE events SELECT a.bar, count(1) WHERE a.foo > 0 GROUP BY a.bar;
hive> INSERT OVERWRITE TABLE events SELECT a.bar, count(1) FROM invites a WHERE a.foo > 0 GROUP BY a.bar;
JOIN
hive> FROM pokes t1 JOIN invites t2 ON (t1.bar = t2.bar) INSERT OVERWRITE TABLE events SELECT t1.bar, t1.foo, t2.foo;
将多表数据插入到同一表中：
FROM src
INSERT OVERWRITE TABLE dest1 SELECT src.* WHERE src.key < 100
INSERT OVERWRITE TABLE dest2 SELECT src.key, src.value WHERE src.key >= 100 and src.key < 200
INSERT OVERWRITE TABLE dest3 PARTITION(ds='2008-04-08', hr='12') SELECT src.key WHERE src.key >= 200 and src.key < 300
INSERT OVERWRITE LOCAL DIRECTORY '/tmp/dest4.out' SELECT src.value WHERE src.key >= 300;
将文件流直接插入文件：
hive> FROM invites a INSERT OVERWRITE TABLE events SELECT TRANSFORM(a.foo, a.bar) AS (oof, rab) USING '/bin/cat' WHERE a.ds > '2008-08-09';
2. 基于Partition的查询
•一般 SELECT 查询会扫描整个表，使用 PARTITIONED BY 子句建表，查询就可以利用分区剪枝（input pruning）的特性
•Hive 当前的实现是，只有分区断言出现在离 FROM 子句最近的那个WHERE 子句中，才会启用分区剪枝
3.Join
Syntax
join_table:
table_reference JOIN table_factor [join_condition]
| table_reference {LEFT|RIGHT|FULL} [OUTER] JOIN table_reference join_condition
| table_reference LEFT SEMI JOIN table_reference join_condition
table_reference:
table_factor
| join_table
table_factor:
tbl_name [alias]
| table_subquery alias
| ( table_references )
join_condition:
ON equality_expression ( AND equality_expression )*
equality_expression:
expression = expression
•Hive 只支持等值连接（equality joins）、外连接（outer joins）和（left semi joins）。Hive 不支持所有非等值的连接，因为非等值连接非常难转化到 map/reduce 任务
•LEFT，RIGHT和FULL OUTER关键字用于处理join中空记录的情况
•LEFT SEMI JOIN 是 IN/EXISTS 子查询的一种更高效的实现
•join 时，每次 map/reduce 任务的逻辑是这样的：reducer 会缓存 join 序列中除了最后一个表的所有表的记录，再通过最后一个表将结果序列化到文件系统
•实践中，应该把最大的那个表写在最后
join 查询时，需要注意几个关键点
只支持等值join
•SELECT a.* FROM a JOIN b ON (a.id = b.id)
•SELECT a.* FROM a JOIN b ON (a.id = b.id AND a.department = b.department)
•可以 join 多于 2 个表，例如
SELECT a.val, b.val, c.val FROM a JOIN b  ON (a.key = b.key1) JOIN c ON (c.key = b.key2)
•如果join中多个表的 join key 是同一个，则 join 会被转化为单个 map/reduce 任务
LEFT，RIGHT和FULL OUTER
例子
•SELECT a.val, b.val FROM a LEFT OUTER JOIN b ON (a.key=b.key)
•如果你想限制 join 的输出，应该在 WHERE 子句中写过滤条件——或是在 join 子句中写
•容易混淆的问题是表分区的情况
• SELECT c.val, d.val FROM c LEFT OUTER JOIN d ON (c.key=d.key)
WHERE a.ds='2010-07-07' AND b.ds='2010-07-07‘
•如果 d 表中找不到对应 c 表的记录，d 表的所有列都会列出 NULL，包括 ds 列。也就是说，join 会过滤 d 表中
不能找到匹配 c 表 join key 的所有记录。这样的话，LEFT OUTER 就使得查询结果与 WHERE 子句无关
•解决办法
•SELECT c.val, d.val FROM c LEFT OUTER JOIN d
ON (c.key=d.key AND d.ds='2009-07-07' AND c.ds='2009-07-07')
LEFT SEMI JOIN
•LEFT SEMI JOIN 的限制是， JOIN 子句中右边的表只能在 ON 子句中设置过滤条件，在 WHERE 子句、SELECT 子句或其他地方过滤都不行
•
•SELECT a.key, a.value FROM a WHERE a.key in (SELECT b.key FROM B);
可以被重写为：
SELECT a.key, a.val FROM a LEFT SEMI JOIN b on (a.key = b.key)
UNION ALL
•用来合并多个select的查询结果，需要保证select中字段须一致
•select_statement UNION ALL select_statement UNION ALL select_statement ...





