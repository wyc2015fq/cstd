# ZooKeeper编程(一) - 零度的博客专栏 - CSDN博客
2016年10月08日 11:19:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：308
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
[]()
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
[]()
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
[]()
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
[]()
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
[]()
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
[]()
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
[]()
zk.delete("/root", -1);        //触发data watches和children watches
Stat s=zk.exists("/root", getWatcher("EXISTS"));
if(s!=null){
    zk.getData("/root", getWatcher("GETDATA"), s);
    zk.getChildren("/root", getWatcher("LISTCHILDREN"));
}
[]()
输出：
GETDATA WatchedEvent state:SyncConnected type:NodeDeleted path:/root
LISTCHILDREN WatchedEvent state:SyncConnected type:NodeDeleted path:/root
EXISTS WatchedEvent state:SyncConnected type:NodeDeleted path:/root
```
[]()
tat s=zk.exists("/root", false);        
zk.setData("/root", "a".getBytes(), s.getVersion());        
zk.delete("/root", -1);        
Stat s=zk.exists("/root", getWatcher("EXISTS"));
if(s!=null){
    zk.getData("/root", getWatcher("GETDATA"), s);
    zk.getChildren("/root", getWatcher("LISTCHILDREN"));
}
[]()
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
[]()
enter barrier:
1.建一个根节点"/root"
2.想进入barrier的线程在"/root"下建立一个子节点"/root/c_i"
3.循环监听"/root"孩子节点数目的变化，当其达到size时就说明有size个线程都已经barrier点了
leave barrier:
1.想离开barrier的线程删除其在"/root"下建立的子节点
2.循环监听"/root"孩子节点数目的变化，当size减到0时它就可以离开barrier了
[]()
Queue的实现：
```
1.建立一个根节点"/root"
2.生产线程在"/root"下建立一个SEQUENTIAL子节点
3.消费线程检查"/root"有没有子节点，如果没有就循环监听"/root"子节点的变化，直到它有子节点。删除序号最小的子节点。
```
原代码：
[+ View Code](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html#)[?](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html#)
```java
package
```
```java
sync;
```
```java
import
```
```java
java.io.IOException;
```
```java
import
```
```java
java.net.InetAddress;
```
```java
import
```
```java
java.net.UnknownHostException;
```
```java
import
```
```java
java.nio.ByteBuffer;
```
```java
import
```
```java
java.util.List;
```
```java
import
```
```java
java.util.Random;
```
```java
import
```
```java
org.apache.zookeeper.CreateMode;
```
```java
import
```
```java
org.apache.zookeeper.KeeperException;
```
```java
import
```
```java
org.apache.zookeeper.WatchedEvent;
```
```java
import
```
```java
org.apache.zookeeper.Watcher;
```
```java
import
```
```java
org.apache.zookeeper.ZooKeeper;
```
```java
import
```
```java
org.apache.zookeeper.ZooDefs.Ids;
```
```java
import
```
```java
org.apache.zookeeper.data.Stat;
```
```java
public
```
```java
class
```
```java
SyncPrimitive
```
```java
implements
```
```java
Watcher {
```
```java
```
```java
static
```
```java
ZooKeeper zk =
```
```java
null
```
```java
;
```
```java
```
```java
static
```
```java
Integer mutex;
```
```java
```
```java
String root;
```
```java
```
```java
//同步原语
```
```java
```
```java
SyncPrimitive(String address) {
```
```java
```
```java
if
```
```java
(zk ==
```
```java
null
```
```java
) {
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
System.out.println(
```
```java
"Starting ZK:"
```
```java
);
```
```java
```
```java
//建立Zookeeper连接，并且指定watcher
```
```java
```
```java
zk =
```
```java
new
```
```java
ZooKeeper(address,
```
```java
3000
```
```java
,
```
```java
this
```
```java
);
```
```java
```
```java
//初始化锁对象
```
```java
```
```java
mutex =
```
```java
new
```
```java
Integer(-
```
```java
1
```
```java
);
```
```java
```
```java
System.out.println(
```
```java
"Finished starting ZK:"
```
```java
+ zk);
```
```java
```
```java
}
```
```java
catch
```
```java
(IOException e) {
```
```java
```
```java
System.out.println(e.toString());
```
```java
```
```java
zk =
```
```java
null
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
@Override
```
```java
```
```java
synchronized
```
```java
public
```
```java
void
```
```java
process(WatchedEvent event) {
```
```java
```
```java
synchronized
```
```java
(mutex) {
```
```java
```
```java
//有事件发生时，调用notify，使其他wait()点得以继续
```
```java
```
```java
mutex.notify();
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
static
```
```java
public
```
```java
class
```
```java
Barrier
```
```java
extends
```
```java
SyncPrimitive {
```
```java
```
```java
int
```
```java
size;
```
```java
```
```java
String name;
```
```java
```
```java
Barrier(String address, String root,
```
```java
int
```
```java
size) {
```
```java
```
```java
super
```
```java
(address);
```
```java
```
```java
this
```
```java
.root = root;
```
```java
```
```java
this
```
```java
.size = size;
```
```java
```
```java
if
```
```java
(zk !=
```
```java
null
```
```java
) {
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
//一个barrier建立一个根目录
```
```java
```
```java
Stat s = zk.exists(root,
```
```java
false
```
```java
);
```
```java
//不注册watcher
```
```java
```
```java
if
```
```java
(s ==
```
```java
null
```
```java
) {
```
```java
```
```java
zk.create(root,
```
```java
new
```
```java
byte
```
```java
[
```
```java
0
```
```java
], Ids.OPEN_ACL_UNSAFE,
```
```java
```
```java
CreateMode.PERSISTENT);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
System.out
```
```java
```
```java
.println(
```
```java
"keeper exception when instantiating
 queue:"
```
```java
```
```java
+ e.toString());
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
System.out.println(
```
```java
"Interrupted exception."
```
```java
);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
//获取自己的主机名
```
```java
```
```java
name =
```
```java
new
```
```java
String(InetAddress.getLocalHost()
```
```java
```
```java
.getCanonicalHostName().toString());
```
```java
```
```java
}
```
```java
catch
```
```java
(UnknownHostException e) {
```
```java
```
```java
System.out.println(e.toString());
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
boolean
```
```java
enter()
```
```java
throws
```
```java
KeeperException, InterruptedException {
```
```java
```
```java
//在根目录下创建一个子节点.create和delete都会触发children wathes,这样getChildren就会收到通知，process()就会被调用
```
```java
```
```java
zk.create(root +
```
```java
"/"
```
```java
+ name,
```
```java
new
```
```java
byte
```
```java
[
```
```java
0
```
```java
], Ids.OPEN_ACL_UNSAFE,
```
```java
```
```java
CreateMode.EPHEMERAL_SEQUENTIAL);
```
```java
```
```java
//一直等，直到根目录下的子节点数目达到size时，函数退出
```
```java
```
```java
while
```
```java
(
```
```java
true
```
```java
) {
```
```java
```
```java
synchronized
```
```java
(mutex) {
```
```java
```
```java
List<String> list = zk.getChildren(root,
```
```java
true
```
```java
);
```
```java
```
```java
if
```
```java
(list.size() < size) {
```
```java
```
```java
mutex.wait();
```
```java
//释放mutex上的锁
```
```java
```
```java
}
```
```java
else
```
```java
{
```
```java
```
```java
return
```
```java
true
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
boolean
```
```java
leave()
```
```java
throws
```
```java
KeeperException, InterruptedException {
```
```java
```
```java
//删除自己创建的节点
```
```java
```
```java
zk.delete(root +
```
```java
"/"
```
```java
+ name,
```
```java
0
```
```java
);
```
```java
```
```java
//一直等，直到根目录下有子节点时，函数退出
```
```java
```
```java
while
```
```java
(
```
```java
true
```
```java
) {
```
```java
```
```java
synchronized
```
```java
(mutex) {
```
```java
```
```java
List<String> list = zk.getChildren(root,
```
```java
true
```
```java
);
```
```java
```
```java
if
```
```java
(list.size() >
```
```java
0
```
```java
) {
```
```java
```
```java
mutex.wait();
```
```java
```
```java
}
```
```java
else
```
```java
{
```
```java
```
```java
return
```
```java
true
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
static
```
```java
public
```
```java
class
```
```java
Queue
```
```java
extends
```
```java
SyncPrimitive {
```
```java
```
```java
Queue(String address, String name) {
```
```java
```
```java
super
```
```java
(address);
```
```java
```
```java
this
```
```java
.root = name;
```
```java
```
```java
if
```
```java
(zk !=
```
```java
null
```
```java
) {
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
//一个queue建立一个根目录
```
```java
```
```java
Stat s = zk.exists(root,
```
```java
false
```
```java
);
```
```java
```
```java
if
```
```java
(s ==
```
```java
null
```
```java
) {
```
```java
```
```java
zk.create(root,
```
```java
new
```
```java
byte
```
```java
[
```
```java
0
```
```java
], Ids.OPEN_ACL_UNSAFE,
```
```java
```
```java
CreateMode.PERSISTENT);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
System.out
```
```java
```
```java
.println(
```
```java
"keeper exception when instantiating
 queue:"
```
```java
```
```java
+ e.toString());
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
System.out.println(
```
```java
"Interrupted exception."
```
```java
);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
//参数i是要创建节点的data
```
```java
```
```java
boolean
```
```java
produce(
```
```java
int
```
```java
i)
```
```java
throws
```
```java
KeeperException, InterruptedException {
```
```java
```
```java
ByteBuffer b = ByteBuffer.allocate(
```
```java
4
```
```java
);
```
```java
```
```java
byte
```
```java
[] value;
```
```java
```
```java
b.putInt(i);
```
```java
```
```java
value = b.array();
```
```java
```
```java
//根目录下创建一个子节点，因为是SEQUENTIAL的，所以先创建的节点具有较小的序号
```
```java
```
```java
zk.create(root +
```
```java
"/element"
```
```java
, value, Ids.OPEN_ACL_UNSAFE,
```
```java
```
```java
CreateMode.PERSISTENT_SEQUENTIAL);
```
```java
```
```java
return
```
```java
true
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
int
```
```java
consume()
```
```java
throws
```
```java
KeeperException, InterruptedException {
```
```java
```
```java
int
```
```java
retvalue = -
```
```java
1
```
```java
;
```
```java
```
```java
Stat stat =
```
```java
null
```
```java
;
```
```java
```
```java
while
```
```java
(
```
```java
true
```
```java
) {
```
```java
```
```java
synchronized
```
```java
(mutex) {
```
```java
```
```java
List<String> list = zk.getChildren(root,
```
```java
true
```
```java
);
```
```java
//并不能保证list[0]就是序号最小的
```
```java
```
```java
//如果根目录下没有子节点就一直等
```
```java
```
```java
if
```
```java
(list.size() ==
```
```java
0
```
```java
) {
```
```java
```
```java
System.out.println(
```
```java
"Going to wait"
```
```java
);
```
```java
```
```java
mutex.wait();
```
```java
```
```java
}
```
```java
```
```java
//找到序号最小的节点将其删除
```
```java
```
```java
else
```
```java
{
```
```java
```
```java
Integer min =
```
```java
new
```
```java
Integer(list.get(
```
```java
0
```
```java
).substring(
```
```java
7
```
```java
));
```
```java
```
```java
for
```
```java
(String s : list) {
```
```java
```
```java
Integer tmp =
```
```java
new
```
```java
Integer(s.substring(
```
```java
7
```
```java
));
```
```java
```
```java
if
```
```java
(tmp < min)
```
```java
```
```java
min = tmp;
```
```java
```
```java
}
```
```java
```
```java
System.out.println(
```
```java
"Temporary value:"
```
```java
+ root
```
```java
```
```java
+
```
```java
"/element"
```
```java
+ min);
```
```java
```
```java
byte
```
```java
[] b = zk.getData(root +
```
```java
"/element"
```
```java
+ min,
```
```java
false
```
```java
,
```
```java
```
```java
stat);
```
```java
```
```java
zk.delete(root +
```
```java
"/element"
```
```java
+ min,
```
```java
0
```
```java
);
```
```java
```
```java
ByteBuffer buffer = ByteBuffer.wrap(b);
```
```java
```
```java
retvalue = buffer.getInt();
```
```java
```
```java
return
```
```java
retvalue;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
public
```
```java
static
```
```java
void
```
```java
main(String[] args) {
```
```java
```
```java
if
```
```java
(args[
```
```java
0
```
```java
].equals(
```
```java
"qTest"
```
```java
))
```
```java
```
```java
queueTest(args);
```
```java
```
```java
else
```
```java
```
```java
barrierTest(args);
```
```java
```
```java
}
```
```java
```
```java
private
```
```java
static
```
```java
void
```
```java
barrierTest(String[] args) {
```
```java
```
```java
Barrier b =
```
```java
new
```
```java
Barrier(args[
```
```java
1
```
```java
],
```
```java
"/b1"
```
```java
,
```
```java
new
```
```java
Integer(args[
```
```java
2
```
```java
]));
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
boolean
```
```java
flag = b.enter();
```
```java
```
```java
System.out.println(
```
```java
"Enter barrier:"
```
```java
+ args[
```
```java
2
```
```java
]);
```
```java
```
```java
if
```
```java
(!flag)
```
```java
```
```java
System.out.println(
```
```java
"Error when entering the barrier"
```
```java
);
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
}
```
```java
```
```java
Random rand =
```
```java
new
```
```java
Random();
```
```java
```
```java
int
```
```java
r = rand.nextInt(
```
```java
100
```
```java
);
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
i =
```
```java
0
```
```java
; i < r; i++) {
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
Thread.sleep(
```
```java
100
```
```java
);
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
b.leave();
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
}
```
```java
```
```java
System.out.println(
```
```java
"Left barrier"
```
```java
);
```
```java
```
```java
}
```
```java
```
```java
private
```
```java
static
```
```java
void
```
```java
queueTest(String[] args) {
```
```java
```
```java
Queue q =
```
```java
new
```
```java
Queue(args[
```
```java
1
```
```java
],
```
```java
"/app1"
```
```java
);
```
```java
```
```java
System.out.println(
```
```java
"Input:"
```
```java
+ args[
```
```java
1
```
```java
]);
```
```java
```
```java
int
```
```java
i;
```
```java
```
```java
Integer max =
```
```java
new
```
```java
Integer(args[
```
```java
2
```
```java
]);
```
```java
```
```java
if
```
```java
(args[
```
```java
3
```
```java
].equals(
```
```java
"p"
```
```java
)) {
```
```java
```
```java
System.out.println(
```
```java
"Producer"
```
```java
);
```
```java
```
```java
for
```
```java
(i =
```
```java
0
```
```java
; i < max; i++)
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
q.produce(
```
```java
10
```
```java
+
```
```java
1
```
```java
);
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
else
```
```java
{
```
```java
```
```java
System.out.println(
```
```java
"Consumer"
```
```java
);
```
```java
```
```java
for
```
```java
(i =
```
```java
0
```
```java
; i < max; i++)
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
int
```
```java
r = q.consume();
```
```java
```
```java
System.out.println(
```
```java
"Item:"
```
```java
+ r);
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
i--;
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
}
```
### Locks
```
[]()
获得锁：
1.创建根节点"/root"
2.在根节点下新建子节点"/root/c-xxxxxx"，SEQUENTIAL模式
3.对根节点调用getChildren()，如果第2步创建的节点是所有子节点中序号最小的，则获得锁；否则进入第4步
4.在序号最小的子节点上调用exists()，当序号最小的子节点被删除后返回第3步
释放锁：
删除自己创建的子节点即可
[]()
原代码：
[+ View Code](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html#)[?](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html#)
```java
package
```
```java
sync;
```
```java
import
```
```java
java.io.IOException;
```
```java
import
```
```java
java.net.InetAddress;
```
```java
import
```
```java
java.util.List;
```
```java
import
```
```java
org.apache.zookeeper.CreateMode;
```
```java
import
```
```java
org.apache.zookeeper.KeeperException;
```
```java
import
```
```java
org.apache.zookeeper.WatchedEvent;
```
```java
import
```
```java
org.apache.zookeeper.Watcher;
```
```java
import
```
```java
org.apache.zookeeper.ZooDefs.Ids;
```
```java
import
```
```java
org.apache.zookeeper.ZooKeeper;
```
```java
import
```
```java
org.apache.zookeeper.data.Stat;
```
```java
public
```
```java
class
```
```java
Locks
```
```java
implements
```
```java
Watcher{
```
```java
```
```java
```
```java
static
```
```java
ZooKeeper zk=
```
```java
null
```
```java
;
```
```java
```
```java
static
```
```java
Integer mutex=
```
```java
null
```
```java
;
```
```java
```
```java
String name=
```
```java
null
```
```java
;
```
```java
```
```java
String path=
```
```java
null
```
```java
;
```
```java
```
```java
@Override
```
```java
```
```java
synchronized
```
```java
public
```
```java
void
```
```java
process(WatchedEvent event) {
```
```java
```
```java
synchronized
```
```java
(mutex){
```
```java
```
```java
mutex.notify();
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
```
```java
Locks(String address){
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
zk=
```
```java
new
```
```java
ZooKeeper(address,
```
```java
2000
```
```java
,
```
```java
this
```
```java
);
```
```java
```
```java
zk.create(
```
```java
"/lock"
```
```java
,
```
```java
new
```
```java
byte
```
```java
[
```
```java
0
```
```java
], Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
```
```java
```
```java
mutex=
```
```java
new
```
```java
Integer(-
```
```java
1
```
```java
);
```
```java
```
```java
name =
```
```java
new
```
```java
String(InetAddress.getLocalHost().getCanonicalHostName().toString());
```
```java
```
```java
}
```
```java
catch
```
```java
(IOException e){
```
```java
```
```java
zk=
```
```java
null
```
```java
;
```
```java
```
```java
}
```
```java
catch
```
```java
(KeeperException e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
}
```
```java
catch
```
```java
(InterruptedException e) {
```
```java
```
```java
e.printStackTrace();
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
```
```java
private
```
```java
int
```
```java
minSeq(List<String> list){
```
```java
```
```java
int
```
```java
min=Integer.parseInt(list.get(
```
```java
0
```
```java
).substring(
```
```java
14
```
```java
));
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
i=
```
```java
1
```
```java
;i<list.size();i++){
```
```java
```
```java
if
```
```java
(min<Integer.parseInt(list.get(i).substring(
```
```java
14
```
```java
)))
```
```java
```
```java
min=Integer.parseInt(list.get(i).substring(
```
```java
14
```
```java
));
```
```java
```
```java
}
```
```java
```
```java
return
```
```java
min;
```
```java
```
```java
}
```
```java
```
```java
```
```java
boolean
```
```java
getLock()
```
```java
throws
```
```java
KeeperException, InterruptedException{
```
```java
```
```java
//create方法返回新建的节点的完整路径
```
```java
```
```java
path=zk.create(
```
```java
"/lock/"
```
```java
+name+
```
```java
"-"
```
```java
,
```
```java
new
```
```java
byte
```
```java
[
```
```java
0
```
```java
], Ids.OPEN_ACL_UNSAFE,CreateMode.EPHEMERAL_SEQUENTIAL);
```
```java
```
```java
int
```
```java
min;
```
```java
```
```java
while
```
```java
(
```
```java
true
```
```java
){
```
```java
```
```java
synchronized
```
```java
(mutex){
```
```java
```
```java
List<String> list=zk.getChildren(
```
```java
"/lock"
```
```java
,
```
```java
false
```
```java
);
```
```java
```
```java
min=minSeq(list);
```
```java
```
```java
//如果刚建的节点是根节点的所有子节点中序号最小的，则获得了锁，可以返回true
```
```java
```
```java
if
```
```java
(min==Integer.parseInt(path.substring(
```
```java
14
```
```java
))){
```
```java
```
```java
return
```
```java
true
```
```java
;
```
```java
```
```java
}
```
```java
else
```
```java
{
```
```java
```
```java
mutex.wait();
```
```java
//等待事件（新建节点或删除节点）发生
```
```java
```
```java
while
```
```java
(
```
```java
true
```
```java
){
```
```java
```
```java
Stat s=zk.exists(
```
```java
"/lock/"
```
```java
+name+
```
```java
"-"
```
```java
+min,
```
```java
true
```
```java
);
```
```java
//查看序号最小的子节点还在不在
```
```java
```
```java
if
```
```java
(s!=
```
```java
null
```
```java
)
```
```java
//如果还在，则继续等待事件发生
```
```java
```
```java
mutex.wait();
```
```java
```
```java
else
```
```java
//如果不在，则跳外层循环中，查看新的最小序号的子节点是谁
```
```java
```
```java
break
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
```
```java
boolean
```
```java
releaseLock()
```
```java
throws
```
```java
KeeperException, InterruptedException{
```
```java
```
```java
if
```
```java
(path!=
```
```java
null
```
```java
){
```
```java
```
```java
zk.delete(path, -
```
```java
1
```
```java
);
```
```java
```
```java
path=
```
```java
null
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
return
```
```java
true
```
```java
;
```
```java
```
```java
}
```
```java
```
```java
```
```java
public
```
```java
static
```
```java
void
```
```java
main(String []args)
```
```java
throws
```
```java
KeeperException, InterruptedException{
```
```java
```
```java
Locks lock1=
```
```java
new
```
```java
Locks(
```
```java
"localhost:2181"
```
```java
);
```
```java
```
```java
if
```
```java
(lock1.getLock()){
```
```java
```
```java
System.out.println(
```
```java
"T1 Get lock at "
```
```java
+System.currentTimeMillis());
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
i=
```
```java
0
```
```java
;i<
```
```java
1000
```
```java
;++i)
```
```java
```
```java
Thread.sleep(
```
```java
5000
```
```java
);
```
```java
```
```java
lock1.releaseLock();
```
```java
```
```java
}
```
```java
```
```java
Locks lock2=
```
```java
new
```
```java
Locks(
```
```java
"localhost:2181"
```
```java
);
```
```java
```
```java
if
```
```java
(lock2.getLock()){
```
```java
```
```java
System.out.println(
```
```java
"T2 Get lock at "
```
```java
+System.currentTimeMillis());
```
```java
```
```java
lock2.releaseLock();
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
}
```
读锁（共享锁）和写锁（排斥锁）并存的情况跟单独只有排斥锁的情况有几点不同：
1.当一个线程想施加读锁时就新建一个节点"/root/read-xxxxxx"，施加写锁时就新建一个节点"/root/write-xxxxxx";
2.欲施加读锁的线程查看"/root"下有没有“write"开头的节点，如果没有则直接获得读锁；如果有，但是"write"节点的序号比自己刚才创建的"read"节点的序号要大说明是先施加的读锁后施加的写锁，所以依然获得读锁；else，在序号最小的"write"节点上调用exists，等待它被删除。
```
```
```
```
```
```
```
