# MySQL用户认证及权限控制



一、MySQL用户认证：

登录并不属于访问控制机制，而属于用户身份识别和认证： 

　　1.用户名—user

　　2.密码—password

　　3.登录mysqld主机—host

实现用户登录MySQL，建立连接。

MySQL的整个访问控制包括两大块：用户管理模块、访问控制模块

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170414190213736-630243271.png)

MySQL中用户管理模块的信息存储在系统表mysql.user中

mysql> SELECT * FROM mysql.user;

![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170414190746892-1630906522.png)

存放的信息：授权用户的基本信息、权限信息

首先：以root用户登录---shell> mysql -u root -p

1、新建用户---localhost本地主机、%表示任何登录

```
mysql> create user 'u1'@'172.16.12.24' identified by '123';
mysql> create user 'u1'@'172.16.11.99' identified by '123';
mysql> create user 'u2'@'%' identified by '123';

mysql> select user,host from mysql.user;
+-----------+--------------+
| user      | host         |
+-----------+--------------+
| u2        | %            |
| u1        | 172.16.11.99 |
| u1        | 172.16.12.24 |
| mysql.sys | localhost    |
| root      | localhost    |
+-----------+--------------+
5 rows in set (0.00 sec)
```

2、删除用户

mysql> use mysql;

mysql> delete from user where User='u1' and Host='172.16.11.99';

3、修改用户名

语法：该语句不影响用户的密码

　　RENAME USER old_user  TO  new_user

　　　　[, old_user  TO  new_user] ...

mysql> rename user 'u1'@'172.16.12.24' to 'win.user'@'172.16.12.24';

mysql> rename user 'u2'@'%' to 'back'@'127.0.0.1';

4、修改用户密码

①推荐方法：

mysql> alter user '用户名'@'登录主机' identified by '密码(自定义)';

②传统方法(mysql5.6)：

语法：

　　SET PASSWORD [FOR user_name] = password_option;

缺省for user_name是给当前用户修改密码

mysql> SET PASSWORD FOR '用户名'@'登录主机' = PASSWORD('密码');

5、破解密码

先关闭mysqld服务

①查看跳过授权表参数

　　shell> mysqld --verbose --help

　　　　--skip-grant-tables

②修改/etc/my.cnf文件，注释密码策略参数

③安全启动mysql

　　shell> mysqld_safe --skip-grant-tables &

④无密码登录mysql -uroot

mysql> flush privileges;    #加载授权表

然后进行修改密码

最后重启数据库，用修改的密码进行登录验证即可

 

\################

在MySQL中常用的查询信息的“信息函数”

 ![img](https://images2015.cnblogs.com/blog/1113510/201704/1113510-20170414190942861-1301417211.png)

  

二、MySQL的授权:

  　　1.系统权限

  　　2.针对某一个数据库

  　　3.某一个数据库里面的某一张表

  　　4.某一个数据库里面的某一张表的某一个列

1、按照权限的授予级别，分类：

　　①全局权限Global Privileges：管理权限，应用到服务器上的所有数据库上(全局权限保存在 mysql.user 表中)-----on *.*

　　②数据库权限Database Privileges：应用到某个特定数据库的所有对象上(数据库权限保存在 mysql.db 表中)-----on db_name.*

　　③表权限Table Privileges：应用到某个特定表的所有列上(表权限保存在 mysql.tables_priv 表中)-----on db_name.tbl_name

　　④列权限Column Privileges：应用到某个特定表的单个列上(列权限保存在 mysql.columns_priv 表中)-----在权限名称后面跟小括号，其中写上列名

　　⑤存储例程权限Stored Routine Privileges：应用到存储过程和函数上(存储例程权限保存在 mysql.procs_priv表中)

　　⑥代理用户权限Proxy User Privileges：使一个用户成为另一个用户的代理(代理用户权限保存在 mysql.proxies_priv表中)

2、基本格式：

　　grant 权限类型 on [对象类型] 某数据库.某表 to 用户名@登录主机;　

　　1、u1可以访问所有数据库的所有表，select

　　　　mysql> grant select on table *.* to 'u1'@'172.16.12.24';

　　2、u1可以访问test数据库的所有表，insert

　　　　mysql> grant insert on table test.* to 'u1'@'172.16.12.24';

　　3、u2可以访问test数据库里面的t1

　　　　mysql> grant all on table test.t1 to 'u2'@'%';

　　4、u2可以访问test数据库里面的t1(id列)

　　　　mysql> grant select (id) on table test.t1 to 'u2'@'%';

3、grant里的新建用户(且授权)：

　　mysql> grant all on *.* to 'zhang'@'%' identified by '123' with grant option;

with grant option子句

　　---通过在grant语句的最后使用该子句，就允许被授权的用户把得到的权限继续授给其它用户。

 

问：能否新建用户但不设密码？

mysql>  grant all on *.* to 'u3'@'%' with grant option;

ERROR 1133 (42000): Can't find any matching row in the user table

\#u3是个未知用户，不定义密码就是授权操作，因此报错没有匹配的用户

解决：

①查看变量sql_mode的值

mysql> show variables like 'sql_mode';

……NO_AUTO_CREATE_USER……    #不自动创建用户(set去掉此项修改sql_mode参数)

expansions：

　　MySQL服务器可以以不同的SQL模式来操作，并且可以为不同客户端应用不同模式；

　　如此，每个应用程序可以根据自己的需求来定制服务器的操作模式；

　　sql_mode模式定义了MySQL应支持的SQL语法，以及应该在数据上执行何种确认检查；通过sql_mode设置，就能在众多不同的环境下、与其他数据库服务器一起更容易地使用MySQL

MySQL常用的有三种SQL模式：传统模式TRADITIONAL、严格模式STRICT、ANSI模式

②修改变量sql_mode的值

mysql>set @@session.sql_mode='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

Query OK, 0 rows affected, 1 warning (0.00 sec)

③新建无密码用户u3且授权

mysql>  grant all on *.* to 'u3'@'%' with grant option;

Query OK, 0 rows affected, 1 warning (0.01 sec)

 

4、废除(revoke)权限

语法：

```
　　REVOKE
　　　　priv_type [(column_list)]
　　　　[, priv_type [(column_list)]] ...
　　　　ON [object_type] priv_level
　　　　FROM user [, user] ...
　　REVOKE ALL PRIVILEGES, GRANT OPTION
　　　　FROM user [, user] ...
　　REVOKE PROXY ON user
　　　　FROM user [, user] ...
```

基本格式：

　　revoke 权限类型 on [对象类型] 权限级别(库表) from user

mysql> revoke all on *.* from 'u3'@'%';

@author：http://www.cnblogs.com/geaozhang/