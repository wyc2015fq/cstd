# MongoDB 查询超时异常 SocketTimeoutException - 文章 - 伯乐在线
原文出处： [忙碌的键盘](https://my.oschina.net/gooke/blog/382722)
在对超过百万条记录的集合进行聚合操作。


```
DBObject match=(DBObject)JSON.parse("{$match:{logType:{'$in':[5,9]}}}");
DBObject group=(DBObject)JSON.parse("{$group:{'_id':'$domainUrl','count':{'$sum':1}}}");
AggregationOutput output = logCollection.aggregate(match,group);
```
偶尔会发生Read timed out 异常。


```
com.mongodb.MongoException$Network: Read operation to server /192.168.10.202:27017 failed on database adLogTable
	at com.mongodb.DBTCPConnector.innerCall(DBTCPConnector.java:253)
	at com.mongodb.DB.command(DB.java:261)
	at com.mongodb.DB.command(DB.java:243)    ...
Caused by: java.net.SocketTimeoutException: Read timed out
    at java.net.SocketInputStream.socketRead0(Native Method)
    at java.net.SocketInputStream.read(SocketInputStream.java:152)
```
通过多次测试，发现执行一次聚合平均时间为5s，超过5s时就会报错！
然后查看MongoDB的配置信息：


```
socket-timeout="5000"   //5s
```
socket-timeout的默认配置为0，也就是没有限制。
没有超时限制，系统出了问题也不容易发现，应该根据实际情况，给出合理的超时时间。
通过多次测试发现最长执行时间为6秒，就把超时时间设置成了10000。


```
socket-timeout="10000"   //10s
```
注意：MongoDB在与Spring整合时，如果要配置多个MongDB源，只会启用最后一个配置。
应该把参数配置信息存储在properties文件中。


```
<code>
  <mongo>
    <options threads-allowed-to-block-for-connection-multiplier="100" connect-timeout="1000" max-wait-time="1000" auto-connect-retry="true" socket-keep-alive="true" socket-timeout="10000" slave-ok="true" write-number="1" write-timeout="0" write-fsync="true"/>
  </mongo>
</code>
```
##### 通过Java API获取配置参数


```
DBCollection logCollection = mongoTemplate.getCollection(collName);
MongoOptions mongoOptions = logCollection.getDB().getMongo().getMongoOptions();
System.out.println(mongoOptions.getSocketTimeout());
```
### 最后一点：  ConnectionTimeOut和SocketTimeOut的区别：
一次完整的请求包括三个阶段：1、建立连接 2、数据传输 3、断开连接
如果与服务器(这里指数据库)请求建立连接的时间超过ConnectionTimeOut，就会抛 ConnectionTimeOutException，即服务器连接超时，没有在规定的时间内建立连接。
如果与服务器连接成功，就开始数据传输了。
如果服务器处理数据用时过长，超过了SocketTimeOut，就会抛出SocketTimeOutExceptin，即服务器响应超时，服务器没有在规定的时间内返回给客户端数据。
