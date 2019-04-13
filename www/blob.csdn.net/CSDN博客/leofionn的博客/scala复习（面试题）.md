
# scala复习（面试题） - leofionn的博客 - CSDN博客


2018年03月06日 17:40:29[leofionn](https://me.csdn.net/qq_36142114)阅读数：4010


1.Scala怎样声明变量与常量？
var val

2.Scala数据类型有哪些？

|数据类型|描述|
|Byte|8位有符号补码整数。数值区间为 -128 到 127|
|Short|16位有符号补码整数。数值区间为 -32768 到 32767|
|Int|32位有符号补码整数。数值区间为 -2147483648 到 2147483647|
|Long|64位有符号补码整数。数值区间为 -9223372036854775808 到 9223372036854775807|
|Float|32位IEEE754单精度浮点数|
|Double|64位IEEE754单精度浮点数|
|Char|16位无符号Unicode字符, 区间值为 U+0000 到 U+FFFF|
|String|字符序列|
|Boolean|true或false|
|Unit|表示无值，和其他语言中void等同。用作不返回任何结果的方法的结果类型。Unit只有一个实例值，写成()。|
|Null|null 或空引用|
|Nothing|Nothing类型在Scala的类层级的最低端；它是任何其他类型的子类型。|
|Any|Any是所有其他类的超类|
|AnyRef|AnyRef类是Scala里所有引用类(reference class)的基类|
上表中列出的数据类型都是对象，也就是说scala没有java中的原生类型。在scala是可以对数字等基础类型调用方法的。
3.String 对象是可变还是不可变？假如要创建一个可以修改的字符串，应该使用哪个类？
在 Scala 中，字符串的类型实际上是 Java String，它本身没有 String 类。
在 Scala 中，String 是一个不可变的对象，所以该对象不可被修改。这就意味着你如果修改字符串就会产生一个新的字符串对象。但其他对象，如数组就是可变的对象。
## 创建字符串
创建字符串实例如下：
vargreeting="Hello World!";或vargreeting:String="Hello World!";你不一定为字符串指定 String 类型，因为 Scala 编译器会自动推断出字符串的类型为 String。
## 可修改字符串String Builder 类
String 对象是不可变的，如果你需要创建一个可以修改的字符串，可以使用 String Builder 类，如下实例:
objectTest{defmain(args:Array[String]){val buf=newStringBuilder;buf+='a'buf++="bcdef"println("buf is : "+buf.toString);}}[皮皮blog](http://blog.csdn.net/pipisorry)

## String 方法
java.lang.String 中常用的方法
|序号|方法及描述|
|1|char charAt(int index)
|返回指定位置的字符
|
|2|int compareTo(Object o)
|比较字符串与对象
|
|3|int compareTo(String anotherString)
|按字典顺序比较两个字符串
|
|4|int compareToIgnoreCase(String str)
|按字典顺序比较两个字符串，不考虑大小写
|
|5|String concat(String str)
|将指定字符串连接到此字符串的结尾。 同样你也可以使用加号(+)来连接。
|
|6|boolean contentEquals(StringBuffer sb)
|将此字符串与指定的 StringBuffer 比较。
|
|7|static String copyValueOf(char[] data)
|返回指定数组中表示该字符序列的 String
|
|8|static String copyValueOf(char[] data, int offset, int count)
|返回指定数组中表示该字符序列的 String
|
|9|boolean endsWith(String suffix)
|测试此字符串是否以指定的后缀结束
|
|10|boolean equals(Object anObject)
|将此字符串与指定的对象比较
|
|11|boolean equalsIgnoreCase(String anotherString)
|将此 String 与另一个 String 比较，不考虑大小写
|
|12|byte getBytes()
|使用平台的默认字符集将此 String 编码为 byte 序列，并将结果存储到一个新的 byte 数组中
|
|13|byte[] getBytes(String charsetName
|使用指定的字符集将此 String 编码为 byte 序列，并将结果存储到一个新的 byte 数组中
|
|14|void getChars(int srcBegin, int srcEnd, char[] dst, int dstBegin)
|将字符从此字符串复制到目标字符数组
|
|15|int hashCode()
|返回此字符串的哈希码
|
|16|int indexOf(int ch)
|返回指定字符在此字符串中第一次出现处的索引
|
|17|int indexOf(int ch, int fromIndex)
|返返回在此字符串中第一次出现指定字符处的索引，从指定的索引开始搜索
|
|18|int indexOf(String str)
|返回指定子字符串在此字符串中第一次出现处的索引
|
|19|int indexOf(String str, int fromIndex)
|返回指定子字符串在此字符串中第一次出现处的索引，从指定的索引开始
|
|20|String intern()
|返回字符串对象的规范化表示形式
|
|21|int lastIndexOf(int ch)
|返回指定字符在此字符串中最后一次出现处的索引
|
|22|int lastIndexOf(int ch, int fromIndex)
|返回指定字符在此字符串中最后一次出现处的索引，从指定的索引处开始进行反向搜索
|
|23|int lastIndexOf(String str)
|返回指定子字符串在此字符串中最右边出现处的索引
|
|24|int lastIndexOf(String str, int fromIndex)
|返回指定子字符串在此字符串中最后一次出现处的索引，从指定的索引开始反向搜索
|
|25|int length()
|返回此字符串的长度
|
|26|boolean matches(String regex)
|告知此字符串是否匹配给定的正则表达式
|
|27|boolean regionMatches(boolean ignoreCase, int toffset, String other, int ooffset, int len)
|测试两个字符串区域是否相等
|
|28|boolean regionMatches(int toffset, String other, int ooffset, int len)
|测试两个字符串区域是否相等
|
|29|String replace(char oldChar, char newChar)
|返回一个新的字符串，它是通过用 newChar 替换此字符串中出现的所有 oldChar 得到的
|
|30|String replaceAll(String regex, String replacement
|使用给定的 replacement 替换此字符串所有匹配给定的正则表达式的子字符串
|
|31|String replaceFirst(String regex, String replacement)
|使用给定的 replacement 替换此字符串匹配给定的正则表达式的第一个子字符串
|
|32|String[] split(String regex)
|根据给定正则表达式的匹配拆分此字符串
|
|33|String[] split(String regex, int limit)
|根据匹配给定的正则表达式来拆分此字符串
|
|34|boolean startsWith(String prefix)
|测试此字符串是否以指定的前缀开始
|
|35|boolean startsWith(String prefix, int toffset)
|测试此字符串从指定索引开始的子字符串是否以指定前缀开始。
|
|36|CharSequence subSequence(int beginIndex, int endIndex)
|返回一个新的字符序列，它是此序列的一个子序列
|
|37|String substring(int beginIndex)
|返回一个新的字符串，它是此字符串的一个子字符串
|
|38|String substring(int beginIndex, int endIndex)
|返回一个新字符串，它是此字符串的一个子字符串
|
|39|char[] toCharArray()
|将此字符串转换为一个新的字符数组
|
|40|String toLowerCase()
|使用默认语言环境的规则将此 String 中的所有字符都转换为小写
|
|41|String toLowerCase(Locale locale)
|使用给定 Locale 的规则将此 String 中的所有字符都转换为小写
|
|42|String toString()
|返回此对象本身（它已经是一个字符串！）
|
|43|String toUpperCase()
|使用默认语言环境的规则将此 String 中的所有字符都转换为大写
|
|44|String toUpperCase(Locale locale)
|使用给定 Locale 的规则将此 String 中的所有字符都转换为大写
|
|45|String trim()
|删除指定字符串的首尾空白符
|
|46|static String valueOf(primitive data type x)
|返回指定类型参数的字符串表示形式
|
（注：引用自http://blog.csdn.net/pipisorry/article/details/52902348）

4.转义字符用什么符号？

### Scala 转义字符
下表列出了常见的转义字符：
|转义字符|Unicode|描述|
|\b|\u0008|退格(BS) ，将当前位置移到前一列|
|\t|\u0009|水平制表(HT) （跳到下一个TAB位置）|
|\n|\u000a|换行(LF) ，将当前位置移到下一行开头|
|\f|\u000c|换页(FF)，将当前位置移到下页开头|
|\r|\u000d|回车(CR) ，将当前位置移到本行开头|
|\"|\u0022|代表一个双引号(")字符|
|\'|\u0027|代表一个单引号（'）字符|
|\\|\u005c|代表一个反斜线字符 '\'|
0 到 255 间的 Unicode 字符可以用一个八进制转义序列来表示，即反斜线‟\‟后跟 最多三个八进制。
在字符或字符串中，反斜线和后面的字符序列不能构成一个合法的转义序列将会导致 编译错误。
以下实例演示了一些转义字符的使用：
objectTest{defmain(args:Array[String]){println("Hello\tWorld\n\n");}}$ scalacTest.scala
$ scalaTestHelloWorld（注：引用自菜鸟教程）

5.IF...ELSE 语法是什么？
if(布尔表达式){
// 如果布尔表达式为 true 则执行该语句块
}else{
// 如果布尔表达式为 false 则执行该语句块
}

6.循环语句哪三种，分别语法是什么？怎样退出循环？
while(condition)
{
statement(s);
}
在这里，statement(s) 可以是一个单独的语句，也可以是几个语句组成的代码块。
condition 可以是任意的表达式，当为任意非零值时都为 true。当条件为 true 时执行循环。 当条件为 false 时，退出循环，程序流将继续执行紧接着循环的下一条语句。

do {
statement(s);
} while( condition );

for( var x <- Range ){
statement(s);
}
以上语法中，Range 可以是一个数字区间表示 i to j ，或者 i until j。左箭头 <- 用于为变量 x 赋值。
for( var x <- List ){
statement(s);
}
以上语法中， List 变量是一个集合，for 循环会迭代所有集合的元素。
for( var x <- List
if condition1; if condition2...
){
statement(s);
}
以上是在 for 循环中使用过滤器的语法。
var retVal = for{ var x <- List
if condition1; if condition2...
}yield x
你可以将 for 循环的返回值作为一个变量存储。
大括号中用于保存变量和条件，retVal 是变量， 循环中的 yield 会把当前的元素记下来，保存在集合中，循环结束后将返回该集合。

当在循环中使用 break 语句，在执行到该语句时，就会中断循环并执行循环体之后的代码块。
// 导入以下包
import scala.util.control._
// 创建 Breaks 对象
val loop = new Breaks;
// 在 breakable 中循环
loop.breakable{
// 循环
for(...){
....
// 循环中断
loop.break;
}
}

7.函数中 Unit是什么意思？
Scala中的Unit类型类似于java中的void，无返回值。主要的不同是在Scala中可以有一个Unit类型值，也就是（），然而java中是没有void类型的值的。除了这一点，Unit和void是等效的。一般来说每一个返回void的java方法对应一个返回Unit的Scala方法。

8.Scala怎样定义一个不带入参，不返回值的函数
def functionName  = {
function body
return [expr]
}

9.Scala怎样定义一个带入参，返回值的函数
def functionName ([参数列表]) : [return type] = {
function body
return [expr]
}

10.什么是闭包？（******************）
闭包是一个函数，返回值依赖于声明在函数外部的一个或多个变量。
var factor = 3
val multiplier = (i:Int) => i * factor

闭包的实质就是代码与用到的非局部变量的混合，即：
闭包 = 代码 + 用到的非局部变量
11.val a = 10，怎样将a转为double类型、String类型？
a.toString
a.toDouble

12.Scala函数中是把方法体的最后一行作为返回值，需不需要显示调用return？
不需要

13.怎样定义一个字符串数组？下标是从1开始的吗？
从0开始val numArr = new Array[Int](10)
从0开始val a=Array("a","b")

14.1 to 10 ==> 1.to(10)，10包含不包含？
包含

15.Range(1, 10)，10包含不包含？for( a <- 1 until 10){ println( "Value of a: " + a );  }，10包含不包含？
都不包含

16.Scala 模式匹配语法是什么？
def matchTest(x: Int): String = x match {
case 1 => "one"
}


17.异常报错的语法？
import java.io.FileReader
import java.io.FileNotFoundException
import java.io.IOException
object Test {
def main(args: Array[String]) {
try {
val f = new FileReader("input.txt")
} catch {
case ex: FileNotFoundException => {
println("Missing file exception")
}
case ex: IOException => {
println("IO Exception")
}
} finally {
println("Exiting finally...")
}
}
}

18.Array、ArrayBuffer，谁是定长？谁是变长？
Array是定长、ArrayBuffer是变长

19.什么是隐式转换函数？什么场景下用？怎样定义？
我们经常引入第三方库，但当我们想要扩展新功能的时候通常是很不方便的，因为我们不能直接修改其代码。scala提供了隐式转换机制和隐式参数帮我们解决诸如这样的问题。
implicit def file2Array(file: File): Array[String] = file.lines

20.Scala面向对象,三大特性是什么？什么区别？
继承：父和子的关系
封装：属性、方法
多态：父类引用指向子类对象

21.Scala 基本语法需要注意点？
1.类名 - 对于所有的类名的第一个字母要大写吗？
是的 class MyFirstScalaClass
2.方法名称 - 所有的方法名称的第一个字母用小写吗？
是的 def myMethodName()

22.对象是什么？类是什么？怎样在IDEA创建文件？
类是对象的抽象，而对象是类的具体实例。类是抽象的，不占用内存，而对象是具体的，占用存储空间。类是用于创建对象的蓝图，它是一个定义包括在特定类型的对象中的方法和变量的软件模板。
new->scala class

23.变长数组ArrayBuffer的系列问题
import scala.collection.mutable.ArrayBuffer
var c = new ArrayBuffer[Int]();
1. 在尾部添加一个元素
c += 2
2. 在尾部添加多个元素
c += (3,4,5)
3. 追加集合
c ++= Array(6,7,8,9)
4. 指定位置添加元素
c.insert(3, 33)  //在下标3之前插入元素
5. 移除尾部n个元素
c.trimEnd(n)
6. 移除开头n个元素
c.trimStart(n)
7. 移除某个位置的元素
c.remove(3)
8. 移除从下标为n开始（包括n）的count个元素
c.remove(n, count)
9. ArrayBuffer 转 Array
c.toArray
10. Array 转 ArrayBuffer
c.toBuffer

【来自@若泽大数据】

