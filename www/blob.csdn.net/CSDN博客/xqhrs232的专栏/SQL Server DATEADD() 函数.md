# SQL Server DATEADD() 函数 - xqhrs232的专栏 - CSDN博客
2018年08月23日 16:50:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：52
原文地址::[http://www.w3school.com.cn/sql/func_dateadd.asp](http://www.w3school.com.cn/sql/func_dateadd.asp)
# SQL Server DATEADD() 函数
[SQL Server Date 函数](http://www.w3school.com.cn/sql/sql_dates.asp)
## 定义和用法
DATEADD() 函数在日期中添加或减去指定的时间间隔。
### 语法
DATEADD(*datepart*,*number*,*date*)
*date* 参数是合法的日期表达式。*number* 是您希望添加的间隔数；对于未来的时间，此数是正数，对于过去的时间，此数是负数。
*datepart* 参数可以是下列的值：
|datepart|缩写|
|----|----|
|年|yy, yyyy|
|季度|qq, q|
|月|mm, m|
|年中的日|dy, y|
|日|dd, d|
|周|wk, ww|
|星期|dw, w|
|小时|hh|
|分钟|mi, n|
|秒|ss, s|
|毫秒|ms|
|微妙|mcs|
|纳秒|ns|
## 实例
假设我们有下面这个 "Orders" 表：
|OrderId|ProductName|OrderDate|
|----|----|----|
|1|'Computer'|2008-12-29 16:25:46.635|
现在，我们希望向 "OrderDate" 添加 2 天，这样就可以找到付款日期。
我们使用如下 SELECT 语句：
SELECT OrderId,`DATEADD(day,2,OrderDate)` AS OrderPayDate
FROM Orders
结果：
|OrderId|OrderPayDate|
|----|----|
|1|2008-12-31 16:25:46.635|
