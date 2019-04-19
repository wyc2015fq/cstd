# 详述MySQL中select的使用 - 逍遥剑客 - CSDN博客
2006年11月30日 23:23:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1351
SELECT [STRAIGHT_JOIN] [SQL_SMALL_RESULT] [SQL_BIG_RESULT] [HIGH_PRIORITY] [DISTINCT|DISTINCTROW|ALL] select_list [INTO {OUTFILE|DUMPFILE} 'file_name' export_options] [FROM table_references [WHERE where_definition] [GROUP BY col_name,...] [HAVING where_definition] [ORDER BY {unsighed_integer|col_name|formura} [ASC|DESC],...] [LIMIT [offset,] rows] [PROCEDURE procedure_name]] 
从这个基本语法可以看出，最简单的SELECT语句是SELECT select_list，实际上利用这个最简单的SELECT语句，你也可以完成许多你期待的功能，首先你能利用它进行MySQL所支持的任何运算，例如：SELECT 1+1，它将返回2；其次，你也能利用它给变量赋值，而在PHP中，运用SELECT语句的这种功能，你就可以自由地运用MySQL的函数为PHP程序进行各种运算，并赋值给变量。在很多的时候，你会发现MySQL拥有许多比PHP更为功能强大的函数。
STRAIGHT_JOIN、SQL_SMALL_RESULT、SQL_BIG_RESULT、HIGH_PRIORITY是MySQL对ANSI SQL92的扩展。如果优化器以非最佳次序联结表，使用STRAIGHT_JOIN可以加快查询。 
SQL_SMALL_RESULT和SQL_BIG_RESULT是一组相对的关键词。它们必须与 GROUP BY、DISTINCT或DISTINCTROW一起使用。SQL_SMALL_RESULT告知优化器结果会很小，要求MySQL使用临时表存储最终的表而不是使用排序；反之，SQL_BIG_RESULT告知优化器结果会很小，要求MySQL使用排序而不是做临时表。
HIGH_PRIORITY将赋予SELECT比一个更新表的语句更高的优先级，使之可以进行一次优先的快速的查询。
以上四个关键词的使用方法的确比较晦涩。幸运的是，在绝大多数情况下，在MySQL中我们完全可以选择不使用这四个关键词。 
DISTINCT、DISTINCTROW对查询返回的结果集提供了一个最基本但是很有用的过滤。那就是结果集中只含非重复行。在这里要注意的是，对关键词DISTINCT、DISTINCTROW来说，空值都是相等的，无论有多少NULL值，只选择一个。而ALL的用法就有画蛇添足之嫌了。它对结果集的产生没有任何影响。 
INTO {OUTFILE|DUMPFILE} 'file_name' export_options，将结果集写入一个文件。文件在服务器主机上被创建，并且不能是已经存在的。语句中的export_options部分的语法与用在LOAD DATAINFILE语句中的FIELDS和LINES子句中的相同。而OUTFILE与DUMPFILE的关键字的区别是：后前只写一行到文件，并没有任何列或行结束。
select list：其中可以包含一项或多项下列内容： 
1、“*”，表示按照create table的顺序排列的所有列。 
2、按照用户所需顺序排列的列名的清单。 
3、可以使用别名取代列名，形式如下：column name as column_heading。 
4、表达式（列名、常量、函数，或以算术或逐位运算符连接的列名、常量和函数的任何组合）。 
5、内部函数或集合函数。 
6、上述各项的任何一种组合。 
FROM：决定SELECT命令中使用哪些表。一般都要求有此项，除非select_list中不含列名（例如，只有常量、算术表达式等）。如果表项中有多个表，用逗号将之分开。在关键词FROM后面的表的顺序不影响结果。 
表名可以给出相关别名，以便使表达清晰。这里的语法是tbl_name [AS] alias_name。例如： 
select t1.name,t2.salary from employee as t1,info as t2 where t1.name=t2.name与select t1.name,t2.salary from employee t1,info t2 where t1.name=t2.name是完全等价的。 
所有对该表的其他引用，例如在where子句和having子句中，都要用别名，别名不能以数字开头。 
where 子句设置了搜索条件，它在insert，update，delete语句中的应用方法也与在select语句中的应用方法完全相同。搜索条件紧跟在关键词 where的后面。如果用户要在语句中使用多个搜索条件，则可用and或or连接。搜索条件的基本语法是[not] expression comparison_operator expression；[not] expression [not] like “match_string”；[not] expression is [not] null；[not] expression [not] between expression and expression；[not] column_name join_operator column_name；[not] boolean_expression。 
and：用来联结两个条件，并在两个条件都是TRUE的时候返回结果。当在同一语句中使用多个逻辑运算符时，and运算符总是最优先，除非用户用括号改变了运算顺序。 
or：用来联结两个条件，当两个条件中有任一条件是TRUE的时候返回结果。当在同一语句中使用多个逻辑运算符时，运算符or通常在运算符and之后进行运算。当然用户可以使用括号改变运算的顺序。 
between：用来标识范围下限的关键词，and后面跟范围上限的值。范围where @val between x and y包含首尾值。如果between后面指定的第一个值大于第二个值，则该查询不返回任何行。 
column_name：在比较中使用的列名。在会产生歧义时，一定要指明列所在的表名。 
comparison_operator：比较运算符。见下表： 
符号 意义 
=  等于 
>  大于 
<  小于 
>=  大于等于 
<=  小于等于 
!=  不等于 
<>  不等于 
在比较char，varchar型数据时，“<”的意思是更接近字母表头部，“>”代表更接近字母表尾部。一般来说，小写字母大于大写字母，大写字母大于数字，但是这可能依赖于服务器上操作系统的比较顺序。 
在比较时，末尾的空格是被忽略的。例如，“Dirk”等于“Dirk ”。 
在比较日期时，“<”表示早于，“>”表示晚于。 
在使用比较运算符比较character和datetime数据时，需用引号将所有数据引起来。 
expression：可能是列名、常数、函数或者是列名或常数的任意组合，以及以算术运算符或逐位运算符连接的函数。算术运算符如下表所示： 
符号   意义 
+    加号 
-    减号    
*    乘号 
/    除号 
is null：在搜索一个NULL值时使用。 
like：关键词，对char、varchar和datetime（不包括秒和毫秒）可以使用like，在MySQL中like也可以用在数字的表达式上。 
当用户在搜索datetime型数据时，最好是使用关键词like，因为完整的datetime记录包含各种各样的日期组件。例如用户在列 arrival_time中加入一个值“9:20”，而子句where arrival_time=“9:20”却没有发现它，因为MySQL把录入的数据转换成了“Jan 1,1900 9:20AM”。然而子句where arrival_time like“%9:20%”就能找到它。 
boolean_expression：返回“true”或“false”值的表达式。 
match_string：由字符和通配符组成的串，用单引号或双引号引起来，是匹配模式。通配符如下表所示： 
符号  意义 
%    0或多个字符的字符串 
_    任何一单个字符 
not：否定任何逻辑表达式，或是关键词，如like，null，between等。 
group by和having子句在select语句中使用，可以将表划分成组并返回匹配having子句条件的组。 
语法：select语句开头 
group by [all] aggregate_free_expression [,aggregate_free_expression]* 
[having search_conditions] 
select语句结尾 
group by：指定表将划分的组群，如果在select表项中包含集合函数，则为各组计算一个总计值。这些总计值的结果以新的列显示，而不是新的行。在 having子句中用户可以引用这些新的总计列。在group by之前的select_list中可以使用avg、count、max、min和sum等集合函数。表可以被任意列的组合分组。 
all：在结果中包含所有组群的Transact-SQL扩展，这里的所有组群甚至包括那些被where子句所排除的组群。如果同时使用having子句，将对all的意义进行否定。 
aggregate_free_expression：不包含集合函数的表达式，Transact-SQL扩展允许在用列名称分组的同时，用无集合函数的表达式分组。 
having：为group by子句设置条件，类似于where为select语句设置条件的方法。having的查找条件可以包括集合函数表达式。除此之外，它的查找条件与where查找条件相同。 
order by：按列排列结果。对select输出的列可以用列名、列别名或列位置来引用。例如：select id as myid,name as myname from mytable group by id、select id as myid,name as myname from mytable group by myid、select id as myid,name as myname from mytable group by 1这三句是完全等价的。当然，我们不赞成用第三种用法，这将给程序的可读性带来不好的影响。为了以降序排列，把DESC关键词加到order by子句中你要排序的列名前。缺省是升序，你也可以用ASC关键词明确指定。 
limit 子句：用来限制select语句返回的行数。limit取1个或2个数字参数，如果给定2个参数，第一个指定要返回的第一行的偏移量，第二个指定返回行的最大数目。初始行的偏移量是0（不是1）。如果给定一个参数，它指出偏移量为0的返回行的最大数目。也就是说limit 5和limit 0,5完全等价。 
至于procedure关键词的含义，我也没搞得太清楚，好象是对存储过程的支持，而MySQL本身不支持存储过程，看来是为了将来扩充的需要而保留的吧。             
