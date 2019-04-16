# 解决Navicat 报错:1130-host ... is not allowed to connect to this MySql server,MySQL - 一个跳popping的quant的博客 - CSDN博客





2017年10月21日 16:08:53[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1305标签：[linux																[mysql																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)
个人分类：[linux](https://blog.csdn.net/FrankieHello/article/category/7144507)










解决方法：

1。 改表法。


可能是你的帐号不允许从远程登陆，只能在localhost。这个时候只要在localhost的那台电脑，登入mysql后，更改 "mysql" 数据库里的 "user" 表里的 "host" 项，从"localhost"改称"%"


mysql -u root -p


mysql>use mysql;


mysql>update user set host = '%' where user = 'root'  and host='localhost';


mysql>select host, user from user;





需要重启MySQL服务。  或 再执行 执行一个语句 mysql>FLUSH RIVILEGES 使修改生效.就可以了





亲测可用。




2. 授权法。


例如，你想myuser使用mypassword从任何主机连接到mysql服务器的话。


GRANT ALL PRIVILEGES ON *.* TO 'myuser'@'%' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;


FLUSH   PRIVILEGES;


如果你想允许用户myuser从ip为192.168.1.6的主机连接到mysql服务器，并使用mypassword作为密码


GRANT ALL PRIVILEGES ON *.* TO 'myuser'@'192.168.1.3' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;


FLUSH   PRIVILEGES;


如果你想允许用户myuser从ip为192.168.1.6的主机连接到mysql服务器的dk数据库，并使用mypassword作为密码


GRANT ALL PRIVILEGES ON dk.* TO 'myuser'@'192.168.1.3' IDENTIFIED BY 'mypassword' WITH GRANT OPTION;


FLUSH   PRIVILEGES;](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




