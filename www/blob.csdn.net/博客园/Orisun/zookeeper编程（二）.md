# zookeeper编程（二） - Orisun - 博客园







# [zookeeper编程（二）](https://www.cnblogs.com/zhangchaoyang/articles/3813217.html)





两年前写过[ZooKeeper编程(一)](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html)，那时候还在实习。近期组内做了个zookeeper编程的分享，就又把各种问题整理了一下。以下只是简单地copy了幻灯片中的内容，写得不够连贯，读者见谅。



## ZooKeeper的轮廓

![](https://images0.cnblogs.com/i/103496/201406/281029544147043.png)

/---root

  |

  \----child1

  |

  \----child2

  |

  \----child3

     |

     \----grandson1

     |

     \----grandson2

采用了简化的praxos算法来确保zookeeper集群节点的数据一致性

只要Quorum的成员有一半以上处于正常状态，就能对外提供服务

任何修改命令都需要leader协调。 在leader的协调过程中，需要3次leader与Follower之间的来回请求响应。并且在此过程中还会涉及事务日志的记录，更糟糕的情况是还有take snapshot的操作。因此此过程可能比较耗时。但Zookeeper的通信中最大特点是异步的，如果请求是连续不断的,Zookeeper的处理是集中处理逻辑，然后批量发送，批量的大小也是有控制的。如果请求量不大，则即刻发送。这样当负载很大时也能保证很大的吞吐量，时效性也在一定程度上进行了保证。

## Zookeeper特性

▪简单：本质上是一个小型分布式文件系统，其数据结构就是znode树，对znode的读写是原子的。尽量保证单个znode在1MB一下，通常几K。

▪可靠：可以运行在一组服务器上，数据在每台server上都有备份，避免单点失效。

▪快速：数据全部加载到内存中，读写比例为10:1时性能最佳（读是局部性的，写需要由leader通知给所有server）。在更新内存中的znode之前会先序列到磁盘，服务重启后数据可以恢复。

▪松耦合：Watcher机制使得各客户端与服务器的交互变得松耦合，每个客户端无需知晓其他客户端的存在，就可以和其他客户端进行数据交互。

## 服务端配置

#  心跳间隔时间

tickTime=2000 

# 最小SessionTimeou 

minSessionTimeout=4000

# 最大SessionTimeou

maxSessionTimeout=100000

# 允许 follower （相对于 leader 而言的“客户端”）连接并同步到 leader 的初始化连接时间为tickTime的多少倍，超过这个时间则连接失败

initLimit=10

# eader 与 follower 之间发送消息，请求和应答时间长度。如果 follower 在设置的时间内不能与 leader 进行通信，那么此 follower 将被丢弃

syncLimit=5

# 客户端连接最大数

maxClientCnxns=30

## 客户端配置

ZooKeeper  zkp = new ZooKeeper("192.168.119.96:2181,   192.168.119.97:2181 , 192.168.119.98:2181 /app/learn", TIMEOUT,null));

▪client连接zookeeper连接的时候，最好指定>=2个zookeeper服务器地址,这样当一个zookeeper当掉后，client会自动failover到其它的连接。这时候default watcher会先收到一个Disconnected事件，连接的新的Zookeeper服务器后会收到一个SyncConnected事件。这种情况下session建立的临时节点不会丢失，所以程序一般不需要对这点做什么特别的工作。

▪如果你指定的zookeeper中有leader,它好像会直接连接到leader，如果是global集群或多机房集群，建议只指定本机房的zooleeper。

▪如果client连接指定的所有zookeeper server都当掉了，先还是会收到一个Disconnected事件，然后临时节点消失，启动zookeeper server后会收到Expired事件。


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

import java.io.IOException;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.data.Stat;

/**
 * 
 * @description ZooKeeper基本读写操作演示类
 * @author zhangchaoyang
 * @date 2014-6-22
 */
public class SimplestDemo {
    private static final int TIMEOUT = 3000;

    public static void main(String[] args) throws IOException, KeeperException,
            InterruptedException {
        // Client向zookeeper发送连接请求
        ZooKeeper zkp = new ZooKeeper("192.168.119.96:2181/app/learn", // 指定zookeeper
                                                                        // server的IP、端口列表（当client连接不上server时会按照此列表尝试连接下一台server），以及默认的根目录
                TIMEOUT,// Session Timeout
                null// 是否设置监听器
        );

        zkp.create("/znodename", // 节点名称
                "znodedata".getBytes(), // 节点上的数据
                Ids.OPEN_ACL_UNSAFE,// ACL
                CreateMode.EPHEMERAL// 节点类型，有三种：PERSISTENT、EPHEMERAL、SEQUENTIAL。EPHEMERAL节点不允许有子节点
        );

        Stat stat = zkp.exists("/znodename",// 节点名，如果节点不存在，则exists()返回null
                false// 是否设置监听器
                );
        if (zkp.exists("/znodename", false) != null) {
            System.out.println("znodename exists now.");
        }

        // 修改节点上存储的数据，需要提供version，version设为-1表示强制修改
        zkp.setData("/znodename", "newdata".getBytes(), stat.getVersion());

        // 读取节点上的数据
        String data = new String(zkp.getData("/znodename", false, stat));
        System.out.println(data);

        // client端主动断开连接
        zkp.close();
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

## Session


▪Zookeeper为每个client分配一个Session，Session数据在每台zookeeper节点上都有备份。

▪Server每隔tickTime向Client发送一个心跳。

▪session失效有两种可能性：client主动调用close() ；Server和Client 长时间（时间大于TIMEOUT ）失去联系。

▪Session失效后与Session关联的数据（比如EPHEMERAL节点，Watcher）会从内存中移除。

▪如果Server和Client只是短暂（时间小于TIMEOUT ）地失去联系，则Session不会失效，Client再次请求Zookeeper时会把上次的SessionID带上。

▪Server和Client失去联系有两种情况：Server宕机（Zookeeper集群会让Client与另一台Server建立连接）；网络异常。


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

import java.io.IOException;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.KeeperException.ConnectionLossException;
import org.apache.zookeeper.KeeperException.SessionExpiredException;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.data.Stat;
import org.apache.zookeeper.ZooKeeper;

/**
 * 
 * @description Zookeeper Session演示类
 * @author zhangchaoyang
 * @date 2014-6-22
 */
public class SessionDemo {
    /**
     * zoo.cfg中的配置：
     * 
     * <pre>
     * tickTime=2000
     * minSessionTimeout=4000（至少是tickTime的2倍）
     * maxSessionTimeout=40000（最大是tickTime的20倍）
     * </pre>
     * 
     * 如果客户端建立连接时指定的TIMEOUT不在[minSessionTimeout,maxSessionTimeout]区间内，
     * 服务端会强制把它修改到该区间内
     */
    private static final int TIMEOUT = 40000; // Session
                                                // Timeout设为40秒，因为心跳周期为2秒，所以如果server向client连续发送20个心跳都收不到回应，则Session过期失效
    private static ZooKeeper zkp = null;

    private static void connect() throws IOException {
        zkp = new ZooKeeper("192.168.119.96:2181/app/learn", TIMEOUT, null);
    }

    private static void createNode() throws KeeperException,
            InterruptedException {
        if (zkp != null) {
            zkp.create("/znodename", "znodedata".getBytes(),
                    Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
        }
    }

    private static String getData() throws KeeperException,
            InterruptedException {
        if (zkp != null) {
            Stat stat = zkp.exists("/znodename", false);
            return new String(zkp.getData("/znodename", false, stat));
        }
        return null;
    }

    private static void disconnect() throws InterruptedException {
        if (zkp != null) {
            zkp.close();
        }
    }

    /**
     * 休息，在此期间我们有三种选择：<br>
     * <ol>
     * <li>永久性断开网络连接
     * <li>断开网络连接一段时间timespan后再连上，其中timespan<{@code TIMEOUT}
     * <li>断开网络连接一段时间timespan后再连上，其中timespan>{@code TIMEOUT}
     * </ol>
     */
    private static void sleepForNetworkDisturbances() {
        try {
            Thread.sleep(2 * TIMEOUT);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        try {
            connect();
        } catch (IOException e) {
            System.err
                    .println("Can't create zookeeper client, please check the network.");
        }
        System.out.println("Session build.");

        try {
            createNode();
        } catch (Exception e) {
            System.err.println("Create znode failed.");
        }
        System.out.println("znode created.");

        sleepForNetworkDisturbances();

        try {
            String data = getData();
            if (data != null) {
                // 在“休息”期间做了第2件事情，Sesion没有过期，EPHEMERAL节点依然存在
                System.out.println("data=" + data);
            }
        } catch (KeeperException e) {
            e.printStackTrace();
            // 在“休息”期间做了第1件事情
            if (e instanceof ConnectionLossException) {
                System.err
                        .println("Oops, network is disconnected. Retry getData().");
                // 如果session没有失效，而仅仅是网络异常，则可以重新尝试获取数据，可能在重试时网络已经正常了
                try {
                    Thread.sleep(1000);
                    String data = getData();
                    if (data != null) {
                        System.out.println("data=" + data);
                    } else {
                        System.out.println("can't get data.");
                    }
                } catch (Exception e1) {
                    e1.printStackTrace();
                }
            }
            // 在“休息”期间做了第3件事情，则session会过期
            else if (e instanceof SessionExpiredException) {
                System.err
                        .println("Session Expired, client will reconnect and create znode again.");
                // 当发再Session Expired时，必须重新建立连接，即new一个ZooKeeper
                try {
                    connect();
                    createNode();
                    String data = getData();
                    if (data != null) {
                        System.out.println("data=" + data);
                    } else {
                        System.out.println("can't get data.");
                    }
                } catch (Exception e1) {
                    e1.printStackTrace();
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        try {
            disconnect();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("Client disconnected.");
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

## Watcher

▪可以注册watcher的方法：getData、exists、getChildren。

▪可以触发watcher的方法：create、delete、setData。连接断开的情况下触发的watcher会丢失。

▪一个Watcher实例是一个回调函数，被回调一次后就被移除了。如果还需要关注数据的变化，需要再次注册watcher。

▪New ZooKeeper时注册的watcher叫default watcher，它不是一次性的，只对client的连接状态变化作出反应。


什么样的操作会产生什么类型的事件：
||event For “/path”|event For “/path/child”|
|----|----|----|
|create(“/path”)|EventType.NodeCreated|--|
|delete(“/path”)|EventType.NodeDeleted|--|
|setData(“/path”)|EventType.NodeDataChanged|--|
|create(“/path/child”)|EventType.NodeChildrenChanged|EventType.NodeCreated|
|delete(“/path/child”)|EventType.NodeChildrenChanged|EventType.NodeDeleted|
|setData(“/path/child”)|--|EventType.NodeDataChanged|

 什么操作会触发EventType.None？

事件类型与watcher的对应关系：
|event For “/path”|DefaultWatcher|exists(“/path”)|getData(“/path”)|getChildren(“/path”)|
|----|----|----|----|----|
|EventType.None|√|√|√|√|
|EventType.NodeCreated||√|√||
|EventType.NodeDeleted||√|√||
|EventType.NodeDataChanged||√|√||
|EventType.NodeChildrenChanged||||√|

操作与watcher的对应关系：


||"/path"|"/path/child"| | | | |
|----|----|----|----|----|----|----|
||exists|getData|getChildren|exists|getData|getChildren|
|create(“/path”)|√|√|||||
|delete(“/path”)|√|√|√||||
|setData(“/path”)|√|√|||||
|create(“/path/child”)|||√|√|√||
|delete(“/path/child”)|||√|√|√|√|
|setData(“/path/child”)||||√|√||



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

import java.io.IOException;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.data.Stat;

/**
 * 
 * @description Zookeeper Watcher演示类
 * @author zhangchaoyang
 * @date 2014-6-22
 */
public class WatcherDemo {
    private static ZooKeeper zkp = null;
    private static final int TIMEOUT = 6000;

    private static Watcher getWatcher(final String msg) {
        return new Watcher() {
            @Override
            public void process(WatchedEvent event) {
                System.out.println(msg + "上的监听被触发\t事件类型" + event.getType()
                        + "\t发生变化的节点" + event.getPath());
            }
        };
    }

    public static void main(String[] args) throws IOException, KeeperException,
            InterruptedException {
        System.out.println("--------------1----------------");
        zkp = new ZooKeeper("192.168.119.96:2181/app/learn", TIMEOUT,
                getWatcher("CONNECT"));
        Thread.sleep(1000);
        
        System.out.println("--------------2----------------");
        zkp.create("/znodename", "znodedata".getBytes(), Ids.OPEN_ACL_UNSAFE,
                CreateMode.PERSISTENT);
        zkp.create("/znodename/childnode", new byte[0], Ids.OPEN_ACL_UNSAFE,
                CreateMode.PERSISTENT);

        Stat stat = zkp.exists("/znodename", getWatcher("EXISTS"));
        zkp.getChildren("/", getWatcher("GETCHILDREN"));
        zkp.getData("/znodename", getWatcher("GETDATA"), stat);

        stat = zkp.exists("/znodename/childnode", getWatcher("EXISTS"));
        zkp.getChildren("/znodename", getWatcher("GETCHILDREN"));
        zkp.getData("/znodename/childnode", getWatcher("GETDATA"), stat);

        // zkp.close();
        zkp = new ZooKeeper("192.168.119.96:2181/app/learn", TIMEOUT,
                getWatcher("CONNECT"));
        Thread.sleep(1000);

        System.out.println("--------------3----------------");
        zkp.delete("/znodename/childnode", -1);
        zkp.delete("/znodename", -1);
        zkp.close();
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

import java.io.IOException;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.KeeperException.ConnectionLossException;
import org.apache.zookeeper.KeeperException.SessionExpiredException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.EventType;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.data.Stat;

/**
 * 
 * @description 自定义持久性的zookeeper watcher
 * @author zhangchaoyang
 * @date 2014-6-22
 */
public class PersistWatcher {

    private static final int TIMEOUT = 6000;
    private static final String znode = "/globalconfnode";
    private static String globalConfData = "";

    private static Watcher getConnectWatcher() {
        return new Watcher() {
            @Override
            public void process(WatchedEvent event) {
                if (event.getType().equals(EventType.None)) {
                    System.out.println("连接状态发生变化。");
                }
            }
        };
    }

    private static Watcher getExistsWatcher(final ZooKeeper zkp) {
        return new Watcher() {
            @Override
            public void process(WatchedEvent event) {
                try {
                    if (event.getType().equals(EventType.NodeDataChanged)
                            || event.getType().equals(EventType.NodeCreated)) {
                        // 节点被创建或修改时更新缓存中的值
                        Stat stat = zkp.exists(znode, this);// 再次注册监听
                        String data = new String(
                                zkp.getData(znode, false, stat));
                        globalConfData = data;
                    } else if (event.getType().equals(EventType.NodeDeleted)) {
                        // 节点被删除时报警
                        System.out
                                .println("global configuration node have been deleted!");
                        try {
                            // 再次注册监听
                            zkp.exists(znode, this);
                        } catch (KeeperException e) {
                            if (e instanceof ConnectionLossException) {
                                System.out.println("连接已断开。");
                            }
                        }
                    }
                } catch (KeeperException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        };
    }

    public static void main(String[] args) {
        try {
            ZooKeeper zkp = new ZooKeeper("192.168.119.96:2181/app/learn",
                    TIMEOUT, 
                    getConnectWatcher());
            zkp.exists(znode, getExistsWatcher(zkp));
            zkp.create(znode, "config_value".getBytes(), Ids.OPEN_ACL_UNSAFE,
                    CreateMode.EPHEMERAL);

            Thread.sleep(500);// 修改节点后必须sleep，等待watcher回调完成
            System.out.println(globalConfData);

            for (int i = 0; i < 4; i++) {
                zkp.setData(znode, ("config_value" + i).getBytes(), -1);
                Thread.sleep(500);// 修改节点后必须sleep，等待watcher回调完成
                System.out.println(globalConfData);
            }

            zkp.close();// EPHEMERAL节点会被删除，但Session并不会马上失效(只不过ConnectionLoss了)，所以还是会触发watcher

            try {
                // 此时Session已失效
                zkp.exists(znode, false);
            } catch (KeeperException e) {
                if (e instanceof SessionExpiredException)
                    System.out.println("Session已失效。");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (KeeperException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

## ACL

▪每个节点有单独的ACL，子节点不能继承父节点的ACL

▪ACL有三个维度：schema,id,permision

▪Schema有7种：

▪world: 它下面只有一个id, 叫anyone

▪auth: 它不需要id

▪digest: 它对应的id为username:BASE64(SHA1(password))

▪ip: 它对应的id为客户机的IP地址，设置的时候可以设置一个ip段，比如ip:192.168.1.0/16, 表示匹配前16个bit的IP段

▪super: 在这种scheme情况下，对应的id拥有超级权限

▪sasl: sasl的对应的id，是一个通过了kerberos认证的用户id

▪Permission有5种:

▪CREATE(c),DELETE(d),READ(r),WRITE(w),ADMIN(a) 


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.KeeperException.NoAuthException;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.data.ACL;

/**
 * 
 * @description Zookeeper ACL演示类
 * @author zhangchaoyang
 * @date 2014-6-22
 */
public class AclDemo {
    private static final int TIMEOUT = 6000;

    public static void main(String[] args) throws IOException, KeeperException,
            InterruptedException {
        ZooKeeper zkp = new ZooKeeper("192.168.119.96:2181/app/learn", TIMEOUT,
                null);

        String schema = "digest";// schema类型有：world,auth,digest,ip,super
        String auth = "username:password";
        zkp.addAuthInfo(schema, auth.getBytes());

        List<ACL> acls = new ArrayList<ACL>();
        for (ACL id : Ids.CREATOR_ALL_ACL) {
            acls.add(id);
        }
        zkp.create("/znodename", "znodedata".getBytes(), acls,
                CreateMode.PERSISTENT);

        ZooKeeper zoo = null;
        try {
            zoo = new ZooKeeper("192.168.119.96:2181/app/learn", TIMEOUT, null);
            System.out.println("采用不合法的认证方式：");
            String badAuthentication = "username:wrongpass";
            zoo.addAuthInfo(schema, badAuthentication.getBytes());
            zoo.getData("/znodename", null, null);
        } catch (KeeperException e) {
            if (e instanceof NoAuthException) {
                System.out.println("认证失败：" + e.getMessage());
            }
            System.out.println("采用合法的认证方式：");
            zoo.addAuthInfo(schema, auth.getBytes());
            String data = new String(zoo.getData("/znodename", null, null));
            if (data != null) {
                System.out.println("认证成功：data=" + data);
            }
        } finally {
            if (zoo != null && zoo.getState().isAlive()) {
                zoo.close();
            }
        }

        zkp.delete("/znodename", -1);
        zkp.close();
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

## 开源工具menagerie

▪基于ZooKeeper实现了分布式的：

▪ReentrantZkLock

▪ReentrantZkReadWriteLock

▪Semaphore

▪CyclicBarrier

▪CountDownLatch

▪BlockingQueue

▪HashMap

▪ListSet


```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

import java.io.IOException;
import java.util.Date;
import java.util.List;
import java.util.concurrent.CountDownLatch;

import org.apache.log4j.Logger;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.Watcher.Event.KeeperState;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.ZooKeeper.States;

/**
 * 
 * @description 使用ZooKeeper实现分布式锁
 * @author zhangchaoyang
 * @date 2014-6-22
 */
public class ZooKeeperLock {

    private static Logger logger = Logger.getLogger(ZooKeeperLock.class);

    private static ZooKeeper zk = null;
    private static final int TIMEOUT = 1000 * 60;
    private static String connStr = null;

    public static void setServerPath(String path) {
        connStr = path + "/app/bqas/lock";
        logger.info("ZooKeeperLock zookeeper node:" + connStr);
    }

    public static boolean getLock(String lockname) throws KeeperException,
            InterruptedException, IOException {
        connect(connStr, TIMEOUT);
        if (lockname.contains("-")) {
            throw new RuntimeException("锁名称不能包含'-'");
        }
        boolean lock = false;
        String path = zk.create("/" + lockname + "-", new byte[0],
                Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL_SEQUENTIAL);
        int selfIndex = getIndex(path);
        List<String> children = zk.getChildren("/", false);
        int min = getMinIndex(children);
        if (min == selfIndex) {
            lock = true;
        }

        return lock;
    }

    public static void releaseLock(String lockname)
            throws InterruptedException, KeeperException {
        disconnect();
    }

    private static int getIndex(String str) {
        int index = -1;
        int pos = str.lastIndexOf("-");
        if (pos >= 0) {
            try {
                index = Integer.parseInt(str.substring(pos + 1));
            } catch (NumberFormatException e) {
                e.printStackTrace();
            }
        }
        return index;
    }

    private static int getMinIndex(List<String> list) {
        int min = Integer.MAX_VALUE;
        for (String ele : list) {
            int index = getIndex(ele);
            if (index < 0) {
                throw new RuntimeException("SEQUENTIAL节点名中不包含数字：" + ele);
            }
            if (index < min) {
                min = index;
            }
        }
        return min;
    }

    private static void waitUntilConnected(CountDownLatch connectedLatch) {
        if (States.CONNECTING == zk.getState()) {
            try {
                connectedLatch.await();
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
        }
    }

    public static boolean connect(String hostPath, int sessionTimeout) {
        if (zk == null || zk.getState() == States.CLOSED) {
            try {
                CountDownLatch connectedLatch = new CountDownLatch(1);
                Watcher watcher = new ConnectedWatcher(connectedLatch);
                zk = new ZooKeeper(hostPath, sessionTimeout, watcher);
                waitUntilConnected(connectedLatch);
            } catch (Exception e) {
                logger.error("Connect to Zookeeper failed:", e);
                return false;
            }
        }
        return true;
    }

    public static boolean disconnect() {
        if (zk != null) {
            if (States.CLOSED != zk.getState()) {
                try {
                    zk.close();
                } catch (InterruptedException e) {
                    logger.error("Disconnect from Zookeeper failed:", e);
                    return false;
                }
            }
        }
        return true;
    }

    static class ConnectedWatcher implements Watcher {

        private CountDownLatch connectedLatch;

        ConnectedWatcher(CountDownLatch connectedLatch) {
            this.connectedLatch = connectedLatch;
        }

        @Override
        public void process(WatchedEvent event) {
            // 事件状态为SyncConnected时，说明与服务端的连接已建立好
            if (event.getState() == KeeperState.SyncConnected) {
                connectedLatch.countDown();
            }
        }
    }

    public static void main(String[] args) {
        String lockname = "writeHitCount2DBlock";
        System.out.println("begin to run.");
        ZooKeeperLock.setServerPath("192.168.119.96:2181");
        try {
            boolean havelock = ZooKeeperLock.getLock(lockname);
            if (havelock) {
                Date date = new Date();
                System.out
                        .println("I got the lock,and I will write DB!" + date);
                Thread.sleep(1000);// 休息一段时间之后再释放锁
            }
            System.out.println("Job done, I will release the lock.");
            ZooKeeperLock.releaseLock(lockname);
        } catch (KeeperException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```




















