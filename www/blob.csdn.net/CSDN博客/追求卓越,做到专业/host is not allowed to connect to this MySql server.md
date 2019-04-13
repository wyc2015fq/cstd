
# host is not allowed to connect to this MySql server - 追求卓越,做到专业 - CSDN博客


2015年11月17日 16:36:04[Waldenz](https://me.csdn.net/enter89)阅读数：348标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)[hostis not allowed																](https://so.csdn.net/so/search/s.do?q=hostis not allowed&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)个人分类：[MySQL																](https://blog.csdn.net/enter89/article/category/5959543)


报错:1130-host ... is not allowed to connect to this MySql server
解决方法：
1。 改表法。
可能是你的帐号不允许从远程登陆，只能在localhost。这个时候只要在localhost的那台电脑，登入mysql后，更改 "mysql" 数据库里的 "user" 表里的 "host" 项，从"localhost"改称"%"
mysql -u root -pvmwaremysql>use mysql;
mysql>update user set host = '%' where user = 'root';

mysql>flush privileges;
mysql>select host, user from user;

mysql>quit
2. 授权法。
例如，你想myuser使用mypassword从任何主机连接到mysql服务器的话。
GRANT ALL PRIVILEGES ON *.* TO 'myuser'@'%' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;
FLUSH PRIVILEGES;
如果你想允许用户myuser从ip为192.168.1.6的主机连接到mysql服务器，并使用mypassword作为密码
GRANT ALL PRIVILEGES ON *.* TO 'myuser'@'192.168.1.3' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;
FLUSH PRIVILEGES;
如果你想允许用户myuser从ip为192.168.1.6的主机连接到mysql服务器的dk数据库，并使用mypassword作为密码
GRANT ALL PRIVILEGES ON dk.* TO 'myuser'@'192.168.1.3' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;
FLUSH PRIVILEGES;
我用的第一个方法,刚开始发现不行,在网上查了一下,少执行一个语句 mysql>FLUSH RIVILEGES 使修改生效.就可以了
另外一种方法,不过我没有亲自试过的,在csdn.net上找的,可以看一下.
在安装mysql的机器上运行：
1、d:\mysql\bin\>mysql -h localhost -u root //这样应该可以进入MySQL服务器
2、mysql>GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION //赋予任何主机访问数据的权限
3、mysql>FLUSH PRIVILEGES //修改生效
4、mysql>EXIT //退出MySQL服务器


