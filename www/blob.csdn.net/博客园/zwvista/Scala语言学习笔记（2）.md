# Scala语言学习笔记（2） - zwvista - 博客园

## [Scala语言学习笔记（2）](https://www.cnblogs.com/zwvista/p/8040425.html)

### 表达式，值，变量，代码块，函数，方法

```
// 表达式
1 + 1
println(1 + 1) // 2
// 值（values）使用 val 关键字声明，带初值时类型可省略。
val x = 1 + 1
println(x) // 2
val x: Int = 1 + 1
// x = 3
// 变量（variables）使用 var 关键字声明，带初值时类型可省略。
var x = 1 + 1
var x: Int = 1 + 1
x = 3
// 代码块（blocks）: 使用大括号括起来，代码块的值由最后一句表达式语句的值来决定。
println({
  val x = 1 + 1
  x + 1
}) // 3
// 函数（functions）: 参数类型不可省略，返回值的类型可省略。
(x: Int) => x + 1
val addOne = (x: Int) => x + 1
println(addOne(1)) // 2
// 函数可以有多个参数
val add = (x: Int, y: Int) => x + y
println(add(1, 2)) // 3
// 函数也可以没有参数
val add = (x: Int, y: Int) => x + y
println(add(1, 2)) // 3
// 方法（methods）: 使用 def 关键字声明，参数类型不可省略，返回值的类型可省略。
def add(x: Int, y: Int): Int = x + y
println(add(1, 2)) // 3
// 方法可以有多个参数列表
def addThenMultiply(x: Int, y: Int)(multiplier: Int): Int = (x + y) * multiplier
println(addThenMultiply(1, 2)(3)) // 9
// 方法也可以没有参数
def name: String = System.getProperty("name")
println("Hello, " + name + "!")
// 方法的值取决于最后一句表达式语句的值
def getSquareString(input: Double): String = {
  val square = input * input
  square.toString
}
```

### 类，样例类，单例对象，特质，Main方法

```
// 类（classes）的定义
class Greeter(prefix: String, suffix: String) {
  def greet(name: String): Unit =
    println(prefix + name + suffix)
}
// 用关键字 new 创建类的实例
val greeter = new Greeter("Hello, ", "!")
greeter.greet("Scala developer") // Hello, Scala developer!
// 样例类（case classes）
case class Point(x: Int, y: Int)
// 创建样例类的实例不用 new
val point = Point(1, 2)
val anotherPoint = Point(1, 2)
// 样例类的实例通过值来比较
if (point == anotherPoint) {
  println(point + " and " + anotherPoint + " are the same.")
} else {
  println(point + " and " + anotherPoint + " are different.")
}
// Point(1,2) and Point(1,2) are the same.
// 单例对象
object IdFactory {
  private var counter = 0
  def create(): Int = {
    counter += 1
    counter
  }
}
// 通过对象的名字来调用它的方法
val newId: Int = IdFactory.create()
println(newId) // 1
val newerId: Int = IdFactory.create()
println(newerId) // 2
// 特质（traits）
trait Greeter {
  def greet(name: String): Unit
}
trait Greeter {
  def greet(name: String): Unit =
    println("Hello, " + name + "!")
}
// 用类来扩展特质
class DefaultGreeter extends Greeter
class CustomizableGreeter(prefix: String, postfix: String) extends Greeter {
  override def greet(name: String): Unit = {
    println(prefix + name + postfix)
  }
}
val greeter = new DefaultGreeter()
greeter.greet("Scala developer") // Hello, Scala developer!
val customGreeter = new CustomizableGreeter("How are you, ", "?")
customGreeter.greet("Scala developer") // How are you, Scala developer?
// Main 方法（在单例对象中定义）
object Main {
  def main(args: Array[String]): Unit =
    println("Hello, Scala developer!")
}
```

### 类型层次

Any 类型是所有类型的超类型。

AnyVal 类型代表值类型。

AnyRef 类型代表引用类型。

Nothing 类型是所有类型的子类型。该类型只有一个实例： null。

```
val list: List[Any] = List(
  "a string",
  732,  // an integer
  'c',  // a character
  true, // a boolean value
  () => "an anonymous function returning a string"
)
list.foreach(element => println(element))
```


