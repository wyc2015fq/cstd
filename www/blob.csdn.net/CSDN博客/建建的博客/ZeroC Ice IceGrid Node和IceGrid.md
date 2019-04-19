# ZeroC Ice IceGrid Node和IceGrid - 建建的博客 - CSDN博客
2017年05月02日 17:48:21[纪建](https://me.csdn.net/u013898698)阅读数：379
个人分类：[ICE](https://blog.csdn.net/u013898698/article/category/6896358)
# IceGrid Node介绍
        绝大多数分布式系统都有一个共同特点，即分布在各个主机上的节点进程并不是完全独立的，而是彼此之间有相互联系和通信的。集群对集群中的节点有一些控制指令，如部署、启停或者调整某些参数等。IceGrid为了达到这个目标，也设计了一个特殊的节点进程-IceGrid Node，它的目标有以下几个：
        1. 负责IceBox的装载和启停；
        2. 采集主机的负载信息；
        3. 执行控制器IceGrid Admin的指令；
        简单的说，IceGrid Node就好像一个容器，启动以后，会安装预先定义的配置文件，加载其中的IceBox，并根据要求启动或停止其中的一个或多个IceBox进程。同时定期采集底层主机的负载信息（CPU Load），这些信息用于基于主机负载的负载均衡算法调度，它同时把自己的控制端口注册到Registry中，IceGrid Admin控制器就可以对所有的IceGrid
 Node集中进行远程管理，下达指令。
        Registry、IceGrid Node、IceBox之间的关系
![](https://img-blog.csdn.net/20161001090834094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        每个IceGrid Node都有一个唯一的名字，配置文件如下：node1.cfg
|1234567891011|`#指定主注册节点的位置``Ice.Default.Locator=IceGrid``/Locator``:tcp -h localhost -p 4061``#设置节点1相关数据的存储目录``IceGrid.Node.Data=D:\Java\workspace\workspace-temp\test_iceBox_registry_iceGridNode\node\data``#指定节点1用于监听客户端连接的端口号``IceGrid.Node.Endpoints=tcp -p 5062  ``#指定节点1的名称        ``IceGrid.Node.Name=node1 ``Ice.StdErr=D:\Java\workspace\workspace-temp\test_iceBox_registry_iceGridNode\node\node.stderr.log  ``#指定错误日志文件``Ice.StdOut=D:\Java\workspace\workspace-temp\test_iceBox_registry_iceGridNode\node\node.stdout.log`|
        注意：目录要先创建好 
        启动：icegridnode --Ice.Config=src/node1.cfg
# IceGrid介绍
        其实就是一个Registry进程+N个IceGrid Node进程。为了方便管理和运维，又增加下面几个程序包。
        1. icegridadmin：用来管理IceGrid的命令行工具，部署或升级IceGrid，查看Gnid节点状态，启停Node中的Server实例等。
        2. IceGridGUI : 是一个 java swing application，作用等同于 icegridadmin , 不过他有图形界面，适合在windows下远程管理IceGrid。
        3. icepatch：为一组命令行工具，分Server端与Client端，是替代rsyn或FTP的好工具，用来在网格中分发和同步IceGrid应用中用户开发的Ice服务的二进制代码，方便运维过程中的服务版本升级和扩容等工作。
        为了学习icegridadmin命令，我们分别启动Registry和Node：分别开三个命令名窗口
        icegridregistry --Ice.Config=src/registry.cfg
        icegridnode --Ice.Config=src/node1.cfg
        icegridadmin -u test -p test --Ice.Default.Locator="IceGrid/Locator:tcp -h localhost -p 4061"
        进入就可以进行各种操作了，这里对这块就不进行详细介绍了，有兴趣的同学可以自己去研究。
参考
         《ZeroC Ice 权威指南》3.4
资料
        源码：链接：http://pan.baidu.com/s/1bol23FL 密码：2jf3
