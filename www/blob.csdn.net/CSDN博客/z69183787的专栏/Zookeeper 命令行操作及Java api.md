# Zookeeper 命令行操作及Java api - z69183787的专栏 - CSDN博客
2016年11月10日 11:01:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1016
1.  连接到zookeeper服务
> 
[java2000_wl@localhost zookeeper-3]$ bin/zkCli.sh -server 127.0.0.1:2181
2.  使用ls命令查看当前zookeeper中包含的内容
> 
[zk: 127.0.0.1:2181(CONNECTED) 1] ls /
[zookeeper]
3.  创建新的znode，使用create命令
> 
[zk:127.0.0.1:2181(CONNECTED) 2] create /zk testData
Created /zk
4.  获取节点中的值  get命令
> 
[zk:127.0.0.1:2181(CONNECTED) 4] get /zk
testData
cZxid = 0x700000008
ctime = Sat Mar 0923:01:24 CST 2013
mZxid = 0x700000008
mtime = Sat Mar 0923:01:24 CST 2013
pZxid = 0x700000008
cversion = 0
dataVersion = 0
aclVersion = 0
ephemeralOwner = 0x0
dataLength = 8
numChildren = 0
5.  使用set命令来对znode关联的字符串进行设置
> 
[zk: 127.0.0.1:2181(CONNECTED) 5] set /zk mydata
cZxid = 0x700000008
ctime = Sat Mar 09 23:01:24 CST 2013
mZxid = 0x700000009
mtime = Sat Mar 09 23:06:03 CST 2013
pZxid = 0x700000008
cversion = 0
dataVersion = 1
aclVersion = 0
ephemeralOwner = 0x0
dataLength = 6
numChildren = 0
6.  删除znode节点
> 
[zk: 127.0.0.1:2181(CONNECTED) 7]delete /zk
**org.apache.zookeeper.ZooKeeper类 主要方法列表**
|方法名称|描述|
|----|----|
|String create(final String path, byte data[], List acl, CreateMode createMode)|创建一个znode节点，参数： 路径、 znode内容，ACL(访问控制列表)、 znode创建类型|
|void delete(final String path, int version)|删除一个znode节点，参数： 路径、版本号；如果版本号与znode的版本号不一致，将无法删除，是一种乐观加锁机制；如果将版本号设置为-1，不会去检测版本，直接删除；|
|Stat exists(final String path, Watcher watcher)|判断某个znode节点是否存在参数： 路径、Watcher(监视器)；当这个znode节点被改变时，将会触发当前Watcher|
|Stat exists(String path, boolean watch)|判断某个znode节点是否存在参数： 路径、并设置是否监控这个目录节点，这里的 watcher 是在创建 ZooKeeper 实例时指定的 watcher|
|Stat setData(final String path, byte data[], int version)|设置某个znode上的数据参数： 路径、数据、版本号；如果为-1，跳过版本检查|
|byte[] getData(final String path, Watcher watcher, Stat stat)|获取某个znode上的数据参数： 路径、监视器、数据版本等信息|
|List getChildren(final String path, Watcher watcher)|获取某个节点下的所有子节点参数： 路径、监视器;该方法有多个重载|
**znode创建类型(CreateMode)：**
> PERSISTENT               持久化节点
PERSISTENT_SEQUENTIAL     顺序自动编号持久化节点，这种节点会根据当前已存在的节点数自动加 1
EPHEMERAL    临时节点， 客户端session超时这类节点就会被自动删除
EPHEMERAL_SEQUENTIAL   临时自动编号节点
**[java]**[view
 plain](http://blog.csdn.net/java2000_wl/article/details/8655146#)[copy](http://blog.csdn.net/java2000_wl/article/details/8655146#)
[print](http://blog.csdn.net/java2000_wl/article/details/8655146#)[?](http://blog.csdn.net/java2000_wl/article/details/8655146#)
- /*
-  * ZookeeperTest.java
-  */
- package com.x.zookeeper;  
- 
- import java.io.IOException;  
- import java.util.List;  
- 
- import org.apache.zookeeper.CreateMode;  
- import org.apache.zookeeper.WatchedEvent;  
- import org.apache.zookeeper.Watcher;  
- import org.apache.zookeeper.ZooKeeper;  
- import org.apache.zookeeper.ZooDefs.Ids;  
- import org.apache.zookeeper.data.Stat;  
- import org.junit.After;  
- import org.junit.Assert;  
- import org.junit.Before;  
- import org.junit.Test;  
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- 
- /**
-  * 
-  * @author http://blog.csdn.net/java2000_wl
-  * @version <b>1.0</b>
-  */
- publicclass ZookeeperTest {  
- 
- privatestaticfinalint SESSION_TIMEOUT = 30000;  
- 
- publicstaticfinal Logger LOGGER = LoggerFactory.getLogger(ZookeeperTest.class);  
- 
- private Watcher watcher =  new Watcher() {  
- 
- publicvoid process(WatchedEvent event) {  
-             LOGGER.info("process : " + event.getType());  
-         }  
-     };  
- 
- private ZooKeeper zooKeeper;  
- 
- /**
-      *  连接zookeeper
-      * <br>------------------------------<br>
-      * @throws IOException
-      */
- @Before
- publicvoid connect() throws IOException {  
-         zooKeeper  = new ZooKeeper("localhost:2181,localhost:2182,localhost:2183", SESSION_TIMEOUT, watcher);  
-     }  
- 
- /**
-      *  关闭连接
-      * <br>------------------------------<br>
-      */
- @After
- publicvoid close() {  
- try {  
-             zooKeeper.close();  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /**
-      * 创建一个znode 
-      *  1.CreateMode 取值  
-      *  PERSISTENT：持久化，这个目录节点存储的数据不会丢失
-      *  PERSISTENT_SEQUENTIAL：顺序自动编号的目录节点，这种目录节点会根据当前已近存在的节点数自动加 1，然后返回给客户端已经成功创建的目录节点名；
-      *  EPHEMERAL：临时目录节点，一旦创建这个节点的客户端与服务器端口也就是 session过期超时，这种节点会被自动删除
-      *  EPHEMERAL_SEQUENTIAL：临时自动编号节点
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testCreate() {  
-         String result = null;  
- try {  
-              result = zooKeeper.create("/zk001", "zk001data".getBytes(), Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          LOGGER.info("create result : {}", result);  
-      }  
- 
- /**
-      * 删除节点  忽略版本
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testDelete() {  
- try {  
-             zooKeeper.delete("/zk001", -1);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-     }  
- 
- /**
-      *   获取数据
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testGetData() {  
-         String result = null;  
- try {  
- byte[] bytes = zooKeeper.getData("/zk001", null, null);  
-              result = new String(bytes);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          LOGGER.info("getdata result : {}", result);  
-     }  
- 
- /**
-      *   获取数据  设置watch
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testGetDataWatch() {  
-         String result = null;  
- try {  
- byte[] bytes = zooKeeper.getData("/zk001", new Watcher() {  
- publicvoid process(WatchedEvent event) {  
-                     LOGGER.info("testGetDataWatch  watch : {}", event.getType());  
-                 }  
-              }, null);  
-              result = new String(bytes);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          LOGGER.info("getdata result : {}", result);  
- 
- // 触发wacth  NodeDataChanged
- try {  
-              zooKeeper.setData("/zk001", "testSetData".getBytes(), -1);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-     }  
- 
- /**
-      *    判断节点是否存在
-      *    设置是否监控这个目录节点，这里的 watcher 是在创建 ZooKeeper实例时指定的 watcher
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testExists() {  
-         Stat stat = null;  
- try {  
-              stat = zooKeeper.exists("/zk001", false);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          Assert.assertNotNull(stat);  
-          LOGGER.info("exists result : {}", stat.getCzxid());  
-     }  
- 
- /**
-      *     设置对应znode下的数据  ,  -1表示匹配所有版本
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testSetData() {  
-         Stat stat = null;  
- try {  
-              stat = zooKeeper.setData("/zk001", "testSetData".getBytes(), -1);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          Assert.assertNotNull(stat);  
-          LOGGER.info("exists result : {}", stat.getVersion());    
-     }  
- 
- /**
-      *    判断节点是否存在, 
-      *    设置是否监控这个目录节点，这里的 watcher 是在创建 ZooKeeper实例时指定的 watcher
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testExistsWatch1() {  
-         Stat stat = null;  
- try {  
-              stat = zooKeeper.exists("/zk001", true);  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          Assert.assertNotNull(stat);  
- 
- try {  
-             zooKeeper.delete("/zk001", -1);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /**
-      *    判断节点是否存在, 
-      *    设置监控这个目录节点的 Watcher
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testExistsWatch2() {  
-         Stat stat = null;  
- try {  
-              stat = zooKeeper.exists("/zk002", new Watcher() {  
- @Override
- publicvoid process(WatchedEvent event) {  
-                     LOGGER.info("testExistsWatch2  watch : {}", event.getType());  
-                 }  
-              });  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-          Assert.assertNotNull(stat);  
- 
- // 触发watch 中的process方法   NodeDataChanged
- try {  
-             zooKeeper.setData("/zk002", "testExistsWatch2".getBytes(), -1);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
- 
- // 不会触发watch 只会触发一次
- try {  
-             zooKeeper.delete("/zk002", -1);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- /**
-      *  获取指定节点下的子节点
-      * <br>------------------------------<br>
-      */
- @Test
- publicvoid testGetChild() {  
- try {  
-              zooKeeper.create("/zk/001", "001".getBytes(), Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);  
-              zooKeeper.create("/zk/002", "002".getBytes(), Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);  
- 
-              List<String> list = zooKeeper.getChildren("/zk", true);  
- for (String node : list) {  
-                 LOGGER.info("node {}", node);  
-             }  
-         } catch (Exception e) {  
-              LOGGER.error(e.getMessage());  
-              Assert.fail();  
-         }  
-     }  
- }  
- 
