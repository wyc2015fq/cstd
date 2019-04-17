# 我的第一个rabbitMQ程序-hello_world - zp704393004的专栏 - CSDN博客





2018年04月18日 09:31:52[原来未知](https://me.csdn.net/zp704393004)阅读数：40








首先确定rabbitMQ已经安装完成，也就是在安装目录下运行rabbitMQ-server能成功

之后进行如下两个步骤：

首先创建一个hello_world_send.py


- 
- import os  
- import pika  
- 
- conn = None
- 
- try:  
- # 获取连接
-     conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))  
- # 获取通道
-     channel = conn.channel()  
- 
- # 在发送队列前，需要确定队列是否存在，如果不存在RabbitMQ将会丢弃，先创建队列
-     channel.queue_declare('hello')  
- 
- # 在RabbitMQ中发送消息，不是直接发送队列，而是发送交换机（exchange），此处不多做研究，后面系列逐渐深入
-     ret = channel.basic_publish(exchange='',  
-                                 routing_key='hello',  
-                                 body="Hello, World!")  
- print" [x] Sent 'Hello World!'"
- print ret  
- except Exception, e:  
- raise e  
- finally:  
- if conn:  
-         conn.close() 


然后创建一个hello_world_recv.py


- 

- import os  
- import pika  
- 
- conn = None
- 
- 
- def callback(ch, method, properties, body):  
- """
-         out body
-     """
- print" [x] Recived ch {0}".format(ch)  
- 
- print" [x] Recived method {0}".format(method)  
- 
- print" [x] Recived properties {0}".format(properties)  
- 
- print" [x] Recived %r" % (body, )  
- 
- try:  
- # get connection
-     conn = pika.BlockingConnection(pika.ConnectionParameters(  
- 'localhost')  
-     )  
- 
- # get channel
-     channel = conn.channel()  
- 
- # declare queue, 重复声明不会报错，但是没有队列的话直接取用会报错
-     channel.queue_declare('hello')  
- 
- # get message
-     channel.basic_consume(callback, queue='hello', no_ack=True)  
- 
- print' [*] Waiting for messages. To exit press CTRL+C'
-     channel.start_consuming()  
- except Exception, e:  
- raise e  
- finally:  
- if conn:  
-         conn.close()  


现在：

1：在rabbitMQ安装目录的sbin下运行rabbitmq-server

2:在python文件目录下运行：python hello_world_recv.py

3:在python文件目录下运行：python hello_world_send.py

成功后可以看到


- $ python send_helloworld.py   
-  [x] Sent 'Hello World!'  
- True  

- $ python recv_helloworld.py   
-  [*] Waiting for messages. To exit press CTRL+C  
-  [x] Recived ch <pika.adapters.blocking_connection.BlockingChannel object at 0x7f61ecc6fa90>  
-  [x] Recived method <Basic.Deliver(['consumer_tag=ctag1.6c2c709930904468b40d0e1a758f7aca', 'delivery_tag=1', 'exchange=', 'redelivered=False', 'routing_key=hello'])>  
-  [x] Recived properties <BasicProperties>  
-  [x] Recived 'Hello, World!'  


运行程序启动后，一直在等待获取消息，可以通过Ctrl-C来中止。Hello World基本就结束了，基本知道RabbitMQ基本的使用方法，后续将会进一步对RabbitMQ的使用场景进行解析。

下面给出一个参考网址：

https://blog.csdn.net/fgf00/article/details/52872730




