# Redis + Jedis + Spring （一）—— 配置&常规操作（GET SET DEL） - z69183787的专栏 - CSDN博客
2017年07月03日 17:35:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：295
[http://snowolf.iteye.com/blog/1666908](http://snowolf.iteye.com/blog/1666908)
前文有述，Spring提供了对于Redis的专门支持：spring-data-redis。此外，类似的还有：
![](http://dl.iteye.com/upload/attachment/0073/0278/067d6115-9a3d-3d12-b759-3b0e882f8d58.jpg)
我想大部分人对**spring-data-hadoop**、**spring-data-mongodb**、**spring-data-redis**以及**spring-data-jpa**表示关注。![](http://snowolf.iteye.com/images/smiles/icon_biggrin.gif)
# 一、简述
**spring**把专门的数据操作独立封装在**spring-data**系列中，**spring-data-redis**自然是针对**Redis**的独立封装了。
当前版本**1.0.1**，主要是将**jedis**、**jredis**、**rjc**以及**srp**等**Redis Client**进行了封装，同时支持事务。已经让我垂涎欲滴了。当然，当前版本不支持**Sharding**。例如，前文曾经通过**Jedis**通过Client配置，实现一致性哈希，达到Sharding的目的。再一点，如果你早在spring1.x写过SpringJdbc的话，现在会觉得似曾相识。![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
在经过一番思想斗争后，我最终放弃了**Jedis**原生实现，拥抱**spring-data-redis**了。为什么？因为，我需要一个有事务机制的框架，一个不需要显式调用对象池操作的框架。这些**spring-data-redis**都解决了。至于Sharding，当前数据量要求还不大，期待**Redis 3.0**吧。
# 二、配置
对象池配置：
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <bean
- id="jedisPoolConfig"
- class="redis.clients.jedis.JedisPoolConfig"
- >
- <property
- name="maxActive"
- value="${redis.pool.maxActive}"/>
- <property
- name="maxIdle"
- value="${redis.pool.maxIdle}"/>
- <property
- name="maxWait"
- value="${redis.pool.maxWait}"/>
- <property
- name="testOnBorrow"
- value="${redis.pool.testOnBorrow}"/>
- </bean>
工厂实现：
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <bean
- id="jedisConnectionFactory"
- class="org.springframework.data.redis.connection.jedis.JedisConnectionFactory"
- >
- <property
- name="hostName"
- value="${redis.ip}"/>
- <property
- name="port"
- value="${redis.port}"/>
- <property
- name="poolConfig"
- ref="jedisPoolConfig"/>
- </bean>
模板类：
Xml代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- <bean
- class="org.springframework.data.redis.core.RedisTemplate"
- p:connection-factory-ref="jedisConnectionFactory"/>
是不是很像配置一个**JdbcTemplate**？其实就这么简单。![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
redis.properties配置如下：
Properties代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- #最大分配的对象数  
- redis.pool.maxActive=1024
- #最大能够保持idel状态的对象数  
- redis.pool.maxIdle=200
- #当池内没有返回对象时，最大等待时间  
- redis.pool.maxWait=1000
- #当调用borrow Object方法时，是否进行有效性检查  
- redis.pool.testOnBorrow=true  
- 
- #IP  
- redis.ip=10.11.20.140
- #Port  
- redis.port=6379
当前只能用一个节点，期待**Redis 3.0**，Sharding吧！![](http://snowolf.iteye.com/images/smiles/icon_cry.gif)
# 三、GET、SET、DEL操作
**Redis**初来乍练，目前也就是用**Memcached**多些，只会这些基本的操作，在这里献丑了！![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
假定做一个UserDao：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- publicinterface UserDao {  
- /**
-      * @param uid
-      * @param address
-      */
- void save(User user);  
- 
- /**
-      * @param uid
-      * @return
-      */
-     User read(String uid);  
- 
- /**
-      * @param uid
-      */
- void delete(String uid);  
- }  
User对象就这么两个属性：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- publicclass User implements Serializable {  
- 
- privatestaticfinallong serialVersionUID = -1267719235225203410L;  
- 
- private String uid;  
- 
- private String address;  
- }  
实现这三个方法需要得到**RedisTemplate**的支持：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- @Autowired
- private RedisTemplate<Serializable, Serializable> redisTemplate;  
 为什么用序列化泛型？我存的数据都是可序列化的内容。还有更多为什么？其实我也解答不了很多，边练边学，我弄通了一定告诉你！![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
## 1.保存-SET
做一个保存造作，使用**Redis**的**SET**命令：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- @Override
- publicvoid save(final User user) {  
-     redisTemplate.execute(new RedisCallback<Object>() {  
- @Override
- public Object doInRedis(RedisConnection connection)  
- throws DataAccessException {  
-             connection.set(  
-                     redisTemplate.getStringSerializer().serialize(  
- "user.uid." + user.getUid()),  
-                     redisTemplate.getStringSerializer().serialize(  
-                             user.getAddress()));  
- returnnull;  
-         }  
-     });  
- }  
 这里是通过模板类，实现方法回调。在spring框架下，可以方便的控制事务，如果你研究过spring的dao源代码，对此一定熟悉。![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
- 传入参数，需要**final**标识，禁止方法内修改。
- 调用**RedisConnection**的**set**方法实现**Redis**的**SET**命令。
- 不管是**Key**，还是**Value**都需要进行**Serialize**。
- 序列化操作，最好使用**RedisTemplate**提供的**Serializer**来完成。
这跟当年的**SpringJdbcTemplate**有那么一拼！
## 2.获取-GET
想要将对象从Redis中取出来，就麻烦一些，需要序列化key，最好判断下这个key是否存在，避免无用功。如果键值存在，需要对数据反序列化。
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- @Override
- public User read(final String uid) {  
- return redisTemplate.execute(new RedisCallback<User>() {  
- @Override
- public User doInRedis(RedisConnection connection)  
- throws DataAccessException {  
- byte[] key = redisTemplate.getStringSerializer().serialize(  
- "user.uid." + uid);  
- if (connection.exists(key)) {  
- byte[] value = connection.get(key);  
-                 String address = redisTemplate.getStringSerializer()  
-                         .deserialize(value);  
-                 User user = new User();  
-                 user.setAddress(address);  
-                 user.setUid(uid);  
- return user;  
-             }  
- returnnull;  
-         }  
-     });  
- }  
 当年写SpringJdbc的时候，就是这样一个字段一个字段拼装的，甭提多累人。好吧，用Spring-Data-Redis，又让我回归了！
- 记得使用泛型，如RedisCallback<User>()
- 使用同一的序列化/反序列化Serializer
- 建议使用connection.exists(key)判别键值是否存在，避免无用功
## 3.删除-DEL
删除，就简单点，不过也需要这样折腾一会：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- @Override
- publicvoid delete(final String uid) {  
-     redisTemplate.execute(new RedisCallback<Object>() {  
- public Object doInRedis(RedisConnection connection) {  
-             connection.del(redisTemplate.getStringSerializer().serialize(  
- "user.uid." + uid));  
- returnnull;  
-         }  
-     });  
- }  
做个**TestCase**，暂时够我用了！![](http://snowolf.iteye.com/images/smiles/icon_biggrin.gif)
## 4. TestCase
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- importstatic org.junit.Assert.*;  
- import org.junit.Before;  
- import org.junit.Test;  
- import org.springframework.context.ApplicationContext;  
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- import org.zlex.redis.dao.UserDao;  
- import org.zlex.redis.domain.User;  
- 
- publicclass UserDaoTest {  
- private ApplicationContext app;  
- private UserDao userDao;  
- 
- @Before
- publicvoid before() throws Exception {  
-         app = new ClassPathXmlApplicationContext("applicationContext.xml");  
-         userDao = (UserDao) app.getBean("userDao");  
-     }  
- 
- @Test
- publicvoid crud() {  
- // -------------- Create ---------------
-         String uid = "u123456";  
-         String address1 = "上海";  
-         User user = new User();  
-         user.setAddress(address1);  
-         user.setUid(uid);  
-         userDao.save(user);  
- 
- // ---------------Read ---------------
-         user = userDao.read(uid);  
- 
-         assertEquals(address1, user.getAddress());  
- 
- // --------------Update ------------
-         String address2 = "北京";  
-         user.setAddress(address2);  
-         userDao.save(user);  
- 
-         user = userDao.read(uid);  
- 
-         assertEquals(address2, user.getAddress());  
- 
- // --------------Delete ------------
-         userDao.delete(uid);  
-         user = userDao.read(uid);  
-         assertNull(user);  
-     }  
- }  
 貌似少了update，也许以后操作Hash时，会用上。![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
看看控制台获得了什么： 
redis 127.0.0.1:6379> get user.uid.u123456
(nil)
redis 127.0.0.1:6379> get user.uid.u123456
"\xe5\x8c\x97\xe4\xba\xac"
redis 127.0.0.1:6379> get user.uid.u123456
"\xe4\xb8\x8a\xe6\xb5\xb7"
redis 127.0.0.1:6379> del user.uid.u123456
(integer) 1
redis 127.0.0.1:6379> get user.uid.u123456
(nil)
redis 127.0.0.1:6379> get user.uid.u123456
"\xe4\xb8\x8a\xe6\xb5\xb7"
 好吧，可以开始用它来存点什么了！
