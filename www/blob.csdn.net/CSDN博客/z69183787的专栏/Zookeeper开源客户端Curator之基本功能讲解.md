# Zookeeper开源客户端Curator之基本功能讲解 - z69183787的专栏 - CSDN博客
2018年02月25日 11:18:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：176
[http://blog.csdn.net/wo541075754/article/details/69138878](http://blog.csdn.net/wo541075754/article/details/69138878)
# 简介
Curator是Netflix公司开源的一套Zookeeper客户端框架。了解过Zookeeper原生API都会清楚其复杂度。Curator帮助我们在其基础上进行封装、实现一些开发细节，包括接连重连、反复注册Watcher和NodeExistsException等。目前已经作为Apache的顶级项目出现，是最流行的Zookeeper客户端之一。从编码风格上来讲，它提供了基于Fluent的编程风格支持。
除此之外，Curator还提供了Zookeeper的各种应用场景：Recipe、共享锁服务、Master选举机制和分布式计数器等。 
![这里写图片描述](https://img-blog.csdn.net/20170404185248925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd281NDEwNzU3NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 项目及依赖
关于项目的介绍信息可以参考Apache官网提供的关于[Curator](http://curator.apache.org/index.html)的资料信息。项目在GitHub上的开源地址随着从Netflix转移到Apache也发生了变化。原地址为：[https://github.com/Netflix/curator](https://github.com/Netflix/curator)，新地址为：[https://github.com/apache/curator](https://github.com/apache/curator)。
## 版本
目前Curator有2.x.x和3.x.x两个系列的版本，支持不同版本的Zookeeper。其中Curator 2.x.x兼容Zookeeper的3.4.x和3.5.x。而Curator 3.x.x只兼容Zookeeper 3.5.x，并且提供了一些诸如动态重新配置、watch删除等新特性。
## 项目组件
|名称|描述|
|----|----|
|Recipes|Zookeeper典型应用场景的实现，这些实现是基于Curator Framework。|
|Framework|Zookeeper API的高层封装，大大简化Zookeeper客户端编程，添加了例如Zookeeper连接管理、重试机制等。|
|Utilities|为Zookeeper提供的各种实用程序。|
|Client|Zookeeper client的封装，用于取代原生的Zookeeper客户端（ZooKeeper类），提供一些非常有用的客户端特性。|
|Errors|Curator如何处理错误，连接问题，可恢复的例外等。|
## Maven依赖
Curator的jar包已经发布到Maven中心，由以下几个artifact的组成。根据需要选择引入具体的artifact。但大多数情况下只用引入curator-recipes即可。
|GroupID/Org|ArtifactID/Name|描述|
|----|----|----|
|org.apache.curator|curator-recipes|所有典型应用场景。需要依赖client和framework，需设置自动获取依赖。|
|org.apache.curator|curator-framework|同组件中framework介绍。|
|org.apache.curator|curator-client|同组件中client介绍。|
|org.apache.curator|curator-test|包含TestingServer、TestingCluster和一些测试工具。|
|org.apache.curator|curator-examples|各种使用Curator特性的案例。|
|org.apache.curator|curator-x-discovery|在framework上构建的服务发现实现。|
|org.apache.curator|curator-x-discoveryserver|可以喝Curator Discovery一起使用的RESTful服务器。|
|org.apache.curator|curator-x-rpc|Curator framework和recipes非java环境的桥接。|
根据上面的描述，开发人员大多数情况下使用的都是curator-recipes的依赖，此依赖的maven配置如下：
```xml
<dependency>
    <groupId>org.apache.curator</groupId>
    <artifactId>curator-recipes</artifactId>
    <version>2.12.0</version>
</dependency>
```
- 1
- 2
- 3
- 4
- 5
由于版本兼容原因，采用了2.x.x的最高版本。
# 案例及功能说明
## 创建会话
Curator的创建会话方式与原生的API和ZkClient的创建方式区别很大。Curator创建客户端是通过CuratorFrameworkFactory工厂类来实现的。其中，此工厂类提供了三种创建客户端的方法。 
前两种方法是通过newClient来实现，仅参数不同而已。
```
public static CuratorFramework newClient(String connectString, RetryPolicy retryPolicy)
public static CuratorFramework newClient(String connectString, int sessionTimeoutMs, int connectionTimeoutMs, RetryPolicy retryPolicy)
```
- 1
- 2
- 3
使用上面方法创建出一个CuratorFramework之后，需要再调用其start()方法完成会话创建。 
实例代码：
```
RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000,3);
        CuratorFramework client = CuratorFrameworkFactory.newClient("127.0.0.1:2181",retryPolicy);
        client.start();
```
- 1
- 2
- 3
```
RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000,3);
CuratorFramework client = CuratorFrameworkFactory.newClient("127.0.0.1:2181",
                5000,1000,retryPolicy);
         client.start();
```
- 1
- 2
- 3
- 4
其中参数RetryPolicy提供重试策略的接口，可以让用户实现自定义的重试策略。默认提供了以下实现，分别为ExponentialBackoffRetry、BoundedExponentialBackoffRetry、RetryForever、RetryNTimes、RetryOneTime、RetryUntilElapsed。
进一步查看源代码可以得知，其实这两种方法内部实现一样，只是对外包装成不同的方法。它们的底层都是通过第三个方法builder来实现的。 
实例代码：
```
RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000,3);
        CuratorFramework client =CuratorFrameworkFactory.builder()
                .connectString("127.0.0.1:2181")
                .retryPolicy(retryPolicy)
                .sessionTimeoutMs(6000)
                .connectionTimeoutMs(3000)
                .build();
        client.start();
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
观察上面的实例，我们可以看到此处已经使用了Fluent风格的编码。其中namespace(“demo”)这项设置用来定义此会话的独立命名空间，随后的相应操作都是在此命名空间下进行操作。
### 重试策略
上面的例子中使用到了ExponentialBackoffRetry重试策略实现。此策略先给定一个初始化sleep时间baseSleepTimeMs，在此基础上结合重试次数，通过以下代码计算当前需要的sleep时间：
```
long sleepMs = baseSleepTimeMs * Math.max(1, random.nextInt(1 << (retryCount + 1)));
if ( sleepMs > maxSleepMs ){
            sleepMs = maxSleepMs;
 }
```
- 1
- 2
- 3
- 4
随着重试次数的增加，计算出的sleep时间也会越来越大。如果超过maxSleepMs则使用maxSleepMs的时间。其中maxRetries限制了最大的尝试次数。
## 创建节点
Curator创建节点的方法也是基于Fluent风格编码，原生API中的参数很多都转化为一层层的方法调用来进行设置。下面简单介绍一下常用的几个节点创建场景。 
（1）创建一个初始内容为空的节点
`client.create().forPath(path);`- 1
Curator默认创建的是持久节点，内容为空。 
（2）创建一个包含内容的节点
`client.create().forPath(path,"我是内容".getBytes());`- 1
Curator和ZkClient不同的是依旧采用Zookeeper原生API的风格，内容使用byte[]作为方法参数。 
（3）创建临时节点，并递归创建父节点
`client.create().creatingParentsIfNeeded().withMode(CreateMode.EPHEMERAL).forPath(path);`- 1
此处Curator和ZkClient一样封装了递归创建父节点的方法。在递归创建父节点时，父节点为持久节点。
## 删除节点
删除节点的方法也是基于Fluent方式来进行操作，不同类型的操作调用新增不同的方法调用即可。 
（1）删除一个子节点
`client.delete().forPath(path);`- 1
（2）删除节点并递归删除其子节点
`client.delete().deletingChildrenIfNeeded().forPath(path);`- 1
（3）指定版本进行删除
`client.delete().withVersion(1).forPath(path);`- 1
如果此版本已经不存在，则删除异常，异常信息如下。
```bash
org.apache.zookeeper.KeeperException$BadVersionException: KeeperErrorCode = BadVersion for
```
- 1
（4）强制保证删除一个节点
`client.delete().guaranteed().forPath(path);`- 1
只要客户端会话有效，那么Curator会在后台持续进行删除操作，直到节点删除成功。比如遇到一些网络异常的情况，此guaranteed的强制删除就会很有效果。
## 读取数据
读取节点数据内容API相当简单，Curator提供了传入一个Stat，使用节点当前的Stat替换到传入的Stat的方法，查询方法执行完成之后，Stat引用已经执行当前最新的节点Stat。
```
// 普通查询
client.getData().forPath(path);
// 包含状态查询
Stat stat = new Stat();
client.getData().storingStatIn(stat()).forPath(path);
```
- 1
- 2
- 3
- 4
- 5
## 更新数据
更新数据，如果未传入version参数，那么更新当前最新版本，如果传入version则更新指定version，如果version已经变更，则抛出异常。
```
// 普通更新
client.setData().forPath(path,"新内容".getBytes());
// 指定版本更新
client.setData().withVersion(1).forPath(path);
```
- 1
- 2
- 3
- 4
版本不一直异常信息：
```bash
org.apache.zookeeper.KeeperException$BadVersionException: KeeperErrorCode = BadVersion for
```
- 1
## 异步接口
在使用以上针对节点的操作API时，我们会发现每个接口都有一个inBackground()方法可供调用。此接口就是Curator提供的异步调用入口。对应的异步处理接口为BackgroundCallback。此接口指提供了一个processResult的方法，用来处理回调结果。其中processResult的参数event中的getType()包含了各种事件类型，getResultCode()包含了各种响应码。
重点说一下inBackground的以下接口：
`public T inBackground(BackgroundCallback callback, Executor executor);`- 1
此接口就允许传入一个Executor实例，用一个专门线程池来处理返回结果之后的业务逻辑。
