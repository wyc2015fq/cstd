# Zookeeper集群部署与配置（三） - z69183787的专栏 - CSDN博客
2018年02月24日 18:12:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：98
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
[http://blog.csdn.net/wo541075754/article/details/56830137](http://blog.csdn.net/wo541075754/article/details/56830137)
## 环境
集群配置的环境与单机配置的环境相同，唯一不同的就是集群是在多台服务器之间配置，当然也有伪集群的配置，也就是在同一台机器上配置多台服务，通过端口号的不同来进行区分。
## 配置文件
在单机配置文件内增加server项的配置，新增之后的配置文件如下：
```
tickTime=2000
initLimit=10
syncLimit=5
dataDir=/tmp/zookeeper
clientPort=2181
server.1=IP1:2888:3888
server.2=IP2:2888:3888
server.3=IP3:2888:3888
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### 配置项解释
|参数名|说明|
|----|----|
|clientPort|无默认值，必须配置，不支持系统属性方式配置。当前服务器对外提供服务端口，客户通过此端口与服务器建立连接，一般设置为1281。集群中不需要统一端口，可任意配置端口。|
|dataDir|无默认值，必须配置。服务器存储快照文件目录。默认情况下，如果没有配置dataLogDir，那么事务日志也会存储在这个目录中。建议将事务日志目录独立配置。|
|tickTime|默认值：3000（ms），，不支持系统属性方式配置。用于配置zookeeper中最小时间单元长度，很多运行时的时间间隔都是使用它的倍数来表示的。|
|initLimit|Leader Zookeeper接收集群其他服务器初始化等待最大时间（10*tickTime）。|
|syncLimit|Leader Zookeeper和集群其他服务器通信最大时间（5*tickTime）。|
server.id=host:port:port解析
每一行此配置表示一个集群中的一台服务器。其中id为Server ID，用来标识该机器在集群中的编号。同时，在所在服务器的数据目录（/tmp/zookeeper）下创建一个myid文件，该文件只有一行内容，并且是一个数字，就是对应每台服务器的Server ID数字。
比如server.1=IP1:2888:3888的myid中的内容就是1。不同服务器的ID需要保持不同，并且和zoo.cfg文件中server.id中的id和myid文件的内容保持一致。id的取值范围为1~255。
其中，server.id中配置参数的第一个port是集群中其他机器与Leader之间通信的端口，第二个port为当Leader宕机或其他故障时，集群进行重新选举Leader时使用的端口。
按照以上相同步骤，配置集群中的其他机器。每个集群的zoo.cfg文件都是相同的，可通过版本控制或其他工具保证每台zookeeper服务器的配置文件相同。集群中每台机器唯一不同的是server.id对应的myid文件中的数字不同。
## 启动验证
完成以上操作之后，启动集群中的服务器，验证服务器状态。此步骤与单机模式相同，不再赘述。
需要注意的是一般zookeeper集群由3~5台服务器组成，即2n+1台机器。后面章节会详细介绍为什么如此。
