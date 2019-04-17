# Google Java风格指南 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:41:37[boonya](https://me.csdn.net/boonya)阅读数：742








原文地址：[https://google.github.io/styleguide/javaguide.html](https://google.github.io/styleguide/javaguide.html)


GIthub上GoogleCode风格的配置文件（支持Eclipse IDE和IntelliJ IDE）：[https://github.com/codeset/google-java-styleguide](https://github.com/codeset/google-java-styleguide)

## 1简介![](https://google.github.io/styleguide/include/link.png)



本文档作为Java编程语言中源代码的Google编码标准的**完整**定义。当且仅当它遵守本文中的规则时，Java源文件被描述为*在Google Style*中。

与其他编程风格指南一样，所涵盖的问题不仅包括格式化的美学问题，也包括其他类型的约定或编码标准。但是，本文档主要关注我们普遍遵循**的快速规则** ，并避免提供不可明确强制执行的建议（无论是通过人工还是工具）。

### 1.1术语注释![](https://google.github.io/styleguide/include/link.png)

在本文件中，除非另有说明：
- 术语*类*被包含性地用于表示“普通”类，枚举类，接口或注释类型（ 
```java
@interface
```
 ）。
- 术语*成员* （类的）包含地用于表示嵌套类，字段，方法*或构造函数* ;即除了初始化器和注释之外的类的所有顶级内容。
- 术语*注释*总是指实现注释。
我们不使用短语“文档注释”，而是使用通用术语“Javadoc”。

其他“术语说明”将偶尔出现在整个文档中。

### 1.2指南说明![](https://google.github.io/styleguide/include/link.png)

本文档中的示例代码**是非规范的** 。
也就是说，虽然示例是在Google风格，但它们可能不会说明代表代码的*唯一*时尚的方式。
示例中所做的可选格式选择不应作为规则强制执行。

## 2源文件基础![](https://google.github.io/styleguide/include/link.png)

### 2.1文件名![](https://google.github.io/styleguide/include/link.png)

源文件名由其包含的顶级类（其中包含[正好一个](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s3.4.1-one-top-level-class)
 ）的区分大小写的名称，以及`.java`扩展名组成。

### 2.2文件编码：UTF-8![](https://google.github.io/styleguide/include/link.png)

源文件以**UTF-8**编码。

### 2.3特殊字符![](https://google.github.io/styleguide/include/link.png)

#### 2.3.1空格字符![](https://google.github.io/styleguide/include/link.png)

除了行终止符序列， **ASCII水平空格字符** （ **0x20** ）是在源文件中任何位置出现的唯一的空格字符。这意味着：
- 字符串和字符文字中的所有其他空白字符都将被转义。
- 制表符**不**用于缩进。

#### 2.3.2特殊转义序列![](https://google.github.io/styleguide/include/link.png)

对于具有[特殊转义序列](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://docs.oracle.com/javase/tutorial/java/data/characters.html&usg=ALkJrhh7YgBXd634GIhNA7Jby1Rq8GHj9A)
 （ 
```java
\b
```
 ，
```java
\t
```
 ，
```java
\n
```

 ，
```java
\f
```
 ，
```java
\r
```
 ，
```java
\"
```

 ，
```java
\'
```
和
```java
\\
```
 ）的任何字符，使用该序列而不是相应的八进制Unicode（例如`\u000a`
 ）转义。

#### 2.3.3非ASCII字符![](https://google.github.io/styleguide/include/link.png)

对于剩余的非ASCII字符，使用实际的Unicode字符（例如
```java
∞
```
 ）或等效的Unicode转义（例如
```java
\u221e
```

 ）。选择仅取决于哪些使得代码**更容易阅读和理解** ，尽管Unicode转义字符串字面值和注释强烈阻止。

**提示：**在Unicode脱机情况下，偶尔即使使用实际的Unicode字符，解释性注释也会非常有用。

例子：
|例|讨论|
|----|----|
|```javaString unitAbbrev="μs";```|最好：完全清楚，即使没有评论。|
|```javaString unitAbbrev="\u03bcs";// "μs"```|允许，但没有理由这样做。|
|```javaString unitAbbrev="\u03bcs";// Greek letter mu, "s"```|允许，但尴尬，容易犯错误。|
|`String unitAbbrev = "\u03bcs";`|差：读者不知道这是什么。|
|```javareturn'\ufeff'+ content;// byte order mark```|良好：对不可打印字符使用转义，如有必要，请注释。|

**提示：**不要因为某些程序可能无法正确处理非ASCII字符而使您的代码变得不可读。如果这应该发生，那些程序被**打破** ，它们必须是**固定的** 。

## 3源文件结构![](https://google.github.io/styleguide/include/link.png)


源文件由以下**顺序组成** ：
- 许可或版权信息（如果存在）
- 软件包语句
- 导入语句
- 完全一个顶级类


**正好一个空白线**分隔存在的每个部分。

### 3.1许可或版权信息（如果存在）![](https://google.github.io/styleguide/include/link.png)

如果许可证或版权信息属于文件，则它属于此处。

### 3.2包装声明![](https://google.github.io/styleguide/include/link.png)

包语句**不是线包装** 。
列限制（第4.4节， [列限制：100](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.4-column-limit) ）不适用于包语句。

### 3.3导入语句![](https://google.github.io/styleguide/include/link.png)

#### 3.3.1无通配符导入![](https://google.github.io/styleguide/include/link.png)

**不使用**静态或其他方式的**通配符导入** 。

#### 3.3.2没有线包装![](https://google.github.io/styleguide/include/link.png)

导入语句**不是线包装的** 。
列限制（第4.4节， [列限制：100](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.4-column-limit) ）不适用于import语句。

#### 3.3.3订购和间距![](https://google.github.io/styleguide/include/link.png)

导入顺序如下：
- 在单个块中的所有静态导入。
- 所有非静态导入在单个块中。

如果存在静态和非静态导入，则单个空白行分隔两个块。
import语句之间没有其他空行。

在每个块中，导入的名称以ASCII排序顺序显示。
（ **注意：**这与以ASCII排序顺序的import *语句不同* ，因为'。'排在';'前面）。

#### 3.3.4没有类的静态导入![](https://google.github.io/styleguide/include/link.png)

静态导入不用于静态嵌套类。它们以正常进口进口。

### 3.4类声明![](https://google.github.io/styleguide/include/link.png)

#### 3.4.1完全一个顶级类声明![](https://google.github.io/styleguide/include/link.png)

每个顶级类都驻留在自己的源文件中。

#### 3.4.2类内容的排序![](https://google.github.io/styleguide/include/link.png)

为类的成员和初始化器选择的顺序可以对可学习性有很大的影响。
然而，没有一个正确的方法来做到这一点;不同的类可以以不同的方式对其内容进行排序。

重要的是每个类使用***一些*逻辑顺序** ，维护者可以解释如果问。例如，新方法不仅习惯性地添加到类的末尾，因为这将产生“按照添加日期的顺序”排序，这不是逻辑排序。

##### 3.4.2.1重载：从不分裂

当一个类有多个构造函数或者多个同名的方法时，这些顺序出现，中间没有其他代码（甚至不是私有成员）。

## 4格式化![](https://google.github.io/styleguide/include/link.png)

**术语注意：***块状构造*指的是类，方法或构造函数的主体。注意，通过关于[数组初始化器](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.8.3.1-array-initializers)的第4.8.3.1节，任何数组初始化器*都可以*被视为块状构造。

### 4.1大括号![](https://google.github.io/styleguide/include/link.png)

#### 4.1.1在可选时使用括号![](https://google.github.io/styleguide/include/link.png)

大括号与
```java
if
```
 ，
```java
else
```
 ，
```java
for
```

 ，
```java
do
```
和
```java
while
```
语句一起使用，即使主体为空或仅包含单个语句。

#### 4.1.2非空块：K＆R风格![](https://google.github.io/styleguide/include/link.png)

大括号遵循Kernighan和Ritchie风格（“ [埃及括号](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://www.codinghorror.com/blog/2012/07/new-programming-jargon.html&usg=ALkJrhjyXVsI4RxrfRB-52FLcxSJoZVBsA) ”）用于*非空*块和块状结构：
- 在大括号前没有换行符。
- 开头大括号后的换行符。
- 换行前的换行。
- 关闭大括号后的换行符， *仅当*该大括号终止语句或终止方法，构造函数或*命名*类的主体时。例如，如果后面跟着
```java
else
```
或逗号，那么大括号后面*没有*换行符。

例子：



```java
return () -> {
  while (condition()) {
    method();
  }
};

return new MyClass() {
  @Override public void method() {
    if (condition()) {
      try {
        something();
      } catch (ProblemException e) {
        recover();
      }
    } else if (otherCondition()) {
      somethingElse();
    } else {
      lastThing();
    }
  }
};
```



枚举类的一些例外在第4.8.1节“枚举类”中给出。

#### 4.1.3空块：可能简洁![](https://google.github.io/styleguide/include/link.png)

空块或块状构造可以是K＆R样式（如[第4.1.2节](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.1.2-blocks-k-r-style)所述）。或者，它可以在打开后立即关闭，在（
```java
{}
```

 ）之间没有字符或换行符，**除非**它是*多块语句的一部分* （直接包含多个块：
```java
if/else
```
或
```java
try/catch/finally
```

 ）。

例子：



```java
// This is acceptable

  void doNothing() {}

  // This is equally acceptable
  void doNothingElse() {
  }
```

```java
// This is not acceptable: No concise empty blocks in a multi-block statement
  try {
    doSomething();
  } catch (Exception e) {}
```

### 4.2块缩进：+2个空格![](https://google.github.io/styleguide/include/link.png)

每次打开新的块或块状构造时，缩进增加两个空格。
当块结束时，缩进返回到上一缩进级别。缩进级别适用于整个块中的代码和注释。
（请参见第4.1.2节“ [非空块：K＆R Style”中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.1.2-blocks-k-r-style)的示例。）

### 4.3每行一个语句![](https://google.github.io/styleguide/include/link.png)

每个语句后面都有换行符。

### 4.4列限制：100![](https://google.github.io/styleguide/include/link.png)

Java代码的列限制为100个字符。
除非如下所述，否则超过此限制的任何行都必须被换行，如第4.5节“ [换行”](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.5-line-wrapping)中所述。

**例外：**
- 无法遵守列限制的行（例如，Javadoc中的长URL或长JSNI方法引用）。
- 
```java
package
```
和
```java
import
```
语句（见第3.2节[包语句](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s3.2-package-statement)和3.3[导入语句](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s3.3-import-statements)
 ）。
- 注释中的命令行可以剪切并粘贴到shell中。

### 4.5线包装![](https://google.github.io/styleguide/include/link.png)

**术语注意：**当可能合法占用单个行的代码被分为多行时，此活动称为*行换行* 。

没有全面的，确定性的公式，显示每种情况下如何包装。
通常有几种有效的方法来换行同一段代码。

**注意：**虽然换行的典型原因是为了避免溢出列限制，即使在事实上符合列限制的代码*也可能*由作者自行决定。

**提示：**提取方法或局部变量可以解决问题，而不需要换行。

#### 4.5.1打破的地方![](https://google.github.io/styleguide/include/link.png)

线包装的主要指令是：更喜欢在**更高的语法层面**打破。
也：
- 当在*非赋值*运算符处断开一行时，中断在符号*之前* 。
（请注意，这与其他语言（例如C ++和JavaScript）在Google样式中使用的做法不同）。- 这也适用于以下“类似运算符”的符号：- 点分隔符（ 
```java
.
```
 ）
- 方法引用（::）的两个冒号
- 类型边界中的
```java
<TextendsFoo&Bar>
```
符号（
```java
<TextendsFoo&Bar>
```

 ）
- 在catch块中的一个管道（ 
```java
catch (FooException|BarException e)
```

 ）。


- 当一个行在*赋值*操作符中断时*，*中断通常出现*在*符号*之后* ，但是任何一种方式都是可以接受的。- 这也适用于增强型
```java
for
```
 （“foreach”）语句中的“赋值运算符”冒号。

- 方法或构造函数名称附加到它后面的开括号（ 
```java
(
```
 ））。
- 逗号（ 
```java
,
```
 ）保持附加到它前面的令牌。
- 在lambda中，一行从不会被折断，除了如果lambda的主体由单个非支持表达式组成，则可能会在箭头之后立即出现中断。例子：
- 
```java
MyLambda<String, Long, Object> lambda =
    (String label, Long value, Object obj) -> {
        ...
    };

Predicate<String> predicate = str ->
    longExpressionInvolving(str);
```



**注意：**换行的主要目标是具有清除代码， *不一定*代码适合最小的行数。

#### 4.5.2缩进连续线至少+4个空格![](https://google.github.io/styleguide/include/link.png)

当换行时，第一行（每个*连续行* ）后面的每一*行*从原始行缩进至少+4。

当存在多个连续线时，根据需要，压痕可以变化超过+4。
一般来说，当且仅当它们以句法并行元素开头时，两个连续行使用相同的缩进级别。

关于[水平对齐的](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.6.3-horizontal-alignment)第4.6.3节阐述了使用可变数量的空间将某些令牌与先前的行对齐的不鼓励做法。

### 4.6空白![](https://google.github.io/styleguide/include/link.png)

#### 4.6.1垂直空白![](https://google.github.io/styleguide/include/link.png)

将显示一个空白行：
- *在类的*连续成员或初始化器之间：字段，构造函数，方法，嵌套类，静态初始化器和实例初始化器。- **异常：**两个连续字段之间的空行（它们之间没有其他代码）是可选的。这样的空白行根据需要用于创建字段的*逻辑分组* 。
- **异常：**枚举常量之间的空行在[第4.8.1节](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.8.1-enum-classes)中[介绍](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.8.1-enum-classes)
 。

- 在语句之间， *根据需要*将代码组织成逻辑子部分。
- *可选地*在第一个成员或初始化*程序*之前，或在类的最后一个成员或初始化程序之后（既不鼓励也不鼓励）。
- 根据本文档其他部分的要求（如第3节， [源文件结构](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s3-source-file-structure)和第3.3节， [导入语句](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s3.3-import-statements) ）。

允许*多个*连续的空行，但不需要（或鼓励）。

#### 4.6.2水平空白![](https://google.github.io/styleguide/include/link.png)

除了语言或其他样式规则所要求的之外，除了文字，注释和Javadoc之外，单个ASCII空间也**仅**出现在以下位置。
- 将任何保留字（例如
```java
if
```
 ，
```java
for
```
或
```java
catch
```

 ）从该行后面的开括号（
```java
(
```
 ））中分离
- 将任何保留字（例如
```java
else
```
或
```java
catch
```
 ）与该行上的关闭大括号（
```java
}
```

 ）分隔开
- 在任何打开的大括号（ 
```java
{
```
 ）之前，有两个例外：- 
```java
@SomeAnnotation({a, b})
```

 （不使用空格）
- 
```java
String[][] x={{"foo"}};
```
（
```java
{{
```
下面第8项）之间不需要空格，

- 在任何二进制或三元运算符的两侧。这也适用于以下“类似运算符”的符号：- 连接符类型的连接
```java
<TextendsFoo&Bar>
```

 ：
```java
<TextendsFoo&Bar>
```

- 用于处理多个异常的catch块的管道： 
```java
catch (FooException|BarException e)
```

- 冒号（：）在增强型
```java
for
```
 （“foreach”）语句中
- lambda表达式中的箭头： 
```java
(String str)-> str.length()
```

但不是- 一个方法引用的两个冒号（::），它写得像
```java
Object::toString
```

- 点分隔符（ 
```java
.
```
 ），它写作
```java
object.toString()
```


- 后
```java
,:;
```
或右括号（
```java
)
```
 ）
- 在开始结束行注释的双斜线（ 
```java
//
```
 ）的两侧。这里，允许多个空格，但不是必需的。
- 在一个声明的类型和变量之间： 
```java
List<String> list
```

- *可选*只在数组初始值设定符的两个大括号内- 
```java
newint[]{5,6}
```
和
```java
newint[]{5,6}
```
都是有效的

此规则从不解释为在行的开始或结束处要求或禁止额外的空间;
它只涉及*内部*空间。
#### 4.6.3水平对齐：从不需要![](https://google.github.io/styleguide/include/link.png)

**术语注意：***水平对齐*是在代码中添加可变数量的附加空间的做法，目的是使某些令牌直接显示在之前行上的某些其他令牌下方。

这种做法是允许的，但Google风格**从不需要** 。
甚至不需要在已经使用的地方*保持*水平对准。

这里是一个没有对齐的例子，然后使用对齐：



```java
private int x; // this is fine
private Color color; // this too

private int   x;      // permitted, but future edits
private Color color;  // may leave it unaligned
```

**提示：**对齐可以帮助可读性，但它会为以后的维护带来问题。考虑一个需要触摸只有一行的未来变化。这种变化可能会使以前令人愉快的格式化失败，这是**允许的** 。更频繁地，它提示编码器（也许你）调整附近线上的空白，可能触发级联系列的重新格式化。那一行的变化现在有一个“爆炸半径”。这可能在最坏的情况下导致无意义的繁忙工作，但最多仍会破坏版本历史信息，减慢审阅者和加剧合并冲突。
### 4.7分组括号：推荐![](https://google.github.io/styleguide/include/link.png)

可选的分组括号只有在作者和审稿人同意没有合理的机会，没有他们的代码将被误解，他们也不会使代码更容易阅读。假设每个读取器都具有存储的整个Java运算符优先级表是不合理的。

### 4.8特定构建体![](https://google.github.io/styleguide/include/link.png)

#### 4.8.1枚举类![](https://google.github.io/styleguide/include/link.png)

在遵循枚举常量的每个逗号后，换行符是可选的。
还允许附加空行（通常只有一个）。这是一个可能性：




```java
private enum Answer {

  YES {
    @Override public String toString() {
      return "yes";
    }
  },

  NO,
  MAYBE
}
```

没有方法和没有文档的常量的枚举类可以可选地被格式化为数组初始化器（见关于[数组初始化器的](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.8.3.1-array-initializers)第4.8.3.1节）。


```java
privete enum Clothes{CLUBS，HEARTS，SPADES，DIAMONDS}
```

由于枚举类*是类* ，因此适用于格式化类的所有其他规则。
#### 4.8.2变量声明![](https://google.github.io/styleguide/include/link.png)

##### 4.8.2.1每个声明一个变量

每个变量声明（字段或本地）只声明一个变量：声明如`int a, b;`不使用。

##### 4.8.2.2需要时声明

局部变量不习惯地在它们的包含块或块状构造的开始处声明。
相反，局部变量被声明为接近它们首次使用的点（在原因内），以最小化它们的范围。局部变量声明通常具有初始化器，或者在声明之后立即初始化。

#### 4.8.3数组![](https://google.github.io/styleguide/include/link.png)

##### 4.8.3.1数组初始化器：可以是“块状”

任何数组初始化器可以*可选*地被格式化为好像它是“块状构造”。
例如，以下都是有效的（ **不是**详尽的列表）：



```java
new int[] {           new int[] {
  0, 1, 2, 3            0,
}                       1,
                        2,
new int[] {             3,
  0, 1,               }
  2, 3
}                     new int[]
                          {0, 1, 2, 3}
```

##### 4.8.3.2没有C样式数组声明

方括号形成*类型*的一部分，而不是变量： 
```java
String[] args
```
 ，而不是`String args[]` 。

#### 4.8.4切换语句![](https://google.github.io/styleguide/include/link.png)

**术语注意：***交换机块*的大括号内是一个或多个*语句组* 。每个语句组由一个或多个*开关标签* （无论是
```java
case
 FOO:
```
或
```java
default:
```
 ：）组成，后跟一个或多个语句。

##### 4.8.4.1缩进

与任何其他块一样，开关块的内容缩进+2。

在切换标签之后，有一个换行符，缩进级别增加+2，就像打开一个块一样。
以下开关标签返回到上一缩进级别，如同一个块已关闭。

##### 4.8.4.2通过：评论

在开关块内，每个语句组都会突然终止（使用
```java
break
```
 ，
```java
continue
```
 ，
```java
return
```
或thrown异常），或者标记有注释，指示执行将继续或*可能*继续到下一个语句组。任何传达堕落想法的评论都是足够的（通常
```java
//
 fall through
```
 ）。在开关组的最后一个语句组中不需要此特殊注释。例：



```java
switch (input) {
  case 1:
  case 2:
    prepareOneOrTwo();
    // fall through
  case 3:
    handleOneTwoOrThree();
    break;
  default:
    handleLargeNumber(input);
}
```


注意，在
```java
case1:
```
之后不需要注释，只在语句组的末尾。

##### 4.8.4.3 `default`情况存在

每个switch语句包括一个
```java
default
```
语句组，即使它不包含代码。

#### 4.8.5注释![](https://google.github.io/styleguide/include/link.png)

应用于类，方法或构造函数的注释立即出现在文档块之后，并且每个注释都列在其自己的行（即每行一个注释）上。这些换行符不构成换行（第4.5节，[换行](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s4.5-line-wrapping)
 ），因此缩进级别不会增加。例：



```java
@Override
@Nullable
public String getNameIfPresent() { ... }
```

**异常：***单个*无参数注释*可以*改为与签名的第一行一起出现，例如：



```java
@Override public int hashCode() { ... }
```


应用于字段的注释也会立即出现在文档块之后，但在这种情况下， *多个*注释（可能已参数化）可能会列在同一行上;例如：


```java
@Partial @Mock DataLoader loader;
```


没有关于参数，局部变量或类型的注释格式化的具体规则。 

#### 4.8.6评论![](https://google.github.io/styleguide/include/link.png)

本节讨论*实现注释* 。
Javadoc在第7节Javadoc中单独解决。

任何换行符之前可以有任意空格，然后是实现注释。
这样的注释使该行非空白。

##### 4.8.6.1块注释样式

块注释缩进到与周围代码相同的级别。它们可以是
```java
/* ... */
```
 style或
```java
//
 ...
```
样式。对于多行
```java
/* ... */
```
注释，后续行必须以`*`与上一行的`*`对齐开始。



```java
/*
 * This is          // And so           /* Or you can
 * okay.            // is this.          * even do this. */
 */
```


注释不包含在用星号或其他字符绘制的框中。 

**提示：**在编写多行注释时，如果您希望自动代码格式化程序在必要时重新换行（段落样式），请使用
```java
/* ... */
```
样式。大多数格式化程序不会在
```java
//
 ...
```
样式注释块中重新换行。

#### 4.8.7修饰符![](https://google.github.io/styleguide/include/link.png)

类和成员修饰符（如果存在）按照Java语言规范建议的顺序显示：



```java
public protected private abstract default static final transient volatile synchronized native strictfp
```

#### 4.8.8数字文字![](https://google.github.io/styleguide/include/link.png)

`long`整数文字使用大写`L`后缀，从不小写（以避免与数字`1`混淆）。例如，`3000000000L`而不是`3000000000l` 。

## 5命名![](https://google.github.io/styleguide/include/link.png)

### 5.1所有标识符共有的规则![](https://google.github.io/styleguide/include/link.png)

标识符只使用ASCII字母和数字，并且在下面指出的少数情况下，下划线。
因此，每个有效的标识符名称由正则表达式`\w+`匹配。

在Google Style中，特殊的前缀或后缀，如在示例`name_` ，`mName` ，`s_name`和`kName`中看到的，**不**使用。

### 5.2按标识符类型划分的规则![](https://google.github.io/styleguide/include/link.png)

#### 5.2.1包名称![](https://google.github.io/styleguide/include/link.png)

包名称都是小写，连续的单词连接在一起（无下划线）。
例如， `com.example.deepspace` ，而不是`com.example.deepSpace`或`com.example.deep_space` 。

#### 5.2.2类名![](https://google.github.io/styleguide/include/link.png)

类名写在[UpperCamelCase中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s5.3-camel-case)
 。

类名通常是名词或名词短语。例如， 
```java
Character
```
或
```java
ImmutableList
```
 。接口名称也可以是名词或名词短语（例如，
```java
List
```

 ），但有时可以是形容词或形容词短语（例如，
```java
Readable
```
 ）。

没有特定的规则，甚至没有成熟的惯例为命名注释类型。

*测试*类从它们正在测试的类的名称开始命名，并以
```java
Test
```
结束。例如，
```java
HashTest
```
或
```java
HashIntegrationTest
```

 。

#### 5.2.3方法名称![](https://google.github.io/styleguide/include/link.png)

方法名称写在[lowerCamelCase中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s5.3-camel-case)
 。

方法名称通常是动词或动词短语。例如，
```java
sendMessage
```
或
```java
stop
```

 。

下划线可能出现在JUnit *测试*方法名称中以分隔名称的逻辑组件。
一个典型的模式是`test *<MethodUnderTest>* _ *<state>*` ，例如
```java
testPop_emptyStack
```
 。没有一个正确的方法来命名测试方法。

#### 5.2.4常量名称![](https://google.github.io/styleguide/include/link.png)

常量名称使用
```java
CONSTANT_CASE
```
 ：所有大写字母，用下划线分隔的单词。但什么*是*常数，究竟是什么？

常数是静态最终字段，其内容是不可变的，并且其方法没有可检测的副作用。
这包括基元，字符串，不可变类型和不可变类型的不可变集合。如果任何实例的observable状态可以改变，它不是一个常量。只是*打算*从来不改变对象是不够的。例子：



```java
// Constants
static final int NUMBER = 5;
static final ImmutableList<String> NAMES = ImmutableList.of("Ed", "Ann");
static final ImmutableMap<String, Integer> AGES = ImmutableMap.of("Ed", 35, "Ann", 32);
static final Joiner COMMA_JOINER = Joiner.on(','); // because Joiner is immutable
static final SomeMutableType[] EMPTY_ARRAY = {};
enum SomeEnum { ENUM_CONSTANT }

// Not constants
static String nonFinal = "non-final";
final String nonStatic = "non-static";
static final Set<String> mutableCollection = new HashSet<String>();
static final ImmutableSet<SomeMutableType> mutableElements = ImmutableSet.of(mutable);
static final ImmutableMap<String, SomeMutableType> mutableValues =
    ImmutableMap.of("Ed", mutableInstance, "Ann", mutableInstance2);
static final Logger logger = Logger.getLogger(MyClass.getName());
static final String[] nonEmptyArray = {"these", "can", "change"};
```


这些名称通常是名词或名词短语。 

#### 5.2.5非常量字段名称![](https://google.github.io/styleguide/include/link.png)

非常量字段名（静态或其他）写在[lowerCamelCase中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s5.3-camel-case)
 。

这些名称通常是名词或名词短语。例如，
```java
computedValues
```
或
```java
index
```

 。

#### 5.2.6参数名称![](https://google.github.io/styleguide/include/link.png)

参数名称写在[lowerCamelCase中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s5.3-camel-case)
 。

应该避免公共方法中的单字符参数名称。

#### 5.2.7局部变量名![](https://google.github.io/styleguide/include/link.png)

局部变量名写在[lowerCamelCase中](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s5.3-camel-case)
 。

即使最终和不可变的局部变量不被认为是常量，并且不应该被定型为常量。

#### 5.2.8类型变量名![](https://google.github.io/styleguide/include/link.png)

每个类型变量都以两种样式之一命名：
- 单个大写字母，可选后跟单个数字（例如
```java
E
```
 ，
```java
T
```
 ，
```java
X
```

 ，
```java
T2
```
 ）
- 用于类的形式的名称（见第5.2.2节， [类名](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s5.2.2-class-names) ），后跟大写字母
```java
T
```
 （示例：
```java
RequestT
```
 ，
```java
FooBarT
```

 ）。

### 5.3骆驼情况：定义![](https://google.github.io/styleguide/include/link.png)

有时，有一个以上的合理方式将英语短语转换为骆驼情况，例如当首字母缩略词或不寻常的结构，如“IPv6”或“iOS”存在。为了提高可预测性，Google Style指定以下（几乎）确定性方案。

从名称的散文形式开始：
- 将短语转换为纯ASCII并删除任何省略号。
例如，“Müller算法”可能变成“Muellers算法”。
- 将此结果划分为单词，分割为空格和任何剩余的标点符号（通常为连字符）。- *推荐：*如果任何字词在常用的情况下已经具有常规的驼峰盒外观，请将其分为其组成部分（例如，“AdWords”成为“广告字词”）。注意，诸如“iOS”这样的词*本身*不是真正的骆驼情况;它*违反任何*惯例，因此本建议不适用。

- 现在小写*一切* （包括首字母缩略词），然后大写只有第一个字符：- ...每个字，产生*上骆驼壳* ，或
- ...每个字除了第一个，产生*较低的驼峰盒*

- 最后，将所有单词连接成单个标识符。

注意，原始单词的壳体几乎完全被忽略。
例子：
|散文形式|正确|不正确|
|----|----|----|
|“XML HTTP请求”|```javaXmlHttpRequest```|`XMLHTTPRequest`|
|“新客户ID”|```javanewCustomerId```|`newCustomerID`|
|“内部秒表”|```javainnerStopwatch```|`innerStopWatch`|
|“在iOS上支持IPv6？|```javasupportsIpv6OnIos```|`supportsIPv6OnIOS`|
|“YouTube进口商”|```javaYouTubeImporter``````javaYoutubeImporter``` *||

*可接受，但不推荐。

**注意：**某些单词在英语中有不明确的连字符：例如“nonempty”和“non-empty”都是正确的，所以方法名称
```java
checkNonempty
```
和
```java
checkNonEmpty
```
也是正确的。

## 6编程实践![](https://google.github.io/styleguide/include/link.png)

### 6.1`@Override` ：始终使用![](https://google.github.io/styleguide/include/link.png)

一个方法在合法时用
```java
@Override
```
注释标记。这包括重写超类方法的类方法，实现接口方法的类方法和重定义超接口方法的接口方法。

**异常：**当父方法为
```java
@Deprecated
```
时，可以省略
```java
@Deprecated
```

 。

### 6.2捕获异常：不被忽略![](https://google.github.io/styleguide/include/link.png)

除非如下所述，在响应捕获的异常时不执行任何操作是非常不正确的。
（典型的响应是记录它，或者如果它被认为是“不可能”，将其重新抛出为一个
```java
AssertionError
```
 。）

当在catch块中没有采取任何行动是真正合适的时候，这是合理的原因在注释中解释。



```java
try {
  int i = Integer.parseInt(response);
  return handleNumericResponse(i);
} catch (NumberFormatException ok) {
  // it's not numeric; that's fine, just continue
}
return handleTextResponse(response);
```

**异常：**在测试中，如果捕获的异常名称为或以
```java
expected
```
开始，*则*可以忽略捕获的异常而不进行注释。以下是一个非常常见的用于确保测试*中*的代码*确实*抛出了预期类型的​​异常的习语，因此在此不需要注释。


```java
try {
  emptyStack.pop();
  fail();
} catch (NoSuchElementException expected) {
}
```

### 6.3静态成员：使用类合格![](https://google.github.io/styleguide/include/link.png)

当对静态类成员的引用必须限定时，它将使用该类的名称限定，而不是该类的类型的引用或表达式。

```java

```

```java
Foo aFoo = ...;
Foo.aStaticMethod(); // good
aFoo.aStaticMethod(); // bad
somethingThatYieldsAFoo().aStaticMethod(); // very bad
```

### 6.4终化剂：未使用![](https://google.github.io/styleguide/include/link.png)

覆盖
```java
Object.finalize
```
是**非常罕见的** 。

**提示：**不要这样做。
如果你绝对必须，首先阅读并理解[*有效的Java*](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=http://books.google.com/books%3Fisbn%3D8131726592&usg=ALkJrhgjh_pkUC39ua0DRIYTIEp4QNjjKA)项目7，“避免终结者”，非常仔细，*然后*不这样做。

## 7 Javadoc![](https://google.github.io/styleguide/include/link.png)

### 7.1格式化![](https://google.github.io/styleguide/include/link.png)

#### 7.1.1一般形式![](https://google.github.io/styleguide/include/link.png)

Javadoc块的*基本*格式如下例所示：



```java
/**
 * Multiple lines of Javadoc text are written here,
 * wrapped normally...
 */
public int method(String p1) { ... }
```


...或在此单行示例中：



```java
/** An especially short bit of Javadoc. */
```


基本形式总是可以接受的。 当不存在at-clause时，单行形式可以被替换，并且Javadoc块（包括注释标记）的整体可以适合在单个行上。
#### 7.1.2段落![](https://google.github.io/styleguide/include/link.png)

一个空行（即，仅包含对齐的前导星号（ `*` ）的行）出现在段落之间，并在“at-clause”组（如果存在）之前。每个段落，但第一个在紧接第一个单词之前有`<p>`后面没有空格。

#### 7.1.3条款![](https://google.github.io/styleguide/include/link.png)

任何使用的标准“at- `@deprecated` ”都出现在`@param``@deprecated` ，`@return``@deprecated` ，
`@return` ， `@param``@deprecated`这些`@deprecated` ，并且这四种类型不会出现空描述。当at-clause不适合单个行时，连续行从`@`的位置缩进四（或更多）个空格。

### 7.2摘要片段![](https://google.github.io/styleguide/include/link.png)

每个Javadoc块以简短**摘要片段**开头。
这个片段非常重要：它是出现在某些上下文中的文本的唯一部分，例如类和方法索引。

这是一个片段 - 一个名词短语或动词短语，而不是一个完整的句子。
它不以`A {@code Foo} is a...`开头`A {@code Foo} is a...` ，或者`This method returns...` ，也不会形成一个完整的命令句，如`Save the record.`。然而，片段被大写和标点，好像它是一个完整的句子。

**提示：**常见的错误是以`/** @return the customer ID */`编写简单的Javadoc。这是不正确的，应该更改为
```java
/**
 Returns the customer ID. */
```

```java
/** Returns the customer ID. */
```
 。

### 7.3其中使用Javadoc![](https://google.github.io/styleguide/include/link.png)

*至少* ，Javadoc存在于每个
```java
public
```
类以及这种类的每个
```java
public
```
或
```java
protected
```
成员，除了下面列出的一些例外。

还可能存在其他Javadoc内容，如第7.3.4节“ [非必需Javadoc”](https://translate.googleusercontent.com/translate_c?act=url&depth=1&hl=zh-CN&ie=UTF8&prev=_t&rurl=translate.google.com.hk&sl=en&sp=nmt4&tl=zh-CN&u=https://google.github.io/styleguide/javaguide.html&usg=ALkJrhj_KEmqNv2CWlXPyQxv0hctIcljsw#s7.3.4-javadoc-non-required)中所述。

#### 7.3.1异常：自明的方法![](https://google.github.io/styleguide/include/link.png)

Javadoc对于“简单的，明显的”方法（如
```java
getFoo
```
是可选的，在*真正和真正*没有什么别的值得说，但“返回foo”。

**重要提示：**引用此例外是不恰当的，因为省略了典型读者可能需要知道的相关信息。例如，对于名为
```java
getCanonicalName
```
的方法，不要省略其文档（只有
```
/**
 Returns the canonical name. */
```
 ），如果典型的读者可能不知道术语“canonical name”手段！

#### 7.3.2异常：覆盖![](https://google.github.io/styleguide/include/link.png)

Javadoc并不总是出现在覆盖超类型方法的方法上。

#### 7.3.4非必需Javadoc![](https://google.github.io/styleguide/include/link.png)

其他类和成员*根据需要或期望*具有Javadoc。

每当实现注释用于定义类或成员的总体目的或行为时，该注释将改为写为Javadoc（使用`/**` ）。

非必需的Javadoc不是严格要求遵守第7.1.2,7.1.3和7.2节的格式化规则，但当然是建议。




