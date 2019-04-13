
# Redis 发布订阅 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:24:26[seven-soft](https://me.csdn.net/softn)阅读数：242个人分类：[Redis																](https://blog.csdn.net/softn/article/category/6332853)



Redis 发布订阅(pub/sub)是一种消息通信模式：发送者(pub)发送消息，订阅者(sub)接收消息。
Redis 客户端可以订阅任意数量的频道。
下图展示了频道 channel1 ， 以及订阅这个频道的三个客户端 —— client2 、 client5 和 client1 之间的关系：
![pubsub1](http://www.runoob.com/wp-content/uploads/2014/11/pubsub1.png)当有新消息通过 PUBLISH 命令发送给频道 channel1 时， 这个消息就会被发送给订阅它的三个客户端：
![pubsub2](http://www.runoob.com/wp-content/uploads/2014/11/pubsub2.png)
---

## 实例
以下实例演示了发布订阅是如何工作的。在我们实例中我们创建了订阅频道名为**redisChat**:
redis127.0.0.1:6379>SUBSCRIBE redisChatReadingmessages...(pressCtrl-C to quit)1)"subscribe"2)"redisChat"3)(integer)1现在，我们先重新开启个 redis 客户端，然后在同一个频道 redisChat 发布两次消息，订阅者就能接收到消息。
redis127.0.0.1:6379>PUBLISH redisChat"Redis is a great caching technique"(integer)1redis127.0.0.1:6379>PUBLISH redisChat"Learn redis by w3cschool.cc"(integer)1\# 订阅者的客户端会显示如下消息1)"message"2)"redisChat"3)"Redis is a great caching technique"1)"message"2)"redisChat"3)"Learn redis by w3cschool.cc"
---

## Redis 发布订阅命令
下表列出了 redis 发布订阅常用命令：
|序号|命令及描述|
|1|[PSUBSCRIBE pattern [pattern ...]](http://www.runoob.com/redis/pub-sub-psubscribe.html)
|订阅一个或多个符合给定模式的频道。|
|2|[PUBSUB subcommand [argument [argument ...]]](http://www.runoob.com/redis/pub-sub-pubsub.html)
|查看订阅与发布系统状态。|
|3|[PUBLISH channel message](http://www.runoob.com/redis/pub-sub-publish.html)
|将信息发送到指定的频道。|
|4|[PUNSUBSCRIBE [pattern [pattern ...]]](http://www.runoob.com/redis/pub-sub-punsubscribe.html)
|退订所有给定模式的频道。|
|5|[SUBSCRIBE channel [channel ...]](http://www.runoob.com/redis/pub-sub-subscribe.html)
|订阅给定的一个或多个频道的信息。|
|6|[UNSUBSCRIBE [channel [channel ...]]](http://www.runoob.com/redis/pub-sub-unsubscribe.html)
|指退订给定的频道。|

