# 在Ubuntu中安装PHP，MySQL，Nginx和phpMyAdmin - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月19日 11:28:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：90
php
apt install php
apt-get install php7.0
apt-get -y install php7.0-fpm
缺少 [mysqli](http://10.80.19.177/phpmyadmin/url.php?url=https%3A%2F%2Fsecure.php.net%2Fmanual%2Fen%2Fbook.mysqli.php) 扩展。请检查 PHP 配置。
apt install php7.0-*
mysql:
apt-get install [mysql](https://www.2cto.com/database/MySQL/)-server mysql-client
mysql.service: Main process exited, code=exited, status=1/FAILURE
systemctl start mysql.service
可能是端口被占用了
安装nginx
apt-get install libpcre3 libpcre3-dev
apt-get install zlib1g-dev
apt-get install openssl
502
[error] 22504#0: *5 connect() failed (111: Connection refused) while connecting to upstream,
upstream: "fastcgi://127.0.0.1:9000"
vim /etc/php/7.0/fpm/pool.d/www.conf
;listen = /var/run/php5-fpm.sock listen = 127.0.0.1:9000
;listen.allowed_clients = 127.0.0.1
pkill php-fpm
/usr/sbin/php-fpm7.0
