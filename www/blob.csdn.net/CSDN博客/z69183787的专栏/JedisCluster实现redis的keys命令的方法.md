# JedisCluster实现redis的keys命令的方法 - z69183787的专栏 - CSDN博客
2016年07月03日 19:24:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6466
由于JedisCluster没有提供对keys命令的封装，只能自己实现：
先定义接口。使用TreeSet返回，是为了可以方便地利用它的first()方法：
**[java]**[view
 plain](http://blog.csdn.net/clementad/article/details/50447930#)[copy](http://blog.csdn.net/clementad/article/details/50447930#)
- publicinterface IRedisOperator {  
- 
- /**
-      * 根据pattern 获取所有的keys
-      * @param pattern
-      * @return
-      */
-     TreeSet<String> keys(String pattern);  
- }  
实现类：
**[java]**[view
 plain](http://blog.csdn.net/clementad/article/details/50447930#)[copy](http://blog.csdn.net/clementad/article/details/50447930#)
- publicclass RedisOperator implements IRedisOperator {  
- 
- publicfinalstatic Logger logger = LoggerFactory.getLogger(RedisOperator.class);  
- 
- @Autowired
- private JedisCluster jedisCluster;  
- 
- @Override
- public TreeSet<String> keys(String pattern){  
-         logger.debug("Start getting keys...");  
-         TreeSet<String> keys = new TreeSet<>();  
-         Map<String, JedisPool> clusterNodes = jedisCluster.getClusterNodes();  
- for(String k : clusterNodes.keySet()){  
-             logger.debug("Getting keys from: {}", k);  
-             JedisPool jp = clusterNodes.get(k);  
-             Jedis connection = jp.getResource();  
- try {  
-                 keys.addAll(connection.keys(pattern));  
-             } catch(Exception e){  
-                 logger.error("Getting keys error: {}", e);  
-             } finally{  
-                 logger.debug("Connection closed.");  
-                 connection.close();//用完一定要close这个链接！！！
-             }  
-         }  
-         logger.debug("Keys gotten!");  
- return keys;  
-     }  
- }  
调用：
**[java]**[view
 plain](http://blog.csdn.net/clementad/article/details/50447930#)[copy](http://blog.csdn.net/clementad/article/details/50447930#)
- TreeSet<String> keys = redisOperator.keys(prefix);  
- Map<String, Object> data = new LinkedHashMap<>();  
- data.put("prefix", prefix);  
- data.put("count", keys.size());  
- String type = "unknown";  
- if(keys.size()>0){  
-     type = redisOperator.type(keys.first());  
- }  
- data.put("type", type);  
- 
