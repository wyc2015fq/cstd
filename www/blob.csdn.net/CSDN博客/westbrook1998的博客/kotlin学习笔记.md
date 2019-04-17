# kotlin学习笔记 - westbrook1998的博客 - CSDN博客





置顶2018年07月13日 15:18:32[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：3352








![](https://user-images.githubusercontent.com/3332274/30523722-0101ba56-9bef-11e7-929c-8843369c5d44.png)

# 最基本语法

## `package`和`import` 和java是基本一致

***不同于java：***

kotlin中不区分导入是类还是函数，可以直接导入顶层函数（不属于任何一个类） 

kotlin中可以把多个类放在同一个文件中，文件名称也可以随意选择
## 函数定义

```
fun sum(a:Int,b:Int):Int{
    return a+b;
}
fun sum(a:Int,b:Int)=a+b
fun printf(s:String):Unit{
    println(s)
}
```

可以省略返回类型（类型推导） 

可以省略花括号和return语句（表达式函数体） 

可以省略Unit（无返回值）
## 变量声明

`var`（可变引用） 和 `val`（不可变引用）

```
// 可省略类型声明
var b:Int=15
var c="str"
// 如果声明时没有初始化，则需要制定类型
var a
a=15
// 变量值可以改变，但类型不可变
var a=35
// a="str" (error)
// val引用自身不可改变，但指向的对象可以改变
val l=arrayListOf("java")
l.add("kotlin")
```

## 字符串模板

`$` 引用变量  `${}` 引用表达式（如`list[0]`）

```
var list= listOf<Int>(1,2,3,4,5)
var l:Int=6
println("Hello $l")
println("Hello $list[0]")
println("Hello ${list[0]}")
```

## 类和属性（对比java，更加简洁）

kotlin中public是默认修饰符，可以省略

```
//普通java类
public class Person {
    private final String name;

    public Person(String name){
        this.name=name;
    }
    public String getName() {
        return name;
    }
}
//kotlin写法
class Person(val name:String)
```

kotlin中属性（字段+访问器）是头等语言属性 

无需手动写`getter` 和 `setter` 可以直接访问属性，但底层调用的是 `getter` 和 `setter`

调用对象构造方法无需`new`
```java
class Person(
    val name:String,
    var isMarried:Boolean
)
fun main(args: Array<String>) {
    val person=Person("Bob",true)
    println(person.name)
    println(person.isMarried)
}
```

可以自定义访问器

```
class Rectangle(val height:Int,val width:Int){
//无需用一个字段来存储是否是正方形
    val isSquare:Boolean
        get() = height==width
}
```

## 枚举类

简单枚举类

```
enum class Color{
    RED,GREEN,BLUE
}
```

带属性、方法的枚举类

```
enum class Color(val r:Int,val g:Int,val b:Int){
    //注意如果属性后面有方法，则要加分号
    RED(255,0,0),GREEN(0,255,0),BLUE(0,0,255);

    fun rgb()=(r*256+g)*256+b
}
```

## when结构

```
fun getMnemonic(color:Color)=
    when(color){
        Color.RED->"red"
        Color.GREEN->"green"
        Color.BLUE->"blue"
    }
```

可以导入常量值，即可省略类名

```
fun getMnemonic(color:Color)=
    when(color){
        RED->"red"
        GREEN->"green"
        BLUE->"blue"
    }
```

when可以使用任意对象，不仅限于常量，字符串等

```
fun mix(c1:Color,c2:Color)=
    when(setOf(c1,c2)){
        setOf(RED,GREEN)->"Yellow"
        setOf(RED,BLUE)->"Pink"
        setOf(GREEN,BLUE)->"Verdant"
        else->"nothing"
    }
```

不带参数的when（可以减少每次判断都需要创建的垃圾对象）

```
fun mixOptimized(c1:Color,c2:Color)=
    when{
        (c1==RED && c2==GREEN) || (c1==GREEN && c2==RED)->"Yello"
        else->"nothing"
    }
```

## 智能转换：合并类型检查和转换

```
//表达式接口，有两个实现类，数字和加法
interface Expr
class Num(val value:Int):Expr
class Sum(val left:Expr,val right:Expr):Expr
fun eval(e:Expr):Int{
    //java写法
    if(e is Num){
        //判断e的实际类型后，强转类型（父类转子类）
        val n=e as Num
        return n.value
    }
    //kotlin写法
    if(e is Sum){
        //判断e的实际类型后，编译器自动进行了类型转换
        return eval(e.right)+ eval(e.left)
    }
    //抛出异常
    throw IllegalArgumentException("Unknown expression")
}
```

## `if`有返回值

可以省略`return`

```
fun eval(e:Expr):Int{
    if(e is Num){
        e.value
    }
    if(e is Sum){
        eval(e.right)+ eval(e.left)
    }
    throw IllegalArgumentException("Unknown expression")
}
```

使用`when`来代替`if`

```
fun eval(e:Expr):Int=
    when(e){
        is Num->e.value
        is Sum->eval(e.right)+eval(e.left)
        else->throw IllegalArgumentException("Unknow expression")
    }
```

## `for`循环（区间）

kotlin中区间都是闭合的

```
fun main(args: Array<String>) {
    val oneToTen=1..10
    println(oneToTen)
    for(i in oneToTen){
        println(i)
    }
    //递减区间
    for(i in 10 downTo 1){
        println(i)
    }
    //设置步长
    for(i in 10 downTo 1 step 2){
        println(i)
    }
    //右开区间
    for(i in 1 until 10){
        println(i)
    }
}
```

## `map`，`list`的迭代

```
fun main(args: Array<String>) {
    val binaryReps=TreeMap<Char,String>()
    for(c in 'A'..'F'){
        val binary=Integer.toBinaryString(c.toInt())
        binaryReps[c]=binary
    }
    for((letter,binary) in binaryReps){
        println("$letter = $binary")
    }
    val list= arrayListOf("10","11","1001")
    for((index,element) in list.withIndex()){
        println("$index:$element")
    }
}
```

## `in` 运算符判断是否在区间

```
fun isLetter(c:Char)=c in 'a'..'z' || c in 'A'..'Z'
fun isNotDigit(c:Char)=c !in '0'..'9'
fun main(args: Array<String>) {
    println(isLetter('q'))
    println(isNotDigit('x'))

}
```

## 异常

kotlin中`throw` 是一个表达式 
`try catch finally`

```
fun readNumber(reader:BufferedReader):Int?{
    try{
        val line=reader.readLine()
        return Integer.parseInt(line)
    }catch (e:NumberFormatException){
        return null
    }finally {
        reader.close()
    }
}
```

`try` 作为表达式

```
fun readNumber(reader:BufferedReader):Int?{
    val number=try{
        Integer.parseInt(reader.readLine())
    }catch (e:NumberFormatException){
        null
    }
}
```

# Kotlin的函数

实现一个格式化输出`List`的函数 
***Java式写法***

```
/**
 * formatted output
 */
fun <T> joinToString(
    collection: Collection<T>,
    separator:String,
    prefix:String,
    postfix:String
):String{
    val result=StringBuilder(prefix)
    for((index,element) in collection.withIndex()){
        if(index>0)
            result.append(separator)
        result.append(element)
    }
    result.append(postfix)
    return result.toString()
}
```

***kotlin写法***

命名参数，即指定参数所对应的值
`joinToString(collection,seperator=" ",prefix=" ",postfix=".")`
设置参数默认值

```
/**
 * formatted output
 */
fun <T> joinToString(
    collection: Collection<T>,
    separator:String=", ",
    prefix:String="",
    postfix:String=""
):String{
    ...
}
```

`kotlin`文件会编译成相应的`java`类，比如`join.kt`会被编译成`JoinKt.class``join.kt`中的顶层函数就被编译成`JoinKt`类中的静态函数

`kotlin` 中还存在顶层属性 

可以用`const val` 定义常量，相当于`public static final`

## 拓展函数和属性

是一个定义在类外边（补充）的一个类的成员函数

```java
fun String.lastChar():Char=this.get(this.length-1)

fun main(args:Array<String>){
    println("Kotlin".lastChar())
}
```

还可以省略`this`

```
fun String.lastChar():Char=get(length-1)
```

**拓展函数不能破坏类的封装性，因此不能访问类私有或者受保护的成员**

`java`中调用`kotlin`函数

```
package sometest;

public class javaTest {
    public static void main(String[] args) {
        char c=KeaneKt.lastChar("Kotlin");
    }
}
```

### 拓展函数不可重写

成员函数和拓展函数的对比 
***成员函数重写后，具体调用哪个类的方法取决于对象实际类型***
***拓展函数不存在重写，只当成静态函数对待，因此具体调用哪个类的函数取决于变量的静态类型（引用的类型）***
```
open class View{
    open fun click()=println("View clicked");
}

class Button:View(){
    override fun click() =println("Button clicked");
}

fun View.showOff()=println("I'm a view")
fun Button.showOff()=println("I'm a button")

fun main(args:Array<String>){
    val view:View=Button()
    view.click()

    val view_2:View=Button()
    view_2.showOff()
}
```

### 拓展属性

属性是字段和访问器的组合 

在`kotlin` 中属性是头等语言特性，在类定义中的属性有默认的访问器（`getter setter`） 

而拓展属性没有支持字段，因此需要自定义`getter/setter`，同时也不能初始化
```
val String.lastChar:Char
    get() = get(length-1)

var StringBuilder.lastChar:Char
    get() = get(length-1)
    set(value:Char) = setCharAt(length-1,value)

fun main(args:Array<String>){
    println("Kotlin".lastChar)
    val sb=StringBuilder("Kotlin")
    sb.lastChar='h'
    println(sb)
}
```

## 可变参数

标准库函数`listOf` 的定义 
`vararg` 表示函数接受任意个数参数

```
public fun <T> listOf(vararg elements: T): List<T> = 
    if (elements.size > 0) elements.asList() 
    else emptyList()
```

在`java`中，可以把整个数组作为可变参数传入，会自动解包 

而在`kotlin` 中，要求用`*(展开运算符)` 显式地解包数组

```
fun main(args:Array<String>){
    val list= listOf("args: ",*args)
    println(list)
}
```

## 中缀调用

在kotlin中，如果函数有`infix` 修饰符标记的话，就允许使用中缀符号调用 

例如`to`
`1 to "one"` 等价于 `1.to("one")`
```
fun main(args:Array<String>){
    val map= mapOf(1 to "one",2.to("two"))
    println(map)
}
```

# 字符串

```
fun main(args:Array<String>){
    //字符串分隔
    println("12.345-6.A".split("\\.".toRegex()))
    println("12.345-6.A".split(".","-"))

    println("/Users/test/chapter.adoc".substringAfter("."))
    println("/Users/test/chapter.adoc".substringBefore("."))
    println("/Users/test/chapter.adoc".substringAfterLast("/"))
    println("/Users/test/chapter.adoc".substringBeforeLast("."))

    //三重引号中可以无需转义
    val kotlinLogin="""
        | //
        |//
        |/ \
    """.trimIndent()
    println(kotlinLogin)
}
```

# 局部函数和拓展

***重复代码***

```
class User(val id:Int,val name:String,val address:String)

fun saveUser(user:User){
    //重复类似的判断
    if(user.name.isEmpty()){
        throw IllegalArgumentException(
            "Can't save user ${user.id}: empty name"
        )
    }
    if(user.address.isEmpty()){
        throw IllegalArgumentException(
            "Can't save user ${user.id}: empty address"
        )
    }
}

fun main(args:Array<String>){
    saveUser(User(1,"",""))
}
```

***提取局部函数避免重复***

```
fun saveUser(user:User){
    fun validate(user:User,
                 value:String,
                 fieldName:String){
        if(value.isEmpty()){
            throw IllegalArgumentException(
                "Can't save user ${user.id}: empty ${fieldName}"
            )
        }
    }

    validate(user,user.name,"Name")
    validate(user,user.address,"Address")
}
```

***局部函数可以访问所在函数的所有参数和变量***

```
fun saveUser(user:User){
    fun validate(value:String,
                 fieldName:String){
        if(value.isEmpty()){
            throw IllegalArgumentException(
                "Can't save user ${user.id}: empty ${fieldName}"
            )
        }
    }

    validate(user.name,"Name")
    validate(user.address,"Address")
}
```

***把验证函数放在User类的拓展函数***

```
fun User.validateBeforeSave(){
    fun validate(value:String,
                 fieldName:String){
        if(value.isEmpty()){
            throw IllegalArgumentException(
                "Can't save user ${id}: empty ${fieldName}"
            )
        }
    }

    validate(name,"Name")
    validate(address,"Address")
}
fun saveUser(user:User){
    user.validateBeforeSave()
}
```

# 类、对象和接口

**vs**

kotlin和java的一些不同 

1. 接口可以包含属性声明 

2. 声明默认是`final` 和`public` 的 

3. 嵌套类不是内部类，没有包含对其外部类的隐式引用
## 接口与继承

`java` 中 `父类/接口.super.方法` == `kotlin` 中 `super<父类/接口>.方法`

```
package javafx.sometest

//接口
interface Clickable{
    fun click()
    //可以带有方法体
    fun showOff()=println("I'm clickable!")
}

interface Focusable{
    fun setFocus(b:Boolean)=
        println("I ${if (b) "got" else "lost"} focus.")

    fun showOff()=println("I'm focusable!")
}

//如果实现多个带有同名方法的接口，编译器强制要求提供具体实现(显示实现)
class Button:Clickable,Focusable{
    override fun showOff() {
        println("implement")
        //显式继承特定父类(接口)的方法
        super<Clickable>.showOff()
        super<Focusable>.showOff()
    }

    //重写
    override fun click()=println("I was clicked")
}

fun main(args:Array<String>){
    Button().click()
    Button().showOff()
}
```

## 默认`final`

`java` 中类和方法默认是`open` 的，而在`kotlin`中，则默认是`final`（不可继承）

```
//open修饰的类 可以继承
open class RichButton:Clickable{
    //默认final 子类不可重写该方法
    fun disable(){}
    //open修饰 子类可以重写
    open fun animate(){}
    //接口默认是open的，所以重写接口的方法也是open的
    //即override 默认是 open的
    override fun click(){}

    //显式声明final
    final override fun showOff() {}
}
```

## 抽象类和抽象方法

```
//不可实例化的抽象类
abstract class Animated{
    //抽象方法 子类必须重写
    abstract fun animate()

    //将非抽象函数显式声明为open 默认为final
    open fun stopAnimating(){}
    fun animateTwice(){}
}
```

## 　类中访问修饰符
|修饰符|相关成员|注|
|----|----|----|
|final|不能被重写|默认|
|open|可以被重写|需显式声明|
|abstract|必须被重写|只能在抽象类中使用|
|override|重写父类或接口中的成员|如果没有显示声明final 重写的成员默认open|

## 　默认public

kotlin的可见性修饰符
|修饰符|类成员|顶层声明|
|----|----|----|
|public|所有地方可见|所有地方可见|
|internal|模块中可见|模块中可见|
|protected|子类中可见||
|private|类中可见|类中可见|

## 　模块（module）

***一个模块就是一组一起编译的Kotlin文件，有可能是一个Intellij IDEA模块，一个Eclipse项目，一个Maven或Gradle项目或者一组使用调用Ant任务进行编译的文件***

## 默认是嵌套类而不是内部类

`Kotlin` 中默认的嵌套类不能访问外部类的实例，除非显式声明 

在`java` 中，内部类有一个外部类的隐式引用，所以如果外部类没有实现`Serializable` 接口，内部类也也无法序列化 

而在`Kotlin` 中，内部类默认是没有外部类的引用的，即默认为`static`
```
interface State:Serializable
interface View{
    fun getCurrentState():State
    fun restoreState(state:State){}
}

class Butoon:View{
    override fun getCurrentState(): State {
        return ButtonState()
    }

    override fun restoreState(state: State) {}

    class ButtonState:State{}
}
```

使用`inner` 修饰符可以持有外部类的引用 

即 
`static class A (Java) == class A (Kotlin)`
`class A (Java) == inner class A (Kotlin)`
`Kotlin` 中使用`this@Outer` 取访问外部类

```
class Outer{
    inner class Inner{
        fun getOuterReference():Outer=this@Outer
    }
}
```

## 密封类

```
//对可能的子类做出限制
sealed class Expr{
    class Num(val value:Int):Expr()
    class Sum(val left:Expr,val right:Expr):Expr()
}
```

## 类的构造

**这种直接加在类名后面的构造方法称为主构造方法**

```
//标准用法 
//constructor用来开始一个构造方法
//init用来引入一个初始化语句块
class User constructor(_nickname:String){
    val nickname:String

    init {
        nickname=_nickname
        //this.nickname=_nickname
    }
}

//简化 无需使用constructor和init
class User(_nickname:String){
    val nickname=_nickname
}

//最简化
class User(val nickname:String)

//带默认值的构造方法
class User(val nickname:String="Kotlin")
```

**也可以声明多个从构造方法**

```
open class View {
    constructor(ctx: Context) {
    }

    constructor(ctx: Context, attr: AttributeSet) {
    }
}

class MyButton : View {
    constructor(ctx: Context)
        : super(ctx) {
    }

    constructor(ctx: Context, attr: AttributeSet)
        : super(ctx, attr) {
    }
}
```

## 接口中的属性

```
interface User{
    val nickname:String
}
class PrivateUser(override val nickname:String):User
class SubscribingUser(val email:String):User{
    override val nickname: String
        //自定义getter
        //没有支持字段来存储这个值 每次调用这个nickname就相当于调用email的这个方法
        get() = email.substringBefore('@')
}
class FacebookUser(val accountId:Int):User{
    fun getFacebookName(accountId: Int)="Test"
    //这里虽然与SubscribingUser类似 但这里有支持字段存储值
    //也就是说实例创建时 nickname就已经存储有值了
    override val nickname: String=getFacebookName(accountId)
}
```

## getter和setter

```
class User(val name:String){
    var address:String="unspecified"
    set(value:String) {
        println("""
            Address was changed for $name:
            "$field" -> "$value". 
        """.trimIndent())
    }
}

class LengthCounter{
    var counter:Int=0
        //修改访问器的可见性
        private set

    //无法直接通过setter修改 但可以通过方法来修改
    fun addWord(word:String){
        counter+=word.length
    }
}
```

## 通用对象方法

`toString()`

打印一个对象的默认显示

```
class Client(val name:String,val postalCode:Int){
    override fun toString(): String="Client(name=$name,postalCode=$postalCode)"
}
```

`equals()`
`Kotlin` 中 `==` 是比较两个对象，也就是调用`equals` 方法  而`===` 才是`Java` 中的`==` 也就是比较两个引用

```
class Client(val name:String,val postalCode:Int){
    override fun equals(other: Any?): Boolean {
        if(other==null || other!is Client){
            return false
        }
        return name==other.name && postalCode==other.postalCode
    }
}
```

`hashCode()`

如果两个对象相等，他们必须有形同的hash值

```
class Client(val name:String,val postalCode:Int){
    override fun hashCode(): Int=name.hashCode()*31+postalCode
}
```

## 数据类——自动生成通用方法

数据类会自动生成通用方法

```
class User(val name:String,val postalCode:Int){
}
data class Client(val name:String,val postalCode:Int){
}
fun main(args: Array<String>) {
    println(Client("hh",123))
    println(User("hh",123))
}
//打印
//Client(name=hh, postalCode=123)
//sometest.User@21a06946
```

## 实现类的委托

**装饰器模式：** 创建一个新类，与原始类实现同样的接口，将原始类的一个实例作为字段保存，与原始类同样的方法不用修改，只需要转发到原始类 

样板代码写法：

```
class DelegatingCollection<T>:Collection<T>{
    private val innerList= arrayListOf<T>()
    override val size: Int
        get() = innerList.size
    override fun contains(element: T): Boolean=innerList.contains(element)
    override fun containsAll(elements: Collection<T>): Boolean=innerList.containsAll(elements)
    override fun isEmpty(): Boolean =innerList.isEmpty()
    override fun iterator(): Iterator<T> =innerList.iterator()
}
```

使用`by` 关键字

```
class CountingSet<T>(
    val innerSet:MutableCollection<T>
):MutableCollection<T> by innerSet{
    var objectsAdded=0

    override fun add(element: T): Boolean {
        objectsAdded++
        return innerSet.add(element)
    }

    override fun addAll(elements: Collection<T>): Boolean {
        objectsAdded+=elements.size
        return innerSet.addAll(elements)
    }
}
```

## 对象声明

将类的声明和创建该类的唯一实例结合起来

```
object Payroll{
    val allEmployees= arrayListOf<Person>()
    fun calculateSalary(){
        for(person in allEmployees){
            //...
        }
    }
}
```

与类不同的是对象声明无需构造器，对象声明后实例已创建，构造方法无意义

## 伴生对象

`Kotlin` 中没有静态方法，静态字段的概念，顶层函数和对象声明大多数情况可以替代 

伴生对象`companion object` 使得可以直接通过容器类来访问这个对象的方法和属性

```
class A{
    companion object {
        fun bar()=println("Companion object called")
    }
}
fun main(args: Array<String>) {
    A.bar()
}
```

伴生对象成员在子类中不能被重写 
**伴生对象也可以有名字，作为一个声明在类中的普通对象**
**一个类只能有一个伴生对象**
```
class Person(val name:String){
    companion object Loader{
        fun fromJSON(jsonText:String):Person{
            //...
        }
    }
}
fun main(args: Array<String>) {
    A.bar()
    A.Companion.bar()
    Person.Loader.fromJSON("HH")
}
```

没有声明对象名称的伴生对象则默认为`Companion`

因此，在使用伴生函数的拓展函数时 可以是这些`类名.` 也可以是`类名.伴生对象名称.`

## 对象表达式

`object` 关键词还可以用来声明匿名对象 
**匿名对象不是单例的**

```
window.addMouseListener(object : MouseAdapter() {
    override fun mouseClicked(e: MouseEvent) {
        // ...
    }

    override fun mouseEntered(e: MouseEvent) {
        // ...
    }
})
```

对象表达式中的代码可以访问创建它的函数中的变量

# Lambda编程

第一个例子

```
data class Person(val name:String,val age:Int)

fun main(args: Array<String>) {
    val people=listOf(Person("alice",29),Person("bob",31))
    println(people.maxBy { it.age })
}
```

这里`maxBy` 函数可以用在任何集合中，传入的参数是一个函数，指定用于比较的值

```
fun main(args: Array<String>) {

    //标准用法
    val sum={x:Int,y:Int->x+y}
    println(sum(1,2))

    //直接调用
    val mul={x:Int->2*x}(5)
    println(mul)

    //用run来执行lambda
    run {println(42)}

}
```

作为例子的`maxBy` 的简写

```java
fun main(args: Array<String>) {

    val people=listOf(Person("alice",29),Person("bob",31))
    //println(people.maxBy ({ p:Person->p.age }))
    //如果该lambda是函数的最后一个实参，花括号可以放括号外面
    //println(people.maxBy(){p:Person->p.age})
    //如果该lambda是函数的唯一实参，括号可以省略
    //println(people.maxBy { p:Person->p.age })
    //类型推导
    //println(people.maxBy { p->p.age })
    //使用it
    println(people.maxBy { it.age })

}
```

## 作用域中访问变量

```
fun test(msg:Array<String>,prefix:String){
    var cnt=0
    msg.forEach { 
        //lambda中访问函数参数
        println("$prefix $it")
        //lambda中访问并修改局部变量
        cnt++
    }
}
fun main(args: Array<String>) {
    //使用it引用
    val list= listOf<Int>(1,2,3,4,5,6)
    list.forEach {
        if(it%2==0){
            println(it)
        }
        else{
            println("change: ${it+1}")
        }
    }
}
```

与`Java` 不同，`Kotlin` 允许在`lambda` 内部访问和修改`非final`变量 称为**捕捉**
**一般来说，局部变量的生命周期被限制在声明这个变量的函数中，但是如果它被`lambda` 捕捉了，使用这个变量的代码可以被存储并稍后再执行（`lambda` 可以被赋值）**
**`非final`变量的捕捉是通过将它的值封装在一个特殊的包装类中，而这个包装类的引用和`lambda`代码一起存储**
## 成员引用

```
//成员引用(包括拓展函数)
val getAge=Person::age
//引用顶层函数
val sal=::salute
//构造方法引用
val createPerson=::Person
fun main(args: Array<String>) {
    run(sal)
    val p= createPerson("mike",20)
    //需要提供一个该类实例作为参数
    println(getAge(p))
}
```

## 集合的函数式api

一些用于集合的高阶函数的用法

```
data class Person(val name:String,val age:Int)
fun main(args: Array<String>) {
    val list=listOf(1,2,3,4)
    val people=listOf(Person("alice",29),Person("bob",31), Person("carol",31))
    val numbers= mapOf(0 to "zero",1 to "one")
    println(list.filter { it%2==0 })
    //[2, 4] 筛除
    println(list.map { it*it })
    //[1, 4, 9, 16]  对所有元素执行操作
    println(people.map { it.name })
    //[alice, bob,carol]
    println(people.filter { it.age==people.maxBy { it.age }!!.age })
    //[Person(name=bob, age=31), Person(name=carol, age=31)]
    println(numbers.mapValues { it.value.toUpperCase() })
    //{0=ZERO, 1=ONE}
    println(people.all { it.age>27 })
    //true 所有符合
    println(people.any { it.age<30 })
    //true 有任一符合
    println(people.count { it.age==31 })
    //2
    println(people.filter { it.age==31 }.size)
    //2 同上
    println(people.find{it.age>27})
    //Person(name=alice, age=29)  找到第一个
    println(people.groupBy { it.age })
    //{29=[Person(name=alice, age=29)], 31=[Person(name=bob, age=31), Person(name=carol, age=31)]} 划分为map
}
```

嵌套集合

```
fun main(args: Array<String>) {
    val books = listOf(Book("Thursday Next", listOf("Jasper Fforde")),
        Book("Mort", listOf("Terry Pratchett")),
        Book("Good Omens", listOf("Terry Pratchett", "Neil Gaiman")))

    println(books.flatMap { it.authors })
    //[Jasper Fforde, Terry Pratchett, Terry Pratchett, Neil Gaiman]  未去重的广义并
    println(books.flatMap { it.authors }.toSet())
    //[Jasper Fforde, Terry Pratchett, Neil Gaiman]  广义并的概念
    val l=listOf(listOf("a","b"), listOf("c","d"))
    println(l.flatten())
    //[a, b, c, d]
}
```

**惰性集合操作——序列**

序列可以避免创建一些临时的中间集合（如果集合过大，十分低效）

```
fun main(args: Array<String>) {
    val people= listOf(Person("alice",29), Person("bob",27), Person("carol",35))
    //之前的操作
    //在map和filter都会分别产生一个列表
    println(people.map { it.name }.filter { it.startsWith('a') })

    //惰性序列操作 相当于是迭代所有元素 最后找出符合条件的元素
    println(people.asSequence().map { it.name }.filter { it.startsWith('a') }.toList())
}
    //通过一个简单的测试可以看出速度差距是特别大的
    //test
    var list=(1..10000000)
    var startTime=System.currentTimeMillis();
    var alist=list.map { it*it }.filter { it%2==0 }
    println(System.currentTimeMillis()-startTime)
    //1191ms
    startTime=System.currentTimeMillis()
    var blist=list.asSequence().map { it*it }.filter { it%2==0 }
    println(System.currentTimeMillis()-startTime)
    //1ms
```

## 创建序列

```
fun main(args: Array<String>) {
    //通过列表生成
    val seq= listOf(1,2,3,5,7).asSequence()
    //通过generateSequence函数生成
    val naturalNumbers= generateSequence(0) { it+1 }
    //因为序列是惰性的 这时候并不能直接打印
    //println(naturalNumbers)
    val numbersTo100=naturalNumbers.takeWhile { it<=100 }
    //只有在调用末端操作时才会求值
    println(numbersTo100.sum())
}
```

## 带接收者的`lambda`：`with`和`apply`

**`with`**
`with`函数接受两个参数，第二个参数是一个`lambda`，第一个参数则是该`lambda`的接收者 

在`lambda`中可以用`this`引用来访问该接收者对象，也可以省略`this`
```java
fun main(args: Array<String>) {
    fun alphabet()=with(StringBuilder()){
        for(letter in 'A'..'Z'){
            append(letter)
        }
        append("\nNow I know the alphabet!")
        toString()
    }
}
```

`with` 返回的是`lambda` 最后的表达式结果，如果是要返回接收者对象，则使用`apply`

```java
fun main(args: Array<String>) {
    fun alphabet()=StringBuilder().apply {
        for(letter in 'A'..'Z'){
            append(letter)
        }
        append("\nNow I know the alphabet!")
    }.toString()
}
```















