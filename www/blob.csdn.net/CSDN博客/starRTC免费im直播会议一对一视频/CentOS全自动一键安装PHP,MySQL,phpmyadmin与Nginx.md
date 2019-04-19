# CentOS全自动一键安装PHP,MySQL,phpmyadmin与Nginx - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:19:53[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：700
运行install_nginx.sh即可
1，需要修改install_nginx.sh中的相应路径：
*#存放源代码和本脚本的目录*compile_dir=**/**root**/**nginx_compile
*#备份*conf_backup_dir=**/**root**/**nginx_compile**/**_bak_
*#nginx安装目录*nginx_install_dir=**/**mnt**/**nginx
nginx_vhost_conf=**/**mnt**/**nginx**/**conf**/**vhosts
website_dir=**/**mnt**/**nginx**/**html**/**elesos_com
 
mv $compile_dir/elesos_com.conf $nginx_vhost_conf
2，需要修改nginx.conf中以**/**mnt开头的相关路径
 
3，需要修改elesos_com.conf中的相关路径与域名。
 
4，需要修改config.inc.php中的数据库连接用户与密码
 
5，需要修改nginx文件中的相关路径
安装完成后需要登录mysql修改root密码，参见[在CentOS中快速安装PHP，MySQL，Nginx和phpMyAdmin](http://192.168.1.100/wiki_elesos_com/index.php?title=%E5%9C%A8CentOS%E4%B8%AD%E5%BF%AB%E9%80%9F%E5%AE%89%E8%A3%85PHP%EF%BC%8CMySQL%EF%BC%8CNginx%E5%92%8CphpMyAdmin&action=edit&redlink=1)
最后可以通过service nginx restart进行重启。
可以通过访问c测试网站是否能够访问。如果不能访问，请查看防火墙设置。
