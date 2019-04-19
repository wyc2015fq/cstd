# Redis的几个认识误区 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月08日 13:40:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1949
此文作者是新浪微博平台架构师杨卫华timyang大师，是他对Redis研究的一个总结，澄清了一些认识上的误区，值得一读。
James Hamilton在[On Designing and Deploying Internet-Scale Service](http://www.mvdirona.com/jrh/talksAndPapers/JamesRH_Lisa.pdf)中的第一条经验“Design
 for failure”是所有互联网架构成功的一个关键。
这1年我们经历了很多眼花缭乱的key value存储产品的诱惑，从Cassandra的淡出(Twitter暂停在主业务使用)到HBase的兴起([Facebook新的邮箱业务选用HBase](http://highscalability.com/blog/2010/11/16/facebooks-new-real-time-messaging-system-hbase-to-store-135.html))，当再回头再去看Redis，发现这个只有1万多行源代码的程序充满了神奇及大量未经挖掘的特性。Redis性能惊人，国内前十大网站的子产品估计用1台Redis就可以满足存储及Cache的需求。除了性能印象之外，业界其实普遍对Redis的认识存在一定误区。本文提出一些观点供大家探讨。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#)] 
- [1 Redis是什么](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#Redis.E6.98.AF.E4.BB.80.E4.B9.88)
- [2 Redis不可能比Memcache快？](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#Redis.E4.B8.8D.E5.8F.AF.E8.83.BD.E6.AF.94Memcache.E5.BF.AB.EF.BC.9F)
- [3 单台Redis的存放数据必须比物理内存小？](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#.E5.8D.95.E5.8F.B0Redis.E7.9A.84.E5.AD.98.E6.94.BE.E6.95.B0.E6.8D.AE.E5.BF.85.E9.A1.BB.E6.AF.94.E7.89.A9.E7.90.86.E5.86.85.E5.AD.98.E5.B0.8F.EF.BC.9F)
- [4 Redis的VM实现是重复造轮子？](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#Redis.E7.9A.84VM.E5.AE.9E.E7.8E.B0.E6.98.AF.E9.87.8D.E5.A4.8D.E9.80.A0.E8.BD.AE.E5.AD.90.EF.BC.9F)
- [5 用get/set方式使用Redis](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#.E7.94.A8get.2Fset.E6.96.B9.E5.BC.8F.E4.BD.BF.E7.94.A8Redis)
- [6 使用aof代替snapshot？](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#.E4.BD.BF.E7.94.A8aof.E4.BB.A3.E6.9B.BFsnapshot.EF.BC.9F)
- [7 小结](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#.E5.B0.8F.E7.BB.93)
- [8 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=Redis%E7%9A%84%E5%87%A0%E4%B8%AA%E8%AE%A4%E8%AF%86%E8%AF%AF%E5%8C%BA#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## Redis是什么
这个问题的结果影响了我们怎么用Redis。如果你认为Redis是一个key value store, 那可能会用它来代替MySQL；如果认为它是一个可以持久化的cache, 可能只是用它保存一些频繁访问的临时数据。还有一些看法则认为Redis是一个memory database，因为它的高性能都是基于内存的。另外一些人则认为Redis是一个data structure server，因为Redis支持复杂的数据特性，比如List,
 Set等。对Redis作用的不同解读决定了你对Redis的使用方式。
互联网数据目前基本使用两种方式来存储，关系数据库或者key value。但是这些互联网业务本身并不属于这两种数据类型，比如用户在社会化平台中的关系，它是一个list，如果要用关系数据库存储就需要转换成一种多行记录的形式，这种形式存在很多冗余数据，每一行需要存储一些重复信息。如果用key value存储则修改和删除又比较麻烦，需要将全部数据读出再写入。Redis在内存中设计了各种数据类型，让业务能够高速原子的访问这些数据结构，并且不需要关心持久存储的问题，从架构上解决了前面两种存储需要走一些弯路的问题。
## Redis不可能比Memcache快？
很多开发者都认为Redis不可能比Memcached快，Memcached完全基于内存，而Redis具有持久化保存特性，即使是异步的，Redis也不可能比Memcached快。但是测试结果基本是Redis占绝对优势。一直在思考这个原因，目前想到的原因有这几方面。
- Libevent。和Memcached不同，Redis并没有选择libevent。Libevent为了迎合通用性造成代码庞大(目前Redis代码还不到libevent的1/3)及牺牲了在特定平台的不少性能。Redis对libevent中的两个文件进行修改实现了自己的epoll event loop。业界不少开发者也建议Redis使用另外一个libevent的高性能替代libev，但是作者还是坚持Redis应该小巧并去依赖的思路。一个印象深刻的细节是编译Redis之前并不需要执行./configure。
- CAS问题。CAS(Check and Set)是Memcached中比较方便的一种防止竞争修改资源的方法。CAS实现需要为每个cache key设置一个隐藏的cas token，cas相当value版本号，每次set会token需要递增，因此带来CPU和内存的双重开销，虽然这些开销很小，但是到单机10G+ cache以及QPS(Query Per Second)上万之后这些开销就会给双方相对带来一些细微性能差别。
## 单台Redis的存放数据必须比物理内存小？
Redis的数据全部放在内存带来了高速的性能，但是也带来一些不合理之处。比如一个中型网站有100万注册用户，如果这些资料都要用Redis来存储，内存的容量必须能够容纳这100万用户。但是实际业务情况是100万用户只有5万活跃用户，1周来访问过1次的也只有15万用户，因此将全部100万用户的数据都放在内存中有不合理之处，RAM需要为冷数据买单。
这跟操作系统非常相似，操作系统所有应用要访问的数据都在内存，但是如果物理内存容纳不下新的数据，操作系统会智能地将部分长期没有访问过的数据交换到磁盘，为新的应用留出空间。现代操作系统给应用提供的并不是物理内存，而是虚拟内存(Virtual Memory)的概念。
基于相同的考虑，Redis 2.0也增加了VM特性。让Redis数据容量突破了物理内存的限制。并实现了数据冷热分离。
## Redis的VM实现是重复造轮子？
Redis的VM依照之前的epoll实现思路依旧是自己实现。但是在前面操作系统的介绍中提到OS也可以自动帮程序实现冷热数据分离，Redis只需要向OS申请一块大内存，OS会自动将热数据放入物理内存，冷数据交换到硬盘，另外一个知名的“理解了现代操作系统”的Varnish就是这样实现，也取得了非常成功的效果。
作者antirez在解释为什么要自己实现VM中提到几个原因。主要是OS的VM换入换出是基于Page概念，比如OS VM1个Page是4K, 4K中只要还有一个元素即使只有1个字节被访问，这个页也不会被SWAP, 换入也是同样道理，读到一个字节可能会换入4K无用的内存。而Redis自己实现则可以达到控制换入的粒度。另外访问操作系统SWAP内存区域时block进程，也是导致Redis要自己实现VM原因之一。
## 用get/set方式使用Redis
作为一个key value存在，很多开发者自然的使用set/get方式来使用Redis，实际上这并不是最优化的使用方法。尤其在未启用VM情况下，Redis全部数据需要放入内存，节约内存尤其重要。
假如一个key-value单元需要最小占用512字节，即使只存一个字节也占了512字节。这时候就有一个设计模式，可以把key复用，几个key-value放入一个key中，value再作为一个set存入，这样同样512字节就会存放10-100倍的容量。
这就是为了节约内存，建议使用hashset而不是set/get的方式来使用Redis。
## 使用aof代替snapshot？
Redis有两种存储方式，默认是snapshot方式，实现方法是定时将内存的快照(snapshot)持久化到硬盘，这种方式的缺点是持久化之后如果出现crash则会丢失一段数据。因此在完美主义者的推动下作者增加了aof方式。aof即append only mode，在写入内存数据的同时将操作命令保存到日志文件，在一个并发更改上万的系统中，命令日志是一个非常庞大的数据，管理维护成本非常高，恢复重建时间会非常长，这样导致失去aof高可用性本意。另外更重要的是Redis是一个内存数据结构模型，所有的优势都是建立在对内存复杂数据结构高效的原子操作上，这样就看出aof是一个非常不协调的部分。
其实aof目的主要是数据可靠性及高可用性，在Redis中有另外一种方法来达到这个目的：Replication。由于Redis的高性能，复制基本没有延迟。这样达到了防止单点故障及实现了高可用。
## 小结
要想成功使用一种产品，我们需要深入了解它的特性。Redis性能突出，如果能够熟练的驾驭，对国内很多大型应用具有很大帮助。希望更多同行加入到Redis使用及代码研究行列。
## 艺搜参考
[http://blog.nosqlfan.com/html/868.html](http://blog.nosqlfan.com/html/868.html)
[http://timyang.net/data/redis-misunderstanding/](http://timyang.net/data/redis-misunderstanding/)
