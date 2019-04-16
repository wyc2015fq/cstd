# DotCMS安装步骤 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年06月11日 00:45:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：66








今天安装了DotCMS，是一个很好用的CMS系统，在此记录一下它的安装过程（Windows平台）：

1.官网上下载dotcms_2.3.1.zip（最新的）

2.将其中的dotserver解压到任意目录

3.配置dotserver下的tomcat/conf/Catalina/localhost/ROOT.xml。改变数据库设置为MySQL（默认使用PostgreSQL），同时配置需要连接的数据库的名称，用户名，密码

4.打开MySQL客户端，创建DotCMS的数据库（注意，DotCMS不会自己创建数据库，此步是必须的），注意名称要与上一步的一致

5.双击dotserver下的bin/startup.bat启动服务器。第一启动会创建数据表，速度比较慢；以后每次启动会比较快。

6.上一步结束后，访问http://localhost/即可访问网站，访问http://localhost/admin即可访问后台



