# 探秘Hadoop生态2：HDFS常用命令与高可用方案 - 程序员江湖 - CSDN博客





2017年03月08日 09:40:46[黄小斜](https://me.csdn.net/a724888)阅读数：371
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









HDFS常用命令： 

注：以下执行命令均在spark安装目录的bin目录下。 

path 为路径 src为文件路径 dist 为文件夹 

1、-help[cmd] 显示命令的帮助信息

./hdfs dfs -helpls
- 1

2、-ls(r) 显示当前目录下的所有文件 -R层层循出文件夹

./hdfs dfs -ls /log/map./hdfs dfs -lsr /log/   (递归的)
- 1
- 2

3、-du(s) 显示目录中所有文件大小,或者当只指定一个文件时，显示此文件的大小

./hdfs dfs -du /user/hadoop/dir1 /user/hadoop/file1 hdfs://host:port/user/hadoop/dir1
- 1

4、-count[-q] 显示当前目录下的所有文件大小 

5、-mv 移动多个文件目录到目标目录

./hdfs dfs -mv /user/hadoop/file1 /user/hadoop/file2
- 1

6、-cp 复制多个文件到目标目录

./hdfs dfs -cp /user/hadoop/file1 /user/hadoop/file2 (将文件从源路径复制到目标路径。这个命令允许有多个源路径，此时目标路径必须是一个目录。)
- 1
- 2

7、-rm(r) 删除文件（夹）

./hdfs dfs -rmr /log/map1  (递归删除)
- 1

8、-put 本地文件复制到hdfs

./hdfs dfs -put test.txt /log/map/
- 1

9、-copyFromLocal 本地文件复制到hdfs

./hdfs dfs -copyFromLOcal /usr/data/text.txt /log/map1/   (将本地的text.txt 复制到hdfs的/log/map1/下)
- 1

10、-moveFromLocal 本地文件移动到hdfs

./hdfs dfs -moveFromLocal /usr/data/text.txt /log/map1/   (将本地的text.txt移动到hdfs的/log/map1/下)
- 1

11、-get[-ignoreCrc] 复制文件到本地，可以忽略crc校验

./hdfs dfs -get /log/map1/*  . (复制到本地当前目录下)/hdfs dfs -get /log/map1/* /usr/data （将hdfs下的/log/map1/下的所有文件全部复制到本地的/usr/data/下 ）
- 1
- 2

12、-getmerge[addnl] 将源目录中的所有文件排序合并到一个文件中，接受一个源目录和一个目标文件作为输入，并且将源目录中所有的文件连接成本地目标文件。addnl是可选的，用于指定在每个文件结尾添加一个换行符。

./hdfs dfs -getmerge /log/map1/* /usr/data（将hdfs上的/log/map1/下的所有文件合并下载到本地的/usr/data下）
- 1

13、-cat 在终端显示文件内容

./hdfs dfs -cat /log/map1/part-00000| head (读取hdfs上的/log/map1下的part-00000文件    head参数，代表前十行。) /hdfs dfs -tail /log/map1/part-00000 (查看文件的最后一千行)
- 1
- 2
- 3

14、-text 在终端显示文件内容，将源文件输出为文本格式。允许的格式是zip和TextRecordInputStream 

15、-copyToLocal[-ignoreCrc] 复制文件到本地 

16、-moveToLocal 移动文件到本地 

17、-mkdir 创建文件夹 后跟-p 可以创建不存在的父路径

./hdfs dfs -mkdir -p /dir1/dir11/dir111  
- 1

18、-touchz 创建一个空文件

19、-grep 从hdfs上过滤包含某个字符的行内容

./hdfs dfs -cat /log/testlog/* | grep 过滤字段









**HDFS1.x、2.x架构图**

后面开始整理之前学习HDFS、Yarn时的笔记。

**HDFS 1.x**



![](https://img-blog.csdn.net/20180705103238438?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



主要角色：
- Namenode * 1
- 存储、管理元数据，包括文件信息、文件与数据块的映射。
- 处理与元数据相关的操作，如枚举路径、查看文件的逻辑大小等。
- 集群的协调者，如Datanode上下线、数据块的生命周期管理。
- Datanode * n
- 存储、管理数据块。
- 处理文件内容的读、写请求。
- Client * n
- 集群的使用者，通过具体的FileSystem访问集群（门面模式）。

**HDFS 2.x**

与HDFS 1.x相比，2.x新增了两个重要feature：HA、Federation。

![](https://img-blog.csdn.net/2018070510325184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

角色变化如下：
- Namenode * 2k
- HA支持一个Active Namenode和一个Standby Namenode，以一个Namespace（或Nameservice）的形式对外服务。在active挂时，自动将将standby切换为新的active，旧的active恢复后成为standby。
- Federation将多个Namespace合并为一个视图，用户可通过viewfs访问。
- Datanode * n
- 所有节点共同对所有Namespace服务。在Datanode上，各Namespace拥有单独的目录（blockpool）。
- Client * n
- 2.x的Federation通过客户端挂载表实现，要通过ViewFileSystem访问NameNode。（这一点也算不上变化，多了一种门面而已）

HA大大加强了HDFS的可用性；Federation则将多个集群的视图合并在一起，方便用户使用。两个特性非常重要，HDFS 3.x在这两个feature上继续工作，如今HA以支持多standby，Federation支持服务器端路由。后面再整理实现方案。

**大数据集群高可用之hdfs**

**hdfs如何保证高可用**




![](https://img-blog.csdn.net/20180705103300738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




从上图中我们可以看到，启动的时候，主备选举是利用 zookeeper 来实现的， hdfs  namenode节点上的 ZKFailoverController 进程， 主要负责控制主备切换，监控 namenode进程是否还活着， 连接 zookeeper进行选举，并且控制本机的 namenode 的行为，如果发现zookeeper的锁节点还没有被抢占创建，就创建锁节点，并把本机的 namenode 变成 active 节点， 如果发现锁节点已经创建，就把 本机的namenode变成standby节点。

如果 ZKFailoverController 发现本机的 namenode 已经挂掉了， 就删除zookeeper上的节点，standby 上的ZKFailoverController感知到删除事件， 就升级本机的namenode成为主节点。

如果 active节点超过 Zookeeper Session Timeout 参数配置的时间没有连接 zookeeper， 同样被认为主节点已经挂了， standby 节点也会升级成为主节点

**hdfs 如何进行主备切换**




![](https://img-blog.csdn.net/20180705103309273?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2E3MjQ4ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







与单机文件系统相似，HDFS对文件系统的目录结构也是按照树状结构维护，Namespace保存了目录树及每个目录/文件节点的属性。除在内存常驻外，这部分数据会定期flush到持久化设备上，生成一个新的FsImage文件，方便NameNode发生重启时，从FsImage及时恢复整个Namespace。

上图我们可以看到基于 journalnode 进行共享元数据，  jounalnode中保存着 editlog，也就是对文件元数据的操作日志，比如用户（hdfs dfs rm /a.txt）删除一个文件， 就产生了一条操作日志，作用到命名空间后，a.txt 这个文件就被干掉了，这个日志就类似于数据库中的 redo 日志， standby节点 周期性的从journalnode中拉取 editlog， 然后从上一次checkpoint的位置开始重放，在内存中将操作结果合并到老的fsimage（命名空间），  并更新checkpoint位置。 最后把合并后的fsimage 上传到 active namenode 保存替换老的fsimage文件，

NameNode 处于 active和standby状态时候，扮演的角色是很不同的，
- active 角色， 把用户对命名空间的操作作为 editlog 写入 journalnode 集群
- standby 角色，EditLogTailer 线程 从 journalnode 上拉取 editlog日志， StandbyCheckpointer 线程把 editlog合并到 fsimage， 然后把 fsimage 文件上传到 active namenode 节点

active 挂的时候，  standby 感知后， 转换为 active 角色， 生成一个新的 epoch（就是每次主备切换就加一， 表明现在谁是老大，在谁的任期内），因为 standby 是阶段性拉取 editlog的，所以肯定最新的一段editlog还没有作用于 fsimage， 需要拉取重放， 因为写 editlog 到 journal的时候， 只要大多数（比如总5个点，要成功写入3个点，总共3个点，要成功写入2个点）写入成功就认为成功了， 这样在拉取恢复的时候，各个journalnode 节点上的数据有可能不一致， 这时候就必须找到大多数里面 事务id最大的点作为基准， 这个事务id就类似于数据库中的 Log sequence number（简称LSN）， 简单来讲， 就是要把 上一次checkpoint 的（LSN） 到 redo 日志里面最大的 （LSN）这之间的 日志进行重放redo，合并到fsimage。

假设一种情况，  老的active节点，是因为网络问题，连接不上 zookeeper， 然后被认为已经死了，其实是假死，这时候网络恢复了， 还认为自己是 leader， 往 journalnode里面写editlog日志， 发现现在已经是别人的天下了（epoch number已经加1了），自己的epoch number已经过时了，就把自己降级为 standby，做standby 角色该干的事情。

为什么要在 secondnamnode 上合并， 主要是为了减轻 namenode的压力， namenode有很多其他的事情要干呢。

**操作**
- hadoop-daemon.sh start journalnode， 启动  journalnode 集群，会根据你配置文件里面配置的机器上去启动 journalnode 集群
- hdfs namenode -format，  格式化 namenode 命名空间
- hadoop-daemon.sh start namenode，nn1 上启动 namenode
- hdfs namenode -bootstrapStandby，  nn2 同步nn1的元数据信息， 会把主节点上的 fsimage editlog目录拷贝过来
- hadoop-daemon.sh start namenode，  nn2 上启动 namenode
- 启动 zk 集群  hdfs zkfc -formatZK 格式化 zkfc zk 中的目录  hadoop-daemon.sh start zkfc   各个namenode节点上启动dfszk failover controller, 先在哪台机器上启动，哪台就是active namenode
- hadoop-daemon.sh start datanode 上传文件 测试 读写文件
- 测试  ha 将 active namenode进程kill，    jps  进程号， kill，    将active namenode机器断开网络  service network stop

我们对集群进行以上的测试， 观察日志，会发现hdfs干的事情基本上就是前面描写的流程。




