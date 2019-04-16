# RabbitMQ 实战教程（二） 工作队列 - 菜鸟路上的小白 - CSDN博客





2016年10月11日 15:56:48[somnus_小凯](https://me.csdn.net/u012486840)阅读数：1234








工作队列的主要任务：避免立刻执行资源密集型任务，然后必须等待其完成。相反地，我们进行任务调度，我们把任务封装为消息发送给队列。工作进行在后台运行并不断的从队列中取出任务然后执行。当你运行了多个工作进程时，任务队列中的任务将会被工作进程共享执行。

这样的概念在web应用中极其有用，当在很短的HTTP请求间需要执行复杂的任务。

![](https://img-blog.csdn.net/20161011155616240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 准备工作

我们使用Thread.sleep来模拟耗时的任务。现在，我们对上面“Hello World”的例子进行改造。

发送端
- package com.lianggzone.rabbitmq.demo.workqueues;
- 
- import java.io.IOException;
- import java.util.concurrent.TimeoutException;
- import com.rabbitmq.client.Channel;
- import com.rabbitmq.client.Connection;
- import com.rabbitmq.client.ConnectionFactory;
- 
- publicclassNewTask{
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
- for(int i =0; i <10; i++){
- String message ="Liang-"+ i;
-             channel.basicPublish("", QUEUE_NAME,null, message.getBytes());
- System.out.println(" [x] Sent '"+ message +"'");
- }
- // 关闭频道和连接  
-         channel.close();
-         connection.close();
- }
- }

接收端
- package com.lianggzone.rabbitmq.demo.workqueues;
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
- publicclassWorker{
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
- finalConsumer consumer =newDefaultConsumer(channel){
- @Override
- publicvoid handleDelivery(String consumerTag,Envelope envelope, AMQP.BasicProperties properties,
- byte[] body)throwsIOException{
- String message =newString(body,"UTF-8");
- 
- System.out.println(" [x] Received '"+ message +"'");
- try{
-                     doWork(message);
- }catch(InterruptedException e){
-                     e.printStackTrace();
- }
- }
- };
- // acknowledgment is covered below
- boolean autoAck =true;
-         channel.basicConsume(QUEUE_NAME, autoAck, consumer);
- }
- 
- privatestaticvoid doWork(String task)throwsInterruptedException{
- String[] taskArr = task.split("-");
- TimeUnit.SECONDS.sleep(Long.valueOf(taskArr[1]));
- }
- }

## 轮询调度（Round-robin dispatching）

任务队列的优点是能够很容易的进行并行工作。如果我们积压了很多工作，我们仅仅通过增加更多的workers就可以解决问题，使系统的伸缩性更加容易。

默认情况下，RabbitMQ将会发送的每一条消息按照顺序给下一个消费者。平均每一个消费者将获得相同数量的消息。这种分配方式称为轮询调度。值得注意的是，这种分配过程是一次性分配，并非一个一个分配。

首先，我们开启两个Worker。然后，启动NewTask进行消息发送。打印结果如下

Worker1
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-0'
- [x]Received'Liang-2'
- [x]Received'Liang-4'
- [x]Received'Liang-6'
- [x]Received'Liang-8'

Worker2
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-1'
- [x]Received'Liang-3'
- [x]Received'Liang-5'
- [x]Received'Liang-7'
- [x]Received'Liang-9'

再做一个实验，我们开启3个worker，然后，启动NewTask进行消息发送。打印结果如下

Worker1
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-0'
- [x]Received'Liang-3'
- [x]Received'Liang-6'
- [x]Received'Liang-9'

Worker2
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-1'
- [x]Received'Liang-4'
- [x]Received'Liang-7'

Worker3
- [*]Waitingfor messages.Toexit press CTRL+C
- [x]Received'Liang-2'
- [x]Received'Liang-5'
- [x]Received'Liang-8'

我们发现，通过增加更多的workers就可以进行并行工作，并且接受的消息平均分配。

## 消息应答（Message acknowledgment）

执行一个任务需要花费几秒钟。在执行任务时发生中断。上面例子，RabbItMQ一旦给消费者发送消息，会马上从内存中移除这个信息。在这种情况下，如果杀死正在执行任务的某个Worker，我们会丢失它正在处理的信息。我们也会丢失已经转发给这个工作者且它还未执行的消息。

我们不希望丢失任何消息。当某个Worker被杀死时，我们希望将任务传递给另一个Worker。

为了保证消息永远不会丢失，RabbitMQ支持消息应答。消费者发送应答给RabbitMQ，告诉它信息已经被接收和处理，然后RabbitMQ可以自由的进行信息删除。如果消费者被杀死而没有发送应答，RabbitMQ会认为该信息没有被完全的处理，然后将会重新转发给别的消费者。通过这种方式，你可以确认信息不会被丢失，即使消者被杀死。

这种机制并没有超时时间这么一说，RabbitMQ只有在消费者连接断开是重新转发此信息。如果消费者处理一个信息需要耗费特别特别长的时间是允许的。

消息应答默认是打开的。上面的代码中我们通过显示的设置autoAsk=true关闭了这种机制。下面我们修改代码Worker.java。主要修改两个地方。
- // 打开应答机制
- boolean ack =false;
- channel.basicConsume(QUEUE_NAME, ack, consumer);
- //每次处理完成一个消息后，手动发送一次应答。  
- channel.basicAck(delivery.getEnvelope().getDeliveryTag(),false);

此时，读者，可以做一个实验，我们开启3个worker，然后，启动NewTask进行消息发送。立即关闭一个worker，观察输出结果，是否重新转发关闭的worker的消息。

## 消息持久化（Message durability）

我们已经学会了如何确保即使消费者死了，消息也不会丢失。但是如果此时RabbitMQ服务被停止，我们的消息仍然会丢失。当RabbitMQ退出或者异常退出，将会丢失所有的队列和信息，除非你告诉它不要丢失。为了保证消息在服务停止的时候，不丢失，我们要考虑保证消息的持久化。

首先，我们需要确认RabbitMQ永远不会丢失我们的队列。为此，我们需要声明它为持久化的。
- boolean durable =true;
- channel.queueDeclare("task_queue", durable,false,false,null);

其次，我们需要标识我们的信息为持久化的。通过设置MessageProperties值为PERSISTENT_TEXT_PLAIN。
- channel.basicPublish("","task_queue",
- MessageProperties.PERSISTENT_TEXT_PLAIN,
-             message.getBytes());

RabbitMQ不允许使用不同的参数重新定义一个队列，所以已经存在的队列，我们无法修改其属性。

此时，读者，可以做一个实验，我们启动NewTask进行消息发送，然后RabbitMQ关闭服务。再重新启动服务abbitMQ，开启1个worker，观察输出结果，是否worker的接受到之前NewTask发送消息。

## 公平转发（Fair dispatch）

你可能已经注意到，目前的消息调度并非是我们想要的。例如，这样一种情况下，一个消费者将一直繁忙，另一个消费者却很快执行完任务后等待，没有任何工作。造成这样的原因，是因为RabbitMQ仅仅是当消息到达队列后进行转发消息，并不在乎有多少任务。

为了解决这样的问题，我们可以使用basicQos方法，传递参数为prefetchCount = 1。这样告诉RabbitMQ不要在同一时间给一个消费者超过一条消息。换句话说，只有在消费者空闲的时候会发送下一条信息。
- int prefetchCount =1;
- channel.basicQos(prefetchCount);



