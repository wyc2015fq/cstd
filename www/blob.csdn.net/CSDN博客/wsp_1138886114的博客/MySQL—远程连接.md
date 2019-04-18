# MySQL—远程连接 - wsp_1138886114的博客 - CSDN博客





2018年07月31日 13:05:25[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：27








首先，你要确保服务器安全组3306端口已经开放.。

授权法 

登录MySQL服务器，执行：

```
mysql> GRANT ALL PRIVILEGES ON *.* TO 'user'@'%' IDENTIFIED BY 'password' WITH GRANT OPTION; 
# %:表示从任何主机连接到mysql服务器
FLUSH   PRIVILEGES;

或者
mysql> GRANT ALL PRIVILEGES ON *.* TO 'user'@'116.30.70.187' IDENTIFIED BY 'password' WITH GRANT OPTION;
#表示从指定ip从任何主机连接到mysql服务器
mysql> FLUSH PRIVILEGES;
```

2.该表法 

可能是你的帐号不允许从远程登陆，只能在localhost。这个时候只要在localhost的那台电脑， 

登入MySQL后，更改 “mysql” 数据库里的 “user” 表里的 “host” 项，从”localhost”改称”%” 

依次执行：
```
mysql -u root -p
mysql>use mysql;
mysql>update user set host = '%' where user = 'root'  and host='localhost';
mysql>select host, user from user;
```

然后重启MySQL服务或再执行语句 `mysql>FLUSH PRIVILEGES`使修改生效。




