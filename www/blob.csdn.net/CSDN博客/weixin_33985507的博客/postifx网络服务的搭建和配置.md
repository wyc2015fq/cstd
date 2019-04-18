# postifx网络服务的搭建和配置 - weixin_33985507的博客 - CSDN博客
2010年01月12日 09:44:29[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
注意：提前做好php环境
软件环境：
源码包：
        postfix-2.4.6.tar.gz
        squirrelmail-1.4.13.tar.bz2
all_locales-1.5.1-20060219.tar.bz2
    RHEL4/5自带rpm包：——
        cyrus-sasl-2.1.22-4
        httpd-2.2.3-6.el5
        php-5.1.6-5.el5
一、配置主机名、IP地址、默认网关、DNS地址等
1、配置主机名
vi /etc/sysconfig/network 
NETWORKING=yes
HOSTNAME=mail.benet.com
vi /etc/hosts    #//添加主机名到IP地址的映射
重启一下网卡
    2、配置IP地址、默认网关、DNS地址等（略）
二、安装配置Postfix
    1、解压安装
shell> groupadd -g 10000 postdrop
shell> useradd-u 1000 -s
/sbin/nologin postfix
shell> tar zxvf postfix-2.4.6.tar.gz
shell> make(提示你安装db4-devel)
shell> make install    #//对安装过程中的提示信息，可以直接回车接受默认值即可。(不用修改，直接回车就行了)
2、建立main.cf主配置文件
shell> cd /etc/postfix/
shell> postconf -n > main.cf(如果main.cf文件有的话，就不用做这一步了)
shell> vi /etc/postfix/main.cf
myhostname = mail.456.com（与邮箱有关）
mydomain = example.com
myorigin = $mydomain伪装
inet_interfaces = all
mydestination = $myhostname, $mydomain, localhost
mynetworks = 127.0.0.0/8 10.100.100.0/8
home_mailbox = Maildir/    #//邮件存储使用Maildir方式
message_size_limit = 31457280    #//限制邮件大小30MB
mailbox_size_limit = 524288000    #//限制邮箱空间500MB
    3、启动postfix
postfix start
三、为postfix提供SMTP认证支持
    1、配置saslauthd服务（cyrus-sasl）
shell> vi /usr/lib/sasl2/smtpd.conf该文件默认不存在
pwcheck_method:saslauthd
shell> vi
/etc/sysconfig/saslauthd
SOCKETDIR=/var/run/saslauthd确认这三行
MECH=shadow
FLAGS=
shell> service saslauthd start
    2、修改postfix配置文件
shell> vi /etc/postfix/main.cf    #//添加相关认证选项
broken_sasl_auth_clients = yes
smtpd_recipient_restrictions = permit_mynetworks, permit_sasl_authenticated,
reject_invalid_hostname, reject_unauth_pipelining, reject_unauth_destination
smtpd_sasl_auth_enable = yes
smtpd_sasl_local_domain = $myhostname
smtpd_sasl_security_options = noanonymous
smtpd_banner = $myhostname ESMTP "Require SMTP Auth."
    3、重新加载服务
shell> postfix reload
四、安装配置Dovecot（提供POP3和IMAP服务）（对源码包不熟悉的，就安装rpm包）
也可安装rpm包(先装mysql)
    1、解压安装
shell> useradd -s /sbin/nologin dovecot
shell> tar zxvf dovecot-1.1.4.tar.gz
shell> cd dovecot-1.1.4
shell> ./configure --sysconfdir=/etc
shell> make
shell> make install
    2、调整dovecot.conf配置文件
shell> cp /etc/dovecot-example.conf /etc/dovecot.conf
shell> vi /etc/dovecot.conf
disable_plaintext_auth = no    #//允许使用明文密码登录
ssl_disable = yes
protocols = pop3 imap
    3、启动dovecot
shell> dovecot -c /etc/dovecot.conf （service dovecot start）
装到这里需要用用oe等测试工具试一下收发邮件.
特别提示:postfix需要/etc/aliases.db的支持，用newaliases来生成
五、安装配置Squirrelmail（提供使用邮件系统的Web界面，需要用到SMTP、IMAP）
    1、解压安装
shell> tar jxvf squirrelmail-1.4.13.tar.bz2 -C
/var/www/html/
/var/www/html/squirrelmail-1.4.13/ 
shell> cd /var/www/html/
shell> mv squirrelmail-1.4.13 squirrelmail
shell> cd
/var/www/html/squirrelmail
shell> mkdir -p attach放附件
shell> chown -R apache:apache attach/ data/    #//假设httpd的运行用户和组身份为"apache"
shell> chmod 730 attach/
让linux支持中文：
下载了语言包all_locales-1.4.9-20070106.tar.bz2
tarxjvf all_locales-1.4.9-20070106.tar.bz2
进入解压目录cp*/var/www/html/squirrelmail/locale/
cd /var/www/html/webmail/config./conf.pl
选择10 设置语言为zh_CN
默认字符集为gb2312
保存退出，重新刷新webmail页面，就成中文了
    2、调整config.php配置
需要运行/var/www/html/squirrelmail/config/config.pl，然后根据提示进行设置，依次来生成config.php
    3、配置启动httpd服务
配置网页目录使用“DocumentRoot
"/var/www/html/squirrelmail"”，……
——过程略。
    —— 用户访问时使用[http://mail.example.com](http://mail.example.com/)的地址
