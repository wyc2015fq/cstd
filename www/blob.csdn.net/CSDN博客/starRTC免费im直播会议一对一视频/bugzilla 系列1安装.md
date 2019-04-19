# bugzilla 系列1安装 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月05日 17:04:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1270

**安装好mysql**
yum install gcc perl* mod_perl-devel -y
wget [https://ftp.mozilla.org/pub/mozilla.org/webtools/bugzilla-5.0.3.tar.gz](https://ftp.mozilla.org/pub/mozilla.org/webtools/bugzilla-5.0.3.tar.gz)
tar -zxf [bugzilla-5.0.3.tar.gz](https://ftp.mozilla.org/pub/mozilla.org/webtools/bugzilla-5.0.3.tar.gz)
mv bug
create database bugzilla_db;
//root用户不用授权，如果是其它用户，需要授权
GRANT ALL PRIVILEGES ON bugzilla_db.* TO root@localhost IDENTIFIED BY 'password';
check for all required bugzilla modules before the install:
./checksetup.pl
After the check is done, we will see some missing modules that needs to be installed:
Type the command below to install all missing modules automatically:
/usr/bin/perl install-module.pl --all
Lets do a check again to make sure all the modules did install
./checksetup.pl
If all is good, we will get a message to edit the localconfig file for installation.
./localconfig
Make sure you input the correct database name, user, and password we created earlier
Run ./checksetup.pl again
./checksetup.pl
If all is well, checksetup.pl should now successfully configure Bugzilla.

用httpd吧，用其它端口即可
附：
配置mysql
increase the maximum attachment size and make it possible to search for short words and terms:
- Alter on Line 52: max_allowed_packet=100M
- Add as new line 32, in the [mysqld] section: ft_min_word_len=2
Add the following to the end of httpd.conf file
<Directory/var/www/html/bugzilla>
 AddHandler cgi-script .cgi Options +Indexes +ExecCGI DirectoryIndex index.cgi AllowOverride Limit FileInfo Indexes</Directory>
At this point we are nearly done. Execute the following line to comment out a line in the .htaccess file that the Bugzilla installation script
 created:
*[root@localhost bugzilla]# sed -i 's/^Options -Indexes$/#Options -Indexes/g' ./.htaccess*
403
**Forbidden**
SELinux

配置：
- **urlbase**: http://<servername>/bugzilla/ or http://<ip address>/bugzilla/
nginx:
用gmail作邮箱
- **mail_delivery_method**: SMTP
- **mailfrom**: new_gmail_address@gmail.com
- **smtpserver**: ~~smtp.gmail.com:465~~
- **smtp_username**: new_gmail_address@gmail.com
- **smtp_password**: new_gmail_password
- **smtp_ssl**: On
Can't locate object method "quit" via package "Net::SMTP::SSL" at Bugzilla/Config/Common.pm line
解决：
按如下步骤安装Net::SMTP::SSL再配置邮箱后OK！
1、cpan -i MIME::Base64
2、cpan -i Net::SSLeay
3、cpan -i Net::SMTP::SSL
再附个连接：http://blog.sina.com.cn/s/blog_53a91ae30100ry4i.html
如果cpan安装找不到资源的话，修改仓库地址
$vi /usr/share/perl5/CPAN/Config.pm
'urllist' => ["ftp://ftp.cuhk.edu.hk/pub/packages/perl/CPAN/"],

The new value for smtpserver is invalid: Cannot connect to smtp.gmail.com using port 465.
解决：
Try port 587. See [http://mail.google.com/support/bin/answer.py?hl=en&answer=13287](http://mail.google.com/support/bin/answer.py?hl=en&answer=13287)

Can't locate object method "message" via package "To use SSL please install IO::Socket::SSL with version>=2.007 at lib/Net/SMTP.pm line 70." (perhaps you forgot to load "To use SSL please
 install IO::Socket::SSL with version>=2.007 at lib/Net/SMTP.pm line 70."?) at Bugzilla/Mailer.pm line 179, <DATA> line 1.
解决：
yum install 'perl(IO::Socket::SSL)'
cpan -i 
IO::Socket::SSL
According to 'perldoc perllocal', I have version 1.967 of IO::Socket::SSL installed.
QQ
smtp.qq.com后面不加:25等其它端口参数
密码处填授权码：
[http://service.mail.qq.com/cgi-bin/help?subtype=1&&id=28&&no=1001256](http://service.mail.qq.com/cgi-bin/help?subtype=1&&id=28&&no=1001256)
使用：
1，添加products和Components就可以添加了
参考：
[https://bugzilla.readthedocs.io/en/latest/](https://bugzilla.readthedocs.io/en/latest/)
[https://www.bugzilla.org/download/](https://www.bugzilla.org/download/)
