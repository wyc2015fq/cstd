
# 阿里云上kafka的安装和配置 - leofionn的博客 - CSDN博客


2019年03月14日 15:46:48[leofionn](https://me.csdn.net/qq_36142114)阅读数：89


下载kafka的地址：
https://www.apache.org/dyn/closer.cgi?path=/kafka/1.1.0/kafka_2.11-1.1.0.tgz
镜像下载kafka
wget http://mirrors.hust.edu.cn/apache/kafka/1.1.0/kafka_2.11-1.1.0.tgz
然后解压
tar -zxvf kafka_2.11-1.1.0.tgz
进入到kafka目录下：
kafka是基于zookeeper的，但是，kafka是自带zookeeper；然后进入到/bin目录下
启动
看一下启动情况
vim nohup.out
启动kafka
./bin/kafka-server-start.sh config/server.properties
启动报错：
Java HotSpot(TM) Server VM warning: INFO: os::commit_memory(0xa6e00000, 1073741824, 0) failed; error='Cannot allocate memory' (errno=12)
处理方案：
修改启动文件
修改 kafka-server-start.sh 将export KAFKA_HEAP_OPTS="-Xmx1G -Xms1G"
改为export KAFKA_HEAP_OPTS="-Xmx256M -Xms128M"
继续启动报错：
Caused by: java.net.UnknownHostException: iZuf68tztea6l5ccdz7wemZ: iZuf68tztea6l5ccdz7wemZ: Name or service not known
处理方案：
vi /etc/hosts
127.0.0.1 izwz920j4zsv1qvh2kt0efz
izwz920j4zsv1qvh2kt0efz为自己的主机名
然后启动成功
配置外网访问
vim config/server.properties
\# The id of the broker. This must be set to a unique integer for each broker.
broker.id=0
port=9092
host.name=阿里云内网地址
advertised.host.name=阿里云外网映射地址
后台启动kafka
nohup ./bin/kafka-server-start.sh config/server.properties &

