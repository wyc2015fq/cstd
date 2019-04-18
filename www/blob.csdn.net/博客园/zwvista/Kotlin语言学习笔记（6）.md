# Kotlin语言学习笔记（6） - zwvista - 博客园

## [Kotlin语言学习笔记（6）](https://www.cnblogs.com/zwvista/p/7006950.html)

### 运算符重载（Operator overloading）

一元运算符
|Expression|Translated to|
|----|----|
|+a|a.unaryPlus()|
|-a|a.unaryMinus()|
|!a|a.not()|

```
data class Point(val x: Int, val y: Int)
operator fun Point.unaryMinus() = Point(-x, -y)
val point = Point(10, 20)
println(-point)  // prints "(-10, -20)"
```

递增递减运算符
|Expression|Translated to|
|----|----|
|a++|a.inc() + see below|
|a--|a.dec() + see below|

算术运算符
|Expression|Translated to|
|----|----|
|a + b|a.plus(b)|
|a - b|a.minus(b)|
|a * b|a.times(b)|
|a / b|a.div(b)|
|a % b|a.rem(b), a.mod(b) (deprecated)|
|a..b|a.rangeTo(b)|

```
data class Counter(val dayIndex: Int) {
    operator fun plus(increment: Int): Counter {
        return Counter(dayIndex + increment)
    }
}
```

in 运算符
|Expression|Translated to|
|----|----|
|a in b|b.contains(a)|
|a !in b|!b.contains(a)|

下标存取运算符
|Expression|Translated to|
|----|----|
|a[i]|a.get(i)|
|a[i, j]|a.get(i, j)|
|a[i_1, ..., i_n]|a.get(i_1, ..., i_n)|
|a[i] = b|a.set(i, b)|
|a[i, j] = b|a.set(i, j, b)|
|a[i_1, ..., i_n] = b|a.set(i_1, ..., i_n, b)|

调用运算符
|Expression|Translated to|
|----|----|
|a()|a.invoke()|
|a(i)|a.invoke(i)|
|a(i, j)|a.invoke(i, j)|
|a(i_1, ..., i_n)|a.invoke(i_1, ..., i_n)|

增强运算符
|Expression|Translated to|
|----|----|
|a += b|a.plusAssign(b)|
|a -= b|a.minusAssign(b)|
|a *= b|a.timesAssign(b)|
|a /= b|a.divAssign(b)|
|a %= b|a.remAssign(b), a.modAssign(b) (deprecated)|

相等不等运算符
|Expression|Translated to|
|----|----|
|a == b|a?.equals(b) ?: (b === null)|
|a != b|!(a?.equals(b) ?: (b === null))|

比较运算符
|Expression|Translated to|
|----|----|
|a > b|a.compareTo(b) > 0|
|a < b|a.compareTo(b) < 0|
|a >= b|a.compareTo(b) >= 0|
|a <= b|a.compareTo(b) <= 0|

### Null 安全性

```
// 可空类型和非空类型
var a: String = "abc"
a = null // compilation error
var b: String? = "abc"
b = null // ok
val l = a.length
val l = b.length // error: variable 'b' can be null
// 检查 null
val l = if (b != null) b.length else -1
if (b != null && b.length > 0) {
    print("String of length ${b.length}")
} else {
    print("Empty string")
}
// 安全调用
b?.length
bob?.department?.head?.name
val listWithNulls: List<String?> = listOf("A", null)
for (item in listWithNulls) {
     item?.let { println(it) } // prints A and ignores null
}
// If either `person` or `person.department` is null, the function is not called:
person?.department?.head = managersPool.getManager()
// Elvis运算符
val l: Int = if (b != null) b.length else -1
val l = b?.length ?: -1
fun foo(node: Node): String? {
    val parent = node.getParent() ?: return null
    val name = node.getName() ?: throw IllegalArgumentException("name expected")
    // ...
}
// !! 运算符
val l = b!!.length
// 安全转型
val aInt: Int? = a as? Int
// 可空类型的集合类型
val nullableList: List<Int?> = listOf(1, 2, null, 4)
val intList: List<Int> = nullableList.filterNotNull()
```

### 异常（Exceptions）

Kotlin 语言不提供 checked exception

```
// throw表达式 和 try表达式
throw MyException("Hi There!")
try {
    // some code
}
catch (e: SomeException) {
    // handler
}
finally {
    // optional finally block
}
// try 是表达式
val a: Int? = try { parseInt(input) } catch (e: NumberFormatException) { null }
// throw 表达式的类型是 Nothing
val s = person.name ?: throw IllegalArgumentException("Name required")
val s = person.name ?: fail("Name required")
println(s)     // 's' is known to be initialized at this point
val x = null           // 'x' has type `Nothing?`
val l = listOf(null)   // 'l' has type `List<Nothing?>
```

### 注解（Annotations）

注解的参数类型包括：
- Java的基本类型
- 字符串
- 枚举
- 其他注解
- 以上类型的数组

使用端的注解包括：
- file
- property（Java不可见）
- field
- get（getter）
- set（setter）
- receiver（扩展函数以及扩展属性的接收者参数）
- param（主体构造器的参数）
- setparam（setter的参数）
- delegate（委托属性）

```
// 语法
annotation class Fancy
// 详细语法
@Target(AnnotationTarget.CLASS, AnnotationTarget.FUNCTION,
        AnnotationTarget.VALUE_PARAMETER, AnnotationTarget.EXPRESSION)
@Retention(AnnotationRetention.SOURCE)
@MustBeDocumented
annotation class Fancy
// 用法
@Fancy class Foo {
    @Fancy fun baz(@Fancy foo: Int): Int {
        return (@Fancy 1)
    }
}
// 给主体构造器加注解时必须使用constructor关键字
class Foo @Inject constructor(dependency: MyDependency) {
    // ...
}
// 给属性存取器加注解
class Foo {
    var x: MyDependency? = null
        @Inject set
}
// 带参数的注解
annotation class Special(val why: String)
@Special("example") class Foo {}
// 把注解用作其他注解的参数
annotation class ReplaceWith(val expression: String)
annotation class Deprecated(
        val message: String,
        val replaceWith: ReplaceWith = ReplaceWith(""))
@Deprecated("This function is deprecated, use === instead", ReplaceWith("this === other"))
// 把类用作注解的参数
import kotlin.reflect.KClass
annotation class Ann(val arg1: KClass<*>, val arg2: KClass<out Any>)
@Ann(String::class, Int::class) class MyClass
// 给lambda表达式加上注解
annotation class Suspendable
val f = @Suspendable { Fiber.sleep(10) }
// 使用端的注解
class Example(@field:Ann val foo,    // annotate Java field
              @get:Ann val bar,      // annotate Java getter
              @param:Ann val quux)   // annotate Java constructor parameter
// 给整个文件加注解
@file:JvmName("Foo")
package org.jetbrains.demo
// Java注解
import org.junit.Test
import org.junit.Assert.*
import org.junit.Rule
import org.junit.rules.*

class Tests {
    // apply @Rule annotation to property getter
    @get:Rule val tempFolder = TemporaryFolder()

    @Test fun simple() {
        val f = tempFolder.newFile()
        assertEquals(42, getTheAnswer())
    }
}
// ①
// Java
public @interface Ann {
    int intValue();
    String stringValue();
}
// Kotlin
@Ann(intValue = 1, stringValue = "abc") class C
// ①
// Java
public @interface AnnWithValue {
    String value();
}
// Kotlin
@AnnWithValue("abc") class C
// ②
// Java
public @interface AnnWithArrayValue {
    String[] value();
}
// Kotlin
@AnnWithArrayValue("abc", "foo", "bar") class C
// ③
// Java
public @interface AnnWithArrayMethod {
    String[] names();
}
// Kotlin 1.2+:
@AnnWithArrayMethod(names = ["abc", "foo", "bar"]) 
class C
// Older Kotlin versions:
@AnnWithArrayMethod(names = arrayOf("abc", "foo", "bar")) 
class D
// ④
// Java
public @interface Ann {
    int value();
}
// Kotlin
fun foo(ann: Ann) {
    val i = ann.value
}
```


