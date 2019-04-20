# kafka管理神器-kafkamanager - Spark高级玩法 - CSDN博客
2018年09月11日 00:02:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：419
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640)
好久没发过kafka的文章了，今天整理一下kafka-manager神器。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640)
**java环境要求**
jdk8
**kafkamanager源码下载**
https://github.com/yahoo/kafka-manager/releases
**编译**
先修改sbt源，否则会报错，而且非常慢
在～/.sbt目录下增加一个文件repositories，内容如下：
[repositories]
local
osc: http://maven.aliyun.com/nexus/content/repositories/central/
oschina-ivy:http://maven.aliyun.com/nexus/content/repositories/central/ , [organization]/[module]/(scala_[scalaVersion]/)(sbt_[sbtVersion]/)[revision]/[type]s/[artifact](-[classifier]).[ext]
ib: http://maven.ibiblio.org/maven2/
ib-ivy: http://maven.ibiblio.org/maven2/ , [organization]/[module]/(scala_[scalaVersion]/)(sbt_[sbtVersion]/)[revision]/[type]s/[artifact](-[classifier]).[ext]
mt-mdh:.sbt meitu$ pwd
/Users/meitu/.sbt
修改源之后编译速度很快而且无故障。
编译是在/opt/hadoop/kafka-manager-1.3.3.18/目录下执行：
./sbt clean dist
要求jdk8假如不是jdk8，可以直接指定路径：
PATH=/usr/local/oracle-java-8/bin:$PATH \
JAVA_HOME=/usr/local/oracle-java-8 \ 
/path/to/sbt -java-home /usr/local/oracle-java-8 clean dist
最后出现
[info] Your package is ready in /opt/hadoop/kafka-manager-1.3.3.18/target/universal/kafka-manager-1.3.3.18.zip
[info]
[success] Total time: 60 s, completed 2018-9-10 13:58:04
代表编译成功，zip生成目录是在
kafka-manager-1.3.3.18/target/universal/kafka-manager-1.3.3.18.zip
在配置conf/application.conf 中加入zookeeper的地址，不一定是kafka的集群zk地址
kafka-manager.zkhosts="localhost:2181"
启动，不加-Dhttp.port参数默认是9000
bin/kafka-manager -Dconfig.file=/path/to/application.conf -Dhttp.port=8080
加入java不是jdk8的话也可以单独指定jdk为jdk8
bin/kafka-manager -java-home /usr/local/oracle-java-8
**kafkamanager的经典作用**
1. 管理多集群
2. 便捷的监控集群状态(topics，consumers，offsets，brokers，副本分布，分区分布)
3. 运行优先副本选举
4. 可以选择指定的broker去产生副本分布策略。
5. 运行分区分布策略，根据4产生的分布策略
6. 根据自定义配置创建topic
7. 删除topic，但是前提是配置kafka的delete.topic.enable=true
8. 可以选择多个topic，然后指定特定的brokers，生成分区分布策略
9. 批量运行8生成的策略
10. 增加已存在topic的分区
11. 更新已存在topic的配置
12. 加入kafka开启jmx的话，可以监控broker和topic级别的监控指标。
**kafka manager图解说明**
**首先创建集群**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWTGs2P7PXCE5Zjk8rLl0B22KNvsEvNzKYzTNtmBOUvBsLKFODadYNqXnCwrCZfMSu2ERkssvkvvA/640)
**接着在topic管理界面**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWTGs2P7PXCE5Zjk8rLl0B2sVHytwiae3uV1lmNG1z4tFrD0nrPxibBLvkqN6tg5MjYRMfEot2SJ6dw/640)
**然后就是消费者界面**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWTGs2P7PXCE5Zjk8rLl0B2libDIxFsHJOYH1x9GibF1wjiapI9eia9osOpxf0wJYsgTq8ibXbX0MgIwqg/640)
主要关注的是图上所属指标，建议大家使用测试，当然非常建议按照本文操作一遍，但是不想编译或者编译不成功也可以，在公众号恢复 **1024** 获取浪院长编译好的。
kafka 开启jmx的方式很简单在kafka-server-start.sh中修改如下
if [ "x$KAFKA_HEAP_OPTS" = "x" ]; then
    export KAFKA_HEAP_OPTS="-Xmx1G -Xms1G"
    export JMX_PORT="9999"
fi
当然，在这里强调一下，对于kafka集群，虽然不耗内存，也建议将kafka的堆内存设置稍微大些。当然，对于很多公司，数据量不足以触发kafka节点内存不够导致的故障。
**推荐阅读：**
[消息队列服务Kafka揭秘：痛点、优势以及适用场景](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485214&idx=1&sn=343ce3ed2fff8ea6b2c4a74018a093c4&chksm=9f38e436a84f6d2026217420caf0ce71713f1524eb416d4b7dde9a859a4919fd22ffcbccdef8&scene=21#wechat_redirect)
[关于Spark Streaming感知kafka动态分区的问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485078&idx=1&sn=5be1f2db95f2853651d8fe06cb78d637&chksm=9f38e5bea84f6ca8c9db462cbc1d85e75f0f2b72ec976e0af68cf1435a102261d8849bf96b5c&scene=21#wechat_redirect)
[Kafka源码系列之源码分析zookeeper在kafka的作用](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484900&idx=1&sn=95577024ba531e6743d06e8c550662a3&chksm=9f38e6cca84f6fda5a37dd6a4f8d858a108a5bbfb802eeb4ed24f1a152a5f60b600e0fca125d&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
