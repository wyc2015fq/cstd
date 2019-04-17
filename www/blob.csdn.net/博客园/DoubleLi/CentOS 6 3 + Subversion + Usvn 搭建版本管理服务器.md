# CentOS 6.3 + Subversion + Usvn 搭建版本管理服务器 - DoubleLi - 博客园






一． Subversion 简介 
Subversion是一个自由，开源的版本控制系统。在Subversion管理下，文件和目录可以超越时空。Subversion将文件存放在中心版本库里。这个版本库很像一个普通的文件服务器，不同的是，它可以记录每一次文件和目录的修改情况。这样就可以籍此将数据恢复到以前的版本，并可以查看数据的更改细节。正因为如此，许多人将版本控制系统当作一种神奇的“时间机器”。 
二． 环境准备 
用1台cent os 6.3。 

[root@localhost ~]# uname -a
Linux yxp2p.server 2.6.32-279.el6.x86_64 #1 SMP Fri Jun 22 12:19:21 UTC 2012 x86_64 x86_64 x86_64 GNU/Linux

[root@localhost ~]# vim /etc/selinux/config    

![](https://images0.cnblogs.com/blog/387466/201410/221132077778873.png)

设置SELINUX=disabledt重启后关闭selinux 。

[root@localhost  ~]# setenforce 0
setenforce: SELinux is disabled

查看是否关闭了selinux。



三、安装

1. 安装所需软件 
[root@localhost ~]# yum install subversion httpd mod_dav_svn php php-mysql mysql-server perl-DBI perl-DBD-mysql mysql-devel mod_auth_mysql 

![](https://images0.cnblogs.com/blog/387466/201410/221136141058718.png)

查看 subversion 是否安装成功，如下图：

![](https://images0.cnblogs.com/blog/387466/201410/221139358408253.png)

2.启动httpd、mysqld服务

[root@localhost ~]# service httpd start
Starting httpd: httpd: Could not reliably determine the server's fully qualified domain name, using yxp2p.server for ServerName
[ OK ]
[root@localhost ~]# service mysqld start
Starting mysqld: [ OK ]



启动后设置mysqld密码，后面安装usvn时需要用

[root@localhost ~]# mysqladmin -u root password 123123



3. 配置开机启动 

![](https://images0.cnblogs.com/blog/387466/201410/221143013877858.png)

[root@localhost ~]# chkconfig svnserve on  设置svn服务随机启动。



四．配置Usvn

[root@localhost ~]# wget https://github.com/usvn/usvn/archive/1.0.7.tar.gz

[root@localhost ~]# tar zxvf 1.0.7 

[root@localhost ~]# mv usvn-1.0.7/ /home/webroot/usvn 

[root@localhost ~]# chown -R apache:apache /home/webroot/usvn 

[root@localhost ~]# vim  /etc/httpd/conf/httpd.conf 

Alias /usvn "/home/webroot/usvn/public"   
<Directory "/home/webroot/usvn/public">     

　　Options +SymLinksIfOwnerMatch     

　　AllowOverride All     

　　Order allow,deny     

　　Allow from all 

</Directory>

配置完后重启httpd 
[root@localhost ~]# service httpd restart 
通过IE打开http://172.16.11.89/usvn/就可以按步骤一步步完成，此时如果打不开，请检查你的iptables、selinux是否有限制。



![](https://images0.cnblogs.com/blog/387466/201410/221148431529747.png)

出现上面的提示，尝试关闭SElinux看能否解决。



![](https://images0.cnblogs.com/blog/387466/201410/221149233551332.png)



访问正常将会出现下面界面。

![](https://images0.cnblogs.com/blog/387466/201410/221149565126135.png)

第四步如果使用默认值不行，请做调整

![](https://images0.cnblogs.com/blog/387466/201410/221150510127751.png)




改为以下路径则可以顺利进入下一步



我本地的的仓库地址是 /home/subversion， 注意设置目录权限

[root@localhost ~]# mkdir -p /home/subversion

[root@localhost ~]# chown -R apache:apache /home/subversion



![](https://images0.cnblogs.com/blog/387466/201410/221153475747578.png)




改为以下路径则可以顺利进入下一步 
填入上面创建的mysqld用户名和密码，勾选“创建数据库”，此处如果你没有安装php-pdo则会报错

![](https://images0.cnblogs.com/blog/387466/201410/221154333405729.png)



创建一个账号用于登陆管理usvn



![](https://images0.cnblogs.com/blog/387466/201410/221155338244785.png)



![](https://images0.cnblogs.com/blog/387466/201410/221156057938519.png)



复制下面的代码到/etc/httpd/conf/httpd.conf里

![](https://images0.cnblogs.com/blog/387466/201410/221158105746297.png)



[root@localhost usvn]# vim /etc/httpd/conf/httpd.conf

![](https://images0.cnblogs.com/blog/387466/201410/221210589967240.png)



点上面的“连接到USVN”登陆

![](https://images0.cnblogs.com/blog/387466/201410/221159167931634.png)



至此整个subversion及USVN搭建完成。需要用USVN测试创建用户、用户组、项目是否正常。



参考链接：

http://wenku.baidu.com/link?url=yKLV9Z1UyA3SCZqcZkDM0miWl5LWLgEJvOh_cY-iPQRIOP23sWg2sNgP_2-is2h_jeGyQfKYE0YSOL8ZMnf-ZlsQzGg-wISVWsU3FynUKwi

http://wiki.ubuntu.org.cn/index.php?title=SubVersion

http://wenku.baidu.com/view/b98a62cb4693daef5ef73d5d.html?re=view

http://zhumeng8337797.blog.163.com/blog/static/1007689142011341202582/

http://wuyaweiwude.iteye.com/blog/1915064

http://www.jb51.net/os/RedHat/73031.html

http://www.linuxidc.com/Linux/2011-05/35626.htm









