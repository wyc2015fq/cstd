# Oracle创建用户、授权用户 - z69183787的专栏 - CSDN博客
2013年11月28日 12:42:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2086
1、Oracle用dbca新创建一个数据库，成功之后。
2、用system用户进入此数库管(toad)创建新用户，并授权。
脚本如下：
1、Oracle用dbca新创建一个数据库，成功之后。
2、用system用户进入此数库管(toad)创建新用户，并授权。
脚本如下：
默认的：最高权限用户：SYSTEM 密码：MANAGER   
          管理员权限用户：SYS     密码：CHANGE_ON_INSTALL
          普通用户：SCOTT           密码：TIGER   
登陆管理员或超级管理员用户可以自己建立属于自己的用户   ： 
命令：CREATE USER USERNAME IDENTIFIED BY PASSWORD;
创建用户名为： USERNAME, 密码为 PASSWORD 的用户
分配权限：
GRANT DBA TO USERNAME; --授予DBA权限
GRANT UNLIMITED TABLESPACE TO USERNAME;--授予不限制的表空间
GRANT SELECT ANY TABLE TO USERNAME; --授予查询任何表
GRANT SELECT ANY DICTIONARY TO USERNAME;--授予 查询 任何字典
