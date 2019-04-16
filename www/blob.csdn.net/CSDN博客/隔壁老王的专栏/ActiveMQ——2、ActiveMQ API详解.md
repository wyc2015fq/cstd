# ActiveMQ——2、ActiveMQ API详解 - 隔壁老王的专栏 - CSDN博客





2016年12月25日 11:41:15[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：536








**1.Connection**

在成功创建正确的ConnectionFactory后，下一步是创建一个连接，它是JMS定义的一个接口。ConnectionFactory负责返回可以与底层消息传递系统进行通信的Connection实现。通常客户端只使用单一连接。根据JMS文档，Connection的目的是“利用JMS提供者封装开放的连接”，以及表示“客户端与提供者服务进程之间的开放TCP/IP套接字”。该文档还指出Connection应该是进行客户端身份验证的地方等等。

当一个Connection被创建时，它的传输默认是关闭的，必须使用start方法开启。一个Connection可以创建一个或多个Session。当一个程序执行完成后，必须关闭之前创建的Connection，否则ActiveMQ不会释放资源。关闭Connection同时也关闭了Session、MessageProducer和MessageConsumer。创建Connection的方法如下：

```java
Connection connection = ConnectionFactory.createConnection();
Connection connection = ConnectionFactory.createConnection(String username, String password);
```
**2.Session**


一旦从ConnectionFactory中获得Connection，必须从Connection中创建一个或多个Session。Session是一个发送或接收消息的线程，可以使用Session创建MessageProducer、MessageConsumer和Message。

Session可以被事务化，也可以不被事务化。可以通过Connection对象创建Session的方法传递一个布尔类型的参数进行设置，如下代码：

```java
Session session = connection.createSession(boolean transacted, int acknowledgeMode);
```
其中transacted为是否使用事务，acknowledgeMode为签收模式。



结束事务有两种方法：提交或者回滚。当一个事务提交时，消息被处理；如果事务中有一个步骤失败，事务就会回滚，这个事务中已经执行的操作将被撤销。在发送消息最后也必须使用session.commit()方法提交事务。

签收模式有三种：

（1）Session.AUTO_ACKNOWLEDGE：当客户端从receive或onMessage成功返回时，Session自动签收客户端的这条消息的收条。

（2）Session.CLIENT_ACKNOWLEDGE：客户端通过调用消息（Message）的acknowledge方法签收消息。在这种情况下，签收发生在Session层面：签收一个已消费的消息会自动签收这个Session所有已经消费的消息的收条。

（3）Session.DUPS_OK_ACKNOWLEDGE：指示Session不必确保对传送的消息的签收。它可能引起消息的重复，但是降低了Session的开销。所以只要客户端能够接受消息的重复，就可以使用该模式。

**3.MessageProducer**

MessageProducer是由Session创建的对象，用来向Destination发送消息。其中send()方法如下：

```java
void send(Destination destination, Message message);
void send(Destination destination, Message message, int deliveryMode, int priority, long timeToLive);
void send(Message message);
void send(Message message, int deliveryMode, int priority, long timeToLive);
```



其中deliveryMode为传送模式，priority为消息优先级**（要使用优先级，需要在conf/activemq.xml中配置才行。在broker->destinationPolicy->policyMap->policyEntries下添加<policyEntry queue="queue_name" prioritizedMessages="true" />，而且优先级仅只对于一个session内发送的消息）**，timeToLive为消息过期时间。ActiveMQ支持PERSISTENT和NON_PERSISTENT两种传送模式，如果不指定传送模式的话，默认是持久性消息。如果能够接受消息丢失，那么使用NON_PERSISTENT可以改善性能和减少存储的开销。

消息优先级有0-9十个级别，0-4是普通消息，5-9是加急消息。如果不指定优先级，默认的优先级是4。JMS不要求严格按照这十个优先级发送消息，但需要保证加急消息要先于普通消息到达。默认情况下消息永远不会过期，如果消息在特定的周期内失去意义，则可以设置过期时间，默认为毫秒。

**4.MessageConsumer**

MessageConsumer是由Session创建的对象，用于从Destination接收消息。

```java
MessageConsumer consumer = session.createConsumer(Destination destination);
MessageConsumer consumer = session.createConsumer(Destination destination, String messageSelector);
MessageConsumer consumer = session.createConsumer(Destination destination, String messageSelector, boolean noLocal);
TopicSubscriber subscriber = session.createDurableSubscriber(Topic topic, String name);
TopicSubscriber subscriber = session.createDurableSubscriber(Topic topic, String name, String messageSelector, boolean noLocal);
```
其中messageSelector为消息选择器，"JMS_TYPE = 'MY_TAG1'"选择器检查了传入消息的JMS_TYPE属性，并确定这个属性是否等于MY_TAG1，如果相等消息则被消费，不相等消息则被忽略；noLocal标识默认为false，当设置为true时限制消费者只能接收和自己相同的连接（Connection）所发布的消息，次标志只适用于主题，不适用于队列；name标识订阅主题所对应的主题名称，持久订阅时需设置此参数。



消息的同步和异步接收：消息的同步接收是指客户端主动去收取消息，客户端可以采用MessageConsumer的receive方法接收消息：

```java
Message message = consumer.receive();
Message message = consumer.receive(long timeout);
Message message = consumer.receiveNoWait();
```
消息的异步接收是指当消息到达时，ActiveMQ主动通知客户端，可以通过注册一个实现MessageListener接口的对象到MessageConsumer。MessageListener只有一个必须实现的方法——onMessage，它只接收一个参数，即Message。在为每个发送到Destination的消息实现onMessage时，将调用该方法。

下面看代码：

Producer：

```java
public class Producer {
    private ConnectionFactory factory;
    private Connection connection;
    private Session session;
    private MessageProducer producer;
    private Destination destination;

    public Producer() throws JMSException {
        this.factory = new ActiveMQConnectionFactory("hyy", "hyy", "tcp://192.168.1.104:61616");
        this.connection = factory.createConnection();
        this.connection.start();
        this.session = connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE);
        this.producer = session.createProducer(null);
        this.destination = session.createQueue("first");
    }

    public void send() throws JMSException {
        MapMessage msg1 = session.createMapMessage();
        msg1.setString("name", "张三");
        msg1.setInt("age", 21);
        //只有xxxProperty才对messageSelector起作用
        msg1.setStringProperty("color", "blue");
        msg1.setIntProperty("sal", 1800);

        MapMessage msg2 = session.createMapMessage();
        msg2.setString("name", "李四");
        msg2.setInt("age", 24);
        msg2.setStringProperty("color", "green");
        msg2.setIntProperty("sal", 1900);

        MapMessage msg3 = session.createMapMessage();
        msg3.setString("name", "王五");
        msg3.setInt("age", 27);
        msg3.setStringProperty("color", "yellow");
        msg3.setIntProperty("sal", 2300);

        MapMessage msg4 = session.createMapMessage();
        msg4.setString("name", "赵六");
        msg4.setInt("age", 30);
        msg4.setStringProperty("color", "red");
        msg4.setIntProperty("sal", 3000);

        producer.send(destination, msg1, DeliveryMode.NON_PERSISTENT, 2,1000 * 60 * 10L);
        producer.send(destination, msg2, DeliveryMode.NON_PERSISTENT, 5,1000 * 60 * 10L);
        producer.send(destination, msg3, DeliveryMode.NON_PERSISTENT, 3,1000 * 60 * 10L);
        producer.send(destination, msg4, DeliveryMode.NON_PERSISTENT, 9,1000 * 60 * 10L);
        connection.close();
    }

    public static void main(String[] args) throws JMSException {
        Producer producer = new Producer();
        producer.send();
    }
}
```
Consumer：
```java
public class Consumer {
    private ConnectionFactory factory;
    private Connection connection;
    private Session session;
    private MessageConsumer consumer;
    private Destination destination;
    private static final String SELECTOR_1 = "color = 'blue'";
    private static final String SELECTOR_2 = "sal > 2000";

    public Consumer() throws JMSException {
        this.factory = new ActiveMQConnectionFactory("hyy", "hyy", "tcp://192.168.1.104:61616");
        this.connection = factory.createConnection();
        this.connection.start();
        this.session = connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE);
        this.destination = session.createQueue("first");
        this.consumer = session.createConsumer(destination, SELECTOR_1);
    }

    public void receive() {
        try {
            consumer.setMessageListener(new Listener());
        } catch (JMSException e) {
            e.printStackTrace();
        }
    }

    class Listener implements MessageListener {

        public void onMessage(Message message) {
            if(message instanceof TextMessage) {

            }

            if(message instanceof MapMessage) {
                System.out.println(message.toString());
                try {
                    System.out.println("name:" + ((MapMessage) message).getString("name"));
                    System.out.println("age:" + ((MapMessage) message).getString("age"));
                } catch (JMSException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) throws JMSException {
        Consumer consumer = new Consumer();
        consumer.receive();
    }
}
```













