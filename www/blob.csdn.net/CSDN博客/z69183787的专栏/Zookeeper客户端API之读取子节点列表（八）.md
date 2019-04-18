# Zookeeper客户端API之读取子节点列表（八） - z69183787的专栏 - CSDN博客
2018年02月24日 21:45:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：266
[http://blog.csdn.net/wo541075754/article/details/66472416](http://blog.csdn.net/wo541075754/article/details/66472416)
本篇博客介绍一下Zookeeper原生客户端API提供的获取子节点列表方法。
## 获取子节点列表方法
### 方法
Zookeeper原生客户端API提供了以下8中获取子节点列表的方法，每个方法的使用说明参考注释内容：
```java
/**
 * 返回指定路径下面的子节点列表。
 * 如果watcher不为null，并且调用成功（没有异常），会将watcher注册在指定的path
 * 上。当path（父节点）被删除或者path下面创建/删除子节点，将触发通知watcher。
 * 
 * 返回结果列表不保证有序性。
 */
public List<String> getChildren(final String path, Watcher watcher)
```
```java
/**
 * 使用说明同上一个方法。
 */
public List<String> getChildren(String path, boolean watch)
```
```java
/**
 * getChildren(String, Watcher)的异步版本
 */
public void getChildren(final String path, Watcher watcher,
            ChildrenCallback cb, Object ctx)
```
```java
/**
 * getChildren(String, boolean)的异步版本
 */
public void getChildren(String path, boolean watch, ChildrenCallback cb,
            Object ctx)
```
```java
/**
 * 为指定的路径返回stat和子节点列表。
 * 
 */
public List<String> getChildren(final String path, Watcher watcher,
            Stat stat)
```
```java
/**
 * 使用说明同上一个方法。
 */
public List<String> getChildren(String path, boolean watch, Stat stat)
```
```java
/**
 * getChildren(String, Watcher, Stat)的异步版本
 */
public void getChildren(final String path, Watcher watcher,
            Children2Callback cb, Object ctx)
```
```java
/**
 * getChildren(String, boolean, Stat)的异步版本
 */
public void getChildren(String path, boolean watch, Children2Callback cb,
            Object ctx)
```
### 参数说明
|参数名|说明|
|----|----|
|path|指定数据节点的路径，获取该节点下面的子节点|
|watcher|注册在path上的Watcher。path被删除或者path下面的创建/删除节点，向客户端发送通知。可为null|
|watch|表示是否需要注册一个watcher。true：注册默认watcher，false：不需要注册watcher|
|cb|注册一个异步的回调函数|
|ctx|用户传递信息|
|stat|指定数据节点状态信息。传入旧stat，方法执行过程中会将其替换为新stat对象。|
具体案例
```java
package com.secbro.learn;
import org.apache.zookeeper.*;
import java.util.List;
import java.util.concurrent.CountDownLatch;
/**
 * 获得子节点列表的方法
 * Created by zhuzs on 2017/3/26.
 */
public class TestGetChildrenMethod implements Watcher {
    private static CountDownLatch countDownLatch = new CountDownLatch(1);
    private static ZooKeeper zooKeeper;
    public static void main(String[] args) throws Exception {
        zooKeeper = new ZooKeeper("127.0.0.1:2181", 5000, new TestGetChildrenMethod());
        countDownLatch.await();
        // 创建父节点/test
        zooKeeper.create("/test", "123".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
        // 在父节点/test下面创建a1节点
        zooKeeper.create("/test/a1", "456".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        // 同步获得结果
        List<String> childrenList = zooKeeper.getChildren("/test", true);
        System.out.println("同步getChildren获得数据结果：" + childrenList);
        // 异步获得结果
        zooKeeper.getChildren("/test",true,new MyChildren2Callback(),null);
        // 在父节点/test下面创建a2节点
        zooKeeper.create("/test/a2", "456".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        Thread.sleep(10000);
    }
    public void process(WatchedEvent event) {
        if (Event.KeeperState.SyncConnected == event.getState()) {
            if(Event.EventType.None == event.getType() && null == event.getPath()){ // 连接时的监听事件
                countDownLatch.countDown();
            } else if (event.getType() == Event.EventType.NodeChildrenChanged){ // 子节点变更时的监听
                try {
                    System.out.println("重新获得Children，并注册监听：" + zooKeeper.getChildren(event.getPath(),true));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
class MyChildren2Callback implements AsyncCallback.Children2Callback{
    public void processResult(int rc, String path, Object ctx, List<String> children, Stat stat) {
        System.out.println("异步获得getChildren结果，rc=" + rc
                + "；path=" + path + "；ctx=" + ctx + "；children=" + children +"；stat=" + stat);
    }
}
```
输出结果：
```
同步getChildren获得数据结果：[a1]
异步获得getChildren结果，rc=0；path=/test；ctx=null；children=[a1]；stat=14277,14277,1490498559704,1490498559704,0,1,0,0,3,1,14278
重新获得Children，并注册监听：[a1, a2]
```
上面的例子，先创建了一个父节点test，然后在其下面创建了a1子节点，并注册监听。当父节点test下面创建了新的a2子节点之后，监听事件触发。但此时需要重新主动获取该父节点下面的子节点，并继续注册监听事件。
上面只提供了新增子节点的案例，其实删除子节点或删除父节点同样会触发监听事件
