# OpenStack大规模部署详解 - =朝晖= - 博客园
# [OpenStack大规模部署详解](https://www.cnblogs.com/dhcn/p/10524954.html)
https://blog.csdn.net/karamos/article/details/80130443
0.前言
今年的2月22日，OpenStack发布了15个版本Ocata。
走过了7年的发展岁月的OpenStack已经成为了云计算领域中最火热的项目之一，并逐渐成为IaaS的事实标准，私有云项目的部署首选。OpenStack社区可能自己都没有想到其发展会如此之迅速，部署规模如此之大，以至于最开始对大规模OpenStack集群的部署支持以及持续可扩展性似乎并没有考虑完备。
众所周知，OpenStack每一个项目都会有数据库的访问以及消息队列的使用，而数据库和消息队列是整个OpenStack扩展的瓶颈。尤其是消息队列，伴随着集群规模的扩展，其性能下降是非常明显的。通常情况下，当集群规模扩展到200个节点，一个消息可能要在十几秒后才会响应，集群的整体性能大大下降[1]，英国电信主管Peter Willis声称一个独立OpenStack集群只能最多管理500个计算节点[2]。
当处理大规模问题时，我们自然会想到分而治之策略，其思想是将一个规模为N的问题分解为K个规模较小的子问题，这些子问题相互独立且与原问题性质相同，解决了子问题就能解决原问题。社区提出的多Region、多Cells以及Cascading等方案都是基于分而治之策略，但它们又有所区别，主要体现在分治的层次不一样，多Region和Cascading方案思想都是将一个大的集群划分为一个个小集群，每个小集群几乎是一个完整的OpenStack环境，然后通过一定的策略把小集群统一管理起来，从而实现使用OpenStack来管理大规模的数据中心。在Grizzly版本引入的Nova Cells概念，其思想是将不同的计算资源划分为一个个的Cell，每个Cell都使用独立的消息队列和数据库服务，从而解决了数据库和消息队列的瓶颈问题，实现了规模的可扩展性。遗憾的是，目前社区还没有一个非常完美的OpenStack大规模部署方案，以上提到的方案都存在各自的优点和缺点，实际部署时应根据物理资源的分布情况、用户资源需求等因素综合选择。本文接下来将谈谈OpenStack大规模部署问题，讨论前面提到的各个方案的优缺点以及分别适用的场景。
1.单集群优化策略
1.1 使用独立的数据库和消息队列
前面提到限制OpenStack规模增长的最主要因素之一就是由于数据库和消息队列的性能瓶颈，因此如果能够有效减轻数据库以及消息队列的负载，理论上就能继续增长节点数量。各个服务使用独立的数据库以及消息队列显然能够有效减小数据库和消息队列的负载。在实践中发现，以下服务建议使用独立的数据库以及消息队列:
Keystone: 用户以及其它API服务的认证都必须经过Keystone组件，每次Token验证都需要访问数据库，随着服务的增多以及规模的增大，数据库的压力将会越来越大，造成Keystone的性能下降，拖垮其它所有服务的API响应。因此为Keystone组件配置专门的数据库服务，保证服务的高性能。
Ceilometer：Ceilometer是一个资源巨耗型服务，在收集消息和事件时会发送大量的消息到队列中，并频繁写入数据库。为了不影响其它服务的性能，Ceilometer通常都搭配专有的数据库服务和消息队列。
Nova: OpenStack最活跃的主体就是虚拟机，而虚拟机的管理都是由Nova负责的。几乎所有对虚拟机的操作都需要通过消息队列发起RPC请求，因此Nova是队列的高生产者和高消费者，当集群规模大时，需要使用独立的消息队列来支撑海量消息的快速传递。
Neutron：Neutron管理的资源非常多，包括网络、子网、路由、Port等，数据库和消息队列访问都十分频繁，并且数据量还较大，使用的独立的数据库服务和消息队列既能提高Neutron本身的服务性能，又能避免影响其它服务的性能。
1.2 使用Fernet Token
前面提到每当OpenStack API收到用户请求时都需要向Keystone验证该Token是否有效，Token是直接保存在数据库中的，增长速率非常快，每次验证都需要查询数据库，并且Token不会自动清理而越积越多，导致查询的性能越来越慢，Keystone验证Token的响应时间会越来越长。所有的OpenStack服务都需要通过Keystone服务完成认证，Keystone的性能下降，将导致其它所有的服务性能下降，因此保证Keystone服务的快速响应至关重要。除此之外，如果部署了多Keystone节点，还需要所有的节点同步Token，可能出现同步延迟而导致服务异常。为此社区在Kilo版本引入了Fernet Token，与UUID Token以及PKI Token不同的是它是基于对称加密技术对Token加密，只需要拥有相同加密解密文件，就可以对Token进行验证，不需要持久化Token，也就无需存在数据库中，避免了对数据库的IO访问，创建Token的速度相对UUID Token要快，不过验证Token则相对要慢些[3]。因此在大规模OpenStack集群中建议使用Fernet Token代替传统的Token方案。
以上优化策略能够一定程度上减少消息队列和数据库的访问，从而增大节点部署规模，但其实并没有根本解决扩展问题，随着部署规模的增长，总会达到瓶颈，理论上不可能支持无限扩展。
2.多Region方案
OpenStack支持将集群划分为不同的Region，所有的Regino除了共享Keystone、Horizon、Swift服务，每个Region都是一个完整的OpenStack环境，其架构如下：
部署时只需要部署一套公共的Keystone和Horizon服务，其它服务按照单Region方式部署即可，通过Endpoint指定Region。用户在请求任何资源时必须指定具体的区域。采用这种方式能够把分布在不同的区域的资源统一管理起来，各个区域之间可以采取不同的部署架构甚至不同的版本。其优点如下：
部署简单，每个区域部署几乎不需要额外的配置，并且区域很容易实现横向扩展。
故障域隔离，各个区域之间互不影响。
灵活自由，各个区域可以使用不同的架构、存储、网络。
但该方案也存在明显的不足：
各个区域之间完全隔离，彼此之间不能共享资源。比如在Region A创建的Volume，不能挂载到Region B的虚拟机中。在Region A的资源，也不能分配到Region B中，可能出现Region负载不均衡问题。
各个区域之间完全独立，不支持跨区域迁移，其中一个区域集群发生故障，虚拟机不能疏散到另一个区域集群中。
Keystone成为最主要的性能瓶颈，必须保证Keystone的可用性，否则将影响所有区域的服务。该问题可以通过部署多Keystone节点解决。
OpenStack多Region方案通过把一个大的集群划分为多个小集群统一管理起来，从而实现了大规模物理资源的统一管理，它特别适合跨数据中心并且分布在不同区域的场景，此时根据区域位置划分Region，比如北京和上海。而对于用户来说，还有以下好处:
用户能根据自己的位置选择离自己最近的区域，从而减少网络延迟,加快访问速度。
用户可以选择在不同的Region间实现异地容灾。当其中一个Region发生重大故障时，能够快速把业务迁移到另一个Region中。
但是需要注意的是，多Region本质就是同时部署了多套OpenStack环境，确切地说并没有解决单OpenStack集群的大规模部署问题。
3.OpenStack Cascading方案以及Trio2o项目
OpenStack Cascading方案[9]是由国内华为提出的用于支持场景包括10万主机、百万虚机、跨多DC的统一管理的大规模OpenStack集群部署。它采取的策略同样是分而治之，即把原来一个大的OpenStack集群拆分成多个小集群，并把拆分的小集群级联起来统一管理，其原理如图：
只有最顶层的OpenStack暴露标准OpenStack API给用户，其包含若干个子OpenStack集群。
底层的OpenStack负责实际的资源分配，但不暴露API给用户，而必须通过其之上的OpenStack调度。
用户请求资源时，首先向顶层OpenStack API发起请求，顶层的OpenStack会基于一定的调度策略选择底层的其中一个OpenStack，被选中的底层OpenStack负责实际的资源分配。
该方案号称支持跨多达100个DC，支持10万个计算节点部署规模，能同时运行100万个虚拟机[10]。但该方案目前仍处于开发和测试阶段，尚无公开的实际部署案例。目前该方案已经分离出两个独立的big-tent项目，一个是Tricircle，专门负责网络相关以及对接Neutron，另一个项目是Trio2o，为多Region OpenStack集群提供统一的API网关。
4.Nova Cells方案
前面提到的OpenStack多Region方案是基于OpenStack环境切分，它对用户可见而非透明的，并且单集群依然不具备支撑大规模的能力和横向扩展能力。而Nova Cells方案是针对服务级别划分的，其最终目标是实现单集群支撑大规模部署能力和具备灵活的扩展能力。Nova Cells方案是社区支持的方案，因此本文将重点介绍，并且会总结下在实际部署中遇到的问题。
4.1 Nova Cells架构和原理介绍
Nova Cells模块是OpenStack在G版本中引入的，其策略是将不同的计算资源划分成一个个Cell，并以树的形式组织，如图所示：
图2 Nova Cell架构
从图中看出，Cells的结构是树形的，一个Cell可能包含若干子Cell，以此逐级向下扩展。每个Cell都有自己独立的消息队列和数据库，从而解决了消息队列和数据库的性能瓶颈问题，Cell与Cell之间主要通过Nova-Cells负责通信，一层一层通过消息队列传递消息，每个Cell都需要知道其Parent Cell以及所有孩子Cells的消息队列地址，这些信息可以保存到该Cell的数据库中，也可以通过json文件指定:
{
    "parent": {
        "name": "parent",
        "api_url": "http://api.example.com:8774",
        "transport_url": "rabbit://rabbit.example.com",
        "weight_offset": 0.0,
        "weight_scale": 1.0,
        "is_parent": true
    },
    "cell1": {
        "name": "cell1",
        "api_url": "http://api.example.com:8774",
        "transport_url": "rabbit://rabbit1.example.com",
        "weight_offset": 0.0,
        "weight_scale": 1.0,
        "is_parent": false
    },
    "cell2": {
        "name": "cell2",
        "api_url": "http://api.example.com:8774",
        "transport_url": "rabbit://rabbit2.example.com",
        "weight_offset": 0.0,
        "weight_scale": 1.0,
        "is_parent": false
    }
}
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
根据节点所在树中位置以及功能，分为两种Cell类型:
api cell，非叶子节点，该类型的Cell不包含计算节点，但包括了一系列子Cells，子Cells会继续调度直到到达叶子节点，即Compute Vell中。其中最顶层的根节点通常也叫作Top Cell。
compute cell，叶子节点，包含一系列计算节点。负责转发请求到其所在的Nova-Conductor服务。
注意: 所有的Nova服务都隶属于某个Cell中，所有的Cells都必须指定Cell类型。
每个Cell节点都有从根节点到该节点的唯一路径，路径默认通过!分割，比如root!cell_1!cell_13，表示从root到cell_1再到cell_13。根节点的Cell类型一定是API就是说Cell对用户是完全透明的，和不使用Cell时是完全一样的。其中Nova-Cells服务是需要额外部署的新服务，该服务主要负责创建虚拟机时，从所有的孩子Cell中选择其中一个子Cell作为虚拟机的Cell，子Cell会继续执行调度直到到达底层的Compute Cell中，最后转发请求到目标Compute Cell所在的Nova-Conductor服务中。因此采用Nova Cells方案后，Nova实际采用的是二级调度策略，第一级由Nova-Cells服务负责调度Cell，第二级由Nova-Scheduler服务负责调度计算节点。
Compute Cell节点担任的职责类似于非Cell架构的控制节点，需要部署除Nova-API服务以外的所有其它Nova服务，每个Cell相当于一个完整的Nova环境，拥有自己的Nova-Conductor、Nova-Scheduler等服务以及数据库服务和消息队列服务，并且包含若干计算节点，每个Cell的组件只服务于其自身所在的Cell，而不是整个集群，因此天生具备支撑单集群大规模部署能力。增大规模时，只需要相应增加Cell即可，因此具有非常灵活的可扩展能力。
子Cell的虚拟机信息会逐层向上同步复制到其父Cell中，Top Cell中包含了所有Cells的虚拟机信息，查看虚拟机信息时，只需要从Top Cell数据库查询即可，不需要遍历子Cell数据库。对虚拟机进行操作时，如果不使用Cell，则只需要根据其Host字段，向宿主机发送RPC请求即可。如果使用了Cell，则需要首先获取虚拟机的Cell信息，通过Cell信息查询消息队列地址，然后往目标消息队列发送RPC请求。
4.2 Nova Cell生产案例
Nova Cells方案很早就已经存在一些生产案例了，其中CERN(欧洲原子能研究中心)OpenStack集群可能是目前公开的规模最大的OpenStack部署集群，截至2016年2月部署规模如下[7]：
单Region，33个Cell。
2个Ceph集群。
~5500个计算节点，5300KVM和200Hyper-V，总包含140k Cores。
超过17000个虚拟机。
~2800个镜像，占44TB存储空间。
~2000个Volumes，已分配800TB。
~2200个注册用户，超过2500个租户。
其Nova部署架构如下图: 
图3 CERN OpenStack集群Nova架构
天河二号是国内千级规模的典型案例之一，于2014年初就已经在国家超算广州中心并对外提供服务,其部署规模如下[5]:
单Region，8个Cell。
每个Cell包含2个控制节点和126个计算节点。
总规模1152个物理节点。
一次能创建大约5000个左右虚拟机。
每秒可查询约1000个虚拟机信息。
除了以上两个经典案例外，Rackspace、NeCTAR、Godaddy[6]、Paypal等也是采用了Nova Cells方案支持千级规模的OpenStack集群部署。这些生产案例实践证明了使用Nova Cells支持大规模OpenStack集群的可能性。
4.3 Nova Cells遇到的坑
刚刚介绍了不少Nova Cells的成功生产案例，让人不禁“蠢蠢欲动”，想要小试牛刀。小编已经架不住诱惑，于是专门申请了23台物理服务器作为Nova Cells测试环境使用，实际部署时包含3个Cells，每个Cell包含7台物理服务器，其中1台控制节点，一台Ceph All-in-one节点，剩余为5个计算节点。另外多余的两台分别作为Top Cell的控制节点和网络节点。部署的OpenStack版本为Mitaka，使用社区原生代码。在部署过程中遇到了大大小小不少坑，有些是配置问题，有些是Cells本身的问题。
1. 虚拟机永久堵塞在scheduling状态
我们知道，每个Cell都使用自己的独立数据库，因此配置Nova的数据库时，显然需要配置其所在Cell的数据库地址。而从Mitaka版本开始已经把一些公共的数据从nova数据库单独分离出来一个数据库nova_api（原因后面说明）。创建虚拟机时Nova-API不会把初始化数据直接保存到nova数据库的instances表中，而是保存到nova_api数据库的build_requests表，此时虚拟机状态为scheduling。Nova API获取虚拟机信息时会首先查询nova_api的build_requests表，如果存在记录，则直接返回虚拟机信息。正常流程下，当执行完调度后，Nova-Conductor会自动删除build_requests的虚拟机信息。但是在配置多Cell情况下，如果nova_api也是配置其所在Cell的数据库地址，当调度到Compute Cell中时，Compute Cell数据库的build_requests显然找不到该虚拟机的信息，因为实际上信息都保存在Top Cell中，因此Top Cell的build_requests中的虚拟机信息将永远不会被删除。此时我们使用nova list查看的虚拟机信息是从build_requests拿到的过时数据，因此我们查看虚拟机状态永久堵塞在scheduling状态。解决办法是所有Cell的nova_api数据库都配置使用同一个数据库，nova_api数据库本来就是设计为保存公共数据的，为所有的Cell所共享。
2. 分配网络失败导致创建虚拟机失败
解决了上一个问题后，发现仍然创建虚拟机失败，虚拟机一直堵塞在spawning状态直到变成error状态，在计算节点调用virsh list命令发现其实虚拟机已经创建成功，只是状态为pause。Nova-Compute现场日志如下:
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [req-c2854f9b-9b00-45da-a2dd-561a3194c45d a8bfa47e180c41089e4232e76b16ac04 42e34b92273249ff9be85ef3d4fd38b8 - - -] [instance: 6a7377f5-86ac-4767-9110-29085de44e00] Failed to allocate network(s)
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00] Traceback (most recent call last):
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00]   File "/usr/lib/python2.7/site-packages/nova/compute/manager.py", line 1920, in _build_and_run_instance
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00]     block_device_info=block_device_info)
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00]   File "/usr/lib/python2.7/site-packages/nova/virt/libvirt/driver.py", line 2596, in spawn
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00]     post_xml_callback=gen_confdrive)
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00]   File "/usr/lib/python2.7/site-packages/nova/virt/libvirt/driver.py", line 4858, in _create_domain_and_network
2016-12-30 00:56:43.078 84245 ERROR nova.compute.manager [instance: 6a7377f5-86ac-4767-9110-29085de44e00]     raise exception.VirtualInterfaceCreateException()
1
2
3
4
5
6
7
8
通过源码发现，正常流程下，创建虚拟机时，Neutron完成虚拟网卡初始化后会通过Notification机制发送通知到消息队列中，Libvirt会默认一直等待Neutron虚拟网卡初始化成功的事件通知。在多Cell环境下，因为Cell只是Nova的概念，其它服务并不能感知Cell的存在，而Nova的Cell使用了自己的消息队列，Neutron服务发往的是公共消息队列，因此Nova-Compute将永远收不到通知，导致等待事件必然超时，Nova误认为创建虚拟网卡失败了，因此创建虚拟机失败。Godday和CERN同样遇到了相同的问题，解决办法为设置vif_plugging_is_fatal为false，表示忽略Neutron消息等待超时问题，继续执行后续步骤。另外需要设置等待的超时时间vif_plugging_timeout，因为我们已经确定肯定会超时，因此设置一个较短的时间，避免创建虚拟机等待过长，Godday设置为5秒，可以作为参考。
3. nova show查看虚拟机的instance_name与计算节点实际名称不一致。
这是因为instance_name默认模板为instance-x % id，其中id为虚拟机在数据库中的主键id(注意不是UUID)，它是自增长的。比如id为63，转化为十六进制为3f，因此instance_name为instance-0000003f。在多Cell情况下，Top Cell保存了所有的虚拟机信息，而子Cell只保存了其管理Cell的虚拟机信息，保存的虚拟机数量必然不相等，因此同一虚拟机保存在Top Cell和子Cell的ID必然不相同。而我们使用Nova API获取虚拟机信息是从Top Cell中获取的，因此和虚拟机所在的Compute Cell中的instance_name不一致。解决办法为修改instance_name_template值，使其不依赖于id值，我们设置为虚拟机UUID。
4.后端存储问题
当部署小规模OpenStack集群时，我们通常都会使用Ceph分布式共享存储作为Openstack的存储后端，此时Glance、Nova和Cinder是共享存储系统的，这能充分利用RBD image的COW(Copy On Write)特性，避免了镜像文件的远程拷贝，加快了创建虚拟机和虚拟块设备的速度。但当规模很大时，所有的节点共享一个Ceph集群必然导致Ceph集群负载巨大，IO性能下降。因此考虑每个Cell使用独立的Ceph集群，Nova和Cinder共享Ceph集群，Glance是所有Cells的公共服务，需要单独部署并对接其它存储设备。由于Glance和Nova不是共享存储了，因此创建虚拟机时就不能直接Clone了，而必须先从Glance下载Base镜像导入到Ceph集群中。创建虚拟机时，首先看本地Ceph集群是否存在base镜像，存在的话直接Clone即可，否则从远端Glance中下载到本地并导入到Ceph集群中，下次创建虚拟机时就能直接Clone了。存在的问题之一时，目前RBD Image Backend并没有实现缓存功能，因此需要自己开发此功能。问题之二，如何管理Cell内部的Ceph镜像缓存，Glance镜像已经删除后，如果本地也没有虚拟机依赖，则可以把Base镜像删除了，这需要定时任务来清理。问题之三，创建Volume时如何保证和虚拟机在同一个Cell中，因为不同的Cell是不同的Ceph集群，挂载就会有问题，其中一个可选方案是通过Available Zone(AZ)来限制，此时用户在创建虚拟机和Volume时都必须指定AZ，当用户需要挂载Volume到其它Cell的虚拟机时，必须先执行迁移操作。
5.很多功能不能用
由于Nova Cells采用二级调度策略，在调度Cells时并不能拿到所有Hypervisor的信息，因此与之相关的功能都不能用或者出错，比如主机集合、Server Group等，调度功能也大大受限，比如Compute Capabilities Filter、Numa Topology Filter、Trusted Filter等，这些Filters为什么不能用了？假如只有Cell1的主机满足Compute Capabilities，但是在调度Cell时调度到了Cell2，由于Cell2没有符合条件的主机，因此必然会调度失败，但显然我们有合法的宿主机。另外，Nova Cells虽然对用户是透明的，但其本身还是存在隔离的，目前不同Cells之间不支持虚拟机迁移操作，当一个Cell出现故障时，也不能疏散到其它Cell中。
6.虚拟机信息不一致
虚拟机信息被保存在多处，所有的子Cell都必须同步复制到Top Cell中，一旦同步出现问题导致数据不一致性，就可能出现非常棘手的问题。比如在Compute Cell中的某一个虚拟机由于某些原因状态变成ERROR了，但没有同步到Top Cell中，用户看到的还是Active状态，但后续的所有操作都会失败，运维人员必须逐一检查该虚拟机经过的所有Cell的数据库数据，直到Compute Cell，发现状态不一致，必须手动修改数据库，这些操作都是十分危险的，必须具有非常熟练的数据库运维能力。
4.4 Nova Cells”涅磐重生”
前面踩到的坑，社区也发现了这些问题，但由于这是由于Nova Cells的设计缺陷所导致的，要修复这些问题，只通过填填补补是不可能解决的，社区对这些问题的唯一反馈就是不建议在新的环境上部署Cells服务，后续Cells相关文档也不会再更新。目前社区已经彻底放弃了该方案，如今Nova Cells开发已经冻结了，意味着不会再开发任何新特性，也不会修复与之相关的Bug，后续的开发也不会保证Cells的兼容性。
So，Nova Cells已死？值得庆幸的是，Nova Cells其实并没有彻底死去，而是涅槃重生了。从L版开始，社区扔掉原设计的同时，吸取之前的教训，开始着手重新设计Nova Cells并彻底重构代码。为了区分，之前的Nova Cells命名为Nova Cells v1，而新方案命名为Nova Cell v2。Nova Cells v2为了避免Nova Cells v1的问题，一开始就提出了几个明确的设计原则和目标:
1.Nova全面支持Nova-Cells
之前Nova Cells是可选安装的，开启Nova Cells功能，必须额外安装Nova-Cells服务以及额外配置，用户对这个功能的了解和关注程度都是比较低的，而参与开发这一功能的开发者也很少[1]，导致Cells的开发力度不够，部署率低，成熟度低。而对于v2版本，Nova开始全面支持，废弃原来的Nova-Cells服务，不需要额外部署其它任何服务，减少了部署的复杂度，也容易从原来的非Cells架构中升级为Cells架构。在N版之后，Nova Cells将成为必须部署方式，相当于Nova的内置功能，而不再是可选功能，这大大增加了用户和开发者的关注度。
2.分离公共数据，只存放一处
为了解决之前的数据一致性问题，在v2版本中，从M版开始把公共数据从原来的nova数据库中分离出来，放在单独的nova_api数据库中，这些公共数据包括:
flavors；
quotas；
security group、rules；
key pairs；
tags；
migrations；
networks。
此方案解决了公共数据的不一致性问题。另外，Top Cell也不再保存虚拟机信息了，而仅仅保存其UUID与Cell之间映射表，虚拟机信息只保存在其所在的Cell中，Top Cell与Compute Cell之间不再需要复制同步。由于完整数据只存放一处，不存在数据不一致问题，拿到的数据保证是正确的。
3.支持Nova的所有功能
前面提到v1版本存在功能限制，除此之外，对Neutron的支持也缺乏测试和验证。而在v2设计目标中强调将支持所有功能，无任何功能限制，并且全面支持Neutron集成，不再考虑Nova-Network。
最新的v2结构已经不是树状的了，而且没有了Nova-Cells这个组件，其架构如图：
从架构图中可以看出，新版本的Nova Cells v2采用单级调度机制替代了原来的二级调度，由Nova-Scheudler服务负责调度Cell，同时负责选择Cell中的主机。另外还设计了个额外的Cell0模块，如果你在进行虚拟机调度操作时，调度到任何一个Cell都出错，或者没有可用Cell的话，这是系统会先将请求放置在Cell0中，Cell0中只有一个Nova DB，没有消息队列和Nova服务。
Nova Cell v2是一个革命性的变化，其设计目标已经非常明确，也是最期待的方案，但离完全实现还有一定的距离，目前还不支持多Cells调度，很多功能正在紧急开发中，目前还不能投入生产使用，不过社区后续会推出v1升级v2或者非Cell转化为Cell架构的工具。
不过Nova Cells v2也存在问题，我认为：
查询虚拟机信息时，需要首先在Top Cell中拿到所有虚拟机的索引和Cell映射，然后需要往所有的Cells请求数据，这可能导致查询性能低下。
当某个Cell出现故障时，就拿不到这部分虚拟机信息了，这个问题该如何处理？
Cells之间通过消息队列通信，如果跨DC部署，性能就会非常低下。
任何方案都不能是十全十美的，虽然Nova Cell v2也存在问题，但仍值得期待并给予厚望，希望Nova Cells v2不会让我们失望，彻底解决OpenStack大规模部署问题。
总结与展望
本文首先介绍了大规模部署OpenStack存在的主要问题，引出数据库和消息队列是限制大规模部署的主要瓶颈，然后针对这个瓶颈问题介绍了一些组件优化策略，最后详细介绍了几种OpenStack大规模部署的方案，分析了其特点和不足。针对大规模部署问题，Nova Cell v2和Trio2o都是比较值得期待的，其设计理念和目标也比较明确，但是离实现和发展成熟还有一定的距离。Region方案只是共享认证和Dashboard，实现统一管理多OpenStack环境，原则上不算是单OpenStack的大规模部署。Nova Cell v1已经有不少大规模部署的案例，但社区已经不再支持，并且不鼓励在新的环境中部署。如果目前需要上线大规模OpenStack生产环境，有以下两种方案：
使用Nova Cell v2，同时加入v2开发，缺点是开发所有功能的周期不确定，也面临很多变数。
使用Nova Cell v1，部署架构有可供参考的案例，缺点是后续的所有问题都需要自己解决，得不到上游的支持。
当然也可以先部署一套小规模的环境，等Cell v2开发完成后，使用升级工具调整架构，增加Cells功能。
参考文献
[1] Nova Cells V2如何帮助OpenStack集群突破性能瓶颈?
[2] British Telecom threatens to abandon OpenStack in its current form.
[3]benchmarking-openstack-keystone-token-formats
[4]Scaling the CERN OpenStack cloud
[5]OpenStack在天河二号的大规模部署实践
[6]OpenStack Architecture at Go Daddy
[7]Cern Cloud Architecture - February, 2016
[8]Moving to Nova Cells without Destroying the World
[9]OpenStack cascading solution
[10]Test report for OpenStack cascading solution to support 1 million VMs in 100 data centers
[11]Cells v1 status
[12]nova cells flow diagram
[13]Nova cells v2 wiki
[14]nova cells table analysis
[15]ocata nova priorities tracking
--------------------- 
作者：少数pai 
来源：CSDN 
原文：https://blog.csdn.net/karamos/article/details/80130443 
版权声明：本文为博主原创文章，转载请附上博文链接！

