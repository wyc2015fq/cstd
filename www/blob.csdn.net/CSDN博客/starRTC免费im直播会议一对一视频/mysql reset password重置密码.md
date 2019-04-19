# mysql reset password重置密码 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月13日 17:11:08[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：270
安全模式启动
chown -R mysql.mysql /var/run/mysqld/
mysqld_safe --skip-grant-tables &
无密码root帐号登陆
mysql -uroot -p //要求输入密码的时候，直接回车
或直接输入mysql
FLUSH PRIVILEGES;
select Host,User from user \G;
看看localhost这个是否存在，
ALTER USER 'root'@'localhost' IDENTIFIED BY 'test@com888';
