# ZooKeeper编程(一) - z69183787的专栏 - CSDN博客
2016年08月31日 11:52:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：405
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
ZooKeeper的用途：distributed coordination;maintaining configuration information, naming, providing distributed synchronization, and providing group services.
Zookeeper的节点都是存放在内存中的，所以读写速度很快。更新日志被记录到了磁盘中，以便用于恢复数据。在更新内在中节点数之前，会先序列化到磁盘中。
为避免单点失效，zookeeper的数据是在多个server上留有备份的。不管客户端连接到的是哪个server，它看到的数据都是一致的。如果client和一个server的TCP连接失效，它会尝试连接另一个server。众多server中有一个是leader。
所有的server 都必须知道彼此的存在。
zookeeper在读写比例为10:1时性能最佳。
每个znode上data的读写都是原子操作。
读是局部性的，即client只需要从与它相连的server上读取数据即可；而client有写请求的话，与之相连的server会通知leader，然后leader会把写操作分发给所有server。所以定要比读慢很多。
在建立zookeeper连接时，给定的地址字符串可以是这样的："192.168.1.1:3000,192.168.1.2:3000,192.168.1.3:3000/app/a"，以后的所有操作就都是在/app/a下进行的。实际上只连接到一台ZooKeeper机器就可了，没必要指定每台zk机器的IP和端口，即用“192.168.1.2:3000/app/a”也是可以的。
当client与一个server断连接时（可能是因为server失效了），它就收不到任何watches；当它与另一个server建立好连接后，它就会收到"session expired"通知。
ACL不是递归的，它只针对当前节点，对子节点没有任何影响。
默认情况下日志文件和数据文件是放在同一个目录下的，为缩短延迟提高响应性，你可以把日志文件单独放在另一个目录下。
为避免swaping，运行java时最好把可用物理内在调得大一些，比如对于4G的内在，可以把它调到3G。java有以下两个运行参数：
-Xms<size>
设置虚拟机可用内存堆的初始大小，缺省单位为字节，该大小为1024的整数倍并且要大于1MB，可用k(K)或m(M)为单位来设置较大的内存数。初始堆大小为2MB。
例如：-Xms6400K，-Xms256M
-Xmx<size>
设置虚拟机内存堆的最大可用大小，缺省单位为字节。该值必须为1024整数倍，并且要大于2MB。可用k(K)或m(M)为单位来设置较大的内存数。缺省堆最大值为64MB。
例如：-Xmx81920K，-Xmx80M
**CreateMode**
PERSISTENT：创建后只要不删就永久存在
EPHEMERAL：会话结束年结点自动被删除，EPHEMERAL结点不允许有子节点
SEQUENTIAL：节点名末尾会自动追加一个10位数的单调递增的序号，同一个节点的所有子节点序号是单调递增的
PERSISTENT_SEQUENTIAL：结合PERSISTENT和SEQUENTIAL
EPHEMERAL_SEQUENTIAL：结合EPHEMERAL和SEQUENTIAL
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package basic;
import java.io.IOException;
import java.util.List;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.ZooDefs.Ids;
public class Demo {
    private static final int TIMEOUT = 3000;
    public static void main(String[] args) throws IOException {
        ZooKeeper zkp = new ZooKeeper("localhost:2181", TIMEOUT, null);
        try {
            // 创建一个EPHEMERAL类型的节点，会话关闭后它会自动被删除
            zkp.create("/node1", "data1".getBytes(), Ids.OPEN_ACL_UNSAFE,CreateMode.EPHEMERAL);
            if (zkp.exists("/node1", false) != null) {
                System.out.println("node1 exists now.");
            }
            try {
                // 当节点名已存在时再去创建它会抛出KeeperException(即使本次的ACL、CreateMode和上次的不一样)
                zkp.create("/node1", "data1".getBytes(), Ids.OPEN_ACL_UNSAFE,CreateMode.PERSISTENT);
            } catch (KeeperException e) {
                System.out.println("KeeperException caught:" + e.getMessage());
            }
            // 关闭会话
            zkp.close();
            
            zkp = new ZooKeeper("localhost:2181", TIMEOUT, null);
            //重新建立会话后node1已经不存在了
            if (zkp.exists("/node1", false) == null) {
                System.out.println("node1 dosn't exists now.");
            }
            //创建SEQUENTIAL节点
            zkp.create("/node-", "same data".getBytes(), Ids.OPEN_ACL_UNSAFE,CreateMode.PERSISTENT_SEQUENTIAL);
            zkp.create("/node-", "same data".getBytes(), Ids.OPEN_ACL_UNSAFE,CreateMode.PERSISTENT_SEQUENTIAL);
            zkp.create("/node-", "same data".getBytes(), Ids.OPEN_ACL_UNSAFE,CreateMode.PERSISTENT_SEQUENTIAL);
            List<String> children = zkp.getChildren("/", null);
            System.out.println("Children of root node:");
            for (String child : children) {
                System.out.println(child);
            }
            zkp.close();
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
第一次运行输出：
node1 exists now.
KeeperException caught:KeeperErrorCode = NodeExists for /node1
node1 dosn't exists now.
Children of root node:
node-0000000003
zookeeper
node-0000000002
node-0000000001
第二次运行输出：
node1 exists now.
KeeperException caught:KeeperErrorCode = NodeExists for /node1
node1 dosn't exists now.
Children of root node:
node-0000000003
zookeeper
node-0000000002
node-0000000001
node-0000000007
node-0000000005
node-0000000006
注意两次会话中创建的PERSISTENT_SEQUENTIAL节点序号并不是连续的，比如上例中缺少了node-0000000004.
**Watcher & Version**
watcher分为两大类：data watches和child watches。getData()和exists()上可以设置data watches，getChildren()上可以设置child watches。
setData()会触发data watches;
create()会触发data watches和child watches;
delete()会触发data watches和child watches.
如果对一个不存在的节点调用了exists()，并设置了watcher，而在连接断开的情况下create/delete了该znode，则watcher会丢失。
在server端用一个map来存放watcher，所以相同的watcher在map中只会出现一次，只要watcher被回调一次，它就会被删除----map解释了watcher的一次性。比如如果在getData()和exists()上设置的是同一个data watcher，调用setData()会触发data watcher，但是getData()和exists()只有一个会收到通知。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 import java.io.IOException;
 2 
 3 import org.apache.zookeeper.CreateMode;
 4 import org.apache.zookeeper.KeeperException;
 5 import org.apache.zookeeper.WatchedEvent;
 6 import org.apache.zookeeper.Watcher;
 7 import org.apache.zookeeper.ZooDefs.Ids;
 8 import org.apache.zookeeper.ZooKeeper;
 9 import org.apache.zookeeper.data.Stat;
10 
11 public class SelfWatcher implements Watcher{
12     
13     ZooKeeper zk=null;
14 
15     @Override
16     public void process(WatchedEvent event) {
17         System.out.println(event.toString());
18     }
19     
20     SelfWatcher(String address){
21         try{
22             zk=new ZooKeeper(address,3000,this);     //在创建ZooKeeper时第三个参数负责设置该类的默认构造函数
23             zk.create("/root", new byte[0], Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
24         }catch(IOException e){
25             e.printStackTrace();
26             zk=null;
27         }catch (KeeperException e) {
28             e.printStackTrace();
29         } catch (InterruptedException e) {
30             e.printStackTrace();
31         }
32     }
33     
34     void setWatcher(){
35         try {
36             Stat s=zk.exists("/root", true);
37             if(s!=null){
38                 zk.getData("/root", false, s);
39             }
40         } catch (KeeperException e) {
41             e.printStackTrace();
42         } catch (InterruptedException e) {
43             e.printStackTrace();
44         }
45     }
46     
47     void trigeWatcher(){
48         try {
49             Stat s=zk.exists("/root", false);        //此处不设置watcher
50             zk.setData("/root", "a".getBytes(), s.getVersion());　　　　//修改数据时需要提供version，version设为-1表示强制修改
51         }catch(Exception e){
52             e.printStackTrace();
53         }
54     }
55     
56     void disconnect(){
57         if(zk!=null)
58             try {
59                 zk.close();
60             } catch (InterruptedException e) {
61                 e.printStackTrace();
62             }
63     }
64     
65     public static void main(String[] args){
66         SelfWatcher inst=new SelfWatcher("127.0.0.1:2181");
67         inst.setWatcher();
68         inst.trigeWatcher();
69         inst.disconnect();
70     }
71 
72 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
可以在创建Zookeeper时指定默认的watcher回调函数，这样在getData()、exists()和getChildren()收到通知时都会调用这个函数--只要它们在参数中设置了true。所以如果把代码22行的this改为null，则不会有任何watcher被注册。
上面的代码输出：
WatchedEvent state:SyncConnected type:None path:null
WatchedEvent state:SyncConnected type:NodeDataChanged path:/root
之所会输出第1 行是因为本身在建立ZooKeeper连接时就会触发watcher。输出每二行是因为在代码的第36行设置了true。
WatchEvent有三种类型：NodeDataChanged、NodeDeleted和NodeChildrenChanged。
调用setData()时会触发NodeDataChanged;
调用create()时会触发NodeDataChanged和NodeChildrenChanged;
调用delete()时上述三种event都会触发。
如果把代码的第36--39行改为：
```
Stat s=zk.exists("/root", false);
if(s!=null){
    zk.getData("/root", true, s);
}
```
或
```
Stat s=zk.exists("/root", true);
if(s!=null){
    zk.getData("/root", true, s);
}
```
跟上面的输出是一样的。这也证明了watcher是一次性的。
设置watcher的另外一种方式是不使用默认的watcher，而是在getData()、exists()和getChildren()中指定各自的watcher。示例代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 public class SelfWatcher{
 2     
 3     ZooKeeper zk=null;
 4 
 5     private Watcher getWatcher(final String msg){
 6         return new Watcher(){
 7             @Override
 8             public void process(WatchedEvent event) {
 9                 System.out.println(msg+"\t"+event.toString());
10             }
11         };
12     }
13     
14     SelfWatcher(String address){
15         try{
16             zk=new ZooKeeper(address,3000,null);     //在创建ZooKeeper时第三个参数负责设置该类的默认构造函数
17             zk.create("/root", new byte[0], Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
18         }catch(IOException e){
19             e.printStackTrace();
20             zk=null;
21         }catch (KeeperException e) {
22             e.printStackTrace();
23         } catch (InterruptedException e) {
24             e.printStackTrace();
25         }
26     }
27     
28     void setWatcher(){
29         try {
30             Stat s=zk.exists("/root", getWatcher("EXISTS"));
31             if(s!=null){
32                 zk.getData("/root", getWatcher("GETDATA"), s);
33             }
34         } catch (KeeperException e) {
35             e.printStackTrace();
36         } catch (InterruptedException e) {
37             e.printStackTrace();
38         }
39     }
40     
41     void trigeWatcher(){
42         try {
43             Stat s=zk.exists("/root", false);        //此处不设置watcher
44             zk.setData("/root", "a".getBytes(), s.getVersion());
45         }catch(Exception e){
46             e.printStackTrace();
47         }
48     }
49     
50     void disconnect(){
51         if(zk!=null)
52             try {
53                 zk.close();
54             } catch (InterruptedException e) {
55                 e.printStackTrace();
56             }
57     }
58     
59     public static void main(String[] args){
60         SelfWatcher inst=new SelfWatcher("127.0.0.1:2181");
61         inst.setWatcher();
62         inst.trigeWatcher();
63         inst.disconnect();
64     }
65 
66 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
输出：
GETDATA WatchedEvent state:SyncConnected type:NodeDataChanged path:/root
EXISTS WatchedEvent state:SyncConnected type:NodeDataChanged path:/root
上例中由于exists和getData分别设置了两个不同的Watcher实例，所以虽然watcher都是由同了一个NodeDataChanged触发的，但exists()和getData()都会收到通知。由于16行创建Zookeeper时没有设置watcher(参数为null)，所以建立连接时没有收到通知。
关于Version:为了方便进行cache validations 和coordinated updates，每个znode都有一个stat结构体，其中包含：version的更改记录、ACL的更改记录、时间戳。znode的数据每更改一次，version就会加1。客户端每次检索data的时候都会把data的version一并读出出来。修改数据时需要提供version。
```
zk.delete("/root", -1);        //触发data watches和children watches。version设为-1时表示要强制删除
zk.getChildren("/root", getWatcher("LISTCHILDREN"));　　　　//getChildren()上可以设置children watches
```
输出：
LISTCHILDREN WatchedEvent state:SyncConnected type:NodeDeleted path:/root
```
zk.delete("/root", -1);        //触发data watches和children watches
Stat s=zk.exists("/root", getWatcher("EXISTS"));　　　　//exists()上可以设置data watches
if(s!=null){
    zk.getChildren("/root", getWatcher("LISTCHILDREN"));
}
```
输出：
EXISTS WatchedEvent state:SyncConnected type:NodeDeleted path:/root
LISTCHILDREN WatchedEvent state:SyncConnected type:NodeDeleted path:/root
```
zk.delete("/root", -1);        //触发data watches和children watches
Stat s=zk.exists("/root", getWatcher("EXISTS"));
if(s!=null){
    zk.getData("/root", getWatcher("GETDATA"), s);
    zk.getChildren("/root", getWatcher("LISTCHILDREN"));
}
```
输出：
GETDATA WatchedEvent state:SyncConnected type:NodeDeleted path:/root
LISTCHILDREN WatchedEvent state:SyncConnected type:NodeDeleted path:/root
EXISTS WatchedEvent state:SyncConnected type:NodeDeleted path:/root
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
tat s=zk.exists("/root", false);        
zk.setData("/root", "a".getBytes(), s.getVersion());        
zk.delete("/root", -1);        
Stat s=zk.exists("/root", getWatcher("EXISTS"));
if(s!=null){
    zk.getData("/root", getWatcher("GETDATA"), s);
    zk.getChildren("/root", getWatcher("LISTCHILDREN"));
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
输出：
GETDATA WatchedEvent state:SyncConnected type:NodeDataChanged path:/root
EXISTS WatchedEvent state:SyncConnected type:NodeDataChanged path:/root
LISTCHILDREN WatchedEvent state:SyncConnected type:NodeDeleted path:/root
按说data watches触发了两次，但是exists()和getData()只会收到一次通知。
### Barriers and Queues
Barrier是指：
1）所有的线程都到达barrier后才能进行后续的计算
或者
2）所有的线程都完成自己的计算后才能离开barrier
Double Barrier是指同时具有上述两点。
Queue就不说了，一个产生--消费模型，先生产的先被消费。
Double Barrier的实现：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
enter barrier:
1.建一个根节点"/root"
2.想进入barrier的线程在"/root"下建立一个子节点"/root/c_i"
3.循环监听"/root"孩子节点数目的变化，当其达到size时就说明有size个线程都已经barrier点了
leave barrier:
1.想离开barrier的线程删除其在"/root"下建立的子节点
2.循环监听"/root"孩子节点数目的变化，当size减到0时它就可以离开barrier了
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Queue的实现：
```
1.建立一个根节点"/root"
2.生产线程在"/root"下建立一个SEQUENTIAL子节点
3.消费线程检查"/root"有没有子节点，如果没有就循环监听"/root"子节点的变化，直到它有子节点。删除序号最小的子节点。
```
原代码：
```java
package sync;
 
import java.io.IOException;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.util.List;
import java.util.Random;
 
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.ZooDefs.Ids;
import org.apache.zookeeper.data.Stat;
 
public class SyncPrimitive implements Watcher {
    static ZooKeeper zk = null;
    static Integer mutex;
    String root;
 
    //同步原语
    SyncPrimitive(String address) {
        if (zk == null) {
            try {
                System.out.println("Starting ZK:");
                //建立Zookeeper连接，并且指定watcher
                zk = new ZooKeeper(address, 3000, this);
                //初始化锁对象
                mutex = new Integer(-1);
                System.out.println("Finished starting ZK:" + zk);
            } catch (IOException e) {
                System.out.println(e.toString());
                zk = null;
            }
        }
    }
 
    @Override
    synchronized public void process(WatchedEvent event) {
        synchronized (mutex) {
            //有事件发生时，调用notify，使其他wait()点得以继续
            mutex.notify();
        }
 
    }
 
    static public class Barrier extends SyncPrimitive {
        int size;
        String name;
 
        Barrier(String address, String root, int size) {
            super(address);
            this.root = root;
            this.size = size;
            if (zk != null) {
                try {
                    //一个barrier建立一个根目录
                    Stat s = zk.exists(root, false);        //不注册watcher
                    if (s == null) {
                        zk.create(root, new byte[0], Ids.OPEN_ACL_UNSAFE,
                                CreateMode.PERSISTENT);
                    }
                } catch (KeeperException e) {
                    System.out
                            .println("keeper exception when instantiating queue:"
                                    + e.toString());
                } catch (InterruptedException e) {
                    System.out.println("Interrupted exception.");
                }
            }
            try {
                //获取自己的主机名
                name = new String(InetAddress.getLocalHost()
                        .getCanonicalHostName().toString());
            } catch (UnknownHostException e) {
                System.out.println(e.toString());
            }
        }
 
        boolean enter() throws KeeperException, InterruptedException {
            //在根目录下创建一个子节点.create和delete都会触发children wathes,这样getChildren就会收到通知，process()就会被调用
            zk.create(root + "/" + name, new byte[0], Ids.OPEN_ACL_UNSAFE,
                    CreateMode.EPHEMERAL_SEQUENTIAL);
            //一直等，直到根目录下的子节点数目达到size时，函数退出
            while (true) {
                synchronized (mutex) {
                    List<String> list = zk.getChildren(root, true);
                    if (list.size() < size) {
                        mutex.wait();       //释放mutex上的锁
                    } else {
                        return true;
                    }
                }
            }
        }
 
        boolean leave() throws KeeperException, InterruptedException {
            //删除自己创建的节点
            zk.delete(root + "/" + name, 0);
            //一直等，直到根目录下有子节点时，函数退出
            while (true) {
                synchronized (mutex) {
                    List<String> list = zk.getChildren(root, true);
                    if (list.size() > 0) {
                        mutex.wait();
                    } else {
                        return true;
                    }
                }
            }
        }
 
    }
 
    static public class Queue extends SyncPrimitive {
        Queue(String address, String name) {
            super(address);
            this.root = name;
            if (zk != null) {
                try {
                    //一个queue建立一个根目录
                    Stat s = zk.exists(root, false);
                    if (s == null) {
                        zk.create(root, new byte[0], Ids.OPEN_ACL_UNSAFE,
                                CreateMode.PERSISTENT);
                    }
                } catch (KeeperException e) {
                    System.out
                            .println("keeper exception when instantiating queue:"
                                    + e.toString());
                } catch (InterruptedException e) {
                    System.out.println("Interrupted exception.");
                }
            }
        }
 
        //参数i是要创建节点的data
        boolean produce(int i) throws KeeperException, InterruptedException {
            ByteBuffer b = ByteBuffer.allocate(4);
            byte[] value;
            b.putInt(i);
            value = b.array();
 
            //根目录下创建一个子节点，因为是SEQUENTIAL的，所以先创建的节点具有较小的序号
            zk.create(root + "/element", value, Ids.OPEN_ACL_UNSAFE,
                    CreateMode.PERSISTENT_SEQUENTIAL);
            return true;
        }
 
        int consume() throws KeeperException, InterruptedException {
            int retvalue = -1;
            Stat stat = null;
            while (true) {
                synchronized (mutex) {
                    List<String> list = zk.getChildren(root, true);       //并不能保证list[0]就是序号最小的
                    //如果根目录下没有子节点就一直等
                    if (list.size() == 0) {
                        System.out.println("Going to wait");
                        mutex.wait();
                    }
                    //找到序号最小的节点将其删除
                    else {
                        Integer min = new Integer(list.get(0).substring(7));
                        for (String s : list) {
                            Integer tmp = new Integer(s.substring(7));
                            if (tmp < min)
                                min = tmp;
                        }
                        System.out.println("Temporary value:" + root
                                + "/element" + min);
                        byte[] b = zk.getData(root + "/element" + min, false,
                                stat);
                        zk.delete(root + "/element" + min, 0);
                        ByteBuffer buffer = ByteBuffer.wrap(b);
                        retvalue = buffer.getInt();
                        return retvalue;
                    }
                }
            }
        }
    }
 
    public static void main(String[] args) {
        if (args[0].equals("qTest"))
            queueTest(args);
        else
            barrierTest(args);
    }
 
    private static void barrierTest(String[] args) {
        Barrier b = new Barrier(args[1], "/b1", new Integer(args[2]));
        try {
            boolean flag = b.enter();
            System.out.println("Enter barrier:" + args[2]);
            if (!flag)
                System.out.println("Error when entering the barrier");
        } catch (KeeperException e) {
 
        } catch (InterruptedException e) {
        }
 
        Random rand = new Random();
        int r = rand.nextInt(100);
        for (int i = 0; i < r; i++) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
 
            }
        }
        try {
            b.leave();
        } catch (KeeperException e) {
 
        } catch (InterruptedException e) {
        }
        System.out.println("Left barrier");
    }
 
    private static void queueTest(String[] args) {
        Queue q = new Queue(args[1], "/app1");
        System.out.println("Input:" + args[1]);
        int i;
        Integer max = new Integer(args[2]);
 
        if (args[3].equals("p")) {
            System.out.println("Producer");
            for (i = 0; i < max; i++)
                try {
                    q.produce(10 + 1);
                } catch (KeeperException e) {
 
                } catch (InterruptedException e) {
                }
        } else {
            System.out.println("Consumer");
            for (i = 0; i < max; i++)
                try {
                    int r = q.consume();
                    System.out.println("Item:" + r);
                } catch (KeeperException e) {
                    i--;
                } catch (InterruptedException e) {
                }
        }
    }
 
}
```
Locks
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
获得锁：
1.创建根节点"/root"
2.在根节点下新建子节点"/root/c-xxxxxx"，SEQUENTIAL模式
3.对根节点调用getChildren()，如果第2步创建的节点是所有子节点中序号最小的，则获得锁；否则进入第4步
4.在序号最小的子节点上调用exists()，当序号最小的子节点被删除后返回第3步
释放锁：
删除自己创建的子节点即可
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
原代码：
[+
 View Code](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html#)
读锁（共享锁）和写锁（排斥锁）并存的情况跟单独只有排斥锁的情况有几点不同：
1.当一个线程想施加读锁时就新建一个节点"/root/read-xxxxxx"，施加写锁时就新建一个节点"/root/write-xxxxxx";
2.欲施加读锁的线程查看"/root"下有没有“write"开头的节点，如果没有则直接获得读锁；如果有，但是"write"节点的序号比自己刚才创建的"read"节点的序号要大说明是先施加的读锁后施加的写锁，所以依然获得读锁；else，在序号最小的"write"节点上调用exists，等待它被删除。
