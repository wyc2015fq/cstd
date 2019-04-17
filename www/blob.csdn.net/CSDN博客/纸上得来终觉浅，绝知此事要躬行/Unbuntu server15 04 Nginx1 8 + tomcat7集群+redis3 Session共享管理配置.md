# Unbuntu server15.04 Nginx1.8 + tomcat7集群+redis3 Session共享管理配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年11月03日 17:06:43[boonya](https://me.csdn.net/boonya)阅读数：3727








        前言：由于笔记本内存不足和Ubuntu操作不熟等原因，搞了两天终于搞定了Unbuntu server15.04 Nginx1.8 + tomcat7+redis-3.05Session共享管理配置，这里先记录配置过程及遇到的问题，以此做一个总结和参考。

        环境：JDK使用1.7.51，Tomcat7.63，Linx（Ubuntu server15.04），redis-3.0.5，Ubuntu虚拟机IP：192.168.28.194.


#### 1.Nginx安装配置

##### 1.1Nginx安装

   此处部分不再做介绍，可以参看文章：[http://blog.csdn.net/boonya/article/details/49507219](http://blog.csdn.net/boonya/article/details/49507219)

##### 1.2Nginx配置

配置$nginx/conf路径下nginx.conf文件：


（1）修改Nginx监听端口和服务器访问节点



```python
upstream tomcat {
        server localhost:8101;
        server localhost:8102;
    }

    server {
        listen       8080;
        server_name  tomcat;
        .....................
   }
```
（2）Nginx详细配置





```python
#user  nobody;
worker_processes  1;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;

    upstream tomcat {
        server localhost:8101;
        server localhost:8102;
    }

    server {
        listen       8080;
        server_name  tomcat;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            root   html;
            index  index.html index.htm;
            proxy_pass http://tomcat;
        }

        location /status{  
            stub_status on;  
            access_log off;  
            auth_basic "NginxStatus";  
            auth_basic_user_file htpasswd;  
        }  

        location ~ \.jsp$ {  
            proxy_pass  http://tomcat;  
        }                
   
        location ~ \.(html|js|css|png|gif)$ {  
            root html;  
            proxy_pass  http://tomcat;
        }  

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }

        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}

        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}

        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }


    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}


    # HTTPS server
    #
    #server {
    #    listen       443 ssl;
    #    server_name  localhost;

    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;

    #    ssl_session_cache    shared:SSL:1m;
    #    ssl_session_timeout  5m;

    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers  on;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}
```



#### 2.Tomcat集群节点配置

##### 2.1创建文件夹和tomcat集群节点目录

（1）在/usr/local下创建文件夹tomcatcluster

![](https://img-blog.csdn.net/20151103145355612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）在tomcatcluster下分别创建访问端口为8101和8102的实例

cp 下载的tomcat压缩包到tomcatcluster下，分别执行两次解压并重名了为tomcat8101和tomcat8102.


![](https://img-blog.csdn.net/20151103145413012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

##### 2.2分别修改tomcat节点配置文件/conf/context.xml

在<Context>对象中加入如下内容：



```
<Valve className="com.radiadesign.catalina.session.RedisSessionHandlerValve" />  

  <Manager className="com.radiadesign.catalina.session.RedisSessionManager"  
             host="192.168.28.194"   
             port="6379"  
             database="0"   
	     password="boonya"
             maxInactiveInterval="60" />
```

注：这里的password属性视redis是否设置密码而定，如果redis没有设置密码可以省去password的配置。


##### 2.3context.xml实现redis session管理的参数配置类



```java
package com.radiadesign.catalina.session;

import org.apache.catalina.Globals;
import org.apache.catalina.Lifecycle;
import org.apache.catalina.LifecycleException;
import org.apache.catalina.LifecycleListener;
import org.apache.catalina.util.LifecycleSupport;
import org.apache.catalina.LifecycleState;
import org.apache.catalina.Loader;
import org.apache.catalina.Valve;
import org.apache.catalina.Session;
import org.apache.catalina.session.ManagerBase;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import redis.clients.jedis.JedisPool;
import redis.clients.jedis.JedisPoolConfig;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.Protocol;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Set;


public class RedisSessionManager extends ManagerBase implements Lifecycle {

  protected byte[] NULL_SESSION = "null".getBytes();

  private final Log log = LogFactory.getLog(RedisSessionManager.class);

  protected String host = "localhost";
  protected int port = 6379;
  protected int database = 0;
  protected String password = null;
  protected int timeout = Protocol.DEFAULT_TIMEOUT;
  protected JedisPool connectionPool;

  protected RedisSessionHandlerValve handlerValve;
  protected ThreadLocal<RedisSession> currentSession = new ThreadLocal<>();
  protected ThreadLocal<String> currentSessionId = new ThreadLocal<>();
  protected ThreadLocal<Boolean> currentSessionIsPersisted = new ThreadLocal<>();
  protected Serializer serializer;

  protected static String name = "RedisSessionManager";

  protected String serializationStrategyClass = "com.radiadesign.catalina.session.JavaSerializer";

  /**
   * The lifecycle event support for this component.
   */
  protected LifecycleSupport lifecycle = new LifecycleSupport(this);

  public String getHost() {
    return host;
  }

  public void setHost(String host) {
    this.host = host;
  }

  public int getPort() {
    return port;
  }

  public void setPort(int port) {
    this.port = port;
  }

  public int getDatabase() {
    return database;
  }

  public void setDatabase(int database) {
    this.database = database;
  }

  public int getTimeout() {
    return timeout;
  }

  public void setTimeout(int timeout) {
    this.timeout = timeout;
  }

  public String getPassword() {
    return password;
  }

  public void setPassword(String password) {
    this.password = password;
  }

  public void setSerializationStrategyClass(String strategy) {
    this.serializationStrategyClass = strategy;
  }

  public int getRejectedSessions() {
    // Essentially do nothing.
    return 0;
  }

  public void setRejectedSessions(int i) {
    // Do nothing.
  }

  protected Jedis acquireConnection() {
    Jedis jedis = connectionPool.getResource();

    if (getDatabase() != 0) {
      jedis.select(getDatabase());
    }

    return jedis;
  }

  protected void returnConnection(Jedis jedis, Boolean error) {
    if (error) {
      connectionPool.returnBrokenResource(jedis);
    } else {
      connectionPool.returnResource(jedis);
    }
  }

  protected void returnConnection(Jedis jedis) {
    returnConnection(jedis, false);
  }

  public void load() throws ClassNotFoundException, IOException {

  }

  public void unload() throws IOException {

  }

  /**
   * Add a lifecycle event listener to this component.
   *
   * @param listener The listener to add
   */
  public void addLifecycleListener(LifecycleListener listener) {
    lifecycle.addLifecycleListener(listener);
  }

  /**
   * Get the lifecycle listeners associated with this lifecycle. If this
   * Lifecycle has no listeners registered, a zero-length array is returned.
   */
  public LifecycleListener[] findLifecycleListeners() {
    return lifecycle.findLifecycleListeners();
  }


  /**
   * Remove a lifecycle event listener from this component.
   *
   * @param listener The listener to remove
   */
  public void removeLifecycleListener(LifecycleListener listener) {
    lifecycle.removeLifecycleListener(listener);
  }

  /**
   * Start this component and implement the requirements
   * of {@link org.apache.catalina.util.LifecycleBase#startInternal()}.
   *
   * @exception LifecycleException if this component detects a fatal error
   *  that prevents this component from being used
   */
  @Override
  protected synchronized void startInternal() throws LifecycleException {
    super.startInternal();

    setState(LifecycleState.STARTING);

    Boolean attachedToValve = false;
    for (Valve valve : getContainer().getPipeline().getValves()) {
      if (valve instanceof RedisSessionHandlerValve) {
        this.handlerValve = (RedisSessionHandlerValve) valve;
        this.handlerValve.setRedisSessionManager(this);
        log.info("Attached to RedisSessionHandlerValve");
        attachedToValve = true;
        break;
      }
    }

    if (!attachedToValve) {
      String error = "Unable to attach to session handling valve; sessions cannot be saved after the request without the valve starting properly.";
      log.fatal(error);
      throw new LifecycleException(error);
    }

    try {
      initializeSerializer();
    } catch (ClassNotFoundException | InstantiationException | IllegalAccessException e) {
      log.fatal("Unable to load serializer", e);
      throw new LifecycleException(e);
    }

    log.info("Will expire sessions after " + getMaxInactiveInterval() + " seconds");

    initializeDatabaseConnection();

    setDistributable(true);
  }


  /**
   * Stop this component and implement the requirements
   * of {@link org.apache.catalina.util.LifecycleBase#stopInternal()}.
   *
   * @exception LifecycleException if this component detects a fatal error
   *  that prevents this component from being used
   */
  @Override
  protected synchronized void stopInternal() throws LifecycleException {
    if (log.isDebugEnabled()) {
      log.debug("Stopping");
    }

    setState(LifecycleState.STOPPING);

    try {
      connectionPool.destroy();
    } catch(Exception e) {
      // Do nothing.
    }

    // Require a new random number generator if we are restarted
    super.stopInternal();
  }

  @Override
  public Session createSession(String sessionId) {
    RedisSession session = (RedisSession)createEmptySession();

    // Initialize the properties of the new session and return it
    session.setNew(true);
    session.setValid(true);
    session.setCreationTime(System.currentTimeMillis());
    session.setMaxInactiveInterval(getMaxInactiveInterval());

    String jvmRoute = getJvmRoute();

    Boolean error = true;
    Jedis jedis = null;

    try {
      jedis = acquireConnection();

      // Ensure generation of a unique session identifier.
      do {
        if (null == sessionId) {
          sessionId = generateSessionId();
        }

        if (jvmRoute != null) {
          sessionId += '.' + jvmRoute;
        }
      } while (jedis.setnx(sessionId.getBytes(), NULL_SESSION) == 1L); // 1 = key set; 0 = key already existed

      /* Even though the key is set in Redis, we are not going to flag
         the current thread as having had the session persisted since
         the session isn't actually serialized to Redis yet.
         This ensures that the save(session) at the end of the request
         will serialize the session into Redis with 'set' instead of 'setnx'. */

      error = false;

      session.setId(sessionId);
      session.tellNew();

      currentSession.set(session);
      currentSessionId.set(sessionId);
      currentSessionIsPersisted.set(false);
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }

    return session;
  }

  @Override
  public Session createEmptySession() {
    return new RedisSession(this);
  }

  @Override
  public void add(Session session) {
    try {
      save(session);
    } catch (IOException ex) {
      log.warn("Unable to add to session manager store: " + ex.getMessage());
      throw new RuntimeException("Unable to add to session manager store.", ex);
    }
  }

  @Override
  public Session findSession(String id) throws IOException {
    RedisSession session;

    if (id == null) {
      session = null;
      currentSessionIsPersisted.set(false);
    } else if (id.equals(currentSessionId.get())) {
      session = currentSession.get();
    } else {
      session = loadSessionFromRedis(id);

      if (session != null) {
        currentSessionIsPersisted.set(true);
      }
    }

    currentSession.set(session);
    currentSessionId.set(id);

    return session;
  }

  public void clear() {
    Jedis jedis = null;
    Boolean error = true;
    try {
      jedis = acquireConnection();
      jedis.flushDB();
      error = false;
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }
  }

  public int getSize() throws IOException {
    Jedis jedis = null;
    Boolean error = true;
    try {
      jedis = acquireConnection();
      int size = jedis.dbSize().intValue();
      error = false;
      return size;
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }
  }

  public String[] keys() throws IOException {
    Jedis jedis = null;
    Boolean error = true;
    try {
      jedis = acquireConnection();
      Set<String> keySet = jedis.keys("*");
      error = false;
      return keySet.toArray(new String[keySet.size()]);
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }
  }

  public RedisSession loadSessionFromRedis(String id) throws IOException {
    RedisSession session;

    Jedis jedis = null;
    Boolean error = true;

    try {
      log.trace("Attempting to load session " + id + " from Redis");

      jedis = acquireConnection();
      byte[] data = jedis.get(id.getBytes());
      error = false;

      if (data == null) {
        log.trace("Session " + id + " not found in Redis");
        session = null;
      } else if (Arrays.equals(NULL_SESSION, data)) {
        throw new IllegalStateException("Race condition encountered: attempted to load session[" + id + "] which has been created but not yet serialized.");
      } else {
        log.trace("Deserializing session " + id + " from Redis");
        session = (RedisSession)createEmptySession();
        serializer.deserializeInto(data, session);
        session.setId(id);
        session.setNew(false);
        session.setMaxInactiveInterval(getMaxInactiveInterval() * 1000);
        session.access();
        session.setValid(true);
        session.resetDirtyTracking();

        if (log.isTraceEnabled()) {
          log.trace("Session Contents [" + id + "]:");
          for (Object name : Collections.list(session.getAttributeNames())) {
              log.trace("  " + name);
          }
        }
      }

      return session;
    } catch (IOException e) {
      log.fatal(e.getMessage());
      throw e;
    } catch (ClassNotFoundException ex) {
      log.fatal("Unable to deserialize into session", ex);
      throw new IOException("Unable to deserialize into session", ex);
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }
  }

  public void save(Session session) throws IOException {
    Jedis jedis = null;
    Boolean error = true;

    try {
      log.trace("Saving session " + session + " into Redis");

      RedisSession redisSession = (RedisSession) session;

      if (log.isTraceEnabled()) {
        log.trace("Session Contents [" + redisSession.getId() + "]:");
        for (Object name : Collections.list(redisSession.getAttributeNames())) {
          log.trace("  " + name);
        }
      }

      Boolean sessionIsDirty = redisSession.isDirty();

      redisSession.resetDirtyTracking();
      byte[] binaryId = redisSession.getId().getBytes();

      jedis = acquireConnection();

      if (sessionIsDirty || currentSessionIsPersisted.get() != true) {
        jedis.set(binaryId, serializer.serializeFrom(redisSession));
      }

      currentSessionIsPersisted.set(true);

      log.trace("Setting expire timeout on session [" + redisSession.getId() + "] to " + getMaxInactiveInterval());
      jedis.expire(binaryId, getMaxInactiveInterval());

      error = false;
    } catch (IOException e) {
      log.error(e.getMessage());

      throw e;
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }
  }

  public void remove(Session session) {
    remove(session, false);
  }

  public void remove(Session session, boolean update) {
    Jedis jedis = null;
    Boolean error = true;

    log.trace("Removing session ID : " + session.getId());

    try {
      jedis = acquireConnection();
      jedis.del(session.getId());
      error = false;
    } finally {
      if (jedis != null) {
        returnConnection(jedis, error);
      }
    }
  }

  public void afterRequest() {
    RedisSession redisSession = currentSession.get();
    if (redisSession != null) {
      currentSession.remove();
      currentSessionId.remove();
      currentSessionIsPersisted.remove();
      log.trace("Session removed from ThreadLocal :" + redisSession.getIdInternal());
    }
  }

  @Override
  public void processExpires() {
    // We are going to use Redis's ability to expire keys for session expiration.

    // Do nothing.
  }

  private void initializeDatabaseConnection() throws LifecycleException {
    try {
      // TODO: Allow configuration of pool (such as size...)
      connectionPool = new JedisPool(new JedisPoolConfig(), getHost(), getPort(), getTimeout(), getPassword());
    } catch (Exception e) {
      e.printStackTrace();
      throw new LifecycleException("Error Connecting to Redis", e);
    }
  }

  private void initializeSerializer() throws ClassNotFoundException, IllegalAccessException, InstantiationException {
    log.info("Attempting to use serializer :" + serializationStrategyClass);
    serializer = (Serializer) Class.forName(serializationStrategyClass).newInstance();

    Loader loader = null;

    if (container != null) {
      loader = container.getLoader();
    }

    ClassLoader classLoader = null;

    if (loader != null) {
        classLoader = loader.getClassLoader();
    }
    serializer.setClassLoader(classLoader);
  }
}
```


源码可以从：[https://github.com/jcoleman/tomcat-redis-session-manager/downloads](https://github.com/jcoleman/tomcat-redis-session-manager/downloads)下载。



##### 2.4redis session管理环境下Tomcat支持包

如下图所示：

![](https://img-blog.csdn.net/20151103151543252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：第二个包jedis-session-manager-mine.jar是我通过源码包重新打包的，因为第一个包tomcat-redis-session-manager-1.2-tomcat-7-java-7.jar似乎打包有问题。这里有必要添加commons-pools-2.4.2.jar，最早我加的是1.6的在Tomcat7下会抛异常（主要是版本不符合）。另外，日志包也是必须的。

##### 2.5分别在Tomcat节点的webapps\ROOT路径下添加测试页面

demo.jsp



```
<%@ page contentType="text/html; charset=UTF-8" %>     
    <%@ page import="java.util.*" %>  
    <html>  
    <head><title>Cluster Test</title></head>     
    <body>    
     <%      
     HttpSession sess = request.getSession(true);      
     System.out.println(sess.getId());    
     String host_ip=request.getServerName()+":"+request.getServerPort();
     out.println("<br> HOST:IP:" + host_ip + "<br>");

     out.println("<br> SESSION ID:" + sess.getId()+"<br>");      
     // 如果有新的请求，则添加session属性    
     String name = request.getParameter("name");       
    if (name != null && name.length() > 0) {     
          String value = request.getParameter("value");       
          sess.setAttribute(name, value);    
    }          
    out.print("<b>Session List:</b>");       
    Enumeration<String> names = sess.getAttributeNames();        
    while (names.hasMoreElements()) {       
         String sname = names.nextElement();        
          String value = sess.getAttribute(sname).toString();         
          out.println( sname + " = " + value+"<br>");        
          System.out.println( sname + " = " + value);    
    }   
      %>jvm1  
    </body>    
     </html>
```



#### 3.测试Nginx访问Tomcat集群

##### 3.1分别启动Tomcat集群节点

启动



```python
/usr/local/tomcatcluster/tomcat8101/bin# ./startup.sh

/usr/local/tomcatcluster/tomcat8102/bin# ./startup.sh
```



关闭




```python
/usr/local/tomcatcluster/tomcat8101/bin# ./shutdown.sh

/usr/local/tomcatcluster/tomcat8102/bin# ./shutdown.sh
```



##### 3.2启动Nginx

启动




```python
sbin/nginx
```



停止




```python
sbin/nginx -s stop
```



##### 3.3打开Nginx访问demo页面

![](https://img-blog.csdn.net/20151104095302631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：Nginx绑定的是8080端口，所以此处通过8080来访问；并且此时看到的sessionID经过多次页面请求刷新都保持不变，说明已经配置成功。


#### 4.关于配置过程中遇到的问题处理方式

##### 4.1通过Nginx访问Tomcat节点无页面返回

此种方式是由于Tomcat没有成功启动，因为在linux下错误不会直接像Windows窗口那样提示出来，这种问题应考虑Tomcat不能正常启动。


![](https://img-blog.csdn.net/20151103153838727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如上所示，上面的页面没有任何返回信息。另外，也可以试图测试访问Tomcat对应的端口地址，如果也是此种情况，也能说明Tomcat没有正常启动。

##### 4.2处理问题的方式

有两种方式可以去尝试处理问题：


（1）可以尝试在windows下使用一个Tomcat测试一下实际产生的问题，解决问题之后再到linux下去修正问题。下面是我在Windows处理问题成功后的截图：

![](https://img-blog.csdn.net/20151103170448863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

多次发送请求后session保持不变，如下图所示：

![](https://img-blog.csdn.net/20151103170536765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）可以查找Tomcat的catalina的日志进行查看处理。此种方式的困难是，linux下日志查看并不方便。


#### 5.参考资料及资源

tomcat-redis-session-manager：[https://github.com/jcoleman/tomcat-redis-session-manager](https://github.com/jcoleman/tomcat-redis-session-manager)


Nginx+Tomcat+Redis session共享：[http://dinguangx.iteye.com/blog/2230980](http://dinguangx.iteye.com/blog/2230980)

redis + Tomcat 8 的session共享解决：[http://www.cnblogs.com/interdrp/p/4868740.html](http://www.cnblogs.com/interdrp/p/4868740.html)

Apache commons pool:[http://commons.apache.org/proper/commons-pool/download_pool.cgi](http://commons.apache.org/proper/commons-pool/download_pool.cgi)




