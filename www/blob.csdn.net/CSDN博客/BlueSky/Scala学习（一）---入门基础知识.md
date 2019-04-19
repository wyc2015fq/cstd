# Scala学习（一）---入门基础知识 - BlueSky - CSDN博客
2015年11月25日 21:21:56[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：551
## 1、初识Scala
Scala将面向对象和函数式编程融为一体，因此具有可扩张性。Scala里函数就是对象。函数类型能够被子类继承的类。面向对象编程的动机非常简单：绝大多数的程序都需要某种结构。而最直接的办法就是把数据和操作放进某种形式的容器中。面向对象编程里最伟大的思想是让这些容器完全地通用化，这样就能像保存数据那样保存操作，并且可以把这些容器作为值存储到其他容器里，或作为参数传递给操作。这样的容器被称为对象。
        Scala是纯粹的面向对象的编程语言：每个值都是对象，每次操作都是方法调用。函数式编程有两种指导理念：第一种理念是函数是头等值。在函数式语言中，函数也是值，与整数或者字符串处于同一个地位。函数可以作为参数传递给其他函数，可以当做结果从函数中返回或者保存在变量里。可以在函数里定义其他函数，就像在函数里定义整数一样。第二种理念是程序的操作应该把输入值映射为输出值而不是就地的修改数据。不可变数据结构是函数式语言的一块基石。
## 第二章：
变量定义：
Scala中有两种变量，val和var，val类似于Java里的final变量。一旦初始化了，val就不能再被赋值。Scala类型初始化可以省略类型变量，也可以将变量类型写在其名称之后，用冒号分隔。如：
val msg:String = "Hello world"
msg: String = Hello world
函数定义：
```
def max(x:Int , y:Int): Int = {
      if (x>y) x
      else y
    }
```
max: (x: Int, y: Int)Int
函数定义以def开始，然后是函数名，括号里是参数列表，每个参数都必须带有前缀冒号的类型标注(因为Scala编译器无法推断函数的参数类型）,参数列表后的标注是函数返回结果类型标注。有时候Scala编译器需要函数结果类型的定义。比方说，如果函数是递归的，那么函数结果类型就必须被明确地说明。如果函数仅包含一个语句，那么连花括号都可以选择不写。即上面的max函数可以写成：
def max2(x:Int , y:Int) = if(x>y) x else y
说明：为了便于阅读代码，显式的说明函数结果类型也是推荐使用的，这样就不用编程者研究了函数体之后再去猜结果类型。
Scala脚本：
Scala脚本命令行参数保存在名为args的Scala数组中。Scala里，数组以零开始，可以通过在括号里指定索引值来访问数组元素。Scala数组steps第一个元素是
steps(0),而不是像Java那样steps[0]。如编写 hello.scala :
//打印第一个参数
println("Hello,"+args(0)+"!")
保存后运行，scala hello.scala world 
结果：Hello,world!
While和if枚举应用：
Scala和java一样，必须把while和if的布尔表达式放在括号里。必须写成if(x>10) 而不能写成 if x > 10 
用foreach和for做枚举：
while循环的编码风格被称为指令式（imperative）编程，而Scala更倾向于函数式(functional)的编程风格。
如打印args中的变量参数：
agrs.foreach(arg => println(arg))
这里是对args调用了foreach方法，并把函数作为参数传入，在这里是名为arg的函数字面量。函数体是println(arg)。
for的用法：
for (arg <- args)  //代表对于args中的arg
  println(arg)
说明：<- 是符号（代表“其中”的意思）它左侧arg是val的名称，因此只写arg即可。
类型参数化数组（Array）：
Scala使用new实例化对象（或叫类实例）。greetStrings对象被定义为val不能被重新赋值成别的数组，但其内部对象可以改变的，比如greetStrings(0)="asia"
```
val greetStrings = new Array[String](3)  
//申请了存储string类型数组Array，其数组长度为3
greetStrings(0) = "Hello"
greetStrings(1) = ","
greetStrings(2) = "word!\n"
//以上等价于val greetStrings = Array("Hello",",","world\n")
for(i <- 0 to 2)
  print(greetStrings(i))
for(str <- gteetStrings)
  print(str)
```
Scala一切皆对象，所以greetStrings(0)相当于转换为greetStrings.apply(i)；当对带有括号并包含一到若干参数的变量赋值时，编译器将使用对象的update方法对括号里的参数（索引值）和括号右边的对象执行调用。比如：
greetStrings(0) = "Hello"  将被转化为 greetStrings.update(0,"Hello")
列表（List）：
方法没有副作用是函数式编程风格的重要理念，计算并返回值是方法唯一的目的。函数式编程思想在面向对象编程中的应用也意味着对象的不可改变性。scala列表一旦创建就不能改变（包括内部元素）。
`<span style="font-size:18px;">val oneTwoThree = List（1,2,3）</span>`列表类最常用的操作是‘：：：’，它可以将两个List连接起来；’：：‘发音为“cons”，是做前缀，相当于把一个放在前面组合为新的整体。如：
```
<span style="font-size:18px;">val aB = List（'a','b'）
val cD = List('c','d')
val aBCD = aB ::: cD</span><pre name="code" class="html"><span style="font-size:18px;">val f = List('f')</span>
```
```
<span style="font-size:18px;">//aBCD:List[Char] = List(a, b, c, d)
</span>
```
val abF = aB :: f
```
<span style="font-size:18px;">//abF:List[Any] = List(List(a, b), f)
</span>
```
Nil是空列表的简写，可以使用cons操作符把所有元素都穿起来，并以Nil来作结尾定义新列表。如：
`<span style="font-size:18px;">val listTest = 1::2::3::Nil</span>`Scala列表不提供append操作，因为随着列表的增长，append的耗时将呈现线性增长，而使用'：：'做前缀则仅消耗固定的时间。
![](https://img-blog.csdn.net/20151125212102605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20151125212121848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

