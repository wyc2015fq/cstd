# Xmanager连接CentOS的远程桌面 - DoubleLi - 博客园






本文主要介绍通过Xmanager连接[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14)远程桌面时，在CentOS系统上需要做的一些配置。

1. Xmanager简介

Xmanager是一个运行于 Windows平台上的高性能的X Server软件。它能把远端Unix/Linux的桌面无缝地带到你的Windows上，甚至电脑是在内网或防火墙后，也能通过SSH协议安全的运行远端的X应用程序。

特点包括：
- 可通过Xcongfig工具设置多个Xmanager设置；
- 支持多用户的Windows终端环境；
- 支持多个IP地址；
- 支持本地资源数据库；
- 通过热键转换键盘映射；
- 支持多窗口下的Windows打印功能等

![](http://www.linuxidc.com/upload/2015_08/150827163590081.gif)

2. CentOS 6.3配置

安装gdm (若已安装则不需安装)

yum -y install gdm

配置系统为图形模式，打开/etc/inittab，修改为id:5:initdefault: (若已为5则不需修改)

vi /etc/inittab

打开/etc/gdm/custom.conf，在[security]和[xdmcp]字段下分别添加如下内容：

[security]
AllowRemoteRoot=true
[xdmcp]
Port=177
Enable=1

关闭防火墙(service iptables stop)或在防火墙上打开udp 协议177 端口，重启机器

![](http://www.linuxidc.com/upload/2015_08/150827163859831.png)

在Windows上打开XBrowser通过IP即可远程连接CentOS

![](http://www.linuxidc.com/upload/2015_08/150827163859832.png)

Linux操作系统入门教程：Xmanager 远程访问Linux图形化界面  [http://www.linuxidc.com/Linux/2015-05/117893.htm](http://www.linuxidc.com/Linux/2015-05/117893.htm)

[RedHat](http://www.linuxidc.com/topicnews.aspx?tid=10) Enterprise Linux 6.4配置Xmanager 4 [http://www.linuxidc.com/Linux/2013-05/85075.htm](http://www.linuxidc.com/Linux/2013-05/85075.htm)

AIX 5.3 下配置 Xmanager 图形化界面 [http://www.linuxidc.com/Linux/2013-02/79340.htm](http://www.linuxidc.com/Linux/2013-02/79340.htm)

使用Xmanager连接CentOS 5.5 [http://www.linuxidc.com/Linux/2012-12/77035.htm](http://www.linuxidc.com/Linux/2012-12/77035.htm)

Xmanager使用总结 [http://www.linuxidc.com/Linux/2012-11/74870.htm](http://www.linuxidc.com/Linux/2012-11/74870.htm)

更多CentOS相关信息见[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) 专题页面 [http://www.linuxidc.com/topicnews.aspx?tid=14](http://www.linuxidc.com/topicnews.aspx?tid=14)

**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2015-08/122356.htm](http://www.linuxidc.com/Linux/2015-08/122356.htm)









