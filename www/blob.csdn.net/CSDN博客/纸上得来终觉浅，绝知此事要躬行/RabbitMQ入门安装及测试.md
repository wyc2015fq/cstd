# RabbitMQ入门安装及测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月16日 16:41:23[boonya](https://me.csdn.net/boonya)阅读数：12537








### 1.RabbitMQ是什么？

 
 MQ全称为Message Queue, [消息队列](http://baike.baidu.com/view/262473.htm)（MQ）是一种[应用程序](http://baike.baidu.com/view/330120.htm)对应用程序的通信方法。[应用程序](http://baike.baidu.com/view/330120.htm)通过读写出入队列的消息（针对应用程序的数据）来通信，而无需专用连接来链接它们。消
 息传递指的是程序之间通过在消息中发送数据进行通信，而不是通过直接调用彼此来通信，直接调用通常是用于诸如[远程过程调用](http://baike.baidu.com/view/431455.htm)的技术。排队指的是[应用程序](http://baike.baidu.com/view/330120.htm)通过
 队列来通信。[队列](http://baike.baidu.com/view/38959.htm)的使用除去了接收和发送[应用程序](http://baike.baidu.com/view/330120.htm)同时执行的要求。其中较为成熟的MQ产品有IBM
 WEBSPHERE MQ。[MQ](http://baike.baidu.com/view/732119.htm)是消费-生产者模型的一个典型的代表，一端往[消息队列](http://baike.baidu.com/view/262473.htm)中不断写入消息，而另一端则可以读取或者订阅队列中的消息。MQ和[JMS](http://baike.baidu.com/view/157103.htm)类似，但不同的是JMS是SUN
 JAVA[消息中间件](http://baike.baidu.com/view/3118541.htm)服务的一个标准和API定义，而MQ则是遵循了AMQP协议的具体实现和产品。RabbitMQ是一个在AMQP基础上完整的，可复用的企业消息系统。他遵循Mozilla
 Public License[开源协议](http://baike.baidu.com/view/1373538.htm)。




### 2.RabbitMQ环境安装

#### 下载安装文件



1）下载erlang支持包：[http://www.erlang.org/download.html](http://www.erlang.org/download.html)如图所示：

![](https://img-blog.csdn.net/20140716160704906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2）下载rabbitmq server：[http://www.rabbitmq.com/releases/rabbitmq-server/](http://www.rabbitmq.com/releases/rabbitmq-server/)

![](https://img-blog.csdn.net/20140716162018724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


下载RabbitMQ client：[http://www.rabbitmq.com/releases/rabbitmq-java-client/](http://www.rabbitmq.com/releases/rabbitmq-java-client/)或者在[http://www.rabbitmq.com/java-client.html](http://www.rabbitmq.com/java-client.html)

![](https://img-blog.csdn.net/20140716162053575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：由于RabbitMQ
 支持erlang、Java和.NET你可以找到不同语言的client支持版本。



#### 配置环境变量 



1）、新建变量：ERLANG_HOME=D:\Program Files\erl6.1，然后再path中添加%ERLANG_HOME%\bin;

2）、新建变量：RABBITMQ_SERVER=D:\Program
 Files\rabbitmq_server-3.3.4，然后再path中添加%RABBITMQ_SERVER%\sbin;

3）、运行sbin/rabbitmq-server.bat,启动RabbitMQ服务器

![](https://img-blog.csdn.net/20140716163026283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 RabbitMQ服务端是用AMPQ协议的， 而客户端支持多种语言（Java, .NET,Erlang......Maybe more........）。下面我们准备用java来写一个‘hello world’，测试RabbitMQ安装。 


### 3.RabbitMQ客户端测试

将以下两个Java类放入Java项目中测试：

#### 接收端代码


```java
package com.boonya.rabbitmq;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
import com.rabbitmq.client.QueueingConsumer;
/**
 * 
 * @author        boonya
 *
 * @having-line---------------------------------------------------------
 * @filename      Receive.java
 * @function      TODO
 * @download      <a href='http://www.rabbitmq.com/download.html'>http://www.rabbitmq.com/download.html</a>
 * @start-at      2014年7月16日,下午2:50:40
 * @having-line---------------------------------------------------------
 */
public class Receive
{
	private final static String QUEUE_NAME = "hello";

	public static void main(String[] argv) throws Exception
	{

		ConnectionFactory factory = new ConnectionFactory();
		factory.setHost("localhost");
		Connection connection = factory.newConnection();
		Channel channel = connection.createChannel();

		channel.queueDeclare(QUEUE_NAME, false, false, false, null);
		System.out.println(" [*] Waiting for messages. To exit press CTRL+C");

		QueueingConsumer consumer = new QueueingConsumer(channel);
		channel.basicConsume(QUEUE_NAME, true, consumer);

		while (true)
		{
			QueueingConsumer.Delivery delivery = consumer.nextDelivery();
			String message = new String(delivery.getBody());
			System.out.println(" [x] Received '" + message + "'");
		}
	}
}
```


#### 发送端代码



```java
package com.boonya.rabbitmq;

import java.io.IOException;
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
/**
 * 
 * @author        boonya
 *
 * @having-line---------------------------------------------------------
 * @filename      Send.java
 * @function      TODO
 * @download      <a href='http://www.rabbitmq.com/download.html'>http://www.rabbitmq.com/download.html</a>
 * @start-at      2014年7月16日,下午2:50:40
 * @having-line---------------------------------------------------------
 */
public class Send
{
	private final static String QUEUE_NAME = "hello";

	public static void main(String[] args) throws IOException
	{
		ConnectionFactory factory = new ConnectionFactory();
		factory.setHost("localhost");
		Connection connection = factory.newConnection();
		Channel channel = connection.createChannel();

		channel.queueDeclare(QUEUE_NAME, false, false, false, null);
		String message = "Hello World!";
		channel.basicPublish("", QUEUE_NAME, null, message.getBytes());
		System.out.println(" [x] Sent '" + message + "'");

		channel.close();
		connection.close();
	}
}
```


 启动客户端程序测试效果图：



![](https://img-blog.csdn.net/20140716163850756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

发送三次数据，接收端收到三条消息。demo下载地址：[http://download.csdn.net/detail/boonya/7642979](http://download.csdn.net/detail/boonya/7642979)（示例程序含Jar包）.



