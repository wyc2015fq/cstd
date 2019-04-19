# docker探索-swarm搭建docker集群 - 零度的博客专栏 - CSDN博客
2018年08月15日 19:27:22[零度anngle](https://me.csdn.net/zmx729618)阅读数：107
原文出处：[http://www.cnblogs.com/520playboy/p/7873903.html](http://www.cnblogs.com/520playboy/p/7873903.html)
**前言**
Swarm 在 Docker 1.12 版本之前属于一个独立的项目，在 Docker 1.12 版本发布之后，该项目合并到了 Docker 中，成为 Docker 的一个子命令，docker swarm是创建服务器集群的工具，只需要几条命令就可以创建一个服务器集群。它内置一些服务器集群需要的工具，比如说：服务查找，网络，负载均衡等等
### 一、环境
     centos 7.3
    Docker version 1.12.6
|ip|角色|
|----|----|
|192.168.6.130|manager|
|192.168.6.131|worker|
|192.168.6.132|worker|
### 二、集群
**2.1、在192.168.6.130中，初始化集群**
[root@jacky jacky]# docker swarm init --advertise-addr 192.168.6.130:2377
Swarm initialized: current node (4devzwb6rpimfpteqr17h2jx9) is now a manager.
To add a worker to this swarm, run the following command:
    docker swarm join \
    --token SWMTKN-1-5r3ujri3th4038jp7q66zrfo56eqo0sabyage8ahc10121evog-ah9bptj9d7txdu6y91w7qxd81 \
    192.168.6.130:2377
To add a manager to this swarm, run 'docker swarm join-token manager' and follow the instructions.
[root@jacky jacky]# 
说明：--advertise-addr 设置监听Ip和端口号 
**2.2、初始化完、查看集群上的节点列表**
[root@jacky jacky]# docker node ls
ID                           HOSTNAME           STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  jacky.localdomain  Ready   Active        Leader
说明：目前只有一个节点，状态为Ready，是个Leader
**2.3、192.168.6.131，加入集群**
首先，切换到192.168.6.131，然后输入
[root@jacky jacky]#  docker swarm join \
> --token SWMTKN-1-5r3ujri3th4038jp7q66zrfo56eqo0sabyage8ahc10121evog-ah9bptj9d7txdu6y91w7qxd81 \
> 192.168.6.130:2377
This node joined a swarm as a worker.
[root@jacky jacky]# 
**2.4、192.168.6.132，加入集群**
首先，切换到192.168.6.132，然后输入
[root@jacky jacky]# docker swarm join \
> --token SWMTKN-1-5r3ujri3th4038jp7q66zrfo56eqo0sabyage8ahc10121evog-ah9bptj9d7txdu6y91w7qxd81 \
> 192.168.6.130:2377
This node joined a swarm as a worker.
[root@jacky jacky]# 
**2.5、接着回到192.168.6.130中，查看集群中节点信息**
[root@jacky jacky]# docker node ls
ID                           HOSTNAME           STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  jacky.localdomain  Ready   Active        Leader
5mjtda2uzzu43v2xuxdco5ogr    jacky.localdomain  Ready   Active        
a8wdtux82dolsbgmv6ff0uu94    jacky.localdomain  Ready   Active
说明：
AVAILABILITY列：
显示调度程序是否可以将任务分配给节点：
- Active 意味着调度程序可以将任务分配给节点。
- Pause 意味着调度程序不会将新任务分配给节点，但现有任务仍在运行。
- Drain 意味着调度程序不会向节点分配新任务。调度程序关闭所有现有任务并在可用节点上调度它们。
MANAGER STATUS列
显示节点是属于manager或者worker
- - 没有值 表示不参与群管理的工作节点。
- Leader 意味着该节点是使得群的所有群管理和编排决策的主要管理器节点。
- Reachable 意味着节点是管理者节点正在参与Raft共识。如果领导节点不可用，则该节点有资格被选为新领导者。
- Unavailable 意味着节点是不能与其他管理器通信的管理器。如果管理器节点不可用，您应该将新的管理器节点加入群集，或者将工作器节点升级为管理器。
**2.6、创建集群网络overlay**
[root@jacky jacky]# docker network create --driver overlay skynet
843z9qb9c6douf7ir7l3iimqn
[root@jacky jacky]# docker network ls
NETWORK ID          NAME                DRIVER              SCOPE
ad5125729239        bridge              bridge              local               
5a15f008fb38        host                host                local               
6echvokyh2m3        ingress             overlay             swarm               
28068704e605        none                null                local               
843z9qb9c6do        skynet              overlay             swarm
可以看到新创建的网络名称为skynet，类型为overlay
### 三、部署测试
**3.1、在管理节点中执行**
docker service create -p 80:80 --name webserver --replicas 5 httpd
**3.2、查看集群中的服务**
[root@node1 jacky]# docker service ls
ID            NAME       REPLICAS  IMAGE                 COMMAND
0blhke4vywh8  viz        0/1       manomarks/visualizer  
7batkp4zv9f3  portainer  1/1       portainer/portainer   -H unix:///var/run/docker.sock
7kw3ovihgqgb  webserver  5/5       httpd    
**3.3、查看集群中的webserver服务**
[root@node1 jacky]# docker service ps webserver
ID                         NAME             IMAGE  NODE             DESIRED STATE  CURRENT STATE          ERROR
e0jqrg479muha7ow8bf34rv39  webserver.1      httpd  node1.jacky.com  Running        Running 2 hours ago    
23n9df2vww079h5rgkxlri4uy   \_ webserver.1  httpd  node1.jacky.com  Shutdown       Complete 2 hours ago   
8b8cs13u9yjsoru3ybyzzv9e6   \_ webserver.1  httpd  node2.jacky.com  Shutdown       Rejected 20 hours ago  "No such image: httpd:latest"
8lvx0ynaohlcfyp11jgji4m3q  webserver.2      httpd  node1.jacky.com  Running        Running 2 hours ago    
0q8lrzlybo1exl3bngwwfy386   \_ webserver.2  httpd  node1.jacky.com  Shutdown       Complete 2 hours ago   
eoq4a2sqx80a0hly6tizt5ucf   \_ webserver.2  httpd  node3.jacky.com  Shutdown       Shutdown 4 hours ago   
10juv2jp1ay9rjbu5hgw6yhs3   \_ webserver.2  httpd  node3.jacky.com  Shutdown       Failed 20 hours ago    "starting container failed: er…"
7xa8uoa8775i5nl0zzi373xbt   \_ webserver.2  httpd  node3.jacky.com  Shutdown       Failed 20 hours ago    "starting container failed: er…"
6puw8t22w0exgiwqzt5vi8fc1  webserver.3      httpd  node1.jacky.com  Running        Running 2 hours ago    
74osfdgl5ovp1c3e5s012b3f6   \_ webserver.3  httpd  node1.jacky.com  Shutdown       Complete 2 hours ago   
cwzuewjsolewap28ctvy3jxaa   \_ webserver.3  httpd  node3.jacky.com  Shutdown       Shutdown 4 hours ago   
9bb5q38zqk153uqdex1yfcu4e   \_ webserver.3  httpd  node3.jacky.com  Shutdown       Failed 20 hours ago    "starting container failed: er…"
1uvoczhsfz5ncp1emdueljwqa   \_ webserver.3  httpd  node3.jacky.com  Shutdown       Failed 20 hours ago    "starting container failed: er…"
0j9hsq18v3pzoecmrjg0qtynh  webserver.4      httpd  node1.jacky.com  Running        Running 2 hours ago    
dyhyy55xlkm3abw2cqm0k8y6h   \_ webserver.4  httpd  node1.jacky.com  Shutdown       Complete 2 hours ago   
8dymtjskyxhw5zj0ombpv0pm1   \_ webserver.4  httpd  node2.jacky.com  Shutdown       Shutdown 4 hours ago   
1b6u7rtknpgmwyfn3j3p94wm6   \_ webserver.4  httpd  node2.jacky.com  Shutdown       Rejected 20 hours ago  "No such image: httpd:latest"
1af72d5vpu1xg3u0qypnvlier   \_ webserver.4  httpd  node2.jacky.com  Shutdown       Rejected 20 hours ago  "No such image: httpd:latest"
897au9dm88i94l0scg69slu6z  webserver.5      httpd  node1.jacky.com  Running        Running 2 hours ago    
eqt7g4bk6e2kqy6qbnr13gfeh   \_ webserver.5  httpd  node1.jacky.com  Shutdown       Complete 2 hours ago   
7vq8u2eycraafwzlmcgg1e80d   \_ webserver.5  httpd  node3.jacky.com  Shutdown       Shutdown 4 hours ago   
ehu6f1xun8ha6lw7cyex7jjrw   \_ webserver.5  httpd  node2.jacky.com  Shutdown       Shutdown 4 hours ago 
**3.4、访问http://192.168.6.130、http://192.168.6.131或http://192.168.6.132**
**![](https://images2017.cnblogs.com/blog/917107/201711/917107-20171122140844790-267367228.png)![](https://images2017.cnblogs.com/blog/917107/201711/917107-20171122140911805-843226342.png)![](https://images2017.cnblogs.com/blog/917107/201711/917107-20171122140933196-26217861.png)**
 说明：192.168.6.131和192.168.6.132没有部署httpd也能狗访问，swarm搭建docker集群测试成功
### 四、安装Portainer，图形化管理docker
**4.1、关闭selinux**
setenforce 0
**4.2、在manager节点中执行，如下：**
docker service create \
--name portainer \
--publish 9000:9000 \
--constraint 'node.role == manager' \
--mount type=bind,src=/var/run/docker.sock,dst=/var/run/docker.sock \
portainer/portainer \
-H unix:///var/run/docker.sock
**4.3、访问192.168.6.130.9000**
![](https://images2017.cnblogs.com/blog/917107/201711/917107-20171122142318102-678942083.png)
### 五、swarm集群其他操作
**5.1、更新节点的可见性状态**
[root@node1 jacky]# docker node ls
ID                           HOSTNAME         STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  node1.jacky.com  Ready   Active        Leader
5mjtda2uzzu43v2xuxdco5ogr    node3.jacky.com  Ready   Active        
a8wdtux82dolsbgmv6ff0uu94    node2.jacky.com  Ready   Active        
[root@node1 jacky]# docker node update --availability Drain node2.jacky.com
node2.jacky.com
[root@node1 jacky]# docker node ls
ID                           HOSTNAME         STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  node1.jacky.com  Ready   Active        Leader
5mjtda2uzzu43v2xuxdco5ogr    node3.jacky.com  Ready   Active        
a8wdtux82dolsbgmv6ff0uu94    node2.jacky.com  Ready   Drain         
[root@node1 jacky]# docker node update --availability Active node2.jacky.com
node2.jacky.com
[root@node1 jacky]# docker node ls
ID                           HOSTNAME         STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  node1.jacky.com  Ready   Active        Leader
5mjtda2uzzu43v2xuxdco5ogr    node3.jacky.com  Ready   Active        
a8wdtux82dolsbgmv6ff0uu94    node2.jacky.com  Ready   Active        
[root@node1 jacky]# 
**5.2、升级或降级节点**
您可以将工作程序节点提升为manager角色。这在管理器节点不可用或者您希望使管理器脱机以进行维护时很有用。 类似地，您可以将管理器节点降级为worker角色。
无论您升级或降级节点，您应该始终在群中维护奇数个管理器节点。
- 升级节点
[root@node1 jacky]# docker node promote node3.jacky.com node2.jacky.com
[root@node1 jacky]# docker node ls
ID                           HOSTNAME         STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  node1.jacky.com  Ready   Active        Leader
5mjtda2uzzu43v2xuxdco5ogr    node3.jacky.com  Ready   Active        Unreachable
a8wdtux82dolsbgmv6ff0uu94    node2.jacky.com  Ready   Active        Reachable- 降级节点
[root@node1 jacky]# docker node demote node3.jacky.com node2.jacky.com
Manager node3.jacky.com demoted in the swarm.
Manager node2.jacky.com demoted in the swarm.
[root@node1 jacky]# docker node ls
ID                           HOSTNAME         STATUS  AVAILABILITY  MANAGER STATUS
4devzwb6rpimfpteqr17h2jx9 *  node1.jacky.com  Ready   Active        Leader
5mjtda2uzzu43v2xuxdco5ogr    node3.jacky.com  Ready   Active        
a8wdtux82dolsbgmv6ff0uu94    node2.jacky.com  Ready   Active  
**5.3、退出docker swarm集群**
在对应的worker节点执行如下命令
[root@node1 jacky]# docker swarm leave
如果想把manager节点上已经退出集群的节点信息删除，如下：
[root@node1 jacky]# docker node rm node2.jacky.com
