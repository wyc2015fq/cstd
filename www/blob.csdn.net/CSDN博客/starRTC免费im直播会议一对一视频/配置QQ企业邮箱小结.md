# 配置QQ企业邮箱小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月12日 09:51:11[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3254
[https://exmail.qq.com/login](https://exmail.qq.com/login)
1，注册管理员账号
2，添加域名
3、设置MX记录
记录类型选:MX记录
主机记录(RR):不填（非万网可以填写@）
记录值与MX优先级分别为：(非万网的可以在mxbiz1.qq.com后面加个点)
mxbiz1.qq.com 优先级：5
mxbiz2.qq.com 优先级：10
没有A记录的最好先设置一条测试A记录 4，设置TXT记录
为了提升域名邮箱发送外域（@qq.com以外的邮箱）邮件的成功率，建议设置一条TXT记录。
TXT记录值为：v=spf1 include:spf.mail.qq.com ~all
5，新版QQ企业邮箱不支持添加cname记录后，通过“mail.企业域名”来访问企业邮箱登录页面
解决方法是配置nginx,在域名处加一条a记录，主机记录(RR)填mail即可。
6,查看MX记录是否已经生效 打开[超级查询基地](http://www.lbase.net/),域名查询，选择MX记录，输入域名。
或者通过cmd输入：nslookup -qt=mx elesos.com
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%85%8D%E7%BD%AEQQ%E4%BC%81%E4%B8%9A%E9%82%AE%E7%AE%B1%E5%B0%8F%E7%BB%93&action=edit&section=1)]常用域名设置
要将域名指向空间商提供的IP地址，请选择「A记录」，要将域名指向另一个域名，请选择「CNAME记录」。
- A记录：将域名指向一个IPv4地址（例如：10.10.10.10），需要增加A记录
- CNAME记录：如果将域名指向一个域名，实现与被指向域名相同的访问效果，需要增加CNAME记录
- MX记录：建立电子邮箱服务，将指向邮件服务器地址，需要设置MX记录
- NS记录：域名解析服务器记录，如果要将子域名指定某个域名服务器来解析，需要设置NS记录
- TXT记录：可任意填写（可为空），通常用做SPF记录（反垃圾邮件）使用
- AAAA记录：将主机名（或域名）指向一个IPv6地址（例如：ff03:0:0:0:0:0:0:c1），需要添加AAAA记录
- SRV记录：记录了哪台计算机提供了哪个服务。格式为：服务的名字.协议的类型（例如：_example-server._tcp）
- 显性URL：将域名指向一个http（s)协议地址，访问域名时，自动跳转至目标地址（例如：将www.net.cn显性转发到www.hichina.com后，访问www.net.cn时，地址栏显示的地址为：www.hichina.com）。
- 隐性URL：与显性URL类似，但隐性转发会隐藏真实的目标地址（例如：将www.net.cn隐性转发到www.hichina.com后，访问www.net.cn时，地址栏显示的地址仍然为：www.net.cn）。
如何使用mail.自己域名登录
虽然腾讯没有提供这个服务了，但是有个变通的办法。
在你的DNS解析中做一条URL条状，将mail条状到以下地址，*****是你自己的域名。
[http://exmail.qq.com/cgi-bin/loginpage?t=logindomain&s=logout&f=biz¶m=@](http://exmail.qq.com/cgi-bin/loginpage?t=logindomain&s=logout&f=biz&param=@)******.com
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=%E9%85%8D%E7%BD%AEQQ%E4%BC%81%E4%B8%9A%E9%82%AE%E7%AE%B1%E5%B0%8F%E7%BB%93&action=edit&section=2)]艺搜参考
[怎样创建腾讯企业邮箱](http://service.exmail.qq.com/cgi-bin/help?subtype=1&&no=1001214&&id=20012)
[哪家免费的企业邮箱相对较好呢?](http://www.zhihu.com/question/20299741)
[什么是域名的MX记录及设置方法？](http://service.exmail.qq.com/cgi-bin/help?subtype=1&&id=20012&&no=1000569)
[如何查看MX记录是否已经生效？](http://service.exmail.qq.com/cgi-bin/help?subtype=1&&id=20012&&no=1000628)
[如何设置企业邮箱的SPF呢？（TXT记录）](http://service.exmail.qq.com/cgi-bin/help?subtype=1&&id=20012&&no=1000580)
