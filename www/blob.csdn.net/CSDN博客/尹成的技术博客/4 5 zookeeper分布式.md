
# 4.5 zookeeper分布式 - 尹成的技术博客 - CSDN博客

2018年11月15日 14:32:45[尹成](https://me.csdn.net/yincheng01)阅读数：26个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



一、	为什么需要zookeeper
	大部分分布式应用需要一个主控、协调器或控制器来管理物理分布的子进程（如资源、任务分配等）
	大部分应用需要开发私有的协调程序，缺乏一个通用的机制
	协调程序的反复编写浪费，且难以形成通用、伸缩性好的协调器
	zookeeper可以提供通用的分布式锁服务，用以协调分布式应用，它是一个开源的分布式应用协调服务
	分布式：多台计算机、它们之间通过网络进行通信、它们有共同的目标
	协调服务
二、	zookeeper简介
zookeeper是什么
	zookeeper是Google的Chubby一个开源的实现，是hadoop的分布式协调服务
	它包含一个简单的原语集，分布式应用程序可以基于它实现同步服务，配置维护和命名服务等
	将zookeeper翻译过来，是动物园管理员，可以想象，它是来管理动物的（大象、蜜蜂、小猪）
	它的架构如下图：
zookeeper能帮我们做什么
	分布式系统协调：多个节点一起完成一个动作
	集群成员管理
	分布式锁
	选主
	分布式事务
	发布/订阅（发布者将数据发布到zk的一个或一系列节点，订阅者进行数据订阅，当有数据变化时，可以及时得到数据的变化通知）
	负载均衡
3.	zookeeper的特性
	zookeeper是简单的
	zookeeper是富有表现力的
	zookeeper是高可用的
	zookeeper是松耦合的交互方式
	zookeeper是一个资源库
4.	zookeeper的数据模型
	层次化的目录结构，命名符合常规文件系统规范
	每个节点在zookeeper中叫做znode，并且其有一个唯一的路径标识
	znode中的数据可以有多个版本，比如某一个路径下存有多个数据版本，那么查询这个路径下的数据就需要带上版本
	节点不支持部分读写，而是一次性完整读写
	其中每个单独的长方形我们称之为一个节点（znode），节点（znode）的全名称是整个目录名，比如Seerver1节点的全节点名称是/NameService/Server1
5.	zookeeper的下载地址
[http://mirrors.cnnic.cn/apache/zookeeper/zookeeper-3.4.9/](http://mirrors.cnnic.cn/apache/zookeeper/zookeeper-3.4.9/)
三、	zookeeper的安装
手动安装（单机、伪分布式、完全分布式）
	解压，创建一个data目录，一个log目录
	在data目录创建myid文件，server1写1
	复制配置文件为zoo.cfg，编辑配置文件，需要修改的位置如下：
dataDir=/xxx/data
dataLogDir=/xxx/log
server.1=主机名:2888:3888
	配置环境变量
[zkServer.sh](http://zkServer.sh)start即可开启
cloudera manager自动化部署
四、	zookeeper基本概念
集群角色
	Leader客户端提供读和写服
	Follower提供读服，所有写服都需要转交给Leader角色，参与选举
	Observer提供读服，参选举过程，一般是了增强zk集群的读请求并发能力
会话
	zk的客户端与zk的服端之间的连接
	通过心跳检测保持客户端连接的存活
	接收来自服端的watch事件通知
	设置超时时间
zookeeper节点类型
	persistent：persistent节点不和特定的session绑定，不会随着创建该节点的session的结束而消失，而是一直存在，除非该节点被显式删除
	ephemeral：ephemeral节点是临时性的，如果创建该节点的session结束了，该节点就会被自动删除，ephemeral节点不能拥有子节点，虽然ephemeral节点与创建它的session绑定，但只要该该节点没有被删除，其他session就可以读写该节点中关联的数据，使用-e参数指定创建ephemeral节点
	sequence：严格的说，sequence并非节点类型中的一种，sequence节点既可以是ephemeral的，也可以是persistent的，创建sequence节点时，ZooKeeper server会在指定的节点名称后加上一个数字序列，该数字序列是递增的，因此可以多次创建相同的sequence节点，而得到不同的节点，使用-s参数指定创建sequence节点
版本
	Version：当前Znode的版本
	Cversion：当前Znode的子节点的版本
	Aversion：当前Znode的ACL（访问控制）版本
watcher
	作用于Znode节点
	watcher设置后，一旦触发一次就会失效，如果需要一直监听，就需要再次注册
	多种事件通知：数据更新，子节点状态等
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

