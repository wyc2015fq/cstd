# Kotlin语言学习笔记（5） - zwvista - 博客园

## [Kotlin语言学习笔记（5）](https://www.cnblogs.com/zwvista/p/6978738.html)

### 委托模式（Delegation）

### 类的委托

```
interface Base {
    fun print()
}
class BaseImpl(val x: Int) : Base {
    override fun print() { print(x) }
}
class Derived(b: Base) : Base by b
fun main(args: Array<String>) {
    val b = BaseImpl(10)
    Derived(b).print() // prints 10
}
```

这里 Derived 类使用 by 关键字将接口 Base 的实现委托给属性 b。

注意被委托的方法也可以被覆盖（重写）。

### 属性的委托

```
class Example {
    var p: String by Delegate()
}
class Delegate {
    operator fun getValue(thisRef: Any?, property: KProperty<*>): String {
        return "$thisRef, thank you for delegating '${property.name}' to me!"
    } 
    operator fun setValue(thisRef: Any?, property: KProperty<*>, value: String) {
        println("$value has been assigned to '${property.name} in $thisRef.'")
    }
}
val e = Example()
println(e.p)
// Example@33a17727, thank you for delegating ‘p’ to me!
e.p = "NEW"
// NEW has been assigned to ‘p’ in Example@33a17727.
```

这里属性 p 被委托给了 Delegate 类。

也就是读取属性 p 时 Delegate 类的 getValue 方法将被调用。

而在设置属性 p 时 Delegate 类的 setValue 方法将被调用。

### 标准的属性委托

### Lazy

```
val lazyValue: String by lazy {
    println("computed!")
    "Hello"
}
fun main(args: Array<String>) {
    println(lazyValue)
    println(lazyValue)
}
// computed!
// Hello
// Hello
```

### Observable

```
import kotlin.properties.Delegates
class User {
    var name: String by Delegates.observable("<no name>") {
        prop, old, new ->
        println("$old -> $new")
    }
}
fun main(args: Array<String>) {
    val user = User()
    user.name = "first"
    user.name = "second"
}
// <no name> -> first
// first -> second
```

### Map

```
class User(val map: Map<String, Any?>) {
    val name: String by map
    val age: Int     by map
}
val user = User(mapOf(
    "name" to "John Doe",
    "age"  to 25
))
println(user.name) // Prints "John Doe"
println(user.age)  // Prints 25
```

### 解构声明（Destructuring Declarations）

解构声明：将某个对象分解成多个变量的声明。

```
// 解构声明
val (name, age) = person
println(name)
println(age)
// 解构声明的语义
val name = person.component1()
val age = person.component2()
// 在for循环中使用解构声明
for ((a, b) in collection) { ... }
// 返回多个值的函数
data class Result(val result: Int, val status: Status)
fun function(...): Result {
    // computations
    return Result(result, status)
}
// Now, to use this function:
val (result, status) = function(...)
// 遍历map时使用解构声明
for ((key, value) in map) {
   // do something with the key and the value
}
// 遍历map时能够使用解构声明的原因
operator fun <K, V> Map<K, V>.iterator(): Iterator<Map.Entry<K, V>> = entrySet().iterator()
operator fun <K, V> Map.Entry<K, V>.component1() = getKey()
operator fun <K, V> Map.Entry<K, V>.component2() = getValue()
// 在解构声明中使用下划线忽略某个变量
val (_, status) = getResult()
// 在lambda表达式中使用解构声明
map.mapValues { entry -> "${entry.value}!" }
map.mapValues { (key, value) -> "$value!" }
// 参数语法
{ a -> ... } // one parameter
{ a, b -> ... } // two parameters
{ (a, b) -> ... } // a destructured pair
{ (a, b), c -> ... } // a destructured pair and another parameter
// 使用下划线
map.mapValues { (_, value) -> "$value!" }
// 指定参数的类型
map.mapValues { (_, value): Map.Entry<Int, String> -> "$value!" }
map.mapValues { (_, value: String) -> "$value!" }
```

### 集合（Collections）

Kotlin语言中的集合分为可变集合和不可变集合。（mutable/immutable collections）

Kotlin语言没有集合的专有语法。

创建集合可以使用标注库的方法。

```
listOf(), mutableListOf(), setOf(), mutableSetOf()
mapOf(a to b, c to d)
```

```
// 可变集合及其不变视图
val numbers: MutableList<Int> = mutableListOf(1, 2, 3)
val readOnlyView: List<Int> = numbers
println(numbers)        // prints "[1, 2, 3]"
numbers.add(4)
println(readOnlyView)   // prints "[1, 2, 3, 4]"
readOnlyView.clear()    // -> does not compile
val strings = hashSetOf("a", "b", "c", "c")
assert(strings.size == 3)
// 不变集合
val items = listOf(1, 2, 3)
// 可变集合及其快照
class Controller {
    private val _items = mutableListOf<String>()
    val items: List<String> get() = _items.toList()
}
// 常用方法
val items = listOf(1, 2, 3, 4)
items.first() == 1
items.last() == 4
items.filter { it % 2 == 0 }   // returns [2, 4]
// 常用方法
val rwList = mutableListOf(1, 2, 3)
rwList.requireNoNulls()        // returns [1, 2, 3]
if (rwList.none { it > 6 }) println("No items above 6")  // prints "No items above 6"
val item = rwList.firstOrNull()
// 常用方法
val readWriteMap = hashMapOf("foo" to 1, "bar" to 2)
println(readWriteMap["foo"])  // prints "1"
val snapshot: Map<String, Int> = HashMap(readWriteMap)
```

### 类型检查和类型转换

```
// 确认变量的类型使用 is 和 !is 运算符
if (obj is String) {
    print(obj.length)
}
if (obj !is String) { // same as !(obj is String)
    print("Not a String")
}
else {
    print(obj.length)
}
// 智能类型转换
fun demo(x: Any) {
    if (x is String) {
        print(x.length) // x is automatically cast to String
    }
}
if (x !is String) return
print(x.length) // x is automatically cast to String
// x is automatically cast to string on the right-hand side of `||`
if (x !is String || x.length == 0) return
// x is automatically cast to string on the right-hand side of `&&`
if (x is String && x.length > 0) {
    print(x.length) // x is automatically cast to String
}
when (x) {
    is Int -> print(x + 1)
    is String -> print(x.length + 1)
    is IntArray -> print(x.sum())
}
// 安全及不安全的类型转换
val x: String = y as String
val x: String? = y as String?
val x: String? = y as? String
// 泛型类型检查
if (something is List<*>) {
    something.forEach { println(it) } // The items are typed as `Any?`
}
fun handleStrings(list: List<String>) {
    if (list is ArrayList) {
        // `list` is smart-cast to `ArrayList<String>`
    }
}
```

### This 表达式
- 在类的内部，this指向当前对象
- 在扩展函数和带接收者的函数字面量中，this指向接收者对象。
- this缺省指向最内层对象，this可以带限定符。

```
class A { // implicit label @A
    inner class B { // implicit label @B
        fun Int.foo() { // implicit label @foo
            val a = this@A // A's this
            val b = this@B // B's this

            val c = this // foo()'s receiver, an Int
            val c1 = this@foo // foo()'s receiver, an Int

            val funLit = lambda@ fun String.() {
                val d = this // funLit's receiver
            }

            val funLit2 = { s: String ->
                // foo()'s receiver, since enclosing lambda expression
                // doesn't have any receiver
                val d1 = this
            }
        }
    }
}
```


