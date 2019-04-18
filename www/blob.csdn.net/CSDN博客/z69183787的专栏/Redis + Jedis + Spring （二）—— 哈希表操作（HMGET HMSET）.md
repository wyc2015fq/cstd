# Redis + Jedis + Spring （二）—— 哈希表操作（HMGET HMSET） - z69183787的专栏 - CSDN博客
2017年07月03日 17:37:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：317
个人分类：[Spring-RedisTemplate](https://blog.csdn.net/z69183787/article/category/7004300)
[http://snowolf.iteye.com/blog/1667104](http://snowolf.iteye.com/blog/1667104)
# 一、预期
接上一篇，扩充User属性：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- publicclass User implements Serializable {  
- privatestaticfinallong serialVersionUID = -1267719235225203410L;  
- 
- private String uid;  
- 
- private String address;  
- 
- private String mobile;  
- 
- private String postCode;  
- }  
 我期望的是： 
redis 127.0.0.1:6379> hmget uc.user.info.uid.u123456 address mobile postCode
1) "\xe4\xb8\x8a\xe6\xb5\xb7"
2) "13800138000"
3) "100859"
几乎就是一个对象了！![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
但是，接下来的代码实现，让我彻底崩溃了！![](http://snowolf.iteye.com/images/smiles/icon_cry.gif)
# 二、代码实现
## 1.保存——HMSET
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- @Override
- publicvoid save(final User user) {  
-     redisTemplate.execute(new RedisCallback<Object>() {  
- @Override
- public Object doInRedis(RedisConnection connection)  
- throws DataAccessException {  
- byte[] key = redisTemplate.getStringSerializer().serialize(  
- "uc.user.info.uid." + user.getUid());  
-             BoundHashOperations<Serializable, byte[], byte[]> boundHashOperations = redisTemplate  
-                     .boundHashOps(key);  
-             boundHashOperations.put(redisTemplate.getStringSerializer()  
-                     .serialize("mobile"), redisTemplate  
-                     .getStringSerializer().serialize(user.getMobile()));  
-             boundHashOperations.put(redisTemplate.getStringSerializer()  
-                     .serialize("address"), redisTemplate  
-                     .getStringSerializer().serialize(user.getAddress()));  
-             boundHashOperations.put(redisTemplate.getStringSerializer()  
-                     .serialize("postCode"), redisTemplate  
-                     .getStringSerializer().serialize(user.getPostCode()));  
-             connection.hMSet(key, boundHashOperations.entries());  
- returnnull;  
-         }  
-     });  
- }  
这里用到：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- BoundHashOperations<Serializable, byte[], byte[]> boundHashOperations = redisTemplate.boundHashOps(key);  
- boundHashOperations.put(redisTemplate.getStringSerializer().serialize("mobile"), redisTemplate.getStringSerializer().serialize(user.getMobile()));  
 看着就有点肿。。。Map封装完以后，用HMSET命令：
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- connection.hMSet(key, boundHashOperations.entries());  
 这时候就完成了哈希表的保存操作，可以在控制台看到相应的数据了。
redis 127.0.0.1:6379> hmget uc.user.info.uid.u123456 address mobile postCode
1) "\xe4\xb8\x8a\xe6\xb5\xb7"
2) "13800138000"
3) "100859"
## 2.获取——HMGET
这一刻，我彻底崩溃了！取出来的值是个List，还得根据取得顺序，逐个反序列化，得到内容。
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- @Override
- public User read(final String uid) {  
- return redisTemplate.execute(new RedisCallback<User>() {  
- @Override
- public User doInRedis(RedisConnection connection)  
- throws DataAccessException {  
- byte[] key = redisTemplate.getStringSerializer().serialize(  
- "uc.user.info.uid." + uid);  
- if (connection.exists(key)) {  
-                 List<byte[]> value = connection.hMGet(  
-                         key,  
-                         redisTemplate.getStringSerializer().serialize(  
- "address"),  
-                         redisTemplate.getStringSerializer().serialize(  
- "mobile"), redisTemplate  
-                                 .getStringSerializer()  
-                                 .serialize("postCode"));  
-                 User user = new User();  
-                 String address = redisTemplate.getStringSerializer()  
-                         .deserialize(value.get(0));  
-                 user.setAddress(address);  
-                 String mobile = redisTemplate.getStringSerializer()  
-                         .deserialize(value.get(1));  
-                 user.setMobile(mobile);  
-                 String postCode = redisTemplate.getStringSerializer()  
-                         .deserialize(value.get(2));  
-                 user.setPostCode(postCode);  
-                 user.setUid(uid);  
- 
- return user;  
-             }  
- returnnull;  
-         }  
-     });  
- }  
 这个实现，跟Redis的命令几乎一模一样，指定Key，指定field，获取其值。
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- List<byte[]> value = connection.hMGet(key,redisTemplate.getStringSerializer().serialize("address"),  
- redisTemplate.getStringSerializer().serialize("mobile"),   
- redisTemplate.getStringSerializer().serialize("postCode"));  
 我绝对相信，要么是我用的过于肤浅，低估了Spring的封装能力。或者，我该直接Json！![](http://snowolf.iteye.com/images/smiles/icon_sad.gif)等等，这不是MongoDB干的事情吗？！
PS：这两篇博客里操作的数据类型，只能是String类型，还没搞定除此以外任何类型。吾将上下而求索～～～
上述操作也许你吐了，接下来的代码，就再吐一次吧！
封装对象的时候，一定要记得次序。。。。这绝对不是一个优质代码的实现风格！
Java代码  ![收藏代码](http://snowolf.iteye.com/images/icon_star.png)
- User user = new User();  
- String address = redisTemplate.getStringSerializer().deserialize(value.get(0));  
- user.setAddress(address);  
- String mobile = redisTemplate.getStringSerializer().deserialize(value.get(1));  
- user.setMobile(mobile);  
- String postCode = redisTemplate.getStringSerializer().deserialize(value.get(2));  
- user.setPostCode(postCode);  
 好吧！苦逼的事情，就此结束。目标Json支持！![](http://snowolf.iteye.com/images/smiles/icon_smile.gif)
