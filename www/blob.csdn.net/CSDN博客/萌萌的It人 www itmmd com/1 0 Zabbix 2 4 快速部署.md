
# 1.0 Zabbix 2.4 快速部署 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月26日 17:30:57[Jlins](https://me.csdn.net/dyllove98)阅读数：2946标签：[Zabbix 2.4																](https://so.csdn.net/so/search/s.do?q=Zabbix 2.4&t=blog)


1. 环境准备
CentOS 6.4
关闭selinux和iptables
两台虚拟机：一台Server，一台Agent

2. 安装官方yum源
所有服务器都要安装此yum源
|1
|rpm -ivh http:|//repo|.zabbix.com|/zabbix/2|.4|/rhel/6/x86_64/zabbix-release-2|.4-1.el6.noarch.rpm
|


3. 部署Zabbix Server
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
|yum|install|zabbix-server-mysql zabbix-web-mysql zabbix-agent  mysql-server mysql -y
|/etc/init|.d|/mysqld|start
|mysql -e|"create database zabbix character set utf8 collate utf8_bin;"
|mysql -e|"grant all privileges on zabbix.* to zabbix@localhost identified by 'zabbix';"
|cd|/usr/share/doc/zabbix-server-mysql-2|.4.*|/create/
|mysql -uroot zabbix < schema.sql
|mysql -uroot zabbix < images.sql
|mysql -uroot zabbix < data.sql
|sed|-i|'s/^.*DBPassword=.*$/DBPassword=zabbix/'|/etc/zabbix/zabbix_server|.conf
|service zabbix-server start
|service zabbix-agent start
|sed|-i|'s/^.*date.timezone =.*$/date.timezone = Asia\/Shanghai/g'|/etc/php|.ini
|sed|-i|'s/^.*post_max_size =.*$/post_max_size = 16M/g'|/etc/php|.ini
|sed|-i|'s/^.*max_execution_time =.*$/max_execution_time = 300/g'|/etc/php|.ini
|sed|-i|'s/^.*max_input_time =.*$/max_input_time = 300/g'|/etc/php|.ini
|service httpd restart
|chkconfig mysqld on
|chkconfig zabbix-server on
|chkconfig zabbix-agent on
|chkconfig httpd on
|


打开浏览器访问[http://server_ip/zabbix/](http://kinda22.blog.51cto.com/zabbix/)
![eP3xT.jpg](http://pic.yupoo.com/kinda22/EeFWpw38/eP3xT.jpg)
直接点击Next...

![pE8cI.jpg](http://pic.yupoo.com/kinda22/EeFYI3oo/pE8cI.jpg)
继续点击Next...

![AZQNJ.jpg](http://pic.yupoo.com/kinda22/EeFYTghz/AZQNJ.jpg)
点击'Test connection' 后，继续Next

![5WHiy.jpg](http://pic.yupoo.com/kinda22/EeFYTVdO/5WHiy.jpg)
继续Next...

![6sev1.jpg](http://pic.yupoo.com/kinda22/EeFYKkos/6sev1.jpg)
继续Next...

![L5WDp.jpg](http://pic.yupoo.com/kinda22/EeFYU6LZ/L5WDp.jpg)
点击'Finish' 完成页面的配置.

![GNPQl.jpg](http://pic.yupoo.com/kinda22/EeFYUr4u/GNPQl.jpg)
通过默认的账号和密码进行登录
Admin/zabbix   注意Admin第一个字母大写


将页面改为中文的方法：
![ZAHXi.jpg](http://pic.yupoo.com/kinda22/EeG2lSos/ZAHXi.jpg)
|1
|2
|sed|-i|'/zh_CN/ s/false/true/'|/usr/share/zabbix/include/locales|.inc.php
|/etc/init|.d|/httpd|restart
|

![EAYgd.jpg](http://pic.yupoo.com/kinda22/EeG5s5AK/EAYgd.jpg)[  ](http://pic.yupoo.com/kinda22/EeG5s5AK/EAYgd.jpg)
现在可以看到已经可以选择中文了，上图是已经生效之后的。


启用Zabbix Server自身监控：
![E9xzh.jpg](http://pic.yupoo.com/kinda22/EeG6IAM3/E9xzh.jpg)
按照上图的继续点击，将默认的Zabbix Server自身的监控启用.

图片不能显示中文：
![C2smy.jpg](http://pic.yupoo.com/kinda22/EeG8oUE4/C2smy.jpg)
语言切换到中文后，选择任意的图形，不能显示中文..
|1
|2
|3
|4
|5
|6
|7
|8
|9
|cd|/usr/share/zabbix/fonts
|mv|graphfont.ttf graphfont.ttf.bak
|wget -SO graphfont.ttf http:|//v|.dbsa.cn|/zabbix/DejaVuSans|.ttf
|\#不替换 graphfont.ttf的方法
|\#下载会字体文件 .ttf
|\#vi /usr/share/zabbix/include/defines.inc.php
|\#('ZBX_GRAPH_FONT_NAME',           'graphfont');  \#将graphfont替换为下载的文件名即可
|\#('ZBX_FONT_NAME', 'graphfont');  \#将graphfont替换为下载的文件名即可
|

![UcX9Q.jpg](http://pic.yupoo.com/kinda22/EeGfKsDx/UcX9Q.jpg)

关注日志
|1
|2
|3
|4
|5
|6
|7
|8
|tail|-f|/var/log/zabbix/zabbix_server|.log
|1943:20141126:142146.893 server|\#23 started [history syncer \#4]
|1942:20141126:142146.893 server|\#22 started [history syncer \#3]
|1947:20141126:142146.900 server|\#25 started [proxy poller \#1]
|1946:20141126:142146.900 server|\#24 started [escalator \#1]
|1950:20141126:142146.906 server|\#26 started [self-monitoring \#1]
|1935:20141126:142147.096 server|\#19 started [discoverer \#1]
|1922:20141126:144903.783 cannot send list of active checks to [127.0.0.1]: host [Zabbix server] not monitored
|

其中有一个'cannot
 send list of active checks to ' 这种类型不算错误，后续更新Zabbix Agent(active)会描述这个情况.



4. 部署Zabbix Agent
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
|yum|install|zabbix-agent  zabbix-sender zabbix-get -y
|[root@localhost ~]|\# grep -vE '(^$|^\\#)' /etc/zabbix/zabbix_agentd.conf
|PidFile=|/var/run/zabbix/zabbix_agentd|.pid
|LogFile=|/var/log/zabbix/zabbix_agentd|.log
|LogFileSize=0
|Server=172.16.43.40|\#修改为Server的IP，支持多个IP，逗号分隔，使用域名的话，需要支持反向解析
|ServerActive=172.16.43.40|\#修改为Server的IP，不支持写多个IP
|HostnameItem=system.|hostname|\#默认是用Hostname需要手动指定主机名，改为HostnameItem 使用内部key的自动获取方式
|Include=|/etc/zabbix/zabbix_agentd|.d/
|/etc/init|.d|/zabbix-agent|start
|chkconfig zabbix-agent on
|


关注日志
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
|tail|-n50 -f|/var/log/zabbix/zabbix_agentd|.log
|1526:20141126:150356.024 Starting Zabbix Agent [localhost.localdomain]. Zabbix 2.4.2 (revision 50419).
|1526:20141126:150356.024 using configuration|file|:|/etc/zabbix/zabbix_agentd|.conf
|1526:20141126:150356.025 agent|\#0 started [main process]
|1530:20141126:150356.026 agent|\#4 started [listener \#3]
|1529:20141126:150356.026 agent|\#3 started [listener \#2]
|1531:20141126:150356.026 agent|\#5 started [active checks \#1]
|1528:20141126:150356.027 agent|\#2 started [listener \#1]
|1527:20141126:150356.027 agent|\#1 started [collector]
|1531:20141126:150356.062 no active checks on server [172.16.43.40:10051]: host [localhost.localdomain] not found
|

同样，'no active checks on server' 也不是一个错误，后续更新的blog会进行描述.

5. 在Server添加一个主机使用Agent方式进行监控

![lFmqs.jpg](http://pic.yupoo.com/kinda22/EeGkEyJV/lFmqs.jpg)
![h2EvF.jpg](http://pic.yupoo.com/kinda22/EeGEHsSE/h2EvF.jpg)

![FVoQK.jpg](http://pic.yupoo.com/kinda22/EeGEJA8A/FVoQK.jpg)

![hFE32.jpg](http://pic.yupoo.com/kinda22/EeGEK5wF/hFE32.jpg)
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)[
            ](http://www.itmmd.com/boke.html)

