# Oracle的表空间、用户和用户授权 - 文章 - 伯乐在线
原文出处： [Sam Xiao](http://www.cnblogs.com/xcj26/p/3759167.html)
**一：Oracle表空间**
**1，Oracle表空间与SQL Server文件组对比**
SQL Server的文件组（文件组包括若干MDF,NDF数据文件）对我们来说并不陌生，前段时间我在博客“[怎样玩转千万级别的数据](http://blog.jobbole.com/47753/)”提到了过表分区的概念，其实表分区说到本质上就是按一定条件把不同数据放在不同的数据文件里，这些数据文件在物理上是相对独立的，在逻辑上又是一个整体。特别强调一下，在SQL Server中，建表时是默认把所有的表都保存在PRIMARY默认文件组 ，当数据库中表很多，并且数据量很大时，会导致数据库性能严重下降，对于一些大数据的处理，有必要将一些大表数据放到不同的文件组中，以提高数据库的性能。这也是优化SQL Server 数据库的一个重要手段。
从SQL Server转到Oracle也许会感到不适，因为Oracle里没有文件组和数据文件的概念。但是有表空间的概念，表空间是Oracle的逻辑结构，一个Oracle数据库可以有一个或者多个表空间，一个表空间又对应着多个物理的数据库文件。其实SQL Server的文件组转到Oracle就是表空间，Oracle的表空间转到SQL Server就是文件组。这篇博文重点来讨论Oracle的表空间。
**2，创建，修改，删除Oracle表空间**
Oracle的表空间分了几种类型，临时表空间，用户表空间，还原表空间，大文件表空间等，我选择临时表空间和用户表空间为例来演示怎么在Oracle中创建表空间，创建表空间，一般都是DBA干的事情，所以我选择sys的用户登录创建表空间。
2.1，创建临时表空间，用TEMPFILE 关键字，下边几个参数要重点注意一下。
SIZE *[K,M]表空间的初始大小，单位可以是[K,M]
MAXSIZE *[K,M]表空间的最大存储值，maxsize unlimited 是指表空间的大小不受限制。
AUTOEXTEND ON NEXT *[K,M] 设置表空间自动扩展
EXTENT MANAGEMENT LOCAL本地管理表空间。
extent是“区间”的意思,在oracle数据库中：extent management 有两种方式 extent management local（本地管理）; extent management dictionary（数据字典管
理）默认的是local本地管理表空间有自动分配(AUTOALLOCATE)和统一大小分配(UNIFORM)两种空间分配方式，自动分配方式(AUTOALLOCATE)是由系统来自动决定extents
大小，而统一大小分配(UNIFORM)则是由用户指定extents大小。这两种分配方式都提高了空间管理效率。uniform：默认为1M大小，在temp表空间里为默认的，但是不能被应用
在undo表空间.


Oracle PL/SQL
```
--创建
CREATE TEMPORARY TABLESPACE  tsTempTest
TEMPFILE 
'E:DataBasetsTempTest01.dbf' SIZE 64M AUTOEXTEND ON NEXT 32M MAXSIZE 1024M,
'E:DataBasetsTempTest02.dbf' SIZE 64M AUTOEXTEND ON NEXT 32M MAXSIZE 1024M,
'E:DataBasetsTempTest03.dbf' SIZE 64M AUTOEXTEND ON NEXT 32M MAXSIZE 1024M EXTENT MANAGEMENT LOCAL;
--增加
ALTER TABLESPACE tsTempTest 
ADD TEMPFILE
 'E:DataBasetsTempTest04.DBF' SIZE 64M AUTOEXTEND ON NEXT 32M maxsize unlimited;
--删除
DROP TABLESPACE tsTempTest INCLUDING CONTENTS AND DATAFILES CASCADE CONSTRAINTS;
```
2.2，创建用户表空间，参数与临时表空间基本一致


Oracle PL/SQL
```
--创建
CREATE TABLESPACE tsUserTest
datafile 
'E:DataBasetsUserTest01.dbf' size 64M AUTOEXTEND ON NEXT 32M maxsize unlimited ,
'E:DataBasetsUserTest02.dbf' size 64M AUTOEXTEND ON NEXT 32M maxsize unlimited ,
'E:DataBasetsUserTest03.dbf' size 64M  AUTOEXTEND ON NEXT 32M maxsize unlimited extent management local ;
--增加
ALTER TABLESPACE tsUserTest 
ADD DATAFILE
 'E:DataBasetsUserTest04.DBF' SIZE 64M AUTOEXTEND ON NEXT 32M maxsize unlimited;
--删除
DROP TABLESPACE tsUserTest INCLUDING CONTENTS AND DATAFILES CASCADE CONSTRAINTS;
```
**二：Oracle用户**
**1，创建用户：**
创建用户时，需要注意如下几个参数。
IDENTIFIED BY *** ，用户的密码。
DEFAULT TABLESPACE  *** 默认用户表空间。
TEMPORARY TABLESPACE *** 默认临时空间。


Oracle PL/SQL
```
CREATE USER xcj01 IDENTIFIED BY xcj01
DEFAULT TABLESPACE tsUserTest
TEMPORARY TABLESPACE tsTempTest;
```
**2，修改密码：**


Oracle PL/SQL
```
alter user xcj01
identified by xcj01;
```
**3，删除用户以及跟用户关联的对象：**


Oracle PL/SQL
```
drop user xcj01 CASCADE;
```
**三：Oracle用户授权**
**1，Oralce的特殊权限**
1.1，系统权限unlimited tablespace是隐含在dba, resource角色中的一个系统权限. 当用户得到dba或resource的角色时, unlimited tablespace系统权限也隐式受权给用户.
1.2，系统权限unlimited tablespace不能被授予role, 可以被授予用户.
1.3，系统权限unlimited tablespace不会随着resource, dba被授予role而授予给用户.
**2，特殊角色**
2.1，DBA角色，是授权数据库管理员的权限
2.2，CONNECT角色， 是授予最终用户的典型权利，最基本的  一个（CREATE SESSION）
2.3，RESOURCE角色，是授予开发人员的  默认有八个权限（CREATE SEQUENCE,CREATE TRIGGER,CREATE CLUSTER,CREATE PROCEDURE,CREATE TYPE,CREATE OPERATOR,CREATE TABLE,CREATE INDEXTYPE）
2.4，exp_full_database角色，拥有导出数据库的权限
2.5，imp_full_database角色，拥有导入数据库的权限
**3，用户授权示**


Oracle PL/SQL
```
--授权
GRANT
CONNECT,                
RESOURCE,               
--DBA,                  
--unlimited tablespace,
CREATE  SESSION,         
CREATE ANY SEQUENCE,     
CREATE ANY TABLE,        
CREATE ANY VIEW ,        
CREATE ANY INDEX,        
CREATE ANY PROCEDURE,    
CREATE ANY DIRECTORY,    
ALTER  SESSION, 
ALTER ANY SEQUENCE,     
ALTER ANY TABLE,        
--ALTER ANY VIEW ,        --不能修改视图
ALTER ANY INDEX,        
ALTER ANY PROCEDURE,    
--ALTER ANY DIRECTORY,    --不能修改目录
--DROP  SESSION,       --不能删除Session
DROP ANY SEQUENCE,     
DROP ANY TABLE,        
DROP ANY VIEW ,        
DROP ANY INDEX,        
DROP ANY PROCEDURE,    
DROP ANY DIRECTORY,    
SELECT ANY TABLE, 
SELECT ANY DICTIONARY,
INSERT ANY TABLE, 
UPDATE ANY TABLE, 
DELETE ANY TABLE,
DEBUG ANY PROCEDURE,
DEBUG CONNECT SESSION,
exp_full_database,  
imp_full_database     
TO xcj01;
```
**四：查询用户的权限**
以下的查询中，基本都要以DBA的角色进行查询，如果权限不够，有可能查询不出来数据。
** 1，查看用户拥有那些角色**


Oracle PL/SQL
```
select * from dba_role_privs a where a.grantee='XCJ01';
--或
select * from dba_sys_privs a where a.grantee='XCJ01';
```
**2，查看角色拥有那些权限**


Oracle PL/SQL
```
select ROLE, PRIVILEGE from role_sys_privs where role='RESOURCE';   --RESOURCE，CONNECT，DBA
--或 
select grantee,privilege from dba_sys_privs where grantee='RESOURCE';
```
**3，为用户取消角色**


Oracle PL/SQL
```
revoke resource from XCJ01;
```
** 4，为用户取消权限**


Oracle PL/SQL
```
revoke unlimited tablespace from XCJ01;
```
**5，最后附加一个非常有用的技巧：查看Oracle的版本号。**


Oracle PL/SQL
```
select * from v$version where rownum 1;
```
**五：总结**
这些是Oracle的入门级知识，不是什么高深的理论，但是是经过项目的实践提炼，不管DBA还是开发人员都能用到，写出来与大家共勉。
**写到最后，你们也要给我动力，帮忙“赞”一下，“赞”的高尿的远！**
