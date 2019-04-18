# Scala语言学习笔记（3） - zwvista - 博客园

## [Scala语言学习笔记（3）](https://www.cnblogs.com/zwvista/p/8059584.html)

### 类

```
// 定义并使用类
class User
val user1 = new User
// 主体构造器（primary constructor）
class Point(var x: Int, var y: Int) {
  def move(dx: Int, dy: Int): Unit = {
    x = x + dx
    y = y + dy
  }
  override def toString: String =
    s"($x, $y)"
}
val point1 = new Point(2, 3)
point1.x  // 2
println(point1)  // prints (2, 3)
// 带缺省参数的主体构造器
class Point(var x: Int = 0, var y: Int = 0)
val origin = new Point  // x and y are both set to 0
val point1 = new Point(1)
println(point1.x)  // prints 1
// 带名字的实参
val point2 = new Point(y=2)
println(point2.y)  // prints 2
// 类成员缺省为 public
// 使用关键字 private 定义 private 成员
class Point {
  private var _x = 0
  private var _y = 0
  private val bound = 100
  // getter 与 setter 语法
  def x = _x
  def x_= (newValue: Int): Unit = {
    if (newValue < bound) _x = newValue else printWarning
  }
  // getter 与 setter 语法
  def y = _y
  def y_= (newValue: Int): Unit = {
    if (newValue < bound) _y = newValue else printWarning
  }
 private def printWarning = println("WARNING: Out of bounds")
}
val point1 = new Point
point1.x = 99
point1.y = 101 // prints the warning
// 带 val 或 var 的参数为 public 属性
class Point(val x: Int, val y: Int)
val point = new Point(1, 2)
point.x = 3  // <-- does not compile
// 不带 val 或 var 的参数为 private 属性
class Point(x: Int, y: Int)
val point = new Point(1, 2)
point.x  // <-- does not compile
```

### 特质（traits）

特质不能实例化，不带参数。特质用于在类之间共享接口和成员。

```
// 特质
trait HairColor
// 泛型特质
trait Iterator[A] {
  def hasNext: Boolean
  def next(): A
}
// 使用类来扩展特质
class IntIterator(to: Int) extends Iterator[Int] {
  private var current = 0
  override def hasNext: Boolean = current < to
  override def next(): Int =  {
    if (hasNext) {
      val t = current
      current += 1
      t
    } else 0
  }
}
val iterator = new IntIterator(10)
println(iterator.next())  // prints 0
println(iterator.next())  // prints 1
// 将特质用作子类型
import scala.collection.mutable.ArrayBuffer
trait Pet {
  val name: String
}
class Cat(val name: String) extends Pet
class Dog(val name: String) extends Pet
val dog = new Dog("Harry")
val cat = new Cat("Sally")
val animals = ArrayBuffer.empty[Pet]
animals.append(dog)
animals.append(cat)
animals.foreach(pet => println(pet.name))  // Prints Harry Sally
```

### 混入（Mixins）

```
// 混入示例 1
abstract class A {
  val message: String
}
class B extends A {
  val message = "I'm an instance of class B"
}
trait C extends A {
  def loudMessage = message.toUpperCase()
}
class D extends B with C
// D 类有个超类 B 还有个混入 C
// B 和 C 具有相同的超类 A 
val d = new D
println(d.message)  // I'm an instance of class B
println(d.loudMessage)  // I'M AN INSTANCE OF CLASS B
// 混入示例 2
abstract class AbsIterator {
  type T
  def hasNext: Boolean
  def next(): T
}
trait RichIterator extends AbsIterator {
  def foreach(f: T => Unit): Unit = while (hasNext) f(next())
}
object StringIteratorTest extends App {
  // RichStringIter 类有个超类 AbsIterator 还有个混入 RichIterator
  class RichStringIter extends StringIterator("Scala") with RichIterator
  val richStringIter = new RichStringIter
  richStringIter foreach println
}
```

### 样例类

```
// 样例类创建实例不用 new
case class Book(isbn: String)
val frankenstein = Book("978-0486282114")
// 构造函数的参数缺省是 public val
case class Message(sender: String, recipient: String, body: String)
val message1 = Message("guillaume@quebec.ca", "jorge@catalonia.es", "Ça va ?")
println(message1.sender)  // prints guillaume@quebec.ca
message1.sender = "travis@washington.us"  // this line does not compile
// 样例类采用值比较
case class Message(sender: String, recipient: String, body: String)
val message2 = Message("jorge@catalonia.es", "guillaume@quebec.ca", "Com va?")
val message3 = Message("jorge@catalonia.es", "guillaume@quebec.ca", "Com va?")
val messagesAreTheSame = message2 == message3  // true
// 样例类的拷贝
case class Message(sender: String, recipient: String, body: String)
val message4 = Message("julien@bretagne.fr", "travis@washington.us", "Me zo o komz gant ma amezeg")
val message5 = message4.copy(sender = message4.recipient, recipient = "claire@bourgogne.fr")
message5.sender  // travis@washington.us
message5.recipient // claire@bourgogne.fr
message5.body  // "Me zo o komz gant ma amezeg"
```


