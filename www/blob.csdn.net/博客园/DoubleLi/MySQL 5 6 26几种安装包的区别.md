# MySQL 5.6.26几种安装包的区别 - DoubleLi - 博客园






[http://downloads.mysql.com/archives/community/](http://downloads.mysql.com/archives/community/)



一、MySQL Installer 5.6.26

mysql-installer-community-5.6.26.0.msi， 364.2MB
MySQL Installer 提供了简单易用、向导式的 MySQL 软件的安装体验过程（目前只支持 Windows），包含的产品有：MySQL Server，所有的 connectors，Workbench 和示例模型，示例数据库，文档。该安装包大而全。

二、MySQL Community Server 5.6.26

a、mysql-5.6.26-win32.msi ， 34.7MB

b、mysql-5.6.26-win32.zip， 343.9MB

上面的a和b其实是等价的。a需要安装，b则是绿色压缩包。a和b都需要用户自行配置。以b为例：

1.复制解压后根目录下的my-default.ini，重命名为my.ini保存到相同路径下


2.修改my.ini
[mysqld]

# 设置mysql的安装目录，如果目录中带空格则必须加引号
basedir = "C:\Program Files\MySQL\MySQL Server 5.6"

# 设置mysql数据库的数据的存放目录，文件夹名必须是data
datadir = "C:\Program Files\MySQL\MySQL Server 5.6\data"

#设置数据库引擎为INNODB
default-storage-engine=INNODB

# 设置mysql服务器编码
character_set_server=utf8

# 设置mysql服务器字符集
collation-server=utf8_general_ci
port = 3306
server_id = 1

[client]
default-character-set = utf8

[WinMySQLAdmin]
Server = "C:\Program Files\MySQL\MySQL Server 5.6\bin\mysqladmin.exe"

3、安装mysql服务
从MS-DOS窗口进入目录C:\Program Files\MySQL\MySQL Server 5.6\bin，运行如下命令：
mysqld --install

4、启动mysql数据库
还在上面的命令窗口里面，输入命令：net start mysql
这样就启动了mysql服务。可以在windows-开始-运行-services.msc，查看到mysql的进程。

5、删除服务
执行mysqld --remove即可

6、设置root密码
执行mysqladmin -u root password 123456



7、推荐使用可视化第三方工具Navicat for MySQL。









