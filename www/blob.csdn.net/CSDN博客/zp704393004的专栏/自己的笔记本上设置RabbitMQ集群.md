# 自己的笔记本上设置RabbitMQ集群 - zp704393004的专栏 - CSDN博客





2018年04月24日 10:44:37[原来未知](https://me.csdn.net/zp704393004)阅读数：171
个人分类：[学习记录																[rabbiMQ](https://blog.csdn.net/zp704393004/article/category/7600136)](https://blog.csdn.net/zp704393004/article/category/7595660)








RabbitMQ 是用 erlang 开发的，集群非常方便，因为 erlang 天生就是一门分布式语言，但其本身并不支持负载均衡。Rabbit 模式大概分为以下三种：单一模式、普通模式和镜像模式。

**单一模式**：最简单的情况，非集群模式。

　　没什么好说的。

**普通模式**：默认的集群模式。

　　对于 Queue 来说，消息实体只存在于其中一个节点，A、B 两个节点仅有相同的元数据，即队列结构。当消息进入 A 节点的 Queue 中后，consumer 从 B 节点拉取时，RabbitMQ 会临时在 A、B 间进行消息传输，把 A 中的消息实体取出并经过 B 发送给 consumer. 所以 consumer 应尽量连接每一个节点，从中取消息。即对于同一个逻辑队列，要在多个节点建立物理 Queue。否则无论 consumer 连 A 或 B，出口总在 A，会产生瓶颈。该模式存在一个问题就是当 A 节点故障后，B 节点无法取到 A 节点中还未消费的消息实体。如果做了消息持久化，那么得等 A 节点恢复，然后才可被消费；如果没有持久化的话，然后就没有然后了……

**镜像模式**：把需要的队列做成镜像队列，存在于多个节点，属于 RabbitMQ 的 HA 方案。

　　该模式解决了上述问题，其实质和普通模式不同之处在于，消息实体会主动在镜像节点间同步，而不是在 consumer 取数据时临时拉取。该模式带来的副作用也很明显，除了降低系统性能外，如果镜像队列数量过多，加之大量的消息进入，集群内部的网络带宽将会被这种同步通讯大大消耗掉,所，在对可靠性要求较高的场合中适用。

## 6.1 集群中的基本概念

　　RabbitMQ 的集群节点包括内存节点、磁盘节点。顾名思义内存节点就是将所有数据放在内存，磁盘节点将数据放在磁盘。不过，如前文所述，如果在投递消息时，打开了消息的持久化，那即使是内存节点，数据还是安全的放在磁盘。

　　一个 RabbitMQ 集群中可以共享 user、vhost、queue、exchange 等，所有的数据和状态都是必须在所有节点上复制的，一个例外是那些当前只属于创建它的节点的消息队列，尽管它们可见且可被所有节点读取。 RabbitMQ 节点可以动态地加入到集群中，一个节点它可以加入到集群中，也可以从集群环集群进行一个基本的负载均衡。

**集群中有两种节点**：
- **内存节点**：只保存状态到内存（一个例外的情况是：持久的 queue 的持久内容将被保存到 disk）；
- **磁盘节点**：保存状态到内存和磁盘，内存节点虽然不写入磁盘，但是它执行比磁盘节点要好，集群中，只需要一个磁盘节点来保存状态 就足够了，如果集群中只有内存节点，那么不能停止它们，否则所有的状态，消息等都会丢失。

**思路**：

　　那么具体如何实现 RabbitMQ 高可用，咱们先搭建一个普通集群模式，在这个模式基础上再配置镜像模式实现高可用，Rabbit 集群前增加一个反向代理，生产者、消费者通过反向代理访问 RabbitMQ 集群。

**架构图**：

![7](https://img-blog.csdn.net/20170520092311712)

上述图里是 3 个 RabbitMQ 运行在同一主机上，分别用不同的服务端口。当然咱们的实际生产中，多个 RabbitMQ 肯定是运行在不同的物理服务器上，否则就失去了高可用的意义。

## 6.2 集群模式配置

设计架构可以如下：在一个集群里，有 4 台机器，其中 1 台使用磁盘模式，另 2 台使用内存模式。2 台内存模式的节点，无疑速度更快，因此客户端（consumer、producer）连接访问它们。而磁盘模式的节点，由于磁盘 IO 相对较慢，因此仅作数据备份使用，另外一台作为反向代理。

四台服务器 hostname 分别为：queue 、panyuntao1、panyuntao2、panyuntao3（ip:172.16.3.110）

配置 RabbitMQ 集群非常简单，只需要几个命令，配置步骤如下：

**Step 1**：queue、panyuntao1、panyuntao2 做为 RabbitMQ 集群节点，分别安装 RabbitMq-Server ，安装后分别启动 RabbitMq-server。

启动命令`# Rabbit-Server start`，安装过程及启动命令参见：`http://www.cnblogs.com/flat_peach/archive/2013/03/04/2943574.html`

**Step 2**：在安装好的 3 台节点服务器中，分别修改`/etc/hosts`文件，指定queue、panyuntao1、panyuntao2 的 hosts，如

```
172.16.3.32 queue

172.16.3.107 panyuntao1

172.16.3.108 panyuntao2
```
- 1
- 2
- 3
- 4
- 5

还有 hostname 文件也要正确，分别是 queue、panyuntao1、panyuntao2，如果修改 hostname 建议在安装 rabbitmq 前修改。请注意 RabbitMQ 集群节点必须在同一个网段里，如果是跨广域网效果就差。

**Step 3**：设置每个节点 Cookie。RabbitMQ 的集群是依赖于 erlang 的集群来工作的，所以必须先构建起 erlang 的集群环境。Erlang 的集群中各节点是通过一个 magic cookie 来实现的，这个 cookie 存放在`/var/lib/rabbitmq/.erlang.cookie`中，文件是 400 的权限。所以必须保证各节点 cookie 保持一致，否则节点之间就无法通信。
`-r--------. 1 rabbitmq rabbitmq 20 3月 5 00:00 /var/lib/rabbitmq/.erlang.cookie`- 1

将其中一台节点上的`.erlang.cookie`值复制下来保存到其他节点上，或者使用`scp`的方法也可，但是要注意文件的权限和属主属组。咱们这里将 queue 中的 cookie 复制到 panyuntao1、panyuntao2 中，先修改下 panyuntao1、panyuntao2 中的`.erlang.cookie`权限：
`#chmod 777  /var/lib/rabbitmq/.erlang.cookie `- 1

将 queue 的`/var/lib/rabbitmq/.erlang.cookie`这个文件，拷贝到 panyuntao1、panyuntao2 的同一位置（反过来亦可），该文件是集群节点进行通信的验证密钥，所有节点必须一致，拷完后重启下 RabbitMQ. 复制好后别忘记还原`.erlang.cookie`的权限，否则可能会遇到错误：
`#chmod 400 /var/lib/rabbitmq/.erlang.cookie `- 1

设置好 cookie 后先将三个节点的 RabbitMQ 重启：

```
# rabbitmqctl stop
# rabbitmq-server start
```
- 1
- 2

**Step 4**：停止所有节点 RabbitMQ 服务，然后使用 detached 参数独立运行，这步很关键，尤其增加节点停止节点后再次启动遇到无法启动都可以参照这个顺序。

```cpp
queue# rabbitmqctl stop
panyuntao1# rabbitmqctl stop
panyuntao2# rabbitmqctl stop

queue# rabbitmq-server -detached
panyuntao1# rabbitmq-server -detached
panyuntao2# rabbitmq-server -detached
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

分别查看下每个节点:

```
queue# rabbitmqctl cluster_status
Cluster status of node rabbit@queue ...
[{nodes,[{disc,[rabbit@queue]}]},
{running_nodes,[rabbit@queue]},
{partitions,[]}]
...done.

panyuntao1# rabbitmqctl cluster_status 
Cluster status of node rabbit@panyuntao1...
[{nodes,[{disc,[rabbit@panyuntao1]}]},
{running_nodes,[rabbit@panyuntao1]},
{partitions,[]}]
...done.

panyuntao2# rabbitmqctl cluster_status
Cluster status of node rabbit@panyuntao2...
[{nodes,[{disc,[rabbit@panyuntao2]}]},
{running_nodes,[rabbit@panyuntao2]},
{partitions,[]}]
...done.
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20

**Step  5**：将 panyuntao1、panyuntao2 作为内存节点与 queue 连接起来，在 panyuntao1 上，执行如下命令

```
panyuntao1# rabbitmqctl stop_app
panyuntao1# rabbitmqctl join_cluster --ram rabbit@queue   
panyuntao1# rabbitmqctl start_app

panyuntao2# rabbitmqctl stop_app
panyuntao2# rabbitmqctl join_cluster --ram rabbit@queue
panyuntao2# rabbitmqctl start_app
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

上述命令先停掉 RabbitMQ 应用，然后调用 cluster 命令，将 panyuntao1 连接到，使两者成为一个集群，最后重启 RabbitMQ 应用。在这个 cluster 命令下，panyuntao1、panyuntao2 是内存节点，queue 是磁盘节点（RabbitMQ启动后，默认是磁盘节点）。

queue 如果要使 panyuntao1 或 panyuntao2 在集群里也是磁盘节点，`join_cluster`命令去掉`--ram`参数即可。
` #rabbitmqctl join_cluster rabbit@queue `- 1

只要在节点列表里包含了自己，它就成为一个磁盘节点。在 RabbitMQ 集群里，必须至少有一个磁盘节点存在。

**Step 6**：在 queue、panyuntao1、panyuntao2 上，运行`cluster_status`命令查看集群状态。

```
[root@queue ~]# rabbitmqctl cluster_status
Cluster status of node rabbit@queue ...
[{nodes,[{disc,[rabbit@queue]},{ram,[rabbit@panyuntao2,rabbit@panyuntao1]}]},
{running_nodes,[rabbit@panyuntao2,rabbit@panyuntao1,rabbit@queue]},
{partitions,[]}]
...done.

[root@panyuntao1 rabbitmq]# rabbitmqctl cluster_status
Cluster status of node rabbit@panyuntao1 ...
[{nodes,[{disc,[rabbit@queue]},{ram,[rabbit@panyuntao2,rabbit@panyuntao1]}]},
{running_nodes,[rabbit@panyuntao2,rabbit@queue,rabbit@panyuntao1]},
{partitions,[]}]
...done.

[root@panyuntao2 rabbitmq]# rabbitmqctl cluster_status
Cluster status of node rabbit@panyuntao2 ...
[{nodes,[{disc,[rabbit@queue]},{ram,[rabbit@panyuntao2,rabbit@panyuntao1]}]},
{running_nodes,[rabbit@panyuntao1,rabbit@queue,rabbit@panyuntao2]},
{partitions,[]}]
...done.
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20

这时咱们可以看到每个节点的集群信息，分别有两个内存节点和一个磁盘节点。

**Step 7**：往任意一台集群节点里写入消息队列，会复制到另一个节点上，咱们会看到两个节点的消息队列数一致。

```
root@panyuntao2 :~# rabbitmqctl list_queues -p hrsystem

Listing queues …
test_queue 10000
…done.

root@panyuntao1 :~# rabbitmqctl list_queues -p hrsystem
Listing queues …
test_queue 10000
…done.

root@queue:~# rabbitmqctl list_queues -p hrsystem
Listing queues …
test_queue 10000
…done.
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15

其中，`-p`参数为vhost名称。

这样RabbitMQ集群就正常工作了,

这种模式更适合非持久化队列，只有该队列是非持久的，客户端才能重新连接到集群里的其他节点，并重新创建队列。假如该队列是持久化的，那么唯一办法是将故障节点恢复起来。  

为什么 RabbitMQ 不将队列复制到集群里每个节点呢？这与它的集群的设计本意相冲突，集群的设计目的就是增加更多节点时，能线性的增加性能（CPU、内存）和容量（内存、磁盘）。理由如下：
- 
**storage space**: If every cluster node had a full copy of every queue, adding nodes wouldn’t give you more storage capacity. For example, if one node could store 1GB of messages, adding two more nodes would simply give you two more copies of the same 1GB of messages.

- 
**performance**: Publishing messages would require replicating those messages to every cluster node. For durable messages that would require triggering disk activity on all nodes for every message. Your network and disk load would increase every time you added a node, keeping the performance of the cluster the same (or possibly worse).


当然 RabbitMQ 新版本集群也支持队列复制（有个选项可以配置）。比如在有五个节点的集群里，可以指定某个队列的内容在 2 个节点上进行存储，从而在性能与高可用性之间取得一个平衡。

## 6.3 镜像模式配置

上面配置 RabbitMQ 默认集群模式，但并不保证队列的高可用性，尽管交换机、绑定这些可以复制到集群里的任何一个节点，但是队列内容不会复制，虽然该模式解决一部分节点压力，但队列节点宕机直接导致该队列无法使用，只能等待重启，所以要想在队列节点宕机或故障也能正常使用，就要复制队列内容到集群里的每个节点，需要创建镜像队列。

下面，咱们看看如何镜像模式来解决复制的问题，从而提高可用性 。

**Step 1：增加负载均衡器**

关于负载均衡器，商业的比如 F5 的 BIG-IP，Radware 的 AppDirector 是硬件架构的产品，可以实现很高的处理能力。但这些产品昂贵的价格会让人止步，所以咱们还有软件负载均衡方案。互联网公司常用的软件 LB 一般有 LVS、HAProxy、Nginx 等。LVS 是一个内核层的产品，主要在第四层负责数据包转发，使用较复杂。HAProxy 和 Nginx 是应用层的产品，但 Nginx 主要用于处理 HTTP，所以这里选择 HAProxy 作为 RabbitMQ 前端的 LB.

HAProxy 的安装使用非常简单，在 CentOS 下直接`yum install haproxy`，然后更改`/etc/haproxy/haproxy.cfg`文件即可，文件内容大概如下：

![13](https://img-blog.csdn.net/20170520100946562)

负载均衡器会监听 5672 端口，轮询咱们的两个内存节点`172.16.3.107`和`172.16.3.108`的 5672 端口，`172.16.3.32`为磁盘节点，只做备份不提供给生产者、消费者使用，当然如果我们服务器资源充足情况也可以配置多个磁盘节点，这样磁盘节点除了故障也不会影响，除非同时出故障。

**Step 2：配置策略**

使用 Rabbit 镜像功能，需要基于 RabbitMQ 策略来实现，政策是用来控制和修改群集范围的某个 vhost 队列行为和 Exchange 行为。在 cluster 中任意节点启用策略，策略会自动同步到集群节点：
`# rabbitmqctl set_policy -p hrsystem ha-allqueue"^" '{"ha-mode":"all"}'`- 1

这行命令在 vhost 名称为 hrsystem 创建了一个策略，策略名称为 ha-allqueue，策略模式为 all 即复制到所有节点，包含新增节点，策略正则表达式为`“^”`表示所有匹配所有队列名称。例如：
`rabbitmqctl set_policy -p hrsystem ha-allqueue "^message" '{"ha-mode":"all"}'`- 1

**注意**：`"^message"`这个规则要根据自己修改，这个是指同步`"message"`开头的队列名称，咱们配置时使用的应用于所有队列，所以表达式为`"^"`，官方`set_policy`说明参见：
`set_policy [-p vhostpath] {name} {pattern} {definition} [priority]`- 1

**ha-mode**：

![14](https://img-blog.csdn.net/20170520101619602)

**step3**：

创建队列时需要指定`ha`参数，如果不指定`x-ha-prolicy`的话将无法复制。下面`C#`代码片段：

```
using ( var bus = RabbitHutch.CreateBus(ConfigurationManager .ConnectionStrings["RabbitMQ"].ToString())) {
    bus.Subscribe< TestMessage>("word_subscriber" , message => RunTable(message),x=>x.WithArgument("x-ha-policy" , "all"));
    Console.WriteLine("Subscription Started. Hit any key quit" );
    Console.ReadKey();
}
```
- 1
- 2
- 3
- 4
- 5

客户端使用负载服务器`172.16.3.110`（panyuntao3）发送消息，队列会被复制到所有节点，当然策略也可以配置制定某几个节点，这时任何节点故障 、或者重启将不会影响我们正常使用某个队列。到这里，咱们就完成了 RabbitMQ 的高可用配置（所有节点都宕机那没有办法了）。使用 RabbitMQ 管理端可以看到集群镜像模式中对列状态：

![15](https://img-blog.csdn.net/20170520101918076)

下面是教材上内容，有问题：（应该是没有禁用插件）

1、确保所有的rabbitMQ服务已经关闭（rabbitmqctl stop)，关闭所有的插件或者卸载所有的插件

2、默认的端口是5672，这样直接启动三个是会冲突的，需要利用RABBITMQ_NODENAME和RABBITMQ_NODE_PORT环境变量来知名制定唯一节点名称和段口号。

$ RABBITMQ_NODE_PORT=5672 RABBITMQ_NODENAME=rabbit rabbitmq-server -detached

Warning: PID file not written; -detached was passed.


$ RABBITMQ_NODE_PORT=5673 RABBITMQ_NODENAME=rabbit_1 rabbitmq-server -detached

Warning: PID file not written; -detached was passed.



$ RABBITMQ_NODE_PORT=5674 RABBITMQ_NODENAME=rabbit_2 rabbitmq-server -detached

Warning: PID file not written; -detached  was passed.

为了保证成功，需要禁用加载的插件：（参考我转载的一个文章）

后面的步骤很简单：

1、停止第二个节点上的RabbitMQ应用程序：

rabbitmqctl -n rabbit_1@localhost stop_app

rabbitmqctl -n rabbit_2@localhost stop_app


2、晴空状态

rabbitmqctl -n rabbit_1@localhost reset

rabbitmqctl -n rabbit_2@localhost reset


3、加入：

rabbitmqctl -n rabbit_1@localhost cluster rabbit@localhost rabbit_1@localhost

最后成功的是：

rabbitmqctl -n rabbit_1@localhost join_cluster rabbit@localhost


Clustering node rabbit_1@localhost with rabbit@localhost ...

rabbitmqctl -n rabbit_2@localhost join_cluster rabbit@localhost --ram【为了将rabbit_2加入ram，另外两个为disc】


4、重启

rabbitmqctl -n rabbit_1@localhost start_app

rabbitmqctl -n rabbit_2@localhost start_app

5、查看状态：

rabbitmqctl cluster_status








