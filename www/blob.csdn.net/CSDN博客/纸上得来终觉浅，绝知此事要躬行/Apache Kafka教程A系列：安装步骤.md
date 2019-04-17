# Apache Kafka教程A系列：安装步骤 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月03日 10:14:36[boonya](https://me.csdn.net/boonya)阅读数：127








原文地址：[https://www.tutorialspoint.com/apache_kafka/apache_kafka_installation_steps.htm](https://www.tutorialspoint.com/apache_kafka/apache_kafka_installation_steps.htm)

以下是在您的机器上安装Java的步骤。



#### 第1步 - 验证Java安装

希望你现在已经在你的机器上安装了java，所以你只需使用下面的命令来验证它。
`$ java -version`如果您的计算机上已成功安装Java，则可以看到已安装的Java版本。


##### 步骤1.1 - 下载JDK
如果未下载Java，请通过访问以下链接并下载最新版本下载最新版本的JDK。
[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
现在最新版本是JDK 8u 60，文件是“jdk-8u60-linux-x64.tar.gz”。 请在您的机器上下载该文件。


##### 步骤1.2 - 提取文件

通常，下载的文件存储在下载文件夹中，验证它并使用以下命令提取tar设置。

```
$ cd /go/to/download/path
$ tar -zxf jdk-8u60-linux-x64.gz
```

##### 步骤1.3 - 移至Opt目录

要让所有用户都可以使用java，请将提取的java内容移至usr / local / java /文件夹。

```
$ su
password: (type password of root user)
$ mkdir /opt/jdk
$ mv jdk-1.8.0_60 /opt/jdk/
```

##### 步骤1.4 - 设置路径

要设置路径和JAVA_HOME变量，请将以下命令添加到〜/ .bashrc文件中。

```
export JAVA_HOME =/usr/jdk/jdk-1.8.0_60
export PATH=$PATH:$JAVA_HOME/bin
```

现在将所有更改应用到当前运行的系统中。
`$ source ~/.bashrc`
##### 步骤1.5 - Java选择

使用以下命令更改Java Alternatives。
`update-alternatives --install /usr/bin/java java /opt/jdk/jdk1.8.0_60/bin/java 100`
##### 步骤1.6 - 再次验证Java

现在使用步骤1中介绍的验证命令（java -version）验证java。


#### 第2步 - ZooKeeper框架安装

##### 步骤2.1 - 下载ZooKeeper
要在您的机器上安装ZooKeeper框架，请访问以下链接并下载最新版本的ZooKeeper。
[http://zookeeper.apache.org/releases.html](http://zookeeper.apache.org/releases.html)
截至目前，ZooKeeper的最新版本是3.4.6（ZooKeeper-3.4.6.tar.gz）。


##### 步骤2.2 - 解压缩tar文件

使用以下命令提取tar文件

```
$ cd opt/
$ tar -zxf zookeeper-3.4.6.tar.gz
$ cd zookeeper-3.4.6
$ mkdir data
```

##### 步骤2.3 - 创建配置文件

使用vi“conf / zoo.cfg”命令打开名为conf / zoo.cfg的配置文件，并将所有以下参数设置为起点。

```
$ vi conf/zoo.cfg
tickTime=2000
dataDir=/path/to/zookeeper/data
clientPort=2181
initLimit=5
syncLimit=2
```
一旦配置文件保存成功并再次返回到终端，您可以启动zookeeper服务器。


##### 步骤2.4 - 启动ZooKeeper服务器
`$ bin/zkServer.sh start`
执行此命令后，您将得到如下所示的响应 -

```
$ JMX enabled by default
$ Using config: /Users/../zookeeper-3.4.6/bin/../conf/zoo.cfg
$ Starting zookeeper ... STARTED
```

##### 步骤2.5 - 启动CLI
`$ bin/zkCli.sh`
输入上述命令后，您将连接到zookeeper服务器，并获得以下响应。

```
Connecting to localhost:2181
................
................
................
Welcome to ZooKeeper!
................
................
WATCHER::
WatchedEvent state:SyncConnected type: None path:null
[zk: localhost:2181(CONNECTED) 0]
```

##### 步骤2.6 - 停止Zookeeper服务器
连接服务器并执行所有操作后，可以使用以下命令停止zookeeper服务器 -

`$ bin/zkServer.sh stop`
现在您已经在您的机器上成功安装了Java和ZooKeeper。 让我们看看安装Apache Kafka的步骤。



#### 第3步 - Apache Kafka安装
让我们继续以下步骤在您的机器上安装Kafka。


##### 步骤3.1 - 下载Kafka
要在您的机器上安装Kafka，请点击下面的链接 - 
[https://www.apache.org/dyn/closer.cgi?path=/kafka/0.9.0.0/kafka_2.11-0.9.0.0.tgz](https://www.apache.org/dyn/closer.cgi?path=/kafka/0.9.0.0/kafka_2.11-0.9.0.0.tgz)
现在最新的版本，即 - kafka_2.11_0.9.0.0.tgz将被下载到您的机器上。


##### 第3.2步 - 提取tar文件

使用以下命令提取tar文件 -

```
$ cd opt/
$ tar -zxf kafka_2.11.0.9.0.0 tar.gz
$ cd kafka_2.11.0.9.0.0
```
现在您已经在您的机器上下载了最新版本的Kafka。


##### 步骤3.3 - 启动服务器

您可以通过提供以下命令启动服务器 -
`$ bin/kafka-server-start.sh config/server.properties`
服务器启动后，您会在屏幕上看到以下响应 -

```
$ bin/kafka-server-start.sh config/server.properties
[2016-01-02 15:37:30,410] INFO KafkaConfig values:
request.timeout.ms = 30000
log.roll.hours = 168
inter.broker.protocol.version = 0.9.0.X
log.preallocate = false
security.inter.broker.protocol = PLAINTEXT
…………………………………………….
…………………………………………….
```

##### 步骤3.4 - 停止服务器

执行所有操作后，可以使用以下命令停止服务器 
`$ bin/kafka-server-stop.sh config/server.properties`现在我们已经讨论了Kafka的安装，我们可以在下一章学习如何在Kafka上进行基本的操作。



