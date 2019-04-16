# RabbitMQ 实战教程（五） 主题 - 菜鸟路上的小白 - CSDN博客





2016年10月11日 16:07:48[somnus_小凯](https://me.csdn.net/u012486840)阅读数：715








虽然使用direct类型的转发器，改善了我们的日志系统。但是仍然存在一些局限性：它不能够基于多重条件进行路由选择。我们有可能希望不仅根据日志的级别，而且想根据日志的来源进行订阅。为了在我们的系统中实现上述的需求，我们需要了解一个更复杂的转发器：topic类型的转发器。

## 主题转发（Topic exchange）

使用topic类型的转发器，不能随意的设置选择键（routing_key），必须是由点隔开的一系列的标识符组成。标识符可以是任何东西，但是一般都与消息的某些特性相关。你可以定义任何数量的标识符，上限为255个字节。

绑定键和选择键的形式一样。topic类型的转发器和direct类型的转发器很类似，一个附带特殊的选择键将会被转发到绑定键与之匹配的队列中。需要注意的是：关于绑定键有两种特殊的情况。
- *可以匹配一个标识符。
- # 可以匹配0个或多个标识符。

topic类型的转发器非常强大，可以实现其他类型的转发器。当一个队列与绑定键#绑定，将会收到所有的消息，类似fanout类型的转发器。当绑定键中不包含任何#与*时，类似direct类型的转发器。

通过下图，我们大概可以了解到topic类型的转发器的处理流程。
![](https://img-blog.csdn.net/20161011160731345?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 案例实战

发送端，连接到RabbitMQ（此时服务需要启动），发送一条数据，然后退出。
- package com.lianggzone.rabbitmq.demo.topics;
- 
- import java.io.IOException;
- import java.util.UUID;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- 
- publicclassEmitLogTopic{
- privatestaticfinalString EXCHANGE_NAME ="topic_logs";
- privatestaticfinalString[] LOG_LEVEL_ARR ={"dao.debug","dao.info","dao.error",
- "service.debug","service.info",
- "service.error","controller.debug",
- "controller.info","controller.error"};
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
-         channel.exchangeDeclare(EXCHANGE_NAME,"topic");
- // 发送消息  
- for(String severity : LOG_LEVEL_ARR){
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
- package com.lianggzone.rabbitmq.demo.topics;
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
- publicclassReceiveLogsTopic{
- privatestaticfinalString EXCHANGE_NAME ="topic_logs";
- privatestaticfinalString[] LOG_LEVEL_ARR ={"#","dao.error","*.error","dao.*","service.#","*.controller.#"};
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
-         channel.exchangeDeclare(EXCHANGE_NAME,"topic");
- // 设置日志级别
- int rand =newRandom().nextInt(5);
- String severity  = LOG_LEVEL_ARR[rand];
- // 创建一个非持久的、唯一的、自动删除的队列
- String queueName = channel.queueDeclare().getQueue();
- // 绑定转发器和队列
-         channel.queueBind(queueName, EXCHANGE_NAME, severity);
- // 打印
- System.out.println(" [*] Waiting for messages. To exit press CTRL+C");
- System.out.println(" [*] LOG INFO : "+ severity);
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

我们多开几个ReceiveLogsTopic。然后，启动EmitLogTopic进行消息发送。打印结果如下

ReceiveLogsTopic1
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG INFO : dao.error
- [x]Received'Liang-MSG log : [dao.error]041cd8ba-df7d-4d20-a11f-ba21a0c2a02a'

ReceiveLogsTopic2
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG INFO :*.error
- [x]Received'Liang-MSG log : [dao.error]041cd8ba-df7d-4d20-a11f-ba21a0c2a02a'
- [x]Received'Liang-MSG log : [service.error]e3565f12-9782-4c22-a91c-f513f31b037d'
- [x]Received'Liang-MSG log : [controller.error]4436101a-3346-41f6-a9af-b8a4fbda451e'

ReceiveLogsTopic3
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG INFO :#
- [x]Received'Liang-MSG log : [dao.debug]4eb08245-2c05-490b-a5a5-2742cb70d831'
- [x]Received'Liang-MSG log : [dao.info]e9d4073b-1e61-4c6f-b531-ac42eaa346af'
- [x]Received'Liang-MSG log : [dao.error]041cd8ba-df7d-4d20-a11f-ba21a0c2a02a'
- [x]Received'Liang-MSG log : [service.debug]0ec84cbf-47ab-4813-a5db-e57d5e78830e'
- [x]Received'Liang-MSG log : [service.info]2e12e1b7-7a09-4eb7-8ad1-8e53f533121c'
- [x]Received'Liang-MSG log : [service.error]e3565f12-9782-4c22-a91c-f513f31b037d'
- [x]Received'Liang-MSG log : [controller.debug]94e5be72-15f6-496d-84f3-2a107bafc92b'
- [x]Received'Liang-MSG log : [controller.info]62bbe378-617d-4214-beb4-98cc53e73272'
- [x]Received'Liang-MSG log : [controller.error]4436101a-3346-41f6-a9af-b8a4fbda451e'

ReceiveLogsTopic4
- [*]Waitingfor messages.Toexit press CTRL+C
- [*] LOG INFO : service.#
- [x]Received'Liang-MSG log : [service.debug]0ec84cbf-47ab-4813-a5db-e57d5e78830e'
- [x]Received'Liang-MSG log : [service.info]2e12e1b7-7a09-4eb7-8ad1-8e53f533121c'
- [x]Received'Liang-MSG log : [service.error]e3565f12-9782-4c22-a91c-f513f31b037d'

我们发现，ReceiveLogsTopic1、ReceiveLogsTopic2、ReceiveLogsTopic3、ReceiveLogsTopic4同时收到了属于自己匹配的消息。尤其是ReceiveLogsTopic1类似于direct类型的转发器，ReceiveLogsTopic3通过“#”匹配到所有消息。



