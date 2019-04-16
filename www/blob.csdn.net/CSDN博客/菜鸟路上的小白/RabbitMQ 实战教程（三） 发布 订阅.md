# RabbitMQ 实战教程（三） 发布/订阅 - 菜鸟路上的小白 - CSDN博客





2016年10月11日 15:59:07[somnus_小凯](https://me.csdn.net/u012486840)阅读数：757
个人分类：[rabbitmq](https://blog.csdn.net/u012486840/article/category/6457407)









在上一个教程中，我们实现了工作队列，一个任务只会发给一个Worker。在这一篇教程，我们将做一些完全不同的改变，我们将提供一个信息给多个消费者。这种模式被称为“发布/订阅”。

为了说明这种模式，我们将建立一个简单的日志系统。这个系统将由两个程序组成，第一个将发出日志消息，第二个将接收并处理日志消息。在我们的日志系统中，每一个运行的接收程序的副本都会收到日志消息。

## 交换器（Exchanges）

![](https://img-blog.csdn.net/20161011155820410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在上一个教程中，我们主要介绍了发送者和接受者，发送者发送消息给RabbitMQ队列，接收者从RabbitMQ队列获取消息。在这一篇教程，我们会引入转发器，展示RabbitMQ的完整的消息模型。

RabbitMQ消息模型的核心思想是，生产者不直接发送任何消息给队列。事实上，一般的情况下，生产者甚至不知道消息应该发送到哪些队列。相反的，生产者只能发送消息给转发器。转发器是非常简单的，一边接收从生产者发来的消息，另一边把消息推送到队列中。转发器必须知道消息如何处理它接收到的消息。是否被添加到一个特定的队列吗？是否应该追加到多个队列？或者是否应该被丢弃？这些规则通过转发器的类型进行定义。

下面列出一些可用的转发器类型： Direct、Topic、Headers、Fanout。目前我们关注最后一个fanout，声明转发器类型的代码：
- channel.exchangeDeclare("logs","fanout");

fanout类型转发器特别简单，把所有接受的消息，广播到所有它所知道的队列。不过这正是我们前述的日志系统所需要的。

在上一个教程中，我们不知道转发器，但仍然能够发送消息到队列。这是因为我们使用了一个默认的转发器，我们用空的字符串（“”）。

之前发送消息的代码
- channel.basicPublish("","hello",null, message.getBytes());

第一个参数表示转发器的名称，我们设置为””，第二个参数表示消息由routingKey决定发送到哪个队列。现在我们可以指定消息发送到的转发器
- channel.basicPublish("logs","",null, message.getBytes());

## 临时队列（Temporary queues）

之前，我们都是使用的队列指定了一个特定的名称，我们需要指出Worker到同一个队列。不过，对于我们的日志系统而言，我们并不关心队列的名称。我们想要接收到所有的消息，而且我们也只对当前正在传递的数据的感兴趣。为了满足我们的需求，需要做两件事。

首先，每当我们连接到RabbitMQ，我们需要一个新的空的队列。要做到这一点，我们可以创建一个随机名称的队列，或者，让服务器给我们提供一个随机的名称。

其次，一旦消费者与RabbitMQ断开，消费者所接收的那个队列应该被自动删除。

Java中我们可以使用queueDeclare()方法，不传递任何参数，来创建一个非持久的、唯一的、自动删除的队列，且队列名称由服务器随机产生。
- String queueName = channel.queueDeclare().getQueue();

## 绑定（Bindings）

![](https://img-blog.csdn.net/20161011155836101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们已经创建了一个fanout转发器和队列。我们现在需要通过绑定告诉转发器把消息发送给我们的队列。转发器和一个队列之间的关系被称为绑定。
- channel.queueBind(queueName,"logs","");

## 案例实战

![](https://img-blog.csdn.net/20161011155848757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

发送端，连接到RabbitMQ（此时服务需要启动），发送一条数据，然后退出。
- package com.lianggzone.rabbitmq.demo.publish;
- 
- import java.io.IOException;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- 
- publicclassEmitLog{
- privatestaticfinalString EXCHANGE_NAME ="logs";
- publicstaticvoid main(String[] args)throwsIOException,TimeoutException{
- // 创建连接
- ConnectionFactory factory =newConnectionFactory();
- // 设置MabbitMQ, 主机ip或者主机名  
-         factory.setHost("localhost");
- // 创建一个连接 
- Connection connection = factory.newConnection();
- // 创建一个通道 
- Channel channel = connection.createChannel();
- // 指定一个转发器
-         channel.exchangeDeclare(EXCHANGE_NAME,"fanout");
- // 发送消息  
- String message ="Liang-MSG log.";
-         channel.basicPublish(EXCHANGE_NAME,"",null, message.getBytes());
- System.out.println(" [x] Sent '"+ message +"'");
- // 关闭频道和连接  
-         channel.close();
-         connection.close();
- }
- }

接受端，不断等待服务器推送消息，然后在控制台输出。
- package com.lianggzone.rabbitmq.demo.publish;
- 
- import java.io.IOException;
- import java.util.concurrent.TimeUnit;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.AMQP;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- import com.rabbitmq.client.Consumer;
- import com.rabbitmq.client.DefaultConsumer;
- import com.rabbitmq.client.Envelope;
- 
- publicclassReceiveLogs{
- privatestaticfinalString EXCHANGE_NAME ="logs";
- publicstaticvoid main(String[] args)throwsIOException,TimeoutException{
- // 创建连接
- ConnectionFactory factory =newConnectionFactory();
- // 设置MabbitMQ, 主机ip或者主机名
-         factory.setHost("localhost");
- // 创建一个连接
- Connection connection = factory.newConnection();
- // 创建一个通道
- Channel channel = connection.createChannel();
- // 指定一个转发器
-         channel.exchangeDeclare(EXCHANGE_NAME,"fanout");
- // 创建一个非持久的、唯一的、自动删除的队列
- String queueName = channel.queueDeclare().getQueue();
- // 绑定转发器和队列
-         channel.queueBind(queueName, EXCHANGE_NAME,"");
- // 打印
- System.out.println(" [*] Waiting for messages. To exit press CTRL+C");
- // 创建队列消费者
- finalConsumer consumer =newDefaultConsumer(channel){
- @Override
- publicvoid handleDelivery(String consumerTag,Envelope envelope, AMQP.BasicProperties properties,
- byte[] body)throwsIOException{
- String message =newString(body,"UTF-8");
- System.out.println(" [x] Received '"+ message +"'");
- }
- };
-         channel.basicConsume(queueName,true, consumer);
- }
- }

首先，我们开启两个ReceiveLogs。然后，启动EmitLog进行消息发送。打印结果如下

ReceiveLogs1
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-MSG log.'

ReceiveLogs 2
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-MSG log.'

我们发现，ReceiveLogs1和ReceiveLogs2同时收到了消息。



