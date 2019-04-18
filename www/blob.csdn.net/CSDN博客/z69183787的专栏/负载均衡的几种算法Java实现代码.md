# 负载均衡的几种算法Java实现代码 - z69183787的专栏 - CSDN博客
2019年01月24日 17:45:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：61
个人分类：[Java-负载均衡算法](https://blog.csdn.net/z69183787/article/category/8639150)
[https://www.cnblogs.com/wxd0108/p/5465938.html](https://www.cnblogs.com/wxd0108/p/5465938.html)
轮询
```java
package class2.zookeeper.loadbalance;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
/**
 * 負載均衡算法，輪詢法
 * @author guoy
 *
 */
public class TestRoundRobin {
    
    static Map<String,Integer> serverWeigthMap  = new HashMap<String,Integer>();
     static{
        serverWeigthMap.put("192.168.1.12", 1);
        serverWeigthMap.put("192.168.1.13", 1);
        serverWeigthMap.put("192.168.1.14", 2);
        serverWeigthMap.put("192.168.1.15", 2);
        serverWeigthMap.put("192.168.1.16", 3);
        serverWeigthMap.put("192.168.1.17", 3);
        serverWeigthMap.put("192.168.1.18", 1);
        serverWeigthMap.put("192.168.1.19", 2);
    }
     Integer  pos = 0;
     public  String roundRobin()
        {
            //重新建立一個map,避免出現由於服務器上線和下線導致的並發問題
            Map<String,Integer> serverMap  = new HashMap<String,Integer>();
            serverMap.putAll(serverWeigthMap);
            //獲取ip列表list
            Set<String> keySet = serverMap.keySet();
            ArrayList<String> keyList = new ArrayList<String>();
            keyList.addAll(keySet);
            
            String server = null;
            
            synchronized (pos) {
                if(pos >=keySet.size()){
                    pos = 0;
                }
                server = keyList.get(pos);
                pos ++;
            }
            return server;
        }
        
        public static void main(String[] args) {
            TestRoundRobin robin = new TestRoundRobin();
            for (int i = 0; i < 20; i++) {
                String serverIp = robin.roundRobin();
                System.out.println(serverIp);
            }
        }
}
```
加权轮询
```java
package class2.zookeeper.loadbalance;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;
/**
 * 加權隨機载均衡算法
 * @author guoy
 *
 */
public class TestWeightRandom {
    
    static Map<String,Integer> serverWeigthMap  = new HashMap<String,Integer>();
     static{
        serverWeigthMap.put("192.168.1.12", 1);
        serverWeigthMap.put("192.168.1.13", 1);
        serverWeigthMap.put("192.168.1.14", 2);
        serverWeigthMap.put("192.168.1.15", 2);
        serverWeigthMap.put("192.168.1.16", 3);
        serverWeigthMap.put("192.168.1.17", 3);
        serverWeigthMap.put("192.168.1.18", 1);
        serverWeigthMap.put("192.168.1.19", 2);
    }
    public static String weightRandom()
    {
        //重新建立一個map,避免出現由於服務器上線和下線導致的並發問題
        Map<String,Integer> serverMap  = new HashMap<String,Integer>();
        serverMap.putAll(serverWeigthMap);
        //獲取ip列表list
        Set<String> keySet = serverMap.keySet();
        Iterator<String> it = keySet.iterator();
        List<String> serverList = new ArrayList<String>();
        while (it.hasNext()) {
            String server = it.next();
            Integer weight = serverMap.get(server);
            for (int i = 0; i < weight; i++) {
                serverList.add(server);
            }
        }        
        Random random = new Random();
        int randomPos = random.nextInt(serverList.size());
        
        String server = serverList.get(randomPos);
        return server;
    }
    
    public static void main(String[] args) {
        String serverIp = weightRandom();
        System.out.println(serverIp);
    }
}
```
随机
```java
package class2.zookeeper.loadbalance;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;
import java.util.Set;
/**
 * 隨機负载均衡算法
 * @author guoy
 *
 */
public class TestRandom {
    
    static Map<String,Integer> serverWeigthMap  = new HashMap<String,Integer>();
     static{
        serverWeigthMap.put("192.168.1.12", 1);
        serverWeigthMap.put("192.168.1.13", 1);
        serverWeigthMap.put("192.168.1.14", 2);
        serverWeigthMap.put("192.168.1.15", 2);
        serverWeigthMap.put("192.168.1.16", 3);
        serverWeigthMap.put("192.168.1.17", 3);
        serverWeigthMap.put("192.168.1.18", 1);
        serverWeigthMap.put("192.168.1.19", 2);
    }
    public static String random()
    {
        //重新建立一個map,避免出現由於服務器上線和下線導致的並發問題
        Map<String,Integer> serverMap  = new HashMap<String,Integer>();
        serverMap.putAll(serverWeigthMap);
        //獲取ip列表list
        Set<String> keySet = serverMap.keySet();
        ArrayList<String> keyList = new ArrayList<String>();
        keyList.addAll(keySet);
        
        Random random = new Random();
        int randomPos = random.nextInt(keyList.size());
        
        String server = keyList.get(randomPos);
        return server;
    }
    
    public static void main(String[] args) {
        String serverIp = random();
        System.out.println(serverIp);
    }
}
```
加权随机
```java
package class2.zookeeper.loadbalance;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;
/**
 * 加權隨機载均衡算法
 * @author guoy
 *
 */
public class TestWeightRandom {
    
    static Map<String,Integer> serverWeigthMap  = new HashMap<String,Integer>();
     static{
        serverWeigthMap.put("192.168.1.12", 1);
        serverWeigthMap.put("192.168.1.13", 1);
        serverWeigthMap.put("192.168.1.14", 2);
        serverWeigthMap.put("192.168.1.15", 2);
        serverWeigthMap.put("192.168.1.16", 3);
        serverWeigthMap.put("192.168.1.17", 3);
        serverWeigthMap.put("192.168.1.18", 1);
        serverWeigthMap.put("192.168.1.19", 2);
    }
    public static String weightRandom()
    {
        //重新建立一個map,避免出現由於服務器上線和下線導致的並發問題
        Map<String,Integer> serverMap  = new HashMap<String,Integer>();
        serverMap.putAll(serverWeigthMap);
        //獲取ip列表list
        Set<String> keySet = serverMap.keySet();
        Iterator<String> it = keySet.iterator();
        List<String> serverList = new ArrayList<String>();
        while (it.hasNext()) {
            String server = it.next();
            Integer weight = serverMap.get(server);
            for (int i = 0; i < weight; i++) {
                serverList.add(server);
            }
        }        
        Random random = new Random();
        int randomPos = random.nextInt(serverList.size());
        
        String server = serverList.get(randomPos);
        return server;
    }
    
    public static void main(String[] args) {
        String serverIp = weightRandom();
        System.out.println(serverIp);
    }
}
```
ip hash
```java
package class2.zookeeper.loadbalance;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
/**
 * 负载均衡 ip_hash算法
 * @author guoy
 *
 */
public class TestIpHash {
    
    static Map<String,Integer> serverWeigthMap  = new HashMap<String,Integer>();
     static{
        serverWeigthMap.put("192.168.1.12", 1);
        serverWeigthMap.put("192.168.1.13", 1);
        serverWeigthMap.put("192.168.1.14", 2);
        serverWeigthMap.put("192.168.1.15", 2);
        serverWeigthMap.put("192.168.1.16", 3);
        serverWeigthMap.put("192.168.1.17", 3);
        serverWeigthMap.put("192.168.1.18", 1);
        serverWeigthMap.put("192.168.1.19", 2);
    }
     /**
      * 获取请求服务器地址
      * @param remoteIp 负载均衡服务器ip
      * @return
      */
    public static String ipHash(String remoteIp)
    {
        //重新建立一個map,避免出現由於服務器上線和下線導致的並發問題
        Map<String,Integer> serverMap  = new HashMap<String,Integer>();
        serverMap.putAll(serverWeigthMap);
        //獲取ip列表list
        Set<String> keySet = serverMap.keySet();
        ArrayList<String> keyList = new ArrayList<String>();
        keyList.addAll(keySet);
        
        int hashCode =remoteIp.hashCode();
        int serverListSize = keyList.size();
        int serverPos = hashCode % serverListSize;
        
        return keyList.get(serverPos);
    }
    
    public static void main(String[] args) {
        String serverIp = ipHash("192.168.1.12");
        System.out.println(serverIp);
    }
}
```
