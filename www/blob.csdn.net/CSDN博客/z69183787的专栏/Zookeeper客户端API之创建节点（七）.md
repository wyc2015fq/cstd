# Zookeeper客户端API之创建节点（七） - z69183787的专栏 - CSDN博客
2018年02月24日 21:44:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：239
[http://blog.csdn.net/wo541075754/article/details/65625481](http://blog.csdn.net/wo541075754/article/details/65625481)
本篇博客主要讲一下使用Zookeeper原生的客户API来创建一个数据节点。
## 创建数据节点方法
Zookeeper提供了两个创建数据节点的方法。
同步创建数据节点方法：
```java
public String create(final String path, byte data[], List<ACL> acl,
            CreateMode createMode)
        throws KeeperException, InterruptedException
```
异步创建数据节点方法：
```java
public void create(final String path, byte data[], List<ACL> acl,
            CreateMode createMode,  StringCallback cb, Object ctx)
```
### 参数说明
|参数名称|说明|
|----|----|
|path|创建节点的路径，比如：/zk-test-create。|
|data[]|字节数组，创建节点初始化内容。使用者需自己进行序列化和反序列化。复杂对象可使用 Hessian或Kryo进行进行序列化和反序列化。|
|acl|节点的acl策略|
|createMode|节点类型，类型定义在枚举CreateMode中：（1）PERSISTENT：持久；（2）PERSISTENT_SEQUENTIAL：持久顺序；（3）EPHEMERAL：临时；（4）EPHEMERAL_SEQUENTIAL：临时顺序。|
|cb|异步创建方法参数。注册的回调函数，需实现StringCallback接口。主要针对public void processResult(int rc, String path, Object ctx, String name);接口进行重写。数据节点创建完成之后，会调用此方法进行业务逻辑处理。|
|ctx|异步创建方法参数。用户传递一个对象，可以在回调方法执行时使用，通常是放一个上下文（Context）信息|
创建节点demo
下面以具体代码来说明不同方法的使用，针对不同的方法有相应的注释说明：
```java
package com.secbro.learn;
import org.apache.zookeeper.*;
import java.util.concurrent.CountDownLatch;
/**
 * Created by zhuzs on 2017/3/23.
 */
public class TestCreateNode implements Watcher {
    private static CountDownLatch countDownLatch = new CountDownLatch(1);
    public static void main(String[] args) throws Exception {
        ZooKeeper zooKeeper = new ZooKeeper("127.0.0.1:2181", 5000, new TestCreateNode());
        countDownLatch.await();
        // 同步创建临时节点
        String ephemeralPath = zooKeeper.create("/zk-test-create-ephemeral-", "123".getBytes(),
                ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        System.out.println("同步创临时建节点成功：" + ephemeralPath);
        // 同步创建临时顺序节点
        String sequentialPath = zooKeeper.create("/zk-test-create-sequential-", "456".getBytes(),
                ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL_SEQUENTIAL);
        System.out.println("同步创建临时顺序节点成功：" + sequentialPath);
        // 异步创建临时节点
        zooKeeper.create("/zk-test-create-async-ephemeral-", "abc".getBytes(),
                ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL, new MyStringCallBack(), "我是传递内容");
        // 异步创建临时顺序节点
        zooKeeper.create("/zk-test-create-async-sequential-","def".getBytes(),
                ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL_SEQUENTIAL,new MyStringCallBack(),"我是传递内容");
        Thread.sleep(10000); // 验证等待回调结果使用，可根据实际情况自行调整
    }
    public void process(WatchedEvent event) {
        if (Event.KeeperState.SyncConnected == event.getState()) {
            countDownLatch.countDown();
        }
    }
}
class MyStringCallBack implements AsyncCallback.StringCallback {
    public void processResult(int rc, String path, Object ctx, String name) {
        System.out.println("异步创建回调结果：状态：" + rc +"；创建路径：" +
                path + "；传递信息：" + ctx + "；实际节点名称：" + name);
    }
}
```
输出结果为：
```
同步创临时建节点成功：/zk-test-create-ephemeral-
同步创建临时顺序节点成功：/zk-test-create-sequential-0000000023
异步创建回调结果：状态：0；创建路径：/zk-test-create-async-ephemeral-；传递信息：我是传递内容；实际节点名称：/zk-test-create-async-ephemeral-
异步创建回调结果：状态：0；创建路径：/zk-test-create-async-sequential-；传递信息：我是传递内容；实际节点名称：/zk-test-create-async-sequential-0000000025
```
根据上面的代码和结果，很容易得知不同方法的使用方式。
## 注意事项
（1）Zookeeper不支持递归创建数据节点，无法在父节点不存在的情况下创建子节点。否则会抛出类似以下异常：
```
Exception in thread "main" org.apache.zookeeper.KeeperException$NoNodeException: KeeperErrorCode = NoNode for /zk-test-create-sequential-/11
    at org.apache.zookeeper.KeeperException.create(KeeperException.java:111)
    at org.apache.zookeeper.KeeperException.create(KeeperException.java:51)
    at org.apache.zookeeper.ZooKeeper.create(ZooKeeper.java:783)
    at com.secbro.learn.TestCreateNode.main(TestCreateNode.java:24)
    at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
    at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
    at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
    at java.lang.reflect.Method.invoke(Method.java:606)
    at com.intellij.rt.execution.application.AppMain.main(AppMain.java:147)
```
（2）如果节点已经存在，再创建同名节点，会抛出NodeExistsException。 
（3）关于权限控制，如果没有特殊要求，可按照上面例子中直接设置为ZooDefs.Ids.OPEN_ACL_UNSAFE，表明之后对节点的任何操作都不受权限控制。
## StringCallback接口相关
StringCallback接口集成了AsyncCallback接口，来实现回调时的业务处理。其中AsyncCallback接口还包8个回调接口：StatCallback、DataCallback、ACLCallback、ChildrenCallback、Children2Callback、VoidCallback、MultiCallback、StringCallback。可以在不同的异步接口中实现不同的回调接口。
StringCallback接口的public void processResult(int rc, String path, Object ctx, String name)方法。 
从上面的实例中已经可以看出。 
int rc为服务器的响应码，0表示调用成功，-4表示连接已断开，-110表示指定节点已存在，-112表示会话已过期。 
String path调用create方法时传入的path。 
Object ctx调用create方法时传入的ctx。 
String name创建成功的节点名称。
