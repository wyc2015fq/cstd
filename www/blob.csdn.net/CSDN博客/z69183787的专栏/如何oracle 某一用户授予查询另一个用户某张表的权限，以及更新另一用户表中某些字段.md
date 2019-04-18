# 如何oracle 某一用户授予查询另一个用户某张表的权限，以及更新另一用户表中某些字段 - z69183787的专栏 - CSDN博客
2013年12月27日 16:52:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21240
create user dwtest identified by ST/G00100000161;
grant connect to dwtest;
grant select on stpt.dw_metro_production  to dwtest;
Ex:
select * from stpt.dw_metro_production t  
查看该表 表结构
select * from user_tab_columns where Table_Name='用户表'; 
select * from all_tab_columns where Table_Name='用户表'; 
select * from dba_tab_columns where Table_Name='用户表'; 
sqlplus userA
grant select on userA.the_table to userB;
grant update (the_column) on the_table to userB;
1。查看表结构：desc表名
2。查看当前用户的表：
select table_name from user_tables;
3.查看所有用户的表名：
select table_name from all_tables;
4.查看所有表名（其中包括系统表）
select table_name from dba_tables;
5.查看所有的表: 
select * from tab/dba_tables/dba_objects/cat;
