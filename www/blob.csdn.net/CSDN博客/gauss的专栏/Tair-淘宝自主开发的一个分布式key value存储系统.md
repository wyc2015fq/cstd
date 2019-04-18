# Tair-淘宝自主开发的一个分布式key/value存储系统 - gauss的专栏 - CSDN博客
2015年07月26日 17:06:15[gauss](https://me.csdn.net/mathlmx)阅读数：2503
## 简介
> 
tair 是淘宝自己开发的一个分布式 key/value 存储引擎. tair 分为持久化和非持久化两种使用方式. 非持久化的 tair
可以看成是一个分布式缓存. 持久化的 tair 将数据存放于磁盘中. 为了解决磁盘损坏导致数据丢失, tair 可以配置数据的备份数目,
tair 自动将一份数据的不同备份放到不同的主机上, 当有主机发生异常, 无法正常提供服务的时候, 其于的备份会继续提供服务.
### tair 的总体结构
> 
tair 作为一个分布式系统, 是由一个中心控制节点和一系列的服务节点组成. 我们称中心控制节点为config server.
服务节点是data server. config server 负责管理所有的data server, 维护data server的状态信息.
data server 对外提供各种数据服务, 并以心跳的形式将自身状况汇报给config server. config server是控制点,
而且是单点, 目前采用一主一备的形式来保证其可靠性. 所有的 data server 地位都是等价的.
![](http://www.lvtao.net/content/uploadfile/201312/3bdf79391dc2ddf5c52a67d08d87ad3720131225075456.jpg)
### tair 的负载均衡算法是什么
> 
tair 的分布采用的是一致性哈希算法, 对于所有的key, 分到Q个桶中, 桶是负载均衡和数据迁移的基本单位. config server
根据一定的策略把每个桶指派到不同的data server上. 因为数据按照key做hash算法, 所以可以认为每个桶中的数据基本是平衡的.
保证了桶分布的均衡性, 就保证了数据分布的均衡性.
![](http://www.lvtao.net/content/uploadfile/201312/2a4ba3d47c43013ec31257aee3be666d20131225075456.jpg)
### 增加或者减少data server的时候会发生什么
> 
当有某台data server故障不可用的时候, config server会发现这个情况, config
server负责重新计算一张新的桶在data server上的分布表, 将原来由故障机器服务的桶的访问重新指派到其它的data server中.
这个时候, 可能会发生数据的迁移. 比如原来由data server A负责的桶, 在新表中需要由 B负责. 而B上并没有该桶的数据,
那么就将数据迁移到B上来. 同时config server会发现哪些桶的备份数目减少了, 然后根据负载情况在负载较低的data
server上增加这些桶的备份. 当系统增加data server的时候, config server根据负载, 协调data
server将他们控制的部分桶迁移到新的data server上. 迁移完成后调整路由. 当然, 系统中可能出现减少了某些data
server 同时增加另外的一些data server. 处理原理同上. 每次路由的变更, config
server都会将新的配置信息推给data server. 在客户端访问data server的时候, 会发送客户端缓存的路由表的版本号.
如果data server发现客户端的版本号过旧, 则会通知客户端去config server取一次新的路由表. 如果客户端访问某台data
server 发生了不可达的情况(该 data server可能宕机了), 客户端会主动去config server取新的路由表.
### 发生迁移的时候data server如何对外提供服务
> 
当迁移发生的时候, 我们举个例子, 假设data server A 要把 桶 3,4,5 迁移给data server B. 因为迁移完成前,
客户端的路由表没有变化, 客户端对 3, 4, 5 的访问请求都会路由到A. 现在假设 3还没迁移, 4 正在迁移中, 5已经迁移完成.
那么如果是对3的访问, 则没什么特别, 跟以前一样. 如果是对5的访问, 则A会把该请求转发给B,并且将B的返回结果返回给客户,
如果是对4的访问, 在A处理, 同时如果是对4的修改操作, 会记录修改log.当桶4迁移完成的时候, 还要把log发送到B,
在B上应用这些log. 最终A B上对于桶4来说, 数据完全一致才是真正的迁移完成. 当然, 如果是因为某data
server宕机而引发的迁移, 客户端会收到一张中间临时状态的分配表. 这张表中, 把宕机的data
server所负责的桶临时指派给有其备份data server来处理. 这个时候, 服务是可用的, 但是负载可能不均衡. 当迁移完成之后,
才能重新达到一个新的负载均衡的状态.
### 桶在data server上分布时候的策略
> 
程序提供了两种生成分配表的策略, 一种叫做负载均衡优先, 一种叫做位置安全优先: 我们先看负载优先策略. 当采用负载优先策略的时候,
config server会尽量的把桶均匀的分布到各个data server上. 所谓尽量是指在不违背下面的原则的条件下尽量负载均衡. 1
每个桶必须有COPY_COUNT份数据 2 一个桶的各份数据不能在同一台主机上; 位置安全优先原则是说, 在不违背上面两个原则的条件下,
还要满足位置安全条件, 然后再考虑负载均衡. 位置信息的获取是通过 _pos_mask(参见安装部署文档中关于配置项的解释) 计算得到.
一般我们通过控制 _pos_mask 来使得不同的机房具有不同的位置信息. 那么在位置安全优先的时候, 必须被满足的条件要增加一条,
一个桶的各份数据不能都位于相同的一个位置(不在同一个机房). 这里有一个问题, 假如只有两个机房, 机房1中有100台data server,
机房2中只有1台data server. 这个时候, 机房2中data server的压力必然会非常大. 于是这里产生了一个控制参数
_build_diff_ratio(参见安装部署文档). 当机房差异比率大于这个配置值时, config server也不再build新表.
机房差异比率是如何计出来的呢? 首先找到机器最多的机房, 不妨设使RA, data server数量是SA. 那么其余的data
server的数量记做SB. 则机房差异比率=|SA – SB|/SA. 因为一般我们线上系统配置的COPY_COUNT是3. 在这个情况下,
不妨设只有两个机房RA和RB, 那么两个机房什么样的data server数量是均衡的范围呢? 当差异比率小于
0.5的时候是可以做到各台data server负载都完全均衡的.这里有一点要注意, 假设RA机房有机器6台,RB有机器3台. 那么差异比率 =
6 – 3 / 6 = 0.5. 这个时候如果进行扩容, 在机房A增加一台data server, 扩容后的差异比率 = 7 – 3 / 7 =
0.57. 也就是说, 只在机器数多的机房增加data server会扩大差异比率.
如果我们的_build_diff_ratio配置值是0.5. 那么进行这种扩容后, config server会拒绝再继续build新表.
### tair 的一致性和可靠性问题
> 
分布式系统中的可靠性和一致性是无法同时保证的, 因为我们必须允许网络错误的发生. tair 采用复制技术来提高可靠性,
并且为了提高效率做了一些优化, 事实上在没有错误发生的时候, tair 提供的是一种强一致性. 但是在有data server发生故障的时候,
客户有可能在一定时间窗口内读不到最新的数据. 甚至发生最新数据丢失的情况.
### tair提供的客户端
> 
tair 的server端是C++写的, 因为server和客户端之间使用socket通信,
理论上只要可以实现socket操作的语言都可以直接实现成tair客户端. 目前实际提供的客户端有java 和 C++.
客户端只需要知道config server的位置信息就可以享受tair集群提供的服务了.
## 主要的性能数据
待补充
# 1. Tair总述
## 1.1 系统架构
一个Tair集群主要包括3个必选模块：configserver、dataserver和client，一个可选模块：invalidserver。通
常情况下，一个集群中包含2台configserver及多台dataServer。两台configserver互为主备并通过维护和
dataserver之间的心跳获知集群中存活可用的dataserver，构建数据在集群中的分布信息（对照表）。dataserver负责数据的存
储，并按照configserver的指示完成数据的复制和迁移工作。client在启动的时候，从configserver获取数据分布信息，根据数据
分布信息和相应的dataserver交互完成用户的请求。invalidserver主要负责对等集群的删除和隐藏操作，保证对等集群的数据一致。
从架构上看，configserver的角色类似于传统应用系统的中心节点，整个集群服务依赖于configserver的正常工作。但实际上相对来
说，tair的configserver是非常轻量级的，当正在工作的服务器宕机的时候另外一台会在秒级别时间内自动接管。而且，如果出现两台服务器同时
宕机的最恶劣情况，只要应用服务器没有新的变化，
tair依然服务正常。而有了configserver这个中心节点，带来的好处就是应用在使用的时候只需要配置configserver的地址（现在可
以直接配置Diamond key），而不需要知道内部节点的情况。
![](http://www.lvtao.net/content/uploadfile/201312/4475694b074835621e8690e07899b37920131225075457.jpg)
### 1.1.1 ConfigServer的功能
1) 通过维护和dataserver心跳来获知集群中存活节点的信息
2) 根据存活节点的信息来构建数据在集群中的分布表。
3) 提供数据分布表的查询服务。
4) 调度dataserver之间的数据迁移、复制。
### 1.1.2 DataServer的功能
1) 提供存储引擎
2) 接受client的put/get/remove等操作
3) 执行数据迁移，复制等
4) 插件：在接受请求的时候处理一些自定义功能
5) 访问统计
### 1.1.3 InvalidServer的功能
1) 接收来自client的invalid/hide等请求后，对属于同一组的集群（双机房独立集群部署方式）做delete/hide操作，保证同一组集群的一致。
2) 集群断网之后的，脏数据清理。
3) 访问统计。
### 1.1.4 client的功能
1) 在应用端提供访问Tair集群的接口。
2) 更新并缓存数据分布表和invalidserver地址等。
3) LocalCache，避免过热数据访问影响tair集群服务。
4) 流控
## 1.2 存储引擎与应用场景
Tair经过这两年的发展演进，除了应用于cache缓存外，在存储（持久化）上支持的应用需求也越来越广泛。现在主要有mdb，rdb，ldb三种存储引擎。
### 1.2.1 mdb
定位于cache缓存，类似于memcache。
支持k/v存取和prefix操作
1.2.1.1 mdb的应用场景
在实际业务中，大部分当缓存用（后端有DB之类的数据源）。
也可用做大访问少量临时数据的存储（例如session登录，防攻击统计等）。
集团内绝对多数cache服务都是采用的tair mdb。
### 1.2.2 rdb
定位于cache缓存，采用了redis的内存存储结构。
支持k/v，list，hash，set，sortedset等数据结构。
1.2.2.1 rdb的应用场景
适用于需要高速访问某些数据结构的应用，例如SNS中常见的的粉丝存储就可以采用set等结构；或者存储一个商品的多个属性（hashmap）；高效的消息队列（list）等。现在有30个左右的应用在使用rdb服务。
### 1.2.3 ldb
定位于高性能存储，并可选择内嵌mdb cache加速，这种情况下cache与持久化存储的数据一致性由tair进行维护。
支持k/v，prefix等数据结构。今后将支持list，hash，set，sortedset等redis支持的数据结构。
1.2.3.1 ldb的应用场景
存储，里面可以细分如下场景：
1) 持续大数据量的存入读取，类似淘宝交易快照。
2) 高频度的更新读取，例如计数器，库存等。
3) 离线大批量数据导入后做查询。参见fastdump
也可以用作cache：
数据量大，响应时间敏感度不高的cache需求可以采用。例如天猫实时推荐。
## 1.3 基本概念
### 1.3.1 configID
唯一标识一个tair集群，每个集群都有一个对应的configID，在当前的大部分应用情况下configID是存放在diamond中的，对应了该集
群的configserver地址和groupname。业务在初始化tairclient的时候需要配置此ConfigID。
### 1.3.2 namespace
又称area， 是tair中分配给应用的一个内存或者持久化存储区域， 可以认为应用的数据存在自己的namespace中。
同一集群（同一个configID）中namespace是唯一的。
通过引入namespace，我们可以支持不同的应用在同集群中使用相同的key来存放数据，也就是key相同，但内容不会冲突。一个namespace
下是如果存放相同的key，那么内容会受到影响，在简单K/V形式下会被覆盖，rdb等带有数据结构的存储引擎内容会根据不同的接口发生不同的变化。
### 1.3.3 quota配额
对应了每个namespace储存区的大小限制，超过配额后数据将面临最近最少使用（LRU）的淘汰。
持久化引擎（ldb）本身没有配额，ldb由于自带了mdb cache，所以也可以设置cache的配额。超过配额后，在内置的mdb内部进行淘汰。
1.3.3.1 配额是怎样计算的
配额大小直接影响数据的命中率和资源利用效率，业务方需要给出一个合适的值，通常的计算方法是评估在保证一定命中率情况下所需要的记录条数，这样配额大小即为： 记录条数 * 平均单条记录大小。
1.3.3.2 管理员如何配置配额
单份数据情况下，业务提供的配额就是需要配置在Tair系统中的配额。但对于多备份，在系统中实际计算的配额为： 业务配额 *　备份数
### 1.3.4 expireTime:过期时间
expiredTime
是指数据的过期时间，当超过过期时间之后，数据将对应用不可见，这个设置同样影响到应用的命中率和资源利用率。不同的存储引擎有不同的策略清理掉过期的数
据。调用接口时，expiredTime单位是秒，可以是相对时间（比如：30s），也可以是绝对时间（比如：当天23时,转换成距1970-1-1
00:00:00的秒数）。
小于0，不更改之前的过期时间
如果不传或者传入0，则表示数据永不过期；
大于0小于当前时间戳是相对时间过期；
大于当前时间戳是绝对时间过期；
### 1.3.5 version
Tair中存储的每个数据都有版本号，版本号在每次更新后都会递增，相应的，在Tair put接口中也有此version参数，这个参数是为了解决并发更新同一个数据而设置的，类似于乐观锁。
很多情况下，更新数据是先get，修改get回来的数据，然后put回系统。如果有多个客户端get到同一份数据，都对其修改并保存，那么先保存的修改就
会被后到达的修改覆盖，从而导致数据一致性问题,在大部分情况下应用能够接受，但在少量特殊情况下，这个是我们不希望发生的。
比如系统中有一个值”1”,
现在A和B客户端同时都取到了这个值。之后A和B客户端都想改动这个值，假设A要改成12，B要改成13，如果不加控制的话，无论A和B谁先更新成功，它
的更新都会被后到的更新覆盖。Tair引入的version机制避免了这样的问题。刚刚的例子中，假设A和B同时取到数据，当时版本号是10，A先更新，
更新成功后，值为12，版本为11。当B更新的时候，由于其基于的版本号是10，此时服务器会拒绝更新，返回version
error，从而避免A的更新被覆盖。B可以选择get新版本的value，然后在其基础上修改，也可以选择强行更新。
1.3.5.1 如何获取到当前key的version
get接口返回的是DataEntry对象，该对象中包含get到的数据的版本号，可以通过getVersion()接口获得该版本号。在put时，将该
版本号作为put的参数即可。 如果不考虑版本问题，则可设置version参数为0，系统将强行覆盖数据，即使版本不一致。
1.3.5.2 version是如何改变的
Version改变的逻辑如下：
1) 如果put新数据且没有设置版本号，会自动将版本设置成1。
2) 如果put是更新老数据且没有版本号，或者put传来的参数版本与当前版本一致，版本号自增1。
3) 如果put是更新老数据且传来的参数版本与当前版本不一致，更新失败，返回VersionError。
4) put时传入的version参数为0，则强制更新成功，版本号自增1。
1.3.5.3 version返回不一致的时候，该如何处理
如果更新所基于的version和系统中当前的版本不一致，则服务器会返回ResultCode.VERERROR。
这时你可以重新get数据，然后在新版本的数据上修改；或者设置version为0重新请求，以达到强制更新的效果，应用可以根据自身对数据一致性的要求
在这两种策略间进行选择。
1.3.5.4 version具体使用案例
如果应用有10个client会对key进行并发put，那么操作过程如下：
1) get key。如果get key成功，则进入步骤2；如果数据不存在，则进入步骤3.
2) 在调用put的时候将get key返回的verison重新传入put接口。服务端根据version是否匹配来返回client是否put成功。
3) get
key数据不存在，则新put数据。此时传入的version必须不是0和1，其他的值都可以（例如1000，要保证所有client是一套逻辑）。因为
传入0，tair会认为强制覆盖；而传入1，第一个client写入会成功，但是新写入时服务端的version以0开始计数啊，所以此时version
也是1，所以下一个到来的client写入也会成功，这样造成了冲突
1.3.5.5 version分布式锁
Tair中存在该key，则认为该key所代表的锁已被lock；不存在该key，在未加锁。操作过程和上面相似。业务方可以在put的时候增加expire，已避免该锁被长期锁住。
当然业务方在选择这种策略的情况下需要考虑并处理Tair宕机带来的锁丢失的情况。
1.3.5.6 什么情况下需要使用version
业务对数据一致性有较高的要求，并且访问并发高，那么通过version可以避免数据的意外结果。
如果不关心并发，那么建议不传入version或者直接传0。
## 1.4 集群部署方式
Tair通过多种集群部署方式，来满足各类应用的容灾需求。
下面所述的双机房可以扩展到多机房，现阶段基本还是采用的双机房。
现总共有4种方式：
mdb存储引擎适用于双机房单集群单份，双机房独立集群，双机房单集群双份。
rdb存储引擎适用于双机房单集群单份。
ldb存储引擎适用于双机房主备集群，双机房单集群单份。
### 1.4.1 双机房单集群单份
双机房单集群单备份数是指，该Tair集群部署在两个机房中（也就是该Tair集群的机器分别在两个机房）， 数据存储份数为1， 该类型集群部署示意图如下所示。数据服务器（Dataserver）分布在两个机房中，他们都属于同一集群。
> 
![](http://www.lvtao.net/content/uploadfile/201312/03a6fbd2dd528d355d582becb823ce8920131225075457.jpg)
使用场景：
1) 后端有无数据源都可。
2) 后端有数据源，且更新比例很高的场景。
优点：
1) 服务器存在于双机房，任一机房宕机保持可用。
2) 单份数据，无论应用在哪个机房，看到的都是同一个数据。
缺点：
1) 应用服务器会跨机房访问。如上图，并假设应用服务器在cm3和cm4，那么cm3的应用服务器也可能调用到cm4的tair机器，cm4的亦然。
2) 当一边机房出现故障时，tair中的数据会失效一半（一半这个数值是按两边机房tair机器数相同估计的，如果不相同，则按对应比例估算）
该部署方式，应用在删除数据时，只需要调用delete即可，无需调用invalid。当然，调用invalid也可，这种方式下会直接退化到delete。
### 1.4.2 双机房独立集群
双机房独立集群是指，在两个机房中同时部署2个独立的Tair集群，这两个集群没有直接关系。下图是一个典型的双机房独立集部署示意图，可以看到，cm3
和cm4各有一个完整的tair集群（2个configserver+多个dataserver）。图中还多了一个invalidserver的角色，
invalidserver接收客户端的invalid或者hide请求后，会对各机房内的集群进行delete或者hide操作，以此保障Tair中的
数据和后端数据源保持一致的。
![](http://www.lvtao.net/content/uploadfile/201312/dc7ad4827fa6578c4a1e392c4f8c4f1320131225075457.jpg)
适用场景：
1) 后端必须要有数据源，否则则退化成单机房集群，Tair集群本身不做同步。
2)
读写比不能过小，不然可能会带来Tair命中率降低。例如某个key，在数据库中被频繁更新，那么此时应用必须调用invalid来确保Tair和DB的
一致性。此时应用读Tair一直会不命中，导致整体命中率低，可能造成DB压力比较大。 如果依然有疑问的话，请联系 tair答疑。
优点：
1) 每个机房拥有独立Tair集群，应用在哪个机房就访问相同机房的Tair集群，不会出现跨机房调用和流量。
2) 单边机房故障，不会影响业务访问tair命中率。
缺点：
1) 后端必须要有数据源，也就是这种部署方式下，Tair必然是当作传统意义上的cache存在的。因为Tair mdb集群之间本身不会做数据同步，多集群间一致性保证依赖于后端数据源，如DB。
2)
当后端数据源数据发生更新后，业务不能直接把数据put到Tair，而是先需要调用invalid接口来失效这些对等集群中的数据（来保持各Tair集群
的数据和后端数据源的一致性）。之后业务可以把数据put到当前Tair集群（注意：只会put到本机房的Tair集群，不会put到对端集群）或者在读
Tair时发生not exist的时候从后端数据源取来放入Tair。
### 1.4.3 双机房单集群双份
双机房单集群双份，是指一个Tair集群部署在2个机房中，数据保存2份，并且同一数据的2个备份不会放在同一个数据服务器上。根据数据分布策略的不同，
还可以将同一份数据的不同备份分布到不同的机房上。该类型的集群部署方式与双机房单集群单份数据的部署方式一样。其不同之处，数据保存份数不一样。该类型
集群部署方式示意图如下图所示，数据服务器分别部署在两个不同的机房里，所有的数据服务器都被相同的配置服务器管理，在逻辑上，他们构成一个独立的集群。
![](http://www.lvtao.net/content/uploadfile/201312/85fe21730f40563efd75e62e34b50ae220131225075457.jpg)
现只有tbsession集群使用了这种部署方式。
适用场景：
后端无数据源，临时数据的存放，非cache。
cache类应用推荐使用双机房独立集群和双机房单集群单份部署方式。
优点：
1) 数据存放两份，数据安全性有一定保障。但由于存储引擎是mdb，数据存放在内存中，无法绝对保证数据不丢失。
2) 当一边机房故障时，另外一边机房依然可以服务，并且数据不丢失。
缺点：
1) 如果机房间网络出现异常情况，依然有较小几率丢失数据。
### 1.4.4 双机房主备集群
这种部署方式中，存在一个主集群和一个备份集群，分别在两个机房中。如下图所示，不妨假设CM3中部署的是主集群，CM4中部署的是备份集群。那么，在正
常情况下，用户只使用主集群，读写数据都与主集群交互。主备集群会自动同步数据（不需要业务去更新两边），保证两个机房数据的最终一致性。当一个机房发生
故障后，备集群会自动切换成主集群，提供服务，保证系统可用性。
![](http://www.lvtao.net/content/uploadfile/201312/180abef1f02729c20f3e717117d9a8df20131225075458.jpg)
适用场景：
该方式只在ldb存储引擎中存在，也就是业务将Tair当作最终存储使用。我们会在当前主集群存两份数据，并由Tair异步将数据更新到备集群，确保数据安全和服务可用。
优点：
1) 数据安全和服务可用性高。
2) 用户调用方便，无需考虑多集群间数据一致性的问题。
 一 如何安装tair:
- 确保安装了automake autoconfig 和 libtool,使用automake --version查看，一般情况下已安装
- 获得底层库 tbsys 和 tbnet的源代码:(svn checkout http://code.taobao.org/svn/tb-common-utils/trunk/ tb-common-utils).
- 获得tair源代码:(svn checkout http://code.taobao.org/svn/tair/trunk/ tair).
- 安装boost-devel库，在用rpm管理软件包的os上可以使用rpm -q boost-devel查看是否已安装该库
- 编译安装tbsys和tbnet
- 编译安装tair
-     tair 的底层依赖于tbsys库和tbnet库, 所以要先编译安装这两个库:
    取得源代码后, 先指定环境变量 TBLIB_ROOT 为需要安装的目录. 这个环境变量在后续 tair 的编译安装中仍旧会被使用到. 比如要安装到当前用户的lib目录下, 则指定 export TBLIB_ROOT="~/lib"
    进入common文件夹, 执行build.sh进行安装. 
-     编译安装tair:
    进入 tair 目录
    运行 bootstrap.sh
    运行 configure.  注意, 在运行configue的时候, 可以使用 --with-boost=xxxx 来指定boost的目录. 使用--with-release=yes 来编译release版本.
    运行 make 进行编译
    运行 make install 进行安装
    二 如何配置tair:
        tair的运行, 至少需要一个 config server 和一个 data server. 推荐使用两个 config server 多个data server的方式. 两个config server有主备之分.
    源代码目录中 share 目录下有三个配置文件的样例, 下面会逐个解说.
    configserver.conf  group.conf 这两个配置文件是config server所需要的. 先看这两个配置文件的配置
-     配置文件 configserver.conf
    [public]
    config_server=x.x.x.x:5198
    config_server=x.x.x.x:5198
    [configserver]
    port=5198
    log_file=logs/config.log
    pid_file=logs/config.pid
    log_level=warn
    group_file=etc/group.conf
    data_dir=data/data
    dev_name=eth0
    public 下面配置的是两台config server的 ip 和端口. 其中排在前面的是主config server. 这一段信息会出现在每一个配置文件中. 请保持这一段信息的严格一致.
    configserver下面的内容是本config server的具体配置:
    port 端口号, 注意 config server会使用该端口做为服务端口, 而使用该端口+1 做为心跳端口
    log_file 日志文件
    pid_file  pid文件, 文件中保存当前进程中的pid
    log_level 日志级别
    group_file 本config server所管理的 group 的配置文件
    data_dir   本config server自身数据的存放目录
    dev_name   所使用的网络设备名
    注意: 例子中, 所有的路径都配置的是相对路径. 这样实际上限制了程序启动时候的工作目录. 这里当然可以使用绝对路径. 
    注意: 程序本身可以把多个config server 或 data server跑在一台主机上, 只要配置不同的端口号就可以.
但是在配置文件的时候, 他们的数据目录必须分开, 程序不会对自己的数据目录加锁, 所以如果跑在同一主机上的服务, 数据目录配置相同,
程序自己不会发现, 却会发生很多莫名其妙的错误. 多个服务跑在同一台主机上, 一般只是在做功能测试的时候使用.
-     配置文件 group.conf
    #group name
    [group_1]
    # data move is 1 means when some data serve down, the migrating will be start.
    # default value is 0
    _data_move=1
 #_min_data_server_count: when data servers left in a group less than
this value, config server will stop serve for this group
    #default value is copy count.
    _min_data_server_count=4
    _copy_count=3
    _bucket_number=1023
    _plugIns_list=libStaticPlugIn.so
    _build_strategy=1 #1 normal 2 rack
    _build_diff_ratio=0.6 #how much difference is allowd between different rack
 # diff_ratio =  |data_sever_count_in_rack1 -
data_server_count_in_rack2| / max (data_sever_count_in_rack1,
data_server_count_in_rack2)
    # diff_ration must less than _build_diff_ratio
 _pos_mask=65535  # 65535 is 0xffff  this will be used to gernerate
rack info. 64 bit serverId & _pos_mask is the rack info,
    _server_list=x.x.x.x:5191
    _server_list=x.x.x.x:5191
    _server_list=x.x.x.x:5191
    _server_list=x.x.x.x:5191
    #quota info
    _areaCapacity_list=1,1124000;   
    _areaCapacity_list=2,1124000;  
     每个group配置文件可以配置多个group, 这样一组config server就可以同时服务于多个 group 了. 不同的 group 用group name区分
     _data_move 当这个配置为1的时候, 如果发生了某个data server宕机,
则系统会尽可能的通过冗余的备份对数据进行迁移. 注意, 如果 copy_count 为大于1的值, 则这个配置无效, 系统总是会发生迁移的.
只有copy_count为1的时候, 该配置才有作用. 
     _min_data_server_count  这个是系统中需要存在的最少data server的个数.  当系统中可正常工作的data server的个数小于这个值的时候, 整个系统会停止服务, 等待人工介入
     _copy_count  这个表示一条数据在系统中实际存储的份数. 如果tair被用作缓存, 这里一般配置1.
如果被用来做存储, 一般配置为3。 当系统中可工作的data server的数量少于这个值的时候, 系统也会停止工作. 比如
_copy_count 为3, 而系统中只有 2 台data server. 这个时候因为要求一条数据的各个备份必须写到不同的data
server上, 所以系统无法完成写入操作, 系统也会停止工作的.
     _bucket_number  这个是hash桶的个数, 一般要 >> data server的数量(10倍以上). 数据的分布, 负载均衡, 数据的迁移都是以桶为单位的.
     _plugIns_list  需要加载的插件的动态库名
     _accept_strategy  默认为0，ds重新连接上cs的时候，需要手动touch group.conf。如果设置成1，则当有ds重新连接会cs的时候，不需要手动touch group.conf。 cs会自动接入该ds。
     _build_strategy  在分配各个桶到不同的data server上去的时候所采用的策略. 目前提供两种策略.
配置为1 则是负载均衡优先, 分配的时候尽量让各个 data server 的负载均衡. 配置为 2 的时候, 是位置安全优先,
会尽量将一份数据的不同备份分配到不同机架的机器上. 配置为3的时候，如果服务器分布在多个机器上，那么会优先使用位置安全优先，即策略2.
如果服务器只在一个机架上，那么退化成策略1，只按负载分布。
     _build_diff_ratio 这个值只有当 _build_strategy 为2的时候才有意义.
实际上是用来表示不同的机架上机器差异大小的. 当位置安全优先的时候, 如果某个机架上的机器不断的停止服务, 必然会导致负载的极度不平衡.
 当两个机架上机器数量差异达到一定程度的时候, 系统也不再继续工作, 等待人工介入.
     _pos_mask  机架信息掩码. 程序使用这个值和由ip以及端口生成的64为的id做与操作, 得到的值就认为是位置信息.
 比如 当此值是65535的时候 是十六进制 0xffff. 因为ip地址的64位存储的时候采用的是网络字节序, 最前32位是端口号,
后32位是网络字节序的ip地址. 所以0xffff 这个配置, 将认为10.1.1.1 和 10.2.1.1 是不同的机架.
     _areaCapacity_list  这是每一个area的配额信息. 这里的单位是 byte. 需要注意的是,
该信息是某个 area 能够使用的所有空间的大小. 举个具体例子:当copy_count为3 共有5个data server的时候,
每个data server上, 该area实际能使用的空间是这个值/(3 * 5). 因为fdb使用mdb作为内部的缓存,
这个值的大小也决定了缓存的效率.
-     data server的配置文件
    [public]
    config_server=172.23.16.225:5198
    config_server=172.23.16.226:5198
    [tairserver]
    storage_engine=mdb
    mdb_type=mdb_shm
    mdb_shm_path=/mdb_shm_path01
    #tairserver listen port
    port=5191
    heartbeat_port=6191
    process_thread_num=16
    slab_mem_size=22528
    log_file=logs/server.log
    pid_file=logs/server.pid
    log_level=warn
    dev_name=bond0
    ulog_dir=fdb/ulog
    ulog_file_number=3
    ulog_file_size=64
    check_expired_hour_range=2-4
    check_slab_hour_range=5-7
    [fdb]
    # in
    # MB
    index_mmap_size=30
    cache_size=2048
    bucket_size=10223
    free_block_pool_size=8
    data_dir=fdb/data
    fdb_name=tair_fdb
    下面解释一下data server的配置文件:
    public 部分不再解说
    storage_engine 这个可以配置成 fdb 或者 mdb.  分别表示是使用内存存储数据(mdb)还是使用磁盘(fdb).
    mdb_type 这个是兼容以前版本用的, 现在都配成mdb_shm就可以了
    mdb_shm_path 这个是用作映射共享内存的文件.
    port data server的工作端口
    heartbeat_port data server的心跳端口
    process_thread_num 工作线程数.  实际上启动的线程会比这个数值多, 因为有一些后台线程.  真正处理请求的线程数量是这里配置的.
    slab_mem_size 所占用的内存数量.  这个值以M为单位, 如果是mdb, 则是mdb能存放的数据量, 如果是fdb, 此值无意义
    ulog_dir 发生迁移的时候, 日志文件的文件目录
    ulog_file_number 用来循环使用的log文件数目
    ulog_file_size 每个日志文件的大小, 单位是M
    check_expired_hour_range 清理超时数据的时间段.  在这个时间段内, 会运行一个后台进程来清理mdb中的超时数据.  一般配置在系统较空闲的时候
    check_slab_hour_range 对slap做平衡的时间段.  一般配置在系统较空闲的时候
    index_mmap_size fdb中索引文件映射到内存的大小, 单位是M
    cache_size fdb中用作缓存的共享内存大小, 单位是M 
    bucket_size fdb在存储数据的时候, 也是一个hash算法, 这儿就是hash桶的数目
    free_block_pool_size 这个用来存放fdb中的空闲位置, 便于重用空间
    data_dir fdb的数据文件目录
    fdb_name fdb数据文件名
    三 运行前的准备:
    因为系统使用共享内存作为数据存储的空间(mdb)或者缓存空间(fdb), 所以需要先更改配置, 使得程序能够使用足够的共享内存.  scripts 目录下有一个脚本 set_shm.sh 是用来做这些修改的, 这个脚本需要root权限来运行.
    四 如何启动集群:
    在完成安装配置之后, 可以启动集群了.  启动的时候需要先启动data server 然后启动cofnig server.
 如果是为已有的集群添加dataserver则可以先启动dataserver进程然后再修改gruop.conf，如果你先修改group.conf
再启动进程，那么需要执行touch group.conf;在scripts目录下有一个脚本 tair.sh 可以用来帮助启动 tair.sh
start_ds 用来启动data server.  tair.sh start_cs 用来启动config server.
 这个脚本比较简单, 它要求配置文件放在固定位置, 采用固定名称.  使用者可以通过执行安装目录下的bin下的 tair_server
(data server) 和 tair_cfg_svr(config server) 来启动集群.
## Tair用户指南
本文档介绍了Tair客户端的工作原理，以及Java和c++客户端的使用方法和接口介绍。
### 工作原理
Tair是一个分布式的key/value存储系统，数据往往存储在多个数据节点上。客户端需要决定数据存储的具体节点，然后才能完成具体的操作。
Tair的客户端通过和configserver交互获取这部分信息。configserver会维护一张表，这张表包含hash值与存储其对应数据的节点的对照关系。客户端在启动时，需要先和configserver通信，获取这张对照表。
在获取到对照表后，客户端便可以开始提供服务。客户端会根据请求的key的hash值，查找对照表中负责该数据的数据节点，然后通过和数据节点通信完成用户的请求。
### 支持的客户端
Tair当前支持Java[http://baike.corp.taobao.com/index.php/GetClient](http://baike.corp.taobao.com/index.php/GetClient)和c++语言的客户端。
### java客户端使用指南
#### java版本兼容性
Tair的java客户端需要JDK 1.5或与其兼容的环境。我们使用Sun公司的JDK 1.5在 Linux和Windows上测试过。
#### 依赖
Tair客户端使用mina（ [http://mina.apache.org/](http://mina.apache.org/) ）通信框架与Tair server通信，所以使用Tair java客户端需要确保运行环境中包含mina的jar包以及其依赖的库，mina请使用1.1.x的版本。
#### 配置java客户端
##### 支持的配置项
|配置项名称|类型|是否必选|默认值|说明|
|----|----|----|----|----|
|configServerList|List<String>|是|无|configserver地址列表，ip:port格式|
|groupName|String|是|无|group的name，一个configserver服务可以管理多个group，所以需要使用groupName加以区分|
|charset|String|否|UTF-8|字符集，这个选项在将String对象序列化时使用|
|compressionThreshold|int|否|8192|压缩阀值，单位为字节。当数据的大小超过这个阀值时，客户端将自动采用zip压缩算法压缩数据，并在接受到数据时自动解压。|
|maxWaitThread|int|否|100|最大等待线程数，当超过这个数量的线程在等待时，新的请求将直接返回超时|
|timeout|int|否|2000|请求的超时时间，单位为毫秒|
##### 初始化Java客户端
// 创建config server列表
List<String> confServers = new ArrayList<String>();
confServers.add("CONFIG_SERVER_ADDREEE:PORT");
confServers.add("CONFIG_SERVER_ADDREEE_2:PORT"); // 可选
// 创建客户端实例
DefaultTairManager tairManager = new DefaultTairManager();
tairManager.setConfigServerList(confServers);
// 设置组名
tairManager.setGroupName("GROUP_NAME");
// 初始化客户端
tairManager.init();
如果你的系统使用spring，那么可以使用类似下面的bean配置：
<bean id="tairManager" class="com.taobao.tair.impl.DefaultTairManager" init-method="init">
        <property name="configServerList">
                <list>
                        <value>CONFIG_SERVER_ADDREEE:PORT</value>
                        <value>CONFIG_SERVER_ADDREEE_2:PORT</value> <!-- 可选 -->
                </list>
        </property>
        <property name="groupName">
                <value>GROUP_NAME</value>
        </property>
</bean>
#### 接口介绍
##### 预备知识
由于Tair中的value除了用户的数据外，好包括version等元信息。所以返回的用户数据将和元数据一起封装在DataEntry对象中。
Tair客户端的所有接口都不抛出异常，操作的结果状态使用ResultCode表示。如果接口会返回数据，则返回的数据和ResultCode一起封装在Result中。
Result和ResultCode都包含有isSuccess方法，如果该方法返回true，则表示请求成功（当get的数据不存在时，该方法也返回 true）。
##### 基本接口
- get接口
get接口用于获取单个数据，要获取的数据由namespace和key指定。
当数据存在时，返回成功，数据存放在DataEntry对象中；
当数据不存在时，返回成功，ResultCode为ResultCode.DATANOTEXSITS，value为null。
示例：
Result<DataEntry> result = tairManager.get(namespace, key);
if (result.isSuccess()) {
    DataEntry entry = result.getValue();
    if(entry != null) {
        // 数据存在
    } else {
        // 数据不存在
    }
} else {
    // 异常处理
}
- mget接口
mget接口用于批量获取同一个namespace中的多个key对应的数据集合。mget在客户端将key列表根据key所在的服务器分组，然后将分组后的key列表发送到对应的服务器上，发送到多个服务器这个步骤是异步的，所以需要的时间不是线性的。
当得到返回结果时
- 如果返回的个数为0，ResultCode为ResultCode.DATANOTEXSITS
- 如果返回的个数小于请求的个数，ResultCode为ResultCode.PARTSUCC
- 全部返回，则返回成功
当有数据返回时，Result对象中的value是一个List<DataEntry>，这个List包含了所有取到的数据，每个 DataEntry都会包括请求的key，返回的value和version信息。
- put接口
put接口有3个签名，分别为：
ResultCode put(int namespace, Serializable key, Serializable value); // version为0，即不关心版本；expireTime为0，即不失效
ResultCode put(int namespace, Serializable key, Serializable value, int version); // expireTime为0，即不失效
ResultCode put(int namespace, Serializable key, Serializable value, int version, int expireTime);
示例：
ResultCode rc = tairManager.put(namespace, key, value);
if (rc.isSuccess()) {
    // put成功
} else if (ResultCode.VERERROR.equals(rc) {
    // 版本错误的处理代码
} else {
    // 其他失败的处理代码
}
// version应该从get返回的DataEntry对象中获取
// 出给使用0强制更新，否则不推荐随意指定版本
rc = tairManager.put(namespace, key, value, version);
// 使用全参数版本的put
rc = tairManager.put(namespace, key, value, version, expireTime);
- delete接口
delete接口用于删除有namespac和key指定的value。如果请求删除的key不存在，tair也将返回成功。
示例：
// 使用删除接口
ResultCode rc = tairManager.delete(namespace, key);
if (rc.isSuccess()) {
    // 删除成功
} else {
    // 删除失败
}
- mdelete接口
mdelete接口用于批量删除数据，该接口只能删除同一个namespace中的多条数据。其工作原理和mget接口类似。
示例：
// 使用批量删除接口
ResultCode rc = tairManager.mdelete(namespace, keys);
if (rc.isSuccess()) {
    // 删除成功
} else {
    // 部分成功处理代码
}
- incr接口
incr和decr接口配合使用可以提供计数器的功能。使用get和put接口也能实现计数器的功能，但由于两个操作不是原子的，很多情况下这不能满足需求。所以我们提供了incr和decr接口，通过这两个接口提供原子的计数器操作。
incr接口的方法签名为： Result<Integer> incr(int namespace, Serializable key, int value, int defaultValue, int expireTime);
接口参数的含义为：
|参数名|含义|
|----|----|
|namespace|计数器所在的namespace|
|key|计数器的key|
|value|本次增加的数量|
|defaultValue|当计数器不存在时的初始化值|
|expireTime|失效时间，单位为秒|
示例：
Result<Integer> result = tairManager.incr(namespace, key, 1, 0);
if (result.isSuccess()) {
    int rv = result.getValue(); // 这里是1
} else {
    // 错误处理
}
// 将返回4
result = tairManager.incr(namespace, key, 3, 0);
// 将返回2
result = tairManager.decr(namespace, key, 2, 0);
- item接口
item接口是对原有key/value接口的扩充，item接口将value视为一个数组，配合服务器端的支持，可以完成以下操作：
- 向item末尾添加新的items
- 获取指定范围的items
- 删除指定范围的items
- 获取并删除指定范围的items
- 获取item的个数
Item接口内部使用json格式，只支持基本类型，包括：
- String
- Long
- Integer
- Double
同一个Value中的类型需要保持一致，否则将返回序列化错误。
每个item可以指定maxcount，当item的条数操作指定的maxcount是，服务器将自动删除最早插入的item。
List<Integer> intList = new ArrayList<Integer>();
intList.add(1);
intList.add(2);
// 添加新的itmes
ResultCode rc = tairManager.addItems(1, key, intList, 50, 0, 0);
// 获取item的总条数
Result<Integer> ic = tairManager.getItemCount(1, key);
int totalCount = ic.getValue();
// 获取所有items
Result<DataEntry> rets = tairManager.getItems(1, key, 0, totalCount);
// 获取第一个item，并将其从系统中删除
rets = tairManager.getAndRemove(1, key, 0, 1);
### C++客户端使用指南
#### 数据操作接口
put
get
remove
incr
decr
##### 初始化(连接到tair)
startup
###### 清理
close
##### 超时时间设置
setTimeout
超时时间意味着客户端对单个请求的最大等待时间，即当客户端发出一个请求后在该时间内还未收到服务端的回应即返回超时。
各个接口的参数详见头文件。
#### 使用流程
CTAIRClinetAPI *tairClient = new CTAIRClientAPI();
assert(tairClient != 0);
if ( !tairClient->startup(configserver_master,configserver_slave,group_name)){
    //没连上，处理错误
}
tairClinet->put(...);
tairClient->get(...);
......
delete tairClient;
or
CTAIRClientAPI tairClient;
tairClient.startup();
//put
//get
//etc.
tairClient.close(); //可选，析构时会自动清理
错误代码说明:
对几个常见的Tair错误码做一个简要的解释
0, "success"
1, -3998 "data not exist" 数据不存在
-3988, "data expired" 数据过期，由于tair的数据删采用的是lazy
delete，也就是删除的数据不会马上做物理删除，而是逻辑上无法读到，所以有时候过期数据也可能返回data not
exist，新版本统一成了返回data not exist。
-1, "connection error or timeout" -3989, "timeout" 超时。
-3997, "version error" 版本错误，tair操作中的版本相当于一种乐观锁的机制，参见
-3994, "serialize error"
被问到次数最多的错误。。。Tair存储的key和value都必须是可序列化的对象，如果是String
，integer等基本类型就会按照java的序列化方式进行序列化，如果是自定义的对象，需要继承Serializable，出现这种错误一般是一个应
用put了一个自定义的对象，另外一个应用找不到这个类的定义，或者put后对对象进行了修改导致。
-3984, "migrate busy" tair的迁移的过程要保证仍然对外提供服务，因此迁移过程会分成很多轮，当迁移过程中的数据增量小到一定的阈值后，会有很小的一段时间停止写服务，这是就会返回migrate busy，稍稍过几秒钟重试应该就可以成功。
-3986, "write not on master"
操作没有发生在主桶上，这一般是拿到了错误的或者过时的对照表，当集群状态发生变化时，client拿着老的对照表读写数据可能会出现这个错误，然后
client就会去config
server获取新的对照表，因此如果有节点的变化（扩容或者节点当机下线）短暂出现这个错误是可以理解，如果一直报这个错误可能是服务端配置不正确。
-5, "key length error" -6, "value length error" key不能超过1K value不能超过1M
-20008, "not support"
不支持的操作，一般是Tair客户端的版本和服务器版本不匹配，不同的存储引擎支持的操作也有所不同.
### TAIR命令说明
- ./tairclient
- 说明：输出如下帮助信息
      ./tairclient -c configserver:port -g groupname
          -c, --configserver     	default port: 5198
          -g, --groupname        	group name
          -l, --cmd_line         	exec cmd
          -q, --cmd_file         	script file
          -h, --help             	print this message
          -v, --verbose         	print debug info
          -V, --version        	print version
- ./tairclient –c configserver:port –g groupname
- 说明：连接到指定的configserver，同时连接到指定的dataserver。 如：“./tairclient –c 
192.168.0.15 –g group_1” 或者 “./tairclient –c 192.168.0.15:6155 –g 
group_1”
- configserver：configserver所在机器的ip号
- port: configerserver.conf中配置的IP端口号，即configserver那台机器开启的端口号。
- groupname: group.conf文件中配置的group name
- ./tairclient -V
- 说明：查看版本信息
- ./tairclient –c configserver:port –g groupname –v
- 说明：按照上述命令连接后，在进行put以及get操作的时候，每次都会打印一些调试的信息。
- ./tairclient –h
- 说明：打印帮助信息（和./tairclient命令的效果一样）
- ./tairclient –c configserver:port –g groupname –l cmd_line
- 说明：在-l之后直接添加命令，实现put、get等。如“./tairclient –c 192.168.0.15 –g 
group_1 “put key data””。“./tairclient –c 192.168.0.15 –g group_1 “get 
key ””。
- ./tairclient –c configserver:port –g groupname –q cmd_file
- 说明：将命令写进文件，然后在连接的时候通过指定命令所在的文件来执行所需的命令。如“./tairclient –c 192.168.0.15 –g group_1 –q cmdfile.txt”。
- 在TAIR>提示符下：
- help
- 作用：输出帮助信息
- 说明：输出如下帮助信息
------------------------------------------------
SYNOPSIS   : put key data [area] [expired]
DESCRIPTION: area   - namespace , default: 0
expired- in seconds, default: 0,never expired
------------------------------------------------
SYNOPSIS   : incr key [count] [initValue] [area]
DESCRIPTION: initValue , default: 0
------------------------------------------------
SYNOPSIS   : get key [area]
------------------------------------------------
SYNOPSIS   : remove key [area]
------------------------------------------------
SYNOPSIS   : stat
DESCRIPTION: get stat info
------------------------------------------------
SYNOPSIS   : delall area
DESCRIPTION: delete all data of [area]
------------------------------------------------
SYNOPSIS   : dump dumpinfo.txt
DESCRIPTION: dumpinfo.txt is a config file of dump,syntax:
area start_time end_time,eg:10 2008-12-09 12:08:07 2008-12-10 12:10:00
- 在TAIR>提示符下 put
- 作用：将(key,data)存至DataServer上。
- 说明：输出如下帮助信息
SYNOPSIS   : put key data [area] [expired]
DESCRIPTION: area   - namespace , default: 0
expired- in seconds, default: 0,never expired
- area: 可以通过该选项指定不同的命名空间，以int型指定，范围在0~1024之间（包括1024），默认情况下为0。
- expired: 数据在DataServer中存放的期限，单位秒，在有效期内对相应key进行get可以得到对应的数据，过了有效期后，对相应的key进行get将失败，提示数据过期，默认情况为0，表示一直有效。
- 在TAIR>提示符下 get
- 作用：根据key值获取对应的数据
- 说明：输出如下帮助信息。
SYNOPSIS : get key [area]
key: 希望获取的数据对应的key值
area：指定命名空间，获取数据时将到相应的命名空间下取数据，默认为0。
- 在TAIR>提示符下incr
- 作用：增加计数
- 说明：输出如下帮助信息
SYNOPSIS   : incr key [count] [initValue] [area]
DESCRIPTION: initValue , default: 0
- count: 本次增加的数量
- initValue:当计数器不存在时的初始化值
- area: 指定命名空间，默认为0。
- 在TAIR>提示符下 stat
- 作用：输出一些统计信息。
- 说明：输出一些状态信息，包括DataServer上的不同命名空间的一些统计信息，如get、put的次数，占据空间的大小等。
- 在TAIR>提示符下 remove
- 作用：移除指定的(key,data)
- 说明：输出如下帮助信息
SYNOPSIS   : remove key [area]
key: 	指定想要移除的key。
area:	指定某个命名空间，默认值为0。
- 在TAIR>提示符下 delall area
- 作用：移除指定的命名空间的全部内容。
- 说明：area: 指定的命名空间的值。
- ./tair_cfg_svr
- 说明：输出如下信息
-f, --config_file  config file
-h, --help         this help
-V, --version      version
-f  config_file：指定configserver的配置文件
-h:	   		 输出上述帮助信息
-V：			 输出版本信息
- ./tair_server
- 说明：输出如下信息
-f, --config_file  config file name
-h, --help         display this help and exit
-V, --version      version and build time
-f config_file：指定DataServer的配置信息
-h:			输出上述帮助信息
-V：			输出版本信息
- ./parameter_printer
- 说明：输出一些参数的设置，如key的最大长度，area的最大值等。
- ./stat_info_test
- 说明：测试系统的状态是否正常。
- ./cst_monitor
- 说明：找到configserver里的桶分配表，将其二进制文件转换成txt文件并输出。如“./cst_monitor ../configserverdir/data/group_1_server_table”。
- ./cst_transfer
- 说明：用法和./cst_monitor相反，将txt格式的信息转换成二进制的信息。
- ./sif_monitor
- 说明：打印出configerserver的信息。如“./sif_monitor ../configserverdir/data/server_info.0”。
# 4. 接口
## 4.1. 接口说明
Result<DataEntry> get(int namespace, Serializable key)
获取数据
参数：
namespace - 数据所在的namespace
key - 要获取的数据的key
返回：success,数据存储在dataentry中，或者返回失败
Result< List<DataEntry>> mget(int namespace, List<? extends Object> keys)
批量获取数据
参数：
namespace - 数据所在的namespace
keys - 要获取的数据的key列表
返回：
如果成功，返回的数据对象为一个Map
ResultCode put(int namespace, Serializable key, Serializable value)
设置数据，如果数据已经存
            
