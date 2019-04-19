# mysql在服务器异常断电后，无法重启解决办法 - fanyun的博客 - CSDN博客
2016年03月18日 13:56:28[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3822
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
**mysql在服务器重启后，无法正常启动解决办法：**
1.第一个是立即关机 使用命令 shutdown -h now 关机，关机后在硬启动，进程就停止了。
2.mv /var/lib/mysql/mysql.sock /var/lib/mysql/mysql.sock.bak改名备份后在执行：# service mysqld restart
mv /var/lib/mysql/mysql.sock /var/lib/mysql/mysql.sock.bak
