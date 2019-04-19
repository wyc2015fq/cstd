# 深入云存储系统Swift核心组件：Ring实现原理剖析 - =朝晖= - 博客园
# [深入云存储系统Swift核心组件：Ring实现原理剖析](https://www.cnblogs.com/dhcn/p/10175299.html)
http://www.cnblogs.com/yuxc/archive/2012/06/22/2558312.html
**简介**
**OpenStack**是一个美国国家航空航天局和Rackspace合作研发的开源云计算项目，并成为Apache下的一个重要开源项目，目前已经发展到了180家公司参与其中。
[OpenStack Object Storage（**Swift**）](http://www.openstack.org/projects/storage/)是[OpenStack](http://www.openstack.org/)开源云计算项目的子项目之一。Swift的目的是使用普通硬件来构建冗余的、可扩展的分布式对象存储集群，存储容量可达PB级。OpenStack Object Storage 最初由 Rackspace 采用Python语言开发，并于 2010 年 7 月贡献给 OpenStack ,作为该开源项目的一部分。它的目的是用于托管 Rackspace的 Cloud Files service ，原始项目代号是 swift，所以沿用至今。
在分布式对象存储中的一个关键问题是数据该如何存放。**Ring**是Swift中最重要的组件，用于记录存储对象与物理位置间映射关系。在涉及查询account、container、object信息时就需要查询集群的ring信息。
先来看一下Swift文档中关于Ring的描述：
       Ring用来确定数据驻留在集群中的位置。有单独对应于Account数据库、container数据库和单个object的ring。
       Ring中每个partition在集群中都(默认)有3个replica。每个partition的位置由ring来维护,并存储在映射中。
       Ring使用zone的概念来保证数据的隔离。每个partition的replica都确保放在了不同的zone中。一个zone可以是一个硬盘，一个服务器，一个机架，一个交换机，甚至是一个数据中心............
.......
在上述Ring的特性描述中提到了Ring使用zone、device、partition和replica等等来维护数据和磁盘间的映射信息。那么在Ring的背后采用什么算法，使用了什么机制来保证数据的安全、高效和可扩展呢？这些概念对于数据存储带来了什么好处？本文逐步深入探讨了Swift如何通过Ring组件来实现冗余的、可扩展的目的。
**1.****普通Hash算法与场景分析**
先来看一个简单的例子假设我们手里有N台存储服务器（以下简称node），打算用于图片文件存储，为了使服务器的负载均衡，需要把对象均匀地映射到每台服务器上，通常会使用哈希算法来实现，计算步骤如下：
1.计算object的hash值Key
2.计算Key mod N值
有N个存储节点，将Key模N得到的余数就是该Key对应的值需要存放的节点。比如，N是2，那么值为0、1、2、3、4的Key需要分别存放在0、1、0、1和0号节点上。如果哈希算法是均匀的，数据就会被平均分配到两个节点中。如果每个数据的访问量比较平均，负载也会被平均分配到两个节点上。
但是，当数据量和访问量进一步增加，两个节点无法满足需求的时候，需要增加一个节点来服务客户端的请求。这时，N变成了3，映射关系变成了Key mod (N+1)，因此，上述哈希值为2、3、4的数据需要重新分配（2->server 2，3 -> server 0，4 -> server 1）。如果数据量很大的话，那么数据量的迁移工作将会非常大。当N已经很大，从N加入一个节点变成N+1个节点的过程，会导致整个哈希环的重新分配，这个过程几乎是无法容忍的，几乎全部的数据都要重新移动一遍。
       我们举例说明，假设有100个node的集群，将107项数据使用md5 hash算法分配到每个node中，Python代码如下：
from hashlib import md5
from struct import unpack_from
NODE_COUNT = 100
DATA_ID_COUNT = 10000000
node_counts = [0] * NODE_COUNT
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
*# This just pulls part of the hash out as an integer*
    hsh = unpack_from(*'>I'*, md5(data_id).digest())[0]
    node_id = hsh % NODE_COUNT
    node_counts[node_id] += 1
desired_count = DATA_ID_COUNT / NODE_COUNT
print *'%d: Desired data ids per node'* % desired_count
max_count = max(node_counts)
over = 100.0 * (max_count - desired_count) / desired_count
print *'%d: Most data ids on one node, %.02f%% over'* % \
    (max_count, over)
min_count = min(node_counts)
under = 100.0 * (desired_count - min_count) / desired_count
print *'%d: Least data ids on one node, %.02f%% under'* % \
    (min_count, under)
100000: Desired data ids per node
100695: Most data ids on one node, 0.69% over
99073: Least data ids on one node, 0.93% under
分布结果如下所示：
|名称|数据项数量|百分比值|
|----|----|----|
|数据项均值|100000|0%|
|最多数据项节点|100695|+0.69%|
|最少数据项节点|99073|-0.93%|
       从数据分布上来看拥有最多/最少数据项的节点没有超出平均值的1%。现在假设增加一个节点提供负载能力，不过得重新分配数据项到新的节点上，代码如下：
from hashlib import md5
from struct import unpack_from  
NODE_COUNT = 100
NEW_NODE_COUNT = 101
DATA_ID_COUNT = 10000000
moved_ids = 0
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    hsh = unpack_from(*'>I'*, md5(str(data_id)).digest())[0]
    node_id = hsh % NODE_COUNT
    new_node_id = hsh % NEW_NODE_COUNT
    if node_id != new_node_id:
        moved_ids += 1
percent_moved = 100.0 * moved_ids / DATA_ID_COUNT
print *'%d ids moved, %.02f%%'* % (moved_ids, percent_moved)
9900989 ids moved, 99.01%
通过计算我们发现，为了提高集群1%的存储能力，我们需要移动9900989个数据项，也就是99.01%的数据项！显然，这种算法严重地影响了系统的性能和可扩展性。
增加1%的存储能力=移动99%的数据？
 这种亏本生意显然做不得，那么怎么办呢？一致性哈希算法就是为了解决这个问题而来。
**2.****一致性哈希算法**
一致性哈希算法是由D. Darger、E. Lehman和T. Leighton 等人于1997年在论文Consistent Hashing and Random Trees:Distributed Caching Protocols for Relieving Hot Spots On the World Wide Web首次提出，目的主要是为了解决分布式网络中的热点问题。在其论文中，提出了一致性哈希算法并给出了衡量一个哈希算法的4个指标：
> 
**平衡性****(Balance)**
       平衡性是指Hash的结果能够尽可能分布均匀，充分利用所有缓存空间。
**单调性****(Monotonicity)**
       单调性是指如果已经有一些内容通过哈希分派到了相应的缓冲中，又有新的缓冲加入到系统中。哈希的结果应能够保证原有已分配的内容可以被映射到新的缓冲中去，而不会被映射到旧的缓冲集合中的其他缓冲区。
**分散性****(Spread)**
       分散性定义了分布式环境中，不同终端通过Hash过程将内容映射至缓存上时，因可见缓存不同，Hash结果不一致，相同的内容被映射至不同的缓冲区。
**负载****(Load)**
       负载是对分散性要求的另一个纬度。既然不同的终端可以将相同的内容映射到不同的缓冲区中，那么对于一个特定的缓冲区而言，也可能被不同的用户映射为不同的内容。
Swift使用该算法的主要目的是在改变集群的node数量时（增加/删除服务器），能够尽可能少地改变已存在key和node的映射关系，以满足单调性。一致性哈希一般两种思路：
1.迁移为主要特点(swift初期采用)
2.引入虚结点，减少移动为特点(swift现采用)
       具体步骤如下：
       1.    首先求出每个节点(机器名或者是IP地址)的哈希值，并将其分配到一个圆环区间上（这里取0-2^32）。
       2.    求出需要存储对象的哈希值，也将其分配到这个圆环上。
       3.    从对象映射到的位置开始顺时针查找，将对象保存到找到的第一个节点上。
       其中这个从哈希到位置映射的圆环，我们就可以理解为何使用术语“**Ring**”来表示了。哈希环空间上的分布如图1所示：
图1 哈希环空间
  假设在这个环形哈希空间中，Cache5被映射在Cache3和Cache4之间，那么受影响的将仅是沿Cache5逆时针遍历直到下一个Cache（Cache3）之间的对象（它们本来映射到Cache4上）。
图2 一致性哈希算法的数据移动
   现在，使用该算法在集群中增加一个node，同时要保证每个节点的数据项数量均衡，代码如下所示，其中node_range_starts表示每个node的数据项的开始位置。
from bisect import bisect_left
from hashlib import md5
from struct import unpack_from 
NODE_COUNT = 100
NEW_NODE_COUNT = 101
DATA_ID_COUNT = 10000000
node_range_starts = []
for node_id in xrange(NODE_COUNT):
    node_range_starts.append(DATA_ID_COUNT /
                             NODE_COUNT * node_id)
new_node_range_starts = []
for new_node_id in xrange(NEW_NODE_COUNT):
    new_node_range_starts.append(DATA_ID_COUNT /
                              NEW_NODE_COUNT * new_node_id)
moved_ids = 0
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    hsh = unpack_from(*'>I'*, md5(str(data_id)).digest())[0]
    node_id = bisect_left(node_range_starts,
                          hsh % DATA_ID_COUNT) % NODE_COUNT
    new_node_id = bisect_left(new_node_range_starts,
                          hsh % DATA_ID_COUNT) % NEW_NODE_COUNT
    if node_id != new_node_id:
        moved_ids += 1
percent_moved = 100.0 * moved_ids / DATA_ID_COUNT
print *'%d ids moved, %.02f%%'* % (moved_ids, percent_moved)
4901707 ids moved, 49.02%
结果虽然比之前好了些，但是提高1%的性能与移动50%的数据仍不理想。
增加1%能力=移动50%数据？
**引入虚拟节点****(Partition)**
考虑到哈希算法在node较少的情况下，改变node数会带来巨大的数据迁移。为了解决这种情况，一致性哈希引入了“虚拟节点”的概念： “虚拟节点”是实际节点在环形空间的复制品，一个实际节点对应了若干个“虚拟节点”，“虚拟节点”在哈希空间中以哈希值排列。
图3 虚拟节点
       引入了“虚拟节点”后，映射关系就从【object--->node】转换成了【object--->virtual node---> node】。查询object所在node的映射关系如下图所示。
图4 对象、虚结点、节点间的映射关系
       对100个node细分为1000个vnode，使用vnode_range_starts来指定vnode的开始范围，vnode2node表示vnode到node的指派，然后增加一个node，完成vnode的重新分配，并计算所移动的数据项：
from bisect import bisect_left
from hashlib import md5
from struct import unpack_from
NODE_COUNT = 100
DATA_ID_COUNT = 10000000
VNODE_COUNT = 1000
vnode_range_starts = []
vnode2node = []
for vnode_id in xrange(VNODE_COUNT):
    vnode_range_starts.append(DATA_ID_COUNT /
                              VNODE_COUNT * vnode_id)
    vnode2node.append(vnode_id % NODE_COUNT)
new_vnode2node = list(vnode2node)
new_node_id = NODE_COUNT
NEW_NODE_COUNT = NODE_COUNT + 1
vnodes_to_reassign = VNODE_COUNT / NEW_NODE_COUNT
while vnodes_to_reassign > 0:
    for node_to_take_from in xrange(NODE_COUNT):
        for vnode_id, node_id in enumerate(new_vnode2node):
            if node_id == node_to_take_from:
                new_vnode2node[vnode_id] = new_node_id
                vnodes_to_reassign -= 1
                if vnodes_to_reassign <= 0:
                    break
        if vnodes_to_reassign <= 0:
            break
moved_ids = 0
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    hsh = unpack_from(*'>I'*, md5(str(data_id)).digest())[0]
    vnode_id = bisect_left(vnode_range_starts,
                         hsh % DATA_ID_COUNT) % VNODE_COUNT
    node_id = vnode2node[vnode_id]
    new_node_id = new_vnode2node[vnode_id]
    if node_id != new_node_id:
        moved_ids += 1
percent_moved = 100.0 * moved_ids / DATA_ID_COUNT
print *'%d ids moved, %.02f%%'* % (moved_ids, percent_moved)
90108 ids moved, 0.90%
结果显示，仅移动了0.9%的数据。与前面相比，整个集群的性能大大提高了。
增加1%的能力=移动0.9%数据
**　　固化虚节点到数据项的映射**
由于虚节点个数在集群的整个生命周期中是不会变化的，它与数据项的映射关系不会发生变化，改变的仅是vnode与node的映射关系，所以需对以上代码进行优化。
from struct import unpack_from
from hashlib import md5
from time import time
NODE_COUNT = 100
DATA_ID_COUNT = 10000000
VNODE_COUNT = 1000
begin = time()
vnode2node = []
for vnode_id in xrange(VNODE_COUNT):
    vnode2node.append(vnode_id % NODE_COUNT)
new_vnode2node = list(vnode2node)
new_node_id = NODE_COUNT
vnodes_to_assign = VNODE_COUNT / (NODE_COUNT + 1)
while vnodes_to_assign > 0:
    for node_to_take_from in xrange(NODE_COUNT):
        for vnode_id, node_id in enumerate(vnode2node):
            if node_id == node_to_take_from:
                vnode2node[vnode_id] = new_node_id
                vnodes_to_assign -= 1
                if vnodes_to_assign <= 0:
                    break
        if vnodes_to_assign <= 0:
            break
moved_id = 0
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    hsh = unpack_from(*'>I'*, md5(str(data_id)).digest())[0]
    vnode_id = hsh % VNODE_COUNT*#(1)*
    node_id = vnode2node[vnode_id]
    new_node_id = new_vnode2node[vnode_id]
    if node_id != new_node_id:
        moved_id += 1
percent_moved = 100.0 * moved_id / DATA_ID_COUNT
print *'%d ids moved, %.02f%%'* % (moved_id, percent_moved)
print *'%d seconds pass ...'* % (time() - begin)
90108 ids moved, 0.90%
**　　预设合理的虚结点数**
       现在已构建好了一致性哈希ring的原型。但是存在一个问题，以上例子中，1000个虚结点对应着100个结点，结点变动时，虚结点就需要重新分配到结点。当100个结点扩展到1001个结点时，此时至少有一个结点分配不到虚结点，那么就需要再增加虚结点数，而虚结点是与数据项对应的哈希关系，如果改变了虚节点数，那么就需要重新分配所有的数据项，这将导致移动大量的数据。
       所以在设置虚结点数的时候，需要对系统预期的规模做充分考虑，假如集群的规模不会超过6000个结点，那么可以将虚结点数设置为结点数的100倍。这样，变动任意一个结点的负载仅影响1%的数据项。此时有6百万个vnode数，使用2bytes来存储结点数(0~65535)。基本的内存占用是6*106*2bytes=12Mb，对于服务器来说完全可以承受。
       在此，引入了2个概念：
       在swift中，为了区分vnode和node，将vnode称为**partition**。
**　　位操作代替取模操作**
       此外，在计算机中使用位操作来确定partition的位置比取模更快。所以，在此引入了**partition power**的概念。
       继续改进ring的代码，设有65536个node(2^16)，有128（2^7）倍个partition数(2^23)。由于MD5码是32位的，使用PARTITION_SHIFT(等于32- PARTITION_POWER)将数据项的MD5哈希值映射到partition的2^23的空间中。
from array import array
from hashlib import md5
from struct import unpack_from
PARTITION_POWER = 23
PARTITION_SHIFT = 32 - PARTITION_POWER
NODE_COUNT = 65536
DATA_ID_COUNT = 100000000
part2node = array(*'H'*)
for part in xrange(2 ** PARTITION_POWER):
    part2node.append(part % NODE_COUNT)
node_counts = [0] * NODE_COUNT
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    part = unpack_from(*'>I'*,
        md5(str(data_id)).digest())[0] >> PARTITION_SHIFT
    node_id = part2node[part]
    node_counts[node_id] += 1
desired_count = DATA_ID_COUNT / NODE_COUNT
print *'%d: Desired data ids per node'* % desired_count
max_count = max(node_counts)
over = 100.0 * (max_count - desired_count) / desired_count
print *'%d: Most data ids on one node, %.02f%% over'* % \
    (max_count, over)
min_count = min(node_counts)
under = 100.0 * (desired_count - min_count) / desired_count
print *'%d: Least data ids on one node, %.02f%% under'* % \
    (min_count, under)
1525: Desired data ids per node
1683: Most data ids on one node, 10.36% over
1360: Least data ids on one node, 10.82% under
       数据不均衡的原因在于数据项相对于partition数太小了(10^8对2^23)，若数据项越多，分布越均衡。
**保证数据安全，引入****replica**
到目前为止，在集群中的数据在本地节点上只有一份，节点一旦发生故障就可能会造成数据的永久性丢失。因此，Swift中引入**replica**的概念使用冗余副本来保证数据的安全。replica的默认值为3，其理论依据主要来源于NWR策略。
       NWR是一种在分布式存储系统中用于控制一致性级别的一种策略。在Amazon的Dynamo云存储系统中，就应用NWR来控制一致性。每个字母的涵义如下：
       N：同一份数据的Replica的份数
       W：是更新一个数据对象的时候需要确保成功更新的份数
       R：读取一个数据需要读取的Replica的份数
       在分布式系统中，数据的单点是不允许存在的。即线上正常存在的Replica数量是1的情况是非常危险的，因为一旦这个Replica再次错误，就可能发生数据的永久性错误。假如我们把N设置成为2，那么，只要有一个存储节点发生损坏，就会有单点的存在。所以N必须大于2。N约高，系统的维护和整体成本就越高。工业界通常把N设置为3。
       因此，在ring的代码中引入replica，数量设置为3，其中 node_ids记录的是3个replica存放的node id。part2node[part]是根据partition id 找到对应的node id。
from array import array
from hashlib import md5
from struct import unpack_from
REPLICAS = 3
PARTITION_POWER = 16
PARTITION_SHIFT = 32 - PARTITION_POWER
PARTITION_MAX = 2 ** PARTITION_POWER - 1
NODE_COUNT = 256
DATA_ID_COUNT = 10000000
part2node = array(*'H'*)
for part in xrange(2 ** PARTITION_POWER):
    part2node.append(part % NODE_COUNT)
node_counts = [0] * NODE_COUNT
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    part = unpack_from(*'>I'*,
        md5(str(data_id)).digest())[0] >> PARTITION_SHIFT
    node_ids = [part2node[part]]
    node_counts[node_ids[0]] += 1
    for replica in xrange(1, REPLICAS):
        while part2node[part] in node_ids:
            part += 1
            if part > PARTITION_MAX:
                part = 0
        node_ids.append(part2node[part])
        node_counts[node_ids[-1]] += 1
desired_count = DATA_ID_COUNT / NODE_COUNT * REPLICAS
print *'%d: Desired data ids per node'* % desired_count
max_count = max(node_counts)
over = 100.0 * (max_count - desired_count) / desired_count
print *'%d: Most data ids on one node, %.02f%% over'* % \
    (max_count, over)
min_count = min(node_counts)
under = 100.0 * (desired_count - min_count) / desired_count
print *'%d: Least data ids on one node, %.02f%% under'* % \
    (min_count, under)
117186: Desired data ids per node
118133: Most data ids on one node, 0.81% over
116093: Least data ids on one node, 0.93% under
    结果如上，由于使用了256个node，分布约有1%的波动，比较均匀了。
但是存在有2个问题：
**　　随机分配映射**
       首先part2node是基于顺序分配的，对于给定的node，它所有partition的copies均在另两个node上，若某个node频繁宕机，与它相应的两个node上的数据项需要频繁复制。解决方法是随机分配partition到node的映射。
**　　分区容忍性和引入****zone**
       其次是当前的集群不满足CAP原理中的分区容忍性（Partition Tolerance）。Gilbert 和Lynch将分区容忍性定义如下：
       No set of failures less than total network failure is allowed to cause the system to respond incorrectly。
         翻译一下，就是除了全部网络节点发生故障以外，所有子节点集合的故障都不允许导致整个系统的响应故障。
现在为止，这些node都在一个机架上，一旦发生断电，网络故障，那么将丧失这一性质。因此就需要一种机制对机器的物理位置进行隔离。所以引入了**zone**的概念。
       在ring代码中引入zone_count，把这些node分割到16个zone中去。其中partition的replica不能放在同一个node上或同一个zone内。
from array import array
from hashlib import md5
from random import shuffle
from struct import unpack_from
REPLICAS = 3
PARTITION_POWER = 16
PARTITION_SHIFT = 32 - PARTITION_POWER
PARTITION_MAX = 2 ** PARTITION_POWER - 1
NODE_COUNT = 256
ZONE_COUNT = 16
DATA_ID_COUNT = 10000000
node2zone = []
while len(node2zone) < NODE_COUNT:
    zone = 0
    while zone < ZONE_COUNT and len(node2zone) < NODE_COUNT:
        node2zone.append(zone)
        zone += 1
part2node = array(*'H'*)
for part in xrange(2 ** PARTITION_POWER):
    part2node.append(part % NODE_COUNT)
shuffle(part2node)
node_counts = [0] * NODE_COUNT
zone_counts = [0] * ZONE_COUNT
for data_id in xrange(DATA_ID_COUNT):
    data_id = str(data_id)
    part = unpack_from(*'>I'*,
        md5(str(data_id)).digest())[0] >> PARTITION_SHIFT
    node_ids = [part2node[part]]
    zones = [node2zone[node_ids[0]]]
    node_counts[node_ids[0]] += 1
    zone_counts[zones[0]] += 1
    for replica in xrange(1, REPLICAS):
        while part2node[part] in node_ids and \
                node2zone[part2node[part]] in zones:
            part += 1
            if part > PARTITION_MAX:
                part = 0
        node_ids.append(part2node[part])
        zones.append(node2zone[node_ids[-1]])
        node_counts[node_ids[-1]] += 1
        zone_counts[zones[-1]] += 1
desired_count = DATA_ID_COUNT / NODE_COUNT * REPLICAS
print *'%d: Desired data ids per node'* % desired_count
max_count = max(node_counts)
over = 100.0 * (max_count - desired_count) / desired_count
print *'%d: Most data ids on one node, %.02f%% over'* % \
    (max_count, over)
min_count = min(node_counts)
under = 100.0 * (desired_count - min_count) / desired_count
print *'%d: Least data ids on one node, %.02f%% under'* % \
    (min_count, under)
desired_count = DATA_ID_COUNT / ZONE_COUNT * REPLICAS
print *'%d: Desired data ids per zone'* % desired_count
max_count = max(zone_counts)
over = 100.0 * (max_count - desired_count) / desired_count
print *'%d: Most data ids in one zone, %.02f%% over'* % \
    (max_count, over)
min_count = min(zone_counts)
under = 100.0 * (desired_count - min_count) / desired_count
print *'%d: Least data ids in one zone, %.02f%% under'* % \
    (min_count, under)
117186: Desired data ids per node
118782: Most data ids on one node, 1.36% over
115632: Least data ids on one node, 1.33% under
1875000: Desired data ids per zone
1878533: Most data ids in one zone, 0.19% over
1869070: Least data ids in one zone, 0.32% under
       到目前为止，ring的基本功能都已经有了：一致性哈希ring、partition、partition power、replica、zone。目前还差weight以及将以上代码改写为类封装成module。
**weight**
引入**weight**的概念，目的是“能者多劳”：解决未来添加存储能力更大的node时，使得可以分配到更多的partition。例如，2T 容量的node的partition数为1T的两倍。
       在ring的构建中，加入了weight属性。本例中weight简单地取1和2两个值，根据每个结点在weight和中的比例分配所需partition数。
from array import array
from hashlib import md5
from random import shuffle
from struct import unpack_from
from time import time
class Ring(object):
    def __init__(self, nodes, part2node, replicas):
        self.nodes = nodes
        self.part2node = part2node
        self.replicas = replicas
        partition_power = 1
        while 2 ** partition_power < len(part2node):
            partition_power += 1
        if len(part2node) != 2 ** partition_power:
            raise Exception(*"part2node's length is not an "*
*"exact power of 2"*)
        self.partition_shift = 32 - partition_power
    def get_nodes(self, data_id):
        data_id = str(data_id)
        part = unpack_from(*'>I'*,
           md5(data_id).digest())[0] >> self.partition_shift
        node_ids = [self.part2node[part]]
        zones = [self.nodes[node_ids[0]]]
        for replica in xrange(1, self.replicas):
            while self.part2node[part] in node_ids and \
                   self.nodes[self.part2node[part]] in zones:
                part += 1
                if part >= len(self.part2node):
                    part = 0
            node_ids.append(self.part2node[part])
            zones.append(self.nodes[node_ids[-1]])
        return [self.nodes[n] for n in node_ids]
def build_ring(nodes, partition_power, replicas):
    begin = time()
    parts = 2 ** partition_power
    total_weight = \
        float(sum(n[*'weight'*] for n in nodes.itervalues()))
    for node in nodes.itervalues():
        node[*'desired_parts'*] = \
            parts / total_weight * node[*'weight'*]
    part2node = array(*'H'*)
    for part in xrange(2 ** partition_power):
        for node in nodes.itervalues():
            if node[*'desired_parts'*] >= 1:
                node[*'desired_parts'*] -= 1
                part2node.append(node[*'id'*])
                break
        else:
            for node in nodes.itervalues():
                if node[*'desired_parts'*] >= 0:
                    node[*'desired_parts'*] -= 1
                    part2node.append(node[*'id'*])
                    break
    shuffle(part2node)
    ring = Ring(nodes, part2node, replicas)
    print *'%.02fs to build ring'* % (time() - begin)
    return ring
def test_ring(ring):
    begin = time()
    DATA_ID_COUNT = 10000000
    node_counts = {}
    zone_counts = {}
    for data_id in xrange(DATA_ID_COUNT):
        for node in ring.get_nodes(data_id):
            node_counts[node[*'id'*]] = \
                node_counts.get(node[*'id'*], 0) + 1
            zone_counts[node[*'zone'*]] = \
                zone_counts.get(node[*'zone'*], 0) + 1
    print *'%ds to test ring'* % (time() - begin)
    total_weight = float(sum(n[*'weight'*] for n in
                             ring.nodes.itervalues()))
    max_over = 0
    max_under = 0
    for node in ring.nodes.itervalues():
        desired = DATA_ID_COUNT * REPLICAS * \
            node[*'weight'*] / total_weight
        diff = node_counts[node[*'id'*]] - desired
        if diff > 0:
            over = 100.0 * diff / desired
            if over > max_over:
                max_over = over
        else:
            under = 100.0 * (-diff) / desired
            if under > max_under:
                max_under = under
    print *'%.02f%% max node over'* % max_over
    print *'%.02f%% max node under'* % max_under
    max_over = 0
    max_under = 0
    for zone in set(n[*'zone'*] for n in
                    ring.nodes.itervalues()):
        zone_weight = sum(n[*'weight'*] for n in
            ring.nodes.itervalues() if n[*'zone'*] == zone)
        desired = DATA_ID_COUNT * REPLICAS * \
            zone_weight / total_weight
        diff = zone_counts[zone] - desired
        if diff > 0:
            over = 100.0 * diff / desired
            if over > max_over:
                max_over = over
        else:
            under = 100.0 * (-diff) / desired
            if under > max_under:
                max_under = under
    print *'%.02f%% max zone over'* % max_over
    print *'%.02f%% max zone under'* % max_under
if __name__ == *'__main__'*:
    PARTITION_POWER = 16
    REPLICAS = 3
    NODE_COUNT = 256
    ZONE_COUNT = 16
    nodes = {}
    while len(nodes) < NODE_COUNT:
        zone = 0
        while zone < ZONE_COUNT and len(nodes) < NODE_COUNT:
            node_id = len(nodes)
            nodes[node_id] = {*'id'*: node_id, *'zone'*: zone,
*'weight'*: 1.0 + (node_id % 2)}
            zone += 1
    ring = build_ring(nodes, PARTITION_POWER, REPLICAS)
    test_ring(ring)
0.10s to build ring
162s to test ring
118581: Most data ids on one node,1.19% over
115537: Least data ids on one node, 1.41% under
1878343: Most data ids in one zone, 0.18% over
1870880: Least data ids in one zone, 0.22% under
       每个node上分布的最大波动为1.19%和1.41%，而zone上的波动分布在0.22%以下。
**总结**
       以上就是ring的构建原理分析，引入一致性哈希的原因是为了减少由于增加结点导致数据项移动的数量来提高单调性，引入partition的原因是为了减少由于节点数过少导致移动过多的数据项、引入replica的原因是防止数据单点提高冗余性，引入zone的原因是为了保证分区容忍性、引入weight的原因是为了保证partition分配的均衡。
那么Ring的结构是否就此止步了呢，在Swift开发人员的博客中提到，只要发现更好的数据映射机制，就将Ring推倒重来，所以未来Ring会如何演化，咱们也可以参与其中，促其不断地进化。
**致谢**
本文为学习笔记，写于SAE实习期间，代码分析来自gholt的swift开发博文，一致性哈希图片来自于sparkliang的博文，也感谢zzcase在邮件讨论中给予我的诸多帮助。
**参考文章**
[http://tlohg.com/building-a-consistent-hashing-ring-part-1](http://tlohg.com/building-a-consistent-hashing-ring-part-1)
[http://blog.csdn.net/zzcase/article/details/6709961](http://blog.csdn.net/zzcase/article/details/6709961)
[http://blog.csdn.net/sparkliang/article/details/5279393](http://blog.csdn.net/sparkliang/article/details/5279393)
[http://blog.csdn.net/x15594/article/details/6270242](http://blog.csdn.net/x15594/article/details/6270242)
[http://ultimatearchitecture.net/index.php/2010/06/22/quorum-nwr/](http://ultimatearchitecture.net/index.php/2010/06/22/quorum-nwr/)
[http://ultimatearchitecture.net/index.php/2010/06/22/consistent_hash_algorithn/](http://ultimatearchitecture.net/index.php/2010/06/22/consistent_hash_algorithn/)
[http://ultimatearchitecture.net/index.php/2010/06/22/eventually_consistency_base-vs-acid/](http://ultimatearchitecture.net/index.php/2010/06/22/eventually_consistency_base-vs-acid/)
[http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.23.3738](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.23.3738)

