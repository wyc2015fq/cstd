# Hadoop集群多次格式化导致容量分配为0不能导入数据 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月03日 15:28:10[boonya](https://me.csdn.net/boonya)阅读数：460
所属专栏：[Hadoop学习实践](https://blog.csdn.net/column/details/14484.html)









**目录**

[遇到问题](#%E9%81%87%E5%88%B0%E9%97%AE%E9%A2%98)

[解决问题](#%E8%A7%A3%E5%86%B3%E9%97%AE%E9%A2%98)

[验证处理情况](#%E9%AA%8C%E8%AF%81%E5%A4%84%E7%90%86%E6%83%85%E5%86%B5)

[查看集群状态](#%E6%9F%A5%E7%9C%8B%E9%9B%86%E7%BE%A4%E7%8A%B6%E6%80%81)

## 遇到问题

使用hadoop  fs  -put 命令上传文件发现失败，报了以下错误：

```
03/01/19 15:18:03 WARN hdfs.DFSClient: DataStreamer Exception
org.apache.hadoop.ipc.RemoteException(java.io.IOException): File /home/input/file1.txt._COPYING_ could only be replicated to 0 nodes instead of minReplication (=1).  There are 2 datanode(s) running and no node(s) are excluded in this operation.
        at org.apache.hadoop.hdfs.server.blockmanagement.BlockManager.chooseTarget(BlockManager.java:1384)
        at org.apache.hadoop.hdfs.server.namenode.FSNamesystem.getAdditionalBlock(FSNamesystem.java:2477)
        at org.apache.hadoop.hdfs.server.namenode.NameNodeRpcServer.addBlock(NameNodeRpcServer.java:555)
        at org.apache.hadoop.hdfs.protocolPB.ClientNamenodeProtocolServerSideTranslatorPB.addBlock(ClientNamenodeProtocolServerSideTranslatorPB.java:387)
        at org.apache.hadoop.hdfs.protocol.proto.ClientNamenodeProtocolProtos$ClientNamenodeProtocol$2.callBlockingMethod(ClientNamenodeProtocolProtos.java:59582)
        at org.apache.hadoop.ipc.ProtobufRpcEngine$Server$ProtoBufRpcInvoker.call(ProtobufRpcEngine.java:585)
        at org.apache.hadoop.ipc.RPC$Server.call(RPC.java:928)
        at org.apache.hadoop.ipc.Server$Handler$1.run(Server.java:2048)
        at org.apache.hadoop.ipc.Server$Handler$1.run(Server.java:2044)
        at java.security.AccessController.doPrivileged(Native Method)
        at javax.security.auth.Subject.doAs(Subject.java:396)
        at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1491)
        at org.apache.hadoop.ipc.Server$Handler.run(Server.java:2042)
        at org.apache.hadoop.ipc.Client.call(Client.java:1347)
        at org.apache.hadoop.ipc.Client.call(Client.java:1300)
        at org.apache.hadoop.ipc.ProtobufRpcEngine$Invoker.invoke(ProtobufRpcEngine.java:206)
        at $Proxy9.addBlock(Unknown Source)
        at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
        at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)
        at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)
        at java.lang.reflect.Method.invoke(Method.java:597)
        at org.apache.hadoop.io.retry.RetryInvocationHandler.invokeMethod(RetryInvocationHandler.java:186)
        at org.apache.hadoop.io.retry.RetryInvocationHandler.invoke(RetryInvocationHandler.java:102)
        at $Proxy9.addBlock(Unknown Source)
        at org.apache.hadoop.hdfs.protocolPB.ClientNamenodeProtocolTranslatorPB.addBlock(ClientNamenodeProtocolTranslatorPB.java:330)
        at org.apache.hadoop.hdfs.DFSOutputStream$DataStreamer.locateFollowingBlock(DFSOutputStream.java:1226)
        at org.apache.hadoop.hdfs.DFSOutputStream$DataStreamer.nextBlockOutputStream(DFSOutputStream.java:1078)
```

上面的问题是由于没有分配有效的存储容量，hadoop dfsadmin -report 查看错误日志：

```
[root@master sbin]# hadoop dfsadmin -report
WARNING: Use of this script to execute dfsadmin is deprecated.
WARNING: Attempting to execute replacement "hdfs dfsadmin" instead.

Configured Capacity: 0 (0 B)
Present Capacity: 0 (0 B)
DFS Remaining: 0 (0 B)
DFS Used: 0 (0 B)
DFS Used%: 0.00%
Replicated Blocks:
	Under replicated blocks: 0
	Blocks with corrupt replicas: 0
	Missing blocks: 0
	Missing blocks (with replication factor 1): 0
	Pending deletion blocks: 0
Erasure Coded Block Groups: 
	Low redundancy block groups: 0
	Block groups with corrupt internal blocks: 0
	Missing block groups: 0
	Pending deletion blocks: 0
```

## 解决问题
- 到sbin目录停止集群
	`./stop-all.sh`


- 清理各个集群节点的数据目录（都需要清理否则还是不能分配）
	
```
rm -rf  /root/hadoop/tmp/*
rm -rf  /root/hadoop/var/*
rm -rf  /root/hadoop/dfs/name/*
rm -rf  /root/hadoop/dfs/data/*
```



- 到bin目录重新格式化
	`./hadoop namenode -format`


- 重新启动集群
	`./start-all.sh`



## 验证处理情况

```
[root@master sbin]# hadoop dfsadmin -report
WARNING: Use of this script to execute dfsadmin is deprecated.
WARNING: Attempting to execute replacement "hdfs dfsadmin" instead.

Configured Capacity: 79401328640 (73.95 GB)
Present Capacity: 68053057536 (63.38 GB)
DFS Remaining: 68053049344 (63.38 GB)
DFS Used: 8192 (8 KB)
DFS Used%: 0.00%
Replicated Blocks:
	Under replicated blocks: 0
	Blocks with corrupt replicas: 0
	Missing blocks: 0
	Missing blocks (with replication factor 1): 0
	Pending deletion blocks: 0
Erasure Coded Block Groups: 
	Low redundancy block groups: 0
	Block groups with corrupt internal blocks: 0
	Missing block groups: 0
	Pending deletion blocks: 0

-------------------------------------------------
Live datanodes (2):

Name: 192.168.1.11:9866 (slave1)
Hostname: slave1
Decommission Status : Normal
Configured Capacity: 39700664320 (36.97 GB)
DFS Used: 4096 (4 KB)
Non DFS Used: 5674192896 (5.28 GB)
DFS Remaining: 34026467328 (31.69 GB)
DFS Used%: 0.00%
DFS Remaining%: 85.71%
Configured Cache Capacity: 0 (0 B)
Cache Used: 0 (0 B)
Cache Remaining: 0 (0 B)
Cache Used%: 100.00%
Cache Remaining%: 0.00%
Xceivers: 1
Last contact: Thu Jan 03 02:14:20 EST 2019
Last Block Report: Thu Jan 03 02:13:55 EST 2019
Num of Blocks: 0


Name: 192.168.1.12:9866 (slave2)
Hostname: slave2
Decommission Status : Normal
Configured Capacity: 39700664320 (36.97 GB)
DFS Used: 4096 (4 KB)
Non DFS Used: 5674078208 (5.28 GB)
DFS Remaining: 34026582016 (31.69 GB)
DFS Used%: 0.00%
DFS Remaining%: 85.71%
Configured Cache Capacity: 0 (0 B)
Cache Used: 0 (0 B)
Cache Remaining: 0 (0 B)
Cache Used%: 100.00%
Cache Remaining%: 0.00%
Xceivers: 1
Last contact: Thu Jan 03 02:14:20 EST 2019
Last Block Report: Thu Jan 03 02:13:55 EST 2019
Num of Blocks: 0


[root@master sbin]#
```

可见已分配容量。 

## 查看集群状态

[http://192.168.1.10:8088/cluster/nodes](http://192.168.1.10:8088/cluster/nodes)

![](https://img-blog.csdnimg.cn/20190103152753875.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

[http://192.168.1.10:50070/dfshealth.html#tab-overview](http://192.168.1.10:50070/dfshealth.html#tab-overview)

![](https://img-blog.csdnimg.cn/20190103154035209.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

Hadoop集群安装实例：[https://blog.csdn.net/boonya/article/details/80719245](https://blog.csdn.net/boonya/article/details/80719245)



