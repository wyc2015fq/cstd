# HBase的region管理 - Spark高级玩法 - CSDN博客
2018年02月08日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1539
HBase 内置的处理拆分和合并的机制一般是合理的，并且它们按照预期处理任务，但在有些情况下，还是需娶按照应用需求对这部分功能进行优化以获得额外的性能改善。
管理拆分
通常HBase 是自动处理region拆分的:一旦它们达到了既定的阈值，region将被拆分成两个，之后它们可以接收新的数据并继续增长。这个默认行为能够满足大多数用例的需求。
其中一种可能出现问题的情况被称之为“拆分/合并风暴”: 当用户的region大小以恒定的速度保持增长时，region拆分会在同一时间发生，因为同时需要压缩region中的存储文件，这个过程会重写拆分之后的region,这将会引起磁盘I/O上升。
与其依赖HBase 自动管理拆分，用户还不如关闭这个行为然后手动调用split 和major.compact 命令。用户可以通过设置这个集群的hbase.hregion.max.filesize值或者在列族级别上把表模式中对应参数设置成非常大的值来完成。为防止手动拆分无法运行，最好不要将其设置为Long.MAX_VALUE.用户最好将这个值设置为一个合理的
上限，例如，100GB (如果触发的话将会导致一个小时的major合并)。
手动运行命令来拆分和压缩region的好处是可以对它们进行时间控制。在不同
region 上交错地运行，这样可以尽可能分散I/O 负载，并且避免拆分/合并风暴。用户可以实现一个可以，调用split()和majorCompact()方法的客户端。也可以使用Shell 交互地调用相关命令，或者使用crond定时地运行它们。用户可以参见RegionSplitter 类(0.90.2 版本添加进来的) 的另一种拆分region 的方法: 其拥有滚动
拆分(rollingsplit) 的特性，用户可以使用该功能拆分正在长时间等待合并操作完成的region (参见-r 和-O 命令行选项)。
另外一个手动管理拆分的优势是用户能够更好地在任意时间控制哪些region 可用。这
对于用户需要解决底层bug 这种少数情况来说是十分有用的，例如，排查某一个region的问题。在使用自动拆分时，用户可能发现要检查的region
已经被两个拆分后的子region 替代了。这些子region 有新的名字，并且客户端需要大量的时间重新定位region,这使得查询所需要的信息变得更加困难。
region 热点
唯一可以缓解这种现象的途径就是手动地将热点region 按特定的边界拆分出一个或多个新region,然后将子region 负载分布到多个region 服务器上。用户可以为region 指定一个拆分行键，即region 被拆分为两部分的位置。用户可以指定region 中任意的行键,这样用户也可以生成大小完全不同的两个region。这个只能在处理非完全连续的行键范围时起作用，因为采用连续的行键时，过一段时间插入的数据总会集中到最近生成的几个region 上。用户需采用盐析主键(saltkey) 或者使用随机的行键来把负载均衡到所有的服务器。
表热点
大部分region分布并不均匀，即大多数region位于同一个region 服务器上，对于拥有很多region 的表来说。这就意味着,即使用随机的key 来写入数据，某一台region服务器的负载仍大于其他的region服务器。用户可以从HBase Shell 或者使用HBaseAdmin 类中的API,并通过move()函数显式地把region从一个region服务器移动到另一个region服务器。另外一个方法就是使用unassign()方法或者Shell命令简单地从当前服务器移除受影响的表的region,master 会立即将其部署到其他region服务器上。
预拆分region
管理拆分能够在集群负载增加时有效地进行负载控制。但是,用户仍然会面临的一个问题是，在用户初始创建一张新表之后，用户需要频繁地拆分region,因为建立的新表通常只有一个region,不推荐让单个region 增长到太大。因此，在表创建时，最好就有较大数量的region.用户可以在创建表时指定需要的region 数目来达到预拆分的目的。
管理接口中的createTable()方法和Shell中的create命令都可以接受以列表形式提供的拆分行键作为参数，该参数在创建表的时候会被用来预拆分region。HBase提供了一个能帮助用户创建预拆分表的工具RegionSplitter。不含参数时它将会显示使用
说明信息:
hbase org.apache.hadoop.hbase.util.RegionSplitter
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW26fDciaXUx8IkLgOLyJzIpusWMFRGG0YrqXGCmnt4YLvvNLsnTnaaXecA889bFonPpgUjFGB9p7w/640?wx_fmt=png)
默认采用的是MD5StringSplit类将行键拆分到不同的段中。用户能够通过实现提供的splitAlgorithm接口来定义自己的算法，并通过使用-D split.algorithm=<your-algorithm-class>参数将它融入到工具之中。
Shell命令指定预拆分
create'ns1:t1', 'f1', SPLITS => ['10', '20', '30', '40'] 
也可以指定文件。
1. create'split_table_test',{NAME =>'cf', COMPRESSION => 'SNAPPY'}, {SPLITS_FILE => 'region_split_info.txt'}  
这里注意，一定要将分区的参数指定单独用一个大括号扩起来，因为分区是针对全表，而不是针对某一个column family。
**决定预拆分**
关于如何设定预拆分的region 数量，用户可以先按照每个服务器10 个region 来进行
预拆分,随着时间的推移观察数据的增长情况。先设置较少的region 数目再稍后滚动
拆分它们是一种更好的方法，因为过多的region 通常会影响集群性能。
另一种方法是,用户可以基于region 中最大的存储文件大小来决定预拆分region 的数量，
随着数据的增加，该大小会随之一起增加。用户希望最大的region 正好能够跳过major 合并，否则用户可能会面对前面所提到的compaction 风暴。
如果用户将region 预拆分的太小，可以通过增加hbase.hregion.majorcompaction 的值来加大major合并的间隔。如果用户的数据规模增加过大，用户可以使用RegionSplitter
工具在所有region 上通过网络l/O 执行安全的滚动拆分。
使用手动拆分和预拆分是高级概念，需要用户有谨慎的计划并仔细监控操作时HBase系统的运行情况。另一方面,这能够避免全局一致的数据增长造成的合并风暴，并可以通过手动拆分摆脱region 热点的困扰。
负载均衡
master 有一个内置的叫做均衡器的特性。在默认的情况下，均衡器每五分钟运行一次,这是通过hbase.balancer.period属性设置的。一旦均衡器启动，它将会尝试均匀分配region 到所有region 服务器。启动均衡器时,均衡器首先会确定一个region 分配计划,
该计划用于描述region 如何移动。然后通过迭代调用管理API 中的unassign()方法开
始移动region。
均衡器有一个可以限制自身运行时间的上限，用户可以通过属性hbase.balancer.max.balancing来配置，默认设置为均衡器运行间隔周期的一半，即两分半钟。
用户可以通过均衡器开关来控制均衡器: 使用Shell 的balance_swith 命令来更改均衡器
的开启和关闭状态，或者使用balanceSwitch()接口来做同样的事情。当禁用均衡器的
时候，它将不会如预期一样自动运行。
均衡器可以显式地使用balancer 命令进行启动，同时也可以使用API 中的balancer()
方法。以上介绍的自动均衡过程会隐式地调用这个方法。HBase会判断如果需要负载均
衡就返回true,返回false 则意味着不能运行均衡器，
原因要么是开关被关闭或者没有工作需要做(已经达到均衡了),也有可能其他工作阻止了其运行。例如，一个region处于事务列表中:
如果一个region 正处于状态转换时，均衡操作将会被跳过。
除了依赖均衡器完成自己的工作,用户还可以使用move命令和API方法显式地将region
移动到另一个服务器上。当用户想控制某张表特定region 的确切位置时，
这种方法是很有用的。详细内容浪尖会在后续的文章中讲述。
合并region
当用户向相应的表中插入数据时，region 自动拆分的情况是很常见的。当然在某些特殊
情况下，用户有可能需要合并region。例如，用户删除大量数据并且想减少每个服务器
管理的region 数目。
HBase 集成了一个工具能够让用户在集群没有工作时合并两个相邻的region。可以使用
命令行工具来获得使用说明:
hbase org.apache.hadoop.hbase.util.Merge
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW26fDciaXUx8IkLgOLyJzIpXFtZAEheewbO99XoDIbdvvqibgcnYribfCEea12UnGib17rWc9We0pJQg/640?wx_fmt=png)
**推荐阅读：**
1，[HBase在滴滴出行的应用场景和最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484343&idx=1&sn=cd0c5bd1fbd8d55dd9779d336880bb3d&chksm=9f38e09fa84f6989105d027e60beb463332313724813ea114ec3f4e29707c8da75fc09434269&scene=21#wechat_redirect)
2，[HBase在滴滴出行的应用场景和最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484343&idx=1&sn=cd0c5bd1fbd8d55dd9779d336880bb3d&chksm=9f38e09fa84f6989105d027e60beb463332313724813ea114ec3f4e29707c8da75fc09434269&scene=21#wechat_redirect)
3，[Hbase源码系列之regionserver应答数据请求服务设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483723&idx=1&sn=177f7efd2d9155614f40bc46c14c3104&chksm=9f38e263a84f6b756436e068ebb00afc4a81a476d61dbf03711dae062969751ce471aa2219f1&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
