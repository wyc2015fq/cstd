# Scala 队列 - Simple 专栏 - CSDN博客
2018年11月19日 15:32:12[Simple_Zz](https://me.csdn.net/love284969214)阅读数：239
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
- 队列是一个**有序**列表，在底层可以用**数组**或是**链表**来实现
- 其输入和输出要遵循**先入先出**的原则。即：先存入队列的数据，要先取出。后存入的要后取出。
- 在Scala中，由设计者直接给我们提供队列类型使用。
- 在Scala中，有 scala.collection.mutable.Queue 和 scala.collection.immutable.Queue ,一般来说我们在开发中通常使用可变集合中的队列
- 在队列中严格遵守，如队列的数据，放在队位，出队列的数据从队列的头部取出。
```
import scala.collection.mutable
object QueueDemo {
  def main(args: Array[String]): Unit = {
    // 创建队列
    val q1 = new mutable.Queue[Any]
    println(q1)
    // 增加元素 (一个元素)
    q1 += 20
    println(q1)
    // 增加元素 (List内所有元素)
    q1 ++= List(2,4,6)
    println(q1)
    // 增加元素 (List)
    q1 += List(9,2)
    println(q1)
    // dequeue 从队列头部取出元素
    val queueException = q1.dequeue()
    println("queueException=" + queueException + "\tq1=" + q1)
    // enqueue 入队列，默认是从队列的尾部加入
    q1.enqueue(5,88,6,4,5,77)
    println(q1)
    // 取出队列的第一个元素
    println(q1.head)
    // 取出队列的最后一个元素
    println(q1.last)
    // 取出除了tail剩余元素(tail代表最开头的元素,可以递归)
    println(q1.tail.tail.tail)
  }
}
```
