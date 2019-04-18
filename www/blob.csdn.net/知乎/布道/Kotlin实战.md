# Kotlin实战 - 知乎
# 



一、简介、语法

1.1 简介

Kotlin是一种在Java虚拟机上运行的静态编程语言，由JetBrains开发。三个特点：静态编译、基于JVM、java互操作。

1.2 语言特征

1.2.1 基本类型

数值
类型	位宽

Double 64

Float 32

Long 64

Int 32

Short 16

Byte 8

其他类型
类型	解释

字符	用Char表示，不能直接当数值使用，需显示转换

字符串	由 String 表示的

字符串模板	包含可求值的代码片段，结果连接到字符串中

布尔	只有 true 或者 false

数组	由 Array 类表示

1.2.2 类和对象介绍

类的访问修饰符
修饰符 对应的成员 备注

final 不能被覆盖 类成员的默认修饰符

open 可以被覆盖 必须显式的指定

abstract 必须被覆盖 只能在抽象类中使用，抽象成员不能有实现

override 在子类中覆盖成员 如果没有被标记为final，覆盖的成员默认是开放的

类的可见性修饰符
修饰符 对应的成员 顶层声明

public(默认可见性) 所有地方可见 所有地方可见

internal 模块内可见 模块内可见

protected 子类内部可见 不可见

private 类内部可见 在文件中可见

Kotlin类种类
普通类

open class People{     
var name:String?=null; 
} 
class Student():People(){     
var level:Int?=null;     
override fun toString():String{  
       return "[name=${name},level=${level}]"     
}
 }
数据类

data class Student1(var name:String,var level:Int);

fun main(args:Array){

var student1 = Student1("Alice",5);

println(student1)

}
枚举类

enum class Direction {
    NORTH, SOUTH, WEST, EAST
}
enum class Color(val rgb: Int) {

RED(0xFF0000),

GREEN(0x00FF00),

BLUE(0x0000FF)
}
嵌套类

class Outer {

     private val bar: Int = 1

     inner class Inner {

     fun foo() = bar

     }
}
//val demo = Outer().Inner().foo() //==1
1.2.3 变量和函数

Kotlin里变量分 可变变量 和不可变变量 二种。

val（来自value）——不可变引用。使用val声明的变量不能再初始化之后再次赋值。它对应的是Java的final变量。
var（来自variable）——可变引用。这种变量的值可以被改变。这种声明对应java的普通（非final）变量。
函数和变量是Kotlin程序的基本要素。函数在kotlin中已经是一等公民。

1.2.4 类型推导和智能转换

类型推导

在 Java里，我们需要自己声明类型，变量名，以及数值。
在 Kotlin 里，先声明变量名，然后是类型，然后是分配值。很多情况下，你不需要声明类型。只要Kotlin 可以推导，就可以。这个规则同样适用与其他一些类型。。在一些无法推导的场景里，你才需要用完整的声明变量语法指明变量类型。
fun sum(a: Int,b:Int):Int{
    return a+b;
}

fun main(args:Array){
    var c=sum(2,3);
    println(c);
}
智能转换

在很多情况下，在Kotlin有时不用使用明确的转换运算符，因为编译器会在需要的时候自动为了不变的值和输入（安全）而使用is进行监测。
when (x) 
{
 is Int -> print(x + 1)
 is String -> print(x.length + 1)
 is IntArray -> print(x.sum()) 
}
1.3 进阶用法

1.3.1 可控制流和表达式

Kotlin控制流四兄弟if, when, for, while。

if 是带有返回值的表达式，所以kotlin没有三元表达式。示例如下：
val max = if (a > b){
      print("Choose a")

     a 

}
when 取代了 C风格语言的switch ，示例如下：
when (x) {

    1 -> print("x == 1")
     2 -> print("x == 2")

    else -> { // Note the block print("x is neither 1 nor 2") } 

}
for 循环可以对所有提供迭代器的变量进行迭代。示例如下：
fun printArray(days:Array){
    for(day in days){
        println(day)

    }
}
//对数组的for循环不会创建迭代器对象，而是被编译成一个基于索引的循环
while和 do...while和其它语言没什么区别示例如下：
while (x > 0){
     x--
} 

do {

    val y = retrieveData()

} while (y != null) // y 在这是可见的
1.3.2 扩展函数

在不修改原类的情况下, Kotlin能给一个类扩展新功能,无需继承该类,也不用任何设计模式(如装饰模式等)
//为MutableList类扩展一个swap函数: 
fun MutableList.swap(index1: Int, index2: Int) {

       val tmp = this[index1] //this: 当前MutableList对象

       this[index1] = this[index2] this[index2] = tmp 
} 

//对MutableList对象调用swap函数:

val list = mutableListOf(1, 2, 3) 

list.swap(0, 2) 

//MutableList泛化类型:
 //为在表达式中使用泛型,要在函数名前添加泛型参数!
 fun  MutableList.swap(index1: Int, index2: Int) {
        val tmp = this[index1]

       this[index1] = this[index2]

       this[index2] = tmp

}
1.3.3 空安全

Kotlin 类型系统的设计目标就是希望消除代码中null 引用带来的危险。
在Kotlin中，有4把刷子消除NPE风险，安全赋值、安全调用、Elvis
操作符、!! 操作符。
安全赋值
在 Kotlin 中,类型系统明确区分可以指向 null 的引用(可为null 引用)与不可以指向null 的引用(非null 引用)

比如，一个通常的 String 类型变量不可以指向 null:

var a: String = "abc" 

a = null // 编译错误
要允许 null值，我们可以将变量声明为可为 null 的字符串, 写作 String? :

var b: String? = "abc”

b = null // ok
假如你对 a 调用方法或访问属性, 可以确信不会产生NPE, 因此你可以安全地编写以下代码:

var b: String? = "abc”

b = null // ok
但如果你要对 b 访问同样的属性, 就不是安全的, 编译器会报告错误:

val l = b.length // 错误: 变量 'b' 可能为 null
安全调用

若安全赋值这块我们没有做，那么我们还可以选择使用安全调用操作符, 写作 ?.

a?.length
安全调用在链式非常实用，POJO类层次嵌套深的话，java中需要每一层都判断是否空. 比如，假如雇员Bob, 可能被派属某个部门Department (也可能不属于任何部门),这个部门可能存在另一个雇员担任部门主管, 那么, 为了取得Bob 所属部门的主管的名字, (如果存在的话), 我们可以编写下面的代码:

bob?.department?.head?.name
这样的链式调用, 只要属性链中任何一个属性为null, 整个表达式就会返回 null.

Elvis操作符

假设我们有一个可为null 的引用 r, 我们可以用说, “如果 r 不为null, 那么就使用它, 否则, 就使用某个非null 的值 x”:

val l: Int = if (b != null) b.length else -1
除了上例这种完整的 if 表达式之外, 还可以使用Elvis 操作符来表达, 写作 ?::

val l = b?.length ?: -1
如果 ?: 左侧的表达式值不是null, Elvis 操作符就会返回它的值,否则,返回右侧表达式的值.注意,只有在左侧表达式值为 null时,才会计算右侧表达式

!! 操作符
对于 NPE 的热爱者们来说, 还有第三个选择方案. 我们可以写 b!!, 对于 b 不为null 的情况, 这个表达式将会返回这个非null 的值(比如, 在我们的例子中就是一个 String 类型值), 如果 b 是null，这个表达式就会抛出一个NPE:

val l = b!!.length
所以, 如果你确实想要NPE, 你可以抛出它, 但你必须明确地提出这个要求, 否则NPE 不会在你没有注意的地方无声无息地出现。

总结：

Kotlin 的类型系统致力于从我们的代码中消除 NullPointerException. 只有以下情况可能导致
NPE:

明确调用 throw NullPointerException()
使用 !! 操作符，详情见上文
外部的 Java代码导致这个异常
初始化过程中存在某些数据不一致 (在构造器中使用了未初始化的 this)
1.4 高阶用法

1.4.1 高级函数

高阶函数(higher-order function)是一种特殊的函数,它接受函数作为参数,或者返回一个函数。比如我们用redis作为分布式锁的操作，我们把业务操作封装在一个Thread 对象中，让分布式锁的函数里跑这个Thread。有了高阶函数，我们可以这样处理：
fun  redisLock(lock: Lock, body: () -> T): T {

       lock.lock()
        try {

          return body()

       } finally { 

                lock.unlock()

       } 
}
1.4.2 Lambda表达式

Lambda表达式，或者匿名函数,是一种”函数字面值(functionliteral)”, 也就是,一个没有声明的函数,但是立即作为表达式传递出去.
fun main(args: Array) {

  val sum = { x: Int, y: Int ->
println("Computing the sum of $x and $y...")

     x + y

      }

  println(sum(1, 2))

}
注：与Java 不同, Kotlin闭包中捕获的变量是可以修改的(译注: Java 中必须为final 变量)







二、为什么kotlin

2.1 Kotlin设计哲学

实用性
它的设计是基于多年的大规模系统设计的行业经验，不是一门研究型语言，尽可能依靠那些已经出现在其他编程语言中并且被证明为成功的特性和解决方案。另一方面是它对工具的专注， IntelliJ IDEA插件的开发紧跟跟编译器开发的步伐。
精简
类型推导，数据类，智能转换，lambda表达式，Kotlin的扩展函数，替代那些冗长、重复的代码片段，使得代码更简洁。
安全
上文提到的空安全，相对于java减少很多NPE问题。
互操作
不同于其他的JVM语言，Kotlin在互操作性方面走得更远，可以毫不费力的从Java代码中调用Kotlin代码。Kotlin类和方法能够像常规的Java类和方法那样被调用。这将给你带来在项目的任何地方混合Java和Kotlin代码的终极灵活性。
2.2 Kotlin周边生态

IDE
IDEA、Eclipse
Spring
spring5引入Kotlin
构建工具
maven、gradle
应用
android官方开发语言
2.3 开源情况
git开源地址：[https://github.com/JetBrains/kotlin](https://link.zhihu.com/?target=https%3A//github.com/JetBrains/kotlin)







三、代码、实战

3.1 IDE准备

IDEA 或者 Eclipse，高版本的IDEA自带Kotlin环境，Eclise需要kotlin的插件

3.2 maven构建




   org.jetbrains.kotlin
    kotlin-stdlib
    ${kotlin.version}

3.3 项目工程

后台的一个应用中使用了kotlin开发，暂未对外开放。







四、参考&引用

1.《Kotlin in action》

2. [http://www.kotlincn.net](https://link.zhihu.com/?target=http%3A//www.kotlincn.net)

                                                    转自：五阿哥技术部资深开发工程师：马存


