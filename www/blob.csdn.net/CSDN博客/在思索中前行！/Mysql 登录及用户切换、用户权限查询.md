# Mysql 登录及用户切换、用户权限查询 - 在思索中前行！ - CSDN博客





2014年06月14日 16:20:02[_Tham](https://me.csdn.net/txl16211)阅读数：13987








启动mysql：


方法一:net start mysql(或者是其他服务名)

方法二:在windows下启动MySQL服务 
 
MySql安装目录：“d:\MySql\”

进入命令行输入窗口cmd（Dos命令输入）切换到C目录 （cd ..返回上级目录)D： 进入D盘 ，再  cd  D:\Program Files\MySQL\MySQL Server 5.5\bin
进入“mysql 安装目录”下运行“mysqld -u root” 命令。写法：
d:\...MySql\bin\>mysqld -u root (Enter)
运行后在进程管理中有mysql的进程，运行成功（可能需要手动关闭命令窗口）方法三：直接在windows服务中启动mysql服务即可。登录Mysql:mysql自带的控制台底下操作mysql。一登录就是root帐户，我不想用这么高的权限，想用一个稍微低级别权限的帐户，怎么办了？回到命令行控制台，进入 ...MySql\bin\> 路径，然后在此路径下输入：mysql -u 用户名 -p 数据库名，回车后会要求输入密码，然后就可以登录了。这样你就可以选择你想使用的用户登录mysql数据库了 。当进入mysql> 后，输入exit/quit命令可以实现退出当前登录用户。
查询数据库中的用户： select user,host from mysql.user;SELECT DISTINCT CONCAT('User: ''',user,'''@''',host,''';') AS query FROM mysql.user; //查看所有用户查看某个用户的详细信息：  show grants for ['用户名'@'%'](mailto:' rel=);
+-------------------------------------------------------------------------------------------------------------------+
| Grants for [cactiuser@%](mailto:cactiuser@%25)                                                                                            |
+-------------------------------------------------------------------------------------------------------------------+
| GRANT PROCESS, SUPER ON *.* TO ['cactiuser'@'%'](mailto:' rel=) IDENTIFIED BY PASSWORD '*DAFF917B80E3314B1ABECBA9DF8785AFD342CE89' | 
| GRANT ALL PRIVILEGES ON `cacti`.* TO ['cactiuser'@'%'](mailto:' rel=)                                                              | 
+-------------------------------------------------------------------------------------------------------------------+
2 rows in set (0.00 sec)


select * from mysql.user where user='用户名' \G  *************************** 1. row ***************************
                 Host: %
                 User: cactiuser
             Password: *DAFF917B80E3314B1ABECBA9DF8785AFD342CE89
          Select_priv: N
          Insert_priv: N
          Update_priv: N
          Delete_priv: N
          Create_priv: N
            Drop_priv: N查看user表结构　需要具体的项可结合表结构来查询
mysql> desc mysql.user;+-----------------------+-----------------------------------+------+-----+---------+-------+
| Field                 | Type                              | Null | Key | Default | Extra |
+-----------------------+-----------------------------------+------+-----+---------+-------+
| Host                  | char(60)                          | NO   | PRI |         |       | 
| User                  | char(16)                          | NO   | PRI |         |       | 
| Password              | char(41)                          | NO   |     |         |       | 
| Select_priv           | enum('N','Y')                     | NO   |     | N       |       | 
******************
******************
+-----------------------+-----------------------------------+------+-----+---------+-------+
39 rows in set (0.00 sec)




