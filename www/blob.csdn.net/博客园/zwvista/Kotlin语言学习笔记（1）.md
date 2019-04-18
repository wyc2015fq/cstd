# Kotlin语言学习笔记（1） - zwvista - 博客园

## [Kotlin语言学习笔记（1）](https://www.cnblogs.com/zwvista/p/6880974.html)

fun main(args: Array<String>) {
    println("Hello, World!")
}

### 基本语法
- 声明常量用val，声明变量用var，声明函数用fun。
类型修饰采用后置语法，省略时由编译器根据上下文自行推导：
声明常量和变量时用冒号指示类型，自带初值时类型修饰可省略。
声明函数时用冒号指示返回类型。
若函数体采用代码块形式，则没有返回值时返回类型可省略，有返回值时返回类型不能省略。
若函数体采用表达式形式，则无论有没有返回值返回类型均可省略。
||常量|变量|函数（有返回值）|函数（无返回值）|
|----|----|----|----|----|
|val i = 10val i: Int = 10|var i = 10var i: Int = 10|fun f(n: Int): Int {return n + 1}fun f(n: Int): Int = n + 1fun f(n: Int) = n + 1|fun f(n: Int): Unit {println(n)} fun f(n: Int) {println(n)}fun f(n: Int): Unit = println(n)fun f(n: Int) = println(n)| |
|let i = 10let i: Int = 10|var i = 10var i: Int = 10|func f(n: Int) -> Int {return n + 1}|func f(n: Int) -> () {print(n)} func f(n: Int) -> Void {print(n)} func f(n: Int) {print(n)}| |

- 字符串模板（string template）/ 字符串插值（string interpolation）的语法形式为 $变量 以及 ${表达式}。
字符串中如果需要表示 $ 字符，可使用 ${'$'} 这种转义形式。
多行字符串使用三引号"""。
||字符串模板/字符串插值|多行字符串|
|----|----|----|
|val x = 10; val y = 20val s = "x=$x, y=${y + 1}"|val price = """${'$'}9.99"""| |
|let x = 10, y = 20let s = "x=\(x), y=\(y + 1)"|var price = """$9.99"""| |
|int x = 10, y = 20;var s = $"x={x}, y={y + 1}";|var price = @"$9.99"| |
|x=10, y=21|<空行>$9.99<空行>| |

- 区间
|Kotlin|Swift|输出|
|----|----|----|
|for (i in 1..4) print(i)|for i in 1...4 {print(i, terminator: "")}|1234|
|for (i in 4..1) print(i)|// for i in 4...1 {print(i, terminator: "")} // error|空字符串|
|for (i in (1..4).reversed()) print(i)|for i in (1...4).reversed() {print(i, terminator: "")}|4321|
|for (i in 4 downTo 1) print(i)|for i in stride(from: 4, through: 1, by: -1) {print(i, terminator: "")}for i in stride(from: 4, to: 0, by: -1) {print(i, terminator: "")}|4321|
|for (i in 1..4 step 2) print(i)|for i in stride(from: 1, through: 4, by: 2) {print(i, terminator: "")}for i in stride(from: 1, to: 4, by: 2) {print(i, terminator: "")}|13|
|for (i in 4 downTo 1 step 2) print(i)|for i in stride(from: 4, through: 1, by: -2) {print(i, terminator: "")}for i in stride(from: 4, to: 1, by: -2) {print(i, terminator: "")}|42|
|for (i in 1 until 10) print(i)|for i in 1..<10 {print(i, terminator: "")}|123456789|
|val x = 3if (x in 1..10) println(x)if (x !in 1..10) println(x)|let x = 3if case 1...10 = x {print(x)}if 1...10 ~= x {print(x)}if (1...10).contains(x) {print(x)}if !(1...10).contains(x) {print(x)}|3|

- 相等
||结构相等/值相等|引用相等|
|----|----|----|
|a == ba != b|a === ba !== b| |
|a == ba != b|a === ba !== b| |

### 控制流

Kotlin语言中的控制流语句有：
- if...else...语句（表达式）：用于处理双分支

val a = 3; val b = 4
val m1: Int
if (a < b) m1 = a else m1 = b
val m2 = if (a < b) a else b
val m3 = if (a < b) {println("a"); a} else {println("b"); b}

- 不带参数的when表达式：用于处理多分支

fun Int.isOdd() = this % 2 == 1
fun Int.isEven() = this % 2 == 0
val x = 3
when {
    x.isOdd() -> print("x is odd")
    x.isEven() -> print("x is even")
    else -> print("x is funny")
}

- 带参数的when表达式：用于处理值匹配
单个值，多个值，表达式，在与不在某个区间，是与不是某个类型，以上所有情况之外

fun describe(obj: Any): String =
    when (obj) {
        1          -> "One"
        2, 3       -> "Two or Three"
        in 4..6    -> "Four to Six"
        "Hello"    -> "Greeting"
        is Long    -> "Long"
        !is String -> "Not a string"
        else       -> "Unknown"
    }
println(describe(1)); // One
println(describe(2)); // Two or Three
println(describe(4)); // Four to Six
println(describe(7)); // Not a string
println(describe(9L)); // Long
println(describe(10.0)); // Not a string
println(describe("abc")); // Unknown

- for...in...语句，while语句，do...while语句：用于处理循环

### 函数
- 函数体有代码块以及表达式两种形式。使用表达式形式时可省略返回类型。
- 没有返回值的函数可以被视为返回类型为 Unit 的函数。
- 函数可以嵌套。局部函数可使用外部函数的变量。

fun outside() {
    var a = 1
    fun inside() {
        a++;
    }
    inside();  // a==2
}

- 函数可以带缺省参数。
- 调用函数时可以使用参数名。

fun add(a: Int = 1, b: Int = 2) = a + b
println(add());  // 3
println(add(3)); // 5
println(add(b = 3)); // 4

- 变长参数：只能有一个，使用 vararg 修饰符，在函数内部被视为数组。
||变长参数|
|----|----|
|fun avg(vararg numbers: Double): Double {}| |
|func avg(numbers: Double...) -> Double {}| |
|double Avg(params double[] numbers) {}| |
|double avg(double... numbers) {}| |

- 函数是一等公民。可以作为函数的参数，也可以成为函数的返回值。
||函数的类型（有返回值）|函数的类型（没有返回值）|
|----|----|----|
|(Int, Int) -> Boolean|(Int, Int) -> Unit| |
|(Int, Int) -> Bool|(Int, Int) -> Void| |
|Func<int, int, bool>|Action<int, int>| |

- 类的方法被称为成员函数。
- 可以使用扩展函数来为既有的类添加功能。
- 中缀形式：仅有一个参数的成员函数以及扩展函数在使用 infix 修饰符修饰的情况下，调用方可使用中缀形式。


