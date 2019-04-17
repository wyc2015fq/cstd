# sql语句相似度统计的说明 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年05月27日 12:25:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4384








1.需求描述：

sql相似度统计是针对数据库审计中采集到sql语句及其满足一定相似度的sql语句统计其执行次数，用于判断是否高危，对于次数执行相对低的给予告警。在统计sql语句执行次数上，要判断同类sql语句的相似度，达到一定相似度的可以定位为同一类sql语句。



2.实现描述：

——输入：相似度值，比如90%；操作对象，比如表名Table_name；操作类型，比如insert
 into、select；执行次数，比如超过10000次则可以不再统计；

——操作：

1）sql语句结构分离

对sql语句进行结构分离，将sql语法基本结构和语句其他字符分别用数据结构接收。

比如insert into table_name(field_1,field_2) values(‘value_1’,’value_2’)，结构抽离后如下：

Para_1=insert into——操作类型

Para_2=table_name——操作对象

Para_3=(field_1,field_2)——插入字段

Para_4= values

Para_5=(‘value_1’,’value_2’)

2）条件判断

提取符合操作类型和操作对象的sql语句。

3）sql语法结构匹配

进行sql语法结构匹配，结构相同才进行下一步字符相似度匹配。比如两条sql语句都是insert
 into # values#，则语法结构相同。

4）相似度匹配

根据输入的相似度值，对语法结构外的字符进行顺序匹配，满足阀值则定义这两条语句是相似，增加执行次数。

——输出：sql语句及其匹配的sql语句执行次数。

案例：sql语句1：select
 field_1 from table_1 where filed_2=tmp2

Sql语句2：select
 field_2 from table_1 where field_3=tmp3

输入：70%相似度，操作对象为table_1，全部操作类型；

操作：

对sql语句进行结构分离后，先进行条件判断，sql语句1和sql语句2都是table_1对象；

然后进行结构匹配，sql语句1和sql语句2都是select#from#where#的基本结构，匹配满足，进入字符相似度匹配；

相似度匹配上有字段和条件，sql语句1的field1和sql语句2的field2相似度顺序字符匹配达到5/6，大于70%；sql语句1的filed_2=tmp2和sql语句2的field_3=tmp3相似度顺序字符匹配达到达到6/12，小于70%；二者平均下来4/5，大于70%，满足阀值，这两个sql语句是相似的，执行次数分别加1。

5）基本sql语法结构
|select#from#where#|
|----|
|insert into#values#|
|update#set#where#|
|delete from#where#|
|create#|
|backup#|
|drop#|
|alter#add#|
|truncate#drop storage|
|lock#in#|
|grant#|
|revoke#|
|deny#|
|rollback#|
|commit#|
|call #|

可在基本sql语法基础上再行设计和延伸。



