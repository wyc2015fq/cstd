# php升级5.3到5.4，5.5，5.6 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月01日 16:36:16[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：641
Laravel要求php大于5.5.9，升级php5.3.3到5.6.14（最新版为 5.6.15 ）
Add EPEL and Remi repositories onto your system:
wget [http://dl.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm](http://dl.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm)
wget [http://rpms.famillecollet.com/enterprise/remi-release-6.rpm](http://rpms.famillecollet.com/enterprise/remi-release-6.rpm)
rpm -Uvh remi-release-6*.rpm epel-release-6*.rpm
enable your newly added Remirepo.
 Open the/etc/yum.repos.d/remi.repo
在想要升级的版本里，enabled=0 line to enabled=1:
Update to PHP 5.4, in[remi]section:
Update to PHP 5.5, in**[remi-php55]**section:
Now update yum:
yum update -y  //这个会升级很多额
报错：
Error: Cannot retrieve metalink for repository: epel. Please verify its path and try again
编辑/etc/yum.repos.d/epel.repo，把基础的恢复，镜像的地址注释掉
编辑[epel]下的baseurl前的#号去掉，mirrorlist前添加#号
再次运行
yum update -y
最后检查
php -v
PHP 5.6.24 (cli) (built: Jul 21 2016 07:42:08) 
Copyright (c) 1997-2016 The PHP Group
Zend Engine v2.6.0, Copyright (c) 1998-2016 Zend Technologies
