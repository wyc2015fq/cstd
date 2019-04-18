# redis基础简介（六）- jedis使用管道（pipeline）对redis进行读写（使用hmset、hgetall测试） - z69183787的专栏 - CSDN博客
2017年09月21日 16:58:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2453
[http://blog.csdn.net/kinglyjn/article/details/54015710](http://blog.csdn.net/kinglyjn/article/details/54015710)
一般情况下，Redis Client端发出一个请求后，通常会阻塞并等待Redis服务端处理，Redis服务端处理完后请求命令后会将结果通过响应报文返回给Client。这有点类似于HBase的Scan，通常是Client端获取每一条记录都是一次RPC调用服务端。在Redis中，有没有类似HBase Scanner Caching的东西呢，一次请求，返回多条记录呢？有，这就是Pipline。官方介绍 [http://redis.io/topics/pipelining](http://redis.io/topics/pipelining)。
通过pipeline方式当有大批量的操作时候，我们可以节省很多原来浪费在网络延迟的时间，需要注意到是用pipeline方式打包命令发送，redis必须在处理完所有命令前先缓存起所有命令的处理结果。打包的命令越多，缓存消耗内存也越多。所以并不是打包的命令越多越好。
![](https://img-blog.csdn.net/20170104091142155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2luZ2x5am4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用Pipeline在对Redis批量读写的时候，性能上有非常大的提升。
使用Java测试了一下：
```java
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.Pipeline;
import redis.clients.jedis.Response;
public class Test {
    public static void main(String[] args) throws Exception {
        Jedis redis = new Jedis("127.0.0.1", 6379, 400000);
        Map<String, String> data = new HashMap<String, String>();
        redis.select(8);
        redis.flushDB();
        // hmset
        long start = System.currentTimeMillis();
        // 直接hmset
        for (int i = 0; i < 10000; i++) {
            data.clear();
            data.put("k_" + i, "v_" + i);
            redis.hmset("key_" + i, data);
        }
        long end = System.currentTimeMillis();
        System.out.println("dbsize:[" + redis.dbSize() + "] .. ");
        System.out.println("hmset without pipeline used [" + (end-start)/1000 + "] seconds ..");
        redis.select(8);
        redis.flushDB();
        // 使用pipeline hmset
        Pipeline p = redis.pipelined();
        start = System.currentTimeMillis();
        for (int i = 0; i < 10000; i++) {
            data.clear();
            data.put("k_" + i, "v_" + i);
            p.hmset("key_" + i, data);
        }
        p.sync();
        end = System.currentTimeMillis();
        System.out.println("dbsize:[" + redis.dbSize() + "] .. ");
        System.out.println("hmset with pipeline used [" + (end-start)/1000 + "] seconds ..");
        // hmget
        Set keys = redis.keys("*");
        // 直接使用Jedis hgetall
        start = System.currentTimeMillis();
        Map<String, Map<String, String>> result = new HashMap<String, Map<String, String>>();
        for (String key : keys) {
            result.put(key, redis.hgetAll(key));
        }
        end = System.currentTimeMillis();
        System.out.println("result size:[" + result.size() + "] ..");
        System.out.println("hgetAll without pipeline used [" + (end-start)/1000 + "] seconds ..");
        // 使用pipeline hgetall
        Map<String, Response<Map<String, String>>> responses = 
                new HashMap<String, Response<Map<String, String>>>(
                keys.size());
        result.clear();
        start = System.currentTimeMillis();
        for (String key : keys) {
            responses.put(key, p.hgetAll(key));
        }
        p.sync();
        for (String k : responses.keySet()) {
            result.put(k, responses.get(k).get());
        }
        end = System.currentTimeMillis();
        System.out.println("result size:[" + result.size() + "] ..");
        System.out.println("hgetAll with pipeline used [" + (end-start)/1000 + "] seconds ..");
        redis.disconnect();
    }
}
//测试结果：
//使用pipeline来批量读写10000条记录，就是小菜一碟，秒完。
dbsize:[10000] ..
hmset without pipeline used [243] seconds ..
dbsize:[10000] ..
hmset with pipeline used [0] seconds ..
result size:[10000] ..
hgetAll without pipeline used [243] seconds ..
result size:[10000] ..
hgetAll with pipeline used [0] seconds ..
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
![](http://static.blog.csdn.net/images/save_snippets.png)
[](http://blog.csdn.net/kinglyjn/article/details/54015710#)[](http://blog.csdn.net/kinglyjn/article/details/54015710#)[](http://blog.csdn.net/kinglyjn/article/details/54015710#)[](http://blog.csdn.net/kinglyjn/article/details/54015710#)[](http://blog.csdn.net/kinglyjn/article/details/54015710#)[](http://blog.csdn.net/kinglyjn/article/details/54015710#)
