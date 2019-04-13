
# CentOS 6-7 配置 sendEmail 发送邮件 - 阳光岛主 - CSDN博客

2018年10月29日 22:09:16[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2561


sendEmail是Linux下优秀的第三方邮件命令，非常简单，可以设置 smtp，不会被认定为SPAM（垃圾邮件）或直接拒收。
本文原文转自米扑博客：[CentOS 6/7 配置 sendEmail 发送邮件](https://blog.mimvp.com/article/26981.html)
sendEmail 官网：[http://caspian.dotconf.net/menu/Software/SendEmail/](http://caspian.dotconf.net/menu/Software/SendEmail/)
sendEmail 下载：[sendEmail-v1.56.tar.gz](http://caspian.dotconf.net/menu/Software/SendEmail/sendEmail-v1.56.tar.gz)（2009）
**1、sendEmail 下载**
[sendEmail-v1.56.tar.gz](http://caspian.dotconf.net/menu/Software/SendEmail/sendEmail-v1.56.tar.gz)
**2、解压、安装**
tar zxvf sendEmail-v1.56.tar.gz
cd sendEmail-v1.56
cp sendEmail /usr/local/bin/
查看解压后的文件
```python
# ll sendEmail-v1.56
total 116
-rw-r--r-- 1 root root 13329 Sep 30  2009 CHANGELOG
-rw-r--r-- 1 root root  7949 Sep 30  2009 README
-rw-r--r-- 1 root root  4701 Sep 30  2009 README-BR.txt
-rwxr-xr-x 1 root root 80213 Sep 30  2009 sendEmail
lrwxrwxrwx 1 root root     9 Sep 30  2009 sendEmail.pl -> sendEmail
-rw-r--r-- 1 root root  1229 Sep 30  2009 TODO
```
**3、sendEmail 参数**
sendEmail -h
-f 表示发送者的邮箱
-t 表示接收者的邮箱
-s 表示SMTP服务器的域名或者ip
-u 表示邮件的主题
-xu 表示SMTP验证的用户名
-xp 表示SMTP验证的密码(注意,这个密码貌似有限制,例如我用d!5neyland就不能被正确识别)
-m 表示邮件的内容
-cc 表示抄送
-bcc 表示暗抄送
**4、应用实例**
**1）qq企业邮箱发送邮件**
/usr/local/bin/sendEmail -f robot@mimvp.com -t yanggang@mimvp.com -s smtp.exmail.qq.com -u "邮件标题" -o message-charset=utf-8 -xu robot@mimvp.com -xp mimvp-password -m "邮件正文内容"
结果错误，信息如下：
```python
# /usr/local/bin/sendEmail -f robot@mimvp.com -t yanggang@mimvp.com -s smtp.exmail.qq.com -u "邮件标题" -o message-charset=utf-8 -xu robot@mimvp.com -xp mimvp-password -m "邮件正文内容"
*******************************************************************
 Using the default of SSL_verify_mode of SSL_VERIFY_NONE for client
 is deprecated! Please set SSL_verify_mode to SSL_VERIFY_PEER
 together with SSL_ca_file|SSL_ca_path for verification.
 If you really don't want to verify the certificate and keep the
 connection open to Man-In-The-Middle attacks please set
 SSL_verify_mode explicitly to SSL_VERIFY_NONE in your application.
*******************************************************************
  at /usr/local/bin/sendEmail line 1906.
invalid SSL_version specified at /usr/share/perl5/vendor_perl/IO/Socket/SSL.pm line 424.
```
查看源码：
vim /usr/share/perl5/vendor_perl/IO/Socket/SSL.pm +424
${*$self}{'_SSL_arguments'} = $arg_hash;
${*$self}{'_SSL_ctx'} = IO::Socket::SSL::SSL_Context->new($arg_hash) || return;
${*$self}{'_SSL_opened'} = 1 if $is_server;
应该是 perl 版本不兼容
```python
# perl -v
 
This is perl 5, version 16, subversion 3 (v5.16.3) built for x86_64-linux-thread-multi
(with 29 registered patches, see perl -V for more detail)
 
Copyright 1987-2012, Larry Wall
```
解决方案，sendEmail 增加两个参数
-o tls=no    // sendEmail -h 查看帮助，解释是 -o tls=<auto|yes|no>         -o fqdn=FQDN  即不验证 ssl
完整且正确命令：
/usr/local/bin/sendEmail -f robot@mimvp.com -t yanggang@mimvp.com -s smtp.exmail.qq.com -u "邮件标题" -o message-charset=utf-8 -xu robot@mimvp.com -xp mimvp-password -m "邮件正文内容" -o tls=no
```python
# /usr/local/bin/sendEmail -f robot@mimvp.com -t yanggang@mimvp.com -s smtp.exmail.qq.com -u "邮件标题" -o message-charset=utf-8 -xu robot@mimvp.com -xp mimvp-password -m "邮件正文内容" -o tls=no
Aug 02 00:27:47 mimvp-sz sendEmail[15493]: Email was sent successfully!
```
qq企业邮箱是 用户名 + 密码；qq个人邮箱是用户名 + 随机码
查看发送的邮件：
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos-6-7-pei-zhi-sendmail-fa-song-you-jian-01.png)
**2）163 个人邮箱发送邮件**
同样需要加上两个参数 -o tls=no
/usr/local/bin/sendEmail -f yanggang@163.com -t yanggang@mimvp.com -s smtp.163.com -u "邮件标题" -o message-charset=utf-8 -xu yanggang@163.com -xp mimvp-auth-code -m "邮件正文内容" -o tls=no
```python
# /usr/local/bin/sendEmail -f yanggang@163.com -t yanggang@mimvp.com -s smtp.163.com -u "邮件标题" -o message-charset=utf-8 -xu yanggang@163.com -xp mimvp-auth-code -m "邮件正文内容" -o tls=no
Aug 02 00:31:37 mimvp-sz sendEmail[15642]: Email was sent successfully!
```
163个人邮箱是 用户名 + 授权码（mimvp-auth-code），不是密码哈，设置获取授权码请见米扑博客：[POP3、SMTP、IMAP 的作用和联系](https://blog.mimvp.com/article/6030.html)
成功发送截图：
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos-6-7-pei-zhi-sendmail-fa-song-you-jian-02.png)
**小结**
经过对比实测，发现阿里云封禁了25端口号的服务器，是无法利用 sendEmail 发送邮件的，只有开放没封25端口号的才可以发送
折腾了利用 smtps://smtp.exmail.qq.com:165 发送邮件，结果没有成功！
感兴趣的您，可以继续折腾下，搞定的欢迎留言告诉我哈 ~.~


