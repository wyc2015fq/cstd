# wdcp（WDlinux Control Panel） 快速安装RPM包，lanmp一件安装 - 在思索中前行！ - CSDN博客





2014年09月25日 16:24:49[_Tham](https://me.csdn.net/txl16211)阅读数：1401








lanmp一键安装包是wdlinux官网2010年开始推出的lamp,lnmp,lnamp(apache,nginx,php,mysql,zend,eAccelerator,pureftpd)应用环境的快速简易安装包，执行一个脚本，整个环境就安装完成就可使用，快速,方便易用,安全稳定.

但由于源码编译需要比较长的时间(30分钟至几小时不等),具体根据机器配置的不同,所用的时间也不同
而且在mysql安装编译时本身要比较久,时常会有遇到"卡住"的现像

在wdcp 1.X时发布过RPM包的安装
在lanmp,wdcp2的版本里,也有好多朋友建议和要求制作RPM安装包,但由于时间原因,也一直没怎么测试和制作

现在,已经制作打包完了,你可以用RPM安装,更简单和快速,5分钟左右
不过如果网速慢,可能下载都要5分钟不只了

支持系统:CentOS 5.X/wdlinux_base 5.X/wdOS 1.0,CentOS 6.X 
支持后台lamp,lnmp,lnamp后台可自由切换

wget [http://down.wdlinux.cn/in/lanmp_wdcp_24_in.sh](http://down.wdlinux.cn/in/lanmp_wdcp_24_in.sh)
sh lanmp_wdcp_24_in.sh
就可以了

卸载
sh lanmp_wdcp_24_in.sh uninstall
就可以
wdcp管理系统后台访问地址
http://ip:8080
默认用户密码
admin
wdlinux.cn

mysql默认的用户密码
root
wdlinux.cn

相关说明
所有软件安装目录/www/wdlinux
站点配置文件
/www/wdlinux/nginx/conf/vhost
/www/wdlinux/apache/conf/vhost

数据库配置文件/www/wdlinux/etc/my.cnf
数据库数据文件目录 /www/wdlinux/mysql/var 

相关软件版本
httpd-2.2.22
nginx-1.0.15
php-5.2.17
mysql-5.1.63
phpmyadmin-3.3.7
zend-3.3.3
eAccelerator-0.9.5.3
pure-ftpd-1.0.35 

更多一键包的说明请看
[http://www.wdlinux.cn/bbs/thread-2523-1-1.html](http://www.wdlinux.cn/bbs/thread-2523-1-1.html)

部分组件的可选安装，如memcache,mysqli,pdo_mysql,innodb等
具体的安装方法可见[http://www.wdlinux.cn/bbs/thread-1356-1-1.html](http://www.wdlinux.cn/bbs/thread-1356-1-1.html)



