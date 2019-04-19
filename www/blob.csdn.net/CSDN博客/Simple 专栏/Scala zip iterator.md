# Scala zip iterator - Simple 专栏 - CSDN博客
2018年12月28日 11:34:03[Simple_Zz](https://me.csdn.net/love284969214)阅读数：22标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
# Zip 拉链(合并)
在开发中，当我们需要将两个集合进行 **对偶元组****合并**，可以使用拉链。
```python
// 拉链
val list1 = List(1, 2 ,3)
val list2 = List(4, 5, 6)
val list3 = list1.zip(list2) //((1,4), (2,5),(3,6))
println("list3=" + list3)
```
注意事项：
- 拉链的本质就是两个集合的合并操作，合并后**每个元素**是一个 **对偶元组**。
- 如果两个集合个数不对应，会造成数据丢失。
- 集合不限于List, 也可以是其它集合比如 Array。
- 如果要取出合并后的各个对偶元组的数据，可以遍历。
# iterator 迭代器
通过iterator方法从集合获得一个迭代器，通过while循环和for表达式对集合进行遍历。
```python
val iterator = List(1, 2, 3, 4, 5).iterator //
println("-------- 遍历方式 while --------")
while (iterator.hasNext) {
    println(iterator.next())
}
println("-------- 遍历方式 for --------")
for(enum <- iterator) {
    println(enum) //
}
```
iterator 的构建实际是 **AbstractIterator****的一个匿名子类**，该子类提供了。
2)该AbstractIterator子类提供了  **hasNext**** next **等方法。
3)因此，我们可以使用 while的方式，使用hasNext next 方法变量。
```python
// 源码
override /*IterableLike*/
def iterator: Iterator[A] = new AbstractIterator[A] {
  var these = self
  def hasNext: Boolean = !these.isEmpty
  def next(): A =
    if (hasNext) {
      val result = these.head; these = these.tail; result
    } else Iterator.empty.next()
  override def toList: List[A] = {
    val xs = these.toList
    these = these.take(0)
    xs
  }
}
```
