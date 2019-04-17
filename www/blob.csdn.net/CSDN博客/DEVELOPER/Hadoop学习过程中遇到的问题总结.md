# Hadoop学习过程中遇到的问题总结 - DEVELOPER - CSDN博客





2018年03月10日 21:00:49[学术袁](https://me.csdn.net/u012827205)阅读数：126
所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









## hdfs 中写入文件报错

```java
Permission denied: user=yuanjunhua, access=WRITE, 
inode="/":root:supergroup:drwxr-xr-x at 
org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.checkFsPermission(
FSPermissionChecker.java:265) at 
org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.check(FSPermission
Checker.java:251) at 
org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.check(FSPermission
Checker.java:232) at 
org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.checkPermission(FS
PermissionChecker.java:176) at 
org.apache.hadoop.hdfs.server.namenode.FSNamesystem.checkPermission(FSNamesys
tem.java:5523) at 
org.apache.hadoop.hdfs.server.namenode.FSNamesystem.checkPermission(FSNamesys
tem.java:5505) at 
org.apache.hadoop.hdfs.server.namenode.FSNamesystem.checkAncestorAccess(FSNam
esystem.java:5479) at 
org.apache.hadoop.hdfs.server.namenode.FSNamesystem.startFileInternal(FSNames
ystem.java:2284) at 
org.apache.hadoop.hdfs.server.namenode.FSNamesystem.startFileInt(FSNamesystem
.java:2237) at 
org.apache.hadoop.hdfs.server.namenode.FSNamesystem.startFile(FSNamesystem.ja
va:2190) at 
org.apache.hadoop.hdfs.server.namenode.NameNodeRpcServer.create(NameNodeRpcSe
rver.java:520) at 
org.apache.hadoop.hdfs.protocolPB.ClientNamenodeProtocolServerSideTranslatorP
B.create(ClientNamenodeProtocolServerSideTranslatorPB.java:354) at 
org.apache.hadoop.hdfs.protocol.proto.ClientNamenodeProtocolProtos$ClientName
nodeProtocol$2.callBlockingMethod(ClientNamenodeProtocolProtos.java) at 
org.apache.hadoop.ipc.ProtobufRpcEngine$Server$ProtoBufRpcInvoker.call(Protob
ufRpcEngine.java:585) at org.apache.hadoop.ipc.RPC$Server.call(RPC.java:928)
 at org.apache.hadoop.ipc.Server$Handler$1.run(Server.java:2013) at 
java.security.AccessController.doPrivileged(Native Method) at 
javax.security.auth.Subject.doAs(Subject.java:422) at 
org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.jav
a:1556) at org.apache.hadoop.ipc.Server$Handler.run(Server.java:2007)
```

![这里写图片描述](https://img-blog.csdn.net/20180310205529540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjgyNzIwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

问题：hdfs 写入权限不够，被拒 

解决方法：在hadoop中的  hdfs.xml 添加就ok

```java
<property>
<name>dfs.permissions</name>
<value>false</value>
</property>
```

## jetty run + redis + CentOS7 的web-jps的页面操作

## redis拒绝连接

Eclipase 错误显示：

```java
redis.clients.jedis.exceptions.JedisConnectionException:
java.net.ConnectException: Connection refused: connect
```

![这里写图片描述](https://img-blog.csdn.net/20180311100030127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjgyNzIwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在CentOS7上的错误显示：

```java
Could not connect to Redis at 127.0.0.1:6379: Connection refused
```

![这里写图片描述](https://img-blog.csdn.net/20180311100131607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjgyNzIwNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解决方法： 

在centos7上启动redis服务，此时会出现`Connection refused`，随后执行命令 `redis-server /etc/redis.conf`。也许这样还不算完，因为此时系统 会告诉你，没有这条命令。我是遇到了，怎么处理呢？此时就需要你把`redis.conf` 中的代码`daemonize no` 改成`daemonize yes` 并把该文件拷贝一份到目录`/etc/` 下，然后再次执行命令 `redis-server /etc/redis.conf`——>`redis-cli` 则ok！



