# mysql修改密码与password字段不存在mysqladmin connect to server at localhost failed - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月19日 10:43:01[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：227
所属专栏：[后台架构](https://blog.csdn.net/column/details/31054.html)
mysqladmin: connect to server at 'localhost' failed
停止mysql服务
systemctl stop mysql
安全模式启动
chown -R mysql.mysql /var/run/mysqld/
mysqld_safe --skip-grant-tables &
无密码root帐号登陆
mysql -uroot -p //要求输入密码的时候，直接回车
update user set password=password("guxxxxxahyVh") where user='root' and host='localhost';
flush privileges;
ERROR 1054 (42S22): Unknown column 'password' in 'field list'
select * from user \G;
password字段改成了authentication_string
update user set authentication_string=password("Isssss88") where user='root' and host='localhost';
