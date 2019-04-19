# activemq  安装 部署 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月09日 15:33:34[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：9
ActiveMQ是一种开源的，实现了JMS1.1规范的，面向消息(MOM)的中间件，为应用程序提供高效的、可扩展的、稳定的和安全的企业级消息通信。ActiveMQ使用Apache提供的授权，任何人都可以对其实现代码进行修改。
ActiveMQ 是Apache出品，最流行的，能力强劲的开源消息总线。
Java Message Service：是Java平台上有关面向消息中间件的技术规范。
JMS类似于JDBC，JDBC是可以用来访问许多不同关系数据库的API，而JMS则提供同样与厂商无关的访问的API，以访问消息收发服务。比如IBM 的MQSeries、BEA的Weblogic JMS service；而ActiveMQ也是其中的一种，所以：activeMQ就是支持jms规范的一个server；它对于JDBC和数据库的关系来说，它就是个mysql（MQSeries就是DB2，Weblogic JMS service就是Oracle）。
目前比较流行的队列服务器有很多，如：ActiveMQ 、RabbitMQ，ZeroMQ，Kafka，MetaMQ，RocketMQ等
安装
[http://activemq.apache.org/](http://activemq.apache.org/)
chmod 775 ./activemq
./activemq start
需要用到两个端口
一个是消息通讯的端口（使用tcp协议默认为61616），可查看conf/activemq.xml
一个是管理控制台端口（默认为8161）可在conf/jetty.xml中修改
启动成功就可以以 http://ip地址:8161 方式访问管理界面，控制台默认用户名和密码为：admin/admin，存放在jetty-realm.properties，可进行编辑修改：格式为 用户名：密码，角色
IM和视频聊天的，可以参考下这个 [https://github.com/starrtc/starrtc-android-demo](https://github.com/starrtc/starrtc-android-demo)
[https://www.jianshu.com/p/72f6aaf7b305](https://www.jianshu.com/p/72f6aaf7b305)
[https://www.cnblogs.com/5201351/p/4901380.html](https://www.cnblogs.com/5201351/p/4901380.html)
