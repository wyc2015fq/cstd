# Kotlin语言学习笔记（4） - zwvista - 博客园

## [Kotlin语言学习笔记（4）](https://www.cnblogs.com/zwvista/p/6965158.html)

### 函数

```
// 函数定义及调用
fun double(x: Int): Int {
    return 2*x
}
val result = double(2)
// 调用方法
Sample().foo() // create instance of class Sample and call foo
// 参数语法
fun powerOf(number: Int, exponent: Int) {
...
}
// 缺省参数
fun read(b: Array<Byte>, off: Int = 0, len: Int = b.size()) {
...
}
// 带缺省参数的方法被覆盖时不能带缺省参数
open class A {
    open fun foo(i: Int = 10) { ... }
}
class B : A() {
    override fun foo(i: Int) { ... }  // no default value allowed
}
// 缺省参数之后还可以有不带缺省值的参数
fun foo(bar: Int = 0, baz: Int) { /* ... */ }
foo(baz = 1) // The default value bar = 0 is used
// 缺省参数之后还可以有 lambda 参数
fun foo(bar: Int = 0, baz: Int = 1, qux: () -> Unit) { /* ... */ }
foo(1) { println("hello") } // Uses the default value baz = 1 
foo { println("hello") }    // Uses both default values bar = 0 and baz = 1
// 具名参数 
fun reformat(str: String,
             normalizeCase: Boolean = true,
             upperCaseFirstLetter: Boolean = true,
             divideByCamelHumps: Boolean = false,
             wordSeparator: Char = ' ') {
...
}
reformat(str)
reformat(str, true, true, false, '_')
reformat(str,
    normalizeCase = true,
    upperCaseFirstLetter = true,
    divideByCamelHumps = false,
    wordSeparator = '_'
)
reformat(str, wordSeparator = '_')
// 展开（spread）运算符
fun foo(vararg strings: String) { /* ... */ }
foo(strings = *arrayOf("a", "b", "c"))
// 返回类型为Unit的函数
fun printHello(name: String?): Unit {
    if (name != null)
        println("Hello ${name}")
    else
        println("Hi there!")
    // `return Unit` or `return` is optional
}
fun printHello(name: String?) {
    ...
}
// 表达式函数
fun double(x: Int): Int = x * 2
fun double(x: Int) = x * 2
// varargs
fun <T> asList(vararg ts: T): List<T> {
    val result = ArrayList<T>()
    for (t in ts) // ts is an Array
        result.add(t)
    return result
}
val list = asList(1, 2, 3)
val a = arrayOf(1, 2, 3)
val list = asList(-1, 0, *a, 4)
// 扩展函数的中缀表示法
infix fun Int.shl(x: Int): Int {
    // ...
}
// calling the function using the infix notation
1 shl 2
// is the same as
1.shl(2)
// 成员函数的中缀表示法
class MyStringCollection {
    infix fun add(s: String) { /* ... */ }
    
    fun build() {
        this add "abc"   // Correct
        add("abc")       // Correct
        add "abc"        // Incorrect: the receiver must be specified
    }
}
// 局部函数
fun dfs(graph: Graph) {
    fun dfs(current: Vertex, visited: Set<Vertex>) {
        if (!visited.add(current)) return
        for (v in current.neighbors)
            dfs(v, visited)
    }
    dfs(graph.vertices[0], HashSet())
}
fun dfs(graph: Graph) {
    val visited = HashSet<Vertex>()
    fun dfs(current: Vertex) {
        if (!visited.add(current)) return
        for (v in current.neighbors)
            dfs(v)
    }
    dfs(graph.vertices[0])
}
// 成员函数
class Sample() {
    fun foo() { print("Foo") }
}
Sample().foo() // creates instance of class Sample and calls foo
// 泛型函数
fun <T> singletonList(item: T): List<T> {
    // ...
}
```

### 高阶函数和lambda表达式（Higher-Order Functions and Lambdas）

高阶函数：将函数作为参数或返回值的函数。

lambda表达式的特点
- lambda表达式使用尖括号
- 参数（类型可省略）位于->符号之前
- 函数体位于->符号之后
- 不能指定返回类型
- lambda表达式参数位于最后时可以脱离小括号
- return语句将跳出包含lambda表达式的外围函数

匿名函数（Anonymous Functions）
- 没有函数名的函数
- 可以指定返回类型
- 必须包含在小括号中
- return语句跳出匿名函数自身

高阶函数和匿名函数统称函数字面量（function literal）。

```
// 高阶函数
fun <T> lock(lock: Lock, body: () -> T): T {
    lock.lock()
    try {
        return body()
    }
    finally {
        lock.unlock()
    }
}
// 使用函数引用（function references）将函数作为参数传给函数
fun toBeSynchronized() = sharedResource.operation()
val result = lock(lock, ::toBeSynchronized)
// 使用lambda表达式
val result = lock(lock, { sharedResource.operation() })
// lambda表达式是最后一个参数时，括弧可以省略
lock (lock) {
    sharedResource.operation()
}
// 高阶函数map
fun <T, R> List<T>.map(transform: (T) -> R): List<R> {
    val result = arrayListOf<R>()
    for (item in this)
        result.add(transform(item))
    return result
}
val doubled = ints.map { value -> value * 2 }
// 只有一个参数时可以使用隐式参数it
ints.map { it * 2 }
// LINQ风格的代码
strings.filter { it.length == 5 }.sortBy { it }.map { it.toUpperCase() }
// 不需要使用的参数可以用下划线表示
map.forEach { _, value -> println("$value!") }
// lambda表达式语法
val sum = { x: Int, y: Int -> x + y }
val sum: (Int, Int) -> Int = { x, y -> x + y }
ints.filter { it > 0 } // this literal is of type '(it: Int) -> Boolean'
// lambda表达式隐式返回最后一个表达式的值
// 两段代码效果相同
ints.filter {
    val shouldFilter = it > 0 
    shouldFilter
}
ints.filter {
    val shouldFilter = it > 0 
    return@filter shouldFilter
}
// 匿名函数
fun(x: Int, y: Int): Int = x + y
fun(x: Int, y: Int): Int {
    return x + y
}
// 使用匿名函数
// 参数类型可省略
ints.filter(fun(item) = item > 0)
// 闭包（Closures）
var sum = 0
ints.filter { it > 0 }.forEach {
    sum += it
}
print(sum)
// 带 receiver（隐含调用方）的函数字面量
sum : Int.(other: Int) -> Int
1.sum(2)
val sum = fun Int.(other: Int): Int = this + other
// String.(Int) -> Boolean 与 (String, Int) -> Boolean 相互兼容
val represents: String.(Int) -> Boolean = { other -> toIntOrNull() == other }
println("123".represents(123)) // true
fun testOperation(op: (String, Int) -> Boolean, a: String, b: Int, c: Boolean) =
    assert(op(a, b) == c)
testOperation(represents, "100", 100, true) // OK
// 通过上下文推导 receiver
class HTML {
    fun body() { ... }
}
fun html(init: HTML.() -> Unit): HTML {
    val html = HTML()  // create the receiver object
    html.init()        // pass the receiver object to the lambda
    return html
}
html {       // lambda with receiver begins here
    body()   // calling a method on the receiver object
}
```


