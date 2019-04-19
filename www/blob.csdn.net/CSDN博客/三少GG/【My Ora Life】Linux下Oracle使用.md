# 【My Ora Life】Linux下Oracle使用 - 三少GG - CSDN博客
2011年06月23日 17:09:00[三少GG](https://me.csdn.net/scut1135)阅读数：960标签：[oracle																[linux																[database																[数据库																[bbs](https://so.csdn.net/so/search/s.do?q=bbs&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=database&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
**启动EM控制台**
su -oracle
**ORACLE_HOME/bin/emctl start dbconsole (就可以访问https://localhost:1158/em的企业控制台)ORACLE_HOME/bin/lsnrctl start**
安装完毕oracle以后，需要创建oracle系统用户，并在/home/oracle下面的.bash_profile添加几个环境变量：ORACLE_SID,ORACLE_BASE,ORACLE_HOME。比如：
export ORACLE_SID=test  export ORACLE_BASE=oracle_install_dir export ORACLE_HOME=xxx
启动步骤：注意$代表shell命令提示符，这里的oracle是9.0以上版本。
$ su - oracle
**$ sqlplus /nologsql> connect / as sysdbasql> startup (一般不需要加参数，只要设置好环境变量）sql> quit (退出sql模式)**
$ lsnrctl start (启动监听器）
$ lsnrctl stop(关闭监听器，在这之前，应该先关闭应用程序）
$ sqlplus  /nolog
sql>**shutdown +**其参数 ：shutdown有四个参数，四个参数的含义如下：
Normal 需要等待所有的用户断开连接
**Immediate 等待用户完成当前的语句**
Transactional 等待用户完成当前的事务
Abort 不做任何等待，直接关闭数据库
normal需要在所有连接用户断开后才执行关闭数据库任务，所以有的时候看起来好象命令没有运行一样！在执行这个命令后不允许新的连接
immediate在用户执行完正在执行的语句后就断开用户连接，并不允许新用户连接。
transactional 在拥护执行完当前事物后断开连接，并不允许新的用户连接数据库。
abort 执行强行断开连接并直接关闭数据库。
前三种方式不回丢失用户数据。第四种在不的已的情况下，不建议采用！
经常遇到的问题：
1）权限问题，解决方法，切换到oracle用户；
2）没有关闭监听器 ，解决方法：关闭监听器
3）有oracle实例没有关闭，解决办法：关闭oracle实例
4）环境变量设置不全，解决办法：修改环境变量
/////////////////////////////////////////////////////////////////////
ORA-12162: TNS:net service name is incorrectly specified 问题 
没有正确设置ORACLE_SID环境变量
关闭：
emctl stop dbconsole
lsnrctl stop
++++++++++++++++++++++++++++++
linux下oracle启动 
1.su oracle 
2.sqlplus /logon 
3.connect test/test as sysdba(test/test是oracle用户和密码) 
4.startup 
5.lsnrctl
首选启动数据库 
su - oracle 
sqlplus /nolog 
conn /as sysdba 
startup 
然后启动监听: 
进入/opt/oracle/product/9.2.0/bin/ 
lsnrctl start
+++++++++++++++++++++++++++++++++++++++++++++++++++++
<logon> is: (<username>[/<password>][@<connect_identifier>] | /)
              [AS SYSDBA | AS SYSOPER | AS SYSASM] | /NOLOG | [EDITION=value]
    Specifies the database account username, password and connect
    identifier for the database connection.  Without a connect
    identifier, SQL*Plus connects to the default database.
    The AS SYSDBA, AS SYSOPER and AS SYSASM  options are database
    administration privileges.
    Example: Connect to database using Net Service Name and the
                 database net service name is ORCL.
           sqlplus [myusername/mypassword@ORCL](mailto:myusername/mypassword@ORCL)
[](mailto:myusername/mypassword@Host/ORCL)
    The /NOLOG option starts SQL*Plus without connecting to a
    database.
+++++++++++++++++++++++++++++++++++++++++++++++
Oracle 常用操作命令
1、su – oracle 不是必需，适合于没有DBA密码时使用，可以不用密码来进入sqlplus界面。
2、sqlplus /nolog 或sqlplus system/manager 或./sqlplus system/manager@ora9i;
3、SQL>connect / as sysdba ;（as sysoper）或connect internal/oracle AS SYSDBA ;(scott/tiger) conn sys/change_on_install as sysdba;
4、SQL>startup; 启动数据库实例
5、 查看当前的所有数据库: select * from v$database;
      select name from v$database;
      desc v$databases; 查看数据库结构字段
7、怎样查看哪些用户拥有SYSDBA、SYSOPER权限:
     SQL>select * from V_$PWFILE_USERS;
     Show user;查看当前数据库连接用户
8、进入test数据库：database test;
9、查看所有的数据库实例：select * from v$instance；
     如：ora9i
10、查看当前库的所有数据表：
    SQL> select TABLE_NAME from all_tables;
    select * from all_tables;
    SQL> select table_name from all_tables where table_name like ‘u’;
    TABLE_NAME———————————————default_auditing_options
11、查看表结构：desc all_tables;
12、显示CQI.T_BBS_XUSER的所有字段结构：
      desc CQI.T_BBS_XUSER;
13、获得CQI.T_BBS_XUSER表中的记录：
      select * from CQI.T_BBS_XUSER;
14、增加数据库用户：(test11/test)
     create user test11 identified by test default tablespace users Temporary TABLESPACE Temp;
15、用户授权:
     grant connect,resource,dba to test11;
     grant sysdba to test11;
     commit;
16、更改数据库用户的密码：(将sys与system的密码改为test.)
alter user sys indentified by test;
alter user system indentified by test;
