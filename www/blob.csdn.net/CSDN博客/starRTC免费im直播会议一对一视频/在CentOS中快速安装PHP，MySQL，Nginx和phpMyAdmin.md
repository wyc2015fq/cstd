# 在CentOS中快速安装PHP，MySQL，Nginx和phpMyAdmin - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 10:54:04[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：284
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
安装环境
yum install -y autoconf automake gcc gcc-c++ git libtool make nasm pkgconfig zlib-devel
## 安装php
yum install -y php php-fpm php-mysql  php-devel php-mbstring php-gd
启动php-fpm
service php-fpm start
将php-fpm加入启动项
chkconfig --levels 2345 php-fpm on
查看
chkconfig --list | grep php-fpm
## 安装mysql
- yum安装
yum install -y mysql-server mysql mysql-devel
开启MySQL Server daemon (mysqld)
service mysqld start
用root帐号登录MySQL Server
mysql -u root
修改 root 帐号的密码: (注: 这一步完成之后，下次使用mysql -p -u root登录)
mysql> SET PASSWORD FOR 'root'@'localhost' = PASSWORD('password');
删除除root外的其它用户
select user,host from mysql.user;
delete from mysql.user where not (user="root");
mysql> FLUSH PRIVILEGES;
select user,host from mysql.user;
mysql> quit;
设置mysqld开机启动
chkconfig --level 2345 mysqld on && service mysqld restart && chkconfig --list | grep mysqld
======================================================
Centos7中用MariaDB代替了mysql数据库。
CentOS 7的yum源中貌似没有正常安装mysql时的mysql-sever文件，需要去官网上下载
# wget [http://dev.mysql.com/get/mysql-community-release-el7-5.noarch.rpm](http://dev.mysql.com/get/mysql-community-release-el7-5.noarch.rpm)
# rpm -ivh mysql-community-release-el7-5.noarch.rpm
# yum install mysql-community-server
附：
rpm -e 需要卸载的安装包
在卸载之前，通常需要使用rpm -q …命令查出需要卸载的安装包名称。
ruby-1.8.7.352-7.el6_2.x86_64 is a duplicate with ruby-1.8.7-1.el6.x86_64
yum erase ruby
成功安装之后重启mysql服务
# service mysqld restart
## 如果仍不能下载，把原来的yum源里自定义的源删除。
## =====================================
## 安装web服务器nginx
下载链接：[http://pan.baidu.com/s/1sjqknO5](http://pan.baidu.com/s/1sjqknO5) 密码：7q59
yum install -y pcre-devel zlib-devel
tar -zxf nginx-1.5.3.tar.gz
cd nginx-1.5.3
./configure --prefix=/opt/nginx
make && make install
查看配置信息是否正确（可以一起复制）
cd /opt/nginx/sbin/
./nginx -t
启动Nginx
/opt/nginx/sbin/nginx
开机启动
echo /opt/nginx/sbin/nginx >> /etc/rc.local
开放80端口
/sbin/iptables -I INPUT -p tcp --dport 80 -j ACCEPT
/etc/init.d/iptables save
service iptables restart
或直接关闭防火墙（可选）
service iptables stop 
开机关闭防火墙（可选）
chkconfig --level 0123456 iptables off
配置Nginx
vim /opt/nginx/conf/nginx.conf
安装vim
yum install -y vim
去掉注释并修改脚本路径为/opt/nginx/html
*# pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000**#*
   location ~ \.php$ **{**
         root           html;
         fastcgi_pass   127.0.0.1:9000;
         fastcgi_index  index.php;
         fastcgi_param  SCRIPT_FILENAME  **/**opt**/**nginx**/**html$fastcgi_script_name;
         include        fastcgi_params;
   **}**
重新加载
cd /opt/nginx/sbin
./nginx -s reload
service php-fpm restart
## 启停nginx的脚本
参见：编写Nginx启停服务脚本
之后就可以service nginx restart重启了
## 安装phpmyadmin
链接：[http://pan.baidu.com/s/1i3pKD3f](http://pan.baidu.com/s/1i3pKD3f) 密码：et1z
tar -jxf phpMyAdmin-4.0.6-all-languages.tar.bz2
mv phpMyAdmin-4.0.6-all-languages phpmyadmin
mv phpmyadmin/ /opt/nginx/html/
cd /opt/nginx/html/phpmyadmin/
cp config.sample.inc.php config.inc.php
修改用户名和密码
vim  config.inc.php
修改下面2行：
$cfg['Servers'][$i]['controluser'] = 'root';// MySQL用户
$cfg['Servers'][$i]['controlpass'] = 'password'; // MySQL用户密码
重新加载
cd /opt/nginx/sbin
./nginx -s reload
service php-fpm restart
问题：Call to undefined function PMA_DBI_num_rows() in /opt/nginx/html/phpmyadmin/libraries/navigation/Nodes/Node.class.php
yum install php-mysql
