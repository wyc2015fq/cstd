# timetunnel搭建 - zj360202的专栏 - CSDN博客





2012年12月12日 15:58:03[zj360202](https://me.csdn.net/zj360202)阅读数：1483








timetunnel终于搭建成功了

timetunnel 目录结构：

--------------1.apps

                   ------------开发中用的，部署过程中这个文件夹下面可以没有内容

--------------2.broker

                   ------------2.1 bin     保存执行脚本的          一般在官网 \timetunnel\broker\target\dist 下面

                                   ---------2.1.1 broker-start.sh 在这个执行过程中，有一行执行代码要关注一下

                                   ---------2.1.2 nohup.out 运行信息

                                   ---------2.1.3 其他.sh  到官网上面下载，启动的时候不要做其他改动

                   ------------2.2 conf

                                   ---------2.2.1 conf.properties 这个比较重要

                                   ---------2.2.2 log4j.properties 默认不要动

                                   ---------2.2.3 zknodes 测试数据

                   -------------2.3 lib

                                   ---------  activation.jar, btrace-agent.jar, btrace-boot.jar, btrace-client.jar, commons-codec.jar, gson.jar,jline.jar,jms.jar,libthrift.jar,log4j.jar,mail.jar

                                   --------- slf4j-api.jar,slf4j-log4j12.jar, timetunnel-broker.jar, timetunnel-idl.jar, timetunnel-testutils.jar, zookeeper.jar

--------------3 client

                  -------------3.1 lib

                                  --------- gson-1.4.jar,libthrift-0.5.0-20100116.jar, log4j-1.2.16.jar, slf4j-api-1.6.1.jar, slf4j-log4j12-1.6.1.jar, timetunnel-client-0.2.0-SNAPSHOT.jar,timetunnel-idl-0.2.0-SNAPSHOT.jar

--------------4 data

                  ------------ 4.1 version-2    zookeeper生成的目录

--------------5 log

                  -------------5.1 systemlogs 和部署没有关系

--------------6 router

                  -------------6.1 bin 

                                   --------6.1.1 router-start.sh

                                   --------6.1.2 nohup.out

                                   --------6.1.3 其他.sh

                  -------------6.2 conf

                                   -------- 6.2.1 router.properties 比较重要

                                   -------- 6.2.2 log4j.properties 默认不变

                  -------------6.3 lib 

                                   -------- commons-lang-2.5.jar, gson-1.4.jar, libthrift-0.5.0.jar, log4j-1.2.16.jar, slf4j-api-1.6.1.jar, slf4j-log4j12-1.6.1.jar, timetunnel2-cluster-0.1.0.jar, timetunnel2-routersrv-0.1.0.jar, timetunnel-idl-0.2.0-SNAPSHOT.jar,
 zk-recipes.jar, zookeeper-3.2.2.jar

                  -------------6.4 log 里面是router运行日志

--------------7 tailfile 测试目录，部署期间不起作用

--------------8 zookeeper 保存zookeeper运行日志的



2.1.1 broker-start.sh

```java
#!/bin/bash

SCRIPT_NAME=$0;
BIN_DIR=`dirname ${SCRIPT_NAME}`;
BASE_DIR="${BIN_DIR}/..";
BASE_LIB=${BASE_DIR}/lib;
BASE_CONF=${BASE_DIR}/conf;
BASE_LOG=${BASE_DIR}/log;

PID_FILE=${BASE_CONF}/.timetunnel.broker.pid;

if [ -f $PID_FILE ];
then
        old_pid=`cat $PID_FILE`;
        pids=`ps aux | grep java | awk '{print $2;}'`;
        for pid in $pids
        do
                if [ $pid -eq $old_pid ];
                then
                        echo "process is running as $pid,please stop it first.";
                        exit 0;
                fi
        done
fi

# VM Options : -XX:NewRatio=1 -XX:+PrintGCTimeStamps -XX:+PrintGCDetails -XX:MaxTenuringThreshold=15
script="java -XX:SurvivorRatio=5 -XX:-UseAdaptiveSizePolicy -XX:NewRatio=1 -XX:+PrintGCTimeStamps -XX:+PrintGCDetails -XX:HeapDumpPath=${BASE_LOG}/broker.hprof -XX:ErrorFile=${BASE_LOG}/crash.log -XX:+PrintCommandLineFlags -Xmx512m -Xms512m -XX:+HeapDumpOnOutOfMemoryError -Dtt.log.file=${BASE_LOG}/broker.log -Dlog4j.configuration=file:${BASE_CONF}/log4j.properties -classpath ${BASE_LIB}/*;${BASE_LIB}/btrace-agent.jar com.taobao.timetunnel.bootstrap.BrokerBootstrap ${BASE_CONF}/conf.properties";
echo $script
nohup $script &
pid=$!
echo $pid > $PID_FILE;
```

就是在script=后面的字符串中是一个java 执行过程，其中-classpath的参数后面跟着classpath路径，会覆盖环境变量中的classpath路径，语法是

java -classpath dir(class文件目录);(用分号隔开)jardir(jar包的路径);zipdir(zip包路径);后面类所在路径(如果在前面的路径中已经存在就不单独写了，最后一个不用分号，用空格)  com.taobao.timetunnel.bootstrap.BrokerBootstrap(这个是class的路径，路径到包名前面，如果不在jar,zip中，com.taobao=com/taobao 这个要注意了)，如果很多包在一个目录下面可以用/lib/*,而执行中报这个包中的某个找不到可以单独写一下，/lib/*;/lib/btrace-agent.jar
 就好了   在最后面一般是跟这个类中执行要引用的配置文件

2.1.2 nohub.out

![](https://img-my.csdn.net/uploads/201212/12/1355297829_6680.jpg)



一般运行信息都在这个文件中，这个文件是在执行脚本的同一层文件夹中存在，其他的也是，如果报错了，就到这个地方看明原因。

2.2.1 conf.properties

```java
############################ 以下是启动必选配置项 ############################ 

# broker的home路径, 默认为当前目录, broker产生持久化数据会存在这个目录下
broker.home = /usr/local/timetunnel/broker/target
# broker组, 分组以便分级服务.
broker.group = group1

# 设置broker对外服务(订阅发布)的端口
external.port = 9999

# 设置broker对内服务(数据可靠同步)的端口, 不设置则broker依旧可以单点运行 
internal.port = 9998

# zookeeper是broker集群的协调者, 必须配置器连接地址和端口, eg : 192.168.1.132:33561,192.168.1.133:33561/root
zookeeper.connectString =172.16.2.240:2181
# 连接zookeeper会话超时时间, 单位毫秒, 它应该是zookeeper服务器配置的tickTime的2~20倍 
zookeeper.sessionTimeout = 10000

# broker的主机名, 也可以是ip, 系统会自动取, 一般无需配置
# broker.host = localhost
broker.host = 172.16.2.240

############################ 以上是启动必选配置项 ############################ 


############################ 以下是调优可选配置项 ############################

# 一个订阅通常一组client来获取, 为保证每个client获取的数量近乎平均, 就需要在累计一定数量时进行同步, 这个同步点默认为200 
 group.syncPoint = 20000

# 为避免内存溢出, broker会根据内存的消耗情况, 将部分数据交换到磁盘文件中, 内存中保存其数据的索引
# 当内存短缺时开始交换, 直到内存充裕, 下面是分别配置短缺和充裕的内存比例值, 最大为99, 最小为1.
# 内存短缺比例默认为45
# memory.shortageRatio = 45
# 内存充裕比例默认为75
# memory.abundantRatio = 75

# 设置broker中thrift server服务的线程数, 默认是5, 可以根据应用要求适当调整
# broker.workThread = 5
# 设置broker中thrift server最大读取内存数, 默认为2的31次方, 一般情况下无需修改 
# broker.maxReadBufferBytes = 
# 设置broker中单个消息的最大值, 默认为4K, 单位可以是K或M
 broker.maxMessageSize = 1M

# chunk.capacity = 64M 
# 内存交换磁盘的文件缓存, 默认32K, 单位可以是K或M
# chunk.buffer = 32K

 
####################:x######## 以上是调优可选配置项 ############################
```



上面172.16.2.240要改成自己的，尽量不要用localhost

其他配置文件也注意是改IP



