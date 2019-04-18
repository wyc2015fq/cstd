# 解决iredmail下postfix发送邮件时报错[need fully-qualified hostname] - weixin_33985507的博客 - CSDN博客
2014年10月24日 13:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：44
iredmail配置好后，尝试从一客户端的OE中发送邮件，结果报错，在mail主机中查看tail /var/log/maillog，发现如下错误信息：
```
Oct 24 12:50:49 mail2 postfix/smtpd[9280]: connect from unknown[192.168.5.240]
Oct 24 12:50:49 mail2 postfix/smtpd[9280]: NOQUEUE: reject: RCPT from unknown[192.168.5.240]: 504 5.5.2 <dzDevelop>: Helo command rejected: need fully-qualified hostname; from=<lon@mail2.yinghuan.com> to=<lon@mail2.yinghuan.com> proto=ESMTP helo=<dzDevelop>
Oct 24 12:50:49 mail2 postfix/smtpd[9280]: NOQUEUE: reject: RCPT from unknown[192.168.5.240]: 504 5.5.2 <dzDevelop>: Helo command rejected: need fully-qualified hostname; from=<lon@mail2.yinghuan.com> to=<gng@yinghun.com> proto=ESMTP helo=<dzDevelop>
Oct 24 12:50:49 mail2 postfix/smtpd[9280]: disconnect from unknown[192.168.5.240]
Oct 24 12:52:11 mail2 postfix/smtpd[9280]: connect from unknown[192.168.5.240]
Oct 24 12:52:11 mail2 postfix/smtpd[9280]: NOQUEUE: reject: RCPT from unknown[192.168.5.240]: 504 5.5.2 <dzDevelop>: Helo command rejected: need fully-qualified hostname; from=<lon@mail2.yinghuan.com> to=<lon@mail2.yinghuan.com> proto=ESMTP helo=<dzDevelop>
Oct 24 12:52:11 mail2 postfix/smtpd[9280]: NOQUEUE: reject: RCPT from unknown[192.168.5.240]: 504 5.5.2 <dzDevelop>: Helo command rejected: need fully-qualified hostname; from=<lon@mail2.yinghuan.com> to=<gng@yinghun.com> proto=ESMTP helo=<dzDevelop>
```
经过努力探索，发现只需要修改postfix的配置文件即可，我的版本postfix配置文件位于/etc/postfix/main.cf
用编辑器打开vim /etc/postfix/main.cf，找到smtpd_helo_restrictions这一行，按照下面修改后保存。
```
smtp_data_xfer_timeout = 600s
smtpd_helo_required = yes
#smtpd_helo_restrictions = permit_mynetworks, permit_sasl_authenticated, reject_non_fqdn_helo_hostname, reject_invalid_helo_hostname, check_helo_access pcre:/etc/postfix/helo_access.pcre
#上面一行修改下面的内容，去掉reject_non_fqdn_helo_hostname
smtpd_helo_restrictions = permit_mynetworks, permit_sasl_authenticated,  reject_invalid_helo_hostname, check_helo_access pcre:/etc/postfix/helo_access.pcre
```
 重新启动postfix
service postfix restart
再次发送，ok了
