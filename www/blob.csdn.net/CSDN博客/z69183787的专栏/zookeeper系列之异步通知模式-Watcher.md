# zookeeper系列之异步通知模式-Watcher - z69183787的专栏 - CSDN博客
2016年08月30日 13:44:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：644
Watcher介绍
Watcher是Zookeeper用来实现distribute lock, distribute configure, distribute queue等应用的主要手段。要监控data_tree上的任何节点的变化（节点本身的增加，删除，数据修改，以及孩子的变化）都可以在获取该数据时注册一个Watcher，这有很像Listener模式。一旦该节点数据变化，Follower会发送一个notification response，client收到notification响应，则会查找对应的Watcher并回调他们。
 有以下接口可以注册Watcher：
1. Stat exists(final String path, Watcher watcher)
2. Stat exists(String path, boolean watch)
3. void exists(String path, boolean watch, StatCallback cb, Object ctx)
4. void exists(final String path, Watcher watcher, StatCallback cb, Object ctx)
5. byte[] getData(final String path, Watcher watcher, Stat stat)
6. byte[] getData(String path, boolean watch, Stat stat)
7. void getData(final String path, Watcher watcher, DataCallback cb, Object ctx)
8. void getData(String path, boolean watch, DataCallback cb, Object ctx)
9. List<string> getChildren(final String path, Watcher watcher)
10. List<string> getChildren(String path, boolean watch)
11. void getChildren(final String path, Watcher watcher,ChildrenCallback cb, Object ctx)
如果参数需要传递watcher,则可以自己定义Watcher进行回调处理。如果是Boolean型变量，当为true时，则使用系统默认的Watcher，系统默认的Watcher是在zookeeper的构造函数中传递的Watcher。如果Watcher为空或者Boolean变量时为false，则表明不注册Watcher。如果获取数据后不需要关注该数据是否变化，就不需要注册Watcher。上面没有返回值的都是异步调用模式。需要注意的是，一旦Watcher被调用后，将会从map中删除，如果还需要关注数据的变化，需要再次注册。
 Watcher原理 要搞清楚Watcher的原理，让我们看看Watcher的工作流程。
![](http://dl.iteye.com/upload/attachment/274895/e719c2c7-3cfd-3f8b-b690-e28051b03ab3.gif)
Watcher的使用与注意事项
Watcher需要每次都要注册。
并不是Watcher的节点被修改n次，客户端注册就会被通知n次。如果watcher的节点频繁被修改，有可能修改了10次，客户端被通知8次。如果client与server连接状态发生改变，watcher总是会被通知。
