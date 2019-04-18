# Zookeeper的客户脚本及命令（五） - z69183787的专栏 - CSDN博客
2018年02月24日 21:25:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：175
[http://blog.csdn.net/wo541075754/article/details/60569892](http://blog.csdn.net/wo541075754/article/details/60569892)
Zookeeper部署完成之后，本篇博客介绍一下Zookeeper的客户端脚本以及相关的操作命令。
## 启动Zookeeper
进入zookeeper的bin目录下，执行以下命令，完成启动：
```
[root@localhost bin]# sh zkServer.sh start
ZooKeeper JMX enabled by default
Using config: /usr/zookeeper/zookeeper-3.4.9/bin/../conf/zoo.cfg
Starting zookeeper ... STARTED
[root@localhost bin]#
```
- 1
- 2
- 3
- 4
- 5
## zkCli脚本
zkCli.sh 为连接zookeeper服务器的客户端脚本：
`[root@localhost bin]# sh zkCli.sh`- 1
执行以上命令后会打印出很多环境变量和启动的连接的信息，当看到最后打印出以下信息，则表明连接成功：
```
WATCHER::
WatchedEvent state:SyncConnected type:None path:null
[zk: localhost:2181(CONNECTED) 0]
```
- 1
- 2
- 3
- 4
需要注意的是上面并没有指定zookeeper服务器的地址，默认连接本地的服务器。 
如果希望指定地址可使用：
`[root@localhost bin]# sh zkCli.sh -server ip:port`- 1
## 创建操作
上面通过zkCli连接上服务器之后就可以执行创建节点（数据节点）命令。 
语法如下：
```bash
create [-s] [-e] path data acl
```
- 1
其中，-s或-e分别指定节点为顺序或临时节点，默认情况下创建的为持久节点。 
执行如下命令：
`[zk: localhost:2181(CONNECTED) 0] create /zk-create-demo 'hello world'`- 1
通过上面的命令创建了一个名字叫zk-create-demo的节点，其中节点的内容为‘hello world’。其中acl命令是用来做权限控制的，此例中没有传递此参数，默认不作任何权限控制。
## 读取操作
上面我们已经创建了一个节点，并在节点中写入了内容。下面，我们需要通过命令查看一下创建的节点及内容。
### ls命令
`ls path [watch]`- 1
ls命令，查询指定路径的下一级所有字节点。
```
[zk: localhost:2181(CONNECTED) 2] ls /
[zk-create-demo, zookeeper]
```
- 1
- 2
通过上面执行命令的结果可以看出，在跟路径下有两个字节点。
### get命令
`get path [watch]`- 1
获取指定节点的数据内容和属性信息。
```
[zk: localhost:2181(CONNECTED) 8] get /zk-create-demo
hello world
cZxid = 0x2
ctime = Sun Mar 05 16:29:53 PST 2017
mZxid = 0x2
mtime = Sun Mar 05 16:29:53 PST 2017
pZxid = 0x2
cversion = 0
dataVersion = 0
aclVersion = 0
ephemeralOwner = 0x0
dataLength = 11
numChildren = 0
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
通过打印的信息，已经可以看到创建节点时写入的‘hello world’了。 
另外还打印出了创建该节点的事物ID（cZxid）、最后一次更新该节点的事物ID（mZxid）和最后一次更新该节点的时间（mtime）等信息。
## 更新操作
`set path data [version]`- 1
通过以上set命令可以更新指定节点。其中data为更新内容，version是指定当然更新是基于Znode的哪个版本进行更新的。
```
[zk: localhost:2181(CONNECTED) 9] set /zk-create-demo 'Hello World!' 0
cZxid = 0x2
ctime = Sun Mar 05 16:29:53 PST 2017
mZxid = 0x3
mtime = Sun Mar 05 16:46:08 PST 2017
pZxid = 0x2
cversion = 0
dataVersion = 1
aclVersion = 0
ephemeralOwner = 0x0
dataLength = 12
numChildren = 0
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
上面执行了一条更新命令，基于版本0，更新zk-create-demo内容为首字母大写的“Hello World！”。更新执行dataVersion版本和mtime都发生的变化。 
使用get命令查看会发现节点的内容也发生了变化。
## 删除操作
执行删除命令为：
`delete path [version]`- 1
此处的version与set中的vision作用一样。
执行以下命令删除节点：
`[zk: localhost:2181(CONNECTED) 11] delete /zk-create-demo`- 1
需要注意的是，如果此节点下面还有子节点则需先删除其子节点，否则会提示Node not empty，删除失败。
