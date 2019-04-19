# spring整合spymemcached实现Memcached的各种操作 - 零度的博客专栏 - CSDN博客
2017年01月19日 11:17:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：3274
1.配置pom.xml、web.xml、applicationContext.xml和servletmvc.xml,仅仅列出与memcached相关配置
pom.xml
```
<dependency>      
		    <groupId>net.spy</groupId>
		    <artifactId>spymemcached</artifactId>
		    <version>2.12.0</version>
		</dependency>
```
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0"  xmlns="http://java.sun.com/xml/ns/javaee" 
	     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
    
    <display-name>SSH-Application</display-name>	
    
	<!--初始化参数-->
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>/WEB-INF/classes/applicationContext*.xml</param-value>
	</context-param> 
	
	<context-param>  
	 	<param-name>javax.servlet.jsp.jstl.fmt.localizationContext</param-name>  
	 	<param-value>message/message-info</param-value>		
	</context-param>
	
	<!--Spring ContextLoaderListener -->
        <listener>
		<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
	</listener> 
	
	
        <!-- ETag过滤器,节省带宽 -->
	<filter>
		<filter-name>etagFilter</filter-name>
		<filter-class>org.springframework.web.filter.ShallowEtagHeaderFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>etagFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	
        <!--UTF-8编码 -->
	<filter>
		<filter-name>CharacterEncodingFilter</filter-name>
		<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>UTF-8</param-value>
		</init-param>
		<init-param>
			<param-name>forceEncoding</param-name>
			<param-value>true</param-value>
		</init-param>
	</filter>	
	<filter-mapping>
		<filter-name>CharacterEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	
        <!-- OpenSessionInViewFilter -->
 	<filter>
		<filter-name>OpenSessionInViewFilter</filter-name>
		<filter-class>org.springframework.orm.hibernate4.support.OpenSessionInViewFilter</filter-class>
		<init-param>
			<param-name>singleSession</param-name>
			<param-value>true</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>OpenSessionInViewFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping> 	
   
 
 
	
        <!-- 清理内存 -->
	<listener>
		<listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
	</listener>
 
        <!-- spring-MVC --> 
	<servlet>
		<servlet-name>springMVC</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>classpath:config/webmvc-config.xml</param-value>
		</init-param>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>springMVC</servlet-name>
		<url-pattern>/</url-pattern>
	</servlet-mapping>
	
	<!-- session过期时长 -->
	<session-config>
		<session-timeout>200</session-timeout>
	</session-config>
	
        <!--默认页面 -->
	<welcome-file-list>
		<welcome-file>/index</welcome-file>
	</welcome-file-list>
	
	<!-- 异常页面 -->
	<error-page>
		<error-code>500</error-code>
		<location>/common/500.jsp</location>
	</error-page>	
	<error-page>
		<error-code>404</error-code>
		<location>/common/404.jsp</location>
	</error-page>
        <error-page>
		<error-code>403</error-code>
		<location>/common/403.jsp</location>
	</error-page>
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
webmvc-config.xml
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:p="http://www.springframework.org/schema/p" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd     
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd 
	http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
    
        <!-- 静态资源 不拦截 -->
	<mvc:resources location="/common/"   mapping="/common/**"/>
	<mvc:resources location="/bank_img/" mapping="/bank_img/**" />
	<mvc:resources location="/image/"    mapping="/image/**" />
	<mvc:resources location="/images/"   mapping="/images/**" />
	<mvc:resources location="/products/" mapping="/products/**" />
	<mvc:resources location="/js/"       mapping="/js/**" /> 
	<mvc:resources location="/css/"      mapping="/css/**" />
	<mvc:resources location="/htm/"      mapping="/htm/**" />
	
    	
	<context:annotation-config/>
	
	<context:component-scan base-package="org.nercita.bcp" use-default-filters="false">
	    <context:include-filter expression="org.springframework.stereotype.Controller" type="annotation" />
	    <context:include-filter expression="org.springframework.web.bind.annotation.ControllerAdvice" type="annotation" />
	</context:component-scan>
	
	<bean id="handlerMapping"  class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping"/>  
	<bean id="handlerAdapter"  class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">
		<property name="messageConverters">
			<list>
				<bean class="org.nercita.core.web.springmvc.StringHttpMessageConverter" />
				<ref bean="msgConverter"/>
			</list>
		</property>
		<property name="webBindingInitializer">
			<bean class="org.nercita.core.web.springmvc.CustomBindInitializer">		   			 				
			</bean> 
		</property>
	</bean>	
	
 	<bean id="msgConverter" class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
        <property name="supportedMediaTypes">
            <list>
                <value>text/html;charset=UTF-8</value>
                <value>text/json;charset=UTF-8</value>
                <value>application/json;charset=UTF-8</value>
            </list>
        </property>
    </bean>  
    
     
    <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="ignoreAcceptHeader" value="true"/>
        <property name="defaultContentType" value="text/html"/>
        <property name="mediaTypes">
            <map>
                <entry key="json" value="application/json"/>
                <entry key="xml" value="application/xml"/>
            </map>
        </property>
        <property name="favorParameter" value="false"/>
        <property name="viewResolvers">
            <list>
                <bean class="org.springframework.web.servlet.view.BeanNameViewResolver"/>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
                    <property name="prefix" value="/WEB-INF/views/"/>
                    <property name="suffix" value=".jsp"/>
                </bean>
            </list>
        </property>
        <property name="defaultViews">
            <list>
                <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView" />
    
                <bean class="org.springframework.web.servlet.view.xml.MarshallingView">
                    <property name="marshaller">
                        <bean class="org.springframework.oxm.xstream.XStreamMarshaller"/>
                    </property>
                </bean>
            </list>
        </property>
    </bean> 
    
    <bean class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
		<property name="defaultErrorView" value="error" />
		<property name="exceptionMappings">
			<props>
				<prop key=".DataAccessException">dataAccessFailure</prop>
				<prop key=".NoSuchRequestHandlingMethodException">resourceNotFound</prop>
				<prop key=".TypeMismatchException">resourceNotFound</prop>
				<prop key=".lang.Exception">uncaughtException</prop>
			</props>
		</property>	
		<property name="statusCodes">     
             <props>     
                 <prop key="errors/error">500</prop>     
                 <prop key="errors/404">404</prop>     
             </props>     
         </property>     
         <!-- 设置日志输出级别，不定义则控制台默认不输出警告等错误日志信息 -->     
         <property name="warnLogCategory" value="WARN"></property>
			
	</bean>   
 	
 
</beans>
```
2、后台代码
```
package org.nercita.bcp.memcached.util;
import java.util.concurrent.TimeUnit;
public interface SpyMemcachedConstants {
    public static int DEFAULT_TIMEOUT = 5;
    public static TimeUnit DEFAULT_TIMEUNIT = TimeUnit.SECONDS;
}
```
```
package org.nercita.bcp.memcached.service;
import java.io.IOException;
import java.io.OutputStream;
import java.net.SocketAddress;
import java.util.Collection;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.Future;
import javax.annotation.Resource;
import org.nercita.bcp.memcached.util.SpyMemcachedConstants;
import org.springframework.stereotype.Service;
import net.spy.memcached.ConnectionObserver;
import net.spy.memcached.MemcachedClient;
import net.spy.memcached.transcoders.Transcoder;
@Service
public class SpyMemcachedService {
	@Resource(name="memcachedClient")
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
        Map<String, Object> map = null;
        Future<Map<String, Object>> f = memcachedClient.asyncGetBulk(keys);
        try {
            map = f.get(SpyMemcachedConstants.DEFAULT_TIMEOUT, SpyMemcachedConstants.DEFAULT_TIMEUNIT);
        } catch (Exception e) {
            f.cancel(false);
        }
        return map;
    }
    public Map<String, Object> asyncGetMulti(String keys[]) {
        Map<String, Object> map = null;
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
    public Transcoder<Object> getTranscoder() {
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
```
package org.nercita.bcp.memcached.web;
import java.io.PrintWriter;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.nercita.bcp.memcached.service.SpyMemcachedService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
@RequestMapping("/memcached/demo")
public class SpyMemcachedController {
    @Autowired
    private SpyMemcachedService spyMemcachedService;
    @RequestMapping("add")
    public void add(HttpServletResponse response, HttpServletRequest request) {
        try {
            boolean flag = spyMemcachedService.add("key1", "add", 1000);
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
            Object val = spyMemcachedService.get("key1");
            this.responseText("get,key=key1,value=" + val, response);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    @RequestMapping("replace")
    public void replace(HttpServletResponse response, HttpServletRequest request) {
        try {
            boolean flag = spyMemcachedService.replace("key1", "replace", 1000);
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
            boolean flag = spyMemcachedService.set("key1", "set", 1000);
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
3、测试
访问：[http://localhost:8080/Sample/memcached/demo/add](http://localhost:8080/Sample/memcached/demo/add)  输出：add成功,key=key1,value=add  测试成功！
访问：[http://localhost:8080/Sample/memcached/demo/get](http://localhost:8080/Sample/memcached/demo/get)   输出：get成功,key=key1,value=add  测试成功！
访问：[http://localhost:8080/Sample/memcached/demo/replace](http://localhost:8080/Sample/memcached/demo/replace)   输出：replace成功,key=key1,value=replace  测试成功！

