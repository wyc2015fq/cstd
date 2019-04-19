# Nagios系列1，选择 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年09月21日 11:24:37[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：61
Zabbix和Nagios哪个更好
zabbix:
1.分布式监控，适合于构建分布式监控系统，具有node，proxy 2种分布式模式
2.自动化功能，自动发现，自动注册主机，自动添加模板，自动添加分组，是天生的自动化运维利器的首选，当然于自动化运维工具搭配，
puppet+zabbix，或者saltstack+zabbix，那是如鱼得水。
3,自定义监控比较方便，自定义监控项非常简单，支持变量，支持low level discovery,参考： [http://itnihao.blog.51cto.com/1741976/1129725](http://itnihao.blog.51cto.com/1741976/1129725)
4.触发器，也就是报警条件有多重判断机制
5.支持多种监控方式，agentd，snmp，ipmi，jmx
6.提供api功能，二次开发方便，你可以选用zabbix来进行二次深度开发，结合cmdb资产管理系统，业务管理系统，从而使你的自动化运维系统达到新的高度。
nagios:
nagios最大的亮点是轻量灵活，且报警机制很强，如果你只是需要监控服务器/服务是否在运行，nagios足矣。
Nagios 要更轻量、灵活，特别是它的插件机制，你可以用自己熟悉的语言实现几乎任何自己想要实现的监控。
综上，zabbix和nagios不存在谁好谁坏的问题，只存在你的需求不同需要选择哪个更好的问题：
如果只为了报警，nagios吧，足够满足需求；
如果需要图表，zabbix吧，别用cacti,nagios+cacti的结合是不如zabbix的all in one方式的。
zabbix相对来说配置简单，基本上在web上配置就行了，nagios相对比较麻烦。
艺搜参考
[开源系统监控软件](http://www.oschina.net/project/tag/146/system-monitor)
[Zabbix 和 Nagios 哪个更好](http://www.zhihu.com/question/19973178)
[https://www.centos.bz/tag/nagios/](https://www.centos.bz/tag/nagios/)
[Linux下Nagios的安装与配置](http://www.cnblogs.com/mchina/archive/2013/02/20/2883404.html)
