# Redis-C客户端-HiRedis-(二) - 三少GG - CSDN博客
2014年01月22日 14:55:26[三少GG](https://me.csdn.net/scut1135)阅读数：6051

# 客户端示例：https://gist.github.com/dspezia/1893378
# hiredis入门[http://www.codingcool.com/2013/02/21/hiredis%E5%85%A5%E9%97%A8/](http://www.codingcool.com/2013/02/21/hiredis%E5%85%A5%E9%97%A8/)
前几篇介绍了redis以及phpredis，主要是因为我所在的项目组用的是php，而我接下来的一个小任务是用c++写一个处理存储在redis里的业务数据的小工具，在redis官方上看推荐的[c++客户端](http://redis.io/clients)，只有一个，而且还是2年前的一个临时项目，而且还要依赖boost，而且看开发者的口气，实在是觉得不敢用啊~
所以打算使用官方推荐的c客户端：[hiredis](https://github.com/redis/hiredis)，它可是官方的哦~而且看git的提交日期，近期也有提交，顿时觉得安心啊！
下载最新版的hiredis后，解压并make便得到了libhiredis.so和libredis.a，非常简单~把头文件也移动到自己系统的include文件夹中就可以在项目中方便的使用了。
hiredis并没有像phpredis那样提供了各种各样针对redis自身结构的封装函数，而是提供了基础的几个函数，例如用于连接redis，向redis提交命令，获取返回等，把底层协议和通信细节封装起来，包括错误甄别和数据结构的转换。总的来说，hiredis还是非常好用的。
hiredis提供的api又分为两大块：同步，异步。由于我能力有限，项目也没有异步要求，所以异步接口部分就不研究了，直接打开解压出来的[example.c](https://github.com/redis/hiredis/blob/master/example.c)文件，你就可以看到同步api是怎么使用的了。剩下的就是redis本身的命令格式和返回值了，可以在[这里](http://redis.io/commands)查阅。
这里我主要备注一下官方提出的一些注意点，方便大家规避问题：
1.一旦redisCommand()发生错误，会返回NULL，也会导致redisContext失效，使其不能再用于执行其他操作。
2.必须调用freeReplyObject()来释放reply对象，不然会造成内存泄露。
3.redisCommand()返回的是void *类型，需要手动进行类型转换(reply = (redisReply *)redisCommand(…))
hiredis还提供了pipeline的支持，用于提高性能。关于redis的pipeline和事务的一些内容我在[这里](http://www.codingcool.com/2013/02/19/phpredis%E7%9A%84%E4%BA%8B%E5%8A%A1%E6%93%8D%E4%BD%9C%E7%BB%86%E8%8A%82/)有提到过，下面贴一下官方提供的用于pipeline的hiredis例子：
redisReply *reply;
redisAppendCommand(context,"SET
 foo bar");
redisAppendCommand(context,"GET
 foo");
redisGetReply(context,&reply);//
 reply for SET
freeReplyObject(reply);
redisGetReply(context,&reply);//
 reply for GET
freeReplyObject(reply);
没啥好说的吧，挺直观的。
可以看一下官方解释的一个[内部机制](https://github.com/redis/hiredis#pipelining)，可能更深刻的理解这些函数的工作方式。
乱译：
> 
……
当任何redisCommand()被调用时，hredis首先根据redis协议格式化命令参数，然后把格式化后的命令存放在上下文（redisContext）的输出缓冲中，这个输出缓冲允许动态扩容，所以它可以存放任意数量的命令而不用担心会溢出。
接下来调用redisGetReply()，这个函数做下面两个任务：
1.如果输入缓冲不为空，则尝试从输入缓冲中读取解析单个回复，并返回它；若为空，则执行第2步。
2.若输入缓冲为空，则把整个输出缓冲写入套接字，并阻塞等待套接字，直到读取并解析到一个回复，并返回它。
由此可见，redisGetReply()函数既输出命令到socket，又从socket读入回复并返回给调用者。
……
接下来轮到介绍hiredis执行完命令后的返回结构了，也就是redisReply的结构：
typedefstruct redisReply {
int type;/*
 REDIS_REPLY_* */
longlong integer;/*
 The integer when type is REDIS_REPLY_INTEGER */
int len;/*
 Length of string */
char*str;/*
 Used for both REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
size_t elements;/*
 number of elements, for REDIS_REPLY_ARRAY */
struct redisReply **element;/*
 elements vector for REDIS_REPLY_ARRAY */
} redisReply;
其中，[type](https://github.com/redis/hiredis#using-replies)包含下面几个值：
REDIS_REPLY_STATUS,REDIS_REPLY_ERROR,REDIS_REPLY_INTEGER,REDIS_REPLY_NIL,REDIS_REPLY_STRING,
REDIS_REPLY_STRING,REDIS_REPLY_ARRAY。
这里并不打算逐一介绍所有的这些类型，好奇心童鞋可以点击提供的链接去github上一探究竟。只介绍一下这个：REDIS_REPLY_INTEGER。
说它之前，不得不先让大家看一下[官方的相关解释](http://redis.io/topics/protocol)，注意看其中“Replies”一节中的“Integer
 reply”段：
This type of reply is just a CRLF terminated string representing an integer, prefixed by a ":" byte. For example ":0\r\n", or ":1000\r\n" are integer replies.
乱译：Integer Reply这种类型其实是用以:开头，以CRLF结尾的字符串来表示的。
在经过hiredis的Reply Paser解析处理后，就会赋值到redisReply对象中的integer属性，并把type属性设置为3（REDIS_REPLY_INTEGER），大体就是这么个意思吧。
这里我遇到了一个问题，很奇怪，看代码：
...
//定义一个kv结构，v为long long int类型。
reply =(redisReply *)redisCommand(context,"SET
 myCounter %lld",(longlongint)7);
freeReplyObject(reply);
reply =(redisReply *)redisCommand(context,"GET
 myCounter");
//以为是REDIS_REPLY_INTEGER，但其实是REDIS_REPLY_STRING
std::cout<<"type:
 "<< reply->type << std::endl;
freeReplyObject(reply);
...
什么情况呢？看了一个[博客](http://www.cnblogs.com/liuhao/archive/2012/08/28/2660688.html)，才明白原因：
> 
即使value值的类型是integer（至少看上去是，实际server也确实是这么存的），但使用GET返回的值的类型（reply->type）仍是REDIS_REPLY_STRING，需要自己程序里转成long long。
阿西吧，原来没理解官方说明中的最后一段文字：
> 
The following commands will reply with an integer reply: SETNX, DEL, EXISTS, INCR, INCRBY, DECR, DECRBY, DBSIZE, LASTSAVE, RENAMENX, MOVE, LLEN, SADD, SREM, SISMEMBER, SCARD.
原来只有上述列出的命令才会返回Integer Reply。而我例子中的GET命令不再上诉范围，它返回的是string，还要自己类型转换！
目前我也就理解了这么多，以后有再补充吧，就到这里，8~
# redis持久化和主从的学习
redis是个好东西，我在之前的几篇博客里也啰嗦过了，呵呵~
但是如果用在实际项目中，如果不是只拿它代替memcache的话，还是非常有必要了解它的持久化内容的，不然很可能出现数据丢失的尴尬情况。
我果断的搜索了一下相关内容，发现很多大牛都做了比较好的总结，贴一下我找到的还不错的帖子：
[链接一](http://justjavac.com/nosql/2012/04/13/redis-persistence-demystified.html)
[链接又一](http://www.m690.com/archives/371)
[链接还一](http://blog.nosqlfan.com/html/199.html)
我就说说我碰到的问题吧~
我的测试环境：
os： win7 64bit
redis： 2.4.5 64bit
当以AOF方式持久化时，客户端运行BGREWRITEAOF命令会导致服务器端报错退出
Exiting on error writing to the append-only file: Bad file descriptor
具体细节可以围观[这里](http://www.dewen.org/q/12127/redis%E5%BC%80%E5%90%AFAOF%E5%90%8E%E6%89%A7%E8%A1%8CBGREWRITEAOF%E5%91%BD%E4%BB%A4%E5%B0%B1%E6%8A%A5%E9%94%99%E9%80%80%E5%87%BA)。
我搜索了一下，这个问题貌似不是大众问题，只有个别人碰见，看到2种解决方法，一个是说把配置文件里的dir参数改成绝对路径，我试了试没用，还有就是更换版本，所以我又下了一个redis 2.2.2 for window，依然用64bit版的来测试，这次就ok了！
然后引申出一个章节，redis官方是没有提供window版本的，我们下的应该都是民间的，那么我就不深究为什么2.4.5会报错了！
由于这个原因，我也把测试环境从win7换成了centOS6.3 x86_64，redis版本也用的是比较新的2.6.9。毕竟官方正版的才是王道啊！
然后是redis的主从，其实redis的主从配置真的不是一般的简单，看[这里](http://www.cnblogs.com/stephen-liu74/archive/2012/02/23/2364717.html)。
简单的都有点让人不好意思了！
但是查看了一下大牛们的分析，redis在集群和单点故障上还是存在缺陷的，要想搞清楚原因，就得看一些比较底层的资料，我列出一些：
链接[1](http://blog.nosqlfan.com/html/3153.html)，[2](http://blog.nosqlfan.com/html/3302.html)，[3](http://blog.nosqlfan.com/html/4077.html)
除此之外，还有redis的运维监控方面的文章不错，可以[看看](http://blog.nosqlfan.com/html/4166.html)
好了，先到这里，回头补充
# hiredis发布/订阅示例
发布订阅(pub/sub)是一种消息通信模式，主要的目的是解耦消息发布者和消息订阅者之间的耦合，这点和设计模式中的观察者模式比较相似。pub /sub不仅仅解决发布者和订阅者直接代码级别耦合也解决两者在物理部署上的耦合。[redis](http://www.w3ccollege.org/category/redis)作为一个pub/sub
 server，在订阅者和发布者之间起到了消息路由的功能。订阅者可以通过subscribe和psubscribe命令向[redis](http://www.w3ccollege.org/category/redis) server订阅自己感兴趣的消息类型，[redis](http://www.w3ccollege.org/category/redis)将消息类型称为通道(channel)。当发布者通过publish命令向[redis](http://www.w3ccollege.org/category/redis)server发送特定类型的消息时。订阅该消息类型的全部client都会收到此消息。这里消息的传递是多对多的。一个client可以订阅多个
 channel,也可以向多个channel发送消息。
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <signal.h>  
    #include "hiredis.h"  
    #include "async.h"  
    #include "adapters/libevent.h"void subCallback(redisAsyncContext *c, void*r, void*priv) {  
        redisReply *reply = r;  
        if (reply == NULL) return;  
        if ( reply->type == REDIS_REPLY_ARRAY && reply->elements ==3 ) {  
            if ( strcmp( reply->element[0]->str, "subscribe" ) !=0 ) {  
                printf( "Received[%s] channel %s: %s\n",  
                        (char*)priv,  
                        reply->element[1]->str,  
                        reply->element[2]->str );  
            }  
        }  
    }  
      
    void connectCallback(const redisAsyncContext *c, int status) {  
        if (status != REDIS_OK) {  
            printf("Error: %s\n", c->errstr);  
            return;  
        }  
        printf("Connected...\n");  
    }  
      
    void disconnectCallback(const redisAsyncContext *c, int status) {  
        if (status != REDIS_OK) {  
            printf("Error: %s\n", c->errstr);  
            return;  
        }  
        printf("Disconnected...\n");  
    }  
      
    int main (int argc, char**argv) {  
        signal(SIGPIPE, SIG_IGN);  
        struct event_base *base= event_base_new();  
      
        redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);  
        if (c->err) {  
            /* Let *c leak for now... */  
            printf("Error: %s\n", c->errstr);  
            return1;  
        }  
      
        redisLibeventAttach(c,base);  
        redisAsyncSetConnectCallback(c,connectCallback);  
        redisAsyncSetDisconnectCallback(c,disconnectCallback);  
        redisAsyncCommand(c, subCallback, (char*) "sub", "SUBSCRIBE foo");  
      
        event_base_dispatch(base);  
        return0;  
    }  
