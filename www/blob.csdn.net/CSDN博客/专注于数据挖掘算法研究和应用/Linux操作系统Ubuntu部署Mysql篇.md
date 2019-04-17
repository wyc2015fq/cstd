# Linux操作系统Ubuntu部署Mysql篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月10日 21:13:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2515








## Ubuntu下Mysql部署

### 1 Mysql安装

1．在线自动安装：

1）安装服务端命令：sudo apt-get install mysql-server

安装过程提示输入root密码，装完已经自动配置好环境变量，可以直接使用mysql的命令；

——查看mysql正在运行：sudo netstat -tapln | grep mysql

——启动和停止mysql命令：/etc/init.d/mysql start|stop|restart

——启动远程访问mysql命令：sudo gedit /etc/mysql/my.cnf

在打开的文件中将bind-address=127.0.0.1设置成要访问的IP地址即可，或者直接注释；

——etc/init.d/mysql start (stop) 为启动和停止服务器 

/etc/mysql/ （或者/etc/my.cnf）主要配置文件所在位置 my.cnf 

/var/lib/mysql/ 放置的是数据库表文件夹，这里的mysql相当于windows下mysql的date文件夹；

——进入mysql命令：mysql -u root -p 

       提示输入密码即可；

2）安装客户端命令：sudo apt-get install mysql-client

2．离线手动安装： 未验证

下载地址：[http://www.mysql.com/downloads/mysql/5.5.html](http://www.mysql.com/downloads/mysql/5.5.html)

版本：mysql-5.5.15-linux2.6-i686.tar.gz

       安装过程：

       1）groupadd mysql

　　2）mkdir /home/mysql

　　3）useradd -g mysql -d /home/mysql mysql

　　4）copy mysql-5.5.15-linux2.6-i686.tar.gz到/usr/local目录

　　5）解压：mysql-5.5.15-linux2.6-i686.tar.gz

　　6）ln -s mysql-5.5.15-linux2.6-i686 mysql

　　7）cd /usr/local/mysql

　　8）chown -R mysql .

　　9）chgrp -R mysql .

　　10） scripts/mysql_install_db --user=mysql (一定要在mysql目录下执行，注意输出的文字里有修改root密码和启动mysql的命令）

11）为root设置密码： ./bin/mysqladmin -u root password 'password'

### 2 WorkBench安装

Mysql图形化操作工具：

1．  下载地址：[http://dev.mysql.com/downloads/workbench/](http://dev.mysql.com/downloads/workbench/)

版本：mysql-workbench-gpl-5.2.34-lubu1010-i386.deb

2．执行命令：sudo dpkg -i mysql-workbench-gpl-5.2.34-lubu1010-i386.deb  ??

   备注：如是rpm包，则执行命令：sudo rpm -i xxx.rpm；

   关键：以root身份执行，sudo su命令；

   如果发生安装不成功提示，可以输入以下命令：apt-get -f install，会修复失败的原因，然后再执行sudo dpkg命令安装deb文件。
            


