# Docker制作数据库中间件Mycat源码镜像 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月30日 10:11:47[boonya](https://me.csdn.net/boonya)阅读数：2610








经过修改的Mycat已经比较符合使用了，为了在使用过程中尽量减少不必要的错误操作，我们打算将其制作成docker镜像。制作docker需要一个稳定的基础环境，这里以boot2docker为基础镜像使用Dokcerfile的方式进行构建。

## 环境原料准备

![](https://img-blog.csdn.net/20170830110314479)

注：只需要准备下面用到的就可以了。


## Dokcerfile配置



```
FROM boot2docker/boot2docker:latest
MAINTAINER boonya@163.com
ADD jdk-8u121-linux-x64.tar.gz /usr/local/
ADD mycat  /usr/local/mycat
ENV JAVA_HOME /usr/local/jdk1.8.0_121
ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
ENV PATH $PATH:$JAVA_HOME/bin
EXPOSE 8066 9066
RUN chmod -R 777 /usr/local/mycat/bin
CMD ["./usr/local/mycat/bin/mycat", "console"]
```
注：必须安装JDK，mycat在java环境中运行。



## Dockerfile构建

构建命令：docker build -t boonyadocker/boot2docker-mycat:latest .


构建日志：



```
[root@mysqlserver java-maven-tomcat-jenkins]# docker build -t boonyadocker/boot2docker-mycat:latest .
Sending build context to Docker daemon 308.2 MB
Sending build context to Docker daemon
Step 0 : FROM boot2docker/boot2docker:latest
 ---> 898cd2a4a97e
Step 1 : MAINTAINER boonya@163.com
 ---> Running in fc976d113c14
 ---> 4c15c54bace0
Removing intermediate container fc976d113c14
Step 2 : ADD jdk-8u121-linux-x64.tar.gz /usr/local/
 ---> c2a68d90a7e8
Removing intermediate container 1dabd2085544
Step 3 : ADD mycat /usr/local/mycat
 ---> ebf0de05e8ec
Removing intermediate container 66eda0fb1dca
Step 4 : ENV JAVA_HOME /usr/local/jdk1.8.0_121
 ---> Running in ab924224a988
 ---> bcbb94be23e1
Removing intermediate container ab924224a988
Step 5 : ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
 ---> Running in f84d0bafc488
 ---> fbe32cae8f24
Removing intermediate container f84d0bafc488
Step 6 : ENV PATH $PATH:$JAVA_HOME/bin
 ---> Running in aec65e85bdcb
 ---> 9684515e4b07
Removing intermediate container aec65e85bdcb
Step 7 : EXPOSE 8066 9066
 ---> Running in 2478366fb94e
 ---> b78e089eacb5
Removing intermediate container 2478366fb94e
Step 8 : RUN chmod -R 777 /usr/local/mycat/bin
 ---> Running in fc6c52bf492b
 ---> d9730028fa48
Removing intermediate container fc6c52bf492b
Step 9 : CMD ./usr/local/mycat/bin/mycat console
 ---> Running in bd7431de7e76
 ---> cf46ef25daf5
Removing intermediate container bd7431de7e76
Successfully built cf46ef25daf5
```



镜像列表：



```
[root@mysqlserver java-maven-tomcat-jenkins]# docker images
REPOSITORY                         TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
boonyadocker/boot2docker-mycat     latest              cf46ef25daf5        20 seconds ago      2.607 GB
boonyadocker/boot2docker-tomcat    latest              31ad702ceffa        12 days ago         2.602 GB
boonyadocker/boot2docker-jenkins   latest              a0090abfd7c8        12 days ago         2.685 GB
boonyadocker/centos-jenkins        latest              1250a4fe0292        12 days ago         659 MB
boonyadocker/centos-jenkins        allow-remote        47e38ec3f8bc        2 weeks ago         658.7 MB
boonyadocker/centos-tomcat         allow-remote        a3cf71b7dd49        2 weeks ago         575.9 MB
tomcat                             latest              70b2ded797df        2 weeks ago         292.4 MB
boot2docker/boot2docker            latest              898cd2a4a97e        2 weeks ago         2.219 GB
centos                             latest              f3b88ddaed16        3 weeks ago         192.5 MB
jenkins                            latest              de57ef56dca0        4 weeks ago         809.9 MB
openjdk                            8-jre               cdcc2156bae7        4 weeks ago         275.1 MB
jetbrains/teamcity-server          latest              47728ea3474b        5 weeks ago         1.308 GB
boonyadocker/jenkins               latest              e14944ad9023        6 weeks ago         654.2 MB
boonyadocker/tomcat-allow-remote   latest              96d1fe00ad03        7 weeks ago         571.4 MB
[root@mysqlserver java-maven-tomcat-jenkins]#
```

注：以上已成功构建。



## 在容器中运行镜像

运行命令：docker run --name mycat  -p 8066:8066 -p 9066:9066 boonyadocker/boot2docker-mycat:latest 


运行日志：



```
Running Mycat-server...
wrapper  | --> Wrapper Started as Console
wrapper  | Launching a JVM...
jvm 1    | Java HotSpot(TM) 64-Bit Server VM warning: ignoring option MaxPermSize=64M; support was removed in 8.0
jvm 1    | Wrapper (Version 3.2.3) http://wrapper.tanukisoftware.org
jvm 1    |   Copyright 1999-2006 Tanuki Software, Inc.  All Rights Reserved.
jvm 1    |
jvm 1    | 2017-08-30 01:48:01,258 [INFO ][WrapperSimpleAppMain] total resouces of dataHost localhost1 is :3  (io.mycat.backend.datasource.PhysicalDBPool:PhysicalDBPool.java:100)
jvm 1    | 2017-08-30 01:48:01,265 [INFO ][WrapperSimpleAppMain] create layer cache pool TableID2DataNodeCache of type encache ,default cache size 10000 ,default expire seconds18000  (io.mycat.cache.CacheService:CacheService.java:125)
jvm 1    | 2017-08-30 01:48:01,266 [INFO ][WrapperSimpleAppMain] create child Cache: TESTDB_ORDERS for layered cache TableID2DataNodeCache, size 50000, expire seconds 18000  (io.mycat.cache.DefaultLayedCachePool:DefaultLayedCachePool.java:80)
jvm 1    | 2017-08-30 01:48:01,391 [INFO ][WrapperSimpleAppMain] dyna class load from ./catlet,and auto check for class file modified every 60 seconds  (io.mycat.config.classloader.DynaClassLoader:DynaClassLoader.java:34)
jvm 1    | 2017-08-30 01:48:01,391 [INFO ][WrapperSimpleAppMain] ===============================================  (io.mycat.MycatServer:MycatServer.java:258)
jvm 1    | 2017-08-30 01:48:01,391 [INFO ][WrapperSimpleAppMain] MyCat is ready to startup ...  (io.mycat.MycatServer:MycatServer.java:259)
jvm 1    | 2017-08-30 01:48:01,392 [INFO ][WrapperSimpleAppMain] Startup processors ...,total processors:8,aio thread pool size:16    
jvm 1    |  each process allocated socket buffer pool  bytes ,a page size:2097152  a page's chunk number(PageSize/ChunkSize) is:512  buffer page's number is:160  (io.mycat.MycatServer:MycatServer.java:271)
jvm 1    | 2017-08-30 01:48:01,392 [INFO ][WrapperSimpleAppMain] sysconfig params:SystemConfig [processorBufferLocalPercent=100, frontSocketSoRcvbuf=1048576, frontSocketSoSndbuf=4194304, backSocketSoRcvbuf=4194304, backSocketSoSndbuf=1048576, frontSocketNoDelay=1, backSocketNoDelay=1, maxStringLiteralLength=65535, frontWriteQueueSize=2048, bindIp=0.0.0.0, serverPort=8066, managerPort=9066, charset=utf8, processors=8, processorExecutor=16, timerExecutor=2, managerExecutor=2, idleTimeout=1800000, catletClassCheckSeconds=60, sqlExecuteTimeout=300, processorCheckPeriod=1000, dataNodeIdleCheckPeriod=300000, dataNodeHeartbeatPeriod=10000, clusterHeartbeatUser=_HEARTBEAT_USER_, clusterHeartbeatPass=_HEARTBEAT_PASS_, clusterHeartbeatPeriod=5000, clusterHeartbeatTimeout=10000, clusterHeartbeatRetry=10, txIsolation=3, parserCommentVersion=50148, sqlRecordCount=10, bufferPoolPageSize=2097152, bufferPoolChunkSize=4096, bufferPoolPageNumber=160, maxResultSet=524288, bigResultSizeSqlCount=10, bufferUsagePercent=80, flowControlRejectStrategy=0, clearBigSqLResultSetMapMs=600000, defaultMaxLimit=100, sequnceHandlerType=2, sqlInterceptor=io.mycat.server.interceptor.impl.DefaultSqlInterceptor, sqlInterceptorType=select, sqlInterceptorFile=/usr/local/mycat/logs/sql.txt, mutiNodeLimitType=0, mutiNodePatchSize=100, defaultSqlParser=druidparser, usingAIO=0, packetHeaderSize=4, maxPacketSize=16777216, mycatNodeId=1]  (io.mycat.MycatServer:MycatServer.java:272)
jvm 1    | log4j:WARN No appenders could be found for logger (io.mycat.memory.MyCatMemory).
jvm 1    | log4j:WARN Please initialize the log4j system properly.
jvm 1    | log4j:WARN See http://logging.apache.org/log4j/1.2/faq.html#noconfig for more info.
jvm 1    | 2017-08-30 01:48:01,612 [INFO ][WrapperSimpleAppMain] using nio network handler   (io.mycat.MycatServer:MycatServer.java:373)
jvm 1    | 2017-08-30 01:48:01,624 [INFO ][WrapperSimpleAppMain] $_MyCatManager is started and listening on 9066  (io.mycat.MycatServer:MycatServer.java:389)
jvm 1    | 2017-08-30 01:48:01,624 [INFO ][WrapperSimpleAppMain] $_MyCatServer is started and listening on 8066  (io.mycat.MycatServer:MycatServer.java:393)
jvm 1    | 2017-08-30 01:48:01,624 [INFO ][WrapperSimpleAppMain] ===============================================  (io.mycat.MycatServer:MycatServer.java:395)
jvm 1    | 2017-08-30 01:48:01,624 [INFO ][WrapperSimpleAppMain] Initialize dataHost ...  (io.mycat.MycatServer:MycatServer.java:399)
jvm 1    | 2017-08-30 01:48:01,624 [INFO ][WrapperSimpleAppMain] init backend myqsl source ,create connections total 10 for hostM1 index :0  (io.mycat.backend.datasource.PhysicalDBPool:PhysicalDBPool.java:294)
jvm 1    | 2017-08-30 01:48:01,636 [INFO ][WrapperSimpleAppMain] no ilde connection in pool,create new connection for hostM1 of schema db3  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413)
jvm 1    | 2017-08-30 01:48:01,636 [INFO ][WrapperSimpleAppMain] no ilde connection in pool,create new connection for hostM1 of schema db2  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413)
jvm 1    | 2017-08-30 01:48:01,637 [INFO ][WrapperSimpleAppMain] no ilde connection in pool,create new connection for hostM1 of schema db1  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413)
jvm 1    | 2017-08-30 01:48:01,637 [INFO ][WrapperSimpleAppMain] no ilde connection in pool,create new connection for hostM1 of schema db3  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413)
jvm 1    | 2017-08-30 01:48:01,638 [INFO ][WrapperSimpleAppMain] no ilde connection in pool,create new connection for hostM1 of schema db2  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413)
..............
```

查看正在运行的容器：




```
[root@mysqlserver ~]# docker ps
CONTAINER ID        IMAGE                                     COMMAND                CREATED             STATUS              PORTS                                            NAMES
34e3ef01f9e4        boonyadocker/boot2docker-mycat:latest     "./usr/local/mycat/b   10 minutes ago      Up 10 minutes       0.0.0.0:8066->8066/tcp, 0.0.0.0:9066->9066/tcp   mycat                          
2b3f9dca2c40        boonyadocker/boot2docker-jenkins:latest   "/bin/sh -c '/usr/lo   12 days ago         Up 9 days           0.0.0.0:8888->8080/tcp                           boot2docker-jenkins-20170817   
9483cc274809        boonyadocker/boot2docker-tomcat:latest    "/bin/sh -c '/usr/lo   12 days ago         Up 9 days           0.0.0.0:8080->8080/tcp                           boot2docker-tomcat-20170817    
[root@mysqlserver ~]#
```

由此可见，mycat镜像制作完成。注意：docker依赖的基础镜像需要斟酌，毕竟boot2docker构建后的镜像太过于庞大，可以试试直接根据centos或者linux基础镜像进行构建。

## 测试mycat连接

首先我们看一下分配给mycat运行容器的IP地址，命令：service iptables status



```
Table: nat
Chain PREROUTING (policy ACCEPT)
num  target     prot opt source               destination         
1    DOCKER     all  --  0.0.0.0/0            0.0.0.0/0           ADDRTYPE match dst-type LOCAL 

Chain POSTROUTING (policy ACCEPT)
num  target     prot opt source               destination         
1    SNAT       all  --  172.17.0.8           172.17.0.8          to:10.10.x.x
2    SNAT       all  --  172.17.0.8           0.0.0.0/0           to:10.10.x.x
3    SNAT       all  --  172.18.0.8           0.0.0.0/0           to:10.10.x.x
4    MASQUERADE  all  --  172.17.0.0/16        0.0.0.0/0           
5    MASQUERADE  tcp  --  172.17.0.8           172.17.0.8          tcp dpt:8080 
6    MASQUERADE  tcp  --  172.17.0.9           172.17.0.9          tcp dpt:8080 
7    MASQUERADE  tcp  --  172.17.0.28          172.17.0.28         tcp dpt:9066 
8    MASQUERADE  tcp  --  172.17.0.28          172.17.0.28         tcp dpt:8066 

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    DOCKER     all  --  0.0.0.0/0           !127.0.0.0/8         ADDRTYPE match dst-type LOCAL 

Chain DOCKER (2 references)
num  target     prot opt source               destination         
1    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:8080 to:172.17.0.8:8080 
2    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:8888 to:172.17.0.9:8080 
3    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:9066 to:172.17.0.28:9066 
4    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:8066 to:172.17.0.28:8066 

Table: filter
Chain INPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:10050 state NEW,ESTABLISHED 
2    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:21 state NEW,ESTABLISHED 
3    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:3306 state NEW,ESTABLISHED 
4    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:3036 
5    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           state RELATED,ESTABLISHED 
6    ACCEPT     icmp --  0.0.0.0/0            0.0.0.0/0           
7    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           
8    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           state NEW tcp dpt:22 
9    REJECT     all  --  0.0.0.0/0            0.0.0.0/0           reject-with icmp-host-prohibited 

Chain FORWARD (policy ACCEPT)
num  target     prot opt source               destination         
1    DOCKER     all  --  0.0.0.0/0            0.0.0.0/0           
2    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           ctstate RELATED,ESTABLISHED 
3    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           
4    REJECT     all  --  0.0.0.0/0            0.0.0.0/0           reject-with icmp-host-prohibited 
5    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp spt:10050 state ESTABLISHED 
2    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp spt:3306 state ESTABLISHED 

Chain DOCKER (1 references)
num  target     prot opt source               destination         
1    ACCEPT     tcp  --  0.0.0.0/0            172.17.0.8          tcp dpt:8080 
2    ACCEPT     tcp  --  0.0.0.0/0            172.17.0.9          tcp dpt:8080 
3    ACCEPT     tcp  --  0.0.0.0/0            172.17.0.28         tcp dpt:9066 
4    ACCEPT     tcp  --  0.0.0.0/0            172.17.0.28         tcp dpt:8066 

[root@mysqlserver java-maven-tomcat-jenkins]#
```
然后我们尝试使用分配的8066端口的ip 172.17.0.28 去连：


![](https://img-blog.csdn.net/20170830102855010)

发现外部是不能直接访问容器的，我们从宿主机IP去访问：

![](https://img-blog.csdn.net/20170830103243180)


这样就访问成功了，密码可以进入容器进行查看：在/usr/local/mycat/conf/server.xml中有定义。一睹mycat数据库的芳容：

![](https://img-blog.csdn.net/20170830104027607)


为了省去翻箱倒柜的步骤，这里还是说下用户和密码：root/digdeep 或 user/user 。

## 提供公共仓库镜像

下载命令：docker pull boonyadocker/boot2docker-mycat

![](https://img-blog.csdn.net/20170830105124495)




