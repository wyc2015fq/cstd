
# Oracle条件查询 - 李昆鹏的博客 - CSDN博客


2018年05月21日 15:37:58[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1907


-------------------------------------------------Oracle条件查询-------------------------------------------------
## . 限定查询
在查询绝大多数都会有条件的限制
语法：select *|列名 from表名where条件
例如：查询工资大于1500的所有雇员
![](https://img-blog.csdn.net/20180521153458226?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 1.  非空和空的限制
Ø  示例：查询每月能得到奖金的雇员
分析：只要字段中存在内容表示不为空，如果不存在内容就是null，
语法：列名 IS NOT NULL
为空  列名 IS NULL
![](https://img-blog.csdn.net/20180521153504904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018052115351690?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø  范例：查询工资大于1500并且有奖金领取的雇员
分析：多个查询条件同时满足之间使用‘AND’
![](https://img-blog.csdn.net/2018052115352386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø  范例:查询工资大于1500或者有奖金的雇员
分析：多个查询条件或满足，条件之间使用“OR”
![](https://img-blog.csdn.net/20180521153528539?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Ø  范例：查询工资不大于1500和没有奖金的人
语法：NOT(查询条件)
![](https://img-blog.csdn.net/2018052115353972?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：基本工资大于1500但是小于3000的全部雇员
分析：sal>1500, sal<3000
![](https://img-blog.csdn.net/20180521153549763?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Between and等于 sal > =1500 and sal <= 3000
![](https://img-blog.csdn.net/20180521153603457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询1981-1-1到1981-12-31号入职的雇员
分析：between and 不仅可以使用在数值之间，也可以用在日期的区间
![](https://img-blog.csdn.net/20180521153610977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例：查询雇员名字叫smith的雇员
在oracle中的查询条件中查询条件的值是区分大小写的
![](https://img-blog.csdn.net/20180521153619818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521153627254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
范例:查询雇员编号是7369,7499,7521的雇员编号的具体信息
如果使用之前的做法可以使用OR关键字
![](https://img-blog.csdn.net/20180521153633109?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
实际上，此时指定了查询范围，那么sql可以使用IN关键字
语法: 列名 IN (值1，值2，....)
列名 NOT IN (值1， 值2，...)
其中的值不仅可以是数值类型也可以是字符串
![](https://img-blog.csdn.net/20180521153642156?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在常用的站点中经常会有模糊查询，即：输入一个关键字，把符合的内容全部的查询出来，在sql中使用LIKE语句完成。
在LIKE中主要使用以下两种通配符
“%”：可以匹配任意长度的内容
“_”:可以匹配一个长度的内容
范例：查询出所有雇员姓名中第二个字符包含“M”的雇员
![](https://img-blog.csdn.net/20180521153654334?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在LIKE中如果没有关键字表示查询全部
![](https://img-blog.csdn.net/20180521153703200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
查询名字中带有“M”的雇员
![](https://img-blog.csdn.net/20180521153711484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在oracle中不等号的用法可以有两种形式“<>”和“!=”
范例：查询雇员编号不是7369的雇员信息
![](https://img-blog.csdn.net/20180521153718144?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521153724362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码示例：
|select|*|from|emp|where|sal>|1500|;|--|按指定的条件查询
|select|*|from|emp|where|comm|is|not|null|;|--|查询条件不是空，数值|0|不算空
|select|*|from|emp|where|comm|is|null|;|--|查询条件是空
|select|*|from|emp|where|sal>|1500|and|comm|is|not|null|;|--|查询多个条件都满足用|and|连接
|select|*|from|emp|where|sal>|1500|or|comm|is|not|null|;|--|查询多个条件满足其中一个条件用|or|连接
|select|*|from|emp|where|sal<=|1500|and|comm|is|not|null|;
|select|*|from|emp|where|not|(sal>|1500|and|comm|is|null|);|--not()|关键字代表查询不满足括号内的条件
|select|*|from|emp|where|sal>|1500|and|sal<|3000|;|--|在指定范围内部查询，不包含边界
|select|*|from|emp|where|sal|between|1500|and|3000|;|--|在指定范围内查询，包含边界
|select|*|from|emp|where|hiredate|between|to_date(|'1981-1-1'|,|'yyyy-MM-dd'|)
|and|to_date(|'1981-12-31'|,|'yyyy-MM-dd'|);|--|查询指定日期内的数据
|select|*|from|emp|where|ename=|'SMITH'|;|--sql|的语句是不区分大小写的，但是表示的值是区分大小写的
|select|*|from|emp|where|empno=|7369|or|empno=|7499|or|empno=|7521|;
|select|*|from|emp|where|empno|in|(|7369|,|7499|,|7521|);|--in()|关键字，代表查询条件为括号里面的数值
|select|*|from|emp|where|empno|not|in|(|7369|,|7499|,|7521|);|--not in()|查询条件不是括号内的数值
|--like|关键字代表模糊查询，|_|可以代表一个字符，|%|可以代表任意长度，这个句子代表查询名字中第二个字符为|M|的数据
|select|*|from|emp|where|ename|like|'_M%'|;
|select|*|from|emp|where|ename|like|'%M__'|--|查询倒数第二个字符为|M|的数据|;
|select|*|from|emp|where|ename|like|'%M%'|;|--|查询字符中含有|M|的数值
|select|*|from|emp|where|ename|like|'%%'|;|--|全匹配
|--  !=|和|<>|都是代表不等于
|select|*|from|emp|where|empno !=|7369|;
|select|*|from|emp|where|empno <>|7369|;
|


