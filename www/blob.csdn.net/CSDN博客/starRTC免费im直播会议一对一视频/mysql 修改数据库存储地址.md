# mysql 修改数据库存储地址 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月31日 15:38:38[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：189
默认目录/var/lib/mysql
systemctl stop mysqld
cp -pr /var/lib/mysql /data/mysql
Create a backup of /etc/my.cnf or /etc/mysql/my.cnf(Debian/Ubuntu) ，然后修改
vi /etc/my.cnf
datadir=/data/mysql
Modify file /etc/apparmor.d/usr.sbin.mysqld (create it if it doesn't exist作用是设置某个可执行程序的访问控制权限，可以限制程序 读/写某个目录/文件，打开/读/写网络端口等等）
Look for lines beginning with /var/lib/mysql. Change /var/lib/mysql in the lines with the new path.
/etc/init.d/apparmor restart
如果启动不了，看看有没有mysql的进程，有则杀之。
