# 12 条用于 Linux 的 MySQL/MariaDB 安全最佳实践 - 文章 - 伯乐在线
原文出处： [tecmint](https://www.tecmint.com/mysql-mariadb-security-best-practices-for-linux/)   译文出处：[oschina](https://www.oschina.net/translate/mysql-mariadb-security-best-practices-for-linux?print)
MySQL 是世界上最流行的开源数据库系统，MariaDB(一个 MySQL 分支)是世界上增长最快的开源数据库系统。在安装 MySQL 服务器之后，在默认配置下是不安全的，确保数据库安全通常是通用数据库管理的基本任务之一。
这将有助于增强和提升整个 Linux 服务器的安全性，因为攻击者总是扫描系统任意部分的漏洞，而数据库在过去是重点目标区域。一个常见的例子是对 MySQL 数据库的 root 密码的强制破解。
在本指南中，我们将会讲解对开发者有帮助的 MySQL/MariaDB 的 Linux 最佳安全实践。
# 1. 安全地安装 MySQL
这是安装 MySQL 服务器后第一个建议的步骤，用于保护数据库服务器。这个脚本可以帮助您提高 MySQL 服务器的安全性：
- 如果您在安装期间没有设置 root 帐户的密码，马上设置它
- 通过删除可从本地主机外部访问的 root 帐户来禁用远程 root 用户登录
- 删除匿名用户帐户和测试数据库，默认情况下，所有用户、甚至匿名用户都可以访问这些帐户和测试数据库


```
# mysql_secure_installation
```
在运行上述命令之后，设置 root 密码并通过输入 [Yes/Y] 和按下 [Enter] 键来回答一系列问题。
![](http://jbcdn2.b0.upaiyun.com/2018/01/f36cccbc6907b330961ecc571419f939.png)
*安全安装 MySQL 情况界面*
# 2. 将数据库服务器绑定到 Loopback 地址
此配置将限制来自远程机器的访问，它告诉 MySQL 服务器只接受来自本地主机的连接。你可以在主配置文件中进行设置。


```
# vi /etc/my.cnf	                   [RHEL/CentOS]	
# vi /etc/mysql/my.conf                    [Debian/Ubuntu] 
OR
# vi /etc/mysql/mysql.conf.d/mysqld.cnf    [Debian/Ubuntu]
```
在 `[mysqld]` 部分中添加下面这一行


```
bind-address = 127.0.0.1
```
# 3. 禁用 MySQL 的 LOCAL INFILE
作为安全性增强的一部分，您需要禁用 **local_infile**，使用下面的指令以防止在 `[mysqld]` 部分从 MySQL 中访问底层文件系统。


```
local-infile=0
```
# 4. 修改 MySQL 的默认端口
设置端口变量用于监听 TCP/IP 连接的 MySQL 端口号。默认端口号是 3306，但是您可以在 **[mysqld] **中修改它。


```
Port=5000
```
# 5、启用 MySQL 日志
日志是了解服务运行过程中发生了什么的最好的方法之一，在受到任何攻击的时候都可以很容易的从日志里看到任何入侵相关的行为。可以通过将下边的变量添加到配置文件[mysqld]部分来开启mysql日志功能。


```
log=/var/log/mysql.log
```
# 6、设置合适的 MySQL 文件的访问权限
确保你已经为所有的 mysql 服务文件和数据路径设置了合适的访问权限。文件 /etc/my.conf 只能由 root 用户修改，这样就可以阻止其他用户修改数据库服务的配置。


```
# chmod 644 /etc/my.cnf
```
# 7、删除 MySQL shell 历史
你在 MySQL shell 中执行的所有的命令都会被 mysql 客户端保存到一个历史文件：~/.mysql_history。这样是很危险的，因为对于你创建过的任何用户账户，所有的在 shell 输入过的用户名和密码都会记录到历史文件里面。


```
# cat /dev/null > ~/.mysql_history
```
# 8. 不要在命令行中运行 MySQL 命令
正如你所知道的，你在终端上输入的所有命令都会被存储在一个历史文件中，具体取决于你正在使用的shell（例如 bash 的 shell 历史文件放在 **~/.bash_history**）。攻击者访问这个历史文件可以很容易地看到记录在那里的任何密码。
非常不建议在命令行里面输入密码，如下：


```
# mysql -u root -ppassword_
```
![](http://jbcdn2.b0.upaiyun.com/2018/01/f505f9f9145e6e2ddd243ec3d3ebf9f5.png)
*使用密码连接 MySQL*
当你查看命令行历史文件的最后的部分时，可以看到之前输入过的密码。


```
# history
```
![](http://jbcdn2.b0.upaiyun.com/2018/01/ec120994b3b6eb87f569f32434861ce9.png)
*查看命令行输入历史*
推荐连接 MySQL 的方式是


```
# mysql -u root -p
Enter password:
```
# 9. 定义特定应用的数据库用户
对于每一个在服务器上运行的应用，只设置一个与该应用相关的数据库用户。例如你有一个 wordpress 网站，如下创建一个 wordpress 的数据库用户：


```
# mysql -u root -p
MariaDB [(none)]> CREATE DATABASE osclass_db;
MariaDB [(none)]> CREATE USER 'osclassdmin'@'localhost' IDENTIFIED BY 'osclass@dmin%!2';
MariaDB [(none)]> GRANT ALL PRIVILEGES ON osclass_db.* TO 'osclassdmin'@'localhost';
MariaDB [(none)]> FLUSH PRIVILEGES;
MariaDB [(none)]> exit
```
并且要记住对于不再使用的数据库用户要删掉。
# 10. 使用额外的安全插件和库
MySQL 包含许多安全插件：验证客户端连接到 MySQL 服务器的请求、密码校验和敏感信息的安全存储等，这些都在免费版本中提供。
在这里可查看更多：[https://dev.mysql.com/doc/refman/5.7/en/security-plugins.html](https://dev.mysql.com/doc/refman/5.7/en/security-plugins.html)
# 11. 定期修改 MySQL 密码
定期修改密码是一个常见的信息/应用/系统安全建议。多久修改一次密码由你内部的安全策略决定。定期修改密码可以阻止长期跟踪你的“窥探者”，获取你的密码，登录你的 MySQL 服务器。


```
MariaDB [(none)]> USE mysql;MariaDB [(none)]> UPDATE user SET password=PASSWORD('YourPasswordHere') WHERE User='root' AND Host = 'localhost';MariaDB [(none)]> FLUSH PRIVILEGES;
```
# 12. 定期更新 MySQL Server 包
强烈建议定期从官方仓库更新 mysql/mariadb 包来获取最新的安全更新和错误改进。通常情况下操作系统中默认的包是过时的。


```
# yum update
# apt update
```
在对 mysql/mariadb server 进行任何修改之后，要重启服务。


```
# systemctl restart mariadb		#RHEL/CentOS
# systemctl restart mysql		#Debian/Ubuntu
```
分享到此结束，欢迎大家在文章下面评论，并且和我们分享文章中没有提到的安全提示。
