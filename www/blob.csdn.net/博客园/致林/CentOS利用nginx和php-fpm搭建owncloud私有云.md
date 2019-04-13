
# CentOS利用nginx和php-fpm搭建owncloud私有云 - 致林 - 博客园






# [CentOS利用nginx和php-fpm搭建owncloud私有云](https://www.cnblogs.com/bincoding/p/6171507.html)
**1.安装owncloud**
CentOS下有一键安装命令
yum install owncloud
默认配置目录： /etc/owncloud
默认内容目录： /usr/share/owncloud
**2.安装配置nginx**
yum install nginx 
service nginx start 
chkconfig nginx on
修改nginx配置，添加：
location ~ .*\.php$ {
    fastcgi_pass127.0.0.1:9000;
}
修改nginx目录为owncloud
\#      root         /usr/share/nginx/html;
       root/usr/share/owncloud;
**3.安装php-fpm**
yum install php-fpm
service php-fpm start
chkconfig php-fpm on
最后登录浏览器测试
![](https://images2015.cnblogs.com/blog/771778/201612/771778-20161213211519776-1361058155.png)





