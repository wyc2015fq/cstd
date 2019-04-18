# linux CentOS 7下zookeeper集群环境搭建 - z69183787的专栏 - CSDN博客
2018年02月25日 11:16:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：141
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
[http://blog.csdn.net/m290345792/article/details/52576890](http://blog.csdn.net/m290345792/article/details/52576890)
> 
# zookeeper说明
ZooKeeper是一个分布式的，开放源码的分布式应用程序协调服务，是Google的Chubby一个开源的实现，是Hadoop和Hbase的重要组件。它是一个为分布式应用提供一致性服务的软件，提供的功能包括：配置维护、域名服务、分布式同步、组服务等。更多详细的资料请自行http://zookeeper.apache.org/。
Zookeeper的安装和配置十分简单, 既可以配置成单机模式, 也可以配置成集群模式。本文只是简单介绍了下集群下的配置步骤（单机模式烦请自行百度）
> 
# zookeeper集群搭建前提
> 
1、请确认至少有两台+主机（如果硬件不够的话，你也可以试用虚拟机）；
2、能够 常用Linux命令。
> 
# 第一步：zookeeper下载
下载地址：http://zookeeper.apache.org/releases.html（个人建议选择清华大学的镜像站点进行下载，至于原因相信你懂的）。
清华镜像下载地址：http://mirrors.tuna.tsinghua.edu.cn/apache/zookeeper/ 。我选择的版本是zookeeper-3.4.9。
如果版本不一致可能存在差异。
# 第二步：zookeeper解压
> 下载下来的文件包为压缩包zookeeper-3.4.9.tar.gz。请使用tar -xzvf zookeeper-3.4.9.tar.gz
![](https://img-blog.csdn.net/20160918193027849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
# 第三步：zookeeper环境变量配置
> 命令：Vi /usr/profile
> 增加zookeeper的环境变量ZOOKEEPER_HOME的本地路径，并且增加到PATH中。
> ![](https://img-blog.csdn.net/20160918193048544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
# 第四步：zookeeper主机域名映射，配置hosts文件。
> 命令：vi /etc/hosts。
![](https://img-blog.csdn.net/20160918193101443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 第五步：zookeeper配置文件配置
# 进入zookeeper的conf子文件夹。/conf/zoo_sample.cfg文件复制一份，并更名为zoo.cfg。增加server节点。
> server.1=Slave1: 2888:3888 
server.2=Slave2: 2888:3888 
server.A=B：C：D  其中 A 是一个数字，表示这个是第几号服务器；B 是这个服务器的 ip 地址（如果做了域名映射为域名）；C 表示的是这个服务器与集群中的 Leader 服务器交换信息的端口；D 表示的是万一集群中的 Leader 服务器挂了，需要一个端口来重新进行选举，选出一个新的
 Leader，而这个端口就是用来执行选举时服务器相互通信的端口。
> ![](https://img-blog.csdn.net/20160918193113748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
# 第六步：zookeeper识别文件创建。创建myid文件
在dataDir目录下创建一个myid文件，然后分别在myid文件中按照zoo.cfg文件的server.A中A的数值，在不同机器上的该文件中填写相应的值。例如server.1 在当前机器的myid中就只写上1就可以了。
> 
# 第七步：Zookeeper配置拷贝到另外一台机器
在另外一台机器上试用scp命令远程拷贝当前Zookeeper文件夹。然后配置对应的环境变量和hosts映射。注意修改myid文件的值为2。
# 第八步：Zookeeper启动验证
进入bin目录执行命令“zkServer.sh start”启动Zookeeper。不同机器上的Zookeeper需要单独启动。执行命令“zkServer.sh stop”将会停止Zookeeper。
> 
![](https://img-blog.csdn.net/20160918193155857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
# 第九步：zookeeper当前状态查看
> 进入bin目录执行命令“zkServer.sh status”。
> 
zookeeper领导者（leader）：
![](https://img-blog.csdn.net/20160918193207139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
zookeeper跟随者(follower)：
> 
![](https://img-blog.csdn.net/20160918193230274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
# 第十步：zookeeper动态日志查看
> 进入bin目录，使用tail –f  zookeeper.out 查看当前输入日志
> ![](https://img-blog.csdn.net/20160918193240779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
> 
> 
到此，完成Zookeeper集群配置工作。
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
> 
# 后话                                                                                                                           
         如果出现Error contacting service. It is probably not running的错误的时候，一般情况下问题存在于防火墙开启的原因，关闭防火墙就可以了。CentOS 7默认使用的是firewall作为防火墙，使用iptables必须重新设置一下。具体设置可以查看该文章《[Centos7
 关闭防火墙](http://blog.csdn.net/m290345792/article/details/52576643)》
