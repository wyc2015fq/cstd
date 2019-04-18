# Zookeeper Watch机制 - z69183787的专栏 - CSDN博客
2016年11月03日 17:27:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7771
Znode发生变化（Znode本身的增加，删除，修改，以及子Znode的变化）可以通过Watch机制通知到客户端。那么要实现Watch，就必须实现org.apache.zookeeper.Watcher接口，并且将实现类的对象传入到可以Watch的方法中。Zookeeper中所有读操作（getData()，getChildren()，exists()）都可以设置Watch选项。Watch事件具有one-time trigger（一次性触发）的特性，如果Watch监视的Znode有变化，那么就会通知设置该Watch的客户端。
### Zookeeper Watch
在上述说道的所有读操作中，如果需要Watcher，我们可以自定义Watcher，如果是Boolean型变量，当为true时，则使用系统默认的Watcher，系统默认的Watcher是在Zookeeper的构造函数中定义的Watcher。参数中Watcher为空或者false，表示不启用Wather。
一，一次性触发器
客户端在Znode设置了Watch时，如果Znode内容发生改变，那么客户端就会获得Watch事件。例如：客户端设置getData("/znode1", true)后，如果/znode1发生改变或者删除，那么客户端就会得到一个/znode1的Watch事件，但是/znode1再次发生变化，那客户端是无法收到Watch事件的，除非客户端设置了新的Watch。
二，发送至客户端
Watch事件是异步发送到Client。Zookeeper可以保证客户端发送过去的更新顺序是有序的。例如：某个Znode没有设置watcher，那么客户端对这个Znode设置Watcher发送到集群之前，该客户端是感知不到该Znode任何的改变情况的。换个角度来解释：由于Watch有一次性触发的特点，所以在服务器端没有Watcher的情况下，Znode的任何变更就不会通知到客户端。不过，即使某个Znode设置了Watcher，且在Znode有变化的情况下通知到了客户端，但是在客户端接收到这个变化事件，但是还没有再次设置Watcher之前，如果其他客户端对该Znode做了修改，这种情况下，Znode第二次的变化客户端是无法收到通知的。这可能是由于网络延迟或者是其他因素导致，所以我们使用Zookeeper不能期望能够监控到节点每次的变化。Zookeeper只能保证最终的一致性，而无法保证强一致性。
三，设置watch的数据内容
Znode改变有很多种方式，例如：节点创建，节点删除，节点改变，子节点改变等等。Zookeeper维护了两个Watch列表，一个节点数据Watch列表，另一个是子节点Watch列表。getData()和exists()设置数据Watch，getChildren()设置子节点Watch。两者选其一，可以让我们根据不同的返回结果选择不同的Watch方式，getData()和exists()返回节点的内容，getChildren()返回子节点列表。因此，setData()触发内容Watch，create()触发当前节点的内容Watch或者是其父节点的子节点Watch。delete()同时触发父节点的子节点Watch和内容Watch，以及子节点的内容Watch。
### Zookeeper Watcher的运行机制
1，Watch是轻量级的，其实就是本地JVM的Callback，服务器端只是存了是否有设置了Watcher的布尔类型。（源码见：org.apache.zookeeper.server.FinalRequestProcessor）
2，在服务端，在FinalRequestProcessor处理对应的Znode操作时，会根据客户端传递的watcher变量，添加到对应的ZKDatabase（org.apache.zookeeper.server.ZKDatabase）中进行持久化存储，同时将自己NIOServerCnxn做为一个Watcher callback，监听服务端事件变化
3，Leader通过投票通过了某次Znode变化的请求后，然后通知对应的Follower，Follower根据自己内存中的zkDataBase信息，发送notification信息给zookeeper客户端。
4，Zookeeper客户端接收到notification信息后，找到对应变化path的watcher列表，挨个进行触发回调。
流程图
![](https://img-blog.csdn.net/20151130144136090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### Zookeeper事件
Zookeeper状态/事件对应关系图：
![](https://img-blog.csdn.net/20151130152159470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ZooKeeper中Watch事件(见：org.apache.zookeeper.Watcher.EventType)：
None                在客户端与Zookeeper集群中的服务器断开连接的时候，客户端会收到这个事件。
NodeCreated                 Znode创建事件
NodeDeleted                 Znode删除事件
NodeDataChanged                 Znode数据内容更新事件。其实本质上该事件只关注dataVersion版本号，但是只要调用了更新接口dataVersion就会有变更。
NodeChildrenChanged                 Znode子节点改变事件，只关注子节点的个数变更，子节点内容有变更是不会通知的。
在事件发生时，ZooKeeper的状态(见：org.apache.zookeeper.Watcher.KeeperState)：
Disconnected(0)                  客户端处于断开连接的状态，并且没有和Zookeeper集群中任何服务器连接。
SyncConnected(3)                客户端处于连接的状态，也就是说客户端连接到了一台server
AuthFailed(4)               验证失败的状态
ConnectedReadOnly(5)              客户端连接到一个只读Server的状态。
SaslAuthenticated(6)             用于通知客户端他们是SASL认证，以至于他们能够SASL认证的权限通过操作Zookeeper。
Expired(-112)            会话超时状态
