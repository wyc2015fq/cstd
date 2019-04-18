# windows安装zookeeper、kafka - weixin_33985507的博客 - CSDN博客
2018年04月04日 22:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
# 一、搭建环境
## [](#1-%E5%AE%89%E8%A3%85jdk)1. 安装JDK
- 安装文件：[http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html下载Server](https://link.jianshu.com?t=http%3A%2F%2Fwww.oracle.com%2Ftechnetwork%2Fjava%2Fjavase%2Fdownloads%2Fjre8-downloads-2133155.html%25E4%25B8%258B%25E8%25BD%25BDServer) JRE.
- 安装完成后需要添加以下的环境变量（右键点击“我的电脑” -> "高级系统设置" -> "环境变量" ）：
```
JAVA_HOME: C:\Program Files (x86)\Java\jre1.8.0_60（这个是默认安装路径，如果安装过程中更改了安装目录，把更改后的路径填上就行了）
PATH: 在现有的值后面添加"%JAVA_HOME%\bin"
```
- 打开cmd运行 `java -version` 查看当前系统Java的版本：
## [](#2-%E5%AE%89%E8%A3%85zookeeper)2. 安装Zookeeper
Kafka的运行依赖于Zookeeper，所以在运行Kafka之前我们需要安装并运行Zookeeper
- 下载安装文件： [http://zookeeper.apache.org/releases.html](https://link.jianshu.com?t=http%3A%2F%2Fzookeeper.apache.org%2Freleases.html)
- 解压文件（本文解压到 D:\zookeeper-3\zookeeper-3.5.3-beta）
- 打开D:\zookeeper-3\zookeeper-3.5.3-beta8\conf，把zoo_sample.cfg重命名成zoo.cfg
- 从文本编辑器里打开zoo.cfg
- 把dataDir的值改成“:G:\zookeeper-3.5.3-beta\data”(任意)
- 添加如下系统变量：
```
ZOOKEEPER_HOME: D:\zookeeper-3\zookeeper-3.5.3-beta
Path: 在现有的值后面添加 "%ZOOKEEPER_HOME%\bin"
```
- 
运行Zookeeper:
- 
打开cmd，进入bin目录
```
cd  D:\zookeeper-3\zookeeper-3.5.3-beta\bin
```
- 
在此窗台继续输入，就能启动zookeeper
```
zkServer.cmd
```
- 
新开一个cmd，进入bin目录，端口查到，就说明已经启动
```
netstat -ano|findstr "2181"
```
## [](#3-%E5%AE%89%E8%A3%85%E5%B9%B6%E8%BF%90%E8%A1%8Ckafka)3. 安装并运行Kafka
- 下载安装文件： [http://kafka.apache.org/downloads.html](https://link.jianshu.com?t=http%3A%2F%2Fkafka.apache.org%2Fdownloads.html)
- 解压文件（本文解压到 D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0）
- 打开D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0\config
- 从文本编辑器里打开 server.properties
- 把 log.dirs的值改成 “D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0\kafka-logs”
- 打开cmd
- 进入kafka文件目录: cd D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0
- 输入并执行以打开kafka:
```
.\bin\windows\kafka-server-start.bat .\config\server.properties
```
## [](#4-%E5%88%9B%E5%BB%BAtopics)4. 创建topics
- 打开cmd 并进入
```
cd D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0\bin\windows
```
- 创建一个topic：
```
kafka-topics.bat --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic test
```
## [](#5-%E6%89%93%E5%BC%80%E4%B8%80%E4%B8%AAproducer)5. 打开一个Producer：
- 打开cmd 并进入
```
cd D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0\bin\windows
```
- 打开一个Producer：
```
kafka-console-producer.bat --broker-list localhost:9092 --topic test
```
## [](#6-%E6%89%93%E5%BC%80%E4%B8%80%E4%B8%AAconsumer)6. 打开一个Consumer:
- 打开cmd 并进入
```
cd D:\kafka_2.11-1.1.0\kafka_2\kafka_2.11-1.1.0\bin\windows
```
- 打开一个Consumer：
```
kafka-console-consumer.bat --zookeeper localhost:2181 --topic test
```
然后就可以在Producer控制台窗口输入消息了。在消息输入过后，很快Consumer窗口就会显示出Producer发送的消息：
至此，Kafka运行环境的搭建就完成啦~~~~
![11487306-e730a84efd0cc765.jpg](https://upload-images.jianshu.io/upload_images/11487306-e730a84efd0cc765.jpg)
1.jpg
