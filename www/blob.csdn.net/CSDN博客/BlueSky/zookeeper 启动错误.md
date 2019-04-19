# zookeeper 启动错误 - BlueSky - CSDN博客
2015年11月23日 12:23:05[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1650
## [zookeeper.out: Permission denied错误解决方法](http://blog.csdn.net/mrcharles/article/details/46771483)
zkServer.sh: line 109: ./zookeeper.out: Permission denied  STARTED
从问题描述，可以看出都是权限问题。在第二个提示中，找到第109行。发现如下：
** 109     nohup $JAVA "-Dzookeeper.log.dir=${ZOO_LOG_DIR}" "-Dzookeeper.root.logger=${ZOO_LOG4J_PROP}" \**
​所以，怀疑是这里有问题。并且，echo ${ZOO_LOG_DIR} ，也没有任何值。
修改etc/profile 增加export ZOO_LOG_DIR=/opt/software/zookeeper-3.4.6/log
source /etc/profile 
再次start zookeeper服务，问题解决
## Error contacting service. It is probably not running.
[hadoop@Master zookeeper]$ bin/zkServer.sh status
JMX enabled by default
Using config: /usr/local/zookeeper/bin/../conf/zoo.cfg
Error contacting service. It is probably not running.
参考了一些blog后，把其他节点的zookeeper也手动启动服务，然而当时并没有什么反应，同样的错误，但是过了一段时间吃了个饭，回来status发现，正常了。原因，感觉是zookeeper之前没有正常关闭，造成的。
