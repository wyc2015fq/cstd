# Zookeeper——4、使用Curator操作Zookeeper - 隔壁老王的专栏 - CSDN博客





2016年12月05日 19:57:23[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：11092








为了更好的实现Java操作zookeeper服务器，后来出现了Curator框架，非常的强大，目前已经是Apache的顶级项目，里面提供了更多丰富的操作，例如session超时重连、主从选举、分布式计数器、分布式锁等等适用于各种复杂的zookeeper场景的API封装。（[zookeeper文章所需的jar包](http://download.csdn.net/detail/haoyuyang/9703158)）

Curator所需的maven依赖：

```
<dependency>
    <groupId>org.apache.curator</groupId>
    <artifactId>curator-framework</artifactId>
    <version>3.2.1</version>
</dependency>
<dependency>
    <groupId>org.apache.curator</groupId>
    <artifactId>curator-recipes</artifactId>
    <version>3.2.1</version>
</dependency>
<dependency>
    <groupId>org.apache.curator</groupId>
    <artifactId>curator-client</artifactId>
    <version>3.2.1</version>
</dependency>
```
Curator框架中使用链式编程风格，易读性更强，使用工厂方法创建zookeeper客户端对象。



**1.使用CuratorFrameworkFactory的两个静态工厂方法（参数不同）来创建zookeeper客户端对象。**

参数1：connectString，zookeeper服务器地址及端口号，多个zookeeper服务器地址以“,”分隔。

参数2：sessionTimeoutMs，会话超时时间，单位毫秒，默认为60000ms。

参数3：connectionTimeoutMs，连接超时时间，单位毫秒，默认为15000ms。

参数4：retryPolicy，重试连接策略，有四种实现，分别为：ExponentialBackoffRetry（重试指定的次数, 且每一次重试之间停顿的时间逐渐增加）、RetryNtimes（指定最大重试次数的重试策略）、RetryOneTimes（仅重试一次）、RetryUntilElapsed（一直重试直到达到规定的时间）

Curator的Helloworld入门：

```java
public class CuratorHelloworld {
    private static final String CONNECT_ADDR = "192.168.1.102:2181,192.168.1.104:2181,192.168.1.105:2181";
    private static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws Exception {
        //重试策略，初试时间1秒，重试10次
        RetryPolicy policy = new ExponentialBackoffRetry(1000, 10);
        //通过工厂创建Curator
        CuratorFramework curator = CuratorFrameworkFactory.builder().connectString(CONNECT_ADDR)
                .sessionTimeoutMs(SESSION_TIMEOUT).retryPolicy(policy).build();
        //开启连接
        curator.start();

        ExecutorService executor = Executors.newCachedThreadPool();

        /**创建节点，creatingParentsIfNeeded()方法的意思是如果父节点不存在，则在创建节点的同时创建父节点；
         * withMode()方法指定创建的节点类型，跟原生的Zookeeper API一样，不设置默认为PERSISTENT类型。
         * */
        curator.create().creatingParentsIfNeeded().withMode(CreateMode.PERSISTENT)
                .inBackground((framework, event) -> { //添加回调
                    System.out.println("Code：" + event.getResultCode());
                    System.out.println("Type：" + event.getType());
                    System.out.println("Path：" + event.getPath());
                }, executor).forPath("/super/c1", "c1内容".getBytes());
        Thread.sleep(5000); //为了能够看到回调信息
        String data = new String(curator.getData().forPath("/super/c1")); //获取节点数据
        System.out.println(data);
        Stat stat = curator.checkExists().forPath("/super/c1"); //判断指定节点是否存在
        System.out.println(stat);
        curator.setData().forPath("/super/c1", "c1新内容".getBytes()); //更新节点数据
        data = new String(curator.getData().forPath("/super/c1"));
        System.out.println(data);
        List<String> children = curator.getChildren().forPath("/super"); //获取子节点
        for(String child : children) {
            System.out.println(child);
        }
        //放心的删除节点，deletingChildrenIfNeeded()方法表示如果存在子节点的话，同时删除子节点
        curator.delete().guaranteed().deletingChildrenIfNeeded().forPath("/super");
        curator.close();
    }
}
```
**PS：create创建节点方法可选的链式项：creatingParentsIfNeeded（是否同时创建父节点）、withMode（创建的节点类型）、forPath（创建的节点路径）、withACL（安全项）**


**delete删除节点方法可选的链式项：deletingChildrenIfNeeded（是否同时删除子节点）、guaranteed（安全删除）、withVersion（版本检查）、forPath（删除的节点路径）**

**inBackground绑定异步回调方法。比如在创建节点时绑定一个回调方法，该回调方法可以输出服务器的状态码以及服务器的事件类型等信息，还可以加入一个线程池进行优化操作。**

**2.Curator的监听**

1）NodeCache：监听节点的新增、修改操作。

```java
public class CuratorWatcher1 {
    private static final String CONNECT_ADDR = "192.168.1.102:2181,192.168.1.104:2181,192.168.1.105:2181";
    private static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws Exception {
        RetryPolicy policy = new ExponentialBackoffRetry(1000, 10);
        CuratorFramework curator = CuratorFrameworkFactory.builder().connectString(CONNECT_ADDR)
                .sessionTimeoutMs(SESSION_TIMEOUT).retryPolicy(policy).build();
        curator.start();
	//最后一个参数表示是否进行压缩
        NodeCache cache = new NodeCache(curator, "/super", false);
        cache.start(true);
        //只会监听节点的创建和修改，删除不会监听
        cache.getListenable().addListener(() -> {
            System.out.println("路径：" + cache.getCurrentData().getPath());
            System.out.println("数据：" + new String(cache.getCurrentData().getData()));
            System.out.println("状态：" + cache.getCurrentData().getStat());
        });

        curator.create().forPath("/super", "1234".getBytes());
        Thread.sleep(1000);
        curator.setData().forPath("/super", "5678".getBytes());
        Thread.sleep(1000);
        curator.delete().forPath("/super");
        Thread.sleep(5000);
        curator.close();
    }
}
```



2）PathChildrenCache：监听子节点的新增、修改、删除操作。

```java
public class CuratorWatcher2 {
    private static final String CONNECT_ADDR = "192.168.1.102:2181,192.168.1.104:2181,192.168.1.105:2181";
    private static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws Exception {
        RetryPolicy policy = new ExponentialBackoffRetry(1000, 10);
        CuratorFramework curator = CuratorFrameworkFactory.builder().connectString(CONNECT_ADDR)
                .sessionTimeoutMs(SESSION_TIMEOUT).retryPolicy(policy).build();
        curator.start();
        //第三个参数表示是否接收节点数据内容
        PathChildrenCache childrenCache = new PathChildrenCache(curator, "/super", true);
        /**
         * 如果不填写这个参数，则无法监听到子节点的数据更新
         如果参数为PathChildrenCache.StartMode.BUILD_INITIAL_CACHE，则会预先创建之前指定的/super节点
         如果参数为PathChildrenCache.StartMode.POST_INITIALIZED_EVENT，效果与BUILD_INITIAL_CACHE相同，只是不会预先创建/super节点
         参数为PathChildrenCache.StartMode.NORMAL时，与不填写参数是同样的效果，不会监听子节点的数据更新操作
         */
        childrenCache.start(PathChildrenCache.StartMode.POST_INITIALIZED_EVENT);
        childrenCache.getListenable().addListener((framework, event) -> {
            switch (event.getType()) {
                case CHILD_ADDED:
                    System.out.println("CHILD_ADDED，类型：" + event.getType() + "，路径：" + event.getData().getPath() + "，数据：" +
                    new String(event.getData().getData()) + "，状态：" + event.getData().getStat());
                    break;
                case CHILD_UPDATED:
                    System.out.println("CHILD_UPDATED，类型：" + event.getType() + "，路径：" + event.getData().getPath() + "，数据：" +
                            new String(event.getData().getData()) + "，状态：" + event.getData().getStat());
                    break;
                case CHILD_REMOVED:
                    System.out.println("CHILD_REMOVED，类型：" + event.getType() + "，路径：" + event.getData().getPath() + "，数据：" +
                            new String(event.getData().getData()) + "，状态：" + event.getData().getStat());
                    break;
                default:
                    break;
            }
        });

        curator.create().forPath("/super", "123".getBytes());
        curator.create().creatingParentsIfNeeded().withMode(CreateMode.PERSISTENT).forPath("/super/c1", "c1内容".getBytes());
        //经测试，不会监听到本节点的数据变更，只会监听到指定节点下子节点数据的变更
        curator.setData().forPath("/super", "456".getBytes());
        curator.setData().forPath("/super/c1", "c1新内容".getBytes());
        curator.delete().guaranteed().deletingChildrenIfNeeded().forPath("/super");
        Thread.sleep(5000);
        curator.close();
    }
}
```

3）TreeCache：既可以监听节点的状态，又可以监听子节点的状态。类似于上面两种Cache的组合。

```java
public class CuratorWatcher3 {
    private static final String CONNECT_ADDR = "192.168.3.58:2181,192.168.3.59:2181,192.168.3.66:2181";
    private static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws Exception {
        RetryPolicy policy = new ExponentialBackoffRetry(1000, 10);
        CuratorFramework curator = CuratorFrameworkFactory.builder().connectString(CONNECT_ADDR).sessionTimeoutMs(SESSION_TIMEOUT)
                .retryPolicy(policy).build();
        curator.start();
        TreeCache treeCache = new TreeCache(curator, "/treeCache");
        treeCache.start();
        treeCache.getListenable().addListener((curatorFramework, treeCacheEvent) -> {
            switch (treeCacheEvent.getType()) {
                case NODE_ADDED:
                    System.out.println("NODE_ADDED：路径：" + treeCacheEvent.getData().getPath() + "，数据：" + new String(treeCacheEvent.getData().getData())
                    + "，状态：" + treeCacheEvent.getData().getStat());
                    break;
                case NODE_UPDATED:
                    System.out.println("NODE_UPDATED：路径：" + treeCacheEvent.getData().getPath() + "，数据：" + new String(treeCacheEvent.getData().getData())
                            + "，状态：" + treeCacheEvent.getData().getStat());
                    break;
                case NODE_REMOVED:
                    System.out.println("NODE_REMOVED：路径：" + treeCacheEvent.getData().getPath() + "，数据：" + new String(treeCacheEvent.getData().getData())
                            + "，状态：" + treeCacheEvent.getData().getStat());
                    break;
                default:
                    break;
            }
        });

        curator.create().forPath("/treeCache", "123".getBytes());
        curator.create().creatingParentsIfNeeded().withMode(CreateMode.PERSISTENT).forPath("/treeCache/c1", "456".getBytes());
        curator.setData().forPath("/treeCache", "789".getBytes());
        curator.setData().forPath("/treeCache/c1", "910".getBytes());
        curator.delete().forPath("/treeCache/c1");
        curator.delete().forPath("/treeCache");
        Thread.sleep(5000);
        curator.close();
    }
}
```
运行结果：![](https://img-blog.csdn.net/20161206141056967)


**PS：Curator 2.4.2的jar包没有TreeCache，我升级到了3.2.1的版本。但是在运行时报java.lang.NoSuchMethodError:org.apache.zookeeper.server.quorum.flexible.QuorumMaj.<init>(Ljava/util/Map;，出现这个错误的原因是因为zookeeper服务器的版本与zookeeper.jar的版本不一致，因此将zookeeper.jar升级到与zookeeper服务器对应的3.5.2。再次运行，又报java.lang.NoSuchMethodError: com.google.common.collect.Sets.newConcurrentHashSet()Ljav;，好吧，一看跟之前的错误一样，都是NoSuchMethodError，我猜想应该是guava的版本与zookeeper.jar所依赖的版本不一致（zookeeper.jar依赖[io.netty](http://mvnrepository.com/artifact/io.netty)，而[io.netty](http://mvnrepository.com/artifact/io.netty)依赖[com.google.protobuf](http://mvnrepository.com/artifact/com.google.protobuf) » [protobuf-java](http://mvnrepository.com/artifact/com.google.protobuf/protobuf-java)），so，将guava的版本升级到了20.0，运行成功！**

**3.Curator应用场景**

①分布式锁（该部分来自[跟着实例学习ZooKeeper的用法： 分布式锁](http://ifeve.com/zookeeper-lock/)）

在分布式场景中，为了保证数据的一致性，经常在程序运行的某一个点需要进行同步操作（Java中提供了Synchronized和ReentrantLock实现）。我们使用Curator基于Zookeeper的特性提供的分布式锁来处理分布式场景的数据一致性。

**可重入锁：InterProcessMutex(CuratorFramework client, String path)**

通过acquire()获得锁，并提供超时机制；通过release()释放锁。makeRevocable(RevocationListener<T> listener)定义了可协商的撤销机制，当别的进程或线程想让你释放锁时，listener会被调用。如果请求撤销当前的锁，可以调用attemptRevoke(CuratorFramework client, String path)。

首先创建一个模拟的公共资源，这个资源期望只能单线程的访问，否则会有并发问题。

```java
public class FakeLimitedResource {
    private final AtomicBoolean inUse = new AtomicBoolean(false);
    public void use() throws Exception {
        //这个例子在使用锁的情况下不会抛出非法并发异常IllegalStateException
        //但是在无锁的情况下，由于sleep了一段时间，所以很容易抛出异常
        if(!inUse.compareAndSet(false, true)) {
            throw new IllegalStateException("Needs to be used by one client at a time");
        }

        try {
            Thread.sleep((long) (3 * Math.random()));
        } finally {
            inUse.set(false);
        }
    }
}
```
然后创建一个ExampleClientThatLocks类，它负责请求锁、使用资源、释放锁这样一个完整的访问过程。
```java
public class ExampleClientThatLocks {
    private final InterProcessMutex lock;
    //private final InterProcessSemaphoreMutex lock;
    private final FakeLimitedResource resource;
    private final String clientName;

    public ExampleClientThatLocks(CuratorFramework framework, String path, FakeLimitedResource resource, String clientName) {
        this.lock = new InterProcessMutex(framework, path);
        //this.lock = new InterProcessSemaphoreMutex(framework, path);
        this.resource = resource;
        this.clientName = clientName;
    }

    public void doWork(long time, TimeUnit timeUnit) throws Exception {
        if(!lock.acquire(time, timeUnit)) {
            throw new IllegalStateException(clientName + " could not acquire the lock!");
        }
        System.out.println(clientName + " has the lock");

        /*if(!lock.acquire(time, timeUnit)) {
            throw new IllegalStateException(clientName + " could not acquire the lock!");
        }
        System.out.println(clientName + " has the lock");*/

        try {
            resource.use();
        } finally {
            System.out.println(clientName + " releasing the lock");
            lock.release();
            //lock.release();
        }
    }
}
```



最后创建主程序来测试。

```java
public class InterProcessMutexExample {
    private static final int QTY = 5;
    private static final int REPETITIONS = QTY * 10;
    private static final String PATH = "/examples/locks";
    private static final String CONNECT_ADDR = "192.168.3.58:2181,192.168.3.59:2181,192.168.3.66:2181";

    public static void main(String[] args) throws Exception {
        final FakeLimitedResource resource = new FakeLimitedResource();
        ExecutorService executor = Executors.newFixedThreadPool(QTY);
        try {
            for(int i=0; i<QTY; i++) {
                final int index = i;
                Callable<Void> task = () -> {
                    CuratorFramework curator = CuratorFrameworkFactory.newClient(CONNECT_ADDR, new RetryNTimes(3, 1000));
                    curator.start();
                    try {
                        final ExampleClientThatLocks example = new ExampleClientThatLocks(curator, PATH, resource, "Client " + index);
                        for(int j=0; j<REPETITIONS; j++) {
                            example.doWork(10, TimeUnit.SECONDS);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    } finally {
                        CloseableUtils.closeQuietly(curator);
                    }
                    return null;
                };
                executor.submit(task);
            }
            executor.shutdown();
            executor.awaitTermination(10, TimeUnit.MINUTES);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
代码也很简单，生成10个client， 每个client重复执行10次，请求锁–访问资源–释放锁的过程。每个client都在独立的线程中。结果可以看到，锁是随机的被每个实例排他性的使用。既然是可重用的，你可以在一个线程中多次调用acquire,在线程拥有锁时它总是返回true。你不应该在多个线程中用同一个InterProcessMutex， 你可以在每个线程中都生成一个InterProcessMutex实例，它们的path都一样，这样它们可以共享同一个锁。



**不可重入锁：InterProcessSemaphoreMutex**

这个锁和可重入锁相比，就是少了Reentrant功能，也就意味着不能在同一个线程中重入，使用方法和上面的类似。将ExampleClientThatLocks修改成如下：

```java
public class ExampleClientThatLocks {
    //private final InterProcessMutex lock;
    private final InterProcessSemaphoreMutex lock;
    private final FakeLimitedResource resource;
    private final String clientName;

    public ExampleClientThatLocks(CuratorFramework framework, String path, FakeLimitedResource resource, String clientName) {
        //this.lock = new InterProcessMutex(framework, path);
        this.lock = new InterProcessSemaphoreMutex(framework, path);
        this.resource = resource;
        this.clientName = clientName;
    }

    public void doWork(long time, TimeUnit timeUnit) throws Exception {
        if(!lock.acquire(time, timeUnit)) {
            throw new IllegalStateException(clientName + " could not acquire the lock!");
        }
        System.out.println(clientName + " has the lock");

        if(!lock.acquire(time, timeUnit)) {
            throw new IllegalStateException(clientName + " could not acquire the lock!");
        }
        System.out.println(clientName + " has the lock");

        try {
            resource.use();
        } finally {
            System.out.println(clientName + " releasing the lock");
            lock.release();
            lock.release();
        }
    }
}
```

注意我们也需要调用release两次。这和JDK的ReentrantLock用法一致。如果少调用一次release，则此线程依然拥有锁。 上面的代码没有问题，我们可以多次调用acquire，后续的acquire也不会阻塞。 将上面的InterProcessMutex换成不可重入锁InterProcessSemaphoreMutex,如果再运行上面的代码，结果就会发现线程被阻塞再第二个acquire上。 也就是此锁不是可重入的。

**可重入读写锁：InterProcessReadWriteLock**

类似JDK的ReentrantReadWriteLock. 一个读写锁管理一对相关的锁。 一个负责读操作，另外一个负责写操作。 读操作在写锁没被使用时可同时由多个进程使用，而写锁使用时不允许读 (阻塞)。 此锁是可重入的。一个拥有写锁的线程可重入读锁，但是读锁却不能进入写锁。 这也意味着写锁可以降级成读锁， 比如请求写锁 —>读锁 —->释放写锁。 从读锁升级成写锁是不行的。


使用时首先创建一个InterProcessReadWriteLock实例，然后再根据你的需求得到读锁或者写锁， 读写锁的类型是InterProcessLock。


在可重入锁的代码基础上，使用下面的ExampleClientReadWriteLocks替换ExampleClientThatLocks类即可。
```java
public class ExampleClientReadWriteLocks {
    private final InterProcessReadWriteLock readWriteLock;
    private final InterProcessMutex readLock;
    private final InterProcessMutex writeLock;
    private final FakeLimitedResource resource;
    private final String clientName;

    public ExampleClientReadWriteLocks(CuratorFramework client, String path, FakeLimitedResource resource, String clientName) {
        this.readWriteLock = new InterProcessReadWriteLock(client, path);
        this.readLock = readWriteLock.readLock();
        this.writeLock = readWriteLock.writeLock();
        this.resource = resource;
        this.clientName = clientName;
    }

    public void doWork(long time, TimeUnit unit) throws Exception {
        if(!writeLock.acquire(time, unit)) {
            throw new IllegalStateException(clientName + " could not acquire the writeLock!");
        }
        System.out.println(clientName + " has the writeLock");

        if(!readLock.acquire(time, unit)) {
            throw new IllegalStateException(clientName + " could not acquire the readLock!");
        }
        System.out.println(clientName + " has the readLock");

        try {
            resource.use();
        } finally {
            readLock.release();
            writeLock.release();
        }
    }
}
```


在这个类中我们首先请求了一个写锁， 然后降级成读锁。 执行业务处理，然后释放读写锁。


**信号量：InterProcessSemaphoreV2**

一个计数的信号量类似JDK的Semaphore。 JDK中Semaphore维护的一组许可(permits)，而Cubator中称之为租约(Lease)。 有两种方式可以决定semaphore的最大租约数。第一种方式是有用户给定的path决定。第二种方式使用SharedCountReader类。 如果不使用SharedCountReader, 没有内部代码检查进程是否假定有10个租约而进程B假定有20个租约。 所以所有的实例必须使用相同的numberOfLeases值.

这次调用acquire会返回一个租约对象。 客户端必须在finally中close这些租约对象，否则这些租约会丢失掉。 但是， 但是，如果客户端session由于某种原因比如crash丢掉， 那么这些客户端持有的租约会自动close， 这样其它客户端可以继续使用这些租约。 租约还可以通过下面的方式返还：



```java
public void returnAll(Collection<Lease> leases)
public void returnLease(Lease lease)
```
注意一次你可以请求多个租约，如果Semaphore当前的租约不够，则请求线程会被阻塞。 同时还提供了超时的重载方法。
```java
public Lease acquire()
public Collection<Lease> acquire(int qty)
public Lease acquire(long time, TimeUnit unit)
public Collection<Lease> acquire(int qty, long time, TimeUnit unit)
```
下面是例子：
```java
public class InterProcessSemaphoreExample {
    private static final int MAX_LEASE = 10;
    private static final String PATH = "/examples/locks";

    public static void main(String[] args) throws Exception {
        FakeLimitedResource resource = new FakeLimitedResource();
        try (TestingServer server = new TestingServer()) {

            CuratorFramework client = CuratorFrameworkFactory.newClient(server.getConnectString(), new ExponentialBackoffRetry(1000, 3));
            client.start();

            InterProcessSemaphoreV2 semaphore = new InterProcessSemaphoreV2(client, PATH, MAX_LEASE);
            Collection<Lease> leases = semaphore.acquire(5);
            System.out.println("get " + leases.size() + " leases");
            Lease lease = semaphore.acquire();
            System.out.println("get another lease");

            resource.use();

            Collection<Lease> leases2 = semaphore.acquire(5, 10, TimeUnit.SECONDS);
            System.out.println("Should timeout and acquire return " + leases2);

            System.out.println("return one lease");
            semaphore.returnLease(lease);
            System.out.println("return another 5 leases");
            semaphore.returnAll(leases);
        }
    }

}
```
首先我们先获得了5个租约， 最后我们把它还给了semaphore。 接着请求了一个租约，因为semaphore还有5个租约，所以请求可以满足，返回一个租约，还剩4个租约。 然后再请求一个租约，因为租约不够，阻塞到超时，还是没能满足，返回结果为null。上面说讲的锁都是公平锁(fair)。
 总ZooKeeper的角度看， 每个客户端都按照请求的顺序获得锁。 相当公平。


**多锁对象：InterProcessMultiLock**

Multi Shared Lock是一个锁的容器。 当调用acquire， 所有的锁都会被acquire，如果请求失败，所有的锁都会被release。 同样调用release时所有的锁都被release(失败被忽略)。 基本上，它就是组锁的代表，在它上面的请求释放操作都会传递给它包含的所有的锁。


例子如下：
```java
public class InterProcessMultiLockExample {
    private static final String PATH1 = "/examples/locks1";
    private static final String PATH2 = "/examples/locks2";

    public static void main(String[] args) throws Exception {
        FakeLimitedResource resource = new FakeLimitedResource();
        try (TestingServer server = new TestingServer()) {
            CuratorFramework client = CuratorFrameworkFactory.newClient(server.getConnectString(), new ExponentialBackoffRetry(1000, 3));
            client.start();

            InterProcessLock lock1 = new InterProcessMutex(client, PATH1);
            InterProcessLock lock2 = new InterProcessSemaphoreMutex(client, PATH2);

            InterProcessMultiLock lock = new InterProcessMultiLock(Arrays.asList(lock1, lock2));

            if (!lock.acquire(10, TimeUnit.SECONDS)) {
                throw new IllegalStateException("could not acquire the lock");
            }
            System.out.println("has the lock");

            System.out.println("has the lock1: " + lock1.isAcquiredInThisProcess());
            System.out.println("has the lock2: " + lock2.isAcquiredInThisProcess());

            try {            
                resource.use(); //access resource exclusively
            } finally {
                System.out.println("releasing the lock");
                lock.release(); // always release the lock in a finally block
            }
            System.out.println("has the lock1: " + lock1.isAcquiredInThisProcess());
            System.out.println("has the lock2: " + lock2.isAcquiredInThisProcess());
        }
    }

}
```
新建一个InterProcessMultiLock， 包含一个重入锁和一个非重入锁。 调用acquire后可以看到线程同时拥有了这两个锁。 调用release看到这两个锁都被释放了。

②分布式计数器

一说到分布式计数器，你可能马上想到AtomicInteger这种经典的方式。如果是在同一个JVM下肯定没有问题，但是在分布式场景下，肯定会存在问题。所以就需要使用Curator框架的DistributedAtomicInteger了。

```java
public class CuratorDistributedAtomicInteger {
    private static final String CONNECT_ADDR = "192.168.1.102:2181,192.168.1.104:2181,192.168.1.105:2181";
    private static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws Exception {
        //重试策略，初试时间1秒，重试10次
        RetryPolicy policy = new ExponentialBackoffRetry(1000, 10);
        //通过工厂创建Curator
        CuratorFramework curator = CuratorFrameworkFactory.builder().connectString(CONNECT_ADDR)
                .sessionTimeoutMs(SESSION_TIMEOUT).retryPolicy(policy).build();
        //开启连接
        curator.start();

        DistributedAtomicInteger atomicInteger = new DistributedAtomicInteger(curator, "/super", new RetryNTimes(3, 1000));
        AtomicValue<Integer> value = atomicInteger.add(1);
        System.out.println(value.succeeded());
        System.out.println(value.preValue()); //新值
        System.out.println(value.postValue()); //旧值
        curator.close();
    }
}
```

③Barrier

分布式Barrier是这样一个类：它会阻塞所有节点上的等待进程，知道某一个被满足，然后所有的节点继续执行。比如赛马比赛中，等赛马陆续来到起跑线前，一声令下，所有的赛马都飞奔而出。

**DistributedBarrier**类实现了栏栅的功能，构造方法如下：

```java
public DistributedBarrier(CuratorFramework client, String barrierPath)
```
首先需要调用setBarrier()方法设置栏栅，它将阻塞在它上面等待的线程，然后需要阻塞的线程调用waitOnBarrier()方法等待放行条件。当条件满足时调用removeBarrier()方法移除栏栅，所有等待的线程将继续执行。

接下来看例子：

```java
public class DistributedBarrierExample {
    private static final String CONNECT_ADDR = "192.168.1.102:2181,192.168.1.104:2181,192.168.1.105:2181";
    private static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws Exception {
        CuratorFramework curator = CuratorFrameworkFactory.newClient(CONNECT_ADDR, new RetryNTimes(3, 1000));
        curator.start();

        ExecutorService executor = Executors.newFixedThreadPool(5);
        DistributedBarrier controlBarrier = new DistributedBarrier(curator, "/example/barrier");
        controlBarrier.setBarrier();

        for(int i=0; i<5; i++) {
            final DistributedBarrier barrier = new DistributedBarrier(curator, "/example/barrier");
            final int index = i;
            Callable<Void> task = () -> {
                Thread.sleep((long) (3 * Math.random()));
                System.out.println("Client#" + index + " wait on Barrier");
                barrier.waitOnBarrier();
                System.out.println("Client#" + index + " begins");
                return null;
            };

            executor.submit(task);
        }

        Thread.sleep(5000);
        controlBarrier.removeBarrier();
        Thread.sleep(5000);
        executor.shutdown();
        curator.close();
    }
}
```
**双栏栅：DistributedDoubleBarrier，**双栏栅允许客户端在计算的开始和结束时同步。当足够的进程加入到双栏栅时，进程开始计算，当计算完成时离开栏栅。DistributedDoubleBarrier构造方法如下：
```java
public DistributedDoubleBarrier(CuratorFramework client, String barrierPath, int memberQty)
```
memberQty是成元数量，当enter()方法被调用时，成员被阻塞，直到所有的成员都调用了enter()方法。当leave()方法被调用时，它也阻塞调用线程，直到所有的成员都调用了leave()方法。就像百米赛跑比赛，发令枪响，所有的运动员开始跑，等所有的运动员跑过终点线，比赛才结束。

例子代码：

```java
public class DistributedDoubleBarrierExample {
    private static final String CONNECT_ADDR = "192.168.1.102:2181,192.168.1.104:2181,192.168.1.105:2181";

    public static void main(String[] args) throws InterruptedException {
        CuratorFramework curator = CuratorFrameworkFactory.newClient(CONNECT_ADDR, new RetryNTimes(3, 1000));
        curator.start();

        ExecutorService executor = Executors.newFixedThreadPool(5);
        for(int i=0; i<5; i++) {
            final DistributedDoubleBarrier barrier = new DistributedDoubleBarrier(curator, "/example/barrier", 5);
            final int index = i;
            Callable<Void> task = () -> {
                Thread.sleep((long) (3000 * Math.random()));
                System.out.println("Client#" + index + " enter");
                barrier.enter();
                System.out.println("Client#" + index + "begin");
                Thread.sleep((long) (3000 * Math.random()));
                barrier.leave();
                System.out.println("Client#" + index + "left");
                return null;
            };
            executor.submit(task);
        }

        executor.shutdown();;
        executor.awaitTermination(10, TimeUnit.MINUTES);
        curator.close();
    }
}
```









