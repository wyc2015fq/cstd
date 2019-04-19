# mysql权限管理 - 建建的博客 - CSDN博客
2017年03月15日 11:07:06[纪建](https://me.csdn.net/u013898698)阅读数：292
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

mysql权限管理
学习目标
- 学习掌握mysql赋权操作
- 了解mysql权限验证流程 
连接mysql的必要条件
- 网络要通畅
- 用户名和密码要正确
- 数据库需要加IP白名单
- 更细粒度的验证（库，表，列权限类型等）
数据有哪些权限：
Data Prvileges
- DATA:SELECT,INSERT,UPDATE,DELETE
Definition Privileges
- DataBase:CREATE,ALTER,DROP
- Table:CREATE,ALTER,DROP
- VIEW/FUNCTION/TRIGGER/PROCEDURE:CREATE,ALTER,DROP
Administrator Privileges
- Shutdown DataBase
- Replication Slave
- Replication Client
- File Privilege
赋权的操作
mysql >help grant
例子：
使用mysql自带的命令，创建一个用户并赋权；
```
#创建用户
mysql> CREATE USER net@localhost IDENTIFIED BY '000000';
Query OK, **0** rows affected (**0.00** sec)
#在所有库赋select权限
mysql> GRANT SELECT ON *.* TO net@localhost WITH GRANT OPTION;
#localhost 是IP白名单；
```
其他方法赋权：
更改数据库记录：
- 首先向User表里面插入一条记录，根据自己的需求选择是否向db和table_pirv表插入记录
- 执行flush privileges 命令，让权限信息生效
更简单方法赋权：
GRANT 语句会判断是否存在该用户，如果不存在则新建；
```
mysql> GRANT SELECT ON *.* TO net@localhost IDENTIFIED BY '000000' WITH GRANT OPTION;
#将密码也放入里面。localhost 是IP白名单；
```
相关操作
查看用户权限：
```
#查看当前用户的权限：
mysql> show grants;
#查看其它用户的权限：
mysql> show grants for nn@localhost;
```
如何更改用户的权限：
```
#回收不需要的权限
mysql> REVOKE SELECT  ON *.* FROM netease@localhost;
#revoke 不能收回usage权限，也就是说revoke用户不能删除用户；
 
#重新赋权：
mysql> GRANT INSERT ON *.* TO netease@localhost;
#all  privilege 参数赋所有权限；
```
如何更改用户的密码：
```
**1**.用新密码，grant语句重新授权如：
mysql> GRANT USAGE ON *.* TO net@localhost IDENTIFIED BY '111111' WITH GRANT OPTION;
#此时便将net用户的密码从'000000'改到了'111111'
 
**2**.更改数据库记录，update user表的password字段
 
**3**.执行set password语句，下例中修改账号nn@localhost密码
mysql> SET PASSWORD FOR  nn@localhost = PASSWORD ('333');
注意：用这种办法，更改完成需要flush privileges刷新权限信息，不推荐；
```
删除用户： drop user 
```
mysql> drop user netease@localhost;
```
With Grant Option
- 允许被授予权利的人把这个权限授予其他的人
权限管理的秘密
mysql权限信息存储结构：
- mysql权限信息是存在数据库表中
- mysql账号对应的密码也加密存储在数据库表中
- 每一种权限类型在元数据里都是枚举类型，表明是否有该权限
有哪些权限相关的表：
- user
- db
- tables_priv
- columns_priv
权限级别从     库级别->表级别->字段级别
权限验证流程：
假设存在用户：netease,用户欲查询city表的数据
第一步：验证User表
mysql> select * from mysql.user where user = 'netease' \G;
如果存在，说明用户存在并且可以连接到数据库中
第二部：验证DB表
mysql> select * from mysql.db;
如果不存在，说明不存在DB层级权限
第三部分：验证table表
mysql> select * from mysql.tables_priv \G;
第四部分：验证column_priv
示例：
当给某个用户所有库的 select 权限时，改权限会在user表中的select 权限显示；
而如果某个权限在某个具体库中有select 权限时，则会在DB表中显示该权限；
table 和  column 的权限 也类似；
就是 如果 对整个DB都有的权限 会在DB权限列显示，如果是具体的table表的权限则会在具体 table表中显示该权限。
user表中的每个权限都代表了对所有数据库都有的权限；
user表中的列主要：用户列，权限列，安全列，资源控制列
连接过程是从user表中的 host,user,password这三列；
host为% 可以匹配任何id，空host值等价于%
当user值与host值有多条对应时：
|host值|user值|被条目匹配的连接|
|----|----|----|
|'thomas.loc.gov'|'fred'|fred,从thomas.loc.gov连接|
|'thomas.loc.gov'|''|任何用户，从thomas.loc.gov连接|
|'%'|'fred'|fred，从任何主机连接|
|'144.155.166.177'|'fred'|fred,从144.155.166.177IP地址的主机连接|
host中既有'thomas.lo.gov'，又有“%”，那么使用fred，从thomas.loc.gov连接过来，显然user表里面这两条记录都符合匹配条件，那系统会选择哪一个呢？
如果有多个匹配，服务器选择原则：
- 服务器在启动时读入user表后进行排序
- 然后当用户试图连接时，以排序的顺序浏览条目
- 服务器使用与客户端和用户名匹配的第一行 
排序时，首先以最具体的host值排序；
注意：user表中host的值 为%，或者为空的时候，表示外部所有IP都可以连接，但是不包括本地服务器localhost，因此如果要包括本地服务器，必须单独为localhost赋予权限。
super，process，file 等管理权限不能指定某个数据库,ON 后面必须跟“*.*”;
小结：
- mysql权限信息都是以数据记录的形式存储在数据库的表中
- mysql的权限验证相比网站登录验证多了白名单的环节，并且粒度更细，可以精确到表和字段
mysql权限上有哪些问题：
- 使用Binary二进制安装管理用户没有设置密码
- mysql默认的test库不受权限控制，存在安全风险
在mysql二进制安装辅助安装工具：mysql_secure_installation使用安全安装 ；
小结：
- 权限相关的操作不要直接操作表，统一使用mysql命令
- 使用二进制安装mysql后，需要重置管理用户的密码
- 线上数据库不要留test库
总结：
- 加IP白名单
- 赋权：GRANT INSERT ON *.* TO netease@localhost;
- 查看:show grants;
- 更改用户权限:先revoke掉再grant
- 更改密码：
-   用grant....inentified by'new_pw'新密码覆盖
-   更改数据库user表
-   使用set password for nn@localhost=PASSWORD('NEW_PASSWORD')
- 权限验证流程：user->db->table->column
- host,user多条匹配，服务器排序原则：先排序最具体值
- %，空表示外部所有IP都可以连接，但不包含本地；
