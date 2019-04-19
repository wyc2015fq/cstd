# Scala 数据类型 - Simple 专栏 - CSDN博客
2018年09月14日 16:59:01[Simple_Zz](https://me.csdn.net/love284969214)阅读数：96标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala与Java具有相同的数据类型，具有相同的内存占用和精度。以下是提供Scala中可用的所有数据类型的详细信息的表格：
|序号|数据类型|说明|
|----|----|----|
|1|Byte|`8`位有符号值，范围从`-128`至`127`|
|2|Short|`16`位有符号值，范围从`-32768`至`32767`|
|3|Int|`32`位有符号值，范围从`-2147483648`至`2147483647`|
|4|Long|`64`位有符号值，范围从`-9223372036854775808`至`9223372036854775807`|
|5|Float|`32`位IEEE 754单精度浮点值|
|6|Double|`64`位IEEE 754双精度浮点值|
|7|Char|`16`位无符号`Unicode`字符。范围从`U+0000`到`U+FFFF`|
|8|String|一个`Char`类型序列|
|9|Boolean|文字值`true`或文字值`false`|
|10|Unit|对应于无值|
|11|Null|`null`或空引用|
|12|Nothing|每种其他类型的亚型; 不包括无值|
|13|Any|任何类型的超类型; 任何对象的类型为`Any`|
|14|AnyRef|任何引用类型的超类型|
上面列出的所有数据类型都是对象。Scala中没有类似Java中那样的原始类型。 这意味着您可以调用`Int`，`Long`等方法。
## Scala基本文字
Scala用于文字的规则是简单直观的，本节介绍所有基本的Scala文字。
**集成文字**
整数文字通常为`Int`类型，或`Long`类型，后跟`L`或`l`后缀。这里有一些整数文字 -
```
0
035
21 
0xFFFFFFFF 
0777L
```
**浮点文字**
浮点文字的类型为`Float`，后跟浮点类型后缀`F`或`f`，否则为`Double`类型。 这里有一些浮点文字 -
```
0.0 
1e30f 
3.14159f 
1.0e100
.1
```
**布尔文字**
布尔文字：`true`和`false`是`Boolean`类型的成员。
**符号文字**
符号文字`'x`是表达式`scala.Symbol(“x”)`的缩写。符号是一个`case`类，其定义如下 -
```
ackage scala
final case class Symbol private (name: String) {
   override def toString: String = "'" + name
}
```
**字符文字**
字符文字是用引号括起来的单个字符。字符是可打印的Unicode字符或由转义序列描述。 这里有一些字符文字 -
```
'a' 
'\u0041'
'\n'
'\t'
```
**字符串文字**
字符串文字是双引号中的一系列字符。字符是可打印的Unicode字符或由转义序列描述。 这里有一些字符串文字 -
```
"Hello,\nWorld!"
"This string contains a \" character."
```
**多行字符串**
多行字符串文字是用三个引号`""" ... """`括起来的一系列字符。字符序列是任意的，除了它最终可能包含三个或更多个连续的引号。
字符不一定是可打印的; 换行符或其他控制字符也是允许的。这是一个多行字符串文字 -
```
"""the present string
spans three
lines."""
```
**Null值**
`null`值的类型为`scala.Null`，因此与每个引用类型兼容。它表示引用一个特殊的`“null”`对象的参考值。
## 转义序列
以下转义序列在字符和字符串文字中被识别。
Unicode为`0`到`255`之间的字符也可以用八进制转义来表示，即反斜杠`“\”`后面是一个最多三个八进制字符的序列。以下是显示几个转义序列字符的示例 -
**示例**
```
object Test {
   def main(args: Array[String]) {
      println("Hello\tWorld\n\n" );
   }
}
```
当上述代码被编译和执行时，它产生以下结果 -
```
Hello   World
```
