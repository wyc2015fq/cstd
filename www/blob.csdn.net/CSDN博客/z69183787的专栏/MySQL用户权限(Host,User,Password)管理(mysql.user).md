# MySQL用户权限(Host,User,Password)管理(mysql.user) - z69183787的专栏 - CSDN博客
2017年07月31日 16:32:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1590
**注：**
　　Host列指定了允许用户登录所使用的IP，比如user=root Host=192.168.1.1。这里的意思就是说root用户只能通过192.168.1.1的客户端去访问。
　　而%是个通配符，如果Host=192.168.1.%，那么就表示只要是IP地址前缀为“192.168.1.”的客户端都可以连接。如果Host=%，表示所有IP都有连接权限。、
　　这也就是为什么在开启远程连接的时候，大部分人都直接把Host改成%的缘故，为了省事。
**1：新增用户：**
注：[MySQL](http://lib.csdn.net/base/mysql)[数据库](http://lib.csdn.net/base/mysql)下user表中，**Host和User为两个主键列**（primary key）****，已经各版本下非空未设置默认字段。
登录后，切换db：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> use mysql;  
- Reading table information for completion oftableandcolumn names  
- You can turn off this feature to get a quicker startup with -A  
- 
- Database changed  
新增用户：
注：限制kaka用户的登陆ip为10.155.123.55，ip为随手写入，如果正确配置为您有效登陆ip，所有ip登陆，则设置Host为 '%'
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> INSERTINTO mysql.user(Host,User,Password) VALUES("10.155.123.55","kaka",PASSWORD("kaka123"));  
在版本 5.6.27:
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> INSERTINTO mysql.user(Host,User,Password,ssl_cipher,x509_issuer,x509_subject) VALUES("10.155.123.55","kaka",PASSWORD("kaka123"),"","","");  
- Query OK, 1 row affected (0.03 sec)  
新增用户（全sql）：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- INSERTINTO `user`(`Host`,`User`,`Password`,`Select_priv`,`Insert_priv`,`Update_priv`,`Delete_priv`,`Create_priv`,`Drop_priv`,`Reload_priv`,`Shutdown_priv`,`Process_priv`,`File_priv`,`Grant_priv`,`References_priv`,`Index_priv`,`Alter_priv`,`Show_db_priv`,`Super_priv`,`Create_tmp_table_priv`,`Lock_tables_priv`,`Execute_priv`,`Repl_slave_priv`,`Repl_client_priv`,`Create_view_priv`,`Show_view_priv`,`Create_routine_priv`,`Alter_routine_priv`,`Create_user_priv`,`Event_priv`,`Trigger_priv`,`Create_tablespace_priv`,`ssl_type`,`ssl_cipher`,`x509_issuer`,`x509_subject`,`max_questions`,`max_updates`,`max_connections`,`max_user_connections`,`plugin`,`authentication_string`,`password_expired`) VALUES ('%','root','*6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','Y','','','','',0,0,0,0,'mysql_native_password','','N');  
新增用户完成，刷新[mysql](http://lib.csdn.net/base/mysql)的系统权限相关表
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> flush privileges;  
- Query OK, 0 rows affected (0.00 sec)  
设置遇到问题，请查看：[MySQL配置和设置问题小结](http://blog.csdn.net/typa01_kk/article/details/49107407)
重启生效：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- [root@Tony_ts_tian bin]# service mysqld restart  
- Shutting down MySQL.... SUCCESS!   
- Starting MySQL. SUCCESS!   
查询用户，Host,User,Password：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql>  SELECT Host,User,PasswordFROM mysql.user;    
- +----------------+------+-------------------------------------------+
- | Host           | User | Password                                  |  
- +----------------+------+-------------------------------------------+
- | localhost      | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | tony\_ts\_tian | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | 127.0.0.1      | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | ::1            | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | 10.155.123.55  | kaka | *90B3D884FB6092549F244125549B77C000A0F9C6 |  
- | %              | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- +----------------+------+-------------------------------------------+
- 6 rowsinset (0.00 sec)  
**2：修改信息，密码，类似可修改其他字段。**
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> UPDATE `user` SETPassword=PASSWORD("123456") WHERE Host='10.155.123.55'ANDUser='kaka';  
- Query OK, 1 row affected (0.02 sec)  
- Rows matched: 1  Changed: 1  Warnings: 0  
- mysql> flush privileges;  
- Query OK, 0 rows affected (0.00 sec)  
- mysql> SELECT Host,User,PasswordFROM `user`;  
- 前:  
- | 10.155.123.55  | kaka | *90B3D884FB6092549F244125549B77C000A0F9C6 |  
- 后：  
- | 10.155.123.55  | kaka | *6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9 |  
**3：删除用户：**
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> DELETEFROM `user` WHERE Host='10.155.123.55'ANDUser='kaka';  
- Query OK, 1 row affected (0.00 sec)  
- 
- mysql> flush privileges;  
- Query OK, 0 rows affected (0.00 sec)  
- 
- mysql> SELECT Host,User,PasswordFROM `user`;  
- +----------------+------+-------------------------------------------+
- | Host           | User | Password                                  |  
- +----------------+------+-------------------------------------------+
- | localhost      | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | tony\_ts\_tian | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | 127.0.0.1      | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | ::1            | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- | %              | root | *71ABCA8B06D46066CEF8062A75256E66243D0FC8 |  
- +----------------+------+-------------------------------------------+
- 5 rowsinset (0.00 sec)  
**4. 权限分配**
**[plain]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- GRANT语法：     
-    GRANT 权限 ON 数据库.* TO 用户名@'登录主机' IDENTIFIED BY '密码'  
- 权限：  
-    ALL,ALTER,CREATE,DROP,SELECT,UPDATE,DELETE  
-    新增用户：权限为USAGE,即为："无权限",想要创建一个没有权限的用户时,可以指定USAGE  
- 数据库：  
-      *.*              表示所有库的所有表  
-      mylove.*         表示mylove库的所有表  
-      mylove.loves     表示mylove库的loves表   
- 用户名：  
-      MySQL的账户名  
- 登陆主机：  
-      允许登陆到MySQL Server的客户端ip  
-      '%'表示所有ip  
-      'localhost' 表示本机  
-      '10.155.123.55' 特定IP  
- 密码：  
-       MySQL的账户名对应的登陆密码  
注： IDENTIFIED BY '密码'，可选。
        GRANT会覆盖用户的部分信息，跟insert 、update执行功能一样。
给用户kaka分配test数据库下user表的查询select权限：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> GRANTSELECTON test.userTO kaka@'10.155.123.55' IDENTIFIED BY'123456';  
- Query OK, 0 rows affected (0.00 sec)  
- mysql> flush privileges;  
- Query OK, 0 rows affected (0.00 sec)  
- mysql> show Grants for'kaka'@'10.155.123.55';  
- +-----------------------------------------------------------------------------------------------------------------+
- | Grants for kaka@10.155.123.55                                                                                   |  
- +-----------------------------------------------------------------------------------------------------------------+
- | GRANT USAGE ON *.* TO'kaka'@'10.155.123.55' IDENTIFIED BYPASSWORD'*6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9' |  
- | GRANTSELECTON `test`.`user` TO'kaka'@'10.155.123.55'                                                         |  
- +-----------------------------------------------------------------------------------------------------------------+
- 2 rowsinset (0.00 sec)  
为了快速[测试](http://lib.csdn.net/base/softwaretest)，我要把ip切回%，ip全访问：
使用和测试：
数据库和数据表请看：[MySQL数据定义语句：CREATE（创建）命令、ALTER（修改）命令、DROP（删除）](http://blog.csdn.net/typa01_kk/article/details/49131993)
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> use mysql  
- Reading table information for completion oftableandcolumn names  
- You can turn off this feature to get a quicker startup with -A  
- Database changed  
- 修改权限Host为所有ip登陆：  
- mysql> UPDATE `user` SET Host='%'WHERE Host='10.155.123.55'ANDUser='kaka';  
- Query OK, 1 row affected (0.00 sec)  
- Rows matched: 1  Changed: 1  Warnings: 0  
- 查看kaka的权限：  
- mysql> show grants for'kaka'@'10.155.123.55';  
- +-----------------------------------------------------------------------------------------------------------------+
- | Grants for kaka@10.155.123.55                                                                                   |  
- +-----------------------------------------------------------------------------------------------------------------+
- | GRANT USAGE ON *.* TO'kaka'@'10.155.123.55' IDENTIFIED BYPASSWORD'*6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9' |  
- | GRANTSELECTON `test`.`user` TO'kaka'@'10.155.123.55'                                                         |  
- +-----------------------------------------------------------------------------------------------------------------+
- 2 rowsinset (0.00 sec)  
- 刷新MySQL的系统权限相关表  
- mysql> flush privileges;  
- Query OK, 0 rows affected (0.00 sec)  
- 查看kaka的权限：  
- mysql> show grants for'kaka'@'%';  
- +-----------------------------------------------------------------------------------------------------+
- | Grants for kaka@%                                                                                   |  
- +-----------------------------------------------------------------------------------------------------+
- | GRANT USAGE ON *.* TO'kaka'@'%' IDENTIFIED BYPASSWORD'*6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9' |  
- +-----------------------------------------------------------------------------------------------------+
- 1 row inset (0.00 sec)  
- 给用户kaka分配weloveshare数据库下user表的查询select权限：  
- mysql> GRANTSELECTON `weloveshare`.`user` TO kaka@'%';  
- Query OK, 0 rows affected (0.00 sec)  
- 查看kaka的权限：  
- mysql> show grants for'kaka'@'%';  
- +-----------------------------------------------------------------------------------------------------+
- | Grants for kaka@%                                                                                   |  
- +-----------------------------------------------------------------------------------------------------+
- | GRANT USAGE ON *.* TO'kaka'@'%' IDENTIFIED BYPASSWORD'*6BB4837EB74329105EE4568DDA7DC67ED2CA2AD9' |  
- | GRANTSELECTON `weloveshare`.`user` TO'kaka'@'%'                                                  |  
- +-----------------------------------------------------------------------------------------------------+
- 2 rowsinset (0.00 sec)  
- 查看weloveshare数据库下user表的数据：  
- mysql> use weloveshare  
- Reading table information for completion oftableandcolumn names  
- You can turn off this feature to get a quicker startup with -A  
- Database changed  
- mysql> select * fromuser;  
- Empty set (0.00 sec)  
- 退出当前用户：  
- mysql> exit;  
- Bye  
- 切换用户kaka:  
- [root@Tony_ts_tian ~]# mysql -u kaka -p  
- Enter password:   
- 登录成功。  
- 切换数据库，查看user表数据：  
- mysql> use weloveshare  
- Reading table information for completion oftableandcolumn names  
- You can turn off this feature to get a quicker startup with -A  
- 
- Database changed  
- mysql> select * fromuser;  
- Empty set (0.00 sec)  
- 插入数据：  
- mysql> INSERTINTO `weloveshare`.`user`(uname,upass,ustatus) VALUES('kaka','kaka123','0');  
- ERROR 1142 (42000): INSERT command denied touser'kaka'@'localhost'fortable'user'
- 提示：INSERT被拒绝。配置成功。  

**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- 注：`weloveshare`.`user`数据库名.数据表名，kaka用户名，%为Host，ip可限制或不 localhost，%，192.168.10.%  
- grant创建、修改、删除、更新、查询MySQL数据表结构权限：  
- GRANTCREATEON `weloveshare`.`user` TO kaka@'%';   
- GRANTALTERON `weloveshare`.`user` TO kaka@'%';   
- GRANTDROPON `weloveshare`.`user` TO kaka@'%';   
- GRANTUPDATEON `weloveshare`.`user` TO kaka@'%';   
- GRANTSELECTON `weloveshare`.`user` TO kaka@'%';   
- grant操作MySQL外键权限:  
- GRANTREFERENCESON `weloveshare`.`user` TO kaka@'%';   
- grant操作MySQL 临时表权限:  
- GRANTCREATETEMPORARY TABLES ON `weloveshare`.`user` TO kaka@'%';   
- grant操作MySQL索引权限  
- GRANTINDEXON `weloveshare`.`user` TO kaka@'%';   
- grant操作MySQL视图、查看视图源代码权限:  
- GRANTCREATEVIEWON `weloveshare`.`user` TO kaka@'%';   
- GRANT SHOW VIEWON `weloveshare`.`user` TO kaka@'%';   
- grant操作MySQL存储过程(查看状态,删除修改)、函数权限。  
- GRANTCREATE ROUTINE ON `weloveshare`.`user` TO kaka@'%';   
- GRANTCREATE ROUTINE ON `weloveshare`.`user` TO kaka@'%';   
- GRANTEXECUTEON `weloveshare`.`user` TO kaka@'%';  
**注：其他的详细权限，请查看，备注附件（最后）。**
**5：查看数据库登陆所有用户：**
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> SELECTDISTINCT CONCAT('User: ''',user,'''@''',host,''';') AS QUERY FROM mysql.user;  
- +--------------------------------+
- | QUERY                          |  
- +--------------------------------+
- | User: 'kaka'@'%';              |  
- | User: 'root'@'%';              |  
- | User: 'root'@'127.0.0.1';      |  
- | User: 'root'@'::1';            |  
- | User: 'root'@'localhost';      |  
- | User: 'root'@'tony\_ts\_tian'; |  
- +--------------------------------+
- 6 rowsinset (0.00 sec)  
查看某个用户的具体权限，比如root：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> show grants for'root'@'%';  
- +--------------------------------------------------------------------------------------------------------------------------------+
- | Grants for root@%                                                                                                              |  
- +--------------------------------------------------------------------------------------------------------------------------------+
- | GRANTALLPRIVILEGESON *.* TO'root'@'%' IDENTIFIED BYPASSWORD'*71ABCA8B06D46066CEF8062A75256E66243D0FC8'WITHGRANTOPTION |  
- +--------------------------------------------------------------------------------------------------------------------------------+
- 1 row inset (0.00 sec)  
或
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> select * from mysql.userwhereuser='root' \G  
注：\G为按列显示数据。
**备注附件：**
查看MySQL数据中user表的表结构：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- mysql> DESC mysql.user;  
- +------------------------+-------------------+------+-----+-----------+-------+
- | Field                  | Type              | Null | Key | Default   | Extra |  
- +------------------------+-------------------+------+-----+-----------+-------+
- | Host                   | char(60)          | NO   | PRI |           |       |  
- | User                   | char(16)          | NO   | PRI |           |       |  
- | Password               | char(41)          | NO   |     |           |       |  
- | Select_priv            | enum('N','Y')     | NO   |     | N         |       |  
- | Insert_priv            | enum('N','Y')     | NO   |     | N         |       |  
- | Update_priv            | enum('N','Y')     | NO   |     | N         |       |  
- | Delete_priv            | enum('N','Y')     | NO   |     | N         |       |  
- | Create_priv            | enum('N','Y')     | NO   |     | N         |       |  
- | Drop_priv              | enum('N','Y')     | NO   |     | N         |       |  
- | Reload_priv            | enum('N','Y')     | NO   |     | N         |       |  
- | Shutdown_priv          | enum('N','Y')     | NO   |     | N         |       |  
- | Process_priv           | enum('N','Y')     | NO   |     | N         |       |  
- | File_priv              | enum('N','Y')     | NO   |     | N         |       |  
- | Grant_priv             | enum('N','Y')     | NO   |     | N         |       |  
- | References_priv        | enum('N','Y')     | NO   |     | N         |       |  
- | Index_priv             | enum('N','Y')     | NO   |     | N         |       |  
- | Alter_priv             | enum('N','Y')     | NO   |     | N         |       |  
- | Show_db_priv           | enum('N','Y')     | NO   |     | N         |       |  
- | Super_priv             | enum('N','Y')     | NO   |     | N         |       |  
- | Create_tmp_table_priv  | enum('N','Y')     | NO   |     | N         |       |  
- | Lock_tables_priv       | enum('N','Y')     | NO   |     | N         |       |  
- | Execute_priv           | enum('N','Y')     | NO   |     | N         |       |  
- | Repl_slave_priv        | enum('N','Y')     | NO   |     | N         |       |  
- | Repl_client_priv       | enum('N','Y')     | NO   |     | N         |       |  
- | Create_view_priv       | enum('N','Y')     | NO   |     | N         |       |  
- | Show_view_priv         | enum('N','Y')     | NO   |     | N         |       |  
- | Create_routine_priv    | enum('N','Y')     | NO   |     | N         |       |  
- | Alter_routine_priv     | enum('N','Y')     | NO   |     | N         |       |  
- | Create_user_priv       | enum('N','Y')     | NO   |     | N         |       |  
- | Event_priv             | enum('N','Y')     | NO   |     | N         |       |  
- | Trigger_priv           | enum('N','Y')     | NO   |     | N         |       |  
- | Create_tablespace_priv | enum('N','Y')     | NO   |     | N         |       |  
- | ssl_type               | enum('','ANY','X509','SPECIFIED') | NO  || |       |  
- | ssl_cipher             | blob              | NO   |     | NULL      |       |  
- | x509_issuer            | blob              | NO   |     | NULL      |       |  
- | x509_subject           | blob              | NO   |     | NULL      |       |  
- | max_questions          | int(11) unsigned  | NO   |     | 0         |       |  
- | max_updates            | int(11) unsigned  | NO   |     | 0         |       |  
- | max_connections        | int(11) unsigned  | NO   |     | 0         |       |  
- | max_user_connections   | int(11) unsigned  | NO   |     | 0         |       |  
- | plugin                 | char(64)          | YES  || mysql_native_password ||  
- | authentication_string  | text              | YES  |     | NULL      |       |  
- | password_expired       | enum('N','Y')     | NO   |     | N         |       |  
- +------------------------+-------------------+------+-----+-----------+-------+
- 43 rowsinset (0.00 sec)  
查看root用户的所有具体权限：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
-                Host: %  
- User: root  
- Password: *71ABCA8B06D46066CEF8062A75256E66243D0FC8  
-         Select_priv: Y  
-         Insert_priv: Y  
-         Update_priv: Y  
-         Delete_priv: Y  
-         Create_priv: Y  
-           Drop_priv: Y  
-         Reload_priv: Y  
-       Shutdown_priv: Y  
-        Process_priv: Y  
-           File_priv: Y  
-          Grant_priv: Y  
-     References_priv: Y  
-          Index_priv: Y  
-          Alter_priv: Y  
-        Show_db_priv: Y  
-          Super_priv: Y  
- eate_tmp_table_priv: Y  
-    Lock_tables_priv: Y  
-        Execute_priv: Y  
-     Repl_slave_priv: Y  
-    Repl_client_priv: Y  
-    Create_view_priv: Y  
-      Show_view_priv: Y  
- Create_routine_priv: Y  
-  Alter_routine_priv: Y  
-    Create_user_priv: Y  
-          Event_priv: Y  
-        Trigger_priv: Y  
- ate_tablespace_priv: Y  
-            ssl_type:   
-          ssl_cipher:   
-         x509_issuer:   
-        x509_subject:   
-       max_questions: 0  
-         max_updates: 0  
-     max_connections: 0  
- ax_user_connections: 0  
-              plugin: mysql_native_password  
- thentication_string:   
-    password_expired: N  
参数说明：
**[sql]**[view
 plain](http://blog.csdn.net/typa01_kk/article/details/49126365#)[copy](http://blog.csdn.net/typa01_kk/article/details/49126365#)
- Select_priv：用户可以通过SELECT命令选择数据。  
- Insert_priv：用户可以通过INSERT命令插入数据;  
- Update_priv：用户可以通过UPDATE命令修改现有数据;  
- Delete_priv：用户可以通过DELETE命令删除现有数据;  
- Create_priv：用户可以创建新的数据库和表;  
- Drop_priv：用户可以删除现有数据库和表;  
- Reload_priv：用户可以执行刷新和重新加载MySQL所用各种内部缓存的特定命令,包括日志、权限、主机、查询和表;重新加载权限表;  
- Shutdown_priv：用户可以关闭MySQL服务器;在将此权限提供给root账户之外的任何用户时,都应当非常谨慎;  
- Process_priv：用户可以通过SHOW PROCESSLIST命令查看其他用户的进程;服务器管理;  
- File_priv：用户可以执行SELECTINTO OUTFILE和LOAD DATA INFILE命令;加载服务器上的文件;  
- Grant_priv：用户可以将已经授予给该用户自己的权限再授予其他用户(任何用户赋予全部已有权限);  
- References_priv;目前只是某些未来功能的占位符；现在没有作用;  
- Index_priv：用户可以创建和删除表索引;用索引查询表;  
- Alter_priv：用户可以重命名和修改表结构;  
- Show_db_priv：用户可以查看服务器上所有数据库的名字,包括用户拥有足够访问权限的数据库;可以考虑对所有用户禁用这个权限,除非有特别不可抗拒的原因;  
- Super_priv：用户可以执行某些强大的管理功能,例如通过KILL命令删除用户进程,使用SETGLOBAL修改全局MySQL变量,执行关于复制和日志的各种命令;超级权限;  
- Create_tmp_table_priv：用户可以创建临时表;  
- Lock_tables_priv：用户可以使用LOCK TABLES命令阻止对表的访问/修改;  
- Execute_priv：用户可以执行存储过程;此权限只在MySQL 5.0及更高版本中有意义;  
- Repl_slave_priv：用户可以读取用于维护复制数据库环境的二进制日志文件;此用户位于主系统中,有利于主机和客户机之间的通信;主服务器管理;  
- Repl_client_priv：用户可以确定复制从服务器和主服务器的位置;从服务器管理;  
- Create_view_priv：用户可以创建视图;此权限只在MySQL 5.0及更高版本中有意义;  
- Show_view_priv：用户可以查看视图或了解视图如何执行;此权限只在MySQL 5.0及更高版本中有意义;  
- Create_routine_priv：用户可以更改或放弃存储过程和函数;此权限是在MySQL 5.0中引入的;  
- Alter_routine_priv：用户可以修改或删除存储函数及函数;此权限是在MySQL 5.0中引入的;  
- Create_user_priv：用户可以执行CREATEUSER命令,这个命令用于创建新的MySQL账户;  
- Event_priv：用户能否创建、修改和删除事件;这个权限是MySQL 5.1.6新增的;  
- Trigger_priv：用户能否创建和删除触发器,这个权限是MySQL 5.1.6新增的;  
- Create_tablespace_priv：创建表空间  
- ssl_type：支持ssl标准加密安全字段  
- ssl_cipher：支持ssl标准加密安全字段  
- x509_issuer：支持x509标准字段  
- x509_subject：支持x509标准字段  
- max_questions：0 每小时允许执行多少次查询  
- max_updates：0 每小时可以执行多少次更新  ：0表示无限制  
- max_connections：0 每小时可以建立的多少次连接：0表示无限制  
- max_user_connections：0 单用户可以同时具有的连接数：0表示无限制  
- plugin：5.5.7开始,mysql引入plugins以进行用户连接时的密码验证,plugin创建外部/代理用户   
- authentication_string：通过authentication_string可以控制两者的映射关系,(PAM plugin等,PAM可以支持多个服务名)尤其是在使用代理用户时，并须声明这一点  
- password_expired：密码过期 Y,说明该用户密码已过期 N相反  
- 
