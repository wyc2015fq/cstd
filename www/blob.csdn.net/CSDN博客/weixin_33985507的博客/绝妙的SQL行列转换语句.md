# 绝妙的SQL行列转换语句 - weixin_33985507的博客 - CSDN博客
2014年03月25日 16:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
说明：普通行列转换(version 1.0)仅针对sql server 2000提供静态和动态写法，version 2.0增加sql server 2005的有关写法。
问题：假设有张学生成绩表(tb)如下:
姓名 课程 分数
张三 语文 74
张三 数学 83
张三 物理 93
李四 语文 74
李四 数学 84
李四 物理 94
想变成(得到如下结果)： 
姓名 语文 数学 物理 
---- ---- ---- ----
李四 74   84   94
张三 74   83   93
-------------------
*/
createtable tb(姓名 varchar(10) , 课程 varchar(10) , 分数 int)
insertinto tb values('张三' , '语文' , 74)
insertinto tb values('张三' , '数学' , 83)
insertinto tb values('张三' , '物理' , 93)
insertinto tb values('李四' , '语文' , 74)
insertinto tb values('李四' , '数学' , 84)
insertinto tb values('李四' , '物理' , 94)
go
--SQL SERVER 2000 静态SQL,指课程只有语文、数学、物理这三门课程。(以下同)
select 姓名 as 姓名 ,
max(case 课程 when'语文'then 分数 else0end) 语文,
max(case 课程 when'数学'then 分数 else0end) 数学,
max(case 课程 when'物理'then 分数 else0end) 物理
from tb
groupby 姓名
--SQL SERVER 2000 动态SQL,指课程不止语文、数学、物理这三门课程。(以下同)
declare@sqlvarchar(8000)
set@sql='select 姓名 '
select@sql=@sql+' , max(case 课程 when '''+ 课程 +''' then 分数 else 0 end) ['+ 课程 +']'
from (selectdistinct 课程 from tb) as a
set@sql=@sql+' from tb group by 姓名'
exec(@sql) 
--SQL SERVER 2005 静态SQL。
select*from (select*from tb) a pivot (max(分数) for 课程 in (语文,数学,物理)) b
--SQL SERVER 2005 动态SQL。
declare@sqlvarchar(8000)
select@sql=isnull(@sql+'],[' , '') + 课程 from tb groupby 课程
set@sql='['+@sql+']'
exec ('select * from (select * from tb) a pivot (max(分数) for 课程 in ('+@sql+')) b')
