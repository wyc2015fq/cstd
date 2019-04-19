# sql中的 between and 用法 - 数据之美的博客 - CSDN博客
2018年01月21日 11:50:35[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：11401
# BETWEEN
指定测试范围。
##### 语法
*test_expression* [ NOT ] BETWEEN *begin_expression* AND* end_expression*
##### 参数
*test_expression*
是用来在由 *begin_expression *和 *end_expression *定义的范围内进行测试的表达式。*test_expression *必须与 *begin_expression *和 *end_expression *具有相同的数据类型。
NOT
指定谓词的结果被取反。
*begin_expression*
是任何有效的 Microsoft® SQL Server™ 表达式。*begin_expression *必须与 *test_expression *和 *end_expression *具有相同的数据类型。
*end_expression*
是任何有效的 SQL Server 表达式。*end_expression *必须与 *test_expression *和 *begin_expression *一样具有相同的数据类型。
AND
作为一个占位符，表示 *test_expression *应该处于由 *begin_expression *和 *end_expression *指定的范围内。
##### 结果类型
Boolean
##### 结果值
如果 *test_expression *的值大于或等于 *begin_expression *的值并且小于或等于 *end_expression *的值，则 BETWEEN 返回 TRUE。
如果 *test_expression *的值小于 *begin_expression *的值或者大于 *end_expression *的值，则 NOT BETWEEN 返回 TRUE。
##### 注释
若要指定排除范围，请使用大于 (>) 和小于 (<) 运算符。如果任何 BETWEEN 或 NOT BETWEEN 谓词的输入为 NULL，则结果是 UNKNOWN。
##### 示例
###### A. 使用 BETWEEN
本例返回书的题头标识符，这些书的年度至今单位销售额是从 4,095 到 12,000。
USE pubs
GO
SELECT title_id, ytd_sales
FROM titles
WHERE ytd_sales BETWEEN 4095 AND 12000
GO
下面是结果集：
title_id ytd_sales 
-------- ----------- 
BU1032   4095        
BU7832   4095        
PC1035   8780        
PC8888   4095        
TC7777   4095        
(5 row(s) affected)
###### B. 使用 > 和 < 代替 BETWEEN
本例使用大于 (>) 和小于 (<) 运算符，由于这些运算符是非包含的，所以返回不同的结果。
USE pubs
GO
SELECT title_id, ytd_sales      
FROM titles      
WHERE ytd_sales > 4095 AND ytd_sales < 12000      
GO
下面是结果集：
title_id ytd_sales   
-------- ----------- 
PC1035   8780        
(1 row(s) affected)
###### C. 使用 NOT BETWEEN
本例找出一个指定范围（从 4,095 到 12,000）外的所有行。
USE pubs
GO
SELECT title_id, ytd_sales
FROM titles
WHERE ytd_sales NOT BETWEEN 4095 AND 12000
GO
下面是结果集：
title_id ytd_sales   
-------- ----------- 
BU1111   3876        
BU2075   18722       
MC2222   2032        
MC3021   22246       
PS1372   375         
PS2091   2045        
PS2106   111         
PS3333   4072        
PS7777   3336        
TC3218   375         
TC4203   15096       
