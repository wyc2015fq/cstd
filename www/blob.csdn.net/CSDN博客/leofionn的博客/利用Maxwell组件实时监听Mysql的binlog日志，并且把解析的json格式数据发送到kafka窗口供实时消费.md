
# 利用Maxwell组件实时监听Mysql的binlog日志，并且把解析的json格式数据发送到kafka窗口供实时消费 - leofionn的博客 - CSDN博客


2018年06月05日 17:41:53[leofionn](https://me.csdn.net/qq_36142114)阅读数：176


## 文档主题：
如何使用Maxwell实时监听Mysql的binlog日志，并且把解析的json格式数据发送到kafka窗口
## 具体步骤
## 一：在linux环境下安装部署好mysql
1 开启binlog
sudo vi /etc/my.cnf
2 mysql的binlog格式有3种，为了把binlog解析成json数据格式，要设置binlog的格式为row（binlog有三种格式：Statement、Row以及Mixed）
server-id=1
log-bin=master（这一步开启binlog）
binlog_format=row
![](https://img-blog.csdn.net/20171023171952315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA5MjE0NjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3重启msyql服务
sudo service mysqld restart
4查看是否已经开启binlog
Mysql>show variables like '%log_bin%';

![](https://img-blog.csdn.net/20171023172858891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA5MjE0NjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


此时，可以在目录/var/lib/mysql下看到生成了相应的binlog监听日志文件，如图，master.000001文件，每次重启msyql服务，就会生成一个新的监听文件

![](https://img-blog.csdn.net/20171023172923855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA5MjE0NjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 第二大步骤：配置Maxwell相关的部署工作
1下载Maxwell
官网
http://maxwells-daemon.io/
组件下载链接
[https://github.com/zendesk/maxwell/releases/download/v1.10.7/maxwell-1.10.7.tar.gz](https://github.com/zendesk/maxwell/releases/download/v1.10.7/maxwell-1.10.7.tar.gz)

2 安装Maxwell
tar-zxf maxwell-1.10.6.tar.gz  -C    ../modules/

3给mysql授权
mysql> GRANT ALL on maxwell.* to'maxwell'@'%' identified by 'XXXXXX';
mysql> GRANT SELECT, REPLICATION CLIENT,REPLICATION SLAVE on *.* to 'maxwell'@'%';
以上图片为官网参考
以我自己的为例：
GRANTALL on *.* to 'user01'@'%' identified by '123456';
把所有数据库的所有表授权给user01用户以密码123456登录
GRANTSELECT, REPLICATION CLIENT, REPLICATION SLAVE on *.* to 'user01'@'%';
flushprivileges;

4开启maxwell命令行（注意，如果没有设置，maxwell默认是把监听的mysql的binlog日志发送到kafka的主题叫maxwell的topic上的）
`bin/maxwell --user='maxwell' --password='XXXXXX' --host='127.0.0.1' \``--producer=kafka --kafka.bootstrap.servers=localhost:9092`
以上为官网参考，我自己的具体的demo
bin/maxwell --user='user01' --password='123456'--host='192.168.136.129' --producer=kafka --kafka.bootstrap.servers=mw:9092解释：host参数是安装mysql的那台主机，最后的kafka.bootstrap.servers是安装kafka集群的节点主机名和端口号

## 第三大步骤，kafka相关配置
说明（以下我的kafka是安装在主机名叫mw，注意kafka里的配置文件端口号要和命令行里给的端口号一致）
1首先启动zookeeper
$sbin/zkServer.sh start
2开启kafka命令行
bin/kafka-server-start.shconfig/server.properties
3创建一个主题叫maxwell以便于接受数据
bin/kafka-topics.sh--create --zookeeper mw:2181 --replication-factor 1 --partitions 1 --topic maxwell
4启动生产者窗口
bin/kafka-console-producer.sh--broker-list mw:9092 --topic maxwell
5启动消费者窗口
bin/kafka-console-consumer.sh--zookeeper mw:2181 --topic maxwell --from-beginning
## 第四大步：最终测试
此时，mysql的binlog已经开启，maxwell命令行也开启，kafka的生产者消费者窗口也开启在监听
开始往mysql里插入修改数据
通过写的jdbc写的多线程程序批量插入修改数据的程序造数据

![](https://img-blog.csdn.net/20171023173016575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA5MjE0NjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


此时，查看kafka的消费者窗口。可以看到，mysql插入修改的数据，以json形式被maxwell发送到了kafka以maxwell主题命名的窗口里

![](https://img-blog.csdn.net/20171023173054656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA5MjE0NjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 第五  额外补充
1在第三大步骤里的第四小步开启maxwell命令行时，可以多跟几个参数对mysql的binlog进行过滤，只筛选某些数据库里的某些表
include_dbs，exclude_dbs，include_tables，exclude_tables
2注意点：kafka的安装版本要和maxwell兼容适应，不然会报错
Kafka clients 0.8 and 0.9 are compatible with brokers running kafka 0.8.
0.10.0.x clients only support 0.10.0.x or later brokers.
Mixing Kafka 0.10 with other versions can lead to serious performance impacts.
最好安装kafka0.10版本的吧

