# RabbitMQ 实战教程（四） 路由 - 菜鸟路上的小白 - CSDN博客





2016年10月11日 16:06:03[somnus_小凯](https://me.csdn.net/u012486840)阅读数：741








在本教程中，我们将添加一个功能，让日志接收者能够订阅部分消息。例如，我们将能够直接将错误写入日志文件（以节省磁盘空间），仍然能够在控制台上打印所有的日志消息。

## 绑定（Bindings）

在上一个教程中，我们已经使用过绑定。类似下面的代码
- channel.queueBind(queueName, EXCHANGE_NAME,"");

绑定表示转发器与队列之间的关系。我们也可以简单的认为：队列对该转发器上的消息感兴趣。

绑定可以附带一个额外的参数routingKey。为了与避免basicPublish方法（发布消息的方法）的参数混淆，我们准备把它称作绑定键（binding key） 。下面展示如何使用绑定键（binding key）来创建一个绑定。
- channel.queueBind(queueName, EXCHANGE_NAME,"black");

绑定键的意义依赖于转发器的类型。对于fanout类型，忽略此参数。

## 直接转发（Direct exchange）

在上一个教程中，日志系统广播所有的消息给所有的消费者。现在，我们可能希望把错误日志写入硬盘，而不把硬盘空间浪费在警告或者消息类型的日志上。之前我们使用fanout类型的转发器，这并没有给我们太多的灵活性，它只能盲目的广播。
![](https://img-blog.csdn.net/20161011160522431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

所以，我们将会使用direct类型的转发器进行替代。direct类型的转发器背后的路由转发算法很简单：消息会被推送至绑定键（binding key）和消息发布附带的选择键（routing key）完全匹配的队列。

我们可以看到direct类型的转发器与两个队列绑定。第一个队列与绑定键orange绑定，第二个绑定有两个绑定，一个与绑定键black绑定，另一个与green绑定键绑定。在这样的设置的情况下，当一个消息附带一个选择键（routing key） orange发布至转发器将会被导向到队列Q1。消息附带一个选择键（routing key）black或者green将会被导向到队列Q2。所有其他消息将被丢弃。

## 多重绑定（Multiple bindings）

![](https://img-blog.csdn.net/20161011160536275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

使用一个绑定键（binding key）绑定多个队列是完全合法的。在这种情况下，一个附带选择键（routing key）的消息将会被转发到Q1和Q2。

## 发送日志（Emitting logs）

我们准备将这种模式用于我们的日志系统。我们将消息发送到direct类型的转发器而不是fanout类型。我们将把日志的严重性作为选择键（routing key） 。这样的话，接收程序可以根据严重性来选择接收。我们首先关注发送日志的代码：
- channel.exchangeDeclare(EXCHANGE_NAME,"direct");

现在，我们准备发送一条消息
- channel.basicPublish(EXCHANGE_NAME, severity,null, message.getBytes());

为了简化代码，我们假定‘severity’是‘info’，‘warning’，‘error’中的一个。

## 订阅（Subscribing）

接收消息将工作就像在以前的教程中，只有一点不同，我们给我们所感兴趣的严重性类型的日志创建一个绑定。
- String queueName = channel.queueDeclare().getQueue();
- for(String severity : argv){
-   channel.queueBind(queueName, EXCHANGE_NAME, severity);
- }

## 案例实战

![](https://img-blog.csdn.net/20161011160551217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

发送端，连接到RabbitMQ（此时服务需要启动），发送一条数据，然后退出。
- package com.lianggzone.rabbitmq.demo.routing;
- 
- import java.io.IOException;
- import java.util.Random;
- import java.util.UUID;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- 
- publicclassEmitLogDirect{
- privatestaticfinalString EXCHANGE_NAME ="direct_logs";
- privatestaticfinalString[] LOG_LEVEL_ARR ={"debug","info","error"};
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
-         channel.exchangeDeclare(EXCHANGE_NAME,"direct");
- // 发送消息  
- for(int i =0; i <10; i++){
- int rand =newRandom().nextInt(3);
- String severity  = LOG_LEVEL_ARR[rand];
- String message ="Liang-MSG log : ["+severity+"]"+ UUID.randomUUID().toString();
- // 发布消息至转发器 
-             channel.basicPublish(EXCHANGE_NAME, severity,null, message.getBytes());
- System.out.println(" [x] Sent '"+ message +"'");
- }
- // 关闭频道和连接  
-         channel.close();
-         connection.close();
- }
- }

接受端，不断等待服务器推送消息，然后在控制台输出。
- package com.lianggzone.rabbitmq.demo.routing;
- 
- import java.io.IOException;
- import java.util.Random;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.AMQP;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- import com.rabbitmq.client.Consumer;
- import com.rabbitmq.client.DefaultConsumer;
- import com.rabbitmq.client.Envelope;
- 
- publicclassReceiveLogsDirect{
- privatestaticfinalString EXCHANGE_NAME ="direct_logs";
- privatestaticfinalString[] LOG_LEVEL_ARR ={"debug","info","error"};
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
-         channel.exchangeDeclare(EXCHANGE_NAME,"direct");
- // 设置日志级别
- int rand =newRandom().nextInt(3);
- String severity  = LOG_LEVEL_ARR[rand];
- // 创建一个非持久的、唯一的、自动删除的队列
- String queueName = channel.queueDeclare().getQueue();
- // 绑定转发器和队列
-         channel.queueBind(queueName, EXCHANGE_NAME, severity);
- // 打印
- System.out.println(" [*] Waiting for messages. To exit press CTRL+C");
- System.out.println(" [*] LOG LEVEL : "+ severity);
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

首先，我们开启三个ReceiveLogsDirect。然后，启动EmitLogDirect进行消息发送。打印结果如下

ReceiveLogsDirect1
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG LEVEL : info
- [x]Received'Liang-MSG log : [info]0dd0ae0c-bf74-4aa9-9852-394e65fbf939'
- [x]Received'Liang-MSG log : [info]b2b032f6-e907-4c95-b676-1790204c5f73'
- [x]Received'Liang-MSG log : [info]14482461-e432-4866-9eb5-a28f4edeb47f'

ReceiveLogsDirect2
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG LEVEL : error
- [x]Received'Liang-MSG log : [error]493dce2a-7ce1-4111-953c-99ab2564a2d0'
- [x]Received'Liang-MSG log : [error]2446dd80-d5f0-4d39-888f-31579b9d2724'
- [x]Received'Liang-MSG log : [error]fe8219e0-5548-40ba-9810-d922d1b03dd8'
- [x]Received'Liang-MSG log : [error]797b6d0e-9928-4505-9c76-56043322b1f0'

ReceiveLogsDirect3
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG LEVEL : debug
- [x]Received'Liang-MSG log : [debug]c05eee3e-b820-4b69-9c3f-c2bbded85195'
- [x]Received'Liang-MSG log : [debug]4645c9ba-4070-41d7-adc9-7f8b2df1e3c8'
- [x]Received'Liang-MSG log : [debug]d3d3ad5c-8f97-49ea-8fd6-c434790e40eb'

我们发现，ReceiveLogsDirect1、ReceiveLogsDirect2、ReceiveLogsDirect3同时收到了属于自己级别的消息。



