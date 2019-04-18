# MySQL Reference Manual中文版 - 深之JohnChen的专栏 - CSDN博客

2007年09月02日 04:48:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4620


[<< Back to man.ChinaUnix.net](http://man.chinaunix.net/)

# MySQL Reference Manual for version 4.1.0-alpha.

[译者序](http://man.chinaunix.net/database/mysql/zh-4.1.0/about.html)

[列类型与函数索引](http://man.chinaunix.net/database/mysql/zh-4.1.0/function_index.html)
- [6 MySQL 语言参考](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html)- [6.1 语言结构](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html)- [6.1.1 文字：怎么写字符串与数字](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Literals)- [6.1.1.1 字符串](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#String_syntax)- 

- [6.1.1.2 数字](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Number_syntax)- 

- [6.1.1.3 十六进制值](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Hexadecimal_values)- 

- [6.1.1.4 `NULL` 值](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#NULL_values)- 

- [6.1.2 数据库、表、索引、列和别名](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Legal_names)- 

- [6.1.3 名字的大小写敏感性](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Name_case_sensitivity)- 

- [6.1.4 用户变量](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Variables)- 

- [6.1.5 系统变量](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#System_Variables)- 

- [6.1.6 注释句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Comments)- 

- [6.1.7 MySQL 对保留字挑剔吗？](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-1.html#Reserved_words)- 

- [6.2 列类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html)- [6.2.1 数字类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#Numeric_types)- 

- [6.2.2 Date 和 Time 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#Date_and_time_types)- [6.2.2.1 Y2K 问题和日期类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#Y2K_issues)- 

- [6.2.2.2 DATETIME、DATE 和 TIMESTAMP 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#DATETIME)- 

- [6.2.2.3 TIME 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#TIME)- 

- [6.2.2.4 YEAR 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#YEAR)- 

- [6.2.3 字符串类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#String_types)- [6.2.3.1 CHAR 和 VARCHAR 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#CHAR)- 

- [6.2.3.2 BLOB 和 TEXT 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#BLOB)- 

- [6.2.3.3 ENUM 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#ENUM)- 

- [6.2.3.4 SET 类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#SET)- 

- [6.2.4 为列选择正确的类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#Choosing_types)- 

- [6.2.5 使用来自其它的数据库引擎的列类型](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#Other-vendor_column_types)- 

- [6.2.6 列类型存储需求](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#Storage_requirements)- [6.2.6.1 数字类型存储需求](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#IDX1093)
- [6.2.6.2 日期和时间类型存储需求](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#IDX1095)
- [6.2.6.3 字符串类型存储需求](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-2.html#IDX1099)

- [6.3 用于 SELECT 和 WHERE 子句的函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html)- [6.3.1 无类型的特殊运算符和函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Non-typed_Operators)- [6.3.1.1 圆括号](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Parentheses)- 

- [6.3.1.2 比较运算符](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Comparison_Operators)- 

- [6.3.1.3 逻辑运算符](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Logical_Operators)- 

- [6.3.1.4 控制流函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Control_flow_functions)- 

- [6.3.2 字符串函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#String_functions)- [6.3.2.1 字符串比较函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#String_comparison_functions)- 

- [6.3.2.2 字母大小写敏感性](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Case_Sensitivity_Operators)- 

- [6.3.3 数字函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Numeric_Functions)- [6.3.3.1 算术运算](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Arithmetic_functions)- 

- [6.3.3.2 数学函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Mathematical_functions)- 

- [6.3.4 日期和时间函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Date_and_time_functions)- 

- [6.3.5 Cast 函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Cast_Functions)- 

- [6.3.6 其它函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Other_Functions)- [6.3.6.1 位函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Bit_functions)- 

- [6.3.6.2 辅助功能函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Miscellaneous_functions)- 

- [6.3.7 用于 GROUP BY 子句的函数](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-3.html#Group_by_functions)- 

- [6.4 数据操纵：SELECT, INSERT, UPDATE, DELETE](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html)- [6.4.1 SELECT 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#SELECT)- [6.4.1.1 JOIN 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#JOIN)- 

- [6.4.1.2 UNION 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#UNION)- 

- [6.4.2 HANDLER 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#HANDLER)- 

- [6.4.3 INSERT 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#INSERT)- [6.4.3.1 INSERT ... SELECT 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#INSERT_SELECT)- 

- [6.4.4 INSERT DELAYED 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#INSERT_DELAYED)- 

- [6.4.5 UPDATE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#UPDATE)- 

- [6.4.6 DELETE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#DELETE)- 

- [6.4.7 TRUNCATE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#TRUNCATE)- 

- [6.4.8 REPLACE句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#REPLACE)- 

- [6.4.9 LOAD DATA INFILE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#LOAD_DATA)- 

- [6.4.10 DO 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-4.html#DO)- 

- [6.5 数据定义： CREATE、DROP、ALTER](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#Data_Definition)- [6.5.1 CREATE DATABASE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#CREATE_DATABASE)- 

- [6.5.2 DROP DATABASE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#DROP_DATABASE)- 

- [6.5.3 CREATE TABLE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#CREATE_TABLE)- [6.5.3.1 隐式的列定义变化](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#Silent_column_changes)- 

- [6.5.4 ALTER TABLE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#ALTER_TABLE)- 

- [6.5.5 RENAME TABLE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#RENAME_TABLE)- 

- [6.5.6 DROP TABLE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#DROP_TABLE)- 

- [6.5.7 CREATE INDEX 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#CREATE_INDEX)- 

- [6.5.8 DROP INDEX 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-5.html#DROP_INDEX)- 

- [6.6 基本的 MySQL 用户实用命令](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-6.html)- [6.6.1 USE 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-6.html#USE)- 

- [6.6.2 DESCRIBE 句法 (获得列的信息)](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-6.html#DESCRIBE)- 

- [6.7 MySQL 事务与锁定命令](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-7.html)- [6.7.1 BEGIN/COMMIT/ROLLBACK 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-7.html#COMMIT)- 

- [6.7.2 LOCK TABLES/UNLOCK TABLES 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-7.html#LOCK_TABLES)- 

- [6.7.3 SET TRANSACTION 句法](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-7.html#SET_TRANSACTION)- 

- [6.8 MySQL 全文搜索](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-8.html)- [6.8.1 全文的限制](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-8.html#Fulltext_Restrictions)- 

- [6.8.2 微调 MySQL 全文搜索](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-8.html#Fulltext_Fine-tuning)- 

- [6.8.3 全文搜索 TODO](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-8.html#Fulltext_TODO)- 

- [6.9 MySQL 查询缓存](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-9.html)- [6.9.1 查询缓存如何运作](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-9.html#Query_Cache_How)- 

- [6.9.2 查询缓存设置](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-9.html#Query_Cache_Configuration)- 

- [6.9.3 在 SELECT 中的查询缓存选项](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-9.html#Query_Cache_in_SELECT)- 

- [6.9.4 查询缓存的状态和维护](http://man.chinaunix.net/database/mysql/zh-4.1.0/06-9.html#Query_Cache_Status_and_Maintenance)

