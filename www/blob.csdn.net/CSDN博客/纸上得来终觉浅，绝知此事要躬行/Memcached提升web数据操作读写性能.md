# Memcached提升web数据操作读写性能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年03月12日 00:47:44[boonya](https://me.csdn.net/boonya)阅读数：2165








**1、下载安装**

1. 从[http://jehiah.cz/projects/memcached-win32/](http://jehiah.cz/projects/memcached-win32/) 上下载memcache的windows稳定版，解压放某个盘下面，比如在F:\memcached

2. 在终端（也即cmd命令界面）下输入 'F:\memcached\memcached.exe -d install' 安装 

3. 再输入： 'F:\memcached\memcached.exe -d start' 启动 

4.开发jar包下载： 

memcached client for java:[https://github.com/gwhalin/Memcached-Java-Client/downloads](https://github.com/gwhalin/Memcached-Java-Client/downloads)

spymemcached：[http://code.google.com/p/spymemcached/](http://code.google.com/p/spymemcached/)

xmemcached：[http://code.google.com/p/xmemcached/](http://code.google.com/p/xmemcached/) --- 注意: 以后memcached将作为windows的一个服务每次开机时自动启动。默认端口：11211。
**2、Memcahe的三种API**

（1）、memcached client for java,memcache 最早推出的Java客户端API,并且不断更新，性能上得到提升并且稳定。 

（2）、spymemcached 

A simple,asynchronous,single-thread memcached client written in java.支持异步单线程的memcached客户端。

使用到JDK1.5听过的nio和concurrent，存取速度高于前者，但是稳定性不好，测试中常报TimeOut异常。 

（3）、xmemcached 

xmemcached同样是基于java nio的客户端，java nio 相对于传统io来说有效率高（特别是在处理并发方面）和资源耗费相对较少的优点。 

传统io需要建立多个连接组成连接池，而nio只需要一个连接即可（nio也可以池化），减少了线程的创建和切换的开销，这一点在高并发操 

作下尤为明显。因此，xmemcached和spymemcached在性能方面都非常优秀，在某些方面（存储的数据量比较小的情况下），xmemcached要 

优于spymemcached。

**3、程序参考示例--见java测试文件**


```java
package com.boonya.mecache;
import com.danga.MemCached.MemCachedClient;
import com.danga.MemCached.SockIOPool;
public class TestMemCached {
   
    /**
     * @param args
     */
    public static void main(String[] args) {
       
        /**
         * 初始化SockIoPool,管理MemCached线程池
         */
        String servers[]={"192.168.20.37:11211"};
        SockIOPool pool=SockIOPool.getInstance();
        pool.setServers(servers);
        pool.setFailover(true);
        pool.setInitConn(10);
        pool.setMinConn(5);
        pool.setMaxConn(300);
        pool.setMaintSleep(30);
        pool.setNagle(false);
        pool.setSocketTO(3000);
        pool.setAliveCheck(true);
        pool.initialize();
       
        /**
         * 创建MemCachedClient实例
         */
        MemCachedClient memcacheClient=new MemCachedClient();
       
        for (int i = 0; i < 100; i++) {
            boolean success=memcacheClient.set("key"+i,"a_memcached");//将对象加入到MemCache缓存
            /**
             * 读取MemCache缓存
             */
            String result=(String) memcacheClient.get("key"+i);
            System.out.println(String.format("set{%d}:%s",i,success));
            System.out.println(String.format("get{%d}:%s",i,result));
        }
       
    }

}
package com.boonya.mecache;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;
import net.spy.memcached.MemcachedClient;
public class TestSpyMemCached {
   
    public static void main(String[] args) throws InterruptedException, ExecutionException {
       
        /**
         * 建立MemCached的示例
         */
        try {
            MemcachedClient memcacheClient=new MemcachedClient(
                    new InetSocketAddress("192.168.20.37", 11211));
           
            Future<Boolean> fbl=null;
            /*将key,过期时间,对应的值设入到MemCache*/
            fbl=memcacheClient.set("user:data:key", 1000, "me");
            if(fbl.get().booleanValue()==true){
                memcacheClient.shutdown();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
       
        /**
         * 获取MemCached的示例
         */
        try {
            MemcachedClient memcacheClient=new MemcachedClient(
                    new InetSocketAddress("192.168.20.37", 11211));
           
            Object obj=memcacheClient.get("user:data:key");
            System.out.println(obj);
            memcacheClient.shutdown();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}

package com.boonya.mecache;
import java.io.IOException;
import java.util.concurrent.TimeoutException;
import net.rubyeye.xmemcached.MemcachedClient;
import net.rubyeye.xmemcached.MemcachedClientBuilder;
import net.rubyeye.xmemcached.XMemcachedClientBuilder;
import net.rubyeye.xmemcached.exception.MemcachedException;
import net.rubyeye.xmemcached.utils.AddrUtil;
public class TestXMemCached {
   
    public static void main(String[] args)  {
        MemcachedClientBuilder builder=new XMemcachedClientBuilder(
                AddrUtil.getAddresses("192.168.20.37:11211"));
        try {
            MemcachedClient memCacheClient=builder.build();
            try {
                memCacheClient.set("key", 0, "data");
               
                String value=memCacheClient.get("key");
                System.out.println("key:"+value);
               
                memCacheClient.delete("key");
                value=memCacheClient.get("key");
                System.out.println("key:"+value);
               
                memCacheClient.shutdown();
            } catch (TimeoutException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (MemcachedException e) {
                e.printStackTrace();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
```



**4、使用建议**

（1）、 memcached client for java不断更新，性能上得到提升，并且稳定。 

（2）、 xmemcached使用广泛,其有如下特点:高性能、支持完整的协议、支持客户端分布、允许设置节点权重、动态增删节点、支持JMX、Spring框架和 

Hibernate-memcached的集成、客户端连接池、可扩展性好等。 



