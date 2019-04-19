# 编写Nginx启停服务脚本 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:19:19[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：573
在/etc/init.d/目录下创建脚本
vim  /etc/init.d/nginx
编写脚本内容:（其中下面2行需要根据情况自行修改）
nginxd=**/**opt**/**nginx**/**sbin**/**nginx
nginx_config=**/**opt**/**nginx**/**conf**/**nginx.conf
脚本下载链接：[http://pan.baidu.com/s/1jGrK0Vs](http://pan.baidu.com/s/1jGrK0Vs) 密码：cxjb
更改脚本权限
chmod 777 /etc/init.d/nginx
测试
service nginx start
或/etc/init.d/nginx start
开机启动
chkconfig --list|grep nginx
chkconfig --add nginx
chkconfig --list|grep nginx
nginx           0:关闭    1:关闭    2:关闭    3:关闭    4:关闭    5:关闭    6:关闭
# chkconfig --level 2345 nginx on
# chkconfig --list|grep nginx
nginx           0:关闭    1:关闭    2:启用    3:启用    4:启用    5:启用    6:关闭
或将此命令加入到rc.local文件中，这样开机的时候nginx就默认启动了
vi /etc/rc.local
加入一行 /etc/init.d/nginx start 保存并退出，下次重启会生效。
## 艺搜参考
[http://www.imhdr.com/nginx/linux-centos-to-configure-nginx-mysql-php-fpm-redis-boot/](http://www.imhdr.com/nginx/linux-centos-to-configure-nginx-mysql-php-fpm-redis-boot/)
