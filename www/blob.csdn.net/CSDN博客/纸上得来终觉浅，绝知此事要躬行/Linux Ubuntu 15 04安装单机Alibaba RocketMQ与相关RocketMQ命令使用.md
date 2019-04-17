# Linux Ubuntu 15.04安装单机Alibaba RocketMQ与相关RocketMQ命令使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月21日 14:44:47[boonya](https://me.csdn.net/boonya)阅读数：1197








前提条件：安装Java（必须），Maven（源码编译，否则可以不下载）。Java的安装这里就不再说明了，注意安装后要设置环境变量。


## 环境准备

**1.下载alibaba-rocketmq**

此处我们下载的是：https://github.com/alibaba/RocketMQ/releases/download/v3.2.6/alibaba-rocketmq-3.2.6.tar.gz

直接cd进入到/usr/local路径：



`wget https://github.com/alibaba/RocketMQ/releases/download/v3.2.6/alibaba-rocketmq-3.2.6.tar.gz`
**2.解压文件到/usr/local**


解压之后得到：/usr/local/alibaba-rocketmq


`tar zxvf alibaba-rocketmq-3.2.6.tar.gz`
**3.在/etc/profile配置rabbitmq环境变量**




```
#RocketMQ
export rocketmq=/usr/local/alibaba-rocketmq 
export PATH=$PATH:$rocketmq/bin
```



**4.使环境变量生效**


`source /etc/profile`


## RocketMQ启动

命令基于sh/bash


**1.启动mqnamesrv**



```
root@ubuntu:/usr/local/alibaba-rocketmq/bin# sh mqnamesrv -n 192.168.234.128:9876
The Name Server boot success.
```




**2.启动mqbroker**



```
root@ubuntu:/usr/local/alibaba-rocketmq/bin# sh mqbroker -n 192.168.234.128:9876
The broker[ubuntu, 192.168.234.128:10911] boot success. and name server is 192.168.234.128:9876
```





## RocketMQ关闭





命令基于sh/bash


**1.关闭mqnamesrv**


`sh mqshutdown namesrv`




**2.关闭mqbroker**


`sh mqshutdown broker`


## Java代码测试

**Producer.java**




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
		producer.setNamesrvAddr("192.168.234.128:9876");
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

**Consumer.java**




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
		consumer.setNamesrvAddr("192.168.234.128:9876");
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





```
14:23:27.293 [main] DEBUG i.n.u.i.l.InternalLoggerFactory - Using SLF4J as the default logging framework
14:23:27.299 [main] DEBUG i.n.c.MultithreadEventLoopGroup - -Dio.netty.eventLoopThreads: 8
14:23:27.307 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Buffer.address: available
14:23:27.307 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.theUnsafe: available
14:23:27.308 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.copyMemory: available
14:23:27.308 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Bits.unaligned: true
14:23:27.308 [main] DEBUG i.n.util.internal.PlatformDependent - Platform: Windows
14:23:27.309 [main] DEBUG i.n.util.internal.PlatformDependent - Java version: 8
14:23:27.309 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noUnsafe: false
14:23:27.309 [main] DEBUG i.n.util.internal.PlatformDependent - sun.misc.Unsafe: available
14:23:27.309 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noJavassist: false
14:23:27.309 [main] DEBUG i.n.util.internal.PlatformDependent - Javassist: unavailable
14:23:27.310 [main] DEBUG i.n.util.internal.PlatformDependent - You don't have Javassist in your class path or you don't have enough permission to load dynamically generated classes.  Please check the configuration for better performance.
14:23:27.310 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.tmpdir: C:\Users\PENGJU~1\AppData\Local\Temp (java.io.tmpdir)
14:23:27.310 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.bitMode: 64 (sun.arch.data.model)
14:23:27.310 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noPreferDirect: false
14:23:27.323 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.noKeySetOptimization: false
14:23:27.323 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.selectorAutoRebuildThreshold: 512
14:23:27.561 [main] DEBUG i.n.util.internal.ThreadLocalRandom - -Dio.netty.initialSeedUniquifier: 0xb15735ec2b121d6e (took 10 ms)
14:23:27.581 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.allocator.type: unpooled
14:23:27.581 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.threadLocalDirectBufferSize: 65536
14:23:27.602 [main] DEBUG io.netty.util.Recycler - -Dio.netty.recycler.maxCapacity.default: 262144
14:23:27.608 [NettyClientWorkerThread_1] DEBUG io.netty.util.ResourceLeakDetector - -Dio.netty.leakDetectionLevel: simple
14:23:27.702 [NettyClientSelector_1] DEBUG io.netty.util.internal.Cleaner0 - java.nio.ByteBuffer.cleaner(): available
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000000, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=0], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000095, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=0], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F000000000000012B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=0], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000001C0, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=1], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000255, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=1], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000002EB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=1], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000380, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=2], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000415, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=2], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000004AB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=2], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000540, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=3], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000005D5, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=3], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F000000000000066B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=3], queueOffset=0]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000700, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=0], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000795, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=0], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F000000000000082B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=0], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000008C0, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=1], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000955, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=1], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000009EB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=1], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000A80, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=2], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000B15, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=2], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000BAB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=2], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000C40, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=3], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000CD5, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=3], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000D6B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=3], queueOffset=1]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000E00, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=0], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000E95, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=0], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000F2B, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=0], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000000FC0, messageQueue=MessageQueue [topic=TopicTest1, brokerName=ubuntu, queueId=1], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F0000000000001055, messageQueue=MessageQueue [topic=TopicTest2, brokerName=ubuntu, queueId=1], queueOffset=2]
SendResult [sendStatus=SEND_OK, msgId=C0A8EA8000002A9F00000000000010EB, messageQueue=MessageQueue [topic=TopicTest3, brokerName=ubuntu, queueId=1], queueOffset=2]
```


```
14:23:20.851 [main] DEBUG i.n.u.i.l.InternalLoggerFactory - Using SLF4J as the default logging framework
14:23:20.856 [main] DEBUG i.n.c.MultithreadEventLoopGroup - -Dio.netty.eventLoopThreads: 8
14:23:20.867 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Buffer.address: available
14:23:20.867 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.theUnsafe: available
14:23:20.867 [main] DEBUG i.n.util.internal.PlatformDependent0 - sun.misc.Unsafe.copyMemory: available
14:23:20.868 [main] DEBUG i.n.util.internal.PlatformDependent0 - java.nio.Bits.unaligned: true
14:23:20.868 [main] DEBUG i.n.util.internal.PlatformDependent - Platform: Windows
14:23:20.868 [main] DEBUG i.n.util.internal.PlatformDependent - Java version: 8
14:23:20.868 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noUnsafe: false
14:23:20.868 [main] DEBUG i.n.util.internal.PlatformDependent - sun.misc.Unsafe: available
14:23:20.869 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noJavassist: false
14:23:20.870 [main] DEBUG i.n.util.internal.PlatformDependent - Javassist: unavailable
14:23:20.870 [main] DEBUG i.n.util.internal.PlatformDependent - You don't have Javassist in your class path or you don't have enough permission to load dynamically generated classes.  Please check the configuration for better performance.
14:23:20.870 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.tmpdir: C:\Users\PENGJU~1\AppData\Local\Temp (java.io.tmpdir)
14:23:20.870 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.bitMode: 64 (sun.arch.data.model)
14:23:20.870 [main] DEBUG i.n.util.internal.PlatformDependent - -Dio.netty.noPreferDirect: false
14:23:20.888 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.noKeySetOptimization: false
14:23:20.888 [main] DEBUG io.netty.channel.nio.NioEventLoop - -Dio.netty.selectorAutoRebuildThreshold: 512
14:23:21.174 [main] DEBUG i.n.util.internal.ThreadLocalRandom - -Dio.netty.initialSeedUniquifier: 0x07c82c04d4f7730b (took 13 ms)
14:23:21.195 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.allocator.type: unpooled
14:23:21.195 [main] DEBUG io.netty.buffer.ByteBufUtil - -Dio.netty.threadLocalDirectBufferSize: 65536
14:23:21.226 [main] DEBUG io.netty.util.Recycler - -Dio.netty.recycler.maxCapacity.default: 262144
14:23:21.237 [NettyClientWorkerThread_1] DEBUG io.netty.util.ResourceLeakDetector - -Dio.netty.leakDetectionLevel: simple
14:23:21.377 [NettyClientSelector_1] DEBUG io.netty.util.internal.Cleaner0 - java.nio.ByteBuffer.cleaner(): available
Consumer Started.
ConsumeMessageThread_1 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_3 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_6 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_2 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_7 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_5 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_8 Receive New Messages: 1
ConsumeMessageThread_4 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_9 Receive New Messages: 1
Hello MetaQA
Hello MetaQA
ConsumeMessageThread_10 Receive New Messages: 1
Hello MetaQA
ConsumeMessageThread_11 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_14 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_15 Receive New Messages: 1
ConsumeMessageThread_13 Receive New Messages: 1
Hello MetaQB
Hello MetaQB
ConsumeMessageThread_12 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_17 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_16 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_18 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_19 Receive New Messages: 1
Hello MetaQB
ConsumeMessageThread_20 Receive New Messages: 1
Hello MetaQB
```


## Topic相关指令

**启动BrokerServer a, BrokerServer b**



启动master A


```
sh mqbroker -n 192.168.234.128:9876 -c ../conf/2m-noslave/broker-a.properties
```


启动master B

`sh mqbroker -n 192.168.234.128:9876 -c ../conf/2m-noslave/broker-b.properties `

**查看Topic信息**



```
sh mqadmin topicList -n 192.168.234.128:9876
```

**查看Topic统计信息**


`sh mqadmin topicStatus -n 192.168.234.128:9876 -t TopicTest1`
**创建Topic**


`sh mqadmin updateTopic -n 192.168.234.128:9876 -c DefaultCluster -t TopicTest1`
**删除Topic**

`sh mqadmin deleteTopic -n 192.168.234.128:9876 -c DefaultCluster -t TopicTest1`
**查看所有消费组group**

`sh mqadmin consumerProgress -n 192.168.234.128:9876`

**查看所有消费组下所有Topic数据堆积情况**


`sh mqadmin consumerProgress -n 192.168.234.128:9876 -g ConsumerGroupName`

参考文章：[http://www.cnblogs.com/xiaodf/p/5075167.html](http://www.cnblogs.com/xiaodf/p/5075167.html)









