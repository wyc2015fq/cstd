# 会了也要看的flink安装部署|适合阅读 - Spark高级玩法 - CSDN博客
2019年02月16日 21:18:09[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：67
Flink支持运行与所有的类linux环境，比如linux，mac os x 和cygwin(windows)，要求一个master节点，一个或者多个worker节点。再部署启动flink集群之前，要准备一下环境，对每个节点的环境要求是：
- 
需要java 1.8.x或者更高版本
- 
ssh(sshd一定要启动，因为要用flink脚本去管理集群远程的节点)
如果，你集群环境不满足这些软件要求，请及时安装和更新。
假如ssh是免密码登陆的，然后保证每个集群的安装路径都一样，这样可以方便的使用flink脚本管理集群。
JAVA_HOME配置是flink集群必须要求的，也可以通过conf/flink-conf.yaml里的env.java.home属性设置。
**Flink集群配置**
在下载flink
https://flink.apache.org/downloads.html
然后解压
tar -zxfflink-1.7.1-bin-hadoop27-scala_2.11.tgz
配置的要点分析
**master选定**
在解压之后，就可以开始编辑flink的conf/flink-conf.yaml进行配置了。
必要重要的点是选定master节点，配置属性叫做：jobmanager.rpc.address。
**内存配置**
根据自己的实际情况集群大小，业务繁忙度设置合理的jobmananger的jvm内存，属性是jobmanager.heap.mb 和taskmanager.heap.mb。
单位是MB。当然，有些集群可能是节点直接内存可能有大有小，这样统一flink的taskmanager配置，会导致一些节点物理内存荒废，所以建议是可以通过FLINK_TM_HEAP这个环境变量来调整，该环境变量会覆盖掉配置文件内部的taskmanager.heap.mb配置。
**指定worker节点**
集群的哪些节点被配置为worker节点，也是需要明确指定的，这个很类似hdfs的配置，需要编辑conf/slaves文件，然后将所有wroker节点的ip或者host写入该文件即可。然后就可以ssh到所有机器的节点上启动集群。
当然，有些人要抬杠了，我就不配置slaves，我要一台一台启动taskmanager，只能说完全没问题。
单节点无需配置，解压之后可以直接启动的。
**配置案例**
官网给出了一个三节点的集群（10.0.0.1到10.0.03）
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVXsmNJSFk5Uko2PcUNNrLnvSYZDiciaw2zZpWibOPMPcUTUKuicmVuQuuBMF72MkxFjTV6bak5hdCwuw/640?wx_fmt=png)
再次强调，在每个节点flink的home路径要存在且统一，最简单的方式是用NFS，当然也可以scp的方式分发到每个节点。
重要配置解析
- 
jobmanager.heap.mb:jobmanager的堆内存。
- 
taskmanager.heap.mb:taskmanager的堆内存。
- 
taskmanager.numberOfTaskSlots:每个机器slots的数量，官方说是cpu的数量一个意思一般slot数就可以设置为cpu数或者cpu数的整数倍。
- 
parallelism.default :任务启动的时候的默认并行度。
- 
taskmanager.tmp.dirs : taskmanager的运行临时路径，可以配置多路径，有ssd更好。
**启动flink集群**
bin/start-cluster.sh 脚本在是启动整个flink集群的脚本，执行的时候会现在本地启动一个jobmanager，然后使用ssh连接到slaves文件配置的所有worker节点启动taskmanager。Taskmanager通过前面配置的jobmanager的rpc端口链接到jobmanager，这样整个集群就启动了。
停止集群采用的脚本是bin/stop-cluster.sh。
上面的启动方式主要是从零启动一个正常的集群，那么很多时候我们会给集群添加机器，这个时候怎么办呢？
Flink提供了两个脚本
增加一个JobManager
bin/jobmanager.sh ((start|start-foreground)[host] [webui-port])|stop|stop-all
增加一个TaskManager
bin/taskmanager.shstart|start-foreground|stop|stop-all
执行命令所在的机器就是你要添加到集群的机器，切记啊。
但是，standalone集群用的不多，还是后面要重点讲的on yarn用的多～
更多flink教程环境加入浪尖知识星球～
推荐阅读：
[浪尖原创flink视频第一弹](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486413&idx=1&sn=627b72d1e65869b9b30ba1ed0c21b780&chksm=9f38e8e5a84f61f36e809c2517192967f2f9c02c62b7d0d3607c59bdefc36ade612ac08df8ae&scene=21#wechat_redirect)
[新年大数据第一指导～内含资料](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486401&idx=1&sn=d1dccb93368bdaf731a2d629165de3f8&chksm=9f38e8e9a84f61ff0cf5e2d5d1ef7b59abbef6c44bbd9cc64f570d9fb58a9ea8cecbcf057343&scene=21#wechat_redirect)
[案例讲解kafka的多分区watermark机制](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486388&idx=1&sn=d8f2ec694f8316637fc217b81e3c8334&chksm=9f38e89ca84f618a09064c583245c6b42476cd22e1ec6e10a2058ea1fa5c2f28c629241e2314&scene=21#wechat_redirect)
2019年与540+一起学习
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
