# java计算对象大小maven包 - weixin_33985507的博客 - CSDN博客
2019年03月04日 18:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
```
<!-- https://mvnrepository.com/artifact/net.sf.ehcache/ehcache -->
<dependency>
    <groupId>net.sf.ehcache</groupId>
    <artifactId>ehcache</artifactId>
    <version>2.10.6</version>
</dependency>
import net.sf.ehcache.pool.sizeof.AgentSizeOf;
import net.sf.ehcache.pool.sizeof.SizeOf;
public class Father {
    public static void main(String[] args) {
        SizeOf sizeOf = new AgentSizeOf();
        Father father = new Father();
        System.out.println("father's size: " + sizeOf.sizeOf(father));
    }
}
```
