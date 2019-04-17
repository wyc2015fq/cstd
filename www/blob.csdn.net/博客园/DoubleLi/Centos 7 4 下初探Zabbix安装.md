# Centos 7.4 下初探Zabbix安装 - DoubleLi - 博客园







工作一波停一波起，感觉离开.net好久了。

最近工作中发现服务器监视都是用了zabbix，对于我这类不懂的狠狠弥补了一下知识。

无意发现zabbix带有api，就想开发个工具调用api来着。可是api调用在host.get就卡住了，就是不返回理想结果。

种种结果决定在个人PC上搭建个zabbix调试，然后各种问题就来了。



### Zabbix安装流程概述
- 
###### 安装Zabbix部署包

- 
###### 安装Mysql数据库

- 
###### 初始化数据库

- 
###### 配置并启动Zabbix Server进程

- 
###### 编辑Zabbix前端的PHP配置

- 前端Web配置
- 配置Zabbix agent进程

### Zabbix是什么

Zabbix是一个**企业级的、开源的、分布式**的监控套件

Zabbix可以**监控网络和服务**的监控状况. Zabbix利用灵活的告警机制，允许用户对事件发送基于**Email的告警**. 这样可以保证快速的对问题作出相应. Zabbix可以利用存储数据提供**杰出的报告及图形化**方式. 这一特性将帮助用户完成容量规划.

Zabbix支持polling和trapping两种方式. 所有的Zabbix报告都可以通过配置参数在WEB前端进行访问. Web前端将帮助你在任何区域都能够迅速获得你的网络及服务状况. Zabbix可以通过尽可能的配置来扮演监控你的IT基础框架的角色，而不管你是来自于小型组织还是大规模的公司.

Zabbix是零成本的. 因为Zabbix编写和发布基于GPL V2协议. 意味着源代码是免费发布的.



### Zabbix特性



### 数据收集[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id2)
- 可用性及性能检测
- 支持SNMP(trapping及polling)、IPMI、JMX监控
- 自定义检测
- 自定义间隔收集收据
- server/proxy/agents吸能



### 灵活的阀值定义[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id3)
- 允许灵活地自定义问题阀值，Zabbix中称为触发器(trigger), 存储在后端数据库中



### 高级告警配置[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id4)
- 可以自定义告警升级(escalation)、接收者及告警方式
- 告警信息可以配置并允许使用宏(macro)变量
- 通过远程命令实行自动化动作(action)



### 实时绘图[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id5)
- 通过内置的绘图方法实现监控数据实时绘图



### 扩展的图形化显示[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id6)
- 允许自定义创建多监控项视图
- 网络拓扑(network maps)
- 自定义的面板(screen)和slide shows，并允许在dashboard页面显示
- 报告
- 高等级(商业)监控资源



### 历史数据存储[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id7)
- 数据存储在数据库中
- 历史数据可配置
- 内置数据清理机制



### 配置简单[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id8)
- 主机通过添加监控设备方式添加
- 一次配置，终生监控(译者注：除非调整或删除)
- 监控设备允许使用模板



### 模板使用[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id9)
- 模板中可以添加组监控
- 模板允许继承



### 网络自动发现[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id10)
- 自动发现网络设备
- agent自动注册
- 自动发现文件系统、网卡设备、SNMP OID等



### 快速的web接口[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#web)
- web前端采用php编写
- 访问无障碍
- 你想怎么做就能做么做
- 审计日志



### Zabbix API[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#zabbix-api)
- Zabbix API提供程序级别的访问接口，第三方程序可以很快接入



### 权限系统[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id11)
- 安全的权限认证
- 用户可以限制允许维护的列表



### 全特性、agent易扩展[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#agent)
- 在监控目标上部署
- 支持Linux及Windows



### 二进制守护进程[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id12)
- C开发，高性能，低内存消耗
- 易移植


### 具备应对复杂环境情况[¶](https://www.cnblogs.com/lixiaobin/p/Zabbix.html#id13)

- 通过Zabbix proxy可以非常容易的创建远程监控




### Zabbix安装


1，安装源码库配置部署包

安装源码库配置部署包。这个部署包包含了yum配置文件。

*rpm -ivh *[*http://repo.zabbix.com/zabbix/3.4/rhel/7/x86_64/zabbix-release-3.4-1.el7.centos.noarch.rpm*](http://repo.zabbix.com/zabbix/3.4/rhel/7/x86_64/zabbix-release-3.4-1.el7.centos.noarch.rpm)



2，安装Zabbix部署包

*yum install zabbix-server-mysql zabbix-web-mysql*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141634309-1702385318.png)



3，安装Zabbix Agent

*yum install zabbix-agent –y*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141636434-1912130298.png)

4，安装Mysql数据库
- 安装下载文件工具

*yum install wget -y*
- 根据Linux发行版本（CentOS、Fedora都属于红帽系），从mysql官方（http://dev.mysql.com/downloads/repo/yum/）获取Yum Repository

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141638246-2000104105.png)

*wget -i *[*http://dev.mysql.com/get/mysql57-community-release-el7-11.noarch.rpm*](http://dev.mysql.com/get/mysql57-community-release-el7-11.noarch.rpm)
- 
###### 安装MySQL的Yum Repository


*yum install mysql57-community-release-el7-11.noarch.rpm*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141640387-777086938.png)
- 安装MySQL数据库的服务器版本

*yum install mysql-community-server -y*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141642434-645058103.png)
- 启动数据库：*systemctl start  mysqld.service*

     使用命令`*systemctl status mysqld.service*`查看MySQL数据库启动后的服务状态：

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141644590-1401937999.png)
- 
###### 获取初始密码


    使用YUM安装并启动MySQL服务后，MySQL进程会自动在进程日志中打印root用户的初始密码：

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141646090-1172402800.png)
- 
###### 修改root用户密码


     使用上面获取的root用户的初始密码，然后进行修改：

*ALTER USER 'root'@'localhost' IDENTIFIED BY 'new password';*
- 大写／小写／数字／符号必须都有
- 至少8字符以上

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141647824-1617893552.png)



5，创建zabbix 数据库，并授权zabbix用户，以及设置zabbix用户登陆密码

*# mysql –uroot -p*

*mysql> create database zabbix character set utf8 collate utf8_bin;*

*mysql> grant all privileges on zabbix.* to zabbix@localhost identified by *[*'Zabbix@1'*](mailto:'Zabbix@1')*;*

*mysql> quit;*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141649465-1434971648.png)

6，进入zabbix数据库脚本导入的目录，将库数据导入zabbix数据库中.

*zcat /usr/share/doc/zabbix-server-mysql-3.4.2/create.sql.gz|mysql -uzabbix -p zabbix*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141651137-564328531.png)

查看插入数据

*use zabbix;show tables*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141652980-607622476.png)

7，修改zabbix－server的配置文件

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141654605-486362186.png)

修改完成后，启动zabbix－server 服务

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141656012-1291995863.png)

###### 8，编辑Zabbix前端的PHP配置 

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141657434-1073598754.png)

  启动httpd将服务添加开机启动项

*systemctl start httpd*

*systemctl enable httpd*



9，客户端访问web安装页面：[http://192.168.59.128/zabbix](http://192.168.59.128/zabbix)

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141659324-785989409.png)

web界面会自动检查所有的条件是否满足，未满足的会提示。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141700949-1817417416.png)

配置数据库连接 。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141702559-1148516511.png)

默认不需要修改，只需要自己随便设置一个name就可以了。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141704105-1055261424.png)

所有的设置信息全部显示。![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141705637-486886082.png)

显示成功安装zabbix，并告知配置文件及路径 。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141707262-662018466.png)

登录界面，默认用户名Admin，默认密码zabbix。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141708652-489377359.png)

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141710387-1879173723.png)

改成中文显示 。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141712199-968894859.png)

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141714105-2105064129.png)

首次登陆需要启用主机 。

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141716012-1458806045.png)



10，配置zabbix-agent

*sed -i 's/Server=127.0.0.1/Server=192.168.1.125/g' /etc/zabbix/zabbix_agentd.conf*

*sed -i 's/ServerActive=127.0.0.1/ServerActive=192.168.1.125/g' /etc/zabbix/zabbix_agentd.confsed -i "s/Hostname=Zabbix server/Hostname=**agent1**/g" /etc/zabbix/zabbix_agentd.confservice zabbix-agent restart*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141717309-568241947.png)

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141719590-605513035.png)

点击Templates 设置关联模板Template OS Linux 并add。



### 总结

总算是安装上去了，感觉挺麻烦的，如果有docker安装文件就好了。接下来可以慢慢研究API的工具了。



### 待解决问题

zabbix-server居然启动失败

##### *PID file /run/zabbix/zabbix_server.pid not readable (yet?)*

![image](https://images2017.cnblogs.com/blog/319628/201710/319628-20171009141721293-430759275.png)

上网收了不少方法都没法解决，有经验的帮忙解答啊


每天成就一小步，积累下来就是一大步。 转发本文请注明出处，谢谢您的阅读与分享！



from:https://www.cnblogs.com/lixiaobin/p/Zabbix.html









