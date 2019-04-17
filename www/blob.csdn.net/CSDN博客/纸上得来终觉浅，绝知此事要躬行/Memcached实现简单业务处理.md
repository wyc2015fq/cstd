# Memcached实现简单业务处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年05月09日 22:19:03[boonya](https://me.csdn.net/boonya)阅读数：1162








--------memcache 基本业务代码解读

**1、创建Memcache客户端连接MemCacheConnection**

```java
package com.boonya.mecache.store;
import com.danga.MemCached.MemCachedClient;
import com.danga.MemCached.SockIOPool;

public class MemCacheConnection {
 
 private static final MemCachedClient memCachedClient = new MemCachedClient();
 
 static {
  String[] servers = {
  // "192.168.20.10:11211"
  "127.0.0.1:11211" };
  Integer[] weights = { 3, 3 };
  /* 实例化连接 */
  SockIOPool pool = SockIOPool.getInstance();
  pool.setServers(servers);
  pool.setWeights(weights);
  /* 设置5个连接池 最大连接数250个 空闲时间6小时 */
  pool.setInitConn(5);
  pool.setMinConn(5);
  pool.setMaxConn(250);
  pool.setMaxIdle(1000 * 60 * 60 * 24);
  pool.setMaintSleep(30);
  pool.setNagle(false);
  pool.setSocketTO(3000);
  pool.setSocketConnectTO(0);
  pool.initialize();

  memCachedClient.setPrimitiveAsString(true);
  memCachedClient.setSanitizeKeys(false);
 }

 public static MemCachedClient getMemcacheClient() {
  
  return memCachedClient;
  
 }
}
```


**2、MemCache缓存数据操作MemStoragehandler**

```java
package com.boonya.mecache.store;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import com.danga.MemCached.MemCachedClient;

public class MemStoragehandler {

 MemCachedClient memClient = MemCacheConnection.getMemcacheClient();
 Connection conn = null;
 Statement stmt = null;

 /**
  * 执行业务处理操作
  * 
  * @param myItems
  *            缓存集合对象
  */
 @SuppressWarnings("rawtypes")
 public void doService(ArrayList myItems) {
  boolean isSaved = this.saveItems(myItems);
  if (isSaved) {
   this.deleteMemcacheItem(myItems, memClient, isSaved);
  }
 }

 /**
  * 删除MemCache缓存中数据
  * 
  * @param myItems
  * @param memClient
  * @param isSaved
  */
 @SuppressWarnings("rawtypes")
 private void deleteMemcacheItem(ArrayList myItems,
   MemCachedClient memClient, boolean isSaved) {
  if (isSaved) {
   for (int d = 0; d < myItems.size(); d++) {
    memClient.delete((String) myItems.get(d));
   }
  }
 }

 /**
  * MemCache缓存中数据批量入库
  * 
  * @param myItems
  *                    数据集合
  * @return true/false 存储标志
  */
 @SuppressWarnings("rawtypes")
 private boolean saveItems(ArrayList myItems) {
  try {
   DBConnection dbconn = new DBConnection();
   conn = dbconn.getConnectionByJDBC("127.0.0.1", "3306", "lbs",
     "root", "root");
   stmt = conn.createStatement();
   conn.setAutoCommit(false);
   /* 从memcached里遍历一个一个取值 */
   for (int h = 0; h < myItems.size(); h++) {
    String str = (String) memClient.get((String) myItems.get(h));
    String key = (String) myItems.get(h);
    String keyarr[] = key.split("_");
    String sql = DBCreateSQL.getkey(str, keyarr[0]);//创建SQL语句
    stmt.executeUpdate(sql);
   }
   conn.commit();
  } catch (Exception e) {
   System.out.println(e.getMessage());
   e.printStackTrace();
   /* 如果事务提交失败flag置为1并回滚 */
   try {
    conn.rollback();
   } catch (SQLException e1) {
    e1.printStackTrace();
   }
   ServiceLog.logerr(myItems.size());
   return false;
  } finally {
   this.free(conn, stmt);
  }
  return true;
 }

 /**
  * 释放数据库连接资源占用
  * @param conn
  * @param stmt
  */
 private void free(Connection conn, Statement stmt) {
  try {
   if (stmt != null) {
    stmt.close();
   }
   stmt = null;
  } catch (Exception e) {
   e.printStackTrace();
  }
  try {
   if (conn != null) {
    conn.setAutoCommit(true);
    conn.close();
   }
   conn = null;
  } catch (Exception e) {
   e.printStackTrace();
  }
 }
}
```


**3、MemCache缓存数据读取MemDataSearching**

```java
package com.boonya.mecache.store;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import com.danga.MemCached.MemCachedClient;

public class MemDataSearching {

 /**
  * 遍历memCached方法，返回key arrayList 在memCached中实际操作为 stats items -> stats
  * (此种方式不推荐，遍历太耗时间，降低了性能 最好事先定义好对象存储在memCache的时候是键值对形式
  * 如：key-name,value=map/hashMap 然后再一层一层的去map中取值，速度会很快 )
  */
 @SuppressWarnings({ "unchecked", "rawtypes" })
 public static ArrayList searchdata() {
  MemCachedClient memClient = MemCacheConnection.getMemcacheClient();
  ArrayList keys = new ArrayList(30000);
  Map slabs = memClient.statsItems();
  Iterator itemsItr = slabs.keySet().iterator();
  while (itemsItr.hasNext()) {
   String serverInfo = itemsItr.next().toString();
   Map itemNames = (Map) slabs.get(serverInfo);
   Iterator itemNameItr = itemNames.keySet().iterator();
   while (itemNameItr.hasNext()) {
    String itemName = itemNameItr.next().toString();
    String[] itemAtt = itemName.split(":");
    if (itemAtt[2].startsWith("number")) {
     Map cachedDump = memClient.statsCacheDump(
       Integer.parseInt(itemAtt[1]), 0);
     Iterator itr = cachedDump.keySet().iterator();
     while (itr.hasNext()) {
      String serverInfo2 = itr.next().toString();
      Map items = (Map) cachedDump.get(serverInfo2);
      Iterator keyItr = items.keySet().iterator();
      while (keyItr.hasNext()) {
       String key = keyItr.next().toString();
       String memKey = key;
       keys.add(memKey);
      }
     }
    }
   }
  }
  return keys;
 }

 /**
  * 改进方式读取,返回对应表对象集合 （ 事先定义好对象存储在memCache的时候是键值对形式
  * 如：key-name,value=map/hashMap 然后再一层一层的去map中取值，速度会很快
  * 结构：分区（多个），分表（多个），表数据行（多行）
  * 
  * 也可以使用分区分表的方式 ）
  * @param tableName  查询数据表的名称
  * 
  * @return
  */
 @SuppressWarnings({ "rawtypes"})
 public static List searchdataPlus(String tableName) {
  MemCachedClient memClient = MemCacheConnection.getMemcacheClient();
  Map tableAreas= memClient.statsItems();            // 获取MAP表集合
  Iterator iteratorTableArea = tableAreas.keySet().iterator();
  while(iteratorTableArea.hasNext()) {              //之所以用while而不用if是因为可能会有多个表分区
   Map area = (Map) iteratorTableArea.next();    // 获取MAP表
   if (area!= null) {
    if (area.containsKey(tableName)) {        // 分区下是否存在需要访问的表
     Map table=(Map) area.get(tableName);
     if(table!=null){
      return (List) table.values();     //表中的对象集合
     }
    }
   }
  }
  return null;
 }

}
```




