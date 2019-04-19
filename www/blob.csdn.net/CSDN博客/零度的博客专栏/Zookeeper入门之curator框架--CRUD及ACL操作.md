# Zookeeper入门之curator框架--CRUD及ACL操作 - 零度的博客专栏 - CSDN博客
2017年03月15日 11:49:25[零度anngle](https://me.csdn.net/zmx729618)阅读数：1628
Maven添加的依赖地址如下：
```
<!-- zk 连接客户端 start -->
         <dependency>
                <groupId>org.apache.curator</groupId>
             <artifactId>curator-framework</artifactId>
             <version>2.8.0</version>
         </dependency>
         <!-- zk 连接客户端 end -->
```
### CuratorFramework
Curator框架提供了一套高级的API， 简化了ZooKeeper的操作。 它增加了很多使用ZooKeeper开发的特性，可以处理ZooKeeper集群复杂的连接管理和重试机制。 这些特性包括：
- 自动化的连接管理: 重新建立到ZooKeeper的连接和重试机制存在一些潜在的错误case。 Curator帮助你处理这些事情，对你来说是透明的。
- 清理API: - 简化了原生的ZooKeeper的方法，事件等
- 提供了一个现代的流式接口
- 提供了Recipes实现： 如前面的文章介绍的那样，基于这些Recipes可以创建很多复杂的分布式应用
Curator框架通过CuratorFrameworkFactory以工厂模式和builder模式创建CuratorFramework实 例。 CuratorFramework实例都是线程安全的，你应该在你的应用中共享同一个CuratorFramework实例.
工厂方法newClient()提供了一个简单方式创建实例。 而Builder提供了更多的参数控制。一旦你创建了一个CuratorFramework实例，你必须调用它的start()启动，在应用退出时调用close()方法关闭.
CuratorFramework提供的方法：
|方法名|描述|
|----|----|
|create()|开始创建操作， 可以调用额外的方法(比如方式mode 或者后台执行background) 并在最后调用forPath()指定要操作的ZNode|
|delete()|开始删除操作. 可以调用额外的方法(版本或者后台处理version or background)并在最后调用forPath()指定要操作的ZNode|
|checkExists()|开始检查ZNode是否存在的操作. 可以调用额外的方法(监控或者后台处理)并在最后调用forPath()指定要操作的ZNode|
|getData()|开始获得ZNode节点数据的操作. 可以调用额外的方法(监控、后台处理或者获取状态watch, background or get stat) 并在最后调用forPath()指定要操作的ZNode|
|setData()|开始设置ZNode节点数据的操作. 可以调用额外的方法(版本或者后台处理) 并在最后调用forPath()指定要操作的ZNode|
|getChildren()|开始获得ZNode的子节点列表。 以调用额外的方法(监控、后台处理或者获取状态watch, background or get stat) 并在最后调用forPath()指定要操作的ZNode|
|inTransaction()|开始是原子ZooKeeper事务. 可以复合create, setData, check, and/or delete 等操作然后调用commit()作为一个原子操作提交|
代码DEMO实现：
```
package com.git.zookeeper.passwordmanager;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.apache.curator.RetryPolicy;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.framework.api.ACLProvider;
import org.apache.curator.framework.recipes.cache.NodeCache;
import org.apache.curator.framework.recipes.cache.NodeCacheListener;
import org.apache.curator.framework.recipes.cache.PathChildrenCache;
import org.apache.curator.framework.recipes.cache.PathChildrenCache.StartMode;
import org.apache.curator.framework.recipes.cache.PathChildrenCacheEvent;
import org.apache.curator.framework.recipes.cache.PathChildrenCacheListener;
import org.apache.curator.retry.ExponentialBackoffRetry;
import org.apache.curator.retry.RetryNTimes;
import org.apache.zookeeper.WatchedEvent;
import org.apache.zookeeper.Watcher;
import org.apache.zookeeper.ZooDefs;
import org.apache.zookeeper.ZooDefs.Perms;
import org.apache.zookeeper.data.ACL;
import org.apache.zookeeper.data.Id;
import org.apache.zookeeper.data.Stat;
/**
 * curator管理zookeeper的相关方法工具类
 * @author songqinghu
 * 要点:
 * 1.连接的建立  (两种 OK--使用权限方式)
 * 2.节点的管理操作,增删改查--层叠节点的操作(OK --设立命名空间)
 * 3.节点的监听操作,和无限监听事件触发
 * 4.节点的访问控制ACL操作,密码的添加,修改
 * 5.节点原子性操作
 * 6.节点的分布式锁操作
 */
public class ZookeeperCuratorUtils {
    /**
     * 先测试玩玩  
     * @描述：XXXXXXX
     * @param args
     * @return void
     * @exception
     * @createTime：2016年5月17日
     * @author: songqinghu
     * @throws Exception 
     */ 
    public static void main(String[] args) throws Exception {
        // nodesList(clientOne(), "/");
           CuratorFramework client = clientTwo();
        // nodesList(client, "/");
        // 使用clientTwo来创建一个节点空间 查看是加密
        // createNode(client, "/usermanager");
        // setDataNode(client, "/usermanager", "test writer 测试写入效果!");
        // getDataNode(client, "/usermanager");
        // createNode(client, "/three/two/testone");
        // deleteDataNode(client, "/three");;
    }
    /**
     * 
     * @描述：创建一个zookeeper连接---连接方式一: 最简单的连接
     * @return void
     * @exception
     * @createTime：2016年5月17日
     * @author: songqinghu
     */
    private static CuratorFramework clientOne(){
        //zk 地址
        String connectString = "10.125.2.44:2181";
        // 连接时间 和重试次数
        RetryPolicy retryPolicy = new ExponentialBackoffRetry(1000, 3);
        CuratorFramework client = CuratorFrameworkFactory.newClient(connectString, retryPolicy);
        client.start();
        return client;
    }
    
    /**
     * 
     * @描述：创建一个zookeeper连接---连接方式二:优选这个
     * @return void
     * @exception
     * @createTime：2016年5月17日
     * @author: songqinghu
     */
    private static CuratorFramework clientTwo(){
        //默认创建的根节点是没有做权限控制的--需要自己手动加权限???----
        ACLProvider aclProvider = new ACLProvider() {
            private List<ACL> acl ;
            @Override
            public List<ACL> getDefaultAcl() {
                if(acl ==null){
                    ArrayList<ACL> acl = ZooDefs.Ids.CREATOR_ALL_ACL;
                    acl.clear();
                    acl.add(new ACL(Perms.ALL, new Id("auth", "admin:admin") ));
                    this.acl = acl;
                }
                return acl;
            }
            @Override
            public List<ACL> getAclForPath(String path) {
                return acl;
            }
        };
        String scheme = "digest";
        byte[] auth = "admin:admin".getBytes();
        int connectionTimeoutMs = 5000;
        String connectString = "10.125.2.44:2181";
        String namespace = "testnamespace";
        CuratorFramework client = CuratorFrameworkFactory.builder().aclProvider(aclProvider).
        authorization(scheme, auth).
        connectionTimeoutMs(connectionTimeoutMs).
        connectString(connectString).
         namespace(namespace).
        retryPolicy(new RetryNTimes(Integer.MAX_VALUE, 1000)).build();
        client.start();
        return client;
    }
    
    /**
     * 
     * @描述：获取子节点列表 打印
     * @return void
     * @exception
     * @createTime：2016年5月17日
     * @author: songqinghu
     * @throws Exception 
     */
    private static void nodesList(CuratorFramework client,String parentPath) throws Exception{
        List<String> paths = client.getChildren().forPath(parentPath);
        for (String path : paths) {
            System.out.println(path);
        }
    }
    /**
     * 
     * @描述：创建一个节点 
     * @param path
     * @return void
     * @exception
     * @createTime：2016年5月17日
     * @author: songqinghu
     * @throws Exception 
     */
    private static void createNode(CuratorFramework client,String path) throws Exception{
        
        Stat stat = client.checkExists().forPath(path);
        System.out.println(stat);
        String forPath = client.create().creatingParentsIfNeeded().forPath(path, "create init !".getBytes());
       // String forPath = client.create().forPath(path);
        System.out.println(forPath);
    }
    
    /**
     * 获取指定节点中信息
     * @throws Exception 
     */
    private static void getDataNode(CuratorFramework client,String path) throws Exception{
        Stat stat = client.checkExists().forPath(path);
        System.out.println(stat);
        byte[] datas = client.getData().forPath(path);
        System.out.println(new String(datas));
    }
    /**
     * 
     * @描述：设置节点中的信息
     * @param client
     * @param path
     * @param message
     * @return void
     * @exception
     * @createTime：2016年5月17日
     * @author: songqinghu
     * @throws Exception 
     */
    private static void setDataNode(CuratorFramework client,String path,String message) throws Exception{
        
        Stat stat = client.checkExists().forPath(path);
        System.out.println(stat);
        client.setData().forPath(path, message.getBytes());
    }
    
    private static void deleteDataNode(CuratorFramework client,String path) throws Exception{
        
        Stat stat = client.checkExists().forPath(path);
        System.out.println("deleteNode : "+stat);
        
        Void forPath = client.delete().deletingChildrenIfNeeded().forPath(path);
        
        System.out.println(forPath);
    }
}
```
