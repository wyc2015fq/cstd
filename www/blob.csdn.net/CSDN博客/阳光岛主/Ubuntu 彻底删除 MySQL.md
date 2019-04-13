
# Ubuntu 彻底删除 MySQL - 阳光岛主 - CSDN博客

2013年10月02日 22:18:13[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6982



# 删除 MySQL
sudo apt-get autoremove --purge mysql-server-5.0
sudo apt-get remove mysql-server
sudo apt-get autoremove mysql-server
sudo apt-get remove mysql-common //这个很重要
上面的其实有一些是多余的


# 清理残留数据
dpkg -l |grep ^rc|awk '{print $2}' |sudo xargs dpkg -P
# 安装 MySQL
sudo apt-get install mysql-server
sudo apt-get install mysql-client
sudo apt-get install php5-mysql 
// 安装php5-mysql 是将php和mysql连接起来
一旦安装完成，MySQL 服务器应该自动启动。您可以在终端提示符后运行以下命令来检查 MySQL 服务器是否正在运行：
sudo netstat -tap | grep mysql当您运行该命令时，您可以看到类似下面的行：
tcp 0 0 localhost.localdomain:mysql *:* LISTEN -
如果服务器不能正常运行，您可以通过下列命令启动它：
sudo /etc/init.d/mysql restart
进入mysql
$mysql -uroot -p 管理员密码

# 配置 MySQL 的管理员密码：
sudo mysqladmin -u root password newpassword
sudo apt-get autoremove mysql-client
sudo apt-get autoremove mysql-client-core-5.5


**参考推荐：**
[ubuntu图下安装软件出现You might want to run 'apt-get -f install' to correct these解决](http://blog.csdn.net/glorin/article/details/6312332)
[UBUNTU 彻底删除 MYSQL 然后重装 MYSQL](http://www.cnblogs.com/steven_oyj/archive/2010/05/24/1742808.html)
[Ubuntu Server 12.04 安装mysql 5.6.10学习笔记](http://www.111cn.net/sys/linux/46343.htm)
[MySQL 5.1参考手册](http://www.phpchina.com/resource/manual/mysql/)（推荐）



