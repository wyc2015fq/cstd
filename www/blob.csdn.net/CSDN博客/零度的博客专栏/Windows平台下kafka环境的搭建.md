# Windows平台下kafka环境的搭建 - 零度的博客专栏 - CSDN博客
2018年06月28日 11:03:04[零度anngle](https://me.csdn.net/zmx729618)阅读数：197
- 近期在搞kafka，在Windows环境搭建的过程中遇到一些问题，把具体的流程几下来防止后面忘了。 
- 准备工作： 
一、安装jdk环境 
[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
二、下载kafka的程序安装包： 下载二进制文件
[http://kafka.apache.org/downloads](http://kafka.apache.org/downloads)
 解压文件提取出里面的文件 
 在cmd命令行打开至根目录下，内容如下： 
![这里写图片描述](https://img-blog.csdn.net/20170115145927531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 (1).bin目录下存放的是程序运行时使用的脚本文件，window平台是一个独立的文件夹里面存放着 .bat 文件，bin的目录下存放的是 Linux 平台使用的 .sh 的shell脚本，在window平台上用不到，嫌麻烦可以删了。 
 (2).config目录下存放的是一些程序运行的配置文件，在后期自定义使用kafka的时候需要修改里面的文件内容。 
 (3).libs目录是打包好的jar包，这个版本自带了zookeeper的jar包，所以在安装的过程中不需要再在本地安装zookeeper了。
三、启动kafka服务并测试生产与消费
1、 启动zookeeper服务： 
-  在正常启动zoopkeeper之前需要修改zookeeper.properties的文件内容，将其data的输出目录指定一下，可自行创建一个文件夹如下： 
![这里写图片描述](https://img-blog.csdn.net/20170115150030816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 然后启动：
`bin\windows\zookeeper-server-start.bat config\zookeeper.properties`- 正常启动后他的状态是这样的： 
![这里写图片描述](https://img-blog.csdn.net/20170115150114333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我在启动时报了个错误：错误: 找不到或无法加载主类 Files\Java\jdk1.8.0_144\lib\dt.jar;C:\Program
我下载的版本是：**kafka_2.11-1.0.0**
解决方法很简单。
打开路径** kafka_2.11-1.0.0\bin\windows** 下的文件 **kafka-run-class.bat **
修改**第179行**
set COMMAND=%JAVA% %KAFKA_HEAP_OPTS% %KAFKA_JVM_PERFORMANCE_OPTS% %KAFKA_JMX_OPTS% %KAFKA_LOG4J_OPTS% -cp **"%CLASSPATH%"** %KAFKA_OPTS% %*
给%CLASSPATH%加上双引号即可。其他版本若遇到此问题，也是修改这一句代码，但不一定在这一行。
2.  启动kfaka服务： 
 在启动前，任然需要修改server.properties中log.dir的配置目录， 
![这里写图片描述](https://img-blog.csdn.net/20170115150149646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 修改后，启动服务： 
       bin\windows\kafka-server-start.bat config\server.properties
![这里写图片描述](https://img-blog.csdn.net/20170115150229157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.  创建一个主题：
`bin\windows\kafka-topics.bat --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic kjTest`- ![这里写图片描述](https://img-blog.csdn.net/20170115150459783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以使用如下命令查看创建的主题列表：
`bin\windows\kafka-topics.bat --list --zookeeper localhost:2181`- ![这里写图片描述](https://img-blog.csdn.net/20170115150545815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.  启动生产者：
`bin\windows\kafka-console-producer.bat --broker-list localhost:9092 --topic kjTest`- ![这里写图片描述](https://img-blog.csdn.net/20170115150612073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此时可以从控制台输入信息，待消费者启动后可接收到生产者发布的消息。 
![这里写图片描述](https://img-blog.csdn.net/20170115150702793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
5.  启动消费者：
`bin\windows\kafka-console-consumer.bat --zookeeper localhost:2181 --topic kjTest --from-beginning`- 此时便能看到发布出去的消息了 
![这里写图片描述](https://img-blog.csdn.net/20170115150739076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上述过程总结：
一、四个命令行窗口
**第一个命令窗口**->启动zookeeper服务：
bin\windows\zookeeper-server-start.bat config\zookeeper.properties
**第二个命令窗口**->启动kfaka服务：
bin\windows\kafka-server-start.bat config\server.properties 
**第三个命令窗口**->启动启produce:
创建一个主题：
bin\windows\kafka-topics.bat --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic ydc1Test
使用如下命令查看创建的主题列表：
bin\windows\kafka-topics.bat --list --zookeeper localhost:2181
启动生产者：
bin\windows\kafka-console-producer.bat --broker-list 192.168.51.41:9092 --topic ydc1Test
**第四个命令窗口**->启动consumer：
bin\windows\kafka-console-consumer.bat --zookeeper localhost:2181 --topic ydc1Test --from-beginning 
二、Java：测试kafka生产和消费
参考网址：http://blog.csdn.net/honglei915/article/details/37563647
三、心得总结：
 1.produce启动的时候，参数使用的是kafka的端口（默认：9092）；而consumer启动的时候使用的是zookeeper（默认：2181）的端口；
 2.必须先创建topic才能使用；
 3.topic本质是以文件的形式储存在zookeeper上的。
四、 在后期还可以启动多个集群服务，同时进行操作，修改server.properties中的内容：便可启动，与上相似。 
```
config/server-1.properties:
        broker.id=<new id>
        listeners=PLAINTEXT://:<new port>
        log.dir=<you log dir>
```
- 
`操作步骤：`- 1).分别启动需要的kafka服务
![这里写图片描述](https://img-blog.csdn.net/20170115150815030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170115150854910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2). 创建主题，设置分区数目 
![这里写图片描述](https://img-blog.csdn.net/20170115150931176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以使用下面的命令查看是否创建成功，和具体的分区 
![这里写图片描述](https://img-blog.csdn.net/20170115151137099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3). 分别启动生产者消费者 
![这里写图片描述](https://img-blog.csdn.net/20170115151332114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170115151400335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4). 可以查看下当前工作状态 
![这里写图片描述](https://img-blog.csdn.net/20170115151431866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
查看得当前服务使用的是broker编号为2的服务，但我们将其down掉后 
![这里写图片描述](https://img-blog.csdn.net/20170115151504976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再次执行消息发送任务： 
![producer](https://img-blog.csdn.net/20170115151544460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![consumer](https://img-blog.csdn.net/20170115151641291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
消息仍能发送成功，命令行中会提示一个服务已关闭。 
 就查看当前状态： 
![这里写图片描述](https://img-blog.csdn.net/20170115151725307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzI0ODU1NzM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 此时的leader换成了broker 0
参考资料：[http://kafka.apache.org/quickstart](http://kafka.apache.org/quickstart)
```
```
- 
```
```
- 
