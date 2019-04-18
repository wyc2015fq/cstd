# 二十六、storm安装与初识 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:50:29[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：271












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_117_default_big.jpeg)



storm是如今用来做实时数据处理的首选，它的高效以及基于分布式系统的考虑备受技术团队青睐，尤其是实时日志处理，以及基于日志的实时分析，从本章节开始我们来认识一下这个强大的开源工具，并通过这个工具来做一个非常有意思的事情，敬请期待

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装zookeeper



zookeeper是storm运行强依赖

注意：

1）用supervision启动zookeeper，保证不能退出

2）用cron定期清理zookeeper的日志数据，不然磁盘会很快占满（以后再研究）



安装步骤：

```
wget http://mirrors.hust.edu.cn/apache/zookeeper/zookeeper-3.4.8/zookeeper-3.4.8.tar.gz
tar zxvf zookeeper-3.4.8.tar.gz
cd zookeeper-3.4.8/
cp conf/zoo_sample.cfg conf/zoo.cfg
```

修改conf/zoo.cfg，把
`dataDir=/tmp/zookeeper`
改成
`dataDir=/var/zookeeper`
创建/var/zookeeper/myid，内容为数字1

执行
`./bin/zkServer.sh start`
成功启动

验证方法：
`./bin/zkCli.sh`
输入help命令查看帮助



为了让zookeeper异常退出后能自动重启，需要安装deamontools

```bash
wget http://cr.yp.to/daemontools/daemontools-0.76.tar.gz
tar zxvf daemontools-0.76.tar.gz
cd admin/daemontools-0.76/
```

vim src/error.h 找到：extern int errno; 改成：#include <errno.h>

执行

```java
package/install
```

这时已经安装好了

```
[root@centos7vm daemontools-0.76]# which supervise
/usr/local/bin/supervise
```

创建/data/service/zookeeper/run文件，内容为：

```bash
#!/bin/bash
exec 2>&1
exec /data/zookeeper-3.4.8/bin/zkServer.sh start
```

增加执行权限
`chmod +x /data/service/zookeeper/run`
杀了之前手工启动的zookeeper，然后执行

```bash
cd /data/service/zookeeper
nohup supervise /data/service/zookeeper &
```

这时zookeeper被supervice启动了，尝试杀一次zookeeper后还会自动起来

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

参考：[http://zookeeper.apache.org/doc/r3.3.3/zookeeperAdmin.html](http://zookeeper.apache.org/doc/r3.3.3/zookeeperAdmin.html)





## 安装storm



```
wget http://apache.fayea.com/storm/apache-storm-1.0.1/apache-storm-1.0.1.tar.gz
tar apache-storm-1.0.1.tar.gz
cd apache-storm-1.0.1
```

修改conf/storm.yaml，添加如下配置

```
storm.zookeeper.servers:
    - "127.0.0.1"
nimbus.seeds: ["127.0.0.1"]
supervisor.slots.ports:
    - 6700
    - 6701
    - 6702
    - 6703
```



启动storm，执行



```
./bin/storm nimbus &
./bin/storm supervisor &
./bin/storm ui &
```



打开web界面，[http://127.0.0.1:8080](http://172.16.142.136:8080/index.html)

界面如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/22d8818e1d83e129e6c48f58f060abc296b679bf.png)



安装完成



## 讲讲storm



storm系统由一个nimbus节点和多个supervisor节点组成，上面因为是部署单机版本，所以只启动了一个supervisor。他们之间是通过zookeeper协调运行的，所以必须依赖zookeeper。nimbus负责分配任务和监控任务，本身不做计算，supervisor负责真正的计算任务。

storm上运行的任务和map-reduce的不同在于它运行的是一种topology任务，也就是一种有向无环图形式的任务服务。

上面配置文件中配置的supervisor.slots.ports包含了4个port，也就是这个supervisor可以监听4个端口同时并发的执行4个任务，因此在web界面里我们看到Free slots是4

在map-reduce系统上运行的任务我们叫做mapper和reducer，相对之下，在storm上运行的任务叫做spout（涛涛不绝地喷口）和bolt（螺栓），在拓扑里传递的消息叫做tuple。spout其实就是信息产生的源头，而bolt就是处理逻辑



下一节我们来试验一些简单的用途，来发觉storm可以用来做些什么事情




