# 利用DB Link两步搞定Oracle两个数据库间的表同步 - z69183787的专栏 - CSDN博客
2014年03月10日 16:46:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5960
1，在目标机上建立Oracle DB Link：
A，在network/admin/tnsname.ora文件中加入源库的连接信息，如：
AAA=
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST =10.5.1.3)(PORT =1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = AAA)
    )
  )
B，在目标机上用sqlplus user/pwd登录。
C，用如下命令建立DB Link:
create public database link AAA_LINK connect to user identified by pwd using 'AAA';
命令说明：
CREATE PUBLIC DATABASE LINK 数据库链接名 CONNECT TO 用户名 IDENTIFIED BY 密码 USING ‘本地配置的数据的实例名’;
如果建立成功，会提示：Database link created.
2，使用如下脚本，即可同步数据表：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)#!/bin/sh
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)# to sync table A to BBB database from AAA database
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)sqlplus user/pwd@BBB<<EOF#这里是BBB上的数据库和密码还有实例名，请按照实际情况修改
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)truncate table A;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)insert into A 
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)select* from b_schema.A@AAA_LINK; #这里是指向要同步的来源表,表名必须是<表所有者>.<表名>@<dblink name>
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)commit;
PS：需要DB支持Advanced replication功能，是否支持，可用如下SQL查看：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)select*from v$optionwhere PARAMETER='Advanced replication';
如果是返回True就表示支持。
