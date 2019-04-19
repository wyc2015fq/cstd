# 分布式应用技术(zookeeper的分布式应用) - 零度的博客专栏 - CSDN博客
2016年11月03日 14:36:09[零度anngle](https://me.csdn.net/zmx729618)阅读数：384
本文讲一些常见的分布式应用层面的技术，其中大部分都依赖于Zookeeper，所以对zookeeper不熟悉的同学可以先看我之前写的两往篇博客[Zookeeper编程（二）](http://www.cnblogs.com/zhangchaoyang/articles/3813217.html)、[Zookeeper编程（一）](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html)
## 一、分布式锁
我们的推荐程序部署在多台服务器上，每天凌晨会去重建信息索引（索引存在Redis上）。建索引这件事情只能让一台服务器去做，其他服务器歇着，这种应用场景自然想到了分布式锁，谁抢到锁谁来建索引。我们在线上分别使用过3种分布式锁：基于zookeeper的锁，基于[MySQL](http://lib.csdn.net/base/14)的锁，基于Redis的锁。MySQL
 lock最稳定；Redis lock使用起来最灵活，同时实时起来也最方便。所以现在我们线上的分布式锁全换成了Redis Lock。
[zookeeper lock的实现原理](http://www.cnblogs.com/zhangchaoyang/articles/2536178.html#lock)参见链接，不再赘述。
MySQL lock的基本思想是大家都去写同一条数据库记录，谁先写上谁获取锁，删除这条记录就相当于释放了锁。整个流程看起来像这样：
```
begin;
select count(*) from table where lockname='xxx' for update;
if count==**0**:
    insert into table (lockname) values ('xxx);
    do the task which must be done once
    delete from table where lockname='xxx'
commit;
```
如果2个进程同时执行第一步，发现lockname不存在，于是都去添加一行记录，还都添加成功了，那岂不是2个进程都获得了锁？莫急，看见select语句后面有个"for update"吗？当where条件中不包含主键时，select ... for update将会锁表，事务提交后才释放表上的锁。（for update仅适用于InnoDB）
Redis lock的实现思想跟mysql相同，不过操作起来更简单。看下面的代码
```
Long i = jedis.setnx(lockName, lockName);    // 若key不存在，则存储 ，并返回1
if (i == 1L) {
    // 设置key的过期时间
    if (live < 0) {
        live = DEFAULT_EXPIRE_TIME;
    }
    jedis.expire(lockName, live);
    logger.info("get redis lock " + lockName + " ,live " + live
            + " seconds");
    rect = true;        //获得锁返回true
} else { // 已存在锁
    logger.info("lockName: " + lockName
            + " locked by other business");
    rect = false;        //没有获得锁返回false
}
```
核心操作是redis提供的setnx()方法，它来保证并发情况下中有1个进程能写成功。另外我们还为redis的key设置了超时时间，即使你获得锁后忘记了释放锁，或者在释放锁之前进程死掉了，不用担心，在达到超时时间后该锁也是会自动释放的。
## 二、Barrier
接着上面的应用场景讲。在推荐系统中，建完信息索引后就要开始为每个用户进行推荐了。推荐任务要分发到每台服务器上去执行，我们没有做单独的任务分发器，而是每台服务器都去同一个数据表里读取所有的用户ID，userid % n == 自己的编号时（n是服务器总数），该服务器就计算这个用户的推荐。计算推荐的过程也伴随着计算用户兴趣，所有用户的推荐计算完毕后，兴趣也就计算完毕了，此时又需要建立兴趣索引。建立兴趣索引又是只能由一台服务器来做的事情。这里有2个关键节点，即必须建完兴趣索引后所有服务器才能开始计算推荐，所有服务器计算完推荐后才能开始建兴趣索引。分布式环境下各服务器之间要想达成这种默契就需要借助于DoubleBarrier。
Barrier是指：
1）所有的线程都到达barrier后才能进行后续的计算
或者
2）所有的线程都完成自己的计算后才能离开barrier
Double Barrier是指同时具有上述两点。
Double Barrier的实现：
```
enter barrier:
1.建一个根节点"/root"
2.想进入barrier的线程在"/root"下建立一个子节点"/root/c_i"
3.循环监听"/root"孩子节点数目的变化，当其达到size时就说明有size个线程都已经barrier点了
leave barrier:
1.想离开barrier的线程删除其在"/root"下建立的子节点
2.循环监听"/root"孩子节点数目的变化，当size减到0时它就可以离开barrier了
```
## 三、服务注册
继续研究上面的应用场景，我们提到每台服务器遇到“userid % n == 自己的编号时（n是服务器总数）”这样的用户时才为其计算推荐，这里有两个问题：
- 集群中服务器的总数如何获取？如果直接设置成上线的服务器的个数会存在2个问题：将来服务器数目增加了n还得跟着改；如果哪天某台服务器进程挂了，那就造成1/n的用户没有推荐数据。
- 本服务器在集群中的编号如何获得？
解决办法是：
每台服务器进程启动时在特定的zookeeper路径下添加一个EPHEMERAL节点，节点是存储的数据为自己的IP（或者其他能唯一标识一台服务器的东西）。之所以要求是EPHEMERAL类型，是因为当进程死掉后该zookeeper节点会自动被删除掉。每天凌晨每台服务器去获取特定zookeeper路径下所有的子节点，子节点数目即为集群中服务器总数。根据IP每台服务器就可以知道自己在所有的孩子节点中排名第几。
ServerCluster.java
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52415314#)[copy](http://blog.csdn.net/z69183787/article/details/52415314#)[print](http://blog.csdn.net/z69183787/article/details/52415314#)[?](http://blog.csdn.net/z69183787/article/details/52415314#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1863459/fork)
- import java.util.List;  
- import java.util.concurrent.Executors;  
- import java.util.concurrent.ScheduledExecutorService;  
- import java.util.concurrent.TimeUnit;  
- 
- import org.apache.commons.logging.Log;  
- import org.apache.commons.logging.LogFactory;  
- import org.apache.curator.framework.CuratorFramework;  
- import org.apache.curator.framework.api.CreateBuilder;  
- import org.apache.zookeeper.CreateMode;  
- 
- 
- /**
-  * 
-  * @Author:orisun
-  * @Since:2016-4-7
-  * @Version:1.0
-  */
- publicclass ServerCluster {  
- 
- privatestatic Log logger = LogFactory.getLog(ServerCluster.class);  
- privatestaticfinal String BASE_PATH = ZkClient.getInstance().getBasePath() + "/cluster";  
- privatestatic ScheduledExecutorService exec = Executors  
-             .newSingleThreadScheduledExecutor();  
- 
- /**
-      * 向集群上报自己的存在，即把自己的IP写到特定的zk节点(EPHEMERAL节点)上去
-      */
- publicstaticvoid reportServer() {  
-         String selfIP = NIC.getLocalIP();  
-         CuratorFramework zkClient = ZkClient.getInstance().getZkClient();  
- boolean exists = false;  
- try {  
-             CreateBuilder cb = zkClient.create();  
- if (zkClient.checkExists().forPath(BASE_PATH) == null) {  
-                 cb.creatingParentsIfNeeded().forPath(BASE_PATH,  
- newbyte[] { 0 });  
-             }  
-             List<String> children = zkClient.getChildren().forPath(BASE_PATH);  
- if (children != null && children.indexOf(selfIP) >= 0) {  
-                 exists = true;  
-             }  
- if (!exists) {  
- // EPHEMERAL节点，进程终止时zookeeper连接断开，节点自动被删除
-                 cb.creatingParentsIfNeeded().withMode(CreateMode.EPHEMERAL)  
-                         .forPath(BASE_PATH + "/" + selfIP, newbyte[] { 0 });  
-                 logger.info(selfIP + " add to cluster");  
-             } else {  
- // 如果发现cluster上已存在该IP，则5秒后再确认一下
-                 logger.info(selfIP + " is already in cluster");  
-                 Thread.sleep(1000 * 5);  
-                 children = zkClient.getChildren().forPath(BASE_PATH);  
-                 exists = false;  
- if (children != null && children.indexOf(selfIP) >= 0) {  
-                     exists = true;  
-                 }  
- if (!exists) {  
- // EPHEMERAL节点，进程终止时zookeeper连接断开，节点自动被删除
-                     cb.creatingParentsIfNeeded()  
-                             .withMode(CreateMode.EPHEMERAL)  
-                             .forPath(BASE_PATH + "/" + selfIP, newbyte[] { 0 });  
-                     logger.info(selfIP + " add to cluster");  
-                 }  
-             }  
-         } catch (Exception e) {  
-             logger.fatal("report to cluster failed", e);  
-         }  
-     }  
- 
- /**
-      * 向集群上报自己的存在，即把自己的IP写到特定的zk节点(EPHEMERAL节点)上去<br>
-      * 为防止zookeeper会话断开而造成节点被删除，每隔10分钟就去写一次
-      */
- publicstaticvoid report() {  
-         exec.scheduleAtFixedRate(new Runnable() {  
- @Override
- publicvoid run() {  
-                 reportServer();  
-             }  
-         }, 0, 10, TimeUnit.MINUTES);  
-     }  
- 
- /**
-      * 获取集群中有多少台机器
-      * 
-      * @return
-      */
- publicstaticint getClusterSize() {  
- int total = 0;  
-         List<String> children = null;  
- try {  
-             CuratorFramework zkClient = ZkClient.getInstance().getZkClient();  
-             children = zkClient.getChildren().forPath(BASE_PATH);  
-         } catch (Exception e) {  
-             logger.error("get children of " + BASE_PATH + " failed", e);  
-         }  
- if (children != null) {  
-             total = children.size();  
-         }  
-         logger.info("cluster size is " + total);  
- return total;  
-     }  
- 
- /**
-      * 获取自己在集群中的编码(从0开始)
-      * 
-      * @return
-      */
- publicstaticint getIndexInCluster() {  
- int index = -1;  
-         CuratorFramework zkClient = ZkClient.getInstance().getZkClient();  
- try {  
-             List<String> children = zkClient.getChildren().forPath(BASE_PATH);  
-             String selfIP = NIC.getLocalIP();  
-             index = children.indexOf(selfIP);  
-         } catch (Exception e) {  
-             logger.fatal("get cluster info failed", e);  
-         }  
-         logger.info("this server's index is " + index);  
- return index;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.api.CreateBuilder;
import org.apache.zookeeper.CreateMode;
/**
 * 
 * @Author:orisun
 * @Since:2016-4-7
 * @Version:1.0
 */
public class ServerCluster {
    private static Log logger = LogFactory.getLog(ServerCluster.class);
    private static final String BASE_PATH = ZkClient.getInstance().getBasePath() + "/cluster";
    private static ScheduledExecutorService exec = Executors
            .newSingleThreadScheduledExecutor();
    /**
     * 向集群上报自己的存在，即把自己的IP写到特定的zk节点(EPHEMERAL节点)上去
     */
    public static void reportServer() {
        String selfIP = NIC.getLocalIP();
        CuratorFramework zkClient = ZkClient.getInstance().getZkClient();
        boolean exists = false;
        try {
            CreateBuilder cb = zkClient.create();
            if (zkClient.checkExists().forPath(BASE_PATH) == null) {
                cb.creatingParentsIfNeeded().forPath(BASE_PATH,
                        new byte[] { 0 });
            }
            List<String> children = zkClient.getChildren().forPath(BASE_PATH);
            if (children != null && children.indexOf(selfIP) >= 0) {
                exists = true;
            }
            if (!exists) {
                // EPHEMERAL节点，进程终止时zookeeper连接断开，节点自动被删除
                cb.creatingParentsIfNeeded().withMode(CreateMode.EPHEMERAL)
                        .forPath(BASE_PATH + "/" + selfIP, new byte[] { 0 });
                logger.info(selfIP + " add to cluster");
            } else {
                // 如果发现cluster上已存在该IP，则5秒后再确认一下
                logger.info(selfIP + " is already in cluster");
                Thread.sleep(1000 * 5);
                children = zkClient.getChildren().forPath(BASE_PATH);
                exists = false;
                if (children != null && children.indexOf(selfIP) >= 0) {
                    exists = true;
                }
                if (!exists) {
                    // EPHEMERAL节点，进程终止时zookeeper连接断开，节点自动被删除
                    cb.creatingParentsIfNeeded()
                            .withMode(CreateMode.EPHEMERAL)
                            .forPath(BASE_PATH + "/" + selfIP, new byte[] { 0 });
                    logger.info(selfIP + " add to cluster");
                }
            }
        } catch (Exception e) {
            logger.fatal("report to cluster failed", e);
        }
    }
    /**
     * 向集群上报自己的存在，即把自己的IP写到特定的zk节点(EPHEMERAL节点)上去<br>
     * 为防止zookeeper会话断开而造成节点被删除，每隔10分钟就去写一次
     */
    public static void report() {
        exec.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                reportServer();
            }
        }, 0, 10, TimeUnit.MINUTES);
    }
    /**
     * 获取集群中有多少台机器
     * 
     * @return
     */
    public static int getClusterSize() {
        int total = 0;
        List<String> children = null;
        try {
            CuratorFramework zkClient = ZkClient.getInstance().getZkClient();
            children = zkClient.getChildren().forPath(BASE_PATH);
        } catch (Exception e) {
            logger.error("get children of " + BASE_PATH + " failed", e);
        }
        if (children != null) {
            total = children.size();
        }
        logger.info("cluster size is " + total);
        return total;
    }
    /**
     * 获取自己在集群中的编码(从0开始)
     * 
     * @return
     */
    public static int getIndexInCluster() {
        int index = -1;
        CuratorFramework zkClient = ZkClient.getInstance().getZkClient();
        try {
            List<String> children = zkClient.getChildren().forPath(BASE_PATH);
            String selfIP = NIC.getLocalIP();
            index = children.indexOf(selfIP);
        } catch (Exception e) {
            logger.fatal("get cluster info failed", e);
        }
        logger.info("this server's index is " + index);
        return index;
    }
}
```
## 四、消息通知
 经常有这样的业务场景，一台服务器执行完任务A后，需要通达其他几台服务器执行任务B。当然们可以借助于MQ框架来实现这种通知的功能，但是这种消息通常一天只发一次，为其单独创建一个消息主题未免太浪费。于是我们又想到了zookeeper，各服务器去监听同一个zookeeper节点的变化，当一台服务器执行完任务A后去把zookeeper节点的值更新当时时间，其他服务器监听到zookeeper节点有变化后，先确认一下变化为UPDATE类型，然后执行任务B，执行完后再去zookeeper节点上添加一个watcher（因为watcher只能使用一次，所以需要重复监听）。在执行任务B的过程中如果节点有变化，其他服务器是感知不到的，这样就防止了任务B在同一个进程内并行执行。
## 五、配置管理
在推荐系统中有很多配置参数，我们不想因为参数的改动而走一次上线流程（要知道参数的改动还是比较频繁的）。简单的做法是参数都写入数据库中，要改参数直接改数据库即可，程序每天去跟数据库同步一次参数。这种做法缺点很明显，就是会有一天的延迟。为了能让程序能实时感知到参数的变化，我们把参数写在zookeeper上，程序去监听zookeeper节点的变化，当有变化时从zookeeper上读取最新有值即可。考虑到zookeeper的稳定时不如MySQL，我们会在MySQL上同时存一份配置参数，通过后台修改参数时会同时修改MySQL和zookeeper。每天应用程序去跟MySQL做一次同步，zookeeper仅作为实时通知的一个工具存在。
 ZkParam.java
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52415314#)[copy](http://blog.csdn.net/z69183787/article/details/52415314#)[print](http://blog.csdn.net/z69183787/article/details/52415314#)[?](http://blog.csdn.net/z69183787/article/details/52415314#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1863459/fork)
- /**
-  * 
-  *@Author:orisun 
-  *@Since:2016-4-7  
-  *@Version:1.0
-  */
- publicclass ZkParam {  
- 
- privatedouble value;  
- private String path;  
- privateint logicid;  
- 
- public ZkParam(double defaultValue, String path, int logicid) {  
- this.value = defaultValue;  
- this.path = path;  
- this.logicid = logicid;  
-     }  
- 
- publicdouble getValue() {  
- return value;  
-     }  
- 
- publicvoid setValue(double value) {  
- this.value = value;  
-     }  
- 
- public String getPath() {  
- return path;  
-     }  
- 
- publicvoid setPath(String path) {  
- this.path = path;  
-     }  
- 
- publicint getLogicid() {  
- return logicid;  
-     }  
- 
- publicvoid setLogicid(int logicid) {  
- this.logicid = logicid;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
/**
 * 
 *@Author:orisun 
 *@Since:2016-4-7  
 *@Version:1.0
 */
public class ZkParam {
    private double value;
    private String path;
    private int logicid;
    public ZkParam(double defaultValue, String path, int logicid) {
        this.value = defaultValue;
        this.path = path;
        this.logicid = logicid;
    }
    public double getValue() {
        return value;
    }
    public void setValue(double value) {
        this.value = value;
    }
    public String getPath() {
        return path;
    }
    public void setPath(String path) {
        this.path = path;
    }
    public int getLogicid() {
        return logicid;
    }
    public void setLogicid(int logicid) {
        this.logicid = logicid;
    }
}
```
ZkConfig.java
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52415314#)[copy](http://blog.csdn.net/z69183787/article/details/52415314#)[print](http://blog.csdn.net/z69183787/article/details/52415314#)[?](http://blog.csdn.net/z69183787/article/details/52415314#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1863459/fork)
- import java.lang.reflect.Field;  
- import java.lang.reflect.InvocationTargetException;  
- import java.lang.reflect.Method;  
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- 
- import org.apache.commons.logging.Log;  
- import org.apache.commons.logging.LogFactory;  
- import org.apache.curator.framework.CuratorFramework;  
- import org.apache.curator.framework.api.CreateBuilder;  
- import org.apache.curator.framework.recipes.cache.NodeCache;  
- import org.apache.curator.framework.recipes.cache.NodeCacheListener;  
- 
- /**
-  * 
-  * @Author:orisun
-  * @Since:2016-4-7
-  * @Version:1.0
-  */
- publicabstractclass ZkConfig {  
- 
- privatestatic Log logger = LogFactory.getLog(ZkConfig.class);  
- privatestaticfinal String ZK_PARAM_NAME = ZkParam.class
-             .getCanonicalName();  
- private Class<?> zkArgClz = null;  
- privatestatic Method getZkPathMethod = null;  
- privatestatic Method getIdMethod = null;  
- privatestatic Method getValueMethod = null;  
- private ExecutorService exec = null;  
- 
- public ZkConfig() {  
-         exec = Executors.newCachedThreadPool();  
- try {  
-             zkArgClz = Class.forName(ZK_PARAM_NAME);  
-             getZkPathMethod = zkArgClz.getMethod("getPath");  
-             getIdMethod = zkArgClz.getMethod("getLogicid");  
-             getValueMethod = zkArgClz.getMethod("getValue");  
-         } catch (Exception e) {  
-             logger.fatal("build " + ZK_PARAM_NAME + " failed", e);  
-             System.exit(1);  
-         }  
-     }  
- 
- publicvoid updateParam(String filedName, ZkParam newArgument) {  
- try {  
-             Method method = this.getClass().getMethod(pareSetName(filedName),  
-                     ZkParam.class);  
-             method.invoke(this, newArgument);  
-             logger.info("set " + filedName + " to " + newArgument.getValue());  
-         } catch (NoSuchMethodException | SecurityException  
-                 | IllegalAccessException | IllegalArgumentException  
-                 | InvocationTargetException e) {  
-             logger.error(  
- "zookeeper node is changed, but update system parameter failed",  
-                     e);  
-         }  
-     }  
- 
- /**
-      * 添加zookeeper监听，参数变化时及时反应到推荐系统中
-      */
- @SuppressWarnings("resource")  
- privatevoid addListener() {  
- final CuratorFramework zkClient = ZkClient.getInstance().getZkClient();  
- try {  
-             Field[] fields = this.getClass().getDeclaredFields();// 父类中的成员获取不到
- for (final Field field : fields) {  
-                 field.setAccessible(true);  
- if (field.getType().getCanonicalName().equals(ZK_PARAM_NAME)) {  
-                     Object zkParamInst = field.get(this);  
- final String path = (String) getZkPathMethod  
-                             .invoke(zkParamInst);  
- if (zkClient.checkExists().forPath(path) != null) {  
- finalint logicid = (int) getIdMethod  
-                                 .invoke(zkParamInst);  
-                         NodeCache nodeCache = new NodeCache(zkClient, path,  
- false);  
-                         nodeCache.start(true);  
-                         nodeCache.getListenable().addListener(  
- new NodeCacheListener() {  
- @Override
- publicvoid nodeChanged() throws Exception {  
- byte[] brr = zkClient.getData()  
-                                                 .forPath(path);  
- double newValue = Double.parseDouble(new String(  
-                                                 brr));  
-                                         ZkParam newArgument = new ZkParam(  
-                                                 newValue, path, logicid);  
-                                         updateParam(field.getName(),  
-                                                 newArgument);  
-                                     }  
-                                 }, exec);  
-                         logger.info("add listener to " + path);  
-                     } else {  
-                         logger.error("will add listner on zookeeper path "
-                                 + path + ", but it dose not exists");  
-                     }  
-                 }  
-             }  
-         } catch (Exception e) {  
-             logger.error("add listener to zookeeper failed", e);  
-             SendMail.sendMail(SystemConfig.getValue("mail_subject"),  
-                     SystemConfig.getValue("mail_receiver"),  
- "add listener to zookeeper failed<br>" + e.getMessage());  
-         }  
-     }  
- 
- /**
-      * 每天定时任务，从MySQL中读取参数的值及参数对应的zkpath，然后监听该zkpath。<br>
-      * 这是为了防止zookeeper连不上，或watcher机制失效
-      */
- publicvoid readFromMysql() {  
- try {  
- final ParamConfigDao dao = new ParamConfigDao();  
-             Field[] fields = this.getClass().getDeclaredFields();  
- for (Field field : fields) {  
-                 field.setAccessible(true);  
- if (field.getType().getCanonicalName().equals(ZK_PARAM_NAME)) {  
-                     Object zkParamInst = field.get(this);  
-                     String path0 = (String) getZkPathMethod.invoke(zkParamInst);  
- int logicid = (int) getIdMethod.invoke(zkParamInst);  
-                     ParamConfig param = dao.getByLogicId(logicid);  
- if (param != null) {  
-                         String path1 = param.getZkpath();  
-                         String path = (path1 != null && path1.length() > 0) ? path1  
-                                 : path0;  
- if (param != null) {  
-                             ZkParam newArgument = new ZkParam(param.getValue(),  
-                                     path, logicid);  
-                             Method method = this.getClass()  
-                                     .getMethod(pareSetName(field.getName()),  
-                                             ZkParam.class);  
-                             method.invoke(this, newArgument);  
-                         }  
-                     } else {  
-                         logger.error("have no such param whoese logicid is "
-                                 + logicid + " im mysql");  
-                     }  
-                 }  
-             }  
-         } catch (Exception e) {  
-             logger.error("read param from mysql failed", e);  
-             SendMail.sendMail(SystemConfig.getValue("mail_subject"),  
-                     SystemConfig.getValue("mail_receiver"),  
- "read param from mysql failed<br>" + e.getMessage());  
-         }  
-         addListener();  
-     }  
- 
- /**
-      * 把参数写入到zookeeper
-      * 
-      * 
-      */
- publicvoid flushToZookeeper() {  
- try {  
-             CuratorFramework zkClient = ZkClient.getInstance().getZkClient();  
-             CreateBuilder cb = zkClient.create();  
-             Field[] fields = this.getClass().getDeclaredFields();  
- for (final Field field : fields) {  
-                 field.setAccessible(true);  
- if (field.getType().getCanonicalName().equals(ZK_PARAM_NAME)) {  
-                     Object zkParamInst = field.get(this);  
-                     String path = (String) getZkPathMethod.invoke(zkParamInst);  
- // 写之前先保证路径是存在的
- if (zkClient.checkExists().forPath(path) == null) {  
-                         cb.creatingParentsIfNeeded().forPath(path,  
- newbyte[] { 0 });  
-                     }  
- double value = (double) getValueMethod.invoke(zkParamInst);  
-                     zkClient.setData().forPath(path,  
-                             String.valueOf(value).getBytes());  
-                 }  
-             }  
-         } catch (Exception e) {  
-             logger.error("flush behavior weight to zookeeper failed", e);  
-             SendMail.sendMail(  
-                     SystemConfig.getValue("mail_subject"),  
-                     SystemConfig.getValue("mail_receiver"),  
- "flush behavior weight to zookeeper failed<br>"
-                             + e.getMessage());  
-         }  
-     }  
- 
- /**
-      * 拼接某属性set 方法
-      * 
-      * @param fldname
-      * @return
-      */
- publicstatic String pareSetName(String fldname) {  
- if (null == fldname || "".equals(fldname)) {  
- returnnull;  
-         }  
-         String pro = "set" + fldname.substring(0, 1).toUpperCase()  
-                 + fldname.substring(1);  
- return pro;  
-     }  
- 
- /**
-      * 拼接某属性get 方法
-      * 
-      * @param fldname
-      * @return
-      */
- publicstatic String pareGetName(String fldname) {  
- if (null == fldname || "".equals(fldname)) {  
- returnnull;  
-         }  
-         String pro = "get" + fldname.substring(0, 1).toUpperCase()  
-                 + fldname.substring(1);  
- return pro;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.api.CreateBuilder;
import org.apache.curator.framework.recipes.cache.NodeCache;
import org.apache.curator.framework.recipes.cache.NodeCacheListener;
/**
 * 
 * @Author:orisun
 * @Since:2016-4-7
 * @Version:1.0
 */
public abstract class ZkConfig {
    private static Log logger = LogFactory.getLog(ZkConfig.class);
    private static final String ZK_PARAM_NAME = ZkParam.class
            .getCanonicalName();
    private Class<?> zkArgClz = null;
    private static Method getZkPathMethod = null;
    private static Method getIdMethod = null;
    private static Method getValueMethod = null;
    private ExecutorService exec = null;
    public ZkConfig() {
        exec = Executors.newCachedThreadPool();
        try {
            zkArgClz = Class.forName(ZK_PARAM_NAME);
            getZkPathMethod = zkArgClz.getMethod("getPath");
            getIdMethod = zkArgClz.getMethod("getLogicid");
            getValueMethod = zkArgClz.getMethod("getValue");
        } catch (Exception e) {
            logger.fatal("build " + ZK_PARAM_NAME + " failed", e);
            System.exit(1);
        }
    }
    public void updateParam(String filedName, ZkParam newArgument) {
        try {
            Method method = this.getClass().getMethod(pareSetName(filedName),
                    ZkParam.class);
            method.invoke(this, newArgument);
            logger.info("set " + filedName + " to " + newArgument.getValue());
        } catch (NoSuchMethodException | SecurityException
                | IllegalAccessException | IllegalArgumentException
                | InvocationTargetException e) {
            logger.error(
                    "zookeeper node is changed, but update system parameter failed",
                    e);
        }
    }
    /**
     * 添加zookeeper监听，参数变化时及时反应到推荐系统中
     */
    @SuppressWarnings("resource")
    private void addListener() {
        final CuratorFramework zkClient = ZkClient.getInstance().getZkClient();
        try {
            Field[] fields = this.getClass().getDeclaredFields();// 父类中的成员获取不到
            for (final Field field : fields) {
                field.setAccessible(true);
                if (field.getType().getCanonicalName().equals(ZK_PARAM_NAME)) {
                    Object zkParamInst = field.get(this);
                    final String path = (String) getZkPathMethod
                            .invoke(zkParamInst);
                    if (zkClient.checkExists().forPath(path) != null) {
                        final int logicid = (int) getIdMethod
                                .invoke(zkParamInst);
                        NodeCache nodeCache = new NodeCache(zkClient, path,
                                false);
                        nodeCache.start(true);
                        nodeCache.getListenable().addListener(
                                new NodeCacheListener() {
                                    @Override
                                    public void nodeChanged() throws Exception {
                                        byte[] brr = zkClient.getData()
                                                .forPath(path);
                                        double newValue = Double.parseDouble(new String(
                                                brr));
                                        ZkParam newArgument = new ZkParam(
                                                newValue, path, logicid);
                                        updateParam(field.getName(),
                                                newArgument);
                                    }
                                }, exec);
                        logger.info("add listener to " + path);
                    } else {
                        logger.error("will add listner on zookeeper path "
                                + path + ", but it dose not exists");
                    }
                }
            }
        } catch (Exception e) {
            logger.error("add listener to zookeeper failed", e);
            SendMail.sendMail(SystemConfig.getValue("mail_subject"),
                    SystemConfig.getValue("mail_receiver"),
                    "add listener to zookeeper failed<br>" + e.getMessage());
        }
    }
    /**
     * 每天定时任务，从MySQL中读取参数的值及参数对应的zkpath，然后监听该zkpath。<br>
     * 这是为了防止zookeeper连不上，或watcher机制失效
     */
    public void readFromMysql() {
        try {
            final ParamConfigDao dao = new ParamConfigDao();
            Field[] fields = this.getClass().getDeclaredFields();
            for (Field field : fields) {
                field.setAccessible(true);
                if (field.getType().getCanonicalName().equals(ZK_PARAM_NAME)) {
                    Object zkParamInst = field.get(this);
                    String path0 = (String) getZkPathMethod.invoke(zkParamInst);
                    int logicid = (int) getIdMethod.invoke(zkParamInst);
                    ParamConfig param = dao.getByLogicId(logicid);
                    if (param != null) {
                        String path1 = param.getZkpath();
                        String path = (path1 != null && path1.length() > 0) ? path1
                                : path0;
                        if (param != null) {
                            ZkParam newArgument = new ZkParam(param.getValue(),
                                    path, logicid);
                            Method method = this.getClass()
                                    .getMethod(pareSetName(field.getName()),
                                            ZkParam.class);
                            method.invoke(this, newArgument);
                        }
                    } else {
                        logger.error("have no such param whoese logicid is "
                                + logicid + " im mysql");
                    }
                }
            }
        } catch (Exception e) {
            logger.error("read param from mysql failed", e);
            SendMail.sendMail(SystemConfig.getValue("mail_subject"),
                    SystemConfig.getValue("mail_receiver"),
                    "read param from mysql failed<br>" + e.getMessage());
        }
        addListener();
    }
    /**
     * 把参数写入到zookeeper
     * 
     * 
     */
    public void flushToZookeeper() {
        try {
            CuratorFramework zkClient = ZkClient.getInstance().getZkClient();
            CreateBuilder cb = zkClient.create();
            Field[] fields = this.getClass().getDeclaredFields();
            for (final Field field : fields) {
                field.setAccessible(true);
                if (field.getType().getCanonicalName().equals(ZK_PARAM_NAME)) {
                    Object zkParamInst = field.get(this);
                    String path = (String) getZkPathMethod.invoke(zkParamInst);
                    // 写之前先保证路径是存在的
                    if (zkClient.checkExists().forPath(path) == null) {
                        cb.creatingParentsIfNeeded().forPath(path,
                                new byte[] { 0 });
                    }
                    double value = (double) getValueMethod.invoke(zkParamInst);
                    zkClient.setData().forPath(path,
                            String.valueOf(value).getBytes());
                }
            }
        } catch (Exception e) {
            logger.error("flush behavior weight to zookeeper failed", e);
            SendMail.sendMail(
                    SystemConfig.getValue("mail_subject"),
                    SystemConfig.getValue("mail_receiver"),
                    "flush behavior weight to zookeeper failed<br>"
                            + e.getMessage());
        }
    }
    /**
     * 拼接某属性set 方法
     * 
     * @param fldname
     * @return
     */
    public static String pareSetName(String fldname) {
        if (null == fldname || "".equals(fldname)) {
            return null;
        }
        String pro = "set" + fldname.substring(0, 1).toUpperCase()
                + fldname.substring(1);
        return pro;
    }
    /**
     * 拼接某属性get 方法
     * 
     * @param fldname
     * @return
     */
    public static String pareGetName(String fldname) {
        if (null == fldname || "".equals(fldname)) {
            return null;
        }
        String pro = "get" + fldname.substring(0, 1).toUpperCase()
                + fldname.substring(1);
        return pro;
    }
}
```
使用示例
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52415314#)[copy](http://blog.csdn.net/z69183787/article/details/52415314#)[print](http://blog.csdn.net/z69183787/article/details/52415314#)[?](http://blog.csdn.net/z69183787/article/details/52415314#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1863459/fork)
- /**
-  * 
-  * @Author:orisun
-  * @Since:2016-4-7
-  * @Version:1.0
-  */
- publicclass TalentRecParamConfig extends ZkConfig {  
- 
- privatestaticfinal String BASE_PATH = ZkClient.getInstance().getBasePath() + "/talent_rec";  
- privatestaticfinal String MAX_REC_NUM_PATH = BASE_PATH + "/max_rec";  
- privatestaticfinal String MIN_TITLE_SIM_PATH = BASE_PATH  
-             + "/min_title_sim";  
- /** 每个职位最多推荐多少个用户 **/
- private ZkParam maxRec = new ZkParam(1000, MAX_REC_NUM_PATH, 21);  
- /** 最小标题相似度阈值 **/
- private ZkParam mimTitleSim = new ZkParam(0.55, MIN_TITLE_SIM_PATH, 22);// TODO
- // 该阈值可能还需要提升
- 
- privatestaticvolatile TalentRecParamConfig instance = null;  
- 
- private TalentRecParamConfig() {  
- super();  
-     }  
- 
- /**
-      * 单例
-      * 
-      * @return
-      */
- publicstatic TalentRecParamConfig getInstance() {  
- if (instance == null) {  
- synchronized (TalentRecParamConfig.class) {  
- if (instance == null) {  
-                     instance = new TalentRecParamConfig();  
-                 }  
-             }  
-         }  
- return instance;  
-     }  
- 
- public ZkParam getMaxRec() {  
- return maxRec;  
-     }  
- 
- publicvoid setMaxRec(ZkParam maxRec) {  
- this.maxRec = maxRec;  
-     }  
- 
- public ZkParam getMimTitleSim() {  
- return mimTitleSim;  
-     }  
- 
- publicvoid setMimTitleSim(ZkParam mimTitleSim) {  
- this.mimTitleSim = mimTitleSim;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
/**
 * 
 * @Author:orisun
 * @Since:2016-4-7
 * @Version:1.0
 */
public class TalentRecParamConfig extends ZkConfig {
    private static final String BASE_PATH = ZkClient.getInstance().getBasePath() + "/talent_rec";
    private static final String MAX_REC_NUM_PATH = BASE_PATH + "/max_rec";
    private static final String MIN_TITLE_SIM_PATH = BASE_PATH
            + "/min_title_sim";
    /** 每个职位最多推荐多少个用户 **/
    private ZkParam maxRec = new ZkParam(1000, MAX_REC_NUM_PATH, 21);
    /** 最小标题相似度阈值 **/
    private ZkParam mimTitleSim = new ZkParam(0.55, MIN_TITLE_SIM_PATH, 22);// TODO
                                                                            // 该阈值可能还需要提升
    private static volatile TalentRecParamConfig instance = null;
    private TalentRecParamConfig() {
        super();
    }
    /**
     * 单例
     * 
     * @return
     */
    public static TalentRecParamConfig getInstance() {
        if (instance == null) {
            synchronized (TalentRecParamConfig.class) {
                if (instance == null) {
                    instance = new TalentRecParamConfig();
                }
            }
        }
        return instance;
    }
    public ZkParam getMaxRec() {
        return maxRec;
    }
    public void setMaxRec(ZkParam maxRec) {
        this.maxRec = maxRec;
    }
    public ZkParam getMimTitleSim() {
        return mimTitleSim;
    }
    public void setMimTitleSim(ZkParam mimTitleSim) {
        this.mimTitleSim = mimTitleSim;
    }
}
```
