# 【redis】集群伸缩（添加删除节点） - Big Smile - CSDN博客
2018年09月16日 11:26:45[王啸tr1912](https://me.csdn.net/tr1912)阅读数：147
注：本文知识点全部来自于《redis开发与运维》这本书在加上博主本人对于redis的理解构成
# 一、集群伸缩原理
        Redis集群提供了灵活的节点扩容和收缩方案。在不影响集群对外服务的情况下，可以为集群添加节点进行扩容也可以下线部分节点进行缩容：
![](https://img-blog.csdn.net/20180912204556956?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们都指导，这样的每一个节点上面都分配了我们的16384槽中的几个，以及对应槽下面的数据。所以我们在伸缩节点的时候，实质上也是对于哈希槽和槽对应数据的一个调整。
首先我们先来看一下经典集群搭建的分布图：
![](https://img-blog.csdn.net/20180912205812912?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
三个主节点分别维护自己负责的槽和对应的数据，如果希望加入1个节点实现集群扩容时，需要通过相关命令把一部分槽和数据迁移给新节点：
![](https://img-blog.csdn.net/20180912205947101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
总结：集群伸缩就是槽和数据在节点之间移动。
# 二、集群扩容
        我们的集群在扩容的时候，是进行的增加节点的操作，故需要把已有节点上的槽和数据分配到新的节点上，那么应该把已有节点上的哪些槽和节点放在新节点上呢？让我们来探究。
      扩容是分布式存储最常见的需求，Redis集群扩容操作可分为如下步骤：
- 准备新节点。
- 加入集群。
- 迁移槽和数据。
**1.准备新节点**
首先我们需要运行命令来启动两个新的redis节点：
```
redis-server conf/redis-6385.conf 
redis-server conf/redis-6386.conf
```
启动之后，这两个redis节点为孤立的节点
**2、加入集群**
```
127.0.0.1:6379> cluster meet 127.0.0.1 6385 
127.0.0.1:6379> cluster meet 127.0.0.1 6386
```
执行上述命令加入已有的集群。
![](https://img-blog.csdn.net/20180912213309785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180912213347193?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
        集群内新旧节点经过一段时间的ping/pong消息通信之后，所有节点会发 现新节点并将它们的状态保存到本地。这个时间按照我们集群的约定时间来确定。
        新节点刚开始都是主节点状态，但是由于没有负责的槽，所以不能接受任何读写操作。对于新节点的后续操作我们一般有两种选择：
- 为它迁移槽和数据实现扩容。
- 作为其他主节点的从节点负责故障转移。
    redis-trib.rb工具也实现了为现有集群添加新节点的命令，还实现了直接添加为从节点的支持，命令如下：
```
redis-trib.rb add-node new_host:new_port existing_host:existing_port --slave     --master-id <arg>
```
使用命令直接添加从节点：
```
redis-trib.rb add-node 127.0.0.1:6385 127.0.0.1:6379 
redis-trib.rb add-node 127.0.0.1:6386 127.0.0.1:6379
```
**3、迁移槽和数据**
         槽是Redis集群管理数据的基本单位，首先需要为新节点制定槽的迁移计划，确定原有节点的哪些槽需要迁移到新节点。迁移计划需要确保每个节点负责相似数量的槽，从而保证各节点的数据均匀。例如，在集群中加入 6385节点，如图所示。加入6385节点后，原有节点负责的槽数量从 6380变为4096个。
![](https://img-blog.csdn.net/20180916105314995?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
槽迁移计划确定后开始逐个把槽内数据从源节点迁移到目标节点。
**迁移数据：**
数据迁移过程是逐个槽进行的，流程如下：
1）对目标节点发送cluster setslot{slot}importing{sourceNodeId}命令，让目标节点准备导入槽的数据。
2）对源节点发送cluster setslot{slot}migrating{targetNodeId}命令，让源节点准备迁出槽的数据。
3）源节点循环执行cluster getkeysinslot{slot}{count}命令，获取count个 属于槽{slot}的键。
![](https://img-blog.csdn.net/2018091610565157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180916110006378?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
   4）在源节点上执行migrate{targetIp}{targetPort}""0{timeout}keys{keys...} 命令，把获取的键通过流水线（pipeline）机制批量迁移到目标节点，批量 迁移版本的migrate命令在Redis3.0.6以上版本提供，之前的migrate命令只能 单个键迁移。对于大量key的场景，批量键迁移将极大降低节点之间网络IO次数。
   5）重复执行步骤3）和步骤4）直到槽下所有的键值数据迁移到目标节点。
   6）向集群内所有主节点发送cluster setslot{slot}node{targetNodeId}命令，通知槽分配给目标节点。为了保证槽节点映射变更及时传播，需要遍历发送给所有主节点更新被迁移的槽指向新节点。
# 三、集群收缩
        收缩集群意味着缩减规模，需要从现有集群中安全下线部分节点。
![](https://img-blog.csdn.net/20180916111107931?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180916111139632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**流程说明：**
1）首先需要确定下线节点是否有负责的槽，如果是，需要把槽迁移到其他节点，保证节点下线后整个集群槽节点映射的完整性。
2）当下线节点不再负责槽或者本身是从节点时，就可以通知集群内其他节点忘记下线节点，当所有的节点忘记该节点后可以正常关闭。
**1、下线迁移槽**
      下线节点需要把自己负责的槽迁移到其他节点，原理与之前节点扩容的 迁移槽过程一致。例如我们把6381和6384节点下线，节点信息如下：
```
127.0.0.1:6381> cluster nodes 
40b8d09d44294d2e23c7c768efc8fcd153446746 127.0.0.1:6381 myself,master - 0 0 2 connected     
12288-16383 4fa7eac4080f0b667ffeab9b87841da49b84a6e4 127.0.0.1:6384 slave 
40b8d09d44294d2e2    3c7c768efc8fcd153446746 0 1469894180780 5 connected ...
```
6381是主节点，负责槽（12288-16383），6384是它的从节点，如图所示。下线6381之前需要把负责的槽迁移到其他节点。
![](https://img-blog.csdn.net/20180916111426883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
       收缩正好和扩容迁移方向相反，6381变为源节点，其他主节点变为目标 节点，源节点需要把自身负责的4096个槽均匀地迁移到其他主节点上。
我们可以使用用redis-trib.rb reshard命令完成槽的迁移：
```
#redis-trib.rb reshard 127.0.0.1:6381 
>>> Performing Cluster Check (using node 127.0.0.1:6381)
 ... 
[OK] All 16384 slots covered. 
How many slots do you want to move (from 1 to 16384)1365 
What is the receiving node ID cfb28ef1deee4e0fa78da86abe5d24566744411e /*输入6379    
节点id作为目标节点.*/ 
Please enter all the source node IDs. 
Type 'all' to use all the nodes as source nodes for the hash slots. 
Type 'done' once you entered all the source nodes IDs. 
Source node #1:40b8d09d44294d2e23c7c768efc8fcd153446746 /*源节点6381 id*/ 
Source node #2:done /* 输入done确认 */ 
... 
Do you want to proceed with the proposed reshard plan (yes/no) yes 
...
```
这样完成了1365个槽往6379节点上面迁移，同样的工作做三遍，即可迁移所有节点到其他节点上。
**2、忘记节点**
由于集群内的节点不停地通过Gossip消息彼此交换节点状态，因此需要
通过一种健壮的机制让集群内所有节点忘记下线的节点。也就是说让其他节 点不再与要下线节点进行Gossip消息交换。Redis提供了cluster forget{downNodeId}命令实现该功能：
![](https://img-blog.csdn.net/20180916112439241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
       当节点接收到cluster forget{down NodeId}命令后，会把nodeId指定的节 点加入到禁用列表中，在禁用列表内的节点不再发送Gossip消息。禁用列表 有效期是60秒，超过60秒节点会再次参与消息交换。也就是说当第一次 forget命令发出后，我们有60秒的时间让集群内的所有节点忘记下线节点。
       线上操作不建议直接使用cluster forget命令下线节点，需要跟大量节点 命令交互，实际操作起来过于繁琐并且容易遗漏forget节点。建议使用redistrib.rb del-node{host：port}{downNodeId}命令。
```
redis-trib.rb del-node 127.0.0.1:6379 4fa7eac4080f0b667ffeab9b87841da49b84a6e4 # 从节点6384 id 
redis-trib.rb del-node 127.0.0.1:6379 40b8d09d44294d2e23c7c768efc8fcd153446746 # 主节点6381 id
```
