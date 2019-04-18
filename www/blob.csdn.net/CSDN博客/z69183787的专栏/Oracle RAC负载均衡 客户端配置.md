# Oracle RAC负载均衡 客户端配置 - z69183787的专栏 - CSDN博客
2014年04月12日 18:18:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1976
#### 附上DBA给的客户端 配置
![](https://img-blog.csdn.net/20140412185204484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
#### RAC的负载均衡
RAC的负载均衡主要是指新会话连接到RAC数据库时，如何判定这个新的连接要连到哪个节点进行工作。在RAC中，负载均衡分为两种，一种是基于客户端连接的，另外一种是基于服务器端的。
客户端的负载均衡配置相对简单，只需要在tnsnames.ora中添加LOAD_BALANCE=ON这么一个选项即可。比如下面的TNS：
> 
RAC =
(DESCRIPTION =
(ADDRESS = (PROTOCOL = TCP)(HOST = rac1-vip)(PORT = 1521))
(ADDRESS = (PROTOCOL = TCP)(HOST = rac2-vip)(PORT = 1521))
(LOAD_BALANCE = ON)** on 和 yes 作用一致。**
(FAILOVER = ON)
(CONNECT_DATA =
(SERVER = DEDICATED)
(SERVICE_NAME = rac)
)
)
这样当客户端连接RAC数据库时，会随机在TNS里面挑个监听地址进行连接。在Oracle10g以前，假如有节点宕机或者类似事故时，客户端可能还是选择连接到这个节点，这样会发生较长时间的TCP等待超时。而在10g以后，由于VIP和FAN的引入，这样的情况可以得到很大程度的改善。客户端的负载均衡在通常情况下能够较好地工作，但是由于连接是在客户端随机发起的，这样客户端并不知道RAC各节点的负荷及连接数情况，有可能负荷大的节点还会源源不断地增加新的连接，导致RAC节点无法均衡工作。
从Oracle 10g开始，服务器端的负载均衡可以根据RAC中各节点的负荷及连接数情况，而判定将新的客户端连接分配到负荷最小的节点上去。RAC中各节点的PMON进程每3秒会将各自节点的负荷（包括LOAD、最大LOAD、CPU使用率）及连接数更新到service_register里面，然后假如节点的负荷有发生变化，将会通知到监听程序，由监听程序再决定新的客户端连接分配至哪个节点。假如RAC中一个节点的监听失败了，PMON每一分钟会去检查一次是否已经恢复正常。
服务器端的监听配置是在各节点的tnsnames.ora里面添加一个连接到各个节点监听的条目，然后再在初始化参数里面设置remote_listeners这个参数。比如：
> 
LISTENERS_RAC =
(ADDRESS_LIST =
(ADDRESS = (PROTOCOL = TCP)(HOST = rac1-vip)(PORT = 1521))
(ADDRESS = (PROTOCOL = TCP)(HOST = rac2-vip)(PORT = 1521))
)
ALTER SYSTEM SET REMOTE_LISTENER = LISTENERS_RAC;
这样服务器端的LOAD BALANCE便配置完成。
但是有时候由于PMON取节点负荷的延迟，导致客户端连接可能还是会连接到负荷较大的节点上，这时候便可以在服务器各节点的listener.ora里面加入PREFER_LEAST_LOADED_NODE=OFF这么一行，这样服务器端的负载均衡将不再根据节点的负荷来进行分配，而是根据节点的连接数进行分配，达到各个节点连接数比较平衡的效果。
另外一个不得不说的便是并行操作，假如有个会话连接以后要进行并行操作。由于连接时是按负荷或连接数连接，这样可能连接时各个节点连接数和负荷等比较平衡，但是这个并行会话启动多个并行进程以后，那么这个节点的负荷及连接数就会有可能上升得比较快。如果在RAC中开启了节点并行，那么有可能会把并行进程分配到多个节点运行以达到负载均衡的效果。
从Oracle 10.2开始，Oracle引入了Load Balance Advisor，对负载均衡有了进一步的改进。结合Service，可以对不同的SERVICE设置不同的负载均衡策略。Load Balance Advisor的配置可以通过DBMS_SERVICE包对SERVICE进行更改而完成。在Load Balance Advisor首先必须设置SERVICE负载均衡的目标，目标分为3种：
> 
GOAL_NONE Disables the load balancing advisory
GOAL_SERVICE_TIME The LBA calculates a weighted moving average of the total elapsed time for completed work plus the bandwidth that’s available to the service to calculate the service goodness. This goal is ideal for services whose workload may change dramatically
 over a short period of time, e.g. an application that services a “clicks and mortar” store that provides customer self-service through an internet-based shopping web site.
GOAL_THROUGHPUT The LBA calculates a weighted moving average of throughput (i.e. the rate at which work is completed) in addition to the bandwidth available to the service to calculate the service goodness. This goal is best suited for long-duration tasks that
 are typically queued to run serially, e.g. scheduled jobs that handle large batches of transactions.
另外可以额外设置连接的负载均衡：
> 
CLB_GOAL_SHORT The Load Balancing Advisory will be used for connection load balancing only if it is enabled (i.e. set to other than GOAL_NONE). If the LBA has been disabled, connection load balancing
 will utilize abridged advice determined by CPU utilization.
CLB_GOAL_LONG Connection load balancing will be determined by first tallying the total number of connections per instance, and then by counting the number of sessions per each service. Oracle recommends using this setting for services whose applications tend
 to connect for long periods of time (e.g. Oracle Forms). The Load Balancing Advisory can be used in conjunction with this setting as long as the connection pool has been sized to accommodate “gravitation “ within the pool without adding or subtracting connections.
 Oracle recommends this option as the most efficient design.

只要配置了REMOTE_LISTENER参数，LOAD_BALANCE参数就自动启用，即使在TNSNAMES.ORA中明确设置为OFF，或者在TNSNAMES.ORA中只配置一个IP地址。
当没有设置REMOTE_LISTENER参数，则TNSNAMES.ORA中配置的LOAD_BALANCE参数决定是否实现LOAD_BALANCE功能，而如果没有进行设置，则默认LOAD_BALANCE参数为ON。
