# Kotlin语言学习笔记（7） - zwvista - 博客园

## [Kotlin语言学习笔记（7）](https://www.cnblogs.com/zwvista/p/8076009.html)

### 反射

```
// 反射
val c = MyClass::class
val c2 = MyClass::class.java
// 获取KClass的引用
val widget: Widget = ...
assert(widget is GoodWidget) { "Bad widget: ${widget::class.qualifiedName}" }
// 函数引用
fun isOdd(x: Int) = x % 2 != 0
val numbers = listOf(1, 2, 3)
println(numbers.filter(::isOdd)) // prints [1, 3]
// 函数重载时也能使用函数引用
fun isOdd(x: Int) = x % 2 != 0
fun isOdd(s: String) = s == "brillig" || s == "slithy" || s == "tove"
val numbers = listOf(1, 2, 3)
println(numbers.filter(::isOdd)) // refers to isOdd(x: Int)
val predicate: (String) -> Boolean = ::isOdd   // refers to isOdd(x: String)
// 实例
fun <A, B, C> compose(f: (B) -> C, g: (A) -> B): (A) -> C {
    return { x -> f(g(x)) }
}
fun length(s: String) = s.length
val oddLength = compose(::isOdd, ::length)
val strings = listOf("a", "ab", "abc")
println(strings.filter(oddLength)) // Prints "[a, abc]"
// 属性引用
// val getter
val x = 1
fun main(args: Array<String>) {
    println(::x.get()) // prints "1"
    println(::x.name)  // prints "x"
}
// var setter
var y = 1
fun main(args: Array<String>) {
    ::y.set(2)
    println(y) // prints "2"
}
// 属性引用用作函数 
val strs = listOf("a", "bc", "def")
println(strs.map(String::length)) // prints [1, 2, 3]
// 类成员函数
class A(val p: Int)
fun main(args: Array<String>) {
    val prop = A::p
    println(prop.get(A(1))) // prints "1"
}
// 扩展属性
val String.lastChar: Char
    get() = this[length - 1]
fun main(args: Array<String>) {
    println(String::lastChar.get("abc")) // prints "c"
}
```


