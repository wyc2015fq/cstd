# order by 语句对null字段的默认排序 - z69183787的专栏 - CSDN博客
2017年04月12日 10:34:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3016
转自：[http://m.blog.csdn.net/article/details?id=6554168](http://m.blog.csdn.net/article/details?id=6554168)
在使用order by语句进行查询结果排序时，不同的数据库对于被排序字段数据行为null的情况，处理方式有所不同，这里我们主要考虑oracle和mysql数据库。
首先我们直接给出结论:
leeyee 写道
【Oracle 结论】 
order by colum asc 时，null默认被放在最后
order by colum desc 时，null默认被放在最前
nulls first 时，强制null放在最前，不为null的按声明顺序[asc|desc]进行排序
nulls last 时，强制null放在最后，不为null的按声明顺序[asc|desc]进行排序 
【MySql 结论】
order by colum asc 时，null默认被放在最前
order by colum desc 时，null默认被放在最后
ORDER BY IF(ISNULL(update_date),0,1) null被强制放在最前，不为null的按声明顺序[asc|desc]进行排序
ORDER BY IF(ISNULL(update_date),1,0) null被强制放在最后，不为null的按声明顺序[asc|desc]进行排序
 注：以上结果是在oracle11g及mysql5下的测试结果。
因此当我们在order by时，为了强制说明对于null的排序顺序就必须使用到：
针对【oracle】我们就需要使用以下语法：
`order by order_col [asc|desc] nulls [first|last]`
而针对【mysql】我们则需要使用以下语法：
`order by IF(ISNULL(my_field),1,0),my_field;`
下面在oracle11g下做个测试：
测试数据：
|rownum|create_date|update_date|
|----|----|----|
|1|20-3月 -11|18-6月 -11|
|2|20-4月 -11||
|3|20-5月 -11|20-6月 -11|
【无排序/默认排序】
`select update_date from table_name ; `
leeyee 写道
[结果]
1 18-6月 -11
2 
3 20-6月 -11
【asc排序】
`select update_date from table_name order by update_date; `
leeyee 写道
[结果]
1 20-6月 -11
2 18-6月 -11
3
【desc排序】
`select update_date from table_name order by update_date desc; `
leeyee 写道
[结果]
1 
2 18-6月 -11
3 20-6月 -11
【asc排序，强制null放在最前】
`select update_date from table_name order by update_date nulls first; `
leeyee 写道
[结果]
1 
2 20-6月 -11
3 18-6月 -11
【asc排序，强制null放在最后】
`select update_date from table_name order by update_date nulls last; `
leeyee 写道
[结果]
1 20-6月 -11
2 18-6月 -11
3
mysql5.0测试
`select update_date from table_name order by IF(ISNULL(update_date),0,1),update_date;`
