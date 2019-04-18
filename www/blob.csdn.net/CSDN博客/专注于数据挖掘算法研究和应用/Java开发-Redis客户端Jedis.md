# Java开发-Redis客户端Jedis - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月05日 10:04:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2697








1、部署Redis后启动服务端；

2、Jedis网址：https://github.com/xetorthio/jedis/tree/2.6

      应用maven管理jar包依赖：新建maven项目（eclipse集成maven管理项目和jar包依赖，请看apach maven官网介绍），在pom.xml增加如下依赖，执行maven install，在maven的\.m2\repository找到Jedis-2.6.0.jar包


<dependency>
    <groupId>redis.clients</groupId>
    <artifactId>jedis</artifactId>
    <version>2.6.0</version>
    <type>jar</type>
    <scope>compile</scope>
</dependency>
3、引入jedis的jar包到工程启动redis客户端开发




Jedis jedis =newJedis("localhost");
jedis.set("foo", "bar");
String value = jedis.get("foo");
4、 redis有很多数据存储结构应用java已有数据类型可以直接存储 ，如List、Map等。





```java
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import redis.clients.jedis.Jedis;

public class JedisUtil {
	
	 public static Jedis createJedis() {
	        Jedis jedis = new Jedis("127.0.0.1");
	        return jedis;
	 }

	 public static Jedis createJedis(String host, int port) {
	        Jedis jedis = new Jedis(host, port);
	        return jedis;
	 }

	 public static Jedis createJedis(String host, int port, String password) {
	        Jedis jedis = new Jedis(host, port);
	        if (!password.isEmpty())
	            jedis.auth(password);       
	        return jedis;
	 }
	    
	 public static void main(String[] args) throws InterruptedException{
		 
		 Jedis jd=createJedis("127.0.0.1",6379);//connect redis server
		 /*
		  * string 
		  */
		 jd.set("id","01");//<key,value> 
	     System.out.println(jd.get("id")); 
	       
	     jd.append("id","02"); //append  
	     System.out.println(jd.get("id"));  
	  	        
	     jd.set("id","001");  //update
	     System.out.println(jd.get("id"));  
	  
	     jd.del("id");  //delete
	     System.out.println(jd.get("id"));  
	  
	     jd.mset("id","01","name","Jason.F");  //more <key,value>
	     System.out.println(jd.mget("id","name")); 
	     
	     /*
	      * Map
	      */
	     Map<String,String> user=new HashMap<String,String>();  
	     user.put("id","01");  //add
	     user.put("name","Jason.F");  
	     jd.hmset("user",user);  
	     
	     List<String> rsmap = jd.hmget("user", "id");  
	     System.out.println(rsmap);   
	     System.out.println(jd.hlen("user")); //存储个数   
	     System.out.println(jd.hkeys("user"));//返回map对象中的所有key 
	     System.out.println(jd.hvals("user"));//返回map对象中的所有value 
	  
	     Iterator<String> iter=jd.hkeys("user").iterator();  
	     while (iter.hasNext()){ 
	    	 String key = iter.next(); 
	    	 System.out.println(key+":"+jd.hmget("user",key));  
	     }  
	     
	     /*
	      * List
	      */
	     jd.del("sort");  
	     System.out.println(jd.lrange("sort",0,-1));  
	     jd.lpush("sort","1");  //add
	     jd.lpush("sort","3");  
	     jd.lpush("sort","2");  
	     // 第一个是key，第二个是起始位置，第三个是结束位置，jedis.llen获取长度 -1表示取得所有  
	     System.out.println(jd.lrange("sort",0,-1));  
	     System.out.println(jd.sort("sort"));
	     System.out.println(jd.lrange("sort",0,-1)); 
	     
	     /*
	      * set
	      */
	     jd.sadd("sname", "zhang");  
	     jd.sadd("sname", "wang");  
	     jd.sadd("sname", "li");  
	     jd.sadd("sname", "noname");  
	     jd.srem("sname", "noname");// 移除noname   
	     System.out.println(jd.smembers("sname"));// 获取所有加入的value  
	     System.out.println(jd.sismember("sname", "zhang"));//判断 zhang是否是sname集合的元素  
	     System.out.println(jd.srandmember("sname"));  
	     System.out.println(jd.scard("sname"));// 返回集合的元素个数  
	 }
}
```


服务端执行日志：

[8524] 05 Feb 10:41:21.566 # Server started, Redis version 2.6.12

[8524] 05 Feb 10:41:21.570 * The server is now ready to accept connections on

rt 6379

[8524] 05 Feb 10:56:22.075 * 1 changes in 900 seconds. Saving...

[8524] 05 Feb 10:56:22.075 * cowBkgdSaveReset deleting 0 SDS and 0 obj items

[8524] 05 Feb 10:56:22.116 * DB saved on disk

[8524] 05 Feb 10:56:22.176 * Background saving terminated with success

[8524] 05 Feb 10:56:22.176 * cowBkgdSaveReset deleting 0 SDS and 0 obj items

[8524] 05 Feb 11:04:11.405 * 10 changes in 300 seconds. Saving...

[8524] 05 Feb 11:04:11.405 * cowBkgdSaveReset deleting 0 SDS and 0 obj items

[8524] 05 Feb 11:04:11.438 * DB saved on disk

[8524] 05 Feb 11:04:11.505 * Background saving terminated with success

[8524] 05 Feb 11:04:11.505 * cowBkgdSaveReset deleting 0 SDS and 0 obj items





