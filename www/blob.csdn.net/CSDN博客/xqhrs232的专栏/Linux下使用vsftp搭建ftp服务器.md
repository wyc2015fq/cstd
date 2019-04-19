# Linux下使用vsftp搭建ftp服务器 - xqhrs232的专栏 - CSDN博客
2016年03月23日 09:35:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：316
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址:[http://www.cnblogs.com/zemliu/archive/2012/06/07/2539135.html](http://www.cnblogs.com/zemliu/archive/2012/06/07/2539135.html)
相关文章
1、Linux平台下快速搭建FTP服务器----[http://jingyan.baidu.com/article/380abd0a77ae041d90192cf4.html](http://jingyan.baidu.com/article/380abd0a77ae041d90192cf4.html)
2、Linux中安装配置ftp服务器方法----[http://www.111cn.net/sys/linux/53259.htm](http://www.111cn.net/sys/linux/53259.htm)
3、[Linux
 FTP 服务器配置简单说明](http://www.cnblogs.com/hnrainll/archive/2011/02/16/1956539.html)----[http://www.cnblogs.com/hnrainll/archive/2011/02/16/1956539.html](http://www.cnblogs.com/hnrainll/archive/2011/02/16/1956539.html)

1.安装vsftp，我就不多说了，我用的是ubuntu，直接apt-get就完了
2.配置vsftpd.conf, vim /etc/vsftpd.conf
下面说说里面比较重要的选项
```
1 anonymous_enable=NO #不允许匿名用户
 2 
 3 local_enable=YES #允许使用本地用户账号登陆
 4 
 5 write_enable=YES #允许ftp用户写数据
 6 
 7 connect_from_port_20=YES #通过20端口传输数据
 8 
 9 chroot_local_user=YES #将用户锁定在自己的主目录，如 /home/ftpuser,这样ftpuser这个用户就不能访问除此目录之外的目录
10 
11 chroot_list_enable=YES #启用不锁定用户在主目录的名单
12 
13 chroot_list_file=/etc/vsftpd.chroot_list #当 chroot_list_enable 启用时，此列表生效，列表中的用户将不受锁定限制，格式为一行一个用户名
```
3.创建用户组合用户
```
1 groupadd ftpgroup
2 
3 useradd ftpuser -g ftpgroup -d /home/ftpuser -M　　#创建一个group为ftpgroup，home目录为/home/ftpuser，-M为系统不创建home目录，因为之前已经指定
4 
5 usermod -s /usr/sbin/nologin ftpuser #将ftpuser设为不可作为普通用户登录
```
4.启动vsftp
```
1 /etc/init.d/vsftpd start
```
5.使用filezilla或者直接在浏览器上输入你的ftp服务器ip，登录即可使用
分类: [Linux](http://www.cnblogs.com/zemliu/category/377797.html)
