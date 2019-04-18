# Redis笔记（一）纯小白版篇 - z69183787的专栏 - CSDN博客
2015年03月25日 15:12:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：707
个人分类：[Redis-基础](https://blog.csdn.net/z69183787/article/category/2923783)

      这是每个人学习Redis使用的第一步最常见的做法：
第一：下载
         Redis下载：从官网[http://redis.io/](http://redis.io/)下载最新的redis,我下载的是
[http://redis.googlecode.com/files/redis-2.6.14.tar.gz](http://redis.googlecode.com/files/redis-2.6.14.tar.gz)
jedis下载：从[https://github.com/xetorthio/jedis/downloads下载jedis2.1.0](https://github.com/xetorthio/jedis/downloads%E4%B8%8B%E8%BD%BDjedis2.1.0) ，路径为：
https://github.com/downloads/xetorthio/jedis/jedis-2.1.0.jar
第二：解压安装
解压tar -zxvfredis-2.6.14.tar.gz
然后进入src文件夹，make 然后makeinstall,详见README文件
第三：测试
         编写测试文件
package test.redis;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisShardInfo;
public class TestRedisConnect {
    private static Jedis jedis;
    static {
        jedis = new Jedis("127.0.0.1");
        jedis.connect();    
    }
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        jedis.incr("testAge");
        System.out.println(jedis.get("testAge"));
    }
}
目录结构：
.
|-- libs
|   `-- jedis-2.1.0.jar
`-- test
    `-- redis
        |-- TestRedisConnect.class
        `-- TestRedisConnect.java
3 directories, 3 files
启动redis服务：redis-server redis.conf
编译：
javac -Djava.ext.dirs=libstest/redis/TestRedisConnect.java
执行：
java -Djava.ext.dirs=libstest.redis.TestRedisConnect
如果输入了1，则测试成功，下一篇讲redis的主从配置。
