# centos7下yum安装mysql - DoubleLi - 博客园






一般在linux下安装mysql 的步骤如下:

安装前看是否安装过mysql，

yum list installed mysql*

如果有就用yum remove卸载mysql

查看yum库下是否有mysql-server

yum list | grep mysql 或 yum -y list mysql*

如果没有（一般在centos7下没有）

wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm

rpm -ivh mysql-community-release-el7-5.noarch.rpm

然后安装mysql

yum install mysql-server

yum install mysql-devel

如果有mysql-server，则按下面步骤

安装mysql客户端：

yum install mysql 


安装mysql 服务器端：

yum install mysql-server

yum install mysql-devel

然后在mysql配置文件/etc/my.cnf的[mysqld]中加入character-set-server=utf8

然后启动mysql服务

service mysqld start

登录mysql

mysql -u root -p

刚安装密码为空，直接按回车

切换到mysql数据库：use mysql;


然后修改密码UPDATE user SET password=password("你的密码") WHERE user='root';

刷新权限

FLUSH PRIVILEGES;

之后采用mysql -h 127.0.0.1 -u root -p 登录然后输入密码

如果没有-h那么是无密码登录 登录的用户没有任何权限，当然能修改密码














