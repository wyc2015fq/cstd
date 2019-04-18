# Kotlin语言学习笔记（2） - zwvista - 博客园

## [Kotlin语言学习笔记（2）](https://www.cnblogs.com/zwvista/p/6917199.html)

### 类（classes）

```
// 类声明
class Invoice {
}
// 空的类
class Empty
// 主体构造器（primary constructor）
class Person constructor(firstName: String) {
}
// 省略了关键字的主体构造器
class Person(firstName: String) {
}
// 主体构造器的代码必须写在init（初始化）代码块里面
class Customer(name: String) {
    init {
        logger.info("Customer initialized with value ${name}")
    }
}
// 属性
class Customer(name: String) {
    val customerKey = name.toUpperCase()
}
// 主体构造器里包含多个属性
class Person(val firstName: String, val lastName: String, var age: Int) {
    // ...
}
// 带注解和访问权限的主体构造器 
class Customer public @Inject constructor(name: String) { ... }
// 从属构造器（secondary constructor）
class Person {
    constructor(parent: Person) {
        parent.children.add(this)
    }
}
// 从属构造器调用主体构造器
class Person(val name: String) {
    constructor(name: String, parent: Person) : this(name) {
        parent.children.add(this)
    }
}
// 无法创建实例的类，因为主体构造器被声明为私有访问权限
class DontCreateMe private constructor () {
}
// 具有缺省值参数的主体构造器
class Customer(val customerName: String = "")
// 生成类的实例不需要new
val invoice = Invoice()
val customer = Customer("Joe Smith")
```

### 继承（inheritance）

注意类和类中的方法在Kotlin语言中缺省不可继承，需要被继承的类和方法必须使用open关键字。

而接口和接口中的方法缺省可继承，不需要open关键字。

```
// 隐式继承自Any
class Example
// open表示可继承
// 子类主体构造器调用基类主体构造器
open class Base(p: Int)
class Derived(p: Int) : Base(p)
// 子类从属构造器调用基类的构造器
class MyView : View {
    constructor(ctx: Context) : super(ctx)
    constructor(ctx: Context, attrs: AttributeSet) : super(ctx, attrs)
}
// 覆盖基类的方法
open class Base {
    open fun v() {}
    fun nv() {}
}
class Derived() : Base() {
    override fun v() {}
}
// 覆盖基类的方法但子类不可继承
open class AnotherDerived() : Base() {
    final override fun v() {}
}
// 覆盖基类的属性
open class Foo {
    open val x: Int get { ... }
}
class Bar1 : Foo() {
    override val x: Int = ...
}
// 在主体构造器中覆盖接口中的属性
interface Foo {
    val count: Int
}
class Bar1(override val count: Int) : Foo
class Bar2 : Foo {
    override var count: Int = 0
}
// 使用特殊语法解决接口的多重继承问题
// 接口中的方法都是open，即可继承的
open class A {
    open fun f() { print("A") }
    fun a() { print("a") }
}
interface B {
    fun f() { print("B") }
    fun b() { print("b") }
}
class C() : A(), B {
    override fun f() {
        super<A>.f() // 调用 A.f()
        super<B>.f() // 调用 B.f()
    }
}
// 继承基类的方法但不提供实现，子类仍然可以是抽象类
open class Base {
    open fun f() {}
}
abstract class Derived : Base() {
    override abstract fun f()
}
```

### 属性（properties）

```
// 属性声明
class Address {
    var name: String = ...
    var street: String = ...
    var city: String = ...
    var state: String? = ...
    var zip: String = ...
}
// 属性访问
fun copyAddress(address: Address): Address {
    val result = Address()
    result.name = address.name
    result.street = address.street
    // ...
    return result
}
// 可读可写属性
var allByDefault: Int?
var initialized = 1
// 只读属性
val simple: Int?
val inferredType = 1
// 只读属性 自定义getter
val isEmpty: Boolean
    get() = this.size == 0
// 可读可写属性 自定义getter setter
var stringRepresentation: String
    get() = this.toString()
    set(value) {
        setDataFromString(value)
    }
// 属性类型自动推导
val isEmpty get() = this.size == 0
// setter为公有但getter为私有
var setterVisibility: String = "abc"
    private set
// setter带注解
var setterWithAnnotation: Any? = null
    @Inject set
// 通过field标识符访问属性背后的字段（backing fields）
var counter = 0
    set(value) {
        if (value >= 0) field = value
    }
// 公有属性以及背后的私有属性（backing properties）
private var _table: Map<String, Int>? = null
public val table: Map<String, Int>
    get() {
        if (_table == null) {
            _table = HashMap()
        }
        return _table ?: throw AssertionError("Set to null by another thread")
    }
// 编译期常量
const val SUBSYSTEM_DEPRECATED: String = "This subsystem is deprecated"
@Deprecated(SUBSYSTEM_DEPRECATED) fun foo() { ... }
// 需要延迟初始化的属性使用 lateinit 来修饰
public class MyTest {
    lateinit var subject: TestSubject
    @SetUp fun setup() {
        subject = TestSubject()
    }
    @Test fun test() {
        subject.method()
    }
}
```

### 接口（interfaces）

```
// 接口中的方法可以有缺省实现
interface MyInterface {
    fun bar()
    fun foo() {
      // ...
    }
}
// 在类中实现接口的方法
class Child : MyInterface {
    override fun bar() {
        // ...
    }
}
// 在类中实现接口的属性
interface MyInterface {
    val prop: Int
    val propertyWithImplementation: String
        get() = "foo"
    fun foo() {
        print(prop)
    }
}
class Child : MyInterface {
    override val prop: Int = 29
}
// 使用特殊语法解决接口的多重继承问题
interface A {
    fun foo() { print("A") }
    fun bar()
}
interface B {
    fun foo() { print("B") }
    fun bar() { print("bar") }
}
class C : A {
    override fun bar() { print("bar") }
}
class D : A, B {
    override fun foo() {
        super<A>.foo()
        super<B>.foo()
    }
    override fun bar() {
        super<B>.bar()
    }
}
```

### 可见性修饰符（visibility modifiers）

这个术语太怪癖，其实就是其他语言中的访问权限。

一共四种可见性（访问权限）：
- private： 类或文件内可见
- protected： private的可见性加上子类可见
- internal： 模块内可见
- public： 到处可见

局部的变量，类以及函数没有可见性。

模块是指物理上的模块（IntelliJ IDEA模块，maven和gradle工程等）

```
// 文件示例
// file name: example.kt
package foo
private fun foo() {} // visible inside example.kt
public var bar: Int = 5 // property is visible everywhere
    private set         // setter is visible only in example.kt
internal val baz = 6    // visible inside the same module
```

```
// 类的示例
open class Outer {
    private val a = 1
    protected open val b = 2
    internal val c = 3
    val d = 4  // public by default
    protected class Nested {
        public val e: Int = 5
    }
}
class Subclass : Outer() {
    // a is not visible
    // b, c and d are visible
    // Nested and e are visible
    override val b = 5   // 'b' is protected
}
class Unrelated(o: Outer) {
    // o.a, o.b are not visible
    // o.c and o.d are visible (same module)
    // Outer.Nested is not visible, and Nested::e is not visible either 
}
```

```
// 主体构造器的可见性
class C private constructor(a: Int) { ... }
```

### 扩展（extensions）

包括扩展函数，扩展属性

扩展函数可以是全局的，也可以声明在某个类或伴生对象之中

```
// 扩展函数（extension functions），内部使用 this 指代调用方对象
fun <T> MutableList<T>.swap(index1: Int, index2: Int) {
    val tmp = this[index1] // 'this' corresponds to the list
    this[index1] = this[index2]
    this[index2] = tmp
}
val l = mutableListOf(1, 2, 3)
l.swap(0, 2)
// 静态分发（dispatched statically）
open class C
class D: C()
fun C.foo() = "c"
fun D.foo() = "d"
fun printFoo(c: C) {
    println(c.foo())
}
printFoo(D()) // 输出 c
// 同名时成员函数优先
class C {
    fun foo() { println("member") }
}
fun C.foo() { println("extension") }
c.foo() // member
// 可以重载成员函数
class C {
    fun foo() { println("member") }
}
fun C.foo(i: Int) { println("extension") }
C().foo(1) // extension
// 扩展可空类型
fun Any?.toString(): String {
    if (this == null) return "null"
    return toString()
}
// 扩展属性（extension properties）
val <T> List<T>.lastIndex: Int
    get() = size - 1
// 扩展伴生对象（companion object）
class MyClass {
    companion object { }  // will be called "Companion"
}
fun MyClass.Companion.foo() {
    // ...
}
// 扩展与包的关系
package foo.bar
fun Baz.goo() { ... } 
//
package com.example.usage
import foo.bar.goo
import foo.bar.*
fun usage(baz: Baz) {
    baz.goo()
}
// 分发接收者（dispatch receiver）及扩展接收者（extension receiver）
class D {
    fun bar() { ... }
}
class C {
    fun baz() { ... }
    fun D.foo() {
        bar()   // calls D.bar
        baz()   // calls C.baz
    }
    fun caller(d: D) {
        d.foo()   // call the extension function
    }
}
// 扩展接收者优先于分发接收者
class C {
    fun D.foo() {
        toString()         // calls D.toString()
        this@C.toString()  // calls C.toString()
    }
}
// 分发接收者有多态，扩展接收者没有多态
open class D {
}
class D1 : D() {
}
open class C {
    open fun D.foo() {
        println("D.foo in C")
    }
    open fun D1.foo() {
        println("D1.foo in C")
    }
    fun caller(d: D) {
        d.foo()
    }
}
class C1 : C() {
    override fun D.foo() {
        println("D.foo in C1")
    }
    override fun D1.foo() {
        println("D1.foo in C1")
    }
}
C().caller(D())   // prints "D.foo in C"
C1().caller(D())  // prints "D.foo in C1" - dispatch receiver is resolved virtually
C().caller(D1())  // prints "D.foo in C" - extension receiver is resolved statically
```


