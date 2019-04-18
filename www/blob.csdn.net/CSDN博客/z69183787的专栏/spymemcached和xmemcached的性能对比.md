# spymemcached和xmemcached的性能对比 - z69183787的专栏 - CSDN博客
2016年10月25日 18:22:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2814

```java
Memcached 客户端程序  
   
Memcached的java客户端已经存在三种了：  
   
?  官方提供的基于传统阻塞io由Greg Whalin维护的客户端  
   
?  Dustin Sallings实现的基于java nio的Spymemcached  
   
?  XMemcached  
   
  
1. 三种API比较  
 1)      memcached client for java  
   
较早推出的memcached JAVA客户端API，应用广泛，运行比较稳定。   
  
  
2)      spymemcached  
   
A simple, asynchronous, single-threaded memcached client written in java. 支持异步，单线程的memcached客户端，用到了java1.5版本的concurrent和nio，存取速度会高于前者，但是稳定性不好，测试中常 报timeOut等相关异常。   
  
  
3)      xmemcached  
   
XMemcached同样是基于java nio的客户端，java nio相比于传统阻塞io模型来说，有效率高（特别在高并发下）和资源耗费相对较少的优点。传统阻塞IO为了提高效率，需要创建一定数量的连接形成连接 池，而nio仅需要一个连接即可（当然,nio也是可以做池化处理），相对来说减少了线程创建和切换的开销，这一点在高并发下特别明显。因此 XMemcached与Spymemcached在性能都非常优秀，在某些方面（存储的数据比较小的情况下）Xmemcached比 Spymemcached的表现更为优秀，具体可以看这个Java Memcached Clients Benchmark。  
   
  
2.  建议  
   
由于memcached client for java发布了新版本，性能上有所提高，并且运行稳定，所以建议使用memcached client for java。  
   
XMemcached也使用得比较广泛，而且有较详细的中文API文档，具有如下特点：高性 能、支持完整的协议、支持客户端分布、允许设置节点权重、动态增删节点、支持JMX、与Spring框架和Hibernate-memcached的集 成、客户端连接池、可扩展性好等。  
   
下面给出这三种客户端的示例程序。  
   
  
3.  示例程序   
1)      memcached client for java  
   
从前面介绍的Java环境的Memcached客户端程序项目网址里，下载最新版的客户端程 序包：java_memcached-release_2.5.1.zip，解压后，文件夹里找到java_memcached- release_2.5.1.jar，这个就是客户端的JAR包。将此JAR包添加到项目的构建路径里，则项目中，就可以使用Memcached了。  
   
示例代码如下：  
   
package temp;  
   
   
   
import com.danga.MemCached.*;  
   
import org.apache.log4j.*;  
   
   
   
public class CacheTest {  
   
    public static void main(String[] args) {  
   
       /**  
   
        * 初始化SockIOPool，管理memcached的连接池  
   
        * */  
   
       String[] servers = { "10.11.15.222:10000" };  
   
       SockIOPool pool = SockIOPool.getInstance();  
   
       pool.setServers(servers);  
   
       pool.setFailover(true);  
   
       pool.setInitConn(10);  
   
       pool.setMinConn(5);  
   
       pool.setMaxConn(250);  
   
       pool.setMaintSleep(30);  
   
       pool.setNagle(false);  
   
       pool.setSocketTO(3000);  
   
       pool.setAliveCheck(true);  
   
       pool.initialize();  
   
   
   
       /**  
   
        * 建立MemcachedClient实例  
   
        * */  
   
       MemCachedClient memCachedClient = new MemCachedClient();  
   
       for (int i = 0; i < 1000; i++) {  
   
           /**  
   
            * 将对象加入到memcached缓存  
   
            * */  
   
           boolean success = memCachedClient.set("" + i, "Hello!");  
   
           /**  
   
            * 从memcached缓存中按key值取对象  
   
            * */  
   
           String result = (String) memCachedClient.get("" + i);  
   
           System.out.println(String.format("set( %d ): %s", i, success));  
   
           System.out.println(String.format("get( %d ): %s", i, result));  
   
       }  
   
    }  
   
}  
   
  
2)      spymemcached  
   
spymemcached当前版本是2.5版本，官方网址是：http://code.google.com/p/spymemcached/。可以从地址：http://spymemcached.googlecode.com/files/memcached-2.5.jar 下载最新版本来使用。  
   
示例代码如下：  
   
package temp;  
   
   
   
import java.net.InetSocketAddress;  
   
import java.util.concurrent.Future;  
   
   
   
import net.spy.memcached.MemcachedClient;  
   
   
   
public class TestSpyMemcache {  
   
    public static void main(String[] args) {  
   
       // 保存对象  
   
       try {  
   
           /* 建立MemcachedClient 实例，并指定memcached服务的IP地址和端口号 */  
   
           MemcachedClient mc = new MemcachedClient(new InetSocketAddress("10.11.15.222", 10000));  
   
           Future<Boolean> b = null;  
   
           /* 将key值，过期时间(秒)和要缓存的对象set到memcached中 */  
   
           b = mc.set("neea:testDaF:ksIdno", 900, "someObject");  
   
           if (b.get().booleanValue() == true) {  
   
              mc.shutdown();  
   
           }  
   
       } catch (Exception ex) {  
   
           ex.printStackTrace();  
   
       }  
   
       // 取得对象  
   
        try {  
   
           /* 建立MemcachedClient 实例，并指定memcached服务的IP地址和端口号 */  
   
           MemcachedClient mc = new MemcachedClient(new InetSocketAddress("10.11.15.222", 10000));  
   
           /* 按照key值从memcached中查找缓存，不存在则返回null */  
   
           Object b = mc.get("neea:testDaF:ksIdno");  
   
           System.out.println(b.toString());  
   
           mc.shutdown();  
   
       } catch (Exception ex) {  
   
           ex.printStackTrace();  
   
       }  
   
    }  
   
}  
   
  
3)      xmemcached  
   
Xmemcached的官方网址是：http://code.google.com/p/xmemcached/，可以从其官网上下载最新版本的1.2.4 来使用。地址是：http://xmemcached.googlecode.com/files/xmemcached-1.2.4-src.tar.gz 。  
   
示例代码如下：  
   
package temp;  
   
   
   
import java.io.IOException;  
   
import java.util.concurrent.TimeoutException;  
   
   
   
import net.rubyeye.xmemcached.utils.AddrUtil;  
   
import net.rubyeye.xmemcached.MemcachedClient;  
   
import net.rubyeye.xmemcached.MemcachedClientBuilder;  
   
import net.rubyeye.xmemcached.XMemcachedClientBuilder;  
   
import net.rubyeye.xmemcached.exception.MemcachedException;  
   
   
   
public class TestXMemcache {  
   
    public static void main(String[] args) {  
   
       MemcachedClientBuilder builder = new XMemcachedClientBuilder(AddrUtil  
   
              .getAddresses("10.11.15.222:10000"));  
   
       MemcachedClient memcachedClient;  
   
       try {  
   
           memcachedClient = builder.build();  
   
   
   
           memcachedClient.set("hello", 0, "Hello,xmemcached");  
   
           String value = memcachedClient.get("hello");  
   
           System.out.println("hello=" + value);  
   
           memcachedClient.delete("hello");  
   
           value = memcachedClient.get("hello");  
   
           System.out.println("hello=" + value);  
   
           // close memcached client  
   
           memcachedClient.shutdown();  
   
       } catch (MemcachedException e) {  
   
           System.err.println("MemcachedClient operation fail");  
   
           e.printStackTrace();  
   
       } catch (TimeoutException e) {  
   
           System.err.println("MemcachedClient operation timeout");  
   
           e.printStackTrace();  
   
       } catch (InterruptedException e) {  
   
           // ignore  
   
       }catch (IOException e) {  
   
           System.err.println("Shutdown MemcachedClient fail");  
   
           e.printStackTrace();  
   
       }  
   
    }  
   
}
```
读写简单类型
测试方法：开N个线程 ，每个线程set(或者get、delete) 10000次，表格如下（数据为tps,仅供参考）
|线程数||spymemcached|||xmemcached||
|----|----|----|----|----|----|----|
||set|get|delete|set|get|delete|
|1|2870|2922|3018|2237|2352|2500|
|10|11015|11227|11449|8579|10440|8354|
|50|19838|20685|22727|13239|24113|14382|
|100|25427|22646|26700|18068|29046|18259|
结论：显然在简单类型的读写上,spymemcached全面占优，xmemcached唯一的亮点在于高并发下get的效率超过了spymemcached。对于连续的get操作，xmemcached将合并成一个批量的get操作提交，从而提高效率。
读写100个元素的map，map的value是个自定义类，启动N个线程，每个线程set(或者get、delete) 100次，表格如下
|线程数||spymemcached||xmemcached|
|----|----|----|----|----|
||set|get|set|get|
|1|492|492|427|492|
|10|159|680|1103|1122|
|50|57|1103|1561|1226|
|100|71|1308|1530|1223|
结论：在复杂对象的读写上，xmemcached全面占优。两者的CPU和内存占用差不多，肉眼观察做不得准。比较奇怪的是spymemcached的set竟然那么慢。
