# Scala Map 映射 - Simple 专栏 - CSDN博客
2018年11月19日 16:48:08[Simple_Zz](https://me.csdn.net/love284969214)阅读数：197
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
# Java中的 Map
- HashMap 是一个散列表 (数组 + 链表) ，它存储的内容是键值对 （key-value） 映射，Java中的HashMap是无序的。
```java
import java.util.HashMap;
public class TestJavaMap {
    public static void main(String[] args) {
        HashMap<String,Integer> hashMap = new HashMap<>();
        hashMap.put("no1",100);
        hashMap.put("no2",200);
        hashMap.put("no3",300);
        hashMap.put("no4",400);
        System.out.println(hashMap);
        System.out.println(hashMap.get("no2"));
    }
}
```
# Scala中的 Map
- Scala中的 Map 和 Java 类似，也是一个散列表，它存储的内容也是键值对 (kru-value) 映射，Scala中不可变的 Map 是有序的，可变的Map 是无序的。
- Scala中，有可变 Map (scala.collection.mutable.Map) 和不可变 Map (scala.collection.immutable.Map)
### 映射 Map 创建
- 默认是immutable.Map
- key-value 支持Any
- 在Map的底层，每对key-valye 是 Tuple2 (元组2)
- 从输出的结果看到，输出顺序和声明顺序一致
```
import scala.collection.mutable
object TestScalaMap {
  def main(args: Array[String]): Unit = {
    // 不可变Map 映射
    val map1 = Map("A" -> 10, "B" -> 20, "C" -> 80)
    println(map1)
    // 可变Map 映射
    val map2 = mutable.Map("A" -> 10, "B" -> 20, "C" -> 80)
    println(map2)
    // 空Map 映射
    val map3 = new mutable.HashMap[String, Int]
    println(map3)
    // 通过元组创建Map
    val map4 = mutable.Map(("A" -> 10), ("B" -> 20), ("C" -> 80))
    println(map4)
  }
}
```
### 映射 Map 取值
- 如果key存在，则返回对应的值
- 如果key不存在，则抛出异常
- 在Java中,如果key不存在则返回null
```
// 方式1 使用map(key）
// 有返回
// 没有抛出异常 NoSuchElementException: key not found: A~
println(map4("A"))
println(map4("A~"))
// 方式2 判断
if (map4.contains("A~")) {
  println("key 存在，值= " +map4("A~"))
} else {
  println("key不存在 :)")
}
// 方式3 map.get(key).get
// 如果存在 返回Some()  Some().get 返回值
// 如果不存在返回None 抛出异常 NoSuchElementException: None.get
println(map4.get("A~").get)
// 方式4 使用map.getOrElse()
// 如果有返回
// 如果没有返回默认值
println(map4.getOrElse("A","默认值"))
```
### 如何选择取值方式建议
- 如果定义map有这个key，则应当使用map(key)，速度快
- 如果不能确定map是否有key，而且有不同的业务逻辑，使用map.contains() 先判断在加入逻辑
- 如果只是简单的希望得到一个值，使用map.getOrElse("ip","默认值")
### 映射 Map 增删改查
```
val map5 = mutable.Map(("A", "北京"), ("B", "广州"), ("C", "上海"), ("D", "深圳"))
map5("E") = "南京"
map5("E") = "武汉"
println(map5)
map5 += ("F" -> "南京")
map5 += ("G" -> "沈阳")
println(map5)
map5 += ("EE" -> "天津", "FF" -> "西安")
println(map5)
map5 -= ("F","G","X")
println(map5)
```
### 映射 Map 遍历
```
val map6 = mutable.Map(("A", 1), ("B", "北京"), ("C", 3))
for ((k, v) <- map6) print(k + ":" + v + "\t")
println()
for (v <- map6.keys) print(v + "\t")
println()
for (v <- map6.values) print(v + "\t")
println()
for (v <- map6) print(v + "\t") //v是Tuple2
println()
```
