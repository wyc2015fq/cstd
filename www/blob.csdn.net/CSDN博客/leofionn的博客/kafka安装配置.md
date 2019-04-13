
# kafka安装配置 - leofionn的博客 - CSDN博客


2018年04月29日 21:37:22[leofionn](https://me.csdn.net/qq_36142114)阅读数：27个人分类：[kafka																](https://blog.csdn.net/qq_36142114/article/category/8756916)


一.Zookeeper部署
1.下载解压zookeeper-3.4.6.tar.gz
\# tar -xvf zookeeper-3.4.6.tar.gz
\# mv zookeeper-3.4.6 zookeeper
\# chown -R root:root zookeeper
2.修改配置
\# cd zookeeper/conf
\# cp zoo_sample.cfg zoo.cfg
\# vi zoo.cfg
dataDir=/usr/local/software/zookeeper/data
server.1=hadoop001:2888:3888
\#mkdir data
\# touch data/myid
\# echo 1 > data/myid
4.启动Zookeeper
\# ./zkServer.sh start
5.查看Zookeeper状态
\# ./zkServer.sh status
6.进入客户端
\# ./zkCli.sh
[zk: localhost:2181(CONNECTED) 0] ls /
[zookeeper]
[zk: localhost:2181(CONNECTED) 1]
[zk: localhost:2181(CONNECTED) 1] help
stat path [watch]
set path data [version]
ls path [watch]
delquota [-n|-b] path
ls2 path [watch]
setAcl path acl
setquota -n|-b val path
history
redo cmdno
printwatches on|off
delete path [version]
sync path
listquota path
rmr path
get path [watch]
create [-s] [-e] path data acl
addauth scheme auth
quit
getAcl path
close
connect host:port
二.Kafka部署
1.下载基于Scala 2.11的kafka版本为0.10.0.1
2.创建logs目录和修改server.properties
[root@hadoop001 software]\# cd kafka
[root@hadoop001 kafka]\# mkdir logs
[root@hadoop001 kafka]\# cd config/
[root@hadoop001 config]\# vi server.properties
broker.id=1
port=9092
host.name=(自己机器id)
log.dirs=/opt/software/kafka/logs
zookeeper.connect=(自己机器id):2181/kafka
4.环境变量
[root@hadoop001 config]\# vi /etc/profile
export KAFKA_HOME=/opt/software/kafka
export PATH=$KAFKA_HOME/bin:$PATH
[root@hadoop001 config]\# source /etc/profile
5.启动/停止
\# nohup kafka-server-start.sh../config/server.properties&
\#这里有坑，../不加，会导致kafka起不了。
\#\#\#停止
bin/kafka-server-stop.sh
\#\#\#停止
bin/kafka-server-stop.sh
【来自@若泽大数据】

