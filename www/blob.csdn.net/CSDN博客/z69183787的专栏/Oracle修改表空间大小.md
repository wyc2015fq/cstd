# Oracle修改表空间大小 - z69183787的专栏 - CSDN博客
2013年11月28日 13:24:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：817
查了很多资料发现原来是Oracle表空间限制，导致无法继续导入数据的原因。如果在建立数据库时没有设置，
Oracle 默认的表空间大小为400M，当数据库中数据量达到这个值，再向数据库中导入数据就会报错。解决方法是
扩展表空间。可以选择将表容量扩大，比如扩展到5G，或者当表空间不够时每次自动增加一定的容量,如每次自增200M。
下面列出详细过程：
1.通过sql plus 命令登录数据库。
　　在命令行下输入sqlplus “登录用户名/口令 as 登录类型”就可以登录，系统内建的用户名常用的是sys，密码是在安装oracle过程中设置的密码，清务必牢记，如果用sys帐户登录，登录类型一定要是sysdba。
2.查看各表空间分配情况。
select tablespace_name, sum(bytes) / 1024 / 1024  from dba_data_files  
 group by tablespace_name;  
3.查看各表空间空闲情况。
select tablespace_name, sum(bytes) / 1024 / 1024  from dba_free_space  group by tablespace_name;  
4.更改数据表大小(10G)
alter database datafile '/ora/oradata/radius/undo.dbf' resize 10240m；
5.设置表空间不足时自动增长
5.1查看表空间是否自动增长
SELECT FILE_NAME,TABLESPACE_NAME,AUTOEXTENSIBLE FROM dba_data_files;
5.2 设置表空间自动增长
ALTER DATABASE DATAFILE 'c:\SmartDB01.ora' AUTOEXTEND ON;//打开自动增长
ALTER DATABASE DATAFILE 'c:\SmartDB01.ora' AUTOEXTEND ON NEXT 200M ;//每次自动增长200m
ALTER DATABASE DATAFILE 'c:\SmartDB01.ora' AUTOEXTEND ON NEXT 200M MAXSIZE 1024M;//每次自动增长200m，数据表最大不超过1G
