# ActiveMQ——3、ActiveMQ高可用与集群搭建 - 隔壁老王的专栏 - CSDN博客





2016年12月29日 19:49:19[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：5657







**1. ActiveMQ的高可用原理**

使用ZooKeeper（集群）注册所有的ActiveMQ Broker。只有其中的一个Broker可以提供服务，被视为 Master，其他的 Broker 处于待机状态，被视为Slave。如果Master因故障而不能提供服务，Zookeeper会从Slave中选举出一个Broker充当Master。

Slave连接Master并同步他们的存储状态，Slave不接受客户端连接。所有的存储操作都将被复制到 连接至 Master的Slaves。如果Master宕了，得到了最新更新的Slave会成为 Master。故障节点在恢复后会重新加入到集群中并连接Master进入Slave模式。

是不是觉得和Redis Sentinel主从高可用的方式很像，这里的zookeeper起到的作用和reids里的sentinel作用差不多。

**另外，附上官方文档的一则警告，请使用者注意。replicated LevelDB 不支持延迟或者计划任务消息。这 些消息存储在另外的LevelDB文件中，如果使用延迟或者计划任务消息，将不会复制到Slave Broker上，不能实现消息的高可用。**

**2.ActiveMQ高可用环境搭建**

本人是在一台CentOS虚拟机上进行测试的，在开发中需要根据自己的实际情况作出相应的调整。在这台服务器上，配置了3个ActiveMQ，如下图所示：

![](https://img-blog.csdn.net/20161229195345447)


后面需要修改的配置文件都在ACTIVEMQ_HOME/conf文件夹下。首先修改每个ActiveMQ的持久化方式（修改ACTIVEMQ_HOME/bin/activemq.xml文件），ActiveMQ默认使用的是kahaDB作为持久化存储数据的，这里修改成levelDB。如下图所示：

**![](https://img-blog.csdn.net/20161229195028958)**

接下来修改ActiveMQ的TCP链接端口号，activemq-1使用默认的61616端口，activemq-2修改为61617，activemq-3修改为61618。如下图所示（注意红框部分）：

![](https://img-blog.csdn.net/20161229195810301)


![](https://img-blog.csdn.net/20161229195843410)


![](https://img-blog.csdn.net/20161229195910581)


修改并保存之后，就是修改jetty的端口号（修改ACTIVEMQ_HOME/bin/jetty.xml文件），因为实在同一台服务器上，不修改的话，第二个和第三个jetty将启动不了。第activemq-1依然使用默认端口8161，activemq-2使用8162，activemq-3使用8163端口，如下图：

![](https://img-blog.csdn.net/20161229200359447)


![](https://img-blog.csdn.net/20161229200417713)


![](https://img-blog.csdn.net/20161229200435037)


到这里，ActiveMQ高可用就配置好了，如果没启动zookeeper的话，先启动zookeeper（可以看我的关于zookeeper的博客），然后分别启动activemq-1，activemq-2，activemq-3。进入ACTIVEMQ_HOME/data下，查看activemq.log文件，如果没有报错则说明启动成功！在zookeeper上，可以看到如下数据：

![](https://img-blog.csdn.net/20161229203258080)


**其中elected不为空的节点表示为Master，由该activemq对外提供服务。**

好了，让我们来撸点代码测试一下吧！

ClustorProducer：

```java
public class ClustorProducer {
    private ConnectionFactory factory;
    private Connection connection;
    private Session session;
    private Destination destination;
    private MessageProducer producer;

    public ClustorProducer() throws JMSException {
        this.factory = new ActiveMQConnectionFactory(ActiveMQConnectionFactory.DEFAULT_USER,
                ActiveMQConnectionFactory.DEFAULT_PASSWORD,
                "failover:(tcp://192.168.4.19:61616,tcp://192.168.4.19:61617,tcp://192.168.4.19:61618)?randomize=false");
        this.connection = factory.createConnection();
        connection.start();
        this.session = connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE);
        this.destination = session.createQueue("first");
        producer = session.createProducer(destination);
    }

    public void send() throws JMSException, InterruptedException {
        for(int i=1; i<=50000; i++) {
            Message message = session.createTextMessage("内容：" + i);
            producer.send(destination, message);
            System.out.println(message);
            Thread.sleep(1000);
        }
    }

    public static void main(String[] args) throws JMSException, InterruptedException {
        ClustorProducer clustorProducer = new ClustorProducer();
        clustorProducer.send();
    }
}
```
ClustorConsumer：
```java
public class ClustorConsumer {
    private ConnectionFactory factory;
    private Connection connection;
    private Session session;
    private Destination destination;
    private MessageConsumer consumer;

    public ClustorConsumer() throws JMSException {
        this.factory = new ActiveMQConnectionFactory(ActiveMQConnectionFactory.DEFAULT_USER,
                ActiveMQConnectionFactory.DEFAULT_PASSWORD,
                "failover:(tcp://192.168.4.19:61616,tcp://192.168.4.19:61617,tcp://192.168.4.19:61618)?randomize=false");
        this.connection = factory.createConnection();
        connection.start();
        this.session = connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE);
        this.destination = session.createQueue("first");
        this.consumer = session.createConsumer(destination);
    }

    public void consume() throws JMSException, InterruptedException {
        while (true) {
            Message message = consumer.receive();
            if(message == null)
                break;
            System.out.println(message);
            Thread.sleep(1000);
        }
    }

    public static void main(String[] args) throws JMSException, InterruptedException {
        ClustorConsumer clustorConsumer = new ClustorConsumer();
        clustorConsumer.consume();
    }
}
```
其中的brokerUrl参数发生了变化变成了：failover:(tcp://192.168.4.19:61616,tcp://192.168.4.19:61617,tcp://192.168.4.19:61618)?randomize=false


停掉三个ActiveMQ中任意的一个，我们可以发现依然可以发送和接收消息。说明ActiveMQ的高可用很成功！


**3.ActiveMQ集群负载均衡搭建**

之前已经实现了ActiveMQ的高可用部署，单仅仅是高可用集群，无法达到负载均衡的作用，接下来只需简单配置就能完成可以实现负载均衡的集群功能：

在集群1的activemq.xml中链接集群2（在persistenceAdapter标签前配置）：

<networkConnectors>

    <networkConnector uri="static:(tcp://192.168.1.103:61616,tcp://192.168.2.103:61617,tcp://192.168.2.103:61618)" duplex="false"/>

</networkConnectors>

在集群2的activemq.xml中链接集群1（在persistenceAdapter标签前配置）：

<networkConnectors>

    <networkConnector uri="static:(tcp://192.168.1.104:61616,tcp://192.168.1.104:61617,tcp://192.168.1.104:61618)" duplex="false"/>

</networkConnectors>

这样就实现了ActiveMQ的集群高可用负载均衡功能。

客户端连接：

ActiveMQ 的客户端只能访问Master的Broker，其他处于Slave的Broker不能访问。所以客户端连接Broker应该使用failover协议。

配置文件地址应为：
failover:(tcp://192.168.1.103:61616,tcp://192.168.1.103:61617,tcp://192.168.1.103:61618)?randomize=false

或：
failover:(tcp://192.168.1.104:61616,tcp://192.168.1.104:61617,tcp://192.168.1.104:61618)?randomize=false


****



