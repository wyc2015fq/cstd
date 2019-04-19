# case when then else end - 数据之美的博客 - CSDN博客
2017年06月08日 09:53:42[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：308
**sql case when then else end**
**查询生成统计列表：**
**[sql]**[view
 plain](http://blog.csdn.net/xuxurui007/article/details/8479953#)[copy](http://blog.csdn.net/xuxurui007/article/details/8479953#)
[print](http://blog.csdn.net/xuxurui007/article/details/8479953#)[?](http://blog.csdn.net/xuxurui007/article/details/8479953#)
- SELECT a.managecom,  
-        a.subtype,  
- count(*) loadsucc,  
- sum(casewhen a.state in  ('4', '5', '6', '7', '8', '9') then 1 else 0 end) recogsucc,  
- sum(casewhen a.state in  ('3', '12', '13') then 1 else 0 end) recogfail,  
- sum(casewhen a.state in  ('1', '2') then 1 else 0 end) waitrecog  
- FROM ocr_docdetail a, ocr_loaddetail c  
- WHERE 1 = 1  
- and a.managecom like'86%'
- and a.managecom = c.managecom  
- and a.bussno = c.bussno  
- and a.subtype = c.subtype  
- and c.loadstate = 0  
- and c.scandate >= date'2012-07-29'
- and c.scandate <= date'2013-01-07'
- groupby a.managecom, a.subtype  
- orderby a.managecom, a.subtype;  
case具有两种格式。简单case函数和case搜索函数。
--简单case函数
case sex
when'1'then'男'
when'2'then'女'
else'其他'end
--case搜索函数
casewhen sex = '1'then'男'
when sex = '2'then'女'
else'其他'end
这两种方式，可以实现相同的功能。简单case函数的写法相对比较简洁，但是和case搜索函数相比，功能方面会有些限制，比如写判定式。 
还有一个需要注重的问题，case函数只返回第一个符合条件的值，剩下的case部分将会被自动忽略。
--比如说，下面这段sql，你永远无法得到“第二类”这个结果
casewhen col_1 in ( 'a', 'b') then'第一类'
when col_1 in ('a')       then'第二类'
else'其他'end
下面我们来看一下，使用case函数都能做些什么事情。 
**一，已知数据按照另外一种方式进行分组，分析。**
有如下数据:(为了看得更清楚，我并没有使用国家代码，而是直接用国家名作为primary key)
|**国家（country）**|**人口（population）**|
|----|----|
|中国|600|
|美国|100|
|加拿大|100|
|英国|200|
|法国|300|
|日本|250|
|德国|200|
|墨西哥|50|
|印度|250|
根据这个国家人口数据，统计亚洲和北美洲的人口数量。应该得到下面这个结果。
|**洲**|**人口**|
|----|----|
|亚洲|1100|
|北美洲|250|
|其他|700|
想要解决这个问题，你会怎么做？生成一个带有洲code的view，是一个解决方法，但是这样很难动态的改变统计的方式。 
假如使用case函数，sql代码如下:
select sum(population),
case country
when'中国'then'亚洲'
when'印度'then'亚洲'
when'日本'then'亚洲'
when'美国'then'北美洲'
when'加拿大'then'北美洲'
when'墨西哥'then'北美洲'
else'其他'end
from   table_a
groupbycase country
when'中国'then'亚洲'
when'印度'then'亚洲'
when'日本'then'亚洲'
when'美国'then'北美洲'
when'加拿大'then'北美洲'
when'墨西哥'then'北美洲'
else'其他'end;
同样的，我们也可以用这个方法来判定工资的等级，并统计每一等级的人数。sql代码如下；
select
casewhen salary <= 500 then'1'
when salary > 500 and salary <= 600  then'2'
when salary > 600 and salary <= 800  then'3'
when salary > 800 and salary <= 1000 then'4'
elsenullend salary_class,
            count(*)
from   table_a
groupby
casewhen salary <= 500 then'1'
when salary > 500 and salary <= 600  then'2'
when salary > 600 and salary <= 800  then'3'
when salary > 800 and salary <= 1000 then'4'
elsenullend;
**二，用一个sql语句完成不同条件的分组。**
有如下数据
|**国家（country）**|**性别（sex）**|**人口（population）**|
|----|----|----|
|中国|1|340|
|中国|2|260|
|美国|1|45|
|美国|2|55|
|加拿大|1|51|
|加拿大|2|49|
|英国|1|40|
|英国|2|60|
按照国家和性别进行分组，得出结果如下
|**国家**|**男**|**女**|
|----|----|----|
|中国|340|260|
|美国|45|55|
|加拿大|51|49|
|英国|40|60|
普通情况下，用union也可以实现用一条语句进行查询。但是那样增加消耗(两个select部分)，而且sql语句会比较长。 
下面是一个是用case函数来完成这个功能的例子
select country,
            sum( casewhen sex = '1'then
            population else 0 end),  --男性人口
            sum( casewhen sex = '2'then
            population else 0 end)   --女性人口
from table_a
groupby country;
这样我们使用select，完成对二维表的输出形式，充分显示了case函数的强大。 
**三，在check中使用case函数。**
在check中使用case函数在很多情况下都是非常不错的解决方法。可能有很多人根本就不用check，那么我建议你在看过下面的例子之后也尝试一下在sql中使用check。
下面我们来举个例子 
公司a，这个公司有个规定，女职员的工资必须高于1000块。假如用check和case来表现的话，如下所示
constraint check_salary check
            ( casewhen sex = '2'
thencasewhen salary > 1000
then 1 else 0 end
else 1 end = 1 )
假如单纯使用check，如下所示
constraint check_salary check
            ( sex = '2'and salary > 1000 )
女职员的条件倒是符合了，男职员就无法输入了。
