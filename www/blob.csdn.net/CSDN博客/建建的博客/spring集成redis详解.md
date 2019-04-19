# spring集成redis详解 - 建建的博客 - CSDN博客
2018年01月25日 18:01:03[纪建](https://me.csdn.net/u013898698)阅读数：93
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
最近工作需要用到redis替代以前的ehcache作为缓存工具，所以就研究了一下。如果你不知道redis怎么安装和使用，请参考我的另外一篇文章[点击打开链接](http://blog.csdn.net/fei565789229/article/details/51629258)。
**一、引包**
项目内引入文件jedis-2.1.0.jar、spring-data-redis-1.0.2.RELEASE.jar，如果启动报错，请再引入commons-pool-1.5.5.jar、commons-pool2-2.2.jar。
**二、工作目录下创建配置文件redis.properties**
**[plain]**[view plain](http://blog.csdn.net/fei565789229/article/details/51635466#)[copy](http://blog.csdn.net/fei565789229/article/details/51635466#)
- #访问地址  
- redis_addr=192.168.1.254  
- #访问端口  
- redis_port=6379  
- #授权密码，有没有这一项取决于要连接的redis服务是否设置了此项  
- redis_auth=test123  
- #连接池的最大数据库连接数。设为0表示无限制  
- redis_max_active=1024  
- #最大空闲数，数据库连接的最大空闲时间。超过空闲时间，数据库连接将被标记为不可用，然后被释放。设为0表示无限制。  
- redis_max_idle=200  
- #最大建立连接等待时间。如果超过此时间将接到异常。设为-1表示无限制。  
- redis_max_wait=10000  
- #在borrow一个jedis实例时，是否提前进行alidate操作；如果为true，则得到的jedis实例均是可用的；  
- redis_test_on_borrow=true  
**三、修改spring配置文件applicationContext.xml**
**[html]**[view plain](http://blog.csdn.net/fei565789229/article/details/51635466#)[copy](http://blog.csdn.net/fei565789229/article/details/51635466#)
- <!-- redis配置 -->
- <!-- redis配置文件 -->
- <context:property-placeholderlocation="classpath*:/redis.properties"ignore-unresolvable="true"/>
- 
- <!-- redis连接池 -->
- <beanid="jedisConfig"class="redis.clients.jedis.JedisPoolConfig">
- <propertyname="maxActive"value="${redis_max_active}"></property>
- <propertyname="maxIdle"value="${redis_max_idle}"></property>
- <propertyname="maxWait"value="${redis_max_wait}"></property>
- <propertyname="testOnBorrow"value="${redis_test_on_borrow}"></property>
- </bean>
- <!-- redis连接工厂 -->
- <beanid="connectionFactory"class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory">
- <propertyname="hostName"value="${redis_addr}"></property>
- <propertyname="port"value="${redis_port}"></property>
- <propertyname="password"value="${redis_auth}"></property>
- <propertyname="poolConfig"ref="jedisConfig"></property>
- </bean>
- <!-- redis操作模板，这里采用尽量面向对象的模板 -->
- <beanid="redisTemplate"class="org.springframework.data.redis.core.StringRedisTemplate">
- <propertyname="connectionFactory"ref="connectionFactory"/>
- <!--     如果不配置Serializer，那么存储的时候只能使用String，如果用对象类型存储，那么会提示错误 can't cast to String！！！-->
- <propertyname="keySerializer">
- <beanclass="org.springframework.data.redis.serializer.StringRedisSerializer"/>
- </property>
- <propertyname="valueSerializer">
- <beanclass="org.springframework.data.redis.serializer.JdkSerializationRedisSerializer"/>
- </property>
- </bean>
**四、注入redisTemplate，封装缓存管理类，其接口就不往上贴了。**
**[java]**[view plain](http://blog.csdn.net/fei565789229/article/details/51635466#)[copy](http://blog.csdn.net/fei565789229/article/details/51635466#)
- package com.segerp.lms.web.servlet;  
- 
- import java.util.List;  
- import java.util.concurrent.TimeUnit;  
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.data.redis.core.StringRedisTemplate;  
- import org.springframework.stereotype.Component;  
- import com.google.gson.Gson;  
- 
- @Component("cacheUtil")  
- publicclass CacheUtilImpl implements CacheUtil {  
- 
- @Autowired
- private StringRedisTemplate redisTemplate;//redis操作模板
- 
- 
- publicvoid put(String key, String value) {  
- if (key==null || "".equals(key)) {  
- return;  
-         }  
-         redisTemplate.opsForHash().put(key, key, value);  
- 
-     }  
- 
- 
- publicvoid put(String key, Object value) {  
- if (key==null || "".equals(key)) {  
- return;  
-         }  
-         redisTemplate.opsForHash().put(key, key, new Gson().toJson(value));  
- 
-     }  
- 
- 
- public <T> T get(String key, Class<T> className) {  
-         Object obj = redisTemplate.opsForHash().get(key, key);  
- if(obj == null){  
- returnnull;  
-         }  
- returnnew Gson().fromJson(""+obj, className);  
-     }  
- 
- 
- public String get(String key) {  
-         Object obj = redisTemplate.opsForHash().get(key, key);  
- if(obj == null){  
- returnnull;  
-         }else{  
- return String.valueOf(obj);  
-         }  
-     }  
- 
- }  
其实看的懂上述代码的人能够看出来：我这里是用的google包的Gson把对象转换成json字符串往redis进行存储的，取值时，再通过Gson().fromJson()转换成对象。用此功能时时需要引入gson-2.1.jar
**五、测试**
编写测试方法,下面对对象的操作，基本上满足了平时工作的需要。
**[java]**[view plain](http://blog.csdn.net/fei565789229/article/details/51635466#)[copy](http://blog.csdn.net/fei565789229/article/details/51635466#)
- publicvoid testRedis(){  
- //简单字符串处理
-     cacheUtil.put("name", "test");  
-     System.out.println("String---name--"+cacheUtil.get("name"));  
- 
- //map
-     Map<String,Object> map = new HashMap<String,Object>();  
-     map.put("key", "value");  
-     map.put("key1", "value1");  
-     cacheUtil.put("map", map);  
- //第一种取值方式
-     Map map1 = cacheUtil.get("map",Map.class);  
- if(map1 != null){  
-         System.out.println("first map---"+map1.get("key"));  
-     }  
- //第二种取值方式
-     Map map2 = new Gson().fromJson(cacheUtil.get("map"), new TypeToken<Map<String,Object>>() {}.getType());  
- if(map2 != null){  
-         System.out.println("second map---"+map2.get("key1"));  
-     }  
- 
- 
- //JavaBean处理
-     TUser user = new TUser();  
-     user.setUserName("test");  
-     cacheUtil.put("user",user);  
-     TUser user1 = cacheUtil.get("user",TUser.class);  
-     System.out.println("javaBean--name--"+user1.getUserName());  
- 
- //List<JavaBean>处理
-     List<TUser> list = new ArrayList<TUser>();  
-     list.add(user);  
-     cacheUtil.put("list", list);  
-     List<TUser> list1 = new Gson().fromJson(cacheUtil.get("list"), new TypeToken<List<TUser>>() {}.getType());  
- if(list1 != null){  
-         System.out.println("List<JavaBean>--"+list1.get(0).getUserName());  
-     }  
- 
- 
- //list<String>
-     List<String> newlist = new ArrayList<String>();  
-     newlist.add("str1");  
-     newlist.add("sr2");  
-     cacheUtil.put("newlist", newlist);  
-     List<String> newlist1 =  new Gson().fromJson(cacheUtil.get("newlist"), new TypeToken<List<String>>(){}.getType());  
-     System.out.println("list<String>--"+newlist1);  
- 
- //List<Map<String,Object>>
-     List<Map<String,Object>> nowlist = new ArrayList<Map<String,Object>>();  
-     Map<String,Object> newmap = new HashMap<String,Object>();  
-     newmap.put("key1", "value1");  
-     newmap.put("key2", "value2");  
-     nowlist.add(newmap);  
-     cacheUtil.put("nowlist", nowlist);  
-     List<Map<String,Object>> nowlist1 =  new Gson().fromJson(cacheUtil.get("nowlist"), new TypeToken<List<Map<String,Object>>>(){}.getType());  
- if(nowlist1 !=null ){  
-         System.out.println(nowlist1.get(0).get("key1"));  
-     }  
-     System.out.println("List<Map<String,Object>>--"+nowlist1);  
- 
- //List<Map<String,TUser>>
-     List<Map<String,TUser>> lastList = new ArrayList<Map<String,TUser>>();  
-     Map<String,TUser> lastMap = new HashMap<String, TUser>();  
-     lastMap.put("user", user);  
-     lastList.add(lastMap);  
-     cacheUtil.put("lastList", lastList);  
-     List<Map<String,TUser>> lastList1 =  new Gson().fromJson(cacheUtil.get("lastList"), new TypeToken<List<Map<String,TUser>>>(){}.getType());  
- if(lastList1 != null){  
-         System.out.println("List<Map<String,TUser>>---"+lastList1.get(0).get("user").getUserName());  
-     }  
-     System.out.println(lastList1);  
- }  
输出结果：
**[plain]**[view plain](http://blog.csdn.net/fei565789229/article/details/51635466#)[copy](http://blog.csdn.net/fei565789229/article/details/51635466#)
- String---name--test  
- first map---value  
- second map---value1  
- javaBean--name--test  
- List<JavaBean>--test  
- list<String>--[str1, sr2]  
- value1  
- List<Map<String,Object>>--[{key2=value2, key1=value1}]  
- List<Map<String,TUser>>---test  
- [{user=com.segerp.lms.pojo.TUser@14f1541}]  
**![](https://img-blog.csdn.net/20160611135212036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
最后，上述需要的jar包在[这里](http://download.csdn.net/detail/fei565789229/9546216)。
            
