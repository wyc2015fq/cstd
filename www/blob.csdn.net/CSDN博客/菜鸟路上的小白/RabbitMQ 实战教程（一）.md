# RabbitMQ 实战教程（一） - 菜鸟路上的小白 - CSDN博客





2016年10月11日 15:55:11[somnus_小凯](https://me.csdn.net/u012486840)阅读数：6939








MQ是消费-生产者模型的一个典型的代表，一端往消息队列中不断写入消息，而另一端则可以读取或者订阅队列中的消息。RabbitMQ是信息传输的中间者。本质上，他从生产者接收消息，转发这些消息给消费者。换句话说，RabbitMQ能够按根据你指定的规则进行消息转发、缓冲、和持久化。

在项目中，将一些无需即时返回且耗时的操作提取出来，进行了异步处理，而这种异步处理的方式大大的节省了服务器的请求响应时间，从而提高了系统的吞吐量。

## 常见术语

producer（生产者），consumer（消费者），broker（RabbitMQ服务）并不需要部署在同一台机器上，实际上在大多数实际的应用中，也不会部署在同一台机器上。

### 生产者（Producer）

一个发送消息的程序是一个producer(生产者)。

一般用下图表示生产者：
![](https://img-blog.csdn.net/20161011155430177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 队列（Queue）

队列类似邮箱。依存于RabbitMQ内部。虽然消息通过RabbitMQ在你的应用中传递，但是它们只能存储在队列中。队列不受任何限制，可以存储任何数量的消息—本质上是一个无限制的缓存。不同的生产者可以通过同一个队列发送消息，此外，不同的消费者也可以从同一个队列上接收消息。

一般用下图表示队列：
![](https://img-blog.csdn.net/20161011155411927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 消费者（Consumer）

消费者属于等待接收消息的程序。
一般使用下图表示消费者：
![](https://img-blog.csdn.net/20161011155446218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 案例实战

![](https://img-blog.csdn.net/20161011155459990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 准备工作

使用Maven进行管理
- <dependency>
- <groupId>com.rabbitmq</groupId>
- <artifactId>amqp-client</artifactId>
- <version>3.6.3</version>
- </dependency>

### 发送端

发送端，连接到RabbitMQ（此时服务需要启动），发送一条数据，然后退出。
- package com.lianggzone.rabbitmq.demo.helloworld;
- 
- import java.io.IOException;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- 
- publicclassSend{
- privatefinalstaticString QUEUE_NAME ="hello";
- publicstaticvoid main(String[] args)throwsIOException,TimeoutException{
- // 创建连接
- ConnectionFactory factory =newConnectionFactory();
- // 设置MabbitMQ, 主机ip或者主机名  
-         factory.setHost("localhost");
- // 创建一个连接 
- Connection connection = factory.newConnection();
- // 创建一个通道 
- Channel channel = connection.createChannel();
- // 指定一个队列
-         channel.queueDeclare(QUEUE_NAME,false,false,false,null);
- // 发送消息  
- String message ="Hello World!";
-         channel.basicPublish("", QUEUE_NAME,null, message.getBytes());
- System.out.println(" [x] Sent '"+ message +"'");
- // 关闭频道和连接  
-         channel.close();
-         connection.close();
- }
- }

### 接受端

接受端，不断等待服务器推送消息，然后在控制台输出。
- package com.lianggzone.rabbitmq.demo.helloworld;
- 
- import java.io.IOException;
- import java.util.concurrent.TimeoutException;
- 
- import com.rabbitmq.client.AMQP;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- import com.rabbitmq.client.Consumer;
- import com.rabbitmq.client.DefaultConsumer;
- import com.rabbitmq.client.Envelope;
- 
- publicclassRecv{
- privatefinalstaticString QUEUE_NAME ="hello";
- publicstaticvoid main(String[] args)throwsIOException,TimeoutException{
- // 创建连接
- ConnectionFactory factory =newConnectionFactory();
- // 设置MabbitMQ, 主机ip或者主机名
-         factory.setHost("localhost");
- // 创建一个连接
- Connection connection = factory.newConnection();
- // 创建一个通道
- Channel channel = connection.createChannel();
- // 指定一个队列
-         channel.queueDeclare(QUEUE_NAME,false,false,false,null);
- System.out.println(" [*] Waiting for messages. To exit press CTRL+C");
- // 创建队列消费者
- Consumer consumer =newDefaultConsumer(channel){
- @Override
- publicvoid handleDelivery(String consumerTag,Envelope envelope, AMQP.BasicProperties properties,
- byte[] body)throwsIOException{
- String message =newString(body,"UTF-8");
- System.out.println(" [x] Received '"+ message +"'");
- }
- };
-         channel.basicConsume(QUEUE_NAME,true, consumer);
- }
- }



