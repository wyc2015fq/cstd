# 【原】【亲测】docker 安装 zabbix - weixin_33985507的博客 - CSDN博客
2017年07月19日 15:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：16
参考地址：[http://blog.csdn.net/u012373815/article/details/71598457](https://link.jianshu.com?t=http://blog.csdn.net/u012373815/article/details/71598457)
Zabbix 是由 Alexei Vladishev 开发的一种网络监视、管理系统，基于 Server-Client[架构](https://link.jianshu.com?t=http://lib.csdn.net/base/architecture)。可用于监视各种网络服务、服务器和网络机器等状态。（from Zabbix）
Zabbix 作为企业级分布式监控系统，具有很多优点，如：分布式监控，支持 node 和 proxy 分布式模式；自动化注册，根据规则，自动注册主机到监控平台，自动添加监控模板；支持 agentd、snmp、ipmi 和 jmx 等很多通信方式。
同时，Zabbix 官方还发布了 Zabbix[Docker](https://link.jianshu.com?t=http://lib.csdn.net/base/docker)镜像。此次我们以 Zabbix 的官方[docker](https://link.jianshu.com?t=http://lib.csdn.net/base/docker)镜像为基础，搭建一个监控平台。
今天搭建的zabbix分为三个部分
zabbix 分三个部分，agent 和 server 、web 三部分
zabbix-agent 部署在被监控机上面,zabbix-server (建议部署在另外一台主机上),zabbix-agent 会发送数据到zabbix-server 或者zabbix-server 主动索取数据，zabbix-server 将获取的数据存在[MySQL](https://link.jianshu.com?t=http://lib.csdn.net/base/mysql)[数据库](https://link.jianshu.com?t=http://lib.csdn.net/base/mysql)中（或者其他的数据库）. (zabbix.com 官网的manual 上面有安装配置) 。web 从server上获取数据，然后展示给用户。
1. 安装mysql
这里的mysql 也采用了 docker的安装部署方式，目的是为了与业务库做到隔离，暂时省略。
2. 安装zabbix－server
拉取镜像
docker pull zabbix/zabbix-server-mysql
启动镜像命令
docker run  --name some-zabbix-server-mysql  -p 10051:10051 --net=host -e DB_SERVER_HOST="数据库ip" -e DB_SERVER_PORT=数据库端口  -e MYSQL_USER="数据库用户名" -e MYSQL_PASSWORD="数据库密码" -d zabbix/zabbix-server-mysql
3. 安装zabbix-web-apache-mysql
拉取镜像
docker pull zabbix/zabbix-web-apache-mysql
启动命令
docker run --name some-zabbix-web-apache-mysql -p 8088:80 -e DB_SERVER_HOST="数据库ip" -e DB_SERVER_PORT=数据库端口  -e MYSQL_USER="数据库用户名" -e MYSQL_PASSWORD="数据库密码" -e ZBX_SERVER_HOST="zabbix服务器IP" -e TZ="Asia/Shanghai" -d zabbix/zabbix-web-apache-mysql
4. 安装agent
在需要监控的机器上 安装agent
拉去镜像
docker pull zabbix/zabbix-agent
启动命令
docker run --name some-zabbix-agent -p 10050:10050 -e ZBX_HOSTNAME="hostname"  -e  ZBX_SERVER_HOST="zabbix服务器IP"  -e ZBX_SERVER_PORT=10051 -d zabbix/zabbix-agent
此时安装成功了。访问安装web 的服务器ip 端口号为8088 进入zabbix 登录页面，默认帐号为Admin 密码为 zabbix 登录后就可以配置自己的监控了。
注意：这里的hostname要写被监视的服务器的ip，否则zabbix-web-apache-mysql否则中添加主机与里对应不上是会出问题的。
5. 使用
5.1 配置自动发现规则
配置——自动发现——创建发现规则
![6151220-00b1652ff564f4dc.png](https://upload-images.jianshu.io/upload_images/6151220-00b1652ff564f4dc.png)
这里的重点是IP范围，我的被监视的linux服务器的ip范围是192.168.30.1-254.
然后使用的过程中，在监视——自动发现中可以看到如下的东西
![6151220-17a36159fdd4c7f6.png](https://upload-images.jianshu.io/upload_images/6151220-17a36159fdd4c7f6.png)
5.2添加主机
配置——主机——创建主机
![6151220-53e139efdbccdbb3.png](https://upload-images.jianshu.io/upload_images/6151220-53e139efdbccdbb3.png)
如上图是我已经添加好的主机，基本在添加的时候录入信息也就这些，注意的几个点，首先是前面说的hostname问题。这里的ip要跟上面第三步的hostname一一对应，否则是会监听失败的，这里其实docker启动agent的时候录入的hostname会被用来生成agent的配置文件，有兴趣可以进入到docker容器查一下。
![6151220-f34702b1533d9daf.png](https://upload-images.jianshu.io/upload_images/6151220-f34702b1533d9daf.png)
如上图是添加监视的生成的图标的模板，用来说生成监视报表用的。
![6151220-5e19f742bef31f96.png](https://upload-images.jianshu.io/upload_images/6151220-5e19f742bef31f96.png)
![6151220-50be0b326a86d99e.png](https://upload-images.jianshu.io/upload_images/6151220-50be0b326a86d99e.png)
选择完之后一定要点击添加，然后更新，否则是不会生效的。
![6151220-a8baa6a9c0c49763.png](https://upload-images.jianshu.io/upload_images/6151220-a8baa6a9c0c49763.png)
配置添加完之后看到主机的可用性变成了绿色，说明添加成功了。
5.3查看报告
![6151220-3553197a2636f13e.png](https://upload-images.jianshu.io/upload_images/6151220-3553197a2636f13e.png)
过一段时间，在监视中，图形里看到如图的报告，说明监视成功了。
