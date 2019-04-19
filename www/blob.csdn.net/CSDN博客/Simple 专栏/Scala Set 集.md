# Scala Set 集 - Simple 专栏 - CSDN博客
2018年11月19日 17:03:33[Simple_Zz](https://me.csdn.net/love284969214)阅读数：32标签：[Scala																[编程基础](https://so.csdn.net/so/search/s.do?q=编程基础&t=blog)](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
集是不重复元素的结合。集不保留顺序，默认是以哈希集实现
### Java 中 Set
java中，HashSet 是实现 Set<E> 接口的一个实体类，数据是以哈希表的形式存放的，里面的不能重复数据。Set 接口是一种一个不包含重复元素的 collection， HashSet 中的数据也是没有顺序的。
```java
import java.util.HashSet;
public class TestJavaSet {
    public static void main(String[] args) {
        HashSet hashSet = new HashSet();
        hashSet.add("jack");
        hashSet.add("tom");
        hashSet.add("lucy");
        hashSet.add("mary");
        System.out.println(hashSet);
    }
}
```
### Scala 中 Set
**默认**情况下，Scala 使用的是**不可变集合**，如果你想使用可变集合，需要引用 **scala.collection.mutable**.Set包
```
import scala.collection.mutable
object TestScalaSet {
  def main(args: Array[String]): Unit = {
    // 不可变集合
    val set01 = Set(1, 2, 4, "abc")
    println(set01)
    // 可变集合
    val set02 = mutable.Set(1, 2, 4, "abc")
    println(set02)
    // 添加 删除
    // 如果添加的对象已经存在，则不会重复添加，也不会报错
    // 如果删除的对象不存在，则不生效，也不会报错
    set02.add(4) //方式1
    set02 += 6 //方式2
    set02.+(5) //方式3
    println(set02)
    set02 -= 2
    set02.remove("abc")
    println(set02)
    // 遍历
    for (x <- set02) {
      print(x + "\t")
    }
    println()
  }
}
```
