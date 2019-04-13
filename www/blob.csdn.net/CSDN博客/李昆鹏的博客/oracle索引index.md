
# oracle索引index - 李昆鹏的博客 - CSDN博客


2018年06月06日 21:40:58[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：39


-----------------------------------------oracle索引index---------------------------------------------
索引是用于加速数据存取的数据对象。合理的使用索引可以大大降低i/o 次数,从而
提高数据访问性能。索引有很多种我们主要介绍常用的几种:
为什么添加了索引之后，会加快查询速度呢？
**图书馆**：如果杂乱地放书的话检索起来就非常困难,所以将书分类，然后再建一个箱子，箱
子里面放卡片，卡片里面可以按类查询,按姓名查或者类别查,这样的话速度会快很多很多，
这个就有点像索引。索引的好处就是提高你找到书的速度，但是正是因为你建了索引，就应
该有人专门来维护索引，维护索引是要有时间精力的开销的，也就是说索引是不能乱建的，
所以建索引有个原则：如果有一个字段如果不经常查询，就不要去建索引。现在把书变成我
们的表，把卡片变成我们的索引，就知道为什么索引会快，为什么会有开销。
创建索引的语法：
创建索引：
1． 单例索引
单例索引是基于单个列所建立的索引，比如:
CREATE index 索引名  on 表名(列名)
2． 复合索引
复合索引是基于两个列或多个列的索引。在同一张表上可以有多个索引，但是
要求列的组合必须不同,比如：
Create index emp_idx1 on emp(ename,job);
Create index emp_idx1 on emp(job,ename);
范例：给person表的name建立索引
createindexpname_indexonperson(name);
范例：给person表创建一个name和gender的索引
createindexpname_gender_indexonperson(name, gender);
索引的使用原则：
Ø在大表上建立索引才有意义
Ø在where子句后面或者是连接条件上建立索引
Ø索引的层次不要超过4层
代码示例：
|select * from PERSON t
|--创建单列索引
|create index pname_index on  person(pname);
|--索引不需要主动使用，根据索引列查询时就会自动的使用索引
|select * from person t where t.pname = '张三';
|--复合索引
|create index pg_index on person(pname,  gender);
|select * from person t where t.pname = '张三' and t.gender = 1;
|--查询数据使用的时候是有索引顺序的，下面这条sql就不会使用pg_index
|select * from person t where t.gender = 1  and t.pname = '张三';
|


