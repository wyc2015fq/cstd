
# Centos7 配置 sendmail、postfix 端口号25、465 - 阳光岛主 - CSDN博客

2018年11月05日 10:34:09[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2339


阿里云、腾讯云的操作系统 CentOS 6.x 和 CentOS 7.x 安装 sendmail、postfix 发送邮件时，会遇到无法使用25端口号发送邮件的问题。
本文分享米扑博客折腾了几天的研究成果，全面详细讨论各种正常和异常场景，您遇到的坑我们可能全部都踩过，并给出解决方案。米扑博客原文：[Centos7 配置 sendmail、postfix 端口号25、465](https://blog.mimvp.com/article/26918.html)
阅读本文前，推荐最好先阅读米扑博客先前的两篇博客：
[Centos7 使用 sendmail 发送邮件](https://blog.mimvp.com/article/26872.html)
[POP3、SMTP、IMAP 的作用和联系](https://blog.mimvp.com/article/6030.html)
在讨论之前，先介绍发送邮件的几个场景：
1）本地发送邮件，是指安装完sendmail后，直接由本机发送邮件（小白级）
2）smtp发送邮件，包含设置163、qq等smtp 25、smtps 465 等端口转发（中级）
3）第三方邮件软件方式，如使用 Ubuntu**heirloom-mailx**、NodeJS**Nodemailer**、**sendEmail**等转发邮件（高级）
4）使用 nginx 代理方式，把端口号25 做转发（专家级）

**一、本地发送邮件**（小白级）
本地发送邮件，一般是安装完 sendmail、mailx 后，什么都不配置，就直接发送邮件，因此默认是由本机发送邮件
yum -y install sendmail* mailx
本地发送邮件命令：
mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
**1、本地发送邮件的正常日志**（2016年9月前购买的阿里云，CentOS 6.5）
mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
```python
# mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
Loading lookup modules from /usr/lib64/exim/4.72-7.el6/lookups
Loaded 0 lookup modules
LOG: MAIN
  <= root@mimvp-bj U=root P=local S=2075
[root@mimvp-bj ~]# Loading lookup modules from /usr/lib64/exim/4.72-7.el6/lookups
Loaded 0 lookup modules
delivering 1fkiJu-0000Zc-C5
Connecting to mxbiz1.qq.com [163.177.89.176]:25 ... connected
  SMTP<< 220 bizmx17.qq.com MX QQ Mail Server
  SMTP>> EHLO mimvp-bj
  SMTP<< 250-bizmx17.qq.com
         250-SIZE 73400320
         250-STARTTLS
         250 OK
  SMTP>> STARTTLS
  SMTP<< 220 Ready to start TLS
  SMTP>> EHLO mimvp-bj
  SMTP<< 250-bizmx17.qq.com
         250-SIZE 73400320
         250 OK
  SMTP>> MAIL FROM:<root@mimvp-bj> SIZE=3147
  SMTP<< 250 Ok
  SMTP>> RCPT TO:<yanggang@mimvp.com>
  SMTP<< 250 Ok
  SMTP>> DATA
  SMTP<< 354 End data with <CR><LF>.<CR><LF>
  SMTP>> writing message and terminating "."
  SMTP<< 250 Ok: queued as
  SMTP>> QUIT
LOG: MAIN
  => yanggang@mimvp.com R=dnslookup T=remote_smtp H=mxbiz1.qq.com [163.177.89.176] X=UNKNOWN:AES128-SHA256:128
LOG: MAIN
  Completed
```
成功接收邮件的结果：
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos7-pei-zhi-sendmail-postfix-duan-kou-hao-25465-01.png)
**2、本地发送邮件的错误日志**（2016年9月后购买的阿里云，CentOS 7.5）
mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
```python
# mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
yanggang@mimvp.com... Connecting to [127.0.0.1] via relay...
220 mimvp-sz2.com ESMTP Sendmail 8.14.7/8.14.7; Wed, 1 Aug 2018 12:05:19 +0800
>>> EHLO mimvp.com
250-mimvp-sz2.com Hello localhost [127.0.0.1], pleased to meet you
250-ENHANCEDSTATUSCODES
250-PIPELINING
250-8BITMIME
250-SIZE
250-DSN
250-ETRN
250-DELIVERBY
250 HELP
>>> MAIL From:<root@mimvp.com> SIZE=1795
250 2.1.0 <root@mimvp.com>... Sender ok
>>> RCPT To:<yanggang@mimvp.com>
>>> DATA
550 5.1.1 <yanggang@mimvp.com>... User unknown
503 5.0.0 Need RCPT (recipient)
>>> RSET
250 2.0.0 Reset state
>>> RSET
250 2.0.0 Reset state
root... Using cached ESMTP connection to [127.0.0.1] via relay...
>>> MAIL From:<> SIZE=2819
250 2.1.0 <>... Sender ok
>>> RCPT To:<root@mimvp.com>
>>> DATA
250 2.1.5 <root@mimvp.com>... Recipient ok
354 Enter mail, end with "." on a line by itself
>>> .
250 2.0.0 w7145JI4002277 Message accepted for delivery
root... Sent (w7145JI4002277 Message accepted for delivery)
Closing connection to [127.0.0.1]
>>> QUIT
221 2.0.0 mimvp-sz2.com closing connection
```
失败接收邮件的结果：
550 5.1.1 <yanggang@mimvp.com>... User unknown
从上面发送日志里，可以看到上图错误提示 User unknown，发送失败了，因此也不可能成功收到邮件
**3、问题分析与解决**
错误提示 User unknown，是很多小白级用户遇到的问题，当然我一开始也是小白级用户，也遇到过此问题
遇到这种问题，如何分析呢？ 哈哈，这就是我深入思考，从小白级逐渐晋升的小秘诀
本地发送邮件，默认是发送方是本机用户名、采用默认25端口号，因此可以通过 telnet 检测25端口号的可用性
a）安装 telnet
yum -y install telnet
退出 telnet 命令：
Ctrl + ] （反中括号），然后 quit
b）telnet 连接检测 1）成功发送邮件的25端口号
先获取本机外网ip，然后 telnet 外网ip 25
telnet 123.57.78.159 25
```python
[root@mimvp-bj ~]# curl ip.cn
当前 IP：123.57.78.159 来自：北京市 阿里云
[root@mimvp-bj ~]# telnet 123.57.78.159 25
Trying 123.57.78.159...
Connected to 123.57.78.159.
Escape character is '^]'.
220 mimvp-bj ESMTP Exim 4.72 Wed, 01 Aug 2018 12:17:43 +0800
```
可见，本地成功发送邮件的服务器，是可以成功连接 25 端口号
c）telnet 连接检测 2）失败发送邮件的25端口号
telnet 47.106.126.18 25
|1
|2
|3
|4
|5
|6
|[root@mimvp-sz2 ~]|\# curl ip.cn
|当前 IP：47.106.126.18 来自：广东省深圳市 阿里云
|[root@mimvp-sz2 ~]|\#
|[root@mimvp-sz2 ~]|\# telnet 47.106.126.18 25
|Trying 47.106.126.18...
|telnet: connect to address 47.106.126.18: Connection timed out
|
可见，本地失败发送邮件的服务器，不可以成功失败连接 25 端口号，错误提示超时
上面就是为什么阿里云新购买的服务器无法发送邮件的原因，阿里云把25端口号禁用了，后面也会影响到下文介绍的 smtp 配置
**经过米扑博客搜集了网上大量资料，创新型总结：**
阿里云客服解释是2016年9月以后新购买的服务器将25端口封掉了，之前购买的不受影响
**阿里云封禁25号端口**
出于安全考虑，阿里云默认封禁TCP 25端口出方向的访问流量，即你无法在阿里云上的云服务器通过TCP 25端口连接外部地址。
TCP 25端口的出方向封禁可能影响您通过TCP 25 端口连接第三方邮件服务商的SMTP服务器对外部发送邮件。
如果您需要使用阿里云上的云服务器对外部发送邮件，建议您使用阿里云邮产品或者使用第三方邮件服务商提供的465端口。
如果存在特殊场景，您必须在云服务器上使用TCP 25端口进行对外连接，请在安全管控平台中提交25端口解封申请。
阿里云：[投诉，无故封禁25端口](https://bbs.aliyun.com/read/301491.html)（2016-12-06）
阿里云：[TCP 25端口解封申请](https://help.aliyun.com/knowledge_detail/56130.html)（2018-06-27）
**这里先小结，给出解决方案：**
1）本地发送邮件，默认用25端口号，由于阿里云2016年9月后新购服务器都禁用了25端口号，因此本地发送邮件是不可能了
2）根据阿里云官方给出的解决方案，可以采用第三方 smtp ssl 465 端口号，即我们马上要介绍的 smtp 和 smtps（可行的技术方案）
3）根据阿里云解决方案，向阿里云申请解封25号端口，但其也只能用第三方smtp，不可本机发送邮件（实际不给开，全封禁了）
阿里云申请解封25号端口官网：[25端口解封](https://yundun.console.aliyun.com/?p=sc#/sc/port)
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos7-pei-zhi-sendmail-postfix-duan-kou-hao-25465-02-700x390.png)
米扑博客尝试向阿里云申请你了，结果是否决的，对阿里系一直没好感，意料之中的坑爹货~

**二、smtp发送邮件**（中级）
配置 smtp 发送邮件，这样邮件的发送者就是正常的邮箱地址，不再是本机root发送
1）正常的发送者邮箱地址，如 yangmipu@mimvp.com、5678543@qq.com 等
2）本机root发送者邮箱地址，如 root@mimvp-bj、root@mimvp-sz.localdomain 等
smtp 默认也是采用 25端口号发送，由于阿里云禁用了25端口号，但这里还是对比介绍下正常和失败的场景
本文配置的qq企业邮箱和163个人邮箱，需要先了解授权码：[POP3、SMTP、IMAP 的作用和联系](https://blog.mimvp.com/article/6030.html)
**1、修改 smtp 配置文件**
vim /etc/mail.rc
添加如下配置项
set from=robot@mimvp.com
set smtp=smtp.exmail.qq.com
set smtp-auth-user=robot@mimvp.com
set smtp-auth-password=mimvp-password
set smtp-auth=login
参数介绍，请见米扑博客：[Centos7 使用 sendmail 发送邮件](https://blog.mimvp.com/article/26872.html)
**2、smtp发送邮件的正常日志**（2016年9月前购买的阿里云，CentOS 6.5）
```python
# mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
Resolving host smtp.exmail.qq.com . . . done.
Connecting to 163.177.72.143 . . . connected.
220 smtp.qq.com Esmtp QQ Mail Server
>>> EHLO mimvp-bj
250-smtp.qq.com
250-PIPELINING
250-SIZE 73400320
250-STARTTLS
250-AUTH LOGIN PLAIN
250-AUTH=LOGIN
250-MAILCOMPRESS
250 8BITMIME
>>> AUTH LOGIN
334 VXNlcm5hbWU6
>>> cm9ib3RAbWltdnAuY29t
334 UGFzc3dvcmQ6
>>> TUltdnBfMjA1MF90b2Jvcg==
235 Authentication successful
>>> MAIL FROM:<robot@mimvp.com>
250 Ok
>>> RCPT TO:<yanggang@mimvp.com>
250 Ok
>>> DATA
354 End data with <CR><LF>.<CR><LF>
>>> .
250 Ok: queued as 
>>> QUIT
221 Bye
```
成功接收邮件的结果：
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos7-pei-zhi-sendmail-postfix-duan-kou-hao-25465-03.png)
**3、smtp 发送邮件的错误日志**（2016年9月后购买的阿里云，CentOS 7.5）
```python
# mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
Resolving host smtp.exmail.qq.com . . . done.
Connecting to 163.177.72.143:smtp . . .Connecting to 163.177.90.125:smtp . . .could not connect: Connection timed out
"/root/dead.letter" 43/1873
. . . message not sent.
```
失败接收邮件的结果：
Connecting to 163.177.72.143:smtp . . .Connecting to 163.177.90.125:smtp . . .could not connect: Connection timed out
从上面发送日志里，可以看到上图错误提示 could not connect: Connection timed out，发送失败了，因此也不可能成功收到邮件
这是因为阿里云禁用了25端口号，让 smtp 默认端口号25 无法通信，可以通过 telnet 来验证
**4、问题分析与解决**
smtp 发送邮件，默认也是采用25端口号，因此可以通过 telnet 检测25端口号的可用性
a）安装 telnet
yum -y install telnet
退出 telnet 命令：
Ctrl + ] （反中括号），然后 quit
b）telnet 连接检测 1）成功发送邮件的25端口号
先获取本机外网ip，然后 telnet 外网ip 25
telnet smtp.exmail.qq.com 25
```python
# telnet smtp.exmail.qq.com 25
Trying 163.177.72.143...
Connected to smtp.exmail.qq.com.
Escape character is '^]'.
220 smtp.qq.com Esmtp QQ Mail Server
^]
telnet> quit
Connection closed.
```
可见，本地成功发送邮件的服务器，是可以成功连接 25 端口号
c）telnet 连接检测 2）失败发送邮件的25端口号
telnet smtp.exmail.qq.com 25
```python
# telnet smtp.exmail.qq.com 25
Trying 163.177.90.125...
telnet: connect to address 163.177.90.125: Connection timed out
Trying 163.177.72.143...
telnet: connect to address 163.177.72.143: Connection timed out
```
可见，smtp失败发送邮件的服务器，不可以成功失败连接 25 端口号，错误提示超时
结论：封禁了25端口号，实际上对本地发送邮件、smtp 发送邮件的方式 ，都给禁掉了
因此，唯一可行的解决方案就是不用 25端口号，改用 smtps ssl 465 端口号，安全发送邮件

**三、smtps 发送邮件**（中级）
465端口是SSL/TLS通讯协议的内容，邮件正文一开始就被加密保护起来了，是看不到邮件明文的。
**465端口（SMTPS）**：465端口是为SMTPS（SMTP-over-SSL）协议服务开放的，这是SMTP协议基于SSL安全协议之上的一种变种协议，它继承了SSL安全协议的非对称加密的高度安全可靠性，可防止邮件泄露。SMTPS和SMTP协议一样，也是用来发送邮件的，只是更安全些，防止邮件被黑客截取泄露，还可实现邮件发送者抗抵赖功能。防止发送者发送之后删除已发邮件，拒不承认发送过这样一份邮件。
上面讲了 telnet 验证25端口，现在验证下 465 端口
\# telnet smtp.exmail.qq.com 465
\# telnet smtp.163.com 465
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|[root@mimvp-sz2 ~]|\# telnet smtp.exmail.qq.com 465
|Trying 163.177.90.125...
|Connected to smtp.exmail.qq.com.
|Escape character is|'^]'|.
|^]
|telnet> Connection closed.
|[root@mimvp-sz2 ~]|\# telnet smtp.163.com 465
|Trying 220.181.12.18...
|Connected to smtp.163.com.
|Escape character is|'^]'|.
|^]
|telnet> Connection closed.
|
发现 465 端口号都是开放的，阿里云没有屏蔽 465端口（哈哈，废话嘛，阿里云推荐采用465端口）
这里介绍两种配置 smtps 465端口号的方式：qq 企业邮件 和 163个人邮箱，基本可以满足绝多数的用户需求了
1）qq 企业邮箱，发送者邮箱 + 密码（若是qq个人邮箱，则是发送者邮箱 + 随机码，不是密码哈）
2）163 个人邮箱，发送者邮箱 + 授权码（163是自己定义的授权码）
若不清楚，请参见米扑博客：[POP3、SMTP、IMAP 的作用和联系](https://blog.mimvp.com/article/6030.html)
**1、qq 企业邮箱设置 smtps 465**
首先，开启 smtp
登录qq企业邮箱 ——> 设置 ——> 客户端设置
接收服务器：**imap.exmail.qq.com (使用SSL，端口号993)**
发送服务器：**smtp.exmail.qq.com (使用SSL，端口号465)**
接着，配置 /etc/mail.rc
vim /etc/mail.rc
|1
|2
|3
|4
|5
|6
|7
|set from=robot@mimvp.com
|set smtp=smtps:|//smtp.exmail.qq.com:465
|set ssl-verify=ignore
|set nss-config-dir=/root/.certs
|set smtp-auth-user=robot@mimvp.com
|set smtp-auth-password=mimvp-password
|set smtp-auth=login
|
创建证书目录
mkdir /root/.certs
然后，ssl 授权，执行如下命令
|1
|2
|3
|4
|5
|echo|-n | openssl s_client -connect smtp.qq.com:465 ||sed|-|ne|'/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p'|> ~/.certs|/qq|.crt
|certutil -A -n|"GeoTrust Global CA"|-t|"C,,"|-d ~/.certs -i ~/.certs|/qq|.crt
|certutil -A -n|"GeoTrust SSL CA"|-t|"C,,"|-d ~/.certs -i ~/.certs|/qq|.crt
|certutil -L -d .certs
|certutil -A -n|"GeoTrust SSL CA - G3"|-t|"Pu,Pu,Pu"|-d ~/.certs/ -i ~/.certs|/qq|.crt
|
完整的执行日志如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|[root@mimvp-hz ~]|\# echo -n | openssl s_client -connect smtp.qq.com:465 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > ~/.certs/qq.crt
|depth=2 C = US, O = DigiCert Inc, OU = www.digicert.com, CN = DigiCert Global Root CA
|verify|return|:1
|depth=1 C = US, O = DigiCert Inc, OU = www.digicert.com, CN = GeoTrust RSA CA 2018
|verify|return|:1
|depth=0 C = CN, ST = Guangdong, L = Shenzhen, O = Shenzhen Tencent Computer Systems Company Limited, OU = R&D, CN = pop.qq.com
|verify|return|:1
|DONE
|[root@mimvp-hz ~]|\# certutil -A -n "GeoTrust Global CA" -t "C,," -d ~/.certs -i ~/.certs/qq.crt
|[root@mimvp-hz ~]|\# certutil -A -n "GeoTrust SSL CA" -t "C,," -d ~/.certs -i ~/.certs/qq.crt
|[root@mimvp-hz ~]|\# certutil -L -d .certs
|Certificate Nickname                                         Trust Attributes
|SSL,S|/MIME|,JAR|/XPI
|GeoTrust SSL CA                                              C,,
|GeoTrust Global CA                                           C,,
|
再次，smtps 加密发送邮件
mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|\# mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd                                                Resolving host smtp.exmail.qq.com . . . done.
|Connecting to 163.177.72.143 . . . connected.
|Error|in|certificate: Peer's certificate issuer is not recognized.
|Comparing DNS name:|"pop.qq.com"
|Comparing DNS name:|"ex.qq.com"
|Comparing DNS name:|"imap.exmail.qq.com"
|Comparing DNS name:|"rtx.exmail.qq.com"
|Comparing DNS name:|"smtp.exmail.qq.com"
|SSL parameters: cipher=AES-128, keysize=128, secretkeysize=128,
|issuer=CN=GeoTrust RSA CA 2018,OU=www.digicert.com,O=DigiCert Inc,C=US
|subject=CN=pop.qq.com,OU=R&D,O=Shenzhen Tencent Computer Systems Company Limited,L=Shenzhen,ST=Guangdong,C=CN
|220 smtp.qq.com Esmtp QQ Mail Server
|>>> EHLO mimvp-hz
|250-smtp.qq.com
|250-PIPELINING
|250-SIZE 73400320
|250-AUTH LOGIN PLAIN
|250-AUTH=LOGIN
|250-MAILCOMPRESS
|250 8BITMIME
|>>> AUTH LOGIN
|334 VXNlcm5hbWU6
|>>> cm9ib3RAbWltdnAuY29t
|334 UGFzc3dvcmQ6
|>>> TUltdnBfMjA1MF90b2Jvcg==
|235 Authentication successful
|>>> MAIL FROM:<robot@mimvp.com>
|250 Ok
|>>> RCPT TO:<yanggang@mimvp.com>
|250 Ok
|>>> DATA
|354 End data with <CR><LF>.<CR><LF>
|>>> .
|250 Ok: queued as
|>>> QUIT
|221 Bye
|
最后，查看成功发送了邮件
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos7-pei-zhi-sendmail-postfix-duan-kou-hao-25465-03.png)
**2、163 个人邮箱设置 smtps 465**
首先，开启 smtp
登录163个人邮箱 ——> 设置 ——> POP3/SMTP/IMAP
服务器地址:
POP3服务器: pop.163.com
SMTP服务器: smtp.163.com
IMAP服务器: imap.163.com
接着，配置 /etc/mail.rc
vim /etc/mail.rc
|1
|2
|3
|4
|5
|6
|7
|set|from=yanggang@163.com
|set|smtp=smtps:|//smtp|.163.com:465
|set|ssl-verify=ignore
|set|nss-config-|dir|=|/root/|.certs
|set|smtp-auth-user=yanggang@163.com
|set|smtp-auth-password=password-auth-code
|set|smtp-auth=login
|
创建证书目录
mkdir /root/.certs
163个人邮箱的密码，不是邮箱密码，而是授权码
申请请参见米扑博客：[POP3、SMTP、IMAP 的作用和联系](https://blog.mimvp.com/article/6030.html)（163 邮箱授权码）
然后，ssl 授权，执行如下命令
|1
|2
|3
|4
|5
|echo|-n | openssl s_client -connect smtp.163.com:465 ||sed|-|ne|'/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p'|> ~/.certs|/163|.crt
|certutil -A -n|"GeoTrust Global CA"|-t|"C,,"|-d ~/.certs -i ~/.certs|/163|.crt
|certutil -A -n|"GeoTrust SSL CA"|-t|"C,,"|-d ~/.certs -i ~/.certs|/163|.crt
|certutil -L -d|/root/|.certs
|certutil -A -n|"GeoTrust SSL CA - G3"|-t|"Pu,Pu,Pu"|-d ~/.certs/ -i ~/.certs|/163|.crt
|
完整的执行日志如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|[root@mimvp-hz ~]|\# echo -n | openssl s_client -connect smtp.163.com:465 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > ~/.certs/163.crt
|depth=2 C = US, O = GeoTrust Inc., CN = GeoTrust Global CA
|verify|return|:1
|depth=1 C = US, O = GeoTrust Inc., CN = GeoTrust SSL CA - G3
|verify|return|:1
|depth=0 C = CN, ST = ZheJiang, L = HangZhou, O =|"NetEase (Hangzhou) Network Co., Ltd"|, CN = *.163.com
|verify|return|:1
|DONE
|[root@mimvp-hz ~]|\# certutil -A -n "GeoTrust Global CA" -t "C,," -d ~/.certs -i ~/.certs/163.crt
|[root@mimvp-hz ~]|\# certutil -A -n "GeoTrust SSL CA" -t "C,," -d ~/.certs -i ~/.certs/163.crt
|[root@mimvp-hz ~]|\# certutil -L -d /root/.certs
|Certificate Nickname                                         Trust Attributes
|SSL,S|/MIME|,JAR|/XPI
|GeoTrust SSL CA                                              C,,
|GeoTrust Global CA                                           C,,
|
查看授权目录 /root/.certs/ 里的文件
|1
|2
|3
|4
|5
|6
|7
|\# ll /root/.certs/
|total 84
|-rw-r--r-- 1 root root  2285 Jul 31 23:17 163.crt
|-rw------- 1 root root 65536 Aug  1 15:36 cert8.db
|-rw------- 1 root root 16384 Aug  1 15:36 key3.db
|-rw-r--r-- 1 root root  2594 Aug  1 13:43 qq.crt
|-rw------- 1 root root 16384 Jul 31 23:18 secmod.db
|
再次，smtps 加密发送邮件
mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|\# mail -v -s "subject_title" yanggang@mimvp.com < /etc/passwd
|Resolving host smtp.163.com . . .|done|.
|Connecting to 220.181.12.12 . . . connected.
|Error|in|certificate: Peer's certificate issuer is not recognized.
|Comparing DNS name:|"*.163.com"
|SSL parameters: cipher=AES-128-GCM, keysize=128, secretkeysize=128,
|issuer=CN=GeoTrust SSL CA - G3,O=GeoTrust Inc.,C=US
|subject=CN=*.163.com,O=|"NetEase (Hangzhou) Network Co., Ltd"|,L=HangZhou,ST=ZheJiang,C=CN
|220 163.com Anti-spam GT|for|Coremail System (163com[20141201])
|>>> EHLO mimvp-hz
|250-mail
|250-PIPELINING
|250-AUTH LOGIN PLAIN
|250-AUTH=LOGIN PLAIN
|250-coremail 1Uxr2xKj7kG0xkI17xGrU7I0s8FY2U3Uj8Cz28x1UUUUU7Ic2I0Y2UFe34-5UCa0xDrUUUUj
|250-STARTTLS
|250 8BITMIME
|>>> AUTH LOGIN
|334 dXNlcm5hbWU6
|>>> eWFuZ2dhbmdfMjA1MEAxNjMuY29t
|334 UGFzc3dvcmQ6
|>>> U3VuYm95MjA1MA==
|235 Authentication successful
|>>> MAIL FROM:<yanggang@163.com>
|250 Mail OK
|>>> RCPT TO:<yanggang@mimvp.com>
|250 Mail OK
|>>> DATA
|354 End data with <CR><LF>.<CR><LF>
|>>> .
|250 Mail OK queued as smtp8,DMCowABXbp2dXmFbV75OLg--.8377S2 1533107869
|>>> QUIT
|221 Bye
|
最后，查看成功发送了邮件
![](https://cdn-blog.mimvp.com/wp-content/uploads/2018/10/centos7-pei-zhi-sendmail-postfix-duan-kou-hao-25465-04.png)
总结，smtps 通过 465端口号，实际测试可以在阿里云老服务器、新服务器上，都可以正常发送邮件
因此，通过 smtps ssl 465 通过 qq企业邮箱和163个人邮箱发送邮件，都成功了，推荐此方案

**四、nginx 发送邮件**（专家级）
本文暂不讲解 nginx 代理转发方式，配置太多繁琐，安装软件太多，知识点太多
感兴趣的可以参见：[Configuring NGINX as a Mail Proxy Server](https://docs.nginx.com/nginx/admin-guide/mail-proxy/mail-proxy/)


