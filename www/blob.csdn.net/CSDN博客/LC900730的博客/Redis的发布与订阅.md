# Redis的发布与订阅 - LC900730的博客 - CSDN博客
2018年01月27日 20:54:42[lc900730](https://me.csdn.net/LC900730)阅读数：435
## 频道的订阅与退订
当一个客户端执行SUBSCRIBE命令订阅某个或某些频道的时候，这个客户端与被订阅频道之间就建立起了一种订阅关系。 
Redis将所有频道的订阅关系都保存在服务器状态的pubsub_channels字典里面，这个字典的键是某个被订阅的频道，而键的值则是一个链表，链表里面记录了所有订阅这个频道的客户端。
```
struct redisServer{
    //保存所有频道的订阅关系
    dict *pubsub_channels;
    //...
}
```
- client-1、client-2、client-3三个客户端正在订阅”news.it”频道
- 客户端client-4正在订阅“news.sport频道”
- client-5和client-6两个客户端正在订阅的”news.business”频道。
![这里写图片描述](https://img-blog.csdn.net/20180127154631049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 模式的订阅与退订
服务器将所有模式的订阅关系保存在服务器状态的pubsub_patterns属性里面
```
struct redisServer{
    //保存所有模式订阅关系
    list *pubsub_patterns;
    //...
}
```
pubsub_patterns属性是一个链表，链表中每一个节点都包含着一个pubsub_pattern结构，这个结构的pattern属性记录了被订阅的模式，而client属性则记录了订阅模式的客户端：
```
typedef struct pubsubPattern{
    //订阅模式的客户端
    redisClient *client;
    //被订阅的模式
    robj *pattern;
}piubsubPattern;
```
![这里写图片描述](https://img-blog.csdn.net/20180127155346099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 发送消息
当一个客户端执行`Publish <channel> <message>`命令将消息发送到频道channel时候，执行如下： 
* 将消息message发送给channel频道的所有订阅者； 
* 如果有一个或多个模式pattern与频道channel相匹配，那么消息message发送给pattern模式的订阅者。
#### 将消息发送给频道订阅者
PUBLISH命令要做的就是在pubsub_channels字典找到频道channel的订阅者名单(一个链表)，然后将消息发送到名单上的所有客户端。 
![这里写图片描述](https://img-blog.csdn.net/20180127204743561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果客户端执行PUBLISH “news.it” “hello” 
那么PUBLISH命令将在pubsub_channels字典中查找键为”news.it”对应的链表值，并且通过遍历链表将信息”hello”发送给订阅者。
