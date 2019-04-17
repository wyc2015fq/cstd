# Window单机测试Alibaba RocketMQ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月21日 11:42:02[boonya](https://me.csdn.net/boonya)阅读数：1041










RocketMQ 是一款开源的分布式消息系统，基于高可用分布式集群技术，提供低延时的、高可靠的消息发布与订阅服务。同时，广泛应用于多个领域，包括异步通信解耦、企业解决方案、金融支付、电信、电子商务、快递物流、广告营销、社交、即时通信、移动应用、手游、视频、物联网、车联网等。

具有以下特点：

- 
能够保证严格的消息顺序

- 
提供丰富的消息拉取模式

- 
高效的订阅者水平扩展能力

- 
实时的消息订阅机制

- 
  亿级消息堆积能力

- 
Metaq3.0 版本改名，产品名称改为RocketMQ


更多阅读请参考：

[阿里开源消息中间件RocketMQ的前世今生](http://jm.taobao.org/2016/11/29/apache-rocketmq-history/)

[Apache基金会宣布接收阿里消息中间件RocketMQ为孵化项目](http://jm.taobao.org/2016/11/29/apache-rocketmq-incubation/)


本文以RocketMQ3.2.6为例。

## RocketMQ部署架构

所有IP都是127.0.0.1，其中NameServer一个，Broker一个，Producer一个，Consumer一个
![](https://img-blog.csdn.net/20170321104454221)

## RocketMQ工作原理

![](https://img-blog.csdn.net/20170321113200040)


## RocketMQ环境搭建

（1）.下载安装包 下载地址：[https://github.com/alibaba/RocketMQ/releases](https://github.com/alibaba/RocketMQ/releases)
![](https://img-blog.csdn.net/20170321104922164)

（2）.解压，并进入bin目录

![](https://img-blog.csdn.net/20170321105000056)

（3）.启动name server
![](https://img-blog.csdn.net/20170321113002180)

（4）启动broker

![](https://img-blog.csdn.net/20170321105433194)


（5）jps测试

![](https://img-blog.csdn.net/20170321105420015)


## 测试代码及异常处理

Producer.java



```java
package com.github.boonya.rocketmq.producer;

import java.util.concurrent.TimeUnit;
import com.alibaba.rocketmq.client.exception.MQClientException;
import com.alibaba.rocketmq.client.producer.DefaultMQProducer;
import com.alibaba.rocketmq.client.producer.SendResult;
import com.alibaba.rocketmq.common.message.Message;

/**
 * 生产者
 * 
 * @package com.github.boonya.rocketmq.producer.Producer
 * @date 2017年3月20日 下午5:10:47
 * @author pengjunlin
 * @comment
 * @update
 */
public class Producer {

	public static void main(String[] args) throws MQClientException,
			InterruptedException {
		/**
		 * 一个应用创建一个Producer，由应用来维护此对象，可以设置为全局对象或者单例<br>
		 * 注意：ProducerGroupName需要由应用来保证唯一<br>
		 * ProducerGroup这个概念发送普通的消息时，作用不大，但是发送分布式事务消息时，比较关键，
		 * 因为服务器会回查这个Group下的任意一个Producer
		 */
		final DefaultMQProducer producer = new DefaultMQProducer(
				"ProducerGroupName");
		producer.setNamesrvAddr("localhost:9876");
		producer.setInstanceName("Producer");

		/**
		 * Producer对象在使用之前必须要调用start初始化，初始化一次即可<br>
		 * 注意：切记不可以在每次发送消息时，都调用start方法
		 */
		producer.start();

		/**
		 * 下面这段代码表明一个Producer对象可以发送多个topic，多个tag的消息。
		 * 注意：send方法是同步调用，只要不抛异常就标识成功。但是发送成功也可会有多种状态，<br>
		 * 例如消息写入Master成功，但是Slave不成功，这种情况消息属于成功，但是对于个别应用如果对消息可靠性要求极高，<br>
		 * 需要对这种情况做处理。另外，消息可能会存在发送失败的情况，失败重试由应用来处理。
		 */
		for (int i = 0; i < 10; i++) {
			try {
				{
					Message msg = new Message("TopicTest1",// topic
							"TagA",// tag
							"OrderID001",// key
							("Hello MetaQA").getBytes());// body
					SendResult sendResult = producer.send(msg);
					System.out.println(sendResult);
				}

				{
					Message msg = new Message("TopicTest2",// topic
							"TagB",// tag
							"OrderID0034",// key
							("Hello MetaQB").getBytes());// body
					SendResult sendResult = producer.send(msg);
					System.out.println(sendResult);
				}

				{
					Message msg = new Message("TopicTest3",// topic
							"TagC",// tag
							"OrderID061",// key
							("Hello MetaQC").getBytes());// body
					SendResult sendResult = producer.send(msg);
					System.out.println(sendResult);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
			TimeUnit.MILLISECONDS.sleep(1000);
		}

		/**
		 * 应用退出时，要调用shutdown来清理资源，关闭网络连接，从MetaQ服务器上注销自己
		 * 注意：我们建议应用在JBOSS、Tomcat等容器的退出钩子里调用shutdown方法
		 */
		// producer.shutdown();
		Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
			public void run() {
				producer.shutdown();
			}
		}));
		System.exit(0);
	}
}
```





Consumer.java



```java
package com.github.boonya.rocketmq.consumer;

import java.util.List;

import com.alibaba.rocketmq.client.consumer.DefaultMQPushConsumer;
import com.alibaba.rocketmq.client.consumer.listener.ConsumeConcurrentlyContext;
import com.alibaba.rocketmq.client.consumer.listener.ConsumeConcurrentlyStatus;
import com.alibaba.rocketmq.client.consumer.listener.MessageListenerConcurrently;
import com.alibaba.rocketmq.client.exception.MQClientException;
import com.alibaba.rocketmq.common.message.MessageExt;

/**
 * 消费者
 * 
 * @package com.github.boonya.rocketmq.consumer.Consumer
 * @date 2017年3月20日 下午5:10:58
 * @author pengjunlin
 * @comment
 * @update
 */
public class Consumer {

	/**
	 * 当前例子是PushConsumer用法，使用方式给用户感觉是消息从RocketMQ服务器推到了应用客户端。<br>
	 * 但是实际PushConsumer内部是使用长轮询Pull方式从MetaQ服务器拉消息，然后再回调用户Listener方法<br>
	 */
	public static void main(String[] args) throws InterruptedException,
			MQClientException {
		/**
		 * 一个应用创建一个Consumer，由应用来维护此对象，可以设置为全局对象或者单例<br>
		 * 注意：ConsumerGroupName需要由应用来保证唯一
		 */
		DefaultMQPushConsumer consumer = new DefaultMQPushConsumer(
				"ConsumerGroupName");
		consumer.setNamesrvAddr("localhost:9876");
		consumer.setInstanceName("Consumber");

		/**
		 * 订阅指定topic下tags分别等于TagA或TagC或TagD
		 */
		consumer.subscribe("TopicTest1", "TagA || TagC || TagD");
		/**
		 * 订阅指定topic下所有消息<br>
		 * 注意：一个consumer对象可以订阅多个topic
		 */
		consumer.subscribe("TopicTest2", "*");

		consumer.registerMessageListener(new MessageListenerConcurrently() {

			public ConsumeConcurrentlyStatus consumeMessage(
					List<MessageExt> msgs, ConsumeConcurrentlyContext context) {

				System.out.println(Thread.currentThread().getName()
						+ " Receive New Messages: " + msgs.size());

				MessageExt msg = msgs.get(0);
				if (msg.getTopic().equals("TopicTest1")) {
					// 执行TopicTest1的消费逻辑
					if (msg.getTags() != null && msg.getTags().equals("TagA")) {
						// 执行TagA的消费
						System.out.println(new String(msg.getBody()));
					} else if (msg.getTags() != null
							&& msg.getTags().equals("TagC")) {
						// 执行TagC的消费
						System.out.println(new String(msg.getBody()));
					} else if (msg.getTags() != null
							&& msg.getTags().equals("TagD")) {
						// 执行TagD的消费
						System.out.println(new String(msg.getBody()));
					}
				} else if (msg.getTopic().equals("TopicTest2")) {
					System.out.println(new String(msg.getBody()));
				}

				return ConsumeConcurrentlyStatus.CONSUME_SUCCESS;

			}
		});

		/**
		 * Consumer对象在使用之前必须要调用start初始化，初始化一次即可<br>
		 */
		consumer.start();

		System.out.println("Consumer Started.");
	}

}
```
运行输出：



Producer sent Message：




```
11:37:28.314 [main] DEBUG i.n.u.i.l.InternalLoggerFactory - Using SLF4J as the default logging framework
11:37:28.321 [main] DEBUG i.n.c.MultithreadEventLoopGroup - -Dio.netty.eventLoopThreads: 8
11:37:28.329 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Buffer.address: available
11:37:28.329 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.theUnsafe: available
11:37:28.329 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.copyMemory: available
11:37:28.329 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Bits.unaligned: true
11:37:28.330 [main] DEBUG i.n.util.internal.PlatformDependent - Platform: Windows
11:37:28.330 [main] DEBUG i.n.util.internal.PlatformDependent - Java version: 8
11:37:28.330 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noUnsafe: false
11:37:28.330 [main] DEBUG i.n.util.internal.PlatformDependent - sun.misc.Unsafe: available
11:37:28.330 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noJavassist: false
11:37:28.331 [main] DEBUG i.n.util.internal.PlatformDependent - Javassist: unavailable
11:37:28.331 [main] DEBUG i.n.util.internal.PlatformDependent - You don't have Javassist in your class path or you don't have enough permission to load dynamically generated classes.  Please check the configuration for better performance.
11:37:28.331 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.tmpdir: C:\Users\PENGJU~1\AppData\Local\Temp (java.io.tmpdir)
11:37:28.331 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.bitMode: 64 (sun.arch.data.model)
11:37:28.332 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noPreferDirect: false
11:37:28.352 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.noKeySetOptimization: false
11:37:28.352 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.selectorAutoRebuildThreshold: 512
11:37:28.590 [main] DEBUG i.n.util.internal.ThreadLocalRandom - -Dio.netty.initialSeedUniquifier: 0xa4432147e0b32bdc (took 9 ms)
11:37:28.607 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.allocator.type: unpooled
11:37:28.607 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.threadLocalDirectBufferSize: 65536
11:37:28.628 [main] DEBUG io.netty.util.Recycler - -Dio.netty.recycler.maxCapacity.default: 262144
11:37:28.633 [NettyClientWorkerThread_1] DEBUG io.netty.util.ResourceLeakDetector - -Dio.netty.leakDetectionLevel: simple
11:37:28.709 [NettyClientSelector_1] DEBUG io.netty.util.internal.Cleaner0 - java.nio.ByteBuffer.cleaner(): available
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001180, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001215, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000012AB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001340, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000013D5, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F000000000000146B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001500, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=2], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001595, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=2], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F000000000000162B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=2], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000016C0, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=3], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001755, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=3], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000017EB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=3], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001880, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=4]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001915, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=4]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000019AB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=4]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001A40, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=4]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001AD5, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=4]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001B6B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=4]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001C00, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=2], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001C95, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=2], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001D2B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=2], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001DC0, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=3], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001E55, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=3], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001EEB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=3], queueOffset=3]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000001F80, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=5]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000002015, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=5]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000020AB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=0], queueOffset=5]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F0000000000002140, messageQueue=MessageQueue [topic=TopicTest1, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=5]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F00000000000021D5, messageQueue=MessageQueue [topic=TopicTest2, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=5]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA0100002A9F000000000000226B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=RD-PJL-1372-01, queueId=1], queueOffset=5]
```


Consumer Received Message：





```
11:37:19.765 [main] DEBUG i.n.u.i.l.InternalLoggerFactory - Using SLF4J as the default logging framework
11:37:19.770 [main] DEBUG i.n.c.MultithreadEventLoopGroup - -Dio.netty.eventLoopThreads: 8
11:37:19.783 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Buffer.address: available
11:37:19.783 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.theUnsafe: available
11:37:19.784 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.copyMemory: available
11:37:19.784 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Bits.unaligned: true
11:37:19.784 [main] DEBUG i.n.util.internal.PlatformDependent - Platform: Windows
11:37:19.785 [main] DEBUG i.n.util.internal.PlatformDependent - Java version: 8
11:37:19.785 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noUnsafe: false
11:37:19.785 [main] DEBUG i.n.util.internal.PlatformDependent - sun.misc.Unsafe: available
11:37:19.785 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noJavassist: false
11:37:19.786 [main] DEBUG i.n.util.internal.PlatformDependent - Javassist: unavailable
11:37:19.786 [main] DEBUG i.n.util.internal.PlatformDependent - You don't have Javassist in your class path or you don't have enough permission to load dynamically generated classes.  Please check the configuration for better performance.
11:37:19.787 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.tmpdir: C:\Users\PENGJU~1\AppData\Local\Temp (java.io.tmpdir)
11:37:19.787 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.bitMode: 64 (sun.arch.data.model)
11:37:19.787 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noPreferDirect: false
11:37:19.803 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.noKeySetOptimization: false
11:37:19.803 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.selectorAutoRebuildThreshold: 512
11:37:20.189 [main] DEBUG i.n.util.internal.ThreadLocalRandom - -Dio.netty.initialSeedUniquifier: 0x7c71314ace4fd2c8 (took 17 ms)
11:37:20.211 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.allocator.type: unpooled
11:37:20.211 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.threadLocalDirectBufferSize: 65536
11:37:20.247 [main] DEBUG io.netty.util.Recycler - -Dio.netty.recycler.maxCapacity.default: 262144
11:37:20.259 [NettyClientWorkerThread_1] DEBUG io.netty.util.ResourceLeakDetector - -Dio.netty.leakDetectionLevel: simple
11:37:20.369 [NettyClientSelector_1] DEBUG io.netty.util.internal.Cleaner0 - java.nio.ByteBuffer.cleaner(): available
Consumer Started.
ConsumeMessageThread_2 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_1 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_3 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_4 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_5 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_6 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_7 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_8 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_9 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_10 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_11 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_12 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_13 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_14 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_15 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_16 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_17 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_18 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_19 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_20 Receive New Messages: 1
Hello MetaQB
```




** No route info of this topic, TopicTest1异常处理**，参考：[https://github.com/alibaba/RocketMQ/issues/44](https://github.com/alibaba/RocketMQ/issues/44)

此问题是启动时未设置：autoCreateTopicEnable=true




参考文章：

[http://www.2cto.com/kf/201610/554136.html](http://www.2cto.com/kf/201610/554136.html)


[http://www.iyunv.com/thread-16475-1-1.html](http://www.iyunv.com/thread-16475-1-1.html)






