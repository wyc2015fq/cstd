# MAC OS安装tar.gz版Mysql(5.6/5.7略有差别) - 零度的博客专栏 - CSDN博客
2017年05月26日 15:12:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：5019
       在 Mac 系统上,，安装 MySQL Server 一般是用 DMG 包在图形化界面下按提示安装，这个就不在介绍。此外 MySQL 还提供了 Compressed TAR Archive 二进制包安装方式, 即免安装解压运行版,，相比 DMG 包，免安装版过程更为简洁, 纯命令行操作, 更符合程序员的风格。
###        一、mysql5.6.x及以下安装方式
          MySQL版本: 5.6.24 (mysql-5.6.24-osx10.9-x86_64.tar.gz)
          MySQL下载: [http://dev.mysql.com/downloads/mysql/](http://dev.mysql.com/downloads/mysql/)
         找到下载的 MySQL tar.gz 文件位置, 浏览器下载的一般在当前用户的 Downloads 目录, 即 `/Users/<YourName>/Downloads`, 进入终端, 解压 tar.gz 文件：
```
cd /Users/<YourName>/Downloads
tar zxvf mysql-5.6.24-osx10.9-x86_64.tar.gz
```
         解压完成后得到 `mysql-5.6.24-osx10.9-x86_64` 目录, 移动解压目录到 MySQL 默认安装路径`/usr/local/mysql` 下,`/usr/local` 路径不存在时, 先
`sudo mkdir /usr/local` 创建。
** 执行以下操作安装：**
```
# 移动解压后的二进制包到安装目录
sudo mv mysql-5.6.24-osx10.9-x86_64 /usr/local/mysql
# 更改 mysql 安装目录所属用户与用户组
cd /usr/local
sudo chown -R root:wheel mysql
# 执行 scripts 目录下的 mysql_install_db 脚本完成一些默认的初始化(创建默认配置文件、授权表等)
cd /usr/local/mysql
sudo scripts/mysql_install_db --user=mysql
```
** 安装完成, 测试启动、重启与停止：**
```
#进入mysql安装目录
cd /usr/local/mysql
# 启动
sudo support-files/mysql.server start
# 重启
sudo support-files/mysql.server restart
# 停止
10.sudo support-files/mysql.server stop
# 检查 MySQL 运行状态
sudo support-files/mysql.server status
```
**          初始化 MySQL root 密码：**
```
# 需要 MySQL 在运行状态执行
cd /usr/local/mysql/bin
./mysqladmin -u root  password root123
```
**          通过自带的 MySQL Client 连接数据库：**
```
cd /usr/local/mysql/bin
./mysql -u root -p
```
       二、mysql5.7.X安装方式
       与 MySQL 5.6 相比，5.7 版本在安装时有两处不同：
       1）初始化方式改变， 从 `scripts/mysql_install_db --user=mysql` 初始化方式变成了`bin/mysqld --initialize --user=mysql` 方式；
       2） 初始密码生成改变,，5.6 的版本在 tar.gz 方式初始化完成后默认 root 密码为空,,5.7 版本在初始化完成后会生成一个临时的 root 密码。
MySQL 版本：5.7.9 (mysql-5.7.9-osx10.10-x86_64.tar.gz)
MySQL下载：[http://dev.mysql.com/downloads/mysql/](http://dev.mysql.com/downloads/mysql/)
      和 MySQL 5.6 tar gz 安装方式一样, 解压并移动到指定安装目录中并执行初始化：
```
# 解压
cd /Users/<YourName>/Downloads
tar zxvf mysql-5.7.9-osx10.10-x86_64.tar.gz
# 移动解压后的二进制包到安装目录
sudo mv mysql-5.7.9-osx10.10-x86_64 /usr/local/mysql
# 更改 mysql 安装目录所属用户与用户组
cd /usr/local
sudo chown -R root:wheel mysql
# 切换到 mysql 安装目录并执行初始化命令并记录生成的临时 root 密码
cd /usr/local/mysql
sudo bin/mysqld --initialize --user=mysql
```
**注意:**需要记录在初始化完成后命令行窗口中显示的临时 root 密码, 如图：
![](https://img-blog.csdn.net/20170526150240191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**        测试启动、重启与停止:**
```
cd /usr/local/mysql
# 启动
sudo support-files/mysql.server start
# 重启
sudo support-files/mysql.server restart
# 停止
sudo support-files/mysql.server stop
# 检查 MySQL 运行状态
sudo support-files/mysql.server status
```
**       修改 MySQL root 初始密码：**
```
# 需要 MySQL 服务在运行状态执行
cd /usr/local/mysql/bin
./mysqladmin -u root -p password 新密码
#输入生成的临时密码，回车
```
**连接登陆到mysql：**
```
cd mysql/bin
./mysql -u root -p
#输入密码
```
        进入成功如图：
![](https://img-blog.csdn.net/20170526150935654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       最后在提供几种其他修改方式root密码的方式：
```
方法1： 用SET PASSWORD命令 
首先登录MySQL。 
格式：mysql> set password for 用户名@localhost = password('新密码'); 
例子：mysql> set password for root@localhost = password('123'); 
方法2：用mysqladmin 
格式：mysqladmin -u用户名 -p旧密码 password 新密码 
例子：mysqladmin -uroot -p123456 password 123 
方法3：用UPDATE直接编辑user表 
首先登录MySQL。 
mysql> use mysql; 
mysql> update user set password=password('123') where user='root' and host='localhost'; 
mysql> flush privileges;
```

