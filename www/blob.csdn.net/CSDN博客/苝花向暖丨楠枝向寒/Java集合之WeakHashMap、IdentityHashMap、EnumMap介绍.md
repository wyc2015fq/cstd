# Java集合之WeakHashMap、IdentityHashMap、EnumMap介绍 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年06月11日 20:47:26[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：44标签：[WeakHashMap																[IdentityHashMap																[EnumMap](https://so.csdn.net/so/search/s.do?q=EnumMap&t=blog)](https://so.csdn.net/so/search/s.do?q=IdentityHashMap&t=blog)](https://so.csdn.net/so/search/s.do?q=WeakHashMap&t=blog)
个人分类：[Map 集合](https://blog.csdn.net/weixin_40247263/article/category/7414678)


## 转自：[点击打开链接](https://blog.csdn.net/wxc880924/article/details/52683097)

## WeakHashMap

WeakHashMap与HashMap的用法基本类似。

区别：
- HashMap的key保留了对实际对象的强引用，这意味着只要HashMap对象不被销毁，还HashMap的所有key所引用的对象就不会被垃圾回收，HashMap也不会自动删除这些key所对应的key-value对；
- WeakHashMap的key只保留对实际对象的弱引用，这意味着如果WeakHashMap对象的key所引用的对象没有被其他强引用变量所引用，则这些key所引用的对象可能被垃圾回收，WeakHashMap也可能自动删除这些key所对应的key-value对。
- WeakHashMap中的每个key对象只持有对实际对象的弱引用，因此，当垃圾回收了该key所对应的实际对象之后，WeakHashMap会自动删除该key对应的key-value对。

注意：

如果需要使用WeakHashMap的key来保留对象的弱引用，则不要让该key所引用的对象具有任何强引用，否则将失去WeakHashMap的意义。

示例：

```
package com.map;

import java.util.WeakHashMap;

public class WeakHashMapTest {
public static void main(String[] args) {
    WeakHashMap wak = new WeakHashMap();
    //两个key都是匿名字符串对象（没有其他引用）
    wak.put(new String("数学"), new String("优良"));
    wak.put(new String("语文"), new String("良好"));

    //该key是一个系统缓存的字符串对象
    wak.put("java", new String("好"));①
    System.out.println(wak);
    //{java=良好, 数学=优良, 语文=良好}

    //通知系统进行垃圾回收
    System.gc();
    System.runFinalization();
    System.out.println(wak);//{java=好}

}
}
```
- 

结果上来看：当系统进行垃圾时，删除了WeakHashMap 对象的前两个key-value对，因为添加前两个key-value对时，这两个key都是匿名的字符串对象，WeakHashMap 只保留了对它们的弱引用，这样垃圾回收时会自动删除这两个key-value对。

WeakHashMap对象中①标示处的key是一个字符串直接量（系统会自动保留对该字符串对象的强引用），所以垃圾回收时不会回收它。

## IdentityHashMap

在IdentityHashMap中，当且仅当两个key严格相等（key1==key2）时，IdentityHashMap才认为两个key相等；相对于普通HashMap而言，只要key1和key2通过equals()方法返回true，且它们的hashCode值相等即可。

注意：

IdentityHashMap是一个特殊的Map实现！此类实现Map接口时，它有意违反Map的通常规范：IdentityHashMap要求两个key严格相等才认为两个key相等。IdentityHashMap不保证key-value对之间的顺序，更不能保证它们的顺序随时间的推移保持不变。

示例：

```
package com.map;

import java.util.IdentityHashMap;

public class IdentityHashMapTest {

    public static void main(String[] args) {
        IdentityHashMap idenmap = new IdentityHashMap();
        idenmap.put(new String("语文"), 80);
        idenmap.put(new String("语文"), 89);

        idenmap.put("java", 80);
        idenmap.put("java", 80);
        System.out.println(idenmap);
        //{语文=80, java=80, 语文=89}
    }
}
```
- 

IdentityHashMap对象中添加了4个key-value对，前2个key-value对中的key是最新创建的字符串对象，它们通过==比较不相等，所以IdentityHashMap 会把他们当成2个key来处理；后2个key-value都是字符串直接量，而且它们的字符序列完全相同，Java使用常量池来管理字符串直接量，所以它们通过==比较返回true，IdentityHashMap 会认为它们是同一个key，因此只有一次可以添加成功。

## EnumMap

EnumMap是一个与枚举类一起使用的Map实现，EnumMap中的所有key都必须是单个枚举类的枚举值。创建EnumMap时必须显示或隐式的指定它对应的枚举类。

EnumMap特征：
- EnumMap在内部以数组形式保存，所以这种实现形式非常紧凑、高效。
- EunmMap根据key的自然顺序（即枚举值在枚举类中的定义顺序）来维护key-value对的顺序。
- EnumMap不允许使用null作为key，但允许使用null作为value。如果试图使用null作为key时将抛出NullpointerException。

如果只是查询是否包含值为null的key，或只是删除值为null的key，都不会抛出异常。

与普通的Map有所区别的是，创建EnumMap是必须指定一个枚举类，从而将该EnumMap和指定枚举类关联起来。

示例：

```
package com.map;

import java.util.EnumMap;

public class EnumMapTest {

    public static void main(String[] args) {

        EnumMap map = new EnumMap(Season.class);
        map.put(Season.SPRING, "春天");
        map.put(Season.SUMMER, "夏天");
        System.out.println(map);
        //{SPRING=春天, SUMMER=夏天}
    }
}

enum Season{
    SPRING,SUMMER,FAIL,WINTER
}
```

