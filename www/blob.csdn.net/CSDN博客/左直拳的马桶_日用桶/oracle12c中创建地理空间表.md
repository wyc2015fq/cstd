# oracle12c中创建地理空间表 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月16日 13:41:02[左直拳](https://me.csdn.net/leftfist)阅读数：149
注意，是空间表，而不是“表空间”。空间表跟地理信息、地图之类有关。
作为一个GIS开发小白（目前，我是JAVA小白，GIS小白，架构小白，……，好多东西都不懂啊。），记录步骤如下：
**一、oracle数据库中操作**
**1、创建一个表空间**，比如，名曰：SDE
```
create tablespace sde datafile 'E:\app\LTHY\PDBRADAR\sde.ora' size 500m autoextend on;
create user sde identified by sde default tablespace sde quota unlimited on sde;
--然后赋各种权限
grant connect, resource to sde; 
grant CREATE SESSION to sde; 
grant CREATE TABLE to sde; 
grant CREATE PROCEDURE to sde; 
grant CREATE SEQUENCE to sde; 
grant CREATE TRIGGER to sde; 
grant CREATE TYPE to sde; 
grant CREATE LIBRARY to sde; 
grant CREATE PUBLIC SYNONYM to sde; 
grant CREATE OPERATOR to sde; 
grant CREATE INDEXTYPE to sde; 
grant DROP PUBLIC SYNONYM to sde; 
grant CREATE VIEW to sde; 
grant UNLIMITED TABLESPACE to sde; 
grant ADMINISTER DATABASE TRIGGER to sde;
Grant execute on dbms_pipe to sde;
Grant execute on dbms_lock to sde;
grant inherit privileges on user system to  sde;
```
**2、设置DLL**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190117090539312.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
选中目标数据库(用sde登录)，编辑其Libraries的ST_SHAPELIB,使其指向oracle12c相应的dll,如：
```
D:\app\Administrator\product\12.1.0\dbhome_1\BIN\st_shapelib.dll
```
但有时这样写上也不行，运行`select st_astext(shape) from 某个空间表`，报错。尝试将ST_SHAPELIB指向路径删掉，按“应用”，这时路径恢复，再运行SQL语句，正常了。
**二、在arcgis catalog上操作**
arcgis是一款著名的地图软件。老美的。oracle并不天然支持地理信息存储，所以最好用arcgis catalog创建空间表。”除非你相当熟悉，否则不要直接在oracle上创建，否则到时这里那里突然会出点啥问题，谁都说不好。”，我同事说。
**1、打开ArcGIS catalog，连接ORACLE上的目标数据库**；用sde进行登录
**2、创建企业级地理数据库**
在ArcGIS catalog中，选中目标数据库，打开ArcToolbox(工具栏中红色图标),
```
数据库管理工具 - 地理数据库管理 - 创建企业级地理数据库
```
如提示 “ORA-04043: 对象 INSTANCES_UTIL 不存在”，可关闭arggis catalog再打开，完成后重启ArcGIS catalog。
这时打开oracle，SDE表空间下，多了许多表。
**3、创建空间表**
1）鼠标右键，新建 - 要素类
在这里，要素类就是空间表，而表就是我们常说的数据表，普通表，也称为“属性表”。
2）存储的要素类型，看需要，无非就是点、线、面之类。如果我们这个表，是想存储对象的坐标，应该选点。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190116115852620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
3）然后默认一步步继续
系统默认会生成俩字段OBJECTID，和空间字段SHAPE，前者是主键，后者就是对应的点、线、面之类，不能修改，但别名可以改，但为慎重起见，也不改。
如此，空间表可生成矣。这时我们在oracle中打开这张表，看其结构，SHAPE的字段类型是：ST_GEOMETRY。
