# redis简介 - Big Smile - CSDN博客
2017年03月26日 22:14:38[王啸tr1912](https://me.csdn.net/tr1912)阅读数：418
        小编最近在研究redis，先有一篇简介奉上。
# 一、什么是redis？
        Redis是一个开源的使用ANSI C语言编写、支持网络、可基于内存亦可持久化的日志型、Key-Value数据库，并提供多种语言的API。从2010年3月15日起，Redis的开发工作由VMware主持。从2013年5月开始，Redis的开发由Pivotal赞助。
       redis是一个key-value存储系统。和Memcached类似，它支持存储的value类型相对更多，包括string(字符串)、list(链表)、set(集合)、zset(sorted set --有序集合)和hash（哈希类型）。这些数据类型都支持push/pop、add/remove及取交集并集和差集及更丰富的操作，而且这些操作都是原子性的。在此基础上，redis支持各种不同方式的排序。与memcached一样，为了保证效率，数据都是缓存在内存中。区别的是redis会周期性的把更新的数据写入磁盘或者把修改操作写入追加的记录文件，并且在此基础上实现了master-slave(主从)同步。
        Redis 是一个高性能的key-value数据库。 redis的出现，很大程度补偿了memcached这类key/value存储的不足，在部 分场合可以对关系数据库起到很好的补充作用。它提供了Java，C/C++，C#，PHP，JavaScript，Perl，Object-C，Python，Ruby，Erlang等客户端，使用很方便。
        Redis支持主从同步。数据可以从主服务器向任意数量的从服务器上同步，从服务器可以是关联其他从服务器的主服务器。这使得Redis可执行单层树复制。存盘可以有意无意的对数据进行写操作。由于完全实现了发布/订阅机制，使得从数据库在任何地方同步树时，可订阅一个频道并接收主服务器完整的消息发布记录。同步对读取操作的可扩展性和数据冗余很有帮助。
        redis的官网地址，非常好记，是redis.io。（特意查了一下，域名后缀io属于国家域名，是british Indian Ocean territory，即英属印度洋领地）。
其实通俗的来讲，redis就是一个以内存操纵为基础的，可以实现集群和持久化的一种非关系型的缓存机制，基于强大的管理和同步。
        非关系可以理解为是基于键值对的关系，然而redis中的数据关系不只是键值对这么简单。
# 二、数据类型
        Redis中的数据类型主要有一下几种：
- String
- Hash
- List
- Set
- Sorted set
- pub/sub
- Transaction
![](https://img-blog.csdn.net/20170326215835564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  首先Redis内部使用一个redisObject对象来表示所有的key和value,redisObject最主要的信息如上图所示：
         type代表一个value对象具体是何种数据类型，
         encoding是不同数据类型在redis内部的存储方式，
         比如：type=string代表value存储的是一个普通字符串，那么对应的encoding可以是raw或者是int,如果是int则代表实际redis内部是按数值型类存储和表示这个字符串的，当然前提是这个字符串本身可以用数值表示，比如:"123" "456"这样的字符串。
        具体的类型学习以后博客中会有提到。
# 三、为什么用redis
        缓存技术方面说到redis大家必然会联想到memcached，了解它们的人应该都知道以下几点吧
- redis与 memcached相比，redis支持key-value数据类型，同事支持list、set、hash等数据结构的存储。
- redis支持数据的备份，即master-slave模式的数据备份。
- redis支持数据的持久化。
- redis在很多方面支持数据库的特性，可以这样说他就是一个数据库系统，而memcached只是简单地K/V缓存。
      它们在性能方面差别不是很大，读取方面尤其是针对批量读取性能方面memcached占据优势。当然redis也有他的优点，如持久性、支持更多的数据结构。
      所以在选择方面如果有持久方面的需求或对数据类型和处理有要求的应该选择redis。
如果简单的key/value 存储应该选择memcached。
      如果你不知道什么是持久，那么简单的讲，持久就是可以持续性存储，因为memcached是将数据存储在内存中的，所以只要有断电或者系统损坏，那么数据基本就不保了，而redis虽然也是存储在内存中，但是他的持久化机制可以在一些时候把数据同步到硬盘上，减少数据损坏丢失。
# 四、总结
        继之前了解过的memcached之后又了解了很多关于redis的知识 ，发现他们虽然形式上的东西差不多，但是实际应用和数据管理上面还是有很多不同的地方，我们需要好好理解和适时的使用他们。
        数据类型的问题和区别的问题在以后的学习中会逐步整理出博客供大家参考。
