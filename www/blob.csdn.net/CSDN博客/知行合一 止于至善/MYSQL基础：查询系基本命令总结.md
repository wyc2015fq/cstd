
# MYSQL基础：查询系基本命令总结 - 知行合一 止于至善 - CSDN博客

2019年03月09日 07:14:08[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：112标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)个人分类：[数据库																](https://blog.csdn.net/liumiaocn/article/category/6328292)



![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
进入mysql命令行的提示符之后，需要确认诸如版本和当前使用的数据库以及用户和使用的字符串编码格式等基本信息，这篇文章进行一下一些常用的查询系的命令的使用总结。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 版本信息确认
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)版本信息的获取有多种方式
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)方式1 : mysql命令行提示信息中获取
如下图所示，从mysql命令行提示符出来之前的提示信息中可以取到，同sqlplus一样。
\# mysql -uroot -pliumiaocnmysql:[Warning]Usinga passwordonthe command line interface can be insecure.Welcometothe MySQL monitor.Commandsendwith;or\g.Your MySQL connection idis26Server version:5.7.16-log MySQL Community Server(GPL)Copyright(c)2000,2016,Oracleand/orits affiliates.Allrights reserved.Oracleisa registered trademarkofOracle Corporationand/orits
affiliates.Other names may be trademarksoftheir respective
owners.Type'help;'or'\h'forhelp.Type'\c'toclear thecurrentinput statement.mysql>方式2: select version();
mysql>selectversion();+------------+|version()|+------------+|5.7.16-log|+------------+1rowinset(0.00sec)mysql>方式3: show variables like ‘version’;
mysql>showvariableslike'version';+---------------+------------+|Variable_name|Value|+---------------+------------+|version|5.7.16-log|+---------------+------------+1rowinset(0.01sec)mysql>数据库信息
查询当前的数据库实例（或者被成为schema）
mysql>showdatabases;+--------------------+|Database|+--------------------+|information_schema||mysql||performance_schema||sys|+--------------------+7rowsinset(0.00sec)mysql>决定使用某个数据库实例
mysql>usemysql
Readingtableinformationforcompletionoftableandcolumnnames
You can turnoffthis featuretoget a quicker startupwith-ADatabasechanged
mysql>这个用处在于使用表的时候是否还需要使用schema的名称,在使用的时候会自动的加上前缀，比如使用desc users命令时，mysql实际执行的则是desc mysql.users;
查询当前数据库实例名称
mysql>selectdatabase();+------------+|database()|+------------+|mysql|+------------+1rowinset(0.00sec)mysql>用户信息确认
确认当前用户信息
mysql>selectuser();+----------------+|user()|+----------------+|root@localhost|+----------------+1rowinset(0.00sec)mysql>确认用户详细信息
在mysql内置的mysql数据库实例中，通过user表的信息可以确认详细信息，包括权限。也可以使用show columns from mysql.user来达到同样效果。
mysql>descmysql.user;+------------------------+-----------------------------------+------+-----+-----------------------+-------+|Field|Type|Null|Key|Default|Extra|+------------------------+-----------------------------------+------+-----+-----------------------+-------+|Host|char(60)|NO|PRI||||User|char(32)|NO|PRI||||Select_priv|enum('N','Y')|NO||N|||Insert_priv|enum('N','Y')|NO||N|||Update_priv|enum('N','Y')|NO||N|||Delete_priv|enum('N','Y')|NO||N|||Create_priv|enum('N','Y')|NO||N|||Drop_priv|enum('N','Y')|NO||N|||Reload_priv|enum('N','Y')|NO||N|||Shutdown_priv|enum('N','Y')|NO||N|||Process_priv|enum('N','Y')|NO||N|||File_priv|enum('N','Y')|NO||N|||Grant_priv|enum('N','Y')|NO||N|||References_priv|enum('N','Y')|NO||N|||Index_priv|enum('N','Y')|NO||N|||Alter_priv|enum('N','Y')|NO||N|||Show_db_priv|enum('N','Y')|NO||N|||Super_priv|enum('N','Y')|NO||N|||Create_tmp_table_priv|enum('N','Y')|NO||N|||Lock_tables_priv|enum('N','Y')|NO||N|||Execute_priv|enum('N','Y')|NO||N|||Repl_slave_priv|enum('N','Y')|NO||N|||Repl_client_priv|enum('N','Y')|NO||N|||Create_view_priv|enum('N','Y')|NO||N|||Show_view_priv|enum('N','Y')|NO||N|||Create_routine_priv|enum('N','Y')|NO||N|||Alter_routine_priv|enum('N','Y')|NO||N|||Create_user_priv|enum('N','Y')|NO||N|||Event_priv|enum('N','Y')|NO||N|||Trigger_priv|enum('N','Y')|NO||N|||Create_tablespace_priv|enum('N','Y')|NO||N|||ssl_type|enum('','ANY','X509','SPECIFIED')|NO|||||ssl_cipher|blob|NO||NULL|||x509_issuer|blob|NO||NULL|||x509_subject|blob|NO||NULL|||max_questions|int(11)unsigned|NO||0|||max_updates|int(11)unsigned|NO||0|||max_connections|int(11)unsigned|NO||0|||max_user_connections|int(11)unsigned|NO||0|||plugin|char(64)|NO||mysql_native_password|||authentication_string|text|YES||NULL|||password_expired|enum('N','Y')|NO||N|||password_last_changed|timestamp|YES||NULL|||password_lifetime|smallint(5)unsigned|YES||NULL|||account_locked|enum('N','Y')|NO||N||+------------------------+-----------------------------------+------+-----+-----------------------+-------+45rowsinset(0.00sec)mysql>select*frommysql.userwhereUser='root'\G;***************************1.row***************************Host:%User: root
           Select_priv: Y
           Insert_priv: Y
           Update_priv: Y
           Delete_priv: Y
           Create_priv: Y
             Drop_priv: Y
           Reload_priv: Y
         Shutdown_priv: Y
          Process_priv: Y
             File_priv: Y
            Grant_priv: Y
       References_priv: Y
            Index_priv: Y
            Alter_priv: Y
          Show_db_priv: Y
            Super_priv: Y
 Create_tmp_table_priv: Y
      Lock_tables_priv: Y
          Execute_priv: Y
       Repl_slave_priv: Y
      Repl_client_priv: Y
      Create_view_priv: Y
        Show_view_priv: Y
   Create_routine_priv: Y
    Alter_routine_priv: Y
      Create_user_priv: Y
            Event_priv: Y
          Trigger_priv: Y
Create_tablespace_priv: Y
              ssl_type: 
            ssl_cipher: 
           x509_issuer: 
          x509_subject: 
         max_questions:0max_updates:0max_connections:0max_user_connections:0plugin: mysql_native_password
 authentication_string:*4AD47E08DAE2BD4F0977EED5D23DC901359DF617
      password_expired: N
 password_last_changed:2018-10-2820:59:18password_lifetime:NULLaccount_locked: N1rowinset(0.00sec)ERROR:Noquery specified
mysql>查询表相关信息
查询当前库中所有表的信息
执行命令：show tables;
查询某一表的详细信息
执行命令：desc 表名
确认整体状态
执行命令：status
可以查看版本/当前用户/字符集等多个信息
mysql>status--------------mysql  Ver14.14Distrib5.7.16,forLinux(x86_64)usingEditLine wrapper
Connection id:26Currentdatabase:	sysCurrentuser:		root@localhostSSL:NotinuseCurrentpager:		stdoutUsingoutfile:''Usingdelimiter:;Server version:5.7.16-log MySQL Community Server(GPL)Protocol version:10Connection:		Localhost via UNIX socket
Server characterset:	latin1
Db     characterset:	utf8
Client characterset:	latin1
Conn.characterset:	latin1
UNIX socket:/var/run/mysqld/mysqld.sock
Uptime:5hours7min25sec
Threads:2Questions:5624Slow queries:0Opens:1867Flushtables:1Opentables:227Queries persecondavg:0.304--------------mysql>查看警告信息
使用show warnings可以查看警告信息
mysql>showwarnings;Emptyset(0.00sec)mysql>查看错误信息
mysql>showerrors;+-------+------+--------------------------------------------------------------------------------------------------------------------------------------------------------+|Level|Code|Message|+-------+------+--------------------------------------------------------------------------------------------------------------------------------------------------------+|Error|1064|You have an errorinyourSQLsyntax;checkthe manual that correspondstoyour MySQL server versionfortherightsyntaxtousenear'logs'at line1|+-------+------+--------------------------------------------------------------------------------------------------------------------------------------------------------+1rowinset(0.00sec)mysql>查询字符集相关设定
mysql>showvariableslike'character%';+--------------------------+----------------------------+|Variable_name|Value|+--------------------------+----------------------------+|character_set_client|latin1||character_set_connection|latin1||character_set_database|utf8||character_set_filesystem|binary||character_set_results|latin1||character_set_server|latin1||character_set_system|utf8||character_sets_dir|/usr/share/mysql/charsets/|+--------------------------+----------------------------+8rowsinset(0.00sec)mysql>也可以使用status确认
确认时间信息
显示当前时间
执行命令：select current_time();
显示当前日期
执行命令：select current_date();
显示当前日期和时间
执行命令：select current_timestamp();
查询启动后的运行时间
类似linux的uptime命令，单位为秒
mysql>showstatuslike'uptime';+---------------+----------+|Variable_name|Value|+---------------+----------+|Uptime|11490523|+---------------+----------+1rowinset(0.00sec)mysql>查看select/insert/update/delete语句的执行次数
mysql>showglobalstatuslike'com_select';+---------------+----------+|Variable_name|Value|+---------------+----------+|Com_select|22262778|+---------------+----------+1rowinset(0.00sec)mysql>showglobalstatuslike'com_insert';+---------------+---------+|Variable_name|Value|+---------------+---------+|Com_insert|3206699|+---------------+---------+1rowinset(0.00sec)mysql>showglobalstatuslike'com_update';+---------------+---------+|Variable_name|Value|+---------------+---------+|Com_update|2875409|+---------------+---------+1rowinset(0.00sec)mysql>showglobalstatuslike'com_delete';+---------------+---------+|Variable_name|Value|+---------------+---------+|Com_delete|2779458|+---------------+---------+1rowinset(0.00sec)mysql>查看当前打开的连接数
mysql>showstatuslike'threads_connected';+-------------------+-------+|Variable_name|Value|+-------------------+-------+|Threads_connected|30|+-------------------+-------+1rowinset(0.00sec)mysql>查看创建用于处理连接的线程数
mysql>showstatuslike'threads_created';+-----------------+-------+|Variable_name|Value|+-----------------+-------+|Threads_created|1121|+-----------------+-------+1rowinset(0.00sec)mysql>如果数量较大，可考虑调节threads_cache_size进行应对
mysql>showvariableslike'thread_cache_size';+-------------------+-------+|Variable_name|Value|+-------------------+-------+|thread_cache_size|9|+-------------------+-------+1rowinset(0.01sec)mysql>查看激活状态的线程数
mysql>showstatuslike'threads_running';+-----------------+-------+|Variable_name|Value|+-----------------+-------+|Threads_running|1|+-----------------+-------+1rowinset(0.00sec)mysql>
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
