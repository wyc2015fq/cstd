# zookeeper集群配置及搭建 - 一个跳popping的quant的博客 - CSDN博客





2017年09月03日 20:12:17[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：726标签：[hadoop																[zookeeper																[集群](https://so.csdn.net/so/search/s.do?q=集群&t=blog)
个人分类：[hadoop](https://blog.csdn.net/FrankieHello/article/category/7113390)





先看一下zookeeper是什么：ZooKeeper是一个分布式的，开放源码的分布式应用程序协调服务，是Google的Chubby一个开源的实现，是Hadoop和Hbase的重要组件。它是一个为分布式应用提供一致性服务的软件，提供的功能包括：**配置维护、域名服务、分布式同步、组服务**等。其中最常用到的就是利用zookeeper进行数据同步了。 

   先看一下zk的几个功能： 

   借鉴于：[http://blog.csdn.net/xuxiuning/article/details/51218941](http://blog.csdn.net/xuxiuning/article/details/51218941)

      1、配置维护 

    在分布式系统中，有好多机器，如果在一台主机中修改完配置文件的时候，要想让其他的主机也同样有这些同样的配置文件，那么就可以通过ssh中的scp来把这些文件拷贝到其他的节点上，而有了zk则可以通过它提供的这个公共管理的地方来修改，从而实现了全局的同步，就可以省去了手动的拷贝，并且保证了它的可靠和一致性。 

    2、 域名服务 

这个可以简单理解为一个电话薄，电话号码不好记，但是人名好记，要打谁的电话，直接查人名就好了。 

分布式环境下，经常需要对应用/服务进行统一命名，便于识别不同服务；类似于域名与ip之间对应关系，域名容易记住；通过名称来获取资源或服务的地址，提供者等信息。 

  3、 分布式锁 

碰到分布二字貌似就难理解了，其实很简单。单机程序的各个进程需要对互斥资源进行访问时需要加锁，那分布式程序分布在各个主机上的进程对互斥资源进行访问时也需要加锁。很多分布式系统有多个可服务的窗口，但是在某个时刻只让一个服务去干活，当这台服务出问题的时候锁释放，立即fail over到另外的服务。这在很多分布式系统中都是这么做，这种设计有一个更好听的名字叫**Leader Election(leader选举)**。举个通俗点的例子，比如银行取钱，有多个窗口，但是呢对你来说，只能有一个窗口对你服务，如果正在对你服务的窗口的柜员突然有急事走了，那咋办？找大堂经理（zookeeper）!大堂经理指定另外的一个窗口继续为你服务！ 

   4、 集群管理 

在分布式的集群中，经常会由于各种原因，比如硬件故障，软件故障，网络问题，有些节点会进进出出。有新的节点加入进来，也有老的节点退出集群。这个时候，集群中有些机器（比如Master节点）需要感知到这种变化，然后根据这种变化做出对应的决策。我已经知道HDFS中namenode是通过datanode的心跳机制来实现上述感知的，那么我们可以先假设Zookeeper其实也是实现了类似心跳机制的功能吧！ 

   zookeeper的部署配置（以三台虚拟机配置为例） 
**1、zookeeper的安装**

   将zookeeper的压缩包解压到/usr/local/zookeeper-3.4.6目录下。配置环境变量ZOOKEEPER_HOME=/usr/local/zookeeper-3.4.6 

PATH=ZOOKEEPER_HOME/bin:$PATH 
**2、zoo.cfg配置**

   zk的文件中不存在zoo.cfg的配置文件，可以把zookeeper-3.4.6/conf/zoo_sample.cfg改为zoo.cfg。 

   在zoo.cfg中要修改dataDir的值，也就是zk存放数据的目录，例如：/usr/local/zookeeper-3.4.6/data，并在该目录下创建data文件夹。 

   在配置文件最后，追加上 

server.1=hadoop1:2888:3888 

server.2=hadoop2:2888:3888 

server.3=hadoop3:2888:3888 

（hadoop1为每台主机的hostname，如果没有配置hostname可以用ip地址） 

（server.id=host:port:port这两个端口号分别代表了该server和集群中的leader交换消息所使用的端口和选举leader时所使用的端口。） 
**3、配置每台主机的myid**

  在每台主机的dataDir目录中，也就是data目录中创建一个myid文件，并输入当前主机的id。例如hadoop1中为1、hadoop2中为2。 
**4、启动和停止**

   在每台主机上，使用zkServer.sh start来启动，zkServer.sh  stop来停止。通过jps查看，可以看到进程中多了一个QuorumPeerMain。三台主机的zk启动后，通过zkServer.sh status可以查看每台zk的状态，会发现三台中有一台的Model是Leader，其余两台的是Follower。另外，在一台中通过zkClient.sh进入客户端，在zk的目录中通过create /zookeepertest 123创建这样一个文件，再在其余几台中启动客户端，就可以发现其余两台中，也有了这个文件，这样就实现了数据同步。](https://so.csdn.net/so/search/s.do?q=zookeeper&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)




