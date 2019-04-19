# mysql导出与导出远程操作 - 零度的博客专栏 - CSDN博客
2019年03月29日 09:57:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：93
（一）数据库备份：
mysqldump -h 10.31.xxx.xxx -u username -p --set-gtid-purged=off --databases dbname --tables tb_table_1  tb_table_2 > backupfile.sql
（二）还原数据库
方法一：
mysql -h 10.31.xx.xx -u username  -p db_name < backupfile.sql
方法二：
# mysql -h 10.31.xx.xx -u zxsy_front -p 
mysql > use db_name
mysql > source backupfile.sql
（三）创建用户并给用户授权
grant select，insert，update on `db-name`.* to 'newuser'@'%' identified by 'crmpassword';
REVOKE All ON *.* FROM 'newuser'@'%';
