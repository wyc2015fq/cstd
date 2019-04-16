# 转载和积累系列 - wifidog认证流程图 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月18日 14:08:20[initphp](https://me.csdn.net/initphp)阅读数：3860








一. 用户上线

1. 用户访问网络，通过iptables将未认证的用户dnat到wifidog进程，wifidog通过307报文将用户重定向到认证服务器

2. 用户打开认证服务器登录页面，输入用户名密码，发送认证请求

3. 认证成功的话服务器会发送302报文，携带token信息重定向到wifidog页面。认证失败的话会返回失败页面

4. 用户携带token信息向wifidog发起认证请求，wifidog再向认证服务器发起请求，认证成功后授权，并将用户重定向到成功页面

![](https://img-blog.csdn.net/20131218140849906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


二. 保活和下线

1. wifidog会定时向认证服务器发送保活消息

2. 当用户主动请求下线后，wifidog此时并没有下线

3. 当wifidog再次发起保活请求时，认证服务器会告诉它用户已下线，此时wifidog会将用户下线

![](https://img-blog.csdn.net/20131218140904921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










