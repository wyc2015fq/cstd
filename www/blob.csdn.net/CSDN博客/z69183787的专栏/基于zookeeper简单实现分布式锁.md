# 基于zookeeper简单实现分布式锁 - z69183787的专栏 - CSDN博客
2016年08月30日 13:55:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：654
个人分类：[分布式-Zookeeper																[锁-分布式锁](https://blog.csdn.net/z69183787/article/category/6393629)](https://blog.csdn.net/z69183787/article/category/5568407)
这里利用zookeeper的EPHEMERAL_SEQUENTIAL类型节点及watcher机制，来简单实现分布式锁。
主要思想：
1、开启10个线程，在disLocks节点下各自创建名为sub的EPHEMERAL_SEQUENTIAL节点；
2、获取disLocks节点下所有子节点，排序，如果自己的节点编号最小，则获取锁；
3、否则watch排在自己前面的节点，监听到其删除后，进入第2步（重新检测排序是防止监听的节点发生连接失效，导致的节点删除情况）；
4、删除自身sub节点，释放连接；
```java
SEQUENTIAL
```
节点名末尾会自动追加一个10位数的单调递增的序号，同一个节点的所有子节点序号是单调递增的
这里插播下zookeeper的4种节点类型：
```java
public enum CreateMode {
   
    /**
     * 持久节点：节点创建后，会一直存在，不会因客户端会话失效而删除；
     */
    PERSISTENT (0, false, false),
    /**
    * 持久顺序节点：基本特性与持久节点一致，创建节点的过程中，zookeeper会在其名字后自动追加一个单调增长的数字后缀，作为新的节点名； 
    */
    PERSISTENT_SEQUENTIAL (2, false, true),
    /**
     *  临时节点：客户端会话失效或连接关闭后，该节点会被自动删除，且不能再临时节点下面创建子节点，否则报如下错：org.apache.zookeeper.KeeperException$NoChildrenForEphemeralsException；
     */
    EPHEMERAL (1, true, false),
    /**
     * 临时顺序节点：基本特性与临时节点一致，创建节点的过程中，zookeeper会在其名字后自动追加一个单调增长的数字后缀，作为新的节点名； 
     */
    EPHEMERAL_SEQUENTIAL (3, true, true);
    private static final Logger LOG = LoggerFactory.getLogger(CreateMode.class);
    private boolean ephemeral;
    private boolean sequential;
    private int flag;
    CreateMode(int flag, boolean ephemeral, boolean sequential) {
        this.flag = flag;
        this.ephemeral = ephemeral;
        this.sequential = sequential;
    }
    public boolean isEphemeral() {
        return ephemeral;
    }
    public boolean isSequential() {
        return sequential;
    }
    public int toFlag() {
        return flag;
    }
    static public CreateMode fromFlag(int flag) throws KeeperException {
        switch(flag) {
        case 0: return CreateMode.PERSISTENT;
        case 1: return CreateMode.EPHEMERAL;
        case 2: return CreateMode.PERSISTENT_SEQUENTIAL;
        case 3: return CreateMode.EPHEMERAL_SEQUENTIAL ;
        default:
            LOG.error("Received an invalid flag value to convert to a CreateMode");
            throw new KeeperException.BadArgumentsException();
        }
    }
}
```
测试代码：
```java
package zookeeper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.apache.zookeeper.*;
import org.apache.zookeeper.data.Stat;
import java.util.List;
import java.io.IOException;
import java.util.Collections;
import java.util.concurrent.CountDownLatch;
public class DistributedLock implements Watcher{
    private int threadId;
    private ZooKeeper zk = null;
    private String selfPath;
    private String waitPath;
    private String LOG_PREFIX_OF_THREAD;
    private static final int SESSION_TIMEOUT = 10000;
    private static final String GROUP_PATH = "/disLocks";
    private static final String SUB_PATH = "/disLocks/sub";
    private static final String CONNECTION_STRING = "192.168.*.*:2181";
    
    private static final int THREAD_NUM = 10; 
    //确保连接zk成功；
    private CountDownLatch connectedSemaphore = new CountDownLatch(1);
    //确保所有线程运行结束；
    private static final CountDownLatch threadSemaphore = new CountDownLatch(THREAD_NUM);
    private static final Logger LOG = LoggerFactory.getLogger(AllZooKeeperWatcher.class);
    public DistributedLock(int id) {
        this.threadId = id;
        LOG_PREFIX_OF_THREAD = "【第"+threadId+"个线程】";
    }
    public static void main(String[] args) {
        for(int i=0; i < THREAD_NUM; i++){
            final int threadId = i+1;
            new Thread(){
                @Override
                public void run() {
                    try{
                        DistributedLock dc = new DistributedLock(threadId);
                        dc.createConnection(CONNECTION_STRING, SESSION_TIMEOUT);
                        //GROUP_PATH不存在的话，由一个线程创建即可；
                        synchronized (threadSemaphore){
                            dc.createPath(GROUP_PATH, "该节点由线程" + threadId + "创建", true);
                        }
                        dc.getLock();
                    } catch (Exception e){
                        LOG.error("【第"+threadId+"个线程】 抛出的异常：");
                        e.printStackTrace();
                    }
                }
            }.start();
        }
        try {
            threadSemaphore.await();
            LOG.info("所有线程运行结束!");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    /**
     * 获取锁
     * @return
     */
    private void getLock() throws KeeperException, InterruptedException {
        selfPath = zk.create(SUB_PATH,null, ZooDefs.Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL_SEQUENTIAL);
        LOG.info(LOG_PREFIX_OF_THREAD+"创建锁路径:"+selfPath);
        if(checkMinPath()){
            getLockSuccess();
        }
    }
    /**
     * 创建节点
     * @param path 节点path
     * @param data 初始数据内容
     * @return
     */
    public boolean createPath( String path, String data, boolean needWatch) throws KeeperException, InterruptedException {
        if(zk.exists(path, needWatch)==null){
            LOG.info( LOG_PREFIX_OF_THREAD + "节点创建成功, Path: "
                    + this.zk.create( path,
                    data.getBytes(),
                    ZooDefs.Ids.OPEN_ACL_UNSAFE,
                    CreateMode.PERSISTENT )
                    + ", content: " + data );
        }
        return true;
    }
    /**
     * 创建ZK连接
     * @param connectString	 ZK服务器地址列表
     * @param sessionTimeout Session超时时间
     */
    public void createConnection( String connectString, int sessionTimeout ) throws IOException, InterruptedException {
            zk = new ZooKeeper( connectString, sessionTimeout, this);
            connectedSemaphore.await();
    }
    /**
     * 获取锁成功
    */
    public void getLockSuccess() throws KeeperException, InterruptedException {
        if(zk.exists(this.selfPath,false) == null){
            LOG.error(LOG_PREFIX_OF_THREAD+"本节点已不在了...");
            return;
        }
        LOG.info(LOG_PREFIX_OF_THREAD + "获取锁成功，赶紧干活！");
        Thread.sleep(2000);
        LOG.info(LOG_PREFIX_OF_THREAD + "删除本节点："+selfPath);
        zk.delete(this.selfPath, -1);
        releaseConnection();
        threadSemaphore.countDown();
    }
    /**
     * 关闭ZK连接
     */
    public void releaseConnection() {
        if ( this.zk !=null ) {
            try {
                this.zk.close();
            } catch ( InterruptedException e ) {}
        }
        LOG.info(LOG_PREFIX_OF_THREAD + "释放连接");
    }
    /**
     * 检查自己是不是最小的节点
     * @return
     */
    public boolean checkMinPath() throws KeeperException, InterruptedException {
         List<String> subNodes = zk.getChildren(GROUP_PATH, false);
         Collections.sort(subNodes);
         int index = subNodes.indexOf( selfPath.substring(GROUP_PATH.length()+1));
         switch (index){
             case -1:{
                 LOG.error(LOG_PREFIX_OF_THREAD+"本节点已不在了..."+selfPath);
                 return false;
             }
             case 0:{
                 LOG.info(LOG_PREFIX_OF_THREAD+"子节点中，我果然是老大"+selfPath);
                 return true;
             }
             default:{
                 this.waitPath = GROUP_PATH +"/"+ subNodes.get(index - 1);
                 LOG.info(LOG_PREFIX_OF_THREAD+"获取子节点中，排在我前面的"+waitPath);
                 try{
                     zk.getData(waitPath, true, new Stat());
                     return false;
                 }catch(KeeperException e){
                     if(zk.exists(waitPath,false) == null){
                         LOG.info(LOG_PREFIX_OF_THREAD+"子节点中，排在我前面的"+waitPath+"已失踪，幸福来得太突然?");
                         return checkMinPath();
                     }else{
                         throw e;
                     }
                 }
             }
                 
         }
     
    }
    @Override
    public void process(WatchedEvent event) {
        if(event == null){
            return;
        }
        Event.KeeperState keeperState = event.getState();
        Event.EventType eventType = event.getType();
        if ( Event.KeeperState.SyncConnected == keeperState) {
            if ( Event.EventType.None == eventType ) {
                LOG.info( LOG_PREFIX_OF_THREAD + "成功连接上ZK服务器" );
                connectedSemaphore.countDown();
            }else if (event.getType() == Event.EventType.NodeDeleted && event.getPath().equals(waitPath)) {
                LOG.info(LOG_PREFIX_OF_THREAD + "收到情报，排我前面的家伙已挂，我是不是可以出山了？");
                try {
                    if(checkMinPath()){
                        getLockSuccess();
                    }
                } catch (KeeperException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }else if ( Event.KeeperState.Disconnected == keeperState ) {
            LOG.info( LOG_PREFIX_OF_THREAD + "与ZK服务器断开连接" );
        } else if ( Event.KeeperState.AuthFailed == keeperState ) {
            LOG.info( LOG_PREFIX_OF_THREAD + "权限检查失败" );
        } else if ( Event.KeeperState.Expired == keeperState ) {
            LOG.info( LOG_PREFIX_OF_THREAD + "会话失效" );
        }
    }
}
```
log配置文件：
```
# DEFAULT 
log4j.rootLogger=INFO,CONSOLE
#
# Log INFO level and above messages to the console
#
log4j.appender.CONSOLE=org.apache.log4j.ConsoleAppender
log4j.appender.CONSOLE.Threshold=INFO
log4j.appender.CONSOLE.layout=org.apache.log4j.PatternLayout
log4j.appender.CONSOLE.layout.ConversionPattern=%d{ISO8601} - %m%n
log4j.appender.COMMONSTAT=org.apache.log4j.DailyRollingFileAppender
log4j.appender.COMMONSTAT.Threshold=INFO
log4j.appender.COMMONSTAT.File=/home/zookeeper/zookeeper-test-agent/logs/test.log
log4j.appender.COMMONSTAT.DatePattern='.'yyyy-MM-dd
log4j.appender.COMMONSTAT.layout=org.apache.log4j.PatternLayout
log4j.appender.COMMONSTAT.layout.ConversionPattern=[%d{yyyy-MM-dd HH:mm:ss}] - %m%n
log4j.logger.org.displaytag=WARN
log4j.logger.org.apache.zookeeper=ERROR
log4j.logger.org.springframework=WARN
log4j.logger.org.I0Itec=WARN
log4j.logger.commonStat=INFO,COMMONSTAT
```
运行结果：
```
2014-11-19 11:34:10,894 - 【第9个线程】成功连接上ZK服务器
2014-11-19 11:34:10,895 - 【第8个线程】成功连接上ZK服务器
2014-11-19 11:34:10,894 - 【第1个线程】成功连接上ZK服务器
2014-11-19 11:34:10,894 - 【第7个线程】成功连接上ZK服务器
2014-11-19 11:34:10,894 - 【第4个线程】成功连接上ZK服务器
2014-11-19 11:34:10,895 - 【第5个线程】成功连接上ZK服务器
2014-11-19 11:34:10,896 - 【第2个线程】成功连接上ZK服务器
2014-11-19 11:34:10,894 - 【第10个线程】成功连接上ZK服务器
2014-11-19 11:34:10,894 - 【第3个线程】成功连接上ZK服务器
2014-11-19 11:34:10,895 - 【第6个线程】成功连接上ZK服务器
2014-11-19 11:34:10,910 - 【第9个线程】节点创建成功, Path: /disLocks, content: 该节点由线程9创建
2014-11-19 11:34:10,912 - 【第9个线程】创建锁路径:/disLocks/sub0000000000
2014-11-19 11:34:10,917 - 【第6个线程】创建锁路径:/disLocks/sub0000000001
2014-11-19 11:34:10,917 - 【第9个线程】子节点中，我果然是老大/disLocks/sub0000000000
2014-11-19 11:34:10,921 - 【第3个线程】创建锁路径:/disLocks/sub0000000002
2014-11-19 11:34:10,922 - 【第6个线程】获取子节点中，排在我前面的/disLocks/sub0000000000
2014-11-19 11:34:10,923 - 【第9个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:10,924 - 【第10个线程】创建锁路径:/disLocks/sub0000000003
2014-11-19 11:34:10,924 - 【第3个线程】获取子节点中，排在我前面的/disLocks/sub0000000001
2014-11-19 11:34:10,928 - 【第10个线程】获取子节点中，排在我前面的/disLocks/sub0000000002
2014-11-19 11:34:10,929 - 【第1个线程】创建锁路径:/disLocks/sub0000000004
2014-11-19 11:34:10,932 - 【第5个线程】创建锁路径:/disLocks/sub0000000005
2014-11-19 11:34:10,935 - 【第1个线程】获取子节点中，排在我前面的/disLocks/sub0000000003
2014-11-19 11:34:10,936 - 【第2个线程】创建锁路径:/disLocks/sub0000000006
2014-11-19 11:34:10,936 - 【第5个线程】获取子节点中，排在我前面的/disLocks/sub0000000004
2014-11-19 11:34:10,940 - 【第4个线程】创建锁路径:/disLocks/sub0000000007
2014-11-19 11:34:10,941 - 【第2个线程】获取子节点中，排在我前面的/disLocks/sub0000000005
2014-11-19 11:34:10,943 - 【第8个线程】创建锁路径:/disLocks/sub0000000008
2014-11-19 11:34:10,944 - 【第4个线程】获取子节点中，排在我前面的/disLocks/sub0000000006
2014-11-19 11:34:10,945 - 【第7个线程】创建锁路径:/disLocks/sub0000000009
2014-11-19 11:34:10,946 - 【第8个线程】获取子节点中，排在我前面的/disLocks/sub0000000007
2014-11-19 11:34:10,947 - 【第7个线程】获取子节点中，排在我前面的/disLocks/sub0000000008
2014-11-19 11:34:12,923 - 【第9个线程】删除本节点：/disLocks/sub0000000000
2014-11-19 11:34:12,926 - 【第6个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:12,928 - 【第6个线程】子节点中，我果然是老大/disLocks/sub0000000001
2014-11-19 11:34:12,930 - 【第9个线程】释放连接
2014-11-19 11:34:12,930 - 【第6个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:14,930 - 【第6个线程】删除本节点：/disLocks/sub0000000001
2014-11-19 11:34:14,937 - 【第3个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:14,941 - 【第3个线程】子节点中，我果然是老大/disLocks/sub0000000002
2014-11-19 11:34:14,943 - 【第6个线程】释放连接
2014-11-19 11:34:14,946 - 【第3个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:16,946 - 【第3个线程】删除本节点：/disLocks/sub0000000002
2014-11-19 11:34:16,949 - 【第10个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:16,951 - 【第10个线程】子节点中，我果然是老大/disLocks/sub0000000003
2014-11-19 11:34:16,953 - 【第3个线程】释放连接
2014-11-19 11:34:16,953 - 【第10个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:18,953 - 【第10个线程】删除本节点：/disLocks/sub0000000003
2014-11-19 11:34:18,957 - 【第1个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:18,960 - 【第10个线程】释放连接
2014-11-19 11:34:18,961 - 【第1个线程】子节点中，我果然是老大/disLocks/sub0000000004
2014-11-19 11:34:18,964 - 【第1个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:20,964 - 【第1个线程】删除本节点：/disLocks/sub0000000004
2014-11-19 11:34:20,967 - 【第5个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:20,969 - 【第5个线程】子节点中，我果然是老大/disLocks/sub0000000005
2014-11-19 11:34:20,971 - 【第1个线程】释放连接
2014-11-19 11:34:20,971 - 【第5个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:22,971 - 【第5个线程】删除本节点：/disLocks/sub0000000005
2014-11-19 11:34:22,974 - 【第2个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:22,978 - 【第2个线程】子节点中，我果然是老大/disLocks/sub0000000006
2014-11-19 11:34:22,979 - 【第5个线程】释放连接
2014-11-19 11:34:22,981 - 【第2个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:24,981 - 【第2个线程】删除本节点：/disLocks/sub0000000006
2014-11-19 11:34:24,985 - 【第4个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:24,989 - 【第2个线程】释放连接
2014-11-19 11:34:24,989 - 【第4个线程】子节点中，我果然是老大/disLocks/sub0000000007
2014-11-19 11:34:24,995 - 【第4个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:26,995 - 【第4个线程】删除本节点：/disLocks/sub0000000007
2014-11-19 11:34:26,998 - 【第8个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:27,000 - 【第8个线程】子节点中，我果然是老大/disLocks/sub0000000008
2014-11-19 11:34:27,004 - 【第8个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:27,004 - 【第4个线程】释放连接
2014-11-19 11:34:29,004 - 【第8个线程】删除本节点：/disLocks/sub0000000008
2014-11-19 11:34:29,007 - 【第7个线程】收到情报，排我前面的家伙已挂，我是不是可以出山了？
2014-11-19 11:34:29,009 - 【第7个线程】子节点中，我果然是老大/disLocks/sub0000000009
2014-11-19 11:34:29,010 - 【第8个线程】释放连接
2014-11-19 11:34:29,011 - 【第7个线程】获取锁成功，赶紧干活！
2014-11-19 11:34:31,011 - 【第7个线程】删除本节点：/disLocks/sub0000000009
2014-11-19 11:34:31,017 - 【第7个线程】释放连接
2014-11-19 11:34:31,017 - 所有线程运行结束!
```
