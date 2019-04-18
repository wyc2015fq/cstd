# Zookeeper客户端API之创建会话（六） - z69183787的专栏 - CSDN博客
2018年02月24日 21:35:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：217
[http://blog.csdn.net/wo541075754/article/details/61190967](http://blog.csdn.net/wo541075754/article/details/61190967)
Zookeeper对外提供了一套Java的客户端API。本篇博客主要讲一下创建会话。
## 创建项目
首选，创建一个基于maven管理的简单java工程。在pom文件中引入zookeeper。
```xml
<dependency>
   <groupId>org.apache.zookeeper</groupId>
   <artifactId>zookeeper</artifactId>
   <version>3.4.9</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
## 编写测试类
首选以最简单的API为例。
`public ZooKeeper(String connectString, int sessionTimeout, Watcher watcher)`- 1
其中，connectString表示要连接的zookeeper服务器地址列表，格式为：192.168.0.1:2181。支持多个地址拼接，中间用逗号分隔。其中地址后面还可以拼接上zookeeper的操作路径，比如：192.168.0.1:2181/zk/test。
sessionTimeout：会话超时时间，单位“毫秒”。通过心跳来监测会话的有效性。
watcher：监听节点的状态变化，如果发生变化则通知此watcher，做出相应处理。如果不需要监听，则可设置为null。
测试代码：
```java
package com.secbro.learn;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import java.io.IOException;
import java.util.Date;
import java.util.concurrent.CountDownLatch;
/**
 * Created by zhuzs on 2017/3/9.
 */
public class TestSession implements Watcher{
    private static CountDownLatch countDownLatch = new CountDownLatch(1);
    public static void main(String[] args) throws IOException {
        Long startTime = new Date().getTime();
        ZooKeeper zooKeeper = new ZooKeeper("192.168.0.1:2181",5000,new TestSession());
        try {
            countDownLatch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("创建连接花费时间：" + (new Date().getTime() - startTime) + "ms");
        System.out.println("连接状态：" + zooKeeper.getState());
    }
    public void process(WatchedEvent event) {
        System.out.println("Receive watcher event:" + event);
        if(Event.KeeperState.SyncConnected == event.getState()){
            countDownLatch.countDown();
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
由于Zookeeper客户端和服务器创建会话是异步过程，因此使用CountDownLatch来阻塞线程，等待服务器创建完成，并发送事件通知。
打印结果为：
```
Receive watcher event:WatchedEvent state:SyncConnected type:None path:null
创建连接花费时间：9155ms
连接状态：CONNECTED
```
- 1
- 2
- 3
## 其他接口
```java
public ZooKeeper(String connectString, int sessionTimeout, Watcher watcher,
            boolean canBeReadOnly)
```
- 1
- 2
此方法多了一个canBeReadOnly参数，此参数表示当前会话是否支持“只读”模式。
```
public ZooKeeper(String connectString, int sessionTimeout, Watcher watcher,
            long sessionId, byte[] sessionPasswd)
```
- 1
- 2
此方法允许传入sessionId和sessionPasswd，目的是为了重复使用会话。通过以下方法获得：
```
zooKeeper.getSessionId();
zooKeeper.getSessionPasswd()
```
- 1
- 2
然后作为参数创建新的连接。当sessionId和sessionPasswd不正确时，服务器会返回Expired事件。
