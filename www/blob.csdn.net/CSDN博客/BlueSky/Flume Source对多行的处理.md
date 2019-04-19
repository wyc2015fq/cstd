# Flume Source对多行的处理 - BlueSky - CSDN博客
2016年03月29日 09:46:11[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：3985
ExecSource会`readLine()`读取日志中的每一行，把其作为每一个flume event的body放进去，这对于大部分这种每行就可以结束的日志记录，是完全可以的：
```
2016-03-18 17:53:48,374 INFO namenode.FSNamesystem (FSNamesystem.java:listCorruptFileBlocks(7217)) - there are no corrupt file blocks.
2016-03-18 17:53:48,278 INFO namenode.FSNamesystem (FSNamesystem.java:listCorruptFileBlocks(7217)) - there are no corrupt file blocks.
```
但是，对于有`stacktrace`的`ERROR`日志记录，如果把一行的内容当作一个flume event会有很大的问题，直观上来看，肯定需要把若干行看作是一个flume event，比如下面这样的日志记录，要作为一个flume event，而不是27个（一共27行）：
```
2016-03-18 17:53:40,278 ERROR [HiveServer2-Handler-Pool: Thread-26]: Error occurred during processing of message.
java.lang.RuntimeException: org.apache.thrift.transport.TTransportException: java.net.SocketException: Connection reset
	at org.apache.thrift.transport.TSaslServerTransport$Factory.getTransport(TSaslServerTransport.java:219)
	at org.apache.thrift.server.TThreadPoolServer$WorkerProcess.run(TThreadPoolServer.java:268)
	at java.util.concurrent.ThreadPoolExecutor.runWorker(ThreadPoolExecutor.java:1145)
	at java.util.concurrent.ThreadPoolExecutor$Worker.run(ThreadPoolExecutor.java:615)
	at java.lang.Thread.run(Thread.java:745)
Caused by: org.apache.thrift.transport.TTransportException: java.net.SocketException: Connection reset
	at org.apache.thrift.transport.TIOStreamTransport.read(TIOStreamTransport.java:129)
	at org.apache.thrift.transport.TTransport.readAll(TTransport.java:86)
	at org.apache.thrift.transport.TSaslTransport.receiveSaslMessage(TSaslTransport.java:178)
	at org.apache.thrift.transport.TSaslServerTransport.handleSaslStartMessage(TSaslServerTransport.java:125)
	at org.apache.thrift.transport.TSaslTransport.open(TSaslTransport.java:271)
	at org.apache.thrift.transport.TSaslServerTransport.open(TSaslServerTransport.java:41)
	at org.apache.thrift.transport.TSaslServerTransport$Factory.getTransport(TSaslServerTransport.java:216)
	... 4 more
Caused by: java.net.SocketException: Connection reset
	at java.net.SocketInputStream.read(SocketInputStream.java:196)
	at java.net.SocketInputStream.read(SocketInputStream.java:122)
	at java.io.BufferedInputStream.fill(BufferedInputStream.java:235)
	at java.io.BufferedInputStream.read1(BufferedInputStream.java:275)
	at java.io.BufferedInputStream.read(BufferedInputStream.java:334)
	at org.apache.thrift.transport.TIOStreamTransport.read(TIOStreamTransport.java:127)
	... 10 more
```
我这里的实现方式是：识别每行的开头部分，如果满足某种条件，就当作一条日志，否则，视作是上一条的日志的一部分。
比如：
对于上面举的例子来说（即符合标准log4j的日志），如果每一行开头满足下面这条正则表达式：
```
\s?\d\d\d\d-\d\d-\d\d\s\d\d:\d\d:\d\d,\d\d\d
```
就当作一条新的日志，如果不满足，就说明该行内容是上一条日志（已规定格式开头的那条）的一部分。
当然，我增加了可以自定义配置以哪种方式开头视为一条日志的regex配置，可以对不通的source进行不通的配置，已满足要求。
有了这样的约束，就可以写出将某些多行看作一个flume event的ExecSource，我把它开源到了github上，如有兴趣，欢迎前去试用，如有任何建议，欢迎提出与指正：MultiLineExecSource
```
github.com/qwurey/flume-source-multiline
```
该版本基于`flume-ng-core 1.6.0`
