# ubuntu下安装mysql及卸载mysql方法 - 建建的博客 - CSDN博客
2018年02月24日 14:43:08[纪建](https://me.csdn.net/u013898698)阅读数：35
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
1. 删除mysql
```
a. sudo apt-get autoremove --purge mysql-server-5.0
b. sudo apt-get remove mysql-server
c. sudo apt-get autoremove mysql-server
d. sudo apt-get remove mysql-common (非常重要)
```
上面的其实有一些是多余的，建议还是按照顺序执行一遍
2. 清理残留数据
`dpkg -l |grep ^rc|awk '{print $2}' |sudo xargs dpkg -P`
3. 安装 mysql
```
a. sudo apt-get install mysql-server
b. sudo apt-get install mysql-client
c. sudo apt-get install php5-mysql(安装php5-mysql是将php和mysql连接起来 )
```
一旦安装完成，MySQL服务器应该自动启动。您可以在终端提示符后运行以下命令来检查 MySQL 服务器是否正在运行：
4. 检查Mysql是否正在运行
`sudo netstat -tap | grep mysql`
当您运行该命令时，您可以看到类似下面的行：
```
root@ubuntu:~# sudo netstat -tap | grep mysql
tcp        0      0 localhost.localdo:mysql *:*                     LISTEN   
870/mysqld
```
如果服务器不能正常运行，您可以通过下列命令启动它：
`sudo /etc/init.d/mysql restart`
