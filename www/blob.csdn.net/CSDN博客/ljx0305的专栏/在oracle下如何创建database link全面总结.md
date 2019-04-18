# 在oracle下如何创建database link全面总结 - ljx0305的专栏 - CSDN博客
2009年04月14日 10:15:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：955标签：[database																[oracle																[数据库																[网络																[数据库服务器																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=database&t=blog)
个人分类：[Oracle体系结构](https://blog.csdn.net/ljx0305/article/category/428729)
**在oracle下如何创建database link全面总结**
物理上存放于网络的多个ORACLE数据库，逻辑上可以看成一个单一的大型数据库，用户可以通过网络对异地数据库中的数据进行存取，而服务器之间的协同处理对于工作站用户及应用程序而言是完全透明的，开发人员无需关心网络的链接细节、数据在网络节点中的具体分布情况和服务器间的协调工作过程。 
数据库之间的链接建立在DATABASE LINK上。要创建一个DATABASE LINK，必须首先在建立链接的数据库上设置链接字符串，即配置一个远程数据库的本地网络服务名。 
链接字符串的配置参见《客户端连接服务器》一节。 
数据库全局名称可以用以下命令查出：
SELECT * FROM GLOBAL_NAME;
修改可以用以下语句来修改参数值：
ALTER SYSTEM SET GLOBAL_NAME=TRUE/FALSE;
<1>、当数据库参数global_name=false时，就不要求数据库链接名称跟远端数据库名称一样。
oracle数据库之间进行连接通讯。
创建数据库链接的语法如下： 
CREATE [PUBLIC] DATABASE LINK link 
CONNECT TO username IDENTIFIED BY password 
USING ‘connectstring’ 
其中：
--demona为用net8 easy config创建的连接字符串
目的方数据库的init.ora文件中的global_names设为false
重新启动数据库设置才能生效
或者用sys用户执行
注意：创建数据库链接的帐号必须有CREATE DATABASE LINK或CREATE PUBLIC DATABASE LINK的系统权限，用来登录到远程数据库的帐号必须有CREATE SESSION权限。这两种权限都包含在CONNECT角色中（CREATE PUBLIC DATABASE LINK权限在DBA中）。 
一个公用数据库链接对于数据库中的所有用户都是可用的，而一个私有链接仅对创建它的用户可用。由一个用户给另外一个用户授权私有数据库链接是不可能的，一个数据库链接要么是公用的，要么是私有的。 
创建数据库链接时，还可以使用缺省登录方式，即不指定远程数据库的用户名和密码： 
create public database link zrhs_link 
using ‘zrhs’; 
在不指定用户名和口令的情况下，ORACLE使用当前的用户名和口令登录到远程数据库。 
USING后面指定的是链接字符串，也就是远程数据库的网络服务名，这个服务名保存在TNSNAMES.ORA文件中，在该文件中定义了协议、主机名、端口和数据库名。 
 （如果有两台数据库服务器db_A和db_B，db_A下用户user_a需要用database link访问到db_B下user_b的数据时，有如下步骤：1、首先用户user_b，这个用户必须要有select 权限.2、在db_B上的tnsnames.ora中需要建立一个连接字符串tnsname_BtoA，可以从db_B连接到db_A。3、在db_A上的tnsnames.ora中需要建立一个连接字符串tnsname_AtoB，可以从db_A连接到db_B。4、然后在登陆到db_A上建立db link，格式如下：  create database link link_AtoB_name connect to user_b identified by  user_b的password using 'tnsname_AtoB'; link_AtoB_name必须是远程数据库的全局名(SID), tnsname_AtoB就是到远程的连接字符串。5、测试是否能够读取数据。select * from dual@link_AtoB_name结果为：  DUMMY  X表示设置成功。）
删除数据库链接的语句是： 
DROP [PUBLIC] DATABASE LINK zrhs_link 
数据库链接的引用 
一般情况下引用数据库链接，可以直接将其放到调用的表名或视图名称后面，中间使用一个 @ 作为分割符： 
SELECT * FROM [worker@zrhs_link](mailto:worker@zrhs_link);
对于经常使用的数据库链接，可以建立一个本地的同义词，方便使用： 
CREATE SYNONYM worker_syn FOR [worker@zrhs_link](mailto:worker@zrhs_link);
还可以建立一个本地的远程视图，方便使用： 
CREATE VIEW worker AS SELECT * FROM [worker@zrhs_link](mailto:worker@zrhs_link) where… ;
现在本视图可与本地数据库中的任何其它视图一样对待，也可以授权给其它用户，访问此视图，但该用户必须有访问数据库链接的权限。 
对于另外一种情况，所要访问的表不在数据库链接中指定的远程帐户下，但该帐户有访问该表的权限，那么我们在表名前要加上该表的用户名： 
SELECT * FROM [camel.worker@zrhs_link](mailto:worker@zrhs_link) ;
<2>、当数据库参数global_name=true时，那就要求数据库链接名称跟远端数据库名称一样
数据库全局名称可以用以下命令查出
SELECT * FROM GLOBAL_NAME;
atabase link(dblink)--数据库链路
创建dblink的语法：
sql>create database link 数据库链路名
connect to 用户名 identified by 口令
using '主机字符串名';
如：
sql>create database link ora9i.us.oracle.com ### 这里的us.oracle.com为oracle默认域名 ###
connect to scott identified by tiger
using 'sun';
1)dblink名必须与远程数据库的全局数据库名（global_name）相同；
2)用户名，口令为远程数据库用户名，口令;
3)主机字符串为本机tnsnames.ora中定义的串;
4)两个同名的数据库间不得建立dblink;
然后，你就可以通过dblink访问远程数据库了。
如：
sql>select * from 表名@ora9i.us.oracle.com;
还可以建立快照(snapshot)通过dblink实现远程数据自动传输。
查看所有的数据库链接，进入系统管理员SQL>操作符下，运行命令： 
SQL>select owner,object_name from dba_objects where object_type='DATABASE LINK';
引用：[http://liumanyun.blog.hexun.com/5963987_d.html](http://liumanyun.blog.hexun.com/5963987_d.html)
