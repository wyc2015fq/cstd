# Kotlin语言学习笔记（3） - zwvista - 博客园

## [Kotlin语言学习笔记（3）](https://www.cnblogs.com/zwvista/p/6919483.html)

### 数据类（Data Classes）
`data class User(val name: String, val age: Int)`
编译器自动生成的有：
- equals()/hashCode()
- toString() 形式为 "User(name=John, age=42)"
- componentN() 函数
- copy() 函数

数据类必须符合以下条件。
- 主体构造器必须至少有一个参数。
- 所有主体构造器的参数都必须被标注为var或val。
- 数据类不可以是 abstract, open, sealed 或 inner。

```
// 缺省参数的例子。
data class User(val name: String = "", val age: Int = 0)

// 自动生成的copy函数。
// fun copy(name: String = this.name, age: Int = this.age) = User(name, age) 
// 实际应用copy函数的代码
val jack = User(name = "Jack", age = 1)
val olderJack = jack.copy(age = 2)

// 数据类与解构
val jane = User("Jane", 35) 
val (name, age) = jane
println("$name, $age years of age") // prints "Jane, 35 years of age"
```

标准数据类包括`Pair`和`Triple`。

### 密封类（Sealed Classes）

密封类对继承有所限制。

密封类可以有子类，但是必须在同一文件中。

```
sealed class Expr
data class Const(val number: Double) : Expr()
data class Sum(val e1: Expr, val e2: Expr) : Expr()
object NotANumber : Expr()
```

密封类的子类的子类不必在同一文件中。

使用密封类的好处在于在when表达式中可以处理所有情况，不必用else。

```
fun eval(expr: Expr): Double = when(expr) {
    is Const -> expr.number
    is Sum -> eval(expr.e1) + eval(expr.e2)
    NotANumber -> Double.NaN
    // the `else` clause is not required because we've covered all the cases
}
```

### 嵌套类（Nested Classes）

```
// 嵌套类
class Outer {
    private val bar: Int = 1
    class Nested {
        fun foo() = 2
    }
}
val demo = Outer.Nested().foo() // == 2

// 内部类（Inner Classes）
class Outer {
    private val bar: Int = 1
    inner class Inner {
        fun foo() = bar
    }
}
val demo = Outer().Inner().foo() // == 1

// 匿名内部类（Anonymous Inner Classes）
window.addMouseListener(object: MouseAdapter() {
    override fun mouseClicked(e: MouseEvent) {
        // ...
    }
    override fun mouseEntered(e: MouseEvent) {
        // ...
    }
})

// 函数型接口的对象生成
val listener = ActionListener { println("clicked") }
```

### 枚举类（Enum Classes）

每个枚举常量都是一个对象。

```
enum class Direction {
    NORTH, SOUTH, WEST, EAST
}

// 初始化
enum class Color(val rgb: Int) {
        RED(0xFF0000),
        GREEN(0x00FF00),
        BLUE(0x0000FF)
}

// 枚举类内部的匿名类
// 在枚举类你内定义抽象方法
// 然后使用枚举对象的匿名类来实现抽象方法
// 这里需要使用分号隔开枚举对象的定义和抽象方法的定义
enum class ProtocolState {
    WAITING {
        override fun signal() = TALKING
    },
    TALKING {
        override fun signal() = WAITING
    };
    abstract fun signal(): ProtocolState
}

// 使用枚举常量
// EnumClass.valueOf(value: String): EnumClass
// EnumClass.values(): Array<EnumClass>
// enumValues<T>()
// enumValueOf<T>()
enum class RGB { RED, GREEN, BLUE }
inline fun <reified T : Enum<T>> printAllValues() {
    print(enumValues<T>().joinToString { it.name })
}
printAllValues<RGB>() // prints RED, GREEN, BLUE

// 每个枚举常量都有如下属性
val name: String
val ordinal: Int
// 枚举类都实现了Comparable接口
```

### 对象表达式（Object Expressions）

对象表达式扩展了Java中的匿名内部类。

```
window.addMouseListener(object : MouseAdapter() {
    override fun mouseClicked(e: MouseEvent) {
        // ...
    }
    override fun mouseEntered(e: MouseEvent) {
        // ...
    }
})
// 构造器带参数的类也能使用对象表达式
open class A(x: Int) {
    public open val y: Int = x
}
interface B {...}
val ab: A = object : A(1), B {
    override val y = 15
}
// 使用对象表达式可以达成匿名类（对象）的效果
fun foo() {
    val adHoc = object {
        var x: Int = 0
        var y: Int = 0
    }
    print(adHoc.x + adHoc.y)
}
// 匿名对象只能用于local和private声明
// 如果用于public声明，则成员不可访问
class C {
    // Private function, so the return type is the anonymous object type
    private fun foo() = object {
        val x: String = "x"
    }
    // Public function, so the return type is Any
    fun publicFoo() = object {
        val x: String = "x"
    }
    fun bar() {
        val x1 = foo().x        // Works
        val x2 = publicFoo().x  // ERROR: Unresolved reference 'x'
    }
}
// 对象表达式中的方法可以访问外层变量（不仅限于final变量）
fun countClicks(window: JComponent) {
    var clickCount = 0
    var enterCount = 0
    window.addMouseListener(object : MouseAdapter() {
        override fun mouseClicked(e: MouseEvent) {
            clickCount++
        }
        override fun mouseEntered(e: MouseEvent) {
            enterCount++
        }
    })
    // ...
}
```

### 对象声明（Object declarations）

使用对象声明可以方便的声明单件（Singleton）。

在类中使用对象声明可以声明类的伴生对象（Companion object）。

伴生对象中声明的方法相当于其他语言的静态方法。

```
object DataProviderManager {
    fun registerDataProvider(provider: DataProvider) {
        // ...
    }
    val allDataProviders: Collection<DataProvider>
        get() = // ...
}
// 使用单件
DataProviderManager.registerDataProvider(...)
// 有基类的情况
object DefaultListener : MouseAdapter() {
    override fun mouseClicked(e: MouseEvent) {
        // ...
    }
    override fun mouseEntered(e: MouseEvent) {
        // ...
    }
}
// 类的伴生对象，用于实现静态方法
class MyClass {
    companion object Factory {
        fun create(): MyClass = MyClass()
    }
}
// 使用伴生对象的方法（相当于静态方法）
val instance = MyClass.create()
// 使用伴生对象自身
class MyClass {
    companion object {
    }
}
val x = MyClass.Companion
// 伴生对象可以实现接口
interface Factory<T> {
    fun create(): T
}
class MyClass {
    companion object : Factory<MyClass> {
        override fun create(): MyClass = MyClass()
    }
}
```

### 对象表达式和对象声明的语义
- 对象表达式是贪婪的（使用时即刻生成）。
- 对象声明是懒惰的 （第一次访问时生成）。
- 伴生对象在相应的类被生成时生成。


