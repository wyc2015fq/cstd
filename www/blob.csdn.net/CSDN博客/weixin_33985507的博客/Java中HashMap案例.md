# Java中HashMap案例 - weixin_33985507的博客 - CSDN博客
2016年11月27日 22:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
```
package ch8;
import java.util.*;
/**
 * Created by Jiqing on 2016/11/27.
 */
public class MapTest {
    public static void main(String[] args) {
        // Map不同于List，它的key不一定是数字
        Map map = new HashMap();
        map.put("疯狂Java讲义",109);
        map.put("疯狂iOS讲义",10);
        map.put("疯狂Ajax讲义",79);
        // 如果新的value覆盖了原有的value，该方法返回被覆盖的value
        System.out.println(map.put("疯狂iOS讲义",99)); // 输出10
        System.out.println(map);
        // 判断是否包含指定key
        System.out.println("是否包含值为疯狂iOS讲义key："+map.containsKey("疯狂iOS讲义"));
        System.out.println("是否包含值为99 value："+map.containsValue(99));
        // 遍历map
        for (Object key : map.keySet()) {
            System.out.println(key+"-->"+map.get(key));
        }
    }
}
```
输出结果：
> 
10
{疯狂Ajax讲义=79, 疯狂iOS讲义=99, 疯狂Java讲义=109}
是否包含值为疯狂iOS讲义key：true
是否包含值为99 value：true
疯狂Ajax讲义-->79
疯狂iOS讲义-->99
疯狂Java讲义-->109
