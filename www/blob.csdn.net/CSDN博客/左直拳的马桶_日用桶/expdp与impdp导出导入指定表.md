# expdp与impdp导出导入指定表 - 左直拳的马桶_日用桶 - CSDN博客
2018年06月19日 19:41:43[左直拳](https://me.csdn.net/leftfist)阅读数：10079
oracle里指定导入导出表，原本在10g或以前，很简单的：
**一、10g或以前**
```
1、导出指定表
exp 'sys/pwd@server1 as sysdba' file=c:\temp\tables.dmp tables=(schema1.table1,schema1.table2)
2、导入指定表
imp 'sys/pwd@server2 as sysdba' file=c:\temp\tables.dmp fromuser=schema1 touser=schema1 tables=(table1,table2) ignore=Y
```
**二、11g或12c**
但12C以后，似乎就不支持这种写法了。可以这样写：
```
1、导出指定表
expdp 'sys/pwd@server1 as sysdba' directory=dbbak dumpfile=tables.dmp logfile=tables.log tables=schema1.table1,schema1.table2
2、导入指定表
--如果源库和目标库对应的表空间没变：
impdp 'sys/pwd@server2 as sysdba' directory=dbbak dumpfile=tables.dmp tables=schema1.table1,schema1.table2  REMAP_SCHEMA=schema1:schema1
--REMAP_SCHEMA=schema1:schema1，源库shema:目标库schema
--如果源库和目标库对应的表空间不一样：
impdp 'sys/pwd@server2 as sysdba' directory=dbbak dumpfile=tables.dmp tables=schema1.table1,schema1.table2  remap_schema=schema1:schema2 remap_tablespace=tablespace1:tablespace2 
--remap_schema=schema1:schema2，源库shema:目标库schema
--remap_tablespace=tablespace1:tablespace2，源表空间：目标表空间
```
注意目标库的schema对应的账号，因为可能涉及到创建表等各种元素，要有足够的权限，才能导进去
其中，dbbak没有的话，要先创建：
在sqlplus下：
```
create directory dbbak as 'c:\temp';--（手动创建temp文件夹）
grant read,write on directory dbbak to public;
```
这个dbbak是个啥东东呢？据说数据库某种程度上类似一个操作系统，它有自己的一套磁盘管理机制，一般不直接使用操作系统的文件系统。甚至乎，它希望直接使用“生磁盘”，就是没有格式化过的磁盘。所以，dbbak是一个磁盘路径映射，要将操作系统下的路径映射到oracle里，才能使用。
以上这个expdp，impdp导出导入指定表，网上搜了之后，发觉很少有直接能使用的例子，我这两个还是综合起来，经过一些测试才通过的。
另外，如果操作系统是WIN2012或以上操作系统，你在power shell里运行以上代码，是不行的。一定要在DOS命令行窗口才可以。似乎这个power shell还停留在oracle 10g时代，一点也不power。
