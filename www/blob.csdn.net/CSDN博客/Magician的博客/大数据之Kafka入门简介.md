
# 大数据之Kafka入门简介 - Magician的博客 - CSDN博客


2018年04月07日 14:18:05[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：234标签：[Kafka																](https://so.csdn.net/so/search/s.do?q=Kafka&t=blog)[简介																](https://so.csdn.net/so/search/s.do?q=简介&t=blog)[组件																](https://so.csdn.net/so/search/s.do?q=组件&t=blog)[JMS																](https://so.csdn.net/so/search/s.do?q=JMS&t=blog)[原理																](https://so.csdn.net/so/search/s.do?q=原理&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=JMS&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=组件&t=blog)个人分类：[Kafka																](https://blog.csdn.net/qq_16633405/article/category/7569741)[大数据																](https://blog.csdn.net/qq_16633405/article/category/6905043)[
							](https://blog.csdn.net/qq_16633405/article/category/7569741)
所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)
[
																	](https://so.csdn.net/so/search/s.do?q=组件&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=简介&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=简介&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=Kafka&t=blog)

[目录](#目录)[前言：](#前言)
[1、Kafka是什么](#1kafka是什么)
[2、JMS是什么](#2jms是什么)
[3、Kafka核心组件（重点）](#3kafka核心组件重点)
[总结：](#总结)


# 目录
## 前言：
作为流式计算中的一个组件，对于它的组成以及运行的原理，学习者也需要相关的了解。以下主要简单介绍了kafka是什么以及对应的组件有哪些；除此之外重点介绍了JMS，毕竟JMS也算是Kafka的核心，还是有必要了解一下。
## 1、Kafka是什么
在流式计算中，**Kafka一般用来缓存数据**，Storm通过消费Kafka的数据进行计算。
Apache Kafka是一个开源消息系统，由Scala写成。是由Apache软件基金会开发的一个开源消息系统项目。
Kafka最初是由LinkedIn开发，并于2011年初开源。2012年10月从Apache
Incubator毕业。该项目的目标是为处理实时数据提供一个统一、高通量、低等待的平台。
Kafka是一个分布式消息队列：生产者、消费者的功能。它提供了类似于JMS的特性，但是在设计实现上完全不同，此外它并不是JMS规范的实现。
Kafka对消息保存时根据Topic进行归类，发送消息者称为Producer,消息接受者称为Consumer,此外kafka集群有多个kafka实例组成，每个实例(server)成为broker。
无论是kafka集群，还是producer和consumer都依赖于zookeeper集群保存一些meta信息，来保证系统可用性。
2、JMS是什么
2.1、JMS的基础
JMS是什么：JMS是Java提供的一套技术规范
JMS干什么用：用来异构系统 集成通信，缓解系统瓶颈，提高系统的伸缩性增强系统用户体验，使得系统模块化和组件化变得可行并更加灵活
总结：为什么需要消息队列（重要）
消息系统的核心作用就是三点：解耦，异步和并行
通过什么方式：生产消费者模式（生产者、服务器、消费者）
![这里写图片描述](https://img-blog.csdn.net/20180407140301553?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.2、JMS消息传输模型
点对点模式（一对一，消费者主动拉取数据，消息收到后消息清除）
点对点模型通常是一个基于拉取或者轮询的消息传送模型，这种模型从队列中请求信息，而不是将消息推送到客户端。这个模型的特点是发送到队列的消息被一个且只有一个接收者接收处理，即使有多个消息监听者也是如此。
发布/订阅模式（一对多，数据生产后，推送给所有订阅者）
发布订阅模型则是一个基于推送的消息传送模型。发布订阅模型可以有多种不同的订阅者，临时订阅者只在主动监听主题时才接收消息，而持久订阅者则监听主题的所有消息，即时当前订阅者不可用，处于离线状态。
![这里写图片描述](https://img-blog.csdn.net/20180407140351745?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.3、JMS核心组件
Destination：消息发送的目的地，也就是前面说的Queue和Topic。
Message ：从字面上就可以看出是被发送的消息。
Producer： 消息的生产者，要发送一个消息，必须通过这个生产者来发送。
MessageConsumer： 与生产者相对应，这是消息的消费者或接收者，通过它来接收一个消息。
Message：
StreamMessage：Java 数据流消息，用标准流操作来顺序的填充和读取。
MapMessage：一个Map类型的消息；名称为 string 类型，而值为 Java 的基本类型。
TextMessage：普通字符串消息，包含一个String。
ObjectMessage：对象消息，包含一个可序列化的Java 对象
BytesMessage：二进制数组消息，包含一个byte[]。
XMLMessage:  一个XML类型的消息。
最常用的是TextMessage和ObjectMessage。
![这里写图片描述](https://img-blog.csdn.net/2018040714055937?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018040714055937?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过与ConnectionFactory可以获得一个connection
通过connection可以获得一个session会话。
3、Kafka核心组件（重点）
Topic ：消息根据Topic进行归类
Producer：发送消息者
Consumer：消息接受者
broker：每个kafka实例(server)
Zookeeper：依赖集群保存meta信息。
![这里写图片描述](https://img-blog.csdn.net/20180407140754455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
总结：
本篇文章只是对Kafka做了一个简单的介绍。通过本篇文章，你只要能知道Kafka的核心组件有哪些以及Kafka是什么就OK；当然能了解JMS更好。下面的文章中会对Kafka做一个深入的介绍。

[
  ](https://img-blog.csdn.net/2018040714055937?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)