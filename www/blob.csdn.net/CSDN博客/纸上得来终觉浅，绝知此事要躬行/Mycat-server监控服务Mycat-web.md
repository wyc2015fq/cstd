# Mycat-server监控服务Mycat-web - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月05日 20:44:48[boonya](https://me.csdn.net/boonya)阅读数：3071








Mycat-web 是对mycat-server提供监控服务。功能不局限于对mycat-server使用。他基于jmx对所有JVM监控。通过JDBC连接对Mycat、Mysql监控。基于snmp协议，监控远程服务器(目前仅限于linux系统)的cpu、内存、网络、磁盘。

GitHub源码：[https://github.com/boonyachengdu/Mycat-Web](https://github.com/boonyachengdu/Mycat-Web)


支持SQL上线，基于sqlwatch开源项目，具备SQL语法检查和备份数据识别，释放DBA精力。###** 新增 mycat all in one 镜像下载**

mycat all in one 是 mycat-server,mycat-web,mysql，zookeeper 于一身的测试开发环境，是您开发测试必备良器，您只需要执行如下几个步骤便可开启属于您的 mycat 之旅 :






> 
**导入 OVA **
- 安装Oracle VM VirtualBox
- 启动Oracle VM VirtualBox
- 下载 mycat-all-in-one 镜像文件，[戳这里下载all-in-one镜像](http://pan.baidu.com/s/1qWMkJPM),密码：v63y
- File（管理） -> Import Appliances（导入虚拟电脑）<网络模式首选桥接模式>
- 选择CentOS 7.ova
- 一路Next


> 
** 启动虚拟机 **
- 登录虚拟机 root/123456
- 启动多实例Mysql


```
mysqld_multi start
```

> 
**启动 Mycat**


```
cd /opt/mycat/
   ./bin/mycat start
```

> 
**  ZK启动 **


```
cd /opt/zookeeper-3.4.6
	bin/zkServer.sh start
	bin/zkCli.sh
```

> 
**体验 Mycat**
- 启动Navicat Premium
- 连接Mycat，IP:8066 test/test
- 连接TESTDB
- 测试


```
select * from t_user;
```

> 
**  mycat eye启动 **


```
cd /opt/mycat-web
	./start.sh 
    访问地址：http://localhost:8082/mycat
```

**请留意 '体验 Mycat'该步骤中的 IP 地址的设定，虚拟机中 IP 地址若与主机地址不匹配会引发连接失败的情况，此时可以将 虚拟机IP 地址修改静态IP地址来解决，修改位于路径**

```
/etc/sysconfig/network-scripts/ifcfg-enp0s3
```

下面的文件，然后运行命令

```
service network restart
```

来让刚刚修改过的文件生效即可

mycat web SQL上线配置参考（可选）

```
修改mycat.properties文件的访问sqlwatch参数配置，具体参考sqlwatch.md
```




