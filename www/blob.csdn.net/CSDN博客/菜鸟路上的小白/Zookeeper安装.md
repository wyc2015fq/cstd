# Zookeeper安装 - 菜鸟路上的小白 - CSDN博客





2016年09月09日 09:33:11[somnus_小凯](https://me.csdn.net/u012486840)阅读数：427
个人分类：[ZooKeeper](https://blog.csdn.net/u012486840/article/category/6409343)








Zookeeper安装
### 1. 安装
wget http://www.apache.org/dist//zookeeper/zookeeper-3.3.6/zookeeper-3.3.6.tar.gztar zxvf zookeeper-3.3.6.tar.gzmv zookeeper-3.3.6 /usr/local/zookeeper-3.3.6-2181cd /usr/local/zookeeper-3.3.6-2181cp conf/zoo_sample.cfg conf/zoo.cfg
### 2. 单实例配置

#### 修改配置文件
vi conf/zoo.cfg#心跳时间tickTime=2000initLimit=10syncLimit=5#数据保存目录dataDir=/data/somnus/logistics/zookeeper-3.3.6#客户端连接端口clientPort=2181
#### 增加内存

zkEnv.sh

zkEnv.sh
#在文件底部增加export JVMFLAGS="-Xms1024m -Xmx2048m $JVMFLAGS"
查询内存大小
ps -ef|grep zookeeper#查询pidjmap --heappid 
### 3. 集群配置

注意：其中data目录和server地址需改成你真实部署机器的信息

#### 修改配置文件
vi conf/zoo.cfgtickTime=2000#连接leader的等待的最长时间(秒)initLimit=10#标识 Leader 与 Follower 之间发送消息，请求和应答时间最长时间syncLimit=5dataDir=/data/somnus/logistics/zookeeper-3.3.6clientPort=2181#server.A=B：C：D：其中 A 是一个数字，表示这个是第几号服务器；B 是这个服务器的 ip 地址；C 表示的是这个服务器与集群中的 Leader 服务器交换信息的端口；D 表示的是万一集群中的 #Leader 服务器挂了，需要一个端口来重新进行选举，选出一个新的 Leader，而这个端口就是用来执行选举时服务器相互通信的端口。如果是伪集群的配置方式，由于 B 都是一样，所以不同的 #Zookeeper 实例通信端口号不能一样，所以要给它们分配不同的端口号。server.1=10.0.30.138:2555:3555server.2=10.0.30.139:2555:3555


#### 增加myid文件

并在data目录下放置myid文件：(上面zoo.cfg中的dataDir)
mkdir datavi myid
myid指明自己的id，对应上面zoo.cfg中server.后的数字，第一台的内容为1，第二台的内容为2，内容如下：

myid
1
### 4. 启动
./bin/zkServer.sh start
### 5. 停止
./bin/zkServer.sh stop
注:命令行参考 (See: [http://zookeeper.apache.org/doc/r3.3.6/zookeeperAdmin.html](http://zookeeper.apache.org/doc/r3.3.6/zookeeperAdmin.html))

### 6. 验证启动状态
telnet 127.0.0.1 2181


### 7. 用法
dubbo.registry.address=zookeeper://10.0.30.138:2181?backup=10.0.30.139:2181
或者

### 8. 测试zk服务

执行$ zkCli.sh -server 10.0.30.138:2181，检查指定服务器是否成功启动。


下面给我的单机3个伪集群的案例：


(1).分别在三台机器上安装好zookeeper，最好是同版本的。

直接cp三个即可，做一下标记，便于区分。

(2).修改配置文件如下

vi conf/zoo.cfg

下面分别是三个节点的配置文件样本
1
![](https://img-blog.csdn.net/20160921152605334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 2

![](https://img-blog.csdn.net/20160921152454255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3
![](https://img-blog.csdn.net/20160921152422769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意：其中data目录和server地址需改成你真实部署机器的信息

(3)．增加myid文件

并在data目录下放置myid文件：(上面zoo.cfg中的dataDir)

#cd /data/retail/zookeeper/

vi myid

myid指明自己的id，对应上面zoo.cfg中server.后的数字，第一台的内容为1，第二台的内容为2，依次类推。



