# ActiveMQ——1、JMS简介与ActiveMQ入门 - 隔壁老王的专栏 - CSDN博客





2016年12月24日 18:20:58[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：930








**1.背景&JMS概述**

当前CORBA、DCOM、RMI等RPC中间件技术已广泛应用于各个领域。但是面对规模和复杂度都越来越高的分布式系统，这些技术也显示出其局限性：

（1）同步通信：客户端发出调用后，必须等待服务对象完成处理并返回结果后才能继续执行；

（2）客户端和服务对象的生命周期紧密耦合：客户进程和服务对象进程都必须正常运行，如果由于服务对象崩溃或者网络故障导致客户的请求不可达，客户端会收到异常；

（3）点对点通信：客户端的一次调用只发送给某个单独的目标对象。

面向消息的中间件（Message Oriented Middleware，MOM）较好的解决了以上问题。发送者将消息发送给消息服务器，消息服务器将消息存放在若干队列中，在合适的时候再将消息发送给接收者。**这种模式下，发送和接收是异步的，发送者无需等待；二者的生命周期未必相同：发送消息的时候接收者不一定运行，接收消息的时候发送者也不一定运行；一对多通信：一个消息可以有多个接收者。**

JAVA消息服务（JMS）定义了JAVA中访问消息中间件的接口。JMS只是接口，并没有给予实现，实现JMS接口的消息中间件称为JMS Provider，已有的MOM系统包括Apache的ActiveMQ、以及阿里巴巴的RocketMQ、IBM的MQSeries、Microsoft的MSMQ和BEA的MessageQ、RabbitMQ等等，他们基本都遵循JMS规范。

**2.JMS术语**

Provider（MessageProvider）：生产者

由会话创建的对象，用于发送消息到目标。用户可以创建某个目标的发送者，也可以创建一个通用的发送者，在发送时指定目标。

Consumer（MessageConsumer）：消费者

由会话创建的对象，用于接收发送到目标的消息。消费者可以同步的（阻塞模式）或异步的（非阻塞模式）接收队列或主题类型的消息。

PTP（Point to Point）：点对点消息模型

Pub/Sub（Publish/Subscribe）：发布/订阅消息模型

Queue：队列目标

Topic：主题目标

ConnectionFactory：连接工厂，JMS用它创建连接

用户用来创建到JMS提供者的连接的被管对象。JMS客户端通过可移植的接口访问连接，这样当下层的实现改变时，客户端代码不需要进行修改。管理员需要在JNDI命名空间中配置连接工厂，这样JMS客户端才能查找到他们。根据消息类型的不同，用户可以使用队列连接工厂或主题连接工厂。

Connection：JMS客户端到JMS Provider的连接

连接代表了应用程序和消息服务器之间的通信链路。在获得连接工厂后，就可以创建一个与JMS提供者通信的连接。根据不同的连接类型，连接允许用户创建会话，以发送和接收队列和主题到目标。

Destination：消息的目的地

目标是一个包装了消息目标标识符的被管对象，消息目标是指消息发布和接收的地点，可以是队列或主题。JMS管理员创建这些对象，然后用户通过JNDI发现它们。和连接工厂一样，管理员可以创建两种类型的消息目标：点对点模型的队列以及发布者/订阅者模型的主题。

Session：会话，一个发送或接收消息的线程

**表示一个单线程的上下文，用于发送和接收消息。由于会话是单线程的，所以消息是连续的，就是说消息是按照发送的顺序一个一个接收的。会话的好处是支持事务，如果用户选择了事务支持，会话上下文将保存一组消息，直到事务被提交才发送这些消息。在提交事务之前，用户可以使用回滚操作来取消这些消息。会话允许用户创建消息生产者来发送消息，创建消息消费者来接收消息。**

Message：消息

在消费者和生产者之间传递的对象，也就是说从一个应用程序创建并发送到另一个应用程序。一个消息有三个主要部分：

①消息头（必须）：包含用于识别和为消息寻找路由的操作设置。

②一组消息属性（可选）：包含额外的属性，支持其他提供者和用户的兼容。可以创建定制的字段和过滤器（消息选择器）。

③一个消息体（可选）：允许用户创建五种类型的消息，文本消息、映射消息、字节消息、流消息和对象消息。

**3.消息格式定义**

**JMS定义了五种不同的消息正文格式，以及调用的消息类型，允许发送并接收一些不同形式的数据，提供现有消息格式的一些级别的兼容性。**

①StreamMessage：JAVA原始值的数据流

②MapMessage：套key-value对

③TextMessage：字符串对象

④ObjectMessage：序列化的Java对象

⑤BytesMessage：字节数组对象

**4.ActiveMQ简介**

ActiveMQ是Apache出品，最流行的，能力强大的开源消息总线。ActiveMQ是一个完全支持JMS 1.1和J2EE 1.4规范的JMS Provider实现，尽管JMS规范出台已经很久了，但JMS在当今的JAVAEE应用中仍然扮演着特殊的地位，可以说ActiveMQ在业界应用最广泛，当然如果想要有更强大的性能和海量数据处理能力，ActiveMQ还需要不断的升级版。80%以上的业务使用ActiveMQ已经足够满足需求，当然后续如天猫、淘宝这种大型的电商网站，尤其是双11这种特殊时期，ActiveMQ需要进行很复杂的优化源码以及架构设计才能完成，ActiveMQ是阿里巴巴的RocketMQ的核心，所以必须要掌握好ActiveMQ。

**5.安装ActiveMQ**

到ActiveMQ官网下载Linux版本的压缩包，我这里使用的是5.14.3。

tar -zxvf apache-activemq-5.14.3-bin.tar.gz，解压ActiveMQ压缩包。

cd apache-activemq-5.14.3/bin，进入ActiveMQ的bin目录

./activemq start，启动ActiveMQ

在浏览器中输入192.168.1.102:8161（你自己安装ActiveMQ机器的ip，8161是ActiveMQ的默认端口号，用户名和密码默认都为admin）打开ActiveMQ管控台，界面如下：

![](https://img-blog.csdn.net/20161224195436099)


**6.使用Java操作ActiveMQ（HelloWorld入门）**

Producer（生产者）：



```java
public class Producer {
    public static void main(String[] args) throws JMSException {
        //1.建立ConnectionFactory工厂对象，需要填入用户名、密码以及ActiveMQ地址，默认端口为tcp://localhost:61616
        ConnectionFactory factory = new ActiveMQConnectionFactory(ActiveMQConnectionFactory.DEFAULT_USER,
                ActiveMQConnectionFactory.DEFAULT_PASSWORD, "tcp://192.168.1.104:61616");
        //2.通过ConnectionFactory工厂对象创建Connection连接，并调用Connection对象的start方法开启连接，默认为关闭状态
        Connection connection = factory.createConnection();
        //3.通过Connection对象创建Session会话（上下文环境对象），用于接收和发送消息，参数一：是否开启事务，参数二：签收模式，一般设置为自动签收
        Session session = connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE);
        //4.通过Session对象创建Destination对象，用来指定生产消息和消费消息来源的对象，在PTP中，Destination被称为Queue（队列），
        //在Publish/Subscribe模式，Destination被称为Topic（主题）。程序中可以有多个Queue和Topic。
        Destination destination = session.createQueue("queue1");
        //5.通过Session对象创建消息的发送和接收对象（生产者和消费者）MessageProducer和MessageConsumer
        MessageProducer producer = session.createProducer(destination);
        //6.使用MessageProducer的setDeliveryMode方法设置持久化特性或非持久化。
        producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);
        //7.使用JMS规范的TextMessage形式创建数据（通过Session对象创建），并用MessageProducer的send方法发送数据。客户端使用receive方法接收数据
        TextMessage message = new ActiveMQTextMessage();
        for(int i=0; i<5; i++) {
            message.setText("消息：" + i);
            producer.send(destination, message);
        }
        //8.关闭Connection
        if(connection != null) {
            connection.close();
        }
    }
}
```
每一行代码都有注释，我在这里就不解释了，我会在后面对ActiveMQ的API进行详细的说明。执行代码后，ActiveMQ的管控台界面如下：



![](https://img-blog.csdn.net/20161224220830348)


Consumer（消费者）：

```java
public class Consumer {
    public static void main(String[] args) throws JMSException {
        //1.建立ConnectionFactory工厂对象，需要填入用户名、密码以及ActiveMQ地址，默认端口为tcp://localhost:61616
        ConnectionFactory factory = new ActiveMQConnectionFactory(ActiveMQConnectionFactory.DEFAULT_USER, ActiveMQConnectionFactory.DEFAULT_PASSWORD,
                "tcp://192.168.1.104:61616");
        //2.通过ConnectionFactory工厂对象创建Connection连接，并调用Connection对象的start方法开启连接，默认为关闭状态
        Connection connection = factory.createConnection();
        connection.start();
        //3.通过Connection对象创建Session会话（上下文环境对象），用于接收和发送消息，参数一：是否开启事务，参数二：签收模式，一般设置为自动签收
        Session session = connection.createSession(Boolean.FALSE, Session.AUTO_ACKNOWLEDGE);
        //4.通过Session对象创建Destination对象，用来指定生产消息和消费消息来源的对象，在PTP中，Destination被称为Queue（队列），
        //在Publish/Subscribe模式，Destination被称为Topic（主题）。程序中可以有多个Queue和Topic。
        Destination destination = session.createQueue("queue1"); //这里的名字需要与生产者的一样
        //5.通过Session对象创建消息的发送和接收对象（生产者和消费者）MessageProducer和MessageConsumer
        MessageConsumer consumer = session.createConsumer(destination);

        while (true) {//循环的监听，一有消息就处理
            //该方法会阻塞，直到接收到消息，还有receive(long l)阻塞指定的时间，receiveNoWait()不阻塞
            TextMessage message = (TextMessage) consumer.receive();
            if(message == null)
                break;
            System.out.println("接收到：" + message.getText());
        }

        if(connection != null) {
            connection.close();
        }
    }
}
```
执行Consumer，控制台打印：



![](https://img-blog.csdn.net/20161224222040130)


ActiveMQ管控台：消息已被消费

![](https://img-blog.csdn.net/20161224222155134)


**7.ActiveMQ的安全验证机制**

修改conf目录下的activemq.xml文件，在123行后加入如下代码：

![](https://img-blog.csdn.net/20161224223829547)


配置完成后，重启ActiveMQ，重新运行Producer生产者，控制台会报如下错误：![](https://img-blog.csdn.net/20161224223938469)

需要修改在创建ConnectionFactory时传入的用户名和密码：

```java
ConnectionFactory factory = new ActiveMQConnectionFactory("hyy",
                "hyy", "tcp://192.168.1.104:61616");
```

**8.Publish/Subscribe模式**

**发布者：**
****
```java
public class TopicPublisher {  
    public static void main(String[] args) throws JMSException {  
        ActiveMQConnectionFactory factory = new ActiveMQConnectionFactory("tcp://localhost:61616");  
        Connection connection = factory.createConnection();  
        connection.start();  
          
        Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);  
        Topic topic = session.createTopic("myTopic.messages");  
  
        MessageProducer producer = session.createProducer(topic);  
        producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);  
  
        while(true) {  
            TextMessage message = session.createTextMessage();  
            message.setText("message_" + System.currentTimeMillis());  
            producer.send(message);  
            System.out.println("Sent message: " + message.getText());  
  
            try {  
                Thread.sleep(1000);  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
        }  
  
//      session.close();  
//      connection.stop();  
//      connection.close();  
    }  
}
```

**订阅者：**

****

```java
public class TopicSubscriber {  
    public static void main(String[] args) throws JMSException {  
        ActiveMQConnectionFactory factory = new ActiveMQConnectionFactory("tcp://localhost:61616");  
        Connection connection = factory.createConnection();  
        connection.start();  
          
        Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);  
        Topic topic = session.createTopic("myTopic.messages");  
  
        MessageConsumer consumer = session.createConsumer(topic);  
        consumer.setMessageListener(new MessageListener() {  
            public void onMessage(Message message) {  
                TextMessage tm = (TextMessage) message;  
                try {  
                    System.out.println("Received message: " + tm.getText());  
                } catch (JMSException e) {  
                    e.printStackTrace();  
                }  
            }  
        });  
//      session.close();  
//      connection.stop();  
//      connection.close();  
    }  
}
```








