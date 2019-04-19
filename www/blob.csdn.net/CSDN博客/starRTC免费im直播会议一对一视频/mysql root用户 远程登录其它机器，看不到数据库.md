# mysql root用户 远程登录其它机器，看不到数据库 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月13日 14:42:16[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：564
在102上访问101上的数据库里，show databases;看不到里面的库，
需要在101上授权就可以了
GRANT ALL PRIVILEGES ON *.* TO 'root'@'192.168.3.102' IDENTIFIED BY 'password' WITH GRANT OPTION;
FLUSH PRIVILEGES;
