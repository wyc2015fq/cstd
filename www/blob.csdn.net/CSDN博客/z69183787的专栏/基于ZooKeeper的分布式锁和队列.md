# 基于ZooKeeper的分布式锁和队列 - z69183787的专栏 - CSDN博客
2017年12月12日 12:34:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：270
个人分类：[分布式-Zookeeper																[锁-分布式锁](https://blog.csdn.net/z69183787/article/category/6393629)](https://blog.csdn.net/z69183787/article/category/5568407)
[https://www.cnblogs.com/luxiaoxun/p/4889764.html](https://www.cnblogs.com/luxiaoxun/p/4889764.html)
在分布式系统中，往往需要一些分布式同步原语来做一些协同工作，[上一篇](http://www.cnblogs.com/luxiaoxun/p/4887452.html)文章介绍了Zookeeper的基本原理，本文介绍下基于Zookeeper的Lock和Queue的实现，主要代码都来自Zookeeper的官方recipe。
### 锁（Lock）
完全分布式锁是全局同步的，这意味着在任何时刻没有两个客户端会同时认为它们都拥有相同的锁，使用 Zookeeper 可以实现分布式锁，需要首先定义一个锁节点（lock root node）。
需要获得锁的客户端按照以下步骤来获取锁：
- 保证锁节点（lock root node）这个父根节点的存在，这个节点是每个要获取lock客户端共用的，这个节点是PERSISTENT的。
- 
第一次需要创建本客户端要获取lock的节点，调用 create( )，并设置 节点为EPHEMERAL_SEQUENTIAL类型，表示该节点为临时的和顺序的。如果获取锁的节点挂掉，则该节点自动失效，可以让其他节点获取锁。
- 
在父锁节点（lock root node）上调用 getChildren( ) ，不需要设置监视标志。 (为了避免“羊群效应”).
- 
按照Fair竞争的原则，将步骤3中的子节点（要获取锁的节点）按照节点顺序的大小做排序，取出编号最小的一个节点做为lock的owner，判断自己的节点id
是否就为owner id，如果是则返回，lock成功。如果不是则调用 exists( )监听比自己小的前一位的id，关注它锁释放的操作（也就是exist watch）。
- 
如果第4步监听exist的watch被触发，则继续按4中的原则判断自己是否能获取到lock。
释放锁：需要释放锁的客户端只需要删除在第2步中创建的节点即可。
注意事项：
一个节点的删除只会导致一个客户端被唤醒，因为每个节点只被一个客户端watch，这避免了“羊群效应”。
一个分布式lock的实现：
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package org.apache.zookeeper.recipes.lock;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import static org.apache.zookeeper.CreateMode.EPHEMERAL_SEQUENTIAL;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.data.ACL;
import org.apache.zookeeper.data.Stat;
import java.util.List;
import java.util.SortedSet;
import java.util.TreeSet;
/**
 * A <a href="package.html">protocol to implement an exclusive
 *  write lock or to elect a leader</a>. <p/> You invoke {@link #lock()} to 
 *  start the process of grabbing the lock; you may get the lock then or it may be 
 *  some time later. <p/> You can register a listener so that you are invoked 
 *  when you get the lock; otherwise you can ask if you have the lock
 *  by calling {@link #isOwner()}
 *
 */
public class WriteLock extends ProtocolSupport {
    private static final Logger LOG = LoggerFactory.getLogger(WriteLock.class);
    private final String dir;
    private String id;
    private ZNodeName idName;
    private String ownerId;
    private String lastChildId;
    private byte[] data = {0x12, 0x34};
    private LockListener callback;
    private LockZooKeeperOperation zop;
    
    /**
     * zookeeper contructor for writelock
     * @param zookeeper zookeeper client instance
     * @param dir the parent path you want to use for locking
     * @param acls the acls that you want to use for all the paths, 
     * if null world read/write is used.
     */
    public WriteLock(ZooKeeper zookeeper, String dir, List<ACL> acl) {
        super(zookeeper);
        this.dir = dir;
        if (acl != null) {
            setAcl(acl);
        }
        this.zop = new LockZooKeeperOperation();
    }
    
    /**
     * zookeeper contructor for writelock with callback
     * @param zookeeper the zookeeper client instance
     * @param dir the parent path you want to use for locking
     * @param acl the acls that you want to use for all the paths
     * @param callback the call back instance
     */
    public WriteLock(ZooKeeper zookeeper, String dir, List<ACL> acl, 
            LockListener callback) {
        this(zookeeper, dir, acl);
        this.callback = callback;
    }
    /**
     * return the current locklistener
     * @return the locklistener
     */
    public LockListener getLockListener() {
        return this.callback;
    }
    
    /**
     * register a different call back listener
     * @param callback the call back instance
     */
    public void setLockListener(LockListener callback) {
        this.callback = callback;
    }
    /**
     * Removes the lock or associated znode if 
     * you no longer require the lock. this also 
     * removes your request in the queue for locking
     * in case you do not already hold the lock.
     * @throws RuntimeException throws a runtime exception
     * if it cannot connect to zookeeper.
     */
    public synchronized void unlock() throws RuntimeException {
        
        if (!isClosed() && id != null) {
            // we don't need to retry this operation in the case of failure
            // as ZK will remove ephemeral files and we don't wanna hang
            // this process when closing if we cannot reconnect to ZK
            try {
                ZooKeeperOperation zopdel = new ZooKeeperOperation() {
                    public boolean execute() throws KeeperException,
                        InterruptedException {
                        zookeeper.delete(id, -1);   
                        return Boolean.TRUE;
                    }
                };
                zopdel.execute();
            } catch (InterruptedException e) {
                LOG.warn("Caught: " + e, e);
                //set that we have been interrupted.
               Thread.currentThread().interrupt();
            } catch (KeeperException.NoNodeException e) {
                // do nothing
            } catch (KeeperException e) {
                LOG.warn("Caught: " + e, e);
                throw (RuntimeException) new RuntimeException(e.getMessage()).
                    initCause(e);
            }
            finally {
                if (callback != null) {
                    callback.lockReleased();
                }
                id = null;
            }
        }
    }
    
    /** 
     * the watcher called on  
     * getting watch while watching 
     * my predecessor
     */
    private class LockWatcher implements Watcher {
        public void process(WatchedEvent event) {
            // lets either become the leader or watch the new/updated node
            LOG.debug("Watcher fired on path: " + event.getPath() + " state: " + 
                    event.getState() + " type " + event.getType());
            try {
                lock();
            } catch (Exception e) {
                LOG.warn("Failed to acquire lock: " + e, e);
            }
        }
    }
    
    /**
     * a zoookeeper operation that is mainly responsible
     * for all the magic required for locking.
     */
    private  class LockZooKeeperOperation implements ZooKeeperOperation {
        
        /** find if we have been created earler if not create our node
         * 
         * @param prefix the prefix node
         * @param zookeeper teh zookeeper client
         * @param dir the dir paretn
         * @throws KeeperException
         * @throws InterruptedException
         */
        private void findPrefixInChildren(String prefix, ZooKeeper zookeeper, String dir) 
            throws KeeperException, InterruptedException {
            List<String> names = zookeeper.getChildren(dir, false);
            for (String name : names) {
                if (name.startsWith(prefix)) {
                    id = dir + "/" + name;
                    if (LOG.isDebugEnabled()) {
                        LOG.debug("Found id created last time: " + id);
                    }
                    break;
                }
            }
            if (id == null) {
                id = zookeeper.create(dir + "/" + prefix, data, 
                        getAcl(), EPHEMERAL_SEQUENTIAL);
                if (LOG.isDebugEnabled()) {
                    LOG.debug("Created id: " + id);
                }
            }
        }
        
        /**
         * the command that is run and retried for actually 
         * obtaining the lock
         * @return if the command was successful or not
         */
        public boolean execute() throws KeeperException, InterruptedException {
            do {
                if (id == null) {
                    long sessionId = zookeeper.getSessionId();
                    String prefix = "x-" + sessionId + "-";
                    // lets try look up the current ID if we failed 
                    // in the middle of creating the znode
                    findPrefixInChildren(prefix, zookeeper, dir);
                    idName = new ZNodeName(id);
                }
                if (id != null) {
                    List<String> names = zookeeper.getChildren(dir, false);
                    if (names.isEmpty()) {
                        LOG.warn("No children in: " + dir + " when we've just " +
                        "created one! Lets recreate it...");
                        // lets force the recreation of the id
                        id = null;
                    } else {
                        // lets sort them explicitly (though they do seem to come back in order ususally :)
                        SortedSet<ZNodeName> sortedNames = new TreeSet<ZNodeName>();
                        for (String name : names) {
                            sortedNames.add(new ZNodeName(dir + "/" + name));
                        }
                        ownerId = sortedNames.first().getName();
                        SortedSet<ZNodeName> lessThanMe = sortedNames.headSet(idName);
                        if (!lessThanMe.isEmpty()) {
                            ZNodeName lastChildName = lessThanMe.last();
                            lastChildId = lastChildName.getName();
                            if (LOG.isDebugEnabled()) {
                                LOG.debug("watching less than me node: " + lastChildId);
                            }
                            Stat stat = zookeeper.exists(lastChildId, new LockWatcher());
                            if (stat != null) {
                                return Boolean.FALSE;
                            } else {
                                LOG.warn("Could not find the" +
                                        " stats for less than me: " + lastChildName.getName());
                            }
                        } else {
                            if (isOwner()) {
                                if (callback != null) {
                                    callback.lockAcquired();
                                }
                                return Boolean.TRUE;
                            }
                        }
                    }
                }
            }
            while (id == null);
            return Boolean.FALSE;
        }
    };
    /**
     * Attempts to acquire the exclusive write lock returning whether or not it was
     * acquired. Note that the exclusive lock may be acquired some time later after
     * this method has been invoked due to the current lock owner going away.
     */
    public synchronized boolean lock() throws KeeperException, InterruptedException {
        if (isClosed()) {
            return false;
        }
        ensurePathExists(dir);
        return (Boolean) retryOperation(zop);
    }
    /**
     * return the parent dir for lock
     * @return the parent dir used for locks.
     */
    public String getDir() {
        return dir;
    }
    /**
     * Returns true if this node is the owner of the
     *  lock (or the leader)
     */
    public boolean isOwner() {
        return id != null && ownerId != null && id.equals(ownerId);
    }
    /**
     * return the id for this lock
     * @return the id for this lock
     */
    public String getId() {
       return this.id;
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
注意这里的lock，可能会失败，会尝试多次，每次失败后会Sleep一段时间。
PS：官方的代码有个小bug，id和ownerId应该都是全路径，即id = dir + "/" + name;原代码在findPrefixInChildren里有问题。
用于辅助节点大小顺序排序的类：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
PS：这个ZNodeName类是被我修改过的，官方的代码比较有问题，官方的先用了节点路径的前缀prefix比较，再去比较sequence序号是不对的，这样会导致sessionid小的总是能拿到锁。应该直接比较全局有序的sequence序号，小的先拿到锁，先到先得。
Zookeeper统一操作ZooKeeperOperation接口：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
因为Zookeeper的操作会失败，这个类封装了多次尝试：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
这个类是本客户端获取到lock和释放lock的时候触发操作的接口：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
### 队列（Queue）
分布式队列是通用的数据结构，为了在 Zookeeper 中实现分布式队列，首先需要指定一个 Znode 节点作为队列节点（queue node）， 各个分布式客户端通过调用 create() 函数向队列中放入数据，调用create()时节点路径名带"qn-"结尾，并设置顺序（sequence）节点标志。 由于设置了节点的顺序标志，新的路径名具有以下字符串模式："_path-to-queue-node_/qn-X"，X 是唯一自增号。需要从队列中获取数据/移除数据的客户端首先调用 getChildren() 函数，有数据则获取（获取数据后可以删除也可以不删），没有则在队列节点（queue
 node）上将 watch 设置为 true，等待触发并处理最小序号的节点（即从序号最小的节点中取数据）。
实现步骤基本如下：
前提：需要一个队列root节点dir
入队：使用create()创建节点，将共享数据data放在该节点上，节点类型为PERSISTENT_SEQUENTIAL，永久顺序性的（也可以设置为临时的，看需求）。
出队：因为队列可能为空，2种方式处理：一种如果为空则wait等待，一种返回异常。
等待方式：这里使用了CountDownLatch的等待和Watcher的通知机制，使用了TreeMap的排序获取节点顺序最小的数据（FIFO）。
抛出异常：getChildren()获取队列数据时，如果size==0则抛出异常。
一个分布式Queue的实现，详细代码：
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package org.apache.zookeeper.recipes.queue;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.TreeMap;
import java.util.concurrent.CountDownLatch;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.apache.zookeeper.CreateMode;
import org.apache.zookeeper.KeeperException;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooDefs;
import org.apache.zookeeper.ZooKeeper;
import org.apache.zookeeper.data.ACL;
import org.apache.zookeeper.data.Stat;
/**
 * 
 * A <a href="package.html">protocol to implement a distributed queue</a>.
 * 
 */
public class DistributedQueue {
    private static final Logger LOG = LoggerFactory.getLogger(DistributedQueue.class);
    private final String dir;
    private ZooKeeper zookeeper;
    private List<ACL> acl = ZooDefs.Ids.OPEN_ACL_UNSAFE;
    private final String prefix = "qn-";
    public DistributedQueue(ZooKeeper zookeeper, String dir, List<ACL> acl){
        this.dir = dir;
        if(acl != null){
            this.acl = acl;
        }
        this.zookeeper = zookeeper;
        
        //Add root dir first if not exists
        if (zookeeper != null) {
            try {
                Stat s = zookeeper.exists(dir, false);
                if (s == null) {
                    zookeeper.create(dir, new byte[0], acl, CreateMode.PERSISTENT);
                }
            } catch (KeeperException e) {
                LOG.error(e.toString());
            } catch (InterruptedException e) {
                LOG.error(e.toString());
            }
        }
    }
    /**
     * Returns a Map of the children, ordered by id.
     * @param watcher optional watcher on getChildren() operation.
     * @return map from id to child name for all children
     */
    private TreeMap<Long,String> orderedChildren(Watcher watcher) throws KeeperException, InterruptedException {
        TreeMap<Long,String> orderedChildren = new TreeMap<Long,String>();
        List<String> childNames = null;
        try{
            childNames = zookeeper.getChildren(dir, watcher);
        }catch (KeeperException.NoNodeException e){
            throw e;
        }
        for(String childName : childNames){
            try{
                //Check format
                if(!childName.regionMatches(0, prefix, 0, prefix.length())){
                    LOG.warn("Found child node with improper name: " + childName);
                    continue;
                }
                String suffix = childName.substring(prefix.length());
                Long childId = new Long(suffix);
                orderedChildren.put(childId,childName);
            }catch(NumberFormatException e){
                LOG.warn("Found child node with improper format : " + childName + " " + e,e);
            }
        }
        return orderedChildren;
    }
    /**
     * Find the smallest child node.
     * @return The name of the smallest child node.
     */
    private String smallestChildName() throws KeeperException, InterruptedException {
        long minId = Long.MAX_VALUE;
        String minName = "";
        List<String> childNames = null;
        try{
            childNames = zookeeper.getChildren(dir, false);
        }catch(KeeperException.NoNodeException e){
            LOG.warn("Caught: " +e,e);
            return null;
        }
        for(String childName : childNames){
            try{
                //Check format
                if(!childName.regionMatches(0, prefix, 0, prefix.length())){
                    LOG.warn("Found child node with improper name: " + childName);
                    continue;
                }
                String suffix = childName.substring(prefix.length());
                long childId = Long.parseLong(suffix);
                if(childId < minId){
                    minId = childId;
                    minName = childName;
                }
            }catch(NumberFormatException e){
                LOG.warn("Found child node with improper format : " + childName + " " + e,e);
            }
        }
        if(minId < Long.MAX_VALUE){
            return minName;
        }else{
            return null;
        }
    }
    /**
     * Return the head of the queue without modifying the queue.
     * @return the data at the head of the queue.
     * @throws NoSuchElementException
     * @throws KeeperException
     * @throws InterruptedException
     */
    public byte[] element() throws NoSuchElementException, KeeperException, InterruptedException {
        TreeMap<Long,String> orderedChildren;
        // element, take, and remove follow the same pattern.
        // We want to return the child node with the smallest sequence number.
        // Since other clients are remove()ing and take()ing nodes concurrently, 
        // the child with the smallest sequence number in orderedChildren might be gone by the time we check.
        // We don't call getChildren again until we have tried the rest of the nodes in sequence order.
        while(true){
            try{
                orderedChildren = orderedChildren(null);
            }catch(KeeperException.NoNodeException e){
                throw new NoSuchElementException();
            }
            if(orderedChildren.size() == 0 ) throw new NoSuchElementException();
            for(String headNode : orderedChildren.values()){
                if(headNode != null){
                    try{
                        return zookeeper.getData(dir+"/"+headNode, false, null);
                    }catch(KeeperException.NoNodeException e){
                        //Another client removed the node first, try next
                    }
                }
            }
        }
    }
    /**
     * Attempts to remove the head of the queue and return it.
     * @return The former head of the queue
     * @throws NoSuchElementException
     * @throws KeeperException
     * @throws InterruptedException
     */
    public byte[] remove() throws NoSuchElementException, KeeperException, InterruptedException {
        TreeMap<Long,String> orderedChildren;
        // Same as for element.  Should refactor this.
        while(true){
            try{
                orderedChildren = orderedChildren(null);
            }catch(KeeperException.NoNodeException e){
                throw new NoSuchElementException();
            }
            if(orderedChildren.size() == 0) throw new NoSuchElementException();
            for(String headNode : orderedChildren.values()){
                String path = dir +"/"+headNode;
                try{
                    byte[] data = zookeeper.getData(path, false, null);
                    zookeeper.delete(path, -1);
                    return data;
                }catch(KeeperException.NoNodeException e){
                    // Another client deleted the node first.
                }
            }
        }
    }
    private class LatchChildWatcher implements Watcher {
        CountDownLatch latch;
        public LatchChildWatcher(){
            latch = new CountDownLatch(1);
        }
        public void process(WatchedEvent event){
            LOG.debug("Watcher fired on path: " + event.getPath() + " state: " + 
                    event.getState() + " type " + event.getType());
            latch.countDown();
        }
        public void await() throws InterruptedException {
            latch.await();
        }
    }
    /**
     * Removes the head of the queue and returns it, blocks until it succeeds.
     * @return The former head of the queue
     * @throws NoSuchElementException
     * @throws KeeperException
     * @throws InterruptedException
     */
    public byte[] take() throws KeeperException, InterruptedException {
        TreeMap<Long,String> orderedChildren;
        // Same as for element.  Should refactor this.
        while(true){
            LatchChildWatcher childWatcher = new LatchChildWatcher();
            try{
                orderedChildren = orderedChildren(childWatcher);
            }catch(KeeperException.NoNodeException e){
                zookeeper.create(dir, new byte[0], acl, CreateMode.PERSISTENT);
                continue;
            }
            if(orderedChildren.size() == 0){
                childWatcher.await();
                continue;
            }
            for(String headNode : orderedChildren.values()){
                String path = dir +"/"+headNode;
                try{
                    byte[] data = zookeeper.getData(path, false, null);
                    zookeeper.delete(path, -1);
                    return data;
                }catch(KeeperException.NoNodeException e){
                    // Another client deleted the node first.
                }
            }
        }
    }
    /**
     * Inserts data into queue.
     * @param data
     * @return true if data was successfully added
     */
    public boolean offer(byte[] data) throws KeeperException, InterruptedException{
        for(;;){
            try{
                zookeeper.create(dir+"/"+prefix, data, acl, CreateMode.PERSISTENT_SEQUENTIAL);
                return true;
            }catch(KeeperException.NoNodeException e){
                zookeeper.create(dir, new byte[0], acl, CreateMode.PERSISTENT);
            }
        }
    }
    /**
     * Returns the data at the first element of the queue, or null if the queue is empty.
     * @return data at the first element of the queue, or null.
     * @throws KeeperException
     * @throws InterruptedException
     */
    public byte[] peek() throws KeeperException, InterruptedException{
        try{
            return element();
        }catch(NoSuchElementException e){
            return null;
        }
    }
    /**
     * Attempts to remove the head of the queue and return it. Returns null if the queue is empty.
     * @return Head of the queue or null.
     * @throws KeeperException
     * @throws InterruptedException
     */
    public byte[] poll() throws KeeperException, InterruptedException {
        try{
            return remove();
        }catch(NoSuchElementException e){
            return null;
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
Apache Curator
Curator是一个封装Zookeeper操作的库，使用这个库的好处是Curator帮你管理和Zookeeper的连接，当连接有问题时会自动重试（retry）。
```
RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000, 3)
CuratorFramework client = CuratorFrameworkFactory.newClient(zookeeperConnectionString, retryPolicy);
client.start();
```
Curator已经封装了一些常用的Recipes
#### Distributed Lock
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
InterProcessMutex lock = new InterProcessMutex(client, lockPath);
if ( lock.acquire(maxWait, waitUnit) ) 
{
    try 
    {
        // do some work inside of the critical section here
    }
    finally
    {
        lock.release();
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
#### Leader Election
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
LeaderSelectorListener listener = new LeaderSelectorListenerAdapter()
{
    public void takeLeadership(CuratorFramework client) throws Exception
    {
        // this callback will get called when you are the leader
        // do whatever leader work you need to and only exit
        // this method when you want to relinquish leadership
    }
}
LeaderSelector selector = new LeaderSelector(client, path, listener);
selector.autoRequeue();  // not required, but this is behavior that you will probably expect
selector.start(); 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
参考：
http://zookeeper.apache.org/doc/trunk/recipes.html
http://curator.apache.org/curator-recipes/index.html
