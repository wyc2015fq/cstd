# 基于Redis实现分布式锁，Redisson使用及源码分析 - z69183787的专栏 - CSDN博客
2018年02月21日 21:29:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：128
个人分类：[锁-分布式锁](https://blog.csdn.net/z69183787/article/category/6393629)
[http://blog.jobbole.com/99751/](http://blog.jobbole.com/99751/)
在分布式场景下，有很多种情况都需要实现最终一致性。在设计远程上下文的领域事件的时候，为了保证最终一致性，在通过领域事件进行通讯的方式中，可以共享存储（领域模型和消息的持久化数据源），或者做全局XA事务（两阶段提交，数据源可分开），也可以借助消息中间件（消费者处理需要能幂等）。通过Observer模式来发布领域事件可以提供很好的高并发性能，并且事件存储也能追溯更小粒度的事件数据，使各个应用系统拥有更好的自治性。
本文主要探讨另外一种实现分布式最终一致性的解决方案——采用分布式锁。基于分布式锁的解决方案，比如zookeeper,redis都是相较于持久化（如利用InnoDB行锁，或事务，或version乐观锁）方案提供了高可用性，并且支持丰富化的使用场景。 本文通过Java版本的redis分布式锁开源框架——Redisson来解析一下实现分布式锁的思路。
# 分布式锁的使用场景
如果是不跨限界上下文的情况，跟本地领域服务相关的数据一致性，尽量还是用事务来保证。但也有些无法用事务或者乐观锁来处理的情况，这些情况大多是对于一个共享型的数据源，有并发写操作的场景，但又不是对于单一领域的操作。
举个例子，还是用租书来比喻,A和B两个人都来租书，在查看图书的时候，发现自己想要看的书《大设计》库存仅剩一本。书店系统中，书作为一种商品，是在商品系统中，以Item表示出租商品的领域模型，同时每一笔交易都会产生一个订单，Order是在订单系统（交易限界上下文）中的领域模型。这里假设先不考虑跨系统通信的问题（感兴趣的可以参考下[领域服务、领域事件](http://blog.csdn.net/lijingyao8206/article/details/50867008)），也暂时不考虑支付环节，但是我们需要保证A,B两个人不会都对于《大设计》产生订单就可以，也就是其中一个人是可以成功下单，另外一个人只要提示库存已没即可。此时，书的库存就是一种共享的分布式资源，下订单，减库存就是一个需要保证一致性的写操作。但又因为两个操作不能在同一个本地事务，或者说，不共享持久化的数据源的情况，这时候就可以考虑用分布式锁来实现。本例子中，就需要对于共享资源——书的库存进行加锁，至于锁的key可以结合领域模型的唯一标识，如itemId,以及操作类型（如操作类型是RENT的）设计一个待加锁的资源标识。当然，这里还有一个并发性能的问题，如果是个库存很多的秒杀类型的业务，那么就不能单纯在itemId 加类型加锁，还需要设计排队队列以及合理的调度算法，防止超卖等等，那些就是题外话了。本文只是将这个场景作为一个切入点，具体怎么设计锁，什么场景用还要结合业务。
# 需要解决的问题
分布式的思路和线程同步锁ReentrantLock的思路是一样的。我们也要考虑如以下几个问题：
- 死锁的情况。复杂的网络环境下，当加锁成功，后续操作正在处理时，获得锁的节点忽然宕机，无法释放锁的情况。如A在Node1 节点申请到了锁资源，但是Node1宕机，锁一直无法释放，订单没有生成，但是其他用户将无法申请到锁资源。
- 锁的性能效率。分布式锁不能成为性能瓶颈或者单点故障不能导致业务异常。
- 如果关键业务，可能需要重入场景，是否设计成可重入锁。这个可以参考下在多线程的情况下，比如ReentrantLock就是一种可重入锁，其内部又提供了公平锁和非公平锁两种实现和应用，本文不继续探讨。带着以上问题，和场景，沿着下文，来一一找到解决方案。
# 基于Redis实现
## Redis 命令
在Redisson介绍前，回顾下Redis的命令，以及不通过任何开源框架，可以基于redis怎么设计一个分布式锁。基于不同应用系统实现的语言，也可以通过其他一些如Jedis,或者Spring的RedisOperations 等，来执行Reids命令[Redis command list](https://github.com/antirez/redis/blob/93e7a130fc9594e41ccfc996b5eca7626ae5356a/src/redis.c#L119)。
分布式锁主要需要以下redis命令，这里列举一下。在实现部分可以继续参照命令的操作含义。
- SETNX key value (SET if Not eXists):当且仅当 key 不存在，将 key 的值设为 value ，并返回1；若给定的 key 已经存在，则 SETNX 不做任何动作，并返回0。详见：[SETNX commond](http://redis.io/commands/setnx)
- GETSET key value:将给定 key 的值设为 value ，并返回 key 的旧值 (old value)，当 key 存在但不是字符串类型时，返回一个错误，当key不存在时，返回nil。详见：[GETSET commond](http://redis.io/commands/GETSET)
- GET key:返回 key 所关联的字符串值，如果 key 不存在那么返回 nil 。详见：[GET Commond](http://redis.io/commands/get)
- DEL key [KEY …]:删除给定的一个或多个 key ,不存在的 key 会被忽略,返回实际删除的key的个数（integer）。详见：[DEL Commond](http://redis.io/commands/del)
- HSET key field value：给一个key 设置一个{field=value}的组合值，如果key没有就直接赋值并返回1，如果field已有，那么就更新value的值，并返回0.详见：[HSET Commond](http://redis.io/commands/HSET)
- HEXISTS key field:当key 中存储着field的时候返回1，如果key或者field至少有一个不存在返回0。详见[HEXISTS Commond](http://redis.io/commands/hexists)
- HINCRBY key field increment:将存储在 key 中的哈希（Hash）对象中的指定字段 field 的值加上增量 increment。如果键 key 不存在，一个保存了哈希对象的新建将被创建。如果字段 field 不存在，在进行当前操作前，其将被创建，且对应的值被置为 0。返回值是增量之后的值。详见：[HINCRBY Commond](http://redis.io/commands/HINCRBY)
- PEXPIRE key milliseconds：设置存活时间，单位是毫秒。expire操作单位是秒。详见:[PEXPIRE Commond](http://redis.io/commands/pexpire)
- PUBLISH channel message:向channel post一个message内容的消息，返回接收消息的客户端数。详见[PUBLISH Commond](http://redis.io/commands/publish)
## Redis 实现分布式锁
假设我们现在要给itemId 1234 和下单操作 OP_ORDER 加锁，key是OP_ORDER_1234,结合上面的redis命令，似乎加锁的时候只要一个SETNX OP_ORDER_1234 currentTimestamp ，如果返回1代表加锁成功，返回0 表示锁被占用着。然后再用DEL OP_ORDER_1234解锁，返回1表示解锁成功，0表示已经被解锁过。然而却还存在着很多问题：SETNX会存在锁竞争，如果在执行过程中客户端宕机，也会引起死锁问题，即锁资源无法释放。并且当一个资源解锁的时候，释放锁之后，其他之前等待的锁没有办法再次自动重试申请锁（除非重新申请锁）。解决死锁的问题其实可以可以向Mysql的死锁检测学习，设置一个失效时间，通过key的时间戳来判断是否需要强制解锁。但是强制解锁也存在问题，一个就是时间差问题，不同的机器的本地时间可能也存在时间差，在很小事务粒度的高并发场景下还是会存在问题，比如删除锁的时候，在判断时间戳已经超过时效，有可能删除了其他已经获取锁的客户端的锁。另外，如果设置了一个超时时间，但是确实执行时间超过了超时时间，那么锁会被自动释放，原来持锁的客户端再次解锁的时候会出现问题，而且最为严重的还是一致性没有得到保障。
所以设计的时候需要考虑以下几点：
- 锁的时效设置。避免单点故障造成死锁，影响其他客户端获取锁。但是也要保证一旦一个客户端持锁，在客户端可用时不会被其他客户端解锁。（网上很多解决方案都是其他客户端等待队列长度判断是否强制解锁，但其实在偶发情况下就不能保证一致性，也就失去了分布式锁的意义）。
- 持锁期间的check,尽量在关键节点检查锁的状态，所以要设计成可重入锁，但在客户端使用时要做好吞吐量的权衡。
- 减少获取锁的操作，尽量减少redis压力。所以需要让客户端的申请锁有一个等待时间，而不是所有申请锁的请求要循环申请锁。
- 加锁的事务或者操作尽量粒度小，减少其他客户端申请锁的等待时间，提高处理效率和并发性。
- 持锁的客户端解锁后，要能通知到其他等待锁的节点，否则其他节点只能一直等待一个预计的时间再触发申请锁。类似线程的notifyAll,要能同步锁状态给其他客户端，并且是分布式消息。
- 考虑任何执行句柄中可能出现的异常，状态的正确流转和处理。比如，不能因为一个节点解锁失败，或者锁查询失败（redis 超时或者其他运行时异常），影响整个等待的任务队列，或者任务池。
### 锁设计
由于时间戳的设计有很多问题，以及上述几个问题，所以再换一种思路。先回顾几个关于锁的概念和经典java API。通过一些java.util.concurrent的API来处理一些本地队列的同步以及等待信号量的处理。
- Semaphore :Semaphore可以控制某个资源可被同时访问的个数，通过 acquire() 获取一个许可，如果没有就等待，而 release() 释放一个许可。其内部维护了一个int 类型的permits。有一个关于厕所的比喻很贴切，10个人在厕所外面排队，厕所有5个坑，只能最多进去五个人，那么就是初始化一个 permits=5的Semaphore。当一个人出来，会release一个坑位，其他等坑的人会被唤醒然后开始要有人进坑。Semaphore同ReentrantLock一样都是基于AbstractQueuedSynchronizer提供了公平锁和非公平锁两种实现。如果等待的人有秩序的排队等着，就说明选择了Semaphore的公平锁实现，如果外面的人没有秩序，谁抢到是谁的（活跃线程就会一直有机会，存在线程饥饿可能），那就是Semaphore的非公平锁实现。无论外面人怎么个等法Semaphore对于出坑的控制是一致的，每次只能是从一个坑里出来一个人。理解起来，其实就是厕所的5个坑位是一个共享资源，也就是permits的值=5,每次acquire一下就是外面来了个人排队，每次release一下就是里面出来个人。厕所聊多有点不雅观，再回归到分布式锁的话题。在刚才讲述的redis实现分布式锁的“第三点”，减少redis申请锁调用频率上就可以通过Semaphore来控制请求。虽然Semaphore只是虚拟机内部的锁粒度的实现（不能跨进程），但是也可以一定程度减轻最后请求redis节点的压力。当然，也有种方法是，随机sleep一段时间再去tryLock之类的，也可以达到减轻最后redis节点压力，但是毕竟使用信号量能更好得控制。而且我们可以再简单点，对于同一个锁对象的申请锁操作，可以设计一个初始化permits = 0的LockEntry,permits = 0也就顾名思义，谁都进不来，厕所维修中。当有一个持锁对象unlock的时候，通过分布式消息机制通知所有等待节点，这时候，再release，这时候permits=1，也就是本虚拟机中只能有一个线程能在acquire（）的阻塞中脱颖而出（当然只是进了坑，但不一定能获取得到分布式锁）。
- ConcurrentHashMap:这个应该不必多说，之谈谈在设计分布式锁中的用途。在上述的“第一点”，对于锁的时效性的设置里提到了，要在持锁线程正常运行（持锁节点没有宕机或内部异常）的时候，保证其一直占用锁。只要占着茅坑的人还在用着，只要他还没有暴毙或者无聊占着茅坑不XX,那就应该让外面的人都等着，不能强行开门托人。再收回来。。。这里ConcurrentHashMap的key无疑是锁对象的标识（我们需要设计的redis的key），value就是一个时间任务对象，比如可以netty的TimerTask或其他定时API，定时得触发给我的锁重新设置延时。这就是好比（好吧，再次用厕所比喻），蹲在里面的人的一种主动行为，隔1分钟敲两下厕所门，让外面的等的人知道，里面的人正在使用中，如果里面的人1分钟超过还没有敲门，可能是里面人挂掉了，那么再采取强制措施，直接开门拽人，释放坑位。
并发API以及一些框架的使用主要是控制锁的进入和调度，加锁的流程以及锁的逻辑也是非常重要。因为redis支持hash结构，除了key作为锁的标识，还可以利用value的结构
### 加锁
下面参数的含义先说明下 :
- KEYS[1] ：需要加锁的key，这里需要是字符串类型。
- ARGV[1] ：锁的超时时间，防止死锁
- ARGV[2] ：锁的唯一标识，也就是刚才介绍的 id（UUID.randomUUID()） + “:” + threadId

Java
``
以上的方法，当返回空是，说明获取到锁，如果返回一个long数值（pttl 命令的返回值），说明锁已被占用，通过返回剩余时间，外部可以做一些等待时间的判断和调整。
### 解锁
也还是先说明一下参数信息：
– KEYS[1] ：需要加锁的key，这里需要是字符串类型。
– KEYS[2] ：redis消息的ChannelName,一个分布式锁对应唯一的一个channelName:“redisson_lock__channel__{” + getName() + “}”
– ARGV[1] ：reids消息体，这里只需要一个字节的标记就可以，主要标记redis的key已经解锁，再结合redis的Subscribe，能唤醒其他订阅解锁消息的客户端线程申请锁。
– ARGV[2] ：锁的超时时间，防止死锁
– ARGV[3] ：锁的唯一标识，也就是刚才介绍的 id（UUID.randomUUID()） + “:” + threadId

Java
``
这就是解锁过程，当然建议提供强制解锁的接口，直接删除key，以防一些紧急故障出现的时候，关键业务节点受到影响。这里还有一个关键点，就是publish命令，通过在锁的唯一通道发布解锁消息，可以减少其他分布式节点的等待或者空转，整体上能提高加锁效率。至于redis的消息订阅可以有多种方式，基于Jedis的订阅API或者Spring的MessageListener都可以实现订阅，这里就可以结合刚才说的Semaphore,在第一次申请锁失败后acquire,接收到分布式消息后release就可以控制申请锁流程的再次进入。下面结合Redisson源码，相信会有更清晰的认识。
# 使用Redisson示例
Redisson使用起来很方便，但是需要redis环境支持eval命令，否则一切都是悲剧，比如me.结果还是要用RedisCommands去写一套。例子就如下，获得一个RLock锁对象，然后tryLock 和unlock。trylock方法提供了锁重入的实现，并且客户端一旦持有锁，就会在能正常运行期间一直持有锁，直到主动unlock或者节点故障，主动失效（超过默认的过期时间）释放锁。

Java
``
Redisson还提供了设置最长等待时间以及设置释放锁时间的含参tryLock接口 boolean tryLock(long waitTime, long leaseTime, TimeUnit unit) throws InterruptedException; 。Redisson的lock 扩展了java.util.concurrent.locks.Lock的实现，也基本按照了Lock接口的实现方案。lock（）方法会一直阻塞申请锁资源，直到有可用的锁释放。下面一部分会详细解析一部分关键实现的代码。
# Redisson源码解析
Redisson 的异步任务(Future,Promise，FutureListener API)，任务计时器(Timeout，TimerTask)，以及通过AbstractChannel连接redis以及写入执行批处理命令等很多都是基于netty框架的。po主因为不能使用eval,所以用Spring提供的redisApi ,RedisOperations来处理redis指令，异步调度等用了Spring的AsyncResult，MessageListener以及一些concurrent api。这里还是先看一下Redisson的实现。
## trylock
这里以带参数的trylock解析一下，无参的trylock是一种默认参数的实现。先源码走读一下。

Java
``
上述方法，调用加锁的逻辑就是在tryAcquire(long leaseTime, TimeUnit unit)中

Java
``
tryAcquire(long leaseTime, TimeUnit unit)只是针对leaseTime的不同参数进行不同的转发处理，再提一下，trylock的无参方法就是直接调用了get(tryLockInnerAsync(Thread.currentThread().getId()));
所以下面再看核心的tryLockInnerAsync 基本命令已经在之前解析过，相信这里看起来应该比较轻松，返回的是一个future对象，是为了异步处理IO，提高系统吞吐量。

Java
``
再说明一下，tryLock(long waitTime, long leaseTime, TimeUnit unit)有leaseTime参数的申请锁方法是会按照leaseTime时间来自动释放锁的。但是没有leaseTime参数的，比如tryLock()或者tryLock(long waitTime, TimeUnit unit)以及lock()是会一直持有锁的。再来看一下没有leaseTime参数的tryLockInnerAsync(Thread.currentThread().getId())

Java
``
这里比有leaseTime参数的trylock就多了异步scheduleExpirationRenewal调度。可以继续看一下,这里的expirationRenewalMap就是之前降到的一个ConcurrentMap结构。下面的这个调度方式很精妙。除非被unlock的cancleTask方法触发，否则会一直循环重置过期时间。

Java
``
这个任务，其实还有一个问题，个人觉得在expirationRenewalMap.containsKey判断时也加上isLocked判断会比较好，以防止unlock时出现redis节点异常的时候，任务没有办法自动停止，或者设置一个最大执行次数的限制也可以，否则极端情况下也会耗尽本地节点的CPU资源。
## unlock
解锁的逻辑相对简单，如下，redis 命令相信看起来也会比较轻松了。

Java
``
这里的 cancelExpirationRenewal对应着取消 scheduleExpirationRenewal的重置expire时间任务。

Java
``
再看一下Redisson是如何处理unlock的redis消息的。这里的消息内容就是unlockMessage = 0L和unlock方法中publish的内容是对应的。

Java
``
Redisson还支持Redis的多种集群配置，一主一备，一主多备，单机等等。也是通过netty的EventExecutorGroup，Promise，Future等API实现调度的。
# 结语
在思考是否采用分布式锁以及采用哪种实现方案的时候，还是要基于业务，技术方案一定是基于业务基础，服务于业务，并且衡量过投入产出比的。所以如果有成熟的解决方案，在业务可承受规模肯定是不要重复造轮子，当然还要经过严谨的测试。在po主用Spring的redis api实现时，也遇到了一些问题。
比如hIncrBy 的字符集问题，在使用命令的时候，当然可以直接set a 1然后incr a 1，这个问题可以参考[ERR value is not an integer or out of range 问题](http://www.hydrogen18.com/blog/first-impression-redis.html)，但在使用RedisConnection的时候，需要通过转码，byte[] value =SafeEncoder.encode(String.valueOf(“1”)) 再 connection.hSet(key, field, value)这样才可以，或者自己通过String转成正确的编码也可以。
还有刚才说的调度pexpire任务，在unlock异常的时候，任务池中的任务无法自动结束。另外就是Spring的MessageListener的onMessage(Message message, byte[] pattern)回调方法message.getBody()是byte数组，消息内容转化的时候要处理一下。
## 资源
- [领域服务、领域事件](http://blog.csdn.net/lijingyao8206/article/details/50867008)
- [几种并发锁性能对比](http://baptiste-wicht.com/posts/2010/09/java-synchronization-mutual-exclusion-benchmark.html)
- [ERR value is not an integer or out of range 问题](http://www.hydrogen18.com/blog/first-impression-redis.html)
- [Redis 命令查询](http://redis.io/commands)
- [Redisson github](https://github.com/mrniko/redisson)
