# 详解 MySQL 中的 explain - 文章 - 伯乐在线
原文出处： [persister](http://www.blogjava.net/persister/archive/2008/10/27/236813.html)
在 explain的帮助下，您就知道什么时候该给表添加索引，以使用索引来查找记录从而让select 运行更快。如果由于不恰当使用索引而引起一些问题的话，可以运行 analyze table来更新该表的统计信息，例如键的基数，它能帮您在优化方面做出更好的选择。
explain 返回了一行记录，它包括了 select语句中用到的各个表的信息。这些表在结果中按照mysql即将执行的查询中读取的顺序列出来。mysql用一次扫描多次连接（single- sweep,multi-join）的方法来解决连接。这意味着mysql从第一个表中读取一条记录，然后在第二个表中查找到对应的记录，然后在第三个表 中查找，依次类推。当所有的表都扫描完了，它输出选择的字段并且回溯所有的表，直到找不到为止，因为有的表中可能有多条匹配的记录下一条记录将从该表读 取，再从下一个表开始继续处理。
在mysql version 4.1中，explain输出的结果格式改变了，使得它更适合例如 union语句、子查询以及派生表的结构。更令人注意的是，它新增了2个字段： id和 select_type。当你使用早于mysql4.1的版本就看不到这些字段了。
explain结果的每行记录显示了每个表的相关信息，每行记录都包含以下几个字段：
**id**
本次 select 的标识符。在查询中每个 select都有一个顺序的数值。
**select_type**
select 的类型，可能会有以下几种：
**simple:** 简单的 select （没有使用 union或子查询）
**primary:** 最外层的 select。
**union:** 第二层，在select 之后使用了 union。
**dependent union:** union 语句中的第二个select，依赖于外部子查询
**subquery:** 子查询中的第一个 select
**dependent subquery:** 子查询中的第一个 subquery依赖于外部的子查询
**derived:** 派生表 select（from子句中的子查询）
**table**
记录查询引用的表。
**type**
表连接类型。以下列出了各种不同类型的表连接，依次是从最好的到最差的：
**system:**表只有一行记录（等于系统表）。这是 const表连接类型的一个特例。
**const:**表中最多只有一行匹配的记录，它在查询一开始的时候就会被读取出来。由于只有一行记录，在余下的优化程序里该行记录的字段值可以被当作是一个 恒定值。const表查询起来非常快，因为只要读取一次！const 用于在和 primary key 或unique 索引中有固定值比较的情形。下面的几个查询中，tbl_name 就是 c表了：

MySQL
```
select * from tbl_name where primary_key=1; 
select * from tbl_namewhere primary_key_part1=1 and primary_key_part2=2;
```
**eq_ref:**从该表中会有一行记录被读取出来以和从前一个表中读取出来的记录做联合。与const类型不同的是，这是最好的连接类型。它用在索引所有部 分都用于做连接并且这个索引是一个primary key 或 unique 类型。
eq_ref可以用于在进行”=”做比较时检索字段。比较的值可以是固定值或者是表达式，表达示中可以使用表里的字段，它们在读表之前已经准备好 了。以下的几个例子中，mysql使用了eq_ref 连接来处理 ref_table：

MySQL
```
select * from ref_table,other_table whereref_table.key_column=other_table.column; 
select * fromref_table,other_table whereref_table.key_column_part1=other_table.column andref_table.key_column_part2=1;
```
**ref:** 该表中所有符合检索值的记录都会被取出来和从上一个表中取出来的记录作联合。ref用于连接程序使用键的最左前缀或者是该键不是 primary key 或 unique索引（换句话说，就是连接程序无法根据键值只取得一条记录）的情况。
当根据键值只查询到少数几条匹配的记录时，这就是一个不错的连接类型。 ref还可以用于检索字段使用 =操作符来比较的时候。以下的几个例子中，mysql将使用 ref 来处理ref_table：

MySQL
```
select * from ref_table where key_column=expr; 
select * fromref_table,other_table whereref_table.key_column=other_table.column; 
select * fromref_table,other_table whereref_table.key_column_part1=other_table.column andref_table.key_column_part2=1;
```
**ref_or_null:** 这种连接类型类似 ref，不同的是mysql会在检索的时候额外的搜索包含null 值的记录。这种连接类型的优化是从mysql4.1.1开始的，它经常用于子查询。在以下的例子中，mysql使用ref_or_null 类型来处理 ref_table：

MySQL
```
select * from ref_table where key_column=expr or key_column is null;
```
**unique_subquery:** 这种类型用例如一下形式的 in 子查询来替换 ref：

MySQL
```
value in (select primary_key from single_table where some_expr)
```
**unique_subquery:** 只是用来完全替换子查询的索引查找函数效率更高了。
**index_subquery:** 这种连接类型类似 unique_subquery。它用子查询来代替in，不过它用于在子查询中没有唯一索引的情况下，例如以下形式：

MySQL
```
value in (select key_column from single_table where some_expr)
```
**range:** 只有在给定范围的记录才会被取出来，利用索引来取得一条记录。key字段表示使用了哪个索引。key_len字段包括了使用的键的最长部分。这种类型时 ref 字段值是 null。range用于将某个字段和一个定植用以下任何操作符比较时 =, <>, >,>=, <, <=, is null, <=>, between, 或 in：

MySQL
```
select * from tbl_name where key_column = 10; select * fromtbl_name where key_column between 10 and 20; select * from tbl_namewhere key_column in (10,20,30); select * from tbl_name wherekey_part1= 10 and key_part2 in (10,20,30);
```
**index:** 连接类型跟 all 一样，不同的是它只扫描索引树。它通常会比 all快点，因为索引文件通常比数据文件小。mysql在查询的字段知识单独的索引的一部分的情况下使用这种连接类型。
**all:** 将对该表做全部扫描以和从前一个表中取得的记录作联合。这时候如果第一个表没有被标识为const的话就不大好了，在其他情况下通常是非常糟糕的。正常地，可以通过增加索引使得能从表中更快的取得记录以避免all。
**possible_keys**
possible_keys字段是指 mysql在搜索表记录时可能使用哪个索引。注意，这个字段完全独立于explain 显示的表顺序。这就意味着 possible_keys里面所包含的索引可能在实际的使用中没用到。如果这个字段的值是null，就表示没有索引被用到。
这种情况下，就可以检查 where子句中哪些字段那些字段适合增加索引以提高查询的性能。就这样，创建一下索引，然后再用explain 检查一下。详细的查看章节”14.2.2 alter tablesyntax”。想看表都有什么索引，可以通过 show index from tbl_name来看。
**key**
key字段显示了mysql实际上要用的索引。当没有任何索引被用到的时候，这个字段的值就是null。想要让mysql强行使用或者忽略在 possible_keys字段中的索引列表，可以在查询语句中使用关键字force index, use index,或 ignore index。
如果是 myisam 和 bdb 类型表，可以使用 analyzetable 来帮助分析使用使用哪个索引更好。如果是 myisam类型表，运行命令 myisamchk –analyze也是一样的效果。详细的可以查看章节”14.5.2.1 analyze tablesyntax”和”5.7.2 table maintenance and crash recovery”。
**key_len**
key_len 字段显示了mysql使用索引的长度。当 key 字段的值为 null时，索引的长度就是 null。注意，key_len的值可以告诉你在联合索引中mysql会真正使用了哪些索引。
**ref**
ref 字段显示了哪些字段或者常量被用来和 key配合从表中查询记录出来。
**rows**
rows 字段显示了mysql认为在查询中应该检索的记录数。
**extra**
本字段显示了查询中mysql的附加信息。以下是这个字段的几个不同值的解释：
**distinct:**mysql当找到当前记录的匹配联合结果的第一条记录之后，就不再搜索其他记录了。
**not exists:**mysql在查询时做一个 left join优化时，当它在当前表中找到了和前一条记录符合 left join条件后，就不再搜索更多的记录了。下面是一个这种类型的查询例子：

MySQL
```
select * from t1 left join t2 on t1.id=t2.id where t2.id isnull;
```
假使 t2.id 定义为 not null。这种情况下，mysql将会扫描表 t1并且用 t1.id 的值在 t2 中查找记录。当在 t2中找到一条匹配的记录时，这就意味着 t2.id 肯定不会都是null，就不会再在 t2 中查找相同 id值的其他记录了。也可以这么说，对于 t1 中的每个记录，mysql只需要在t2 中做一次查找，而不管在 t2 中实际有多少匹配的记录。

MySQL
```
range checked for each record (index map: #)
```
mysql没找到合适的可用的索引。取代的办法是，对于前一个表的每一个行连接，它会做一个检验以决定该使用哪个索引（如果有的话），并且使用这个索引来从表里取得记录。这个过程不会很快，但总比没有任何索引时做表连接来得快。
**using filesort:** mysql需要额外的做一遍从而以排好的顺序取得记录。排序程序根据连接的类型遍历所有的记录，并且将所有符合 where条件的记录的要排序的键和指向记录的指针存储起来。这些键已经排完序了，对应的记录也会按照排好的顺序取出来。详情请看”7.2.9how mysql optimizes order by”。
**using index**
字段的信息直接从索引树中的信息取得，而不再去扫描实际的记录。这种策略用于查询时的字段是一个独立索引的一部分。
**using temporary:** mysql需要创建临时表存储结果以完成查询。这种情况通常发生在查询时包含了groupby 和 order by 子句，它以不同的方式列出了各个字段。
**using where**
where子句将用来限制哪些记录匹配了下一个表或者发送给客户端。除非你特别地想要取得或者检查表种的所有记录，否则的话当查询的extra 字段值不是 using where 并且表连接类型是 all 或 index时可能表示有问题。
如果你想要让查询尽可能的快，那么就应该注意 extra 字段的值为usingfilesort 和 using temporary 的情况。
你可以通过 explain 的结果中 rows字段的值的乘积大概地知道本次连接表现如何。它可以粗略地告诉我们mysql在查询过程中会查询多少条记录。如果是使用系统变量 max_join_size 来取得查询结果，这个乘积还可以用来确定会执行哪些多表select 语句。
下面的例子展示了如何通过 explain提供的信息来较大程度地优化多表联合查询的性能。
假设有下面的 select 语句，正打算用 explain 来检测：

MySQL
```
explain select tt.ticketnumber, tt.timein, tt.projectreference,tt.estimatedshipdate,
tt.actualshipdate, tt.clientid,tt.servicecodes, tt.repetitiveid, tt.currentprocess,
tt.currentdppers tt.recordvolume, tt.dpprinted, et.country,et_1.country,
do.custname from tt, et, et as et_1, do wherett.submittime is null and tt.actualpc = et.employid
andtt.assignedpc = et_1.employid and tt.clientid = do.custnmbr;
```
在这个例子中，先做以下假设：
要比较的字段定义如下：
**table  column  columntype**
** tt  actualpc char(10)**
** tt  assignedpc char(10)**
** tt  clientid char(10)**
** et  employid char(15)**
** do  custnmbr char(15)**
数据表的索引如下：
**table  index**
** tt  actualpc**
** tt  assignedpc**
** tt  clientid**
** et  employid (primary key)**
** do  custnmbr (primary key)**
tt.actualpc 的值是不均匀分布的。
在任何优化措施未采取之前，经过 explain分析的结果显示如下：

MySQL
```
table type possible_keys key key_len ref rows extra
et all primarynull null null 74
do all primary null null null 2135
et_1 allprimary null null null 74
tt all assignedpc, null null null 3872 clientid, actualpc range checked for each record (key map: 35)
```
由于字段 type 的对于每个表值都是all，这个结果意味着mysql对所有的表做一个迪卡尔积；这就是说，每条记录的组合。这将需要花很长的时间，因为需要扫描每个表总 记录数乘积的总和。在这情况下，它的积是74 * 2135 * 74 * 3872 = 45,268,558,720条记录。如果数据表更大的话，你可以想象一下需要多长的时间。
在这里有个问题是当字段定义一样的时候，mysql就可以在这些字段上更快的是用索引（对isam类型的表来说，除非字段定义完全一样，否则不会使用索 引）。在这个前提下，varchar和 char是一样的除非它们定义的长度不一致。由于 tt.actualpc 定义为char(10)，et.employid 定义为 char(15)，二者长度不一致。
为了解决这个问题，需要用 alter table 来加大 actualpc的长度从10到15个字符：

MySQL
```
mysql> alter table tt modify actualpc varchar(15);
```
现在 tt.actualpc 和 et.employid 都是 varchar(15)
了。再来执行一次 explain 语句看看结果：

MySQL
```
table type possible_keys key key_len ref rows extra
tt allassignedpc, null null null 3872 using clientid, where actualpc
do all primary null null null 2135 range checked for each record (keymap: 1)
et_1 all primary null null null 74 range checked for eachrecord (key map: 1) et eq_ref primary primary 15 tt.actualpc 1
```
这还不够，它还可以做的更好：现在 rows值乘积已经少了74倍。这次查询需要用2秒钟。
第二个改变是消除在比较 tt.assignedpc = et_1.employid 和 tt.clientid= do.custnmbr 中字段的长度不一致问题：

MySQL
```
mysql> alter table tt modify assignedpc varchar(15), ->modify clientid varchar(15);
```
现在 explain 的结果如下：

MySQL
```
table type possible_keys key key_len ref rows extra
et all primary null null null 74
tt ref assignedpc, actualpc 15 et.employid 52 using clientid, where actualpc
et_1 eq_ref primary primary 15 tt.assignedpc 1
do eq_ref primary primary 15 tt.clientid 1
```
这看起来已经是能做的最好的结果了。
遗留下来的问题是，mysql默认地认为字段 tt.actualpc的值是均匀分布的，然而表 tt并非如此。幸好，我们可以很方便的让mysql分析索引的分布：

MySQL
```
mysql> analyze table tt;
```
到此为止，表连接已经优化的很完美了，explain 的结果如下：

MySQL
```
table type possible_keys key key_len ref rows extra
tt all assignedpc null null null 3872 using clientid, where actualpc
et eq_ref primary primary 15 tt.actualpc 1
et_1 eq_ref primary primary 15 tt.assignedpc 1
do eq_ref primary primary 15 tt.clientid 1
```
请注意，explain 结果中的 rows字段的值也是mysql的连接优化程序大致猜测的，请检查这个值跟真实值是否基本一致。如果不是，可以通过在select 语句中使用 straight_join 来取得更好的性能，同时可以试着在from分句中用不同的次序列出各个表。
