# hive 创建表和导入数据 - zj360202的专栏 - CSDN博客





2013年02月05日 14:44:23[zj360202](https://me.csdn.net/zj360202)阅读数：2718








1.创建表

```
CREATE TABLE disp_log_data ( 
ip STRING,
time DOUBLE,
title STRING,
url STRING,
hostname STRING,
referrer STRING,
uvid STRING,
isnewvisitor int,
scr STRING,
spm_pre STRING,
spm_url STRING,
spm_cnt STRING,
islogin int,
userid int,
categoryid int,
brandid int,
adid STRING,
shopid int,
itemid int) 
ROW FORMAT DELIMITED 
FIELDS TERMINATED BY '\001'
STORED AS TEXTFILE;
```

创建表的时候如果报下面的错表示 创建表的时候 用户不对，要切换用户，不一定root用户就能操作

```java
FAILED: Error in metadata: MetaException(message:Got exception: org.apache.hadoop.security.AccessControlException Permission denied: user=root, access=WRITE, inode="/user/hive/warehouse":hdfs:supergroup:drwxr-xr-x
        at org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.check(FSPermissionChecker.java:205)
        at org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.check(FSPermissionChecker.java:186)
        at org.apache.hadoop.hdfs.server.namenode.FSPermissionChecker.checkPermission(FSPermissionChecker.java:135)
        at org.apache.hadoop.hdfs.server.namenode.FSNamesystem.checkPermission(FSNamesystem.java:4547)
        at org.apache.hadoop.hdfs.server.namenode.FSNamesystem.checkAncestorAccess(FSNamesystem.java:4518)
        at org.apache.hadoop.hdfs.server.namenode.FSNamesystem.mkdirsInternal(FSNamesystem.java:2880)
        at org.apache.hadoop.hdfs.server.namenode.FSNamesystem.mkdirsInt(FSNamesystem.java:2844)
        at org.apache.hadoop.hdfs.server.namenode.FSNamesystem.mkdirs(FSNamesystem.java:2823)
        at org.apache.hadoop.hdfs.server.namenode.NameNodeRpcServer.mkdirs(NameNodeRpcServer.java:639)
        at org.apache.hadoop.hdfs.protocolPB.ClientNamenodeProtocolServerSideTranslatorPB.mkdirs(ClientNamenodeProtocolServerSideTranslatorPB.java:417)
        at org.apache.hadoop.hdfs.protocol.proto.ClientNamenodeProtocolProtos$ClientNamenodeProtocol$2.callBlockingMethod(ClientNamenodeProtocolProtos.java:44096)
        at org.apache.hadoop.ipc.ProtobufRpcEngine$Server$ProtoBufRpcInvoker.call(ProtobufRpcEngine.java:453)
        at org.apache.hadoop.ipc.RPC$Server.call(RPC.java:898)
        at org.apache.hadoop.ipc.Server$Handler$1.run(Server.java:1693)
        at org.apache.hadoop.ipc.Server$Handler$1.run(Server.java:1689)
        at java.security.AccessController.doPrivileged(Native Method)
        at javax.security.auth.Subject.doAs(Subject.java:415)
        at org.apache.hadoop.security.UserGroupInformation.doAs(UserGroupInformation.java:1332)
        at org.apache.hadoop.ipc.Server$Handler.run(Server.java:1687)
)
FAILED: Execution Error, return code 1 from org.apache.hadoop.hive.ql.exec.DDLTask
```



2.插入数据
`LOAD DATA LOCAL INPATH 'disp_20130204_14_disp1.log' OVERWRITE INTO TABLE disp_log_data;`
这一句是插入本地文件路径，这条语句表明'disp_20130204_14_disp1.log'这个文件在你执行hive进入hive>当时的路径，当然还是要关心权限问题，当前用户有很多地方不能访问，要将问题置于当前用户可以访问的一个路径下面

`LOAD DATA INPATH 'disp_20130204_14_disp1.log' OVERWRITE INTO TABLE disp_log_data;`
这一句是导入hdfs的文件路径

```java
FAILED: Error in semantic analysis: Line 1:18 Invalid path ''disp_20130204_14_disp1.log'': No files matching path hdfs://nameservice1/user/hdfs/disp_20130204_14_disp1.log
```

执行完后会自动转化到hdfs://nameservice1/user/hdfs/disp_20130204_14_disp1.log





