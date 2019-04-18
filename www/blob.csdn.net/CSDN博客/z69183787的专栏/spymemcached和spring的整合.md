# spymemcached和spring的整合 - z69183787的专栏 - CSDN博客
2016年10月25日 18:29:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1637
[1.配置web.xml、applicationContext.xml和servletmvc.xml](http://blog.csdn.net/jifengwan/article/details/50643140#config)
[2.Java后台代码](http://blog.csdn.net/jifengwan/article/details/50643140#java)
[3.测试](http://blog.csdn.net/jifengwan/article/details/50643140#test)
目录结构:
![这里写图片描述](http://www.hyqian.com/wp-content/uploads/2016/02/20160206200645_96910.png)
1.配置web.xml、applicationContext.xml和servletmvc.xml
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0" xmlns="http://java.sun.com/xml/ns/javaee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
    http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>/WEB-INF/classes/applicationContext.xml</param-value>
    </context-param>
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    <servlet>
        <servlet-name>servletmvc</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>/WEB-INF/classes/servletmvc.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>servletmvc</servlet-name>
        <url-pattern>*.do</url-pattern>
    </servlet-mapping>
</web-app>
```
applicationContext.xml
```
<?xml version="1.0" encoding="utf-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx" xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
      http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
      http://www.springframework.org/schema/aop  
      http://www.springframework.org/schema/aop/spring-aop-4.0.xsd
      http://www.springframework.org/schema/tx 
      http://www.springframework.org/schema/tx/spring-tx-4.0.xsd
      http://www.springframework.org/schema/context 
            http://www.springframework.org/schema/context/spring-context-4.0.xsd">
    <!-- 
        枚举类型要想注入到类中，一定要先使用org.springframework.beans.factory.config.FieldRetrievingFactoryBean类将枚举类型进行转换
        将DefaultHashAlgorithm.KETAMA_HASH转换为KETAMA_HASH这个bean，
        然后在要注入的bean中使用<property name="hashAlg" ref="KETAMA_HASH" />引用即可。
     -->
    <bean id="KETAMA_HASH" class="org.springframework.beans.factory.config.FieldRetrievingFactoryBean">
        <property name="staticField" value="net.spy.memcached.DefaultHashAlgorithm.KETAMA_HASH" />
    </bean>
    <bean id="memcachedClient" class="net.spy.memcached.spring.MemcachedClientFactoryBean">
        <!-- 一个字符串，包括由空格或逗号分隔的主机或IP地址与端口号 -->
        <property name="servers" value="192.168.1.100:11211" />
        <!-- 指定要使用的协议(BINARY,TEXT),默认是TEXT -->
        <property name="protocol" value="BINARY" />
        <!-- 设置默认的转码器(默认以net.spy.memcached.transcoders.SerializingTranscoder) -->
        <property name="transcoder">
            <bean class="net.spy.memcached.transcoders.SerializingTranscoder">
                <property name="compressionThreshold" value="1024" />
            </bean>
        </property>
        <!-- 以毫秒为单位设置默认的操作超时时间 -->
        <property name="opTimeout" value="1000" />
        <property name="timeoutExceptionThreshold" value="1998" />
        <!-- 设置哈希算法 -->
        <property name="hashAlg" ref="KETAMA_HASH" />
        <!-- 设置定位器类型(ARRAY_MOD,CONSISTENT),默认是ARRAY_MOD -->
        <property name="locatorType" value="CONSISTENT" />
        <!-- 设置故障模式(取消，重新分配，重试)，默认是重新分配 -->
        <property name="failureMode" value="Redistribute" />
        <!-- 想使用Nagle算法，设置为true -->
        <property name="useNagleAlgorithm" value="false" />
    </bean>
    <bean id="memcachedManager" class="com.memcached.spymemcached.SpyMemcachedManager">
        <property name="memcachedClient" ref="memcachedClient" />
    </bean>
</beans>
```
属性说明：
Servers
一个字符串，包括由空格或逗号分隔的主机或IP地址与端口号
Daemon
设置IO线程的守护进程(默认为true)状态
FailureMode
设置故障模式(取消，重新分配，重试)，默认是重新分配
HashAlg
设置哈希算法(见net.spy.memcached.HashAlgorithm的值)
InitialObservers
设置初始连接的观察者(观察初始连接)
LocatorType
设置定位器类型(ARRAY_MOD,CONSISTENT),默认是ARRAY_MOD
MaxReconnectDelay
设置最大的连接延迟
OpFact
设置操作工厂
OpQueueFactory
设置操作队列工厂
OpTimeout
以毫秒为单位设置默认的操作超时时间
Protocol
指定要使用的协议(BINARY,TEXT),默认是TEXT
ReadBufferSize
设置读取的缓冲区大小
ReadOpQueueFactory
设置读队列工厂
ShouldOptimize
如果默认操作优化是不可取的，设置为false(默认为true)
Transcoder
设置默认的转码器(默认以net.spy.memcached.transcoders.SerializingTranscoder)
UseNagleAlgorithm
如果你想使用Nagle算法，设置为true
WriteOpQueueFactory
设置写队列工厂
AuthDescriptor
设置authDescriptor,在新的连接上使用身份验证
servletmvc.xml
```
<?xml version="1.0" encoding="utf-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:mvc="http://www.springframework.org/schema/mvc"
    xmlns:context="http://www.springframework.org/schema/context"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
            http://www.springframework.org/schema/mvc 
            http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd
            http://www.springframework.org/schema/context 
            http://www.springframework.org/schema/context/spring-context-4.0.xsd">
    <context:component-scan base-package="com.memcached.spymemcached">
        <context:include-filter type="annotation" expression="org.springframework.stereotype.Controller" />
    </context:component-scan>
</beans>
```
2.[Java](http://lib.csdn.net/base/javaee)后台代码
```java
import java.util.concurrent.TimeUnit;
public interface SpyMemcachedConstants {
    public static int DEFAULT_TIMEOUT = 5;
    public static TimeUnit DEFAULT_TIMEUNIT = TimeUnit.SECONDS;
}
```
```java
import java.io.IOException;
import java.io.OutputStream;
import java.net.SocketAddress;
import java.util.Collection;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Future;
import net.spy.memcached.ConnectionObserver;
import net.spy.memcached.MemcachedClient;
import net.spy.memcached.transcoders.Transcoder;
public class SpyMemcachedManager {
    private MemcachedClient memcachedClient;
    public void addObserver(ConnectionObserver obs) {
        memcachedClient.addObserver(obs);
    }
    public void removeObserver(ConnectionObserver obs) {
        memcachedClient.removeObserver(obs);
    }
    // ---- Basic Operation Start ----//
    public boolean set(String key, Object value, int expire) {
        Future<Boolean> f = memcachedClient.set(key, expire, value);
        return getBooleanValue(f);
    }
    public Object get(String key) {
        return memcachedClient.get(key);
    }
    public Object asyncGet(String key) {
        Object obj = null;
        Future<Object> f = memcachedClient.asyncGet(key);
        try {
            obj = f.get(SpyMemcachedConstants.DEFAULT_TIMEOUT, SpyMemcachedConstants.DEFAULT_TIMEUNIT);
        } catch (Exception e) {
            f.cancel(false);
        }
        return obj;
    }
    public boolean add(String key, Object value, int expire) {
        Future<Boolean> f = memcachedClient.add(key, expire, value);
        return getBooleanValue(f);
    }
    public boolean replace(String key, Object value, int expire) {
        Future<Boolean> f = memcachedClient.replace(key, expire, value);
        return getBooleanValue(f);
    }
    public boolean delete(String key) {
        Future<Boolean> f = memcachedClient.delete(key);
        return getBooleanValue(f);
    }
    public boolean flush() {
        Future<Boolean> f = memcachedClient.flush();
        return getBooleanValue(f);
    }
    public Map<String, Object> getMulti(Collection<String> keys) {
        return memcachedClient.getBulk(keys);
    }
    public Map<String, Object> getMulti(String[] keys) {
        return memcachedClient.getBulk(keys);
    }
    public Map<String, Object> asyncGetMulti(Collection<String> keys) {
        Map map = null;
        Future<Map<String, Object>> f = memcachedClient.asyncGetBulk(keys);
        try {
            map = f.get(SpyMemcachedConstants.DEFAULT_TIMEOUT, SpyMemcachedConstants.DEFAULT_TIMEUNIT);
        } catch (Exception e) {
            f.cancel(false);
        }
        return map;
    }
    public Map<String, Object> asyncGetMulti(String keys[]) {
        Map map = null;
        Future<Map<String, Object>> f = memcachedClient.asyncGetBulk(keys);
        try {
            map = f.get(SpyMemcachedConstants.DEFAULT_TIMEOUT, SpyMemcachedConstants.DEFAULT_TIMEUNIT);
        } catch (Exception e) {
            f.cancel(false);
        }
        return map;
    }
    // ---- Basic Operation End ----//
    // ---- increment & decrement Start ----//
    public long increment(String key, int by, long defaultValue, int expire) {
        return memcachedClient.incr(key, by, defaultValue, expire);
    }
    public long increment(String key, int by) {
        return memcachedClient.incr(key, by);
    }
    public long decrement(String key, int by, long defaultValue, int expire) {
        return memcachedClient.decr(key, by, defaultValue, expire);
    }
    public long decrement(String key, int by) {
        return memcachedClient.decr(key, by);
    }
    public long asyncIncrement(String key, int by) {
        Future<Long> f = memcachedClient.asyncIncr(key, by);
        return getLongValue(f);
    }
    public long asyncDecrement(String key, int by) {
        Future<Long> f = memcachedClient.asyncDecr(key, by);
        return getLongValue(f);
    }
    // ---- increment & decrement End ----//
    public void printStats() throws IOException {
        printStats(null);
    }
    public void printStats(OutputStream stream) throws IOException {
        Map<SocketAddress, Map<String, String>> statMap = memcachedClient
                .getStats();
        if (stream == null) {
            stream = System.out;
        }
        StringBuffer buf = new StringBuffer();
        Set<SocketAddress> addrSet = statMap.keySet();
        Iterator<SocketAddress> iter = addrSet.iterator();
        while (iter.hasNext()) {
            SocketAddress addr = iter.next();
            buf.append(addr.toString() + "/n");
            Map<String, String> stat = statMap.get(addr);
            Set<String> keys = stat.keySet();
            Iterator<String> keyIter = keys.iterator();
            while (keyIter.hasNext()) {
                String key = keyIter.next();
                String value = stat.get(key);
                buf.append("  key=" + key + ";value=" + value + "/n");
            }
            buf.append("/n");
        }
        stream.write(buf.toString().getBytes());
        stream.flush();
    }
    public Transcoder getTranscoder() {
        return memcachedClient.getTranscoder();
    }
    private long getLongValue(Future<Long> f) {
        try {
            Long l = f.get(SpyMemcachedConstants.DEFAULT_TIMEOUT, SpyMemcachedConstants.DEFAULT_TIMEUNIT);
            return l.longValue();
        } catch (Exception e) {
            f.cancel(false);
        }
        return -1;
    }
    private boolean getBooleanValue(Future<Boolean> f) {
        try {
            Boolean bool = f.get(SpyMemcachedConstants.DEFAULT_TIMEOUT, SpyMemcachedConstants.DEFAULT_TIMEUNIT);
            return bool.booleanValue();
        } catch (Exception e) {
            f.cancel(false);
            return false;
        }
    }
    public MemcachedClient getMemcachedClient() {
        return memcachedClient;
    }
    public void setMemcachedClient(MemcachedClient memcachedClient) {
        this.memcachedClient = memcachedClient;
    }
}
```
```java
import java.io.PrintWriter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
@RequestMapping("/memcached/demo1")
public class Demo1 {
    @Autowired
    private SpyMemcachedManager memcachedManager;
    @RequestMapping("add")
    public void add(HttpServletResponse response, HttpServletRequest request) {
        try {
            boolean flag = memcachedManager.add("key1", "add", 1000);
            if (flag) {
                this.responseText("add成功,key=key1,value=add", response);
            } else {
                this.responseText("add失败！", response);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @RequestMapping("get")
    public void get(HttpServletResponse response, HttpServletRequest request) {
        try {
            Object val = memcachedManager.get("key1");
            this.responseText("get,key=key1,value=" + val, response);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @RequestMapping("replace")
    public void replace(HttpServletResponse response, HttpServletRequest request) {
        try {
            boolean flag = memcachedManager.replace("key1", "replace", 1000);
            if (flag) {
                this.responseText("replace成功,key=key1,value=replace", response);
            } else {
                this.responseText("replace失败！", response);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @RequestMapping("set")
    public void set(HttpServletResponse response, HttpServletRequest request) {
        try {
            boolean flag = memcachedManager.set("key1", "set", 1000);
            if (flag) {
                this.responseText("set成功,key=key1,value=set", response);
            } else {
                this.responseText("set失败！", response);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    /**
     * 响应字符串 用于ajax请求响应
     * @param str
     * @throws Exception
     */
    public void responseText(String str, HttpServletResponse reponse) throws  Exception{
        reponse.setContentType("text/html;charset=UTF-8");
        PrintWriter out = reponse.getWriter();
        out.write(str);
        out.flush();
        out.close();
    }   
}
```
3.测试
启动memcached
```
[root@localhost ~]# memcached -m 64 -l 192.168.1.100 -p 11211-u root
1
```
分别访问如下的接口:
add 
[http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/add.do](http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/add.do)
显示”add成功,key=key1,value=add”,表示成功. 
[http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/get.do](http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/get.do)
显示”get,key=key1,value=add”,表示成功.
replace 
[http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/replace.do](http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/replace.do)
显示”replace成功,key=key1,value=replace”,表示成功. 
[http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/get.do](http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/get.do)
显示”get,key=key1,value=replace”,表示成功.
set 
[http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/set.do](http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/set.do)
显示”set成功,key=key1,value=set”,表示成功. 
[http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/get.do](http://localhost:8080/SpyMemcachedDemo/spymemcached/demo1/get.do)
显示”get,key=key1,value=set”,表示成功.
-完-
[Demo下载](http://www.hyqian.com/wp-content/uploads/2016/02/SpyMemcachedDemo.tar.gz)
参考:
memcached和spymemcached的使用参考上一遍文章:[http://blog.csdn.net/jifengwan/article/details/50640766](http://blog.csdn.net/jifengwan/article/details/50640766)
memcached之java客户端:spymemcached与[spring](http://lib.csdn.net/base/javaee)整合:[http://blog.csdn.net/fhd001/article/details/6764542](http://blog.csdn.net/fhd001/article/details/6764542)
spring注入枚举类型作为参数:[http://blog.csdn.net/yingkongshi99/article/details/22991931](http://blog.csdn.net/yingkongshi99/article/details/22991931)
