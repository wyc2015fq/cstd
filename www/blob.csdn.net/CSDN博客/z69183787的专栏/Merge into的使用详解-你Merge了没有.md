# Merge into的使用详解-你Merge了没有 - z69183787的专栏 - CSDN博客
2014年05月08日 15:26:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7200
Merge是一个非常有用的功能，类似于Mysql里的insert into on duplicate key. 
Oracle在9i引入了merge命令, 
通过这个merge你能够在一个SQL语句中对一个表同时执行inserts和updates操作. 当然是update还是insert是依据于你的指定的条件判断的，Merge into可以实现用B表来更新A表数据，如果A表中没有，则把B表的数据插入A表. MERGE命令从一个或多个数据源中选择行来updating或inserting到一个或多个表 
语法如下 
MERGE INTO [your table-name] [rename your table here] 
USING ( [write your query here] )[rename your query-sql and using just like a table] 
ON ([conditional expression here] AND [...]...) 
WHEN MATHED THEN [here you can execute some update sql or something else ] 
WHEN NOT MATHED THEN [execute something else here ! ] 
我们先看看一个简单的例子，来介绍一个merge into的用法 
merge into products p using newproducts np on (p.product_id = np.product_id) 
when matched then 
update set p.product_name = np.product_name 
when not matched then 
insert values(np.product_id, np.product_name, np.category) 
在这个例子里。前面的merger into products using newproducts 表示的用newproducts表来merge到products表，merge的匹配关系就是on后面的条件子句的内容，这里根据两个表的product_id来进行匹配，那么匹配上了我们的操作是就是when matched then的子句里的动作了，这里的动作是update
 set p.product_name = np.product_name, 很显然就是把newproduct里的内容，赋值到product的product_name里。如果没有匹配上则insert这样的一条语句进去。 大家看看这个merget inot的用法是不是一目了然了呀。这里merger的功能，好比比较，然后选择更新或者是插入，是一系列的组合拳，在做merge的时候，这样同样的情况下，merge的性能是优于同等功能的update/insert语句的。有人曾经分析merge是批量处理对性能贡献很大，个人觉得这个是没有考据的。 
我们也可以在using后面使用视图或者子查询。比如我们把newproducts换成 
merge into products p using (select * from newproducts) np on (p.product_id = np.product_id) 
when matched then 
update set p.product_name = np.product_name 
when not matched then 
insert values(np.product_id, np.product_name, np.category) 
也是可以的。 
在Oracle 10g中MERGE有如下一些改进： 
1、UPDATE或INSERT子句是可选的 
2、UPDATE和INSERT子句可以加WHERE子句 
3、在ON条件中使用常量过滤谓词来insert所有的行到目标表中,不需要连接源表和目标表 
4、UPDATE子句后面可以跟DELETE子句来去除一些不需要的行 
我们通过实例来一一看看如上的新特性 
1. UPDATE或INSERT子句是可选的 
在9i里由于必须insert into和update都要存在，也就是不是update就是insert，不支持单一的操作，虽然还是可以曲线救国，呵呵 但是有些过于强势了。而10g里就是可选了，能符合我们更多的需求了 
比如上面的句子 
我们可以只存在update或者insert 
merge into products p using newproducts np on (p.product_id = np.product_id) 
when matched then 
update set p.product_name = np.product_name 
这里，如果匹配就更新，不存在就不管了。 
2. UPDATE和INSERT子句可以加WHERE子句 
这也是一个功能性的改进，能够符合我们更多的需求，这个where的作用很明显是一个过滤的条件，是我们加入一些额外的条件，对只对满足where条件的进行更新和insert 
merge into products p using (select * from newproducts) np on (p.product_id = np.product_id) 
when matched then 
update set p.product_name = np.product_name where np.product_name like 'OL%' 
这里表示只是对product_name开头是'OL'的匹配上的进行update，如果开头不是'OL'的就是匹配了也不做什么事情，insert里也可以加入where 
比如 
merge into products p using (select * from newproducts) np on (p.product_id = np.product_id) 
when matched then 
update set p.product_name = np.product_name where np.product_name like 'OL%' 
when not matched then 
insert values(np.product_id, np.product_name, np.category) where np.product_name like 'OL%' 
这里注意比较一下，他们返回的结果行数，是有着差异的。 
3. 在ON条件中使用常量过滤谓词来insert所有的行到目标表中,不需要连接源表和目标表 
merge into products p using (select * from newproducts) np on (1=0) 
when matched then 
update set p.product_name = np.product_name 
when not matched then 
insert values(np.product_id, np.product_name, np.category) 
个人觉得这个功能没有太大的意义，我们的insert into本身就支持这样的功能，没有必要使用merge 
4. UPDATE子句后面可以跟DELETE子句来去除一些不需要的行 
delete只能和update配合，从而达到删除满足where条件的子句的纪录 
merge into products p using (select * from newproducts) np on (p.product_id = np.product_id) 
when matched then 
update set p.product_name = np.product_name delete where p.product_id = np.product_id where np.product_name like 'OL%' 
when not matched then 
insert values(np.product_id, np.product_name, np.category) 
这里我们达到的目的就是 会把匹配的记录的prodcut_name更新到product里，并且把product_name开头为OL的删除掉。
merge into也是一个dml语句，和其他的dml语句一样需要通过rollback和commit 结束事务。 
Merge是一个非常强大的功能，而且是我们需求里经常会用到的一个有用的功能，所以我们一定要好好的学习到。 
文中需要的测试脚本在附件里提供下载。 
[merge into sample.sql](http://www.inthirties.com/attachment.jsp?aid=117)
