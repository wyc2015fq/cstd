# zabbix主动被动模式说明/区别 - DoubleLi - 博客园






**说明：**
使用zabbix代理有很多好处，一方面可以监控不可达的远程区域；另一方面当监控项目数以万计的时候使用代理可以有效分担zabbix server压力，也简化分布式监控的维护。

**具体：**
主动、被动模式都是相对于proxy来说的。proxy主动发送数据就是主动模式；proxy等待server的请求，再发送数据就是被动模式。因为主动模式可以有效减轻zabbix server压力，需要监控的东西很多时一定要把监控模式更改为主动监控

**被动模式流程**，被动模式一定要记得设置Server ＝ ServerIP
被动模式工作流程:
Server 打开一个TCP连接
Server发送一个key 为agent.ping
Agent接受这个请求，然后响应< HEADER >< DATALEN >
Server对接受到的数据进行处理
TCP连接关闭

**主动模式流程**，主动模式一定要记得设置ServerActive=ServerIP
Agent向Server建立一个TCP连接
Agent请求需要检测的数据列表
Server响应Agent，发送一个Items列表
Agent允许响应
TCP连接完成本次会话关闭
Agent开始周期性地收集数据

**附录：**
**附录1.proxy大体安装步骤**
1、安装zabbix proxy 启动–enable-proxy参数

2、创建数据库，注意proxy只需也只能导入schema.sql，导入其它sql，如data.sql会出现某些错误
create database zabbix-proxy character set utf8; //#创建库
grant all privileges on zabbix-proxy.* to zabbix@‘localhost’ identifide by ‘zabbix’ //#数据库授权
flush privileges; //刷新库
mysql -uzabbix -pzabbix zabbix-proxy < schema.sql //#与zabbix-server不同的是,zabbix-proxy 只需也只能导入schema.sql 3、配置zabbix_proxy.conf ProxyMode 0 主动模式 1被动模式 Server 该参数工作于主动模式中,从x.x.x.xip中的zabbix-server获取监控项目 ServerPort 默认为10051，工作于主动模式中 Hostname 代理主机名，不可重复具有唯一性 HostnameItem HostnameItem=system.hostname 当Hostname没有定义才生效 ListenPort zabbix-proxy 默认端口号 SourceIP 多网卡环境中 指定zabbix-proxy外网连接的ip DBHost zabbix-proxy 数据库ip DBName zabbix-proxy 数据路名称 DBUser zabbix-proxy 数据库用户 DBPasswd zabbix-proxy 数据库密码 DBSock zabbix-proxy mysqk.sock 文件 4、启动zabbix-proxy # service zabbix-proxy start # chkconfig zabbix-proxy on 5、zabbix GUI配置Zabbix-Proxy Administrator－Proxies－Create proxy Proxy name 即zabbix-proxy的 Hostname(与zabbix_proxy.conf Hostname一致) Proxy mode zabbix工作模式 Active(主动) passive(被动) Proxy host 选择哪些主机机器处于Proxy模式中 Description 添加描述信息 6、在添加主机监控的时候Monitored proxy 选择Zabbix Proxy









