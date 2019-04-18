# 如何使用Redis做MySQL的缓存 - weixin_33985507的博客 - CSDN博客
2018年09月18日 10:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：37
应用Redis实现数据的读写，同时利用队列处理器定时将数据写入mysql。
同时要注意避免冲突，在redis启动时去mysql读取所有表键值存入redis中，往redis写数据时，对redis主键自增并进行读取，若mysql更新失败，则需要及时清除缓存及同步redis主键。
这样处理，主要是实时读写redis，而mysql数据则通过队列异步处理，缓解mysql压力，不过这种方法应用场景主要基于高并发，而且redis的高可用集群架构相对更复杂，一般不是很推荐。
![](https://img2018.cnblogs.com/blog/163758/201809/163758-20180918102806084-717500736.png)
**redis如何做到和mysql数据库的同步**
**【方案一】**
http://www.zhihu.com/question/23401553?sort=created
程序实现mysql更新、添加、删除就删除redis数据。
程序查询redis，不存在就查询mysql并保存redi
redis和mysql数据的同步，代码级别大致可以这样做：
读: 读redis->没有，读mysql->把mysql数据写回redi
写: 写mysql->成功，写redis（捕捉所有mysql的修改，写入和删除事件，对redis进行操作）
**【方案二】**
http://www.linuxidc.com/Linux/2015-01/380.htm
实时获取mysql binlog进行解析，然后修改redi
MySQL到Redis数据方案
无论MySQL还是Redis，自身都带有数据同步的机制，像比较常用的MySQL的Master/Slave模式，就是由Slave端分析Master的binlog来实现的，这样的数据其实还是一个异步过程，只不过当服务器都在同一内网时，异步的延迟几乎可以忽略。
那么理论上我们也可以用同样方式，分析MySQL的binlog文件并将数据插入Redis。但是这需要对binlog文件以及MySQL有非常深入的理解，同时由于binlog存在Statement/Row/Mixedlevel多种形式，分析binlog实现同步的工作量是非常大的。
因此这里选择了一种开发成本更加低廉的方式，借用已经比较成熟的MySQL UDF，将MySQL数据首先放入Gearman中，然后通过一个自己编写的PHP Gearman Worker，将数据同步到Redis。比分析binlog的方式增加了不少流程，但是实现成本更低，更容易操作。
**【方案三】**
使用mysql的udf，详情请看MySQL :: MySQL 5.1 Reference Manual :: 22.3 Adding New Functions to MySQL 然后通过trigger在表update和insert之后进行函数的调用，写入到redis中去。大致是这个样子。
【http://www.zhihu.com/question/27738066】
1.首先明确是不是一定要上缓存，当前架构的瓶颈在哪里，若瓶颈真是数据库操作上，再继续往下看。
2.明确memcached和redis的区别，到底要使用哪个。前者终究是个缓存，不可能永久保存数据（LRU机制），支持分布式，后者除了缓存的同时也支持把数据持久化到磁盘等，redis要自己去实现分布式缓存（貌似最新版本的已集成），自己去实现一致性hash。因为不知道你们的应用场景，不好说一定要用memcache还是redis，说不定用mongodb会更好，比如在存储日志方面。
3.缓存量大但又不常变化的数据，比如评论。
4.你的思路是对的，清晰明了，读DB前，先读缓存，如果有直接返回，如果没有再读DB，然后写入缓存层并返回。
5.考虑是否需要主从，读写分离，考虑是否分布式部署，考虑是否后续水平伸缩。
6.想要一劳永逸，后续维护和扩展方便，那就将现有的代码架构优化，按你说的替换数据库组件需要改动大量代码，说明当前架构存在问题。可以利用现有的一些框架，比如SpringMVC，将你的应用层和业务层和数据库层解耦。再上缓存之前把这些做好。
7.把读取缓存等操作做成服务组件，对业务层提供服务，业务层对应用层提供服务。
8.保留原始数据库组件，优化成服务组件，方便后续业务层灵活调用缓存或者是数据库。
9.不建议一次性全量上缓存，最开始不动核心业务，可以将边缘业务先换成缓存组件，一步步换至核心业务。
10.刷新内存，以memcached为例，新增，修改和删除操作，一般采用lazy load的策略，即新增时只写入数据库，并不会马上更新Memcached，而是等到再次读取时才会加载到Memcached中，修改和删除操作也是更新数据库，然后将Memcached中的数据标记为失效，等待下次读取时再加载。
