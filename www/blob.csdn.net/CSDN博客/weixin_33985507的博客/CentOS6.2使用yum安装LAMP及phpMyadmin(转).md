# CentOS6.2使用yum安装LAMP及phpMyadmin(转) - weixin_33985507的博客 - CSDN博客
2013年05月17日 23:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
介绍如何在CentOs6.2下面使用YUM配置安装LAMP环境，一些兄弟也很喜欢使用编译的安装方法，个人觉得如果不是对服务器做定制，用yum安装稳定简单，何必去download&make&make install呢。
Step1. 为centos配置rpmforge及epel源。centos官方的源其实也够用，不过像php的一些扩展如php-mcrypt在官方源中并没有。
rpmforge源可以在 http://pkgs.repoforge.org/rpmforge-release/中下载安装，名为rpmforge- release-*.i686.rpm 或 rpmforge-release-*.x86_64.rpm,找最新的版本
epel源可以在http://mirrors.ustc.edu.cn/Fedora/epel/6/ 中找到，32系统在i386文件夹中，64位的在x86_64文件夹中，名为epel-release-6-*.noarch.rpm，找最新的版本
这里以32位系统为例子，
#如果提示The requested URL returned error: 404，请自行换成最新的版本  
rpm -ivh http://pkgs.repoforge.org/rpmforge-release/rpmforge-release-0.5.2-2.el6.rf.i686.rpm  
rpm -ivh http://download.Fedoraproject.org/pub/epel/6/i386/epel-release-6-5.noarch.rpm  
Step 2. 安装php/mysql/httpd
#安装apache  
yum -y install httpd httpd-devel   
#安装mysql  
yum -y install mysql mysql-devel mysql-server mysql-libs  
#安装php  
yum -y install php  
#安装php扩展  
yum -y install php-mysql php-gd php-imap php-ldap php-odbc php-mbstring php-devel php-soap php-cli php-pdo   
yum -y install php-mcrypt php-tidy php-xml php-xmlrpc php-pear    
#其它不常用php扩展, www.linuxidc.com 可不用安装  
yum -y install php-pecl-memcache php-eaccelerator  
#安装phpmyadmin  
yum -y install phpmyadmin   
#安装完成之后，启动  
service httpd start  
service mysqld start  
Step 3.配置httpd、mysqld开机自启动
chkconfig mysqld on  
chkconfig httpd on  
Step 4.配置phpmyadmin
修改/usr/share/phpmyadmin/config.inc.php
#将这里abcde文字修改为自己的字符，可任意  
$cfg['blowfish_secret'] = 'abcde';  
常见问题
1. phpmyadmin提示未开启php-mcrypt,这是因为没有安装php-mrcrpt扩展。在官方源没有php-mrcrpt扩展，请安装epel源后再yum install php-mcrypt
2. phpmyadmin无法访问
phpmyadmin默认只能使用http://localhost/phpmyadmin来访问，可以修改/etc/httpd/conf.d/phpmyadmin.conf中对应部分为
<Directory "/usr/share/phpmyadmin">  
  Order Deny,Allow  
#  Deny from all  
  Allow from All  
</Directory>  
不过强烈不支持这么做的，把phpmyadmin暴露了不安全
注意：
apache解析php，修改httpd.conf文件，添加
Addtype application/x-httpd-php .php .phtml
Addtype application/x-httpd-php-source .phps
myql可以远程登录
设置mysql密码
mysql>; USE mysql; 
mysql>; UPDATE user SET Password=PASSWORD('newpassword') WHERE user='root'; 
mysql>; FLUSH PRIVILEGES; 
1.2.3 允许远程登录 
mysql -u root -p 
Enter Password: <your new password> 
mysql>GRANT ALL PRIVILEGES ON *.* TO '用户名'@'%' IDENTIFIED BY '密码' WITH GRANT OPTION; 
完成后就能用mysql-front远程管理mysql了。 
设为开机启动 
chkconfig mysqld on 
