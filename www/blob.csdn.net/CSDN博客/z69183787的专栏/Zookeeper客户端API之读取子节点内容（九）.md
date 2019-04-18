# Zookeeper客户端API之读取子节点内容（九） - z69183787的专栏 - CSDN博客
2018年02月25日 11:17:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：119
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
[http://blog.csdn.net/wo541075754/article/details/66967706](http://blog.csdn.net/wo541075754/article/details/66967706)
本篇博客介绍一下Zookeeper原声API读取节点内容的使用方法。
## 读取节点内容方法介绍
### 方法
Zookeeper提供了两个方法来获取节点内容，同步获取和异步获取：
```java
public byte[] getData(String path, boolean watch, Stat stat)
```
- 1
```java
public void getData(final String path, Watcher watcher,
            DataCallback cb, Object ctx)
```
- 1
- 2
### 参数说明
|参数|说明|
|----|----|
|path|指定数据节点的路径，获取该节点下面的子节点|
|watcher|注册在path上的Watcher。节点变更会通知会向客户端发起通知。|
|stat|指定数据节点状态信息。传入旧stat，方法执行过程中会将其替换为新stat对象。|
|watch|表示是否需要注册一个watcher。true：注册默认watcher，false：不需要注册watcher|
|cb|注册一个异步回调函数|
|ctx|传递上下文信息|
## 具体案例
### 同步方法
```java
package com.secbro.learn;
import org.apache.zookeeper.*;
import org.apache.zookeeper.data.Stat;
import java.util.concurrent.CountDownLatch;
/**
 *  读取节点数据
 * Created by zhuzs on 2017/3/27.
 */
public class TestGetData implements Watcher{
    private static CountDownLatch countDownLatch = new CountDownLatch(1);
    private static ZooKeeper zooKeeper;
    private static Stat stat = new Stat();
    public static void main(String[] args) throws Exception{
        zooKeeper = new ZooKeeper("127.0.0.1:2181", 5000, new TestGetData());
        countDownLatch.await();
        String path = "/test-get-data";
        zooKeeper.create(path,"123".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        System.out.println("同步读取节点内容：" + new String(zooKeeper.getData(path,true,stat)));
        System.out.println("同步读取Stat：czxid=" + stat.getCzxid()
                + ";mzxid=" + stat.getMzxid() + ";version="  + stat.getVersion());
        zooKeeper.setData(path,"123".getBytes(),-1);
        Thread.sleep(10000);
    }
    public void process(WatchedEvent event) {
        if (Event.KeeperState.SyncConnected == event.getState()) {
            if(Event.EventType.None == event.getType() && null == event.getPath()){ // 连接时的监听事件
                countDownLatch.countDown();
            } else if (event.getType() == Event.EventType.NodeDataChanged){ // 子节点内容变更时的监听
                try {
                    System.out.println("监听获得通知内容：data="
                            + new String(zooKeeper.getData(event.getPath(),true,stat)));
                    System.out.println("监听获得通知Stat：czxid=" + stat.getCzxid()
                            + ";mzxid=" + stat.getMzxid() + ";version="  + stat.getVersion());
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
运行结果：
```
同步读取节点内容：123
同步读取Stat：czxid=14700;mzxid=14700;version=0
监听获得通知内容：data=123
监听获得通知Stat：czxid=14700;mzxid=14701;version=1
```
- 1
- 2
- 3
- 4
代码的基本逻辑为创建一个临时节点，然后读取临时节点内容，并注册监听，当节点变化（内容变化或版本信息变化），触发监听事件，获取最新的节点信息。
### 异步方法
```java
package com.secbro.learn;
import org.apache.zookeeper.*;
import org.apache.zookeeper.data.Stat;
import java.util.concurrent.CountDownLatch;
/**
 *  读取节点数据
 * Created by zhuzs on 2017/3/27.
 */
public class TestGetData implements Watcher{
    private static CountDownLatch countDownLatch = new CountDownLatch(1);
    private static ZooKeeper zooKeeper;
    private static Stat stat = new Stat();
    public static void main(String[] args) throws Exception{
        zooKeeper = new ZooKeeper("127.0.0.1:2181", 5000, new TestGetData());
        countDownLatch.await();
        String path = "/test-get-data";
        zooKeeper.create(path,"123".getBytes(), ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        // 异步读取节点内容
        zooKeeper.getData(path,true,new MyDataCallback(),null);
        zooKeeper.setData(path,"123".getBytes(),-1);
        Thread.sleep(10000);
    }
    public void process(WatchedEvent event) {
        if (Event.KeeperState.SyncConnected == event.getState()) {
            if(Event.EventType.None == event.getType() && null == event.getPath()){ // 连接时的监听事件
                countDownLatch.countDown();
            } else if (event.getType() == Event.EventType.NodeDataChanged){ // 子节点内容变更时的监听
                try {
                    System.out.println("监听获得通知内容：data="
                            + new String(zooKeeper.getData(event.getPath(),true,stat)));
                    System.out.println("监听获得通知Stat：czxid=" + stat.getCzxid()
                            + ";mzxid=" + stat.getMzxid() + ";version="  + stat.getVersion());
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
class MyDataCallback implements AsyncCallback.DataCallback{
    public void processResult(int rc, String path, Object ctx, byte[] data, Stat stat) {
        System.out.println("异步返回结果：rc=" + rc + ";path=" + path + ";data=" + new String(data));
        System.out.println("异步读取Stat：czxid=" + stat.getCzxid()
                + ";mzxid=" + stat.getMzxid() + ";version="  + stat.getVersion());
    }
}
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
运行结果：
```
异步返回结果：rc=0;path=/test-get-data;data=123
异步读取Stat：czxid=14704;mzxid=14704;version=0
监听获得通知内容：data=123
监听获得通知Stat：czxid=14704;mzxid=14705;version=1
```
- 1
- 2
- 3
- 4
异步方法与同步方法业务逻辑基本相同，区别点在于将同步获取改为异步获取。
