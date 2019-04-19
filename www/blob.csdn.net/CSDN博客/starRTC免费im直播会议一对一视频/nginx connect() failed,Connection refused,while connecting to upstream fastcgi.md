# nginx connect() failed,Connection refused,while connecting to upstream fastcgi - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月19日 11:26:55[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：205
所属专栏：[后台架构](https://blog.csdn.net/column/details/31054.html)
connect() failed (111: Connection refused) while connecting to upstream fastcgi://127.0.0.1:9000
netstat -ant | grep 9000
vim /etc/php/7.0/fpm/php-fpm.conf
or /etc/php/7.0/fpm/pool.d/www.conf
发现 listen = /run/php/php7.0-fpm.sock
修改nginx fastcgi_pass
- location ~ \.php$ {
- fastcgi_pass unix:/tmp/php-cgi.sock;
connect() to unix:/run/php/php7.0-fpm.sock failed (13: Permission denied)
The php user was www-data but the nginx user was nginx.
 /etc/php/7.0/fpm/pool.d/[www.conf](http://www.conf)
You can change nginx to use www-data user,
or, as I did, add nginx user to the www-data group using sudo usermod -a -G www-data nginx
