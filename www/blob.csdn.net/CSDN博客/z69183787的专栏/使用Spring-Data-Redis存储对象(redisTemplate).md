# 使用Spring-Data-Redis存储对象(redisTemplate) - z69183787的专栏 - CSDN博客
2017年07月04日 18:16:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5480

http://blog.csdn.net/u010739551/article/details/48207691
[Redis](http://lib.csdn.net/base/redis)是一种nosql[数据库](http://lib.csdn.net/base/mysql)，在开发中常用做缓存。Jedis是[redis](http://lib.csdn.net/base/redis)在[Java](http://lib.csdn.net/base/java)中的redis-
 client.在此之前，希望已经了解redis的基本使用和Maven的使用。建立Maven Project之后，在POM.xml中添加jedis和[spring](http://lib.csdn.net/base/javaee)-data-redis的依赖如下：
**[html]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- <dependency>
- <groupId>redis.clients</groupId>
- <artifactId>jedis</artifactId>
- <version>2.0.0</version>
- <type>jar</type>
- <scope>compile</scope>
- </dependency>
- <!-- spring-redis -->
- <dependency>
- <groupId>org.springframework.data</groupId>
- <artifactId>spring-data-redis</artifactId>
- <version>1.0.0.RELEASE</version>
- </dependency>
Redis连接数据库参数如下：applicationContext-redis.properties
**[plain]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- #redis config  
- redis.pool.maxActive=100  
- redis.pool.maxIdle=20  
- redis.pool.maxWait=1000  
- redis.pool.testOnBorrow=true  
- redis.hostname=localhost  
- redis.port=6379  
- redis.password=  
在上下文配置中使用key-value读取方式读取properties中的值：
**[html]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- <!-- Jedis 连接池配置-->
- <beanid="jedisPoolConfig"class="redis.clients.jedis.JedisPoolConfig">
- <propertyname="maxActive"value="${redis.pool.maxActive}"/>
- <propertyname="maxIdle"value="${redis.pool.maxIdle}"/>
- <propertyname="maxWait"value="${redis.pool.maxWait}"/>
- <propertyname="testOnBorrow"value="${redis.pool.testOnBorrow}"/>
- </bean>
- <!-- Jedis ConnectionFactory 数据库连接配置-->
- <beanid="jedisConnectionFactory"class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory">
- <propertyname="hostName"value="${redis.hostname}"/>
- <propertyname="port"value="${redis.port}"/>
- <propertyname="password"value="${redis.password}"/>
- <propertyname="poolConfig"ref="jedisPoolConfig"/>
- </bean>
- <!—- redisTemplate配置，redisTemplate是对Jedis的对redis操作的扩展，有更多的操作，封装使操作更便捷 -->
- <beanid="redisTemplate"class="org.springframework.data.redis.core.RedisTemplate"p:connection-factory-ref="jedisConnectionFactory"/>
上面redisTemplate已经基本配置完成。
接下来创建User类，必须实现或者间接实现Serializable接口：
Redis存储对象是使用序列化，spring-data-redis已经将序列化的功能内置，不需要我们去管，我们只需要调用api就可以使用。SerialVersionUID字段对序列化扩展有用，为了以后扩展或者缩减字段时不会造成反序列化出错。
**[java]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- publicclass User implements Serializable {  
- 
- privatestaticfinallong serialVersionUID = -7898194272883238670L;  
- 
- publicstaticfinal String OBJECT_KEY = "USER";  
- 
- public User() {  
-     }  
- 
- public User(String id) {  
-     }  
- 
- public User(String id, String name) {  
- this.id = id;  
- this.name = name;  
-     }  
- 
- private String id;  
- 
- private String name;  
- 
- public String getId() {  
- return id;  
-     }  
- 
- publicvoid setId(String id) {  
- this.id = id;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- public String toString() {  
- return"User [id=" + id + ", name=" + name + "]";  
-     }  
- 
- public String getKey() {  
- return getId();  
-     }  
- 
- public String getObjectKey() {  
- return OBJECT_KEY;  
-     }  
- }  
创建userService类来操作redis增删查改缓存对象。
**[java]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- publicclass UserService {  
- 
-     RedisTemplate<String, User> redisTemplate;  
- 
- public RedisTemplate<String, User> getRedisTemplate() {  
- return redisTemplate;  
-     }  
- 
- publicvoid setRedisTemplate(RedisTemplate<String, User> redisTemplate) {  
- this.redisTemplate = redisTemplate;  
-     }  
- 
- publicvoid put(User user) {  
-         redisTemplate.opsForHash().put(user.getObjectKey(), user.getKey(), user);  
-     }  
- 
- publicvoid delete(User key) {  
-         redisTemplate.opsForHash().delete(key.getObjectKey(), key.getKey());  
-     }  
- 
- public User get(User key) {  
- return (User) redisTemplate.opsForHash().get(key.getObjectKey(), key.getKey());  
-     }  
- }  
在上下文中配置redisTemplate注入，在使用bean方式来注入时，redisTemplate必须有setter/getter方法：
**[html]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- <beanid="userService"class="Service.UserService">
- <propertyname="redisTemplate">
- <refbean="redisTemplate"/>
- </property>
- </bean>
======如果使用注解方式自动注入，则可以注释掉上面的bean配置方式======
在UserService注解@Service(“userService”),也可以在Service里写名字，默认是第一字母小写。
**[java]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- @Service("userService")  
- publicclass UserService {  
- 
- @Autowired
-     RedisTemplate<String, User> redisTemplate;  
-     ……  
-         ……  
- }  
在上下文配置文件中，添加自动扫描包的context节点，Base-package的路径要覆盖包含注解的类文件：
**[html]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- <context:component-scanbase-package="*"/>
在main中来简单操作一下：
**[java]**[view
 plain](http://blog.csdn.net/aacm1992/article/details/21977237#)[copy](http://blog.csdn.net/aacm1992/article/details/21977237#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/253985/fork)
- publicclass Main {  
- publicstaticvoid main( String[] args )  
-     {  
-         ApplicationContext applicationContext = new ClassPathXmlApplicationContext("classpath*:/conf/applicationContext.xml"        );  
-         UserService userService =  (UserService) applicationContext.getBean("userService");  
- 
-         User user1 = new User("user1ID", "User 1");  
-         User user2 = new User("user2ID", "User 2");  
- 
-         System.out.println("==== getting objects from redis ====");  
-         System.out.println("User is not in redis yet: " + userService.get(user1));  
-         System.out.println("User is not in redis yet: " + userService.get(user2));  
- 
-         System.out.println("==== putting objects into redis ====");  
-         userService.put(user1);  
-         userService.put(user2);  
- 
-         System.out.println("==== getting objects from redis ====");  
-         System.out.println("User should be in redis yet: " + userService.get(user1));  
-         System.out.println("User should be in redis yet: " + userService.get(user2));  
- 
-         System.out.println("==== deleting objects from redis ====");  
-         userService.delete(user1);  
-         userService.delete(user2);  
- 
-         System.out.println("==== getting objects from redis ====");  
-         System.out.println("User is not in redis yet: " + userService.get(user1));  
-         System.out.println("User is not in redis yet: " + userService.get(user2));  
- 
-     }  
- }  
确保redis服务器是开启状态之后就可以运行程序。运行结果如下：
![](https://img-blog.csdn.net/20140324183114578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWFjbTE5OTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[](http://blog.csdn.net/u010739551/article/details/48207691#)[](http://blog.csdn.net/u010739551/article/details/48207691#)[](http://blog.csdn.net/u010739551/article/details/48207691#)[](http://blog.csdn.net/u010739551/article/details/48207691#)[](http://blog.csdn.net/u010739551/article/details/48207691#)[](http://blog.csdn.net/u010739551/article/details/48207691#)

