# Scala 对象创建流程分析 - Simple 专栏 - CSDN博客
2018年11月14日 11:43:58[Simple_Zz](https://me.csdn.net/love284969214)阅读数：82
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
```python
object ObjectCreate {
  def main(args: Array[String]): Unit = {
    var p: Person = new Person("jack", 20)
  }
}
class Person {
  var age: Short = 90
  var name: String = _
  def this(n: String, a: Short) {
    this()
    this.name = n
    this.age = a
  }
}
```
- 加载类的信息 （属性信息，方法信息）
- 在内存中 （堆） 开辟空间
- 使用父类的构造器 （主和辅助） 进行初始
- 使用主构造器对属性进行初始化  【age:90,name null】
- 使用辅助构造器对属性进行初始化【age:20,name jack】
- 将开辟的对象地址赋给 p 这个引用
