# mysql在数据库表中查询表名和字段名 - fanyun的博客 - CSDN博客
2016年03月18日 14:05:55[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：5546
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
1.第一种查询方法：
     SELECT table_name, column_name from information_schema.columns WHERE column_name LIKE 'Name';
2.第二种查询方法：
    SELECT column_name from information_schema.columns WHERE column_name LIKE ’%searchTerm%’ AND table_schema = ‘DateDB’
3.第三种查询方法：
    SELECT column_name from information_schema.columns WHERE column_name LIKE ’%searchTerm%’ AND table_schema = ‘DateDB’ AND table_name = ‘DBTable’
