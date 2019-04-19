# Java 编程规范 [Java Code Conventions] - Simple 专栏 - CSDN博客
2012年01月04日 10:41:06[Simple_Zz](https://me.csdn.net/love284969214)阅读数：362标签：[java																[程序开发																[编程规范](https://so.csdn.net/so/search/s.do?q=编程规范&t=blog)](https://so.csdn.net/so/search/s.do?q=程序开发&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
*Oracle官方Java编码规范（英文版）*
*[http://www.oracle.com/technetwork/java/codeconvtoc-136057.html](http://www.oracle.com/technetwork/java/codeconvtoc-136057.html)*
# 1 介绍（Introduction）
## 1.1 为什么要有编码规范（Why Have Code Conventions）
编码规范对于程序员而言尤为首要，有以下几个原因：
-  一个软件的生命周期中，80％的花费在于维护
-  几乎没有任何一个软件，在其全部生命周期中，均由最初的开辟人员来维护
-  编码规范可以改良软件的可读性，可以让程序员尽快而彻底地懂得新的代码
-  若是你将源码作为产品公布，就须要确任它是否被很好的打包并且清楚无误，一如你已构建的其它任何产品
为了履行规范，每个软件开辟人员必须一致遵守编码规范。每小我。
## 1.2 版权声明（Acknowledgments）
本文档反应的是Sun MicroSystems公司，Java说话规范中的编码标准项目组。首要供献者包含：Peter King，Patrick Naughton，Mike DeMoney，Jonni Kanerva，Kathy Walrath以及Scott Hommel。
本文档现由Scott Hommel维护，有关评论定见请发至shommel＠eng.sun.com
# 2 文件名（File Names）
这项目组列出了常用的文件名及厥后缀。
## 2.1 文件后缀（File Suffixes）
Java程序应用下列文件后缀：
|文件类别|文件后缀|
|----|----|
|Java源文件|.java|
|Java字节码文件|.class|
## 2.2 常用文件名（Common File Names）
常用的文件名包含：
|文件名|用处|
|----|----|
|GNUmakefile|makefiles的首选文件名。我们采取gnumake来创建（build）软件。|
|README|概述特定目次下所含内容的文件的首选文件名|
# 3 文件组织（File Organization）
一个文件由被空行分别而成的段落以及标识每个段落的可选注释共同构成。跨越2000行的程序难以浏览，应当尽量避免。"Java源文件典范"供给了一个布局公道的Java程序典范。
## 3.1 Java源文件（Java Source Files）
每个Java源文件都包含一个单一的公共类或接口。若私有类和接口与一个公共类相接洽关系，可以将它们和公共类放入同一个源文件。公共类必须是这个文件中的第一个类或接口。
Java源文件还遵守以下规矩：
- 开首注释（拜见"开首注释"）
- 包和引入语句（拜见"包和引入语句"）
- 类和接口声明（拜见"类和接口声明"）
### 3.1.1 开首注释（Beginning Comments）
所有的源文件都应当在开首有一个C说话风格的注释，此中列出类名、版本信息、日期和版权声明：
```
/*
 * Classname
 *
 * Version information
 *
 * Date
 *
 * Copyright notice
 */
```
### 3.1.2 包和引入语句（Package and Import Statements）
在多半Java源文件中，第一个非注释行是包语句。在它之后可以跟引入语句。例如：
```
package java.awt;
 
import java.awt.peer.CanvasPeer;
```
### 3.1.3 类和接口声明（Class and Interface Declarations）
下表描述了类和接口声明的各个项目组以及它们呈现的先后次序。拜见"Java源文件典范"中一个包含注释的例子。
||类/接口声明的各项目组|注解|
|----|----|----|
|1|类/接口文档注释（/**……*/）|该注释中所需包含的信息，拜见"文档注释"|
|2|类或接口的声明||
|3|类/接话柄现的注释（/*……*/）若是有须要的话|该注释应包含任何有关全部类或接口的信息，而这些信息又不合适作为类/接口文档注释。|
|4|类的（静态）变量|起首是类的公共变量，随后是维护变量，再后是包一级此外变量（没有接见润饰符，access modifier），最后是私有变量。|
|5|实例变量|起首是公共级，随后是维护级，再后是包一级（没有接见润饰符），最后是私有级。|
|6|机关器||
|7|办法|这些办法应当按功能，而非感化域或接见权限，分组。例如，一个私有的类办法可以置于两个公有的实例办法之间。其目标是为了更便于浏览和懂得代码。|
# 4 缩进排版（Indentation）
4个空格常被作为缩进排版的一个单位。缩进的确切申明并未具体指定（空格 vs. 制表符）。一个制表符便是8个空格（而非4个）。
## 4.1 行长度（Line Length）
尽量避免一行的长度跨越80个字符，因为很多终端和对象不克不及很益处理惩罚之。
重视：用于文档中的例子应当应用更短的行长，长度一般不跨越70个字符。
## 4.2 换行（Wrapping Lines）
当一个表达式无法容纳在一行内时，可以根据如下一般规矩断开之：
-  在一个逗号后面断开
-  在一个操纵符前面断开
-  宁可选择较高等别（higher-level）的断开，而非较初级别（lower-level）的断开
-  新的一行应当与上一行同一级别表达式的开首处对齐
-  若是以上规矩导致你的代码杂沓或者使你的代码都堆挤在右边，那就代之以缩进8个空格。
以下是断创办法调用的一些例子：
```
someMethod（longExpression1， longExpression2， longExpression3，
        longExpression4， longExpression5）;
var = someMethod1（longExpression1，
                someMethod2（longExpression2，
                        longExpression3））;
```
【**译者注**】上方是SUN（Oracle）网站上html版起原根蒂根基始写法，笔者看了半天是一头雾水；对比pdf版本看，才发明是SUN的html版本写的不合错误：
亦即，新的一行在上一行同一级别表达式的开首对齐：longExpression4与longExpression1对齐；someMethod2与longExpression1对齐，longExpression3与longExpression2对齐：
```
someMethod（longExpression1， longExpression2， longExpression3，
           longExpression4， longExpression5）;
var = someMethod1（longExpression1，
                  someMethod2（longExpression2，
                              longExpression3））;
```
以下是两个断开算术表达式的例子。前者更好，因为断开处位于括号表达式的外边，这是个较高等此外断开。
```
longName1 = longName2 * （longName3 + longName4 - longName5）
           + 4 * longname6; //PREFFER
longName1 = longName2 * （longName3 + longName4
                       - longName5） + 4 * longname6; //AVOID
```
【**译者注**】上方是SUN（Oracle）网站上html版起原根蒂根基始写法，笔者看了半天是一头雾水；对比pdf版本看，才发明是SUN的html版本写的不合错误：
亦即，新的一行在上一行同一级别表达式的开首对齐：加号与longName2对齐；减号与longName3对齐（固然分行的处所不合错误，但书写还是要对齐的）。
```
longName1 = longName2 * （longName3 + longName4 - longName5）
            + 4 * longname6; //PREFFER
longName1 = longName2 * （longName3 + longName4
                         - longName5） + 4 * longname6; //AVOID
```
以下是两个缩进办法声明的例子。前者是常规景象。后者若应用常规的缩进体式格式将会使第二行和第三行移得很靠右，所以代之以缩进8个空格
```
//CONVENTIONALINDENTATION
someMethod（int anArg， Object anotherArg， String yetAnotherArg，
           Object andStillAnother） {
    ...
}
//INDENT 8 SPACESTO AVOID VERY DEEP INDENTS
private static synchronized horkingLongMethodName（int anArg，
        Object anotherArg， String yetAnotherArg，
        Object andStillAnother） {
    ...
}
```
if语句的换行凡是应用8个空格的规矩，因为常规缩进（4个空格）会使语句体看起来斗劲费劲。比如：
```
//DON’T USE THISINDENTATION
if （（condition1&& condition2）
    || （condition3 && condition4）
    ||!（condition5 && condition6）） { //BAD WRAPS
    doSomethingAboutIt（）;            //MAKE THIS LINE EASY TO MISS
}
//USE THISINDENTATION INSTEAD
if （（condition1&& condition2）
        || （condition3 && condition4）
        ||!（condition5 && condition6）） {
    doSomethingAboutIt（）;
}
//OR USE THIS
if （（condition1&& condition2） || （condition3 && condition4）
        ||!（condition5 && condition6）） {
    doSomethingAboutIt（）;
}
```
这里有三种可行的办法用于处理惩罚三元运算表达式：
```
alpha = aLongBooleanExpression） ？ beta : gamma;
alpha = （aLongBooleanExpression） ？ beta
                                 : gamma;
 
alpha = （aLongBooleanExpression）
        ？ beta
        : gamma;
```
# 5 注释（Comments）
Java程序有两类注释：实现注释（implementation comments）和文档注释（document comments）。实现注释是那些在C++中见过的，应用/*...*/和//界定的注释。文档注释（被称为"doc comments"）是Java独有的，并由/**...*/界定。文档注释可以经由过程javadoc对象转换成HTML文件。
实现注释用以注释代码或者实现细节。文档注释从实现（implementation-free）的角度描述代码的规范。它可以被那些手头没有源码的开辟人员读懂。
注释应被用来给出代码的总括，并供给代码自身没有供给的附加信息。注释应当仅包含与浏览和懂得程序有关的信息。例如，响应的包如何被建树或位于哪个目次下之类的信息不该包含在注释中。
在注释里，对设计决定计划中首要的或者不是显而易见的处所进行申明是可以的，但应避免供给代码中己清楚表达出来的反复信息。多余的的注释很轻易过期。凡是应避免那些代码更新就可能过期的注释。
重视：频繁的注释有时反应出代码的低质量。当你感觉被迫要加注释的时辰，推敲一下重写代码使其更清楚。
注释不该写在用星号或其他字符画出来的大框里。注释不该包含诸如制表符和回退符之类的特别字符。
## 5.1 实现注释的格局（Implementation Comment Formats）
程序可以有4种实现注释的风格：块（block）、单行（single-line）、尾端（trailing）和行末（end-of-line）。
### 5.1.1 块注释（Block Comments）
块注释凡是用于供给对文件，办法，数据布局和算法的描述。块注释被置于每个文件的开端处以及每个办法之前。它们也可以被用于其他处所，比如办法内部。在功能和办法内部的块注释应当和它们所描述的代码具有一样的缩进格局。
块注释之首应当有一个空行，用于把块注释和代码分别开来，比如：
```
/*
   * Here is ablock comment.
   */
```
块注释可以以/*-开首，如许indent（1）就可以将之辨认为一个代码块的开端，而不会重排它。
```
/*-
    * Here is ablock comment with some very special
    * formattingthat I want indent（1） to ignore.
    *
    *    one
    *        two
    *            three
    */
```
重视：若是你不应用indent（1），就不必在代码中应用/*-，或为他人可能对你的代码运行indent（1）作让步。
拜见"文档注释"
### 5.1.2 单行注释（Single-Line Comments）
短注释可以显示在一行内，并与厥后的代码具有一样的缩进层级。若是一个注释不克不及在一行内写完，就该采取块注释（拜见"块注释"）。单行注释之前应当有一个空行。以下是一个Java代码中单行注释的例子：
```
if （condition） {
 
    /* Handle thecondition. */
    ...
}
```
### 5.1.3 尾端注释（Trailing Comments）
极短的注释可以与它们所要描述的代码位于同一行，然则应当有足够的空白来分隔代码和注释。如有多个短注释呈现于大段代码中，它们应当具有雷同的缩进。
以下是一个Java代码中尾端注释的例子：
```
if （a == 2） {
    return TRUE;            /* special case */
} else {
    return isPrime（a）;      /* works only for odda */
}
```
### 5.1.4 行末注释（End-Of-Line Comments）
注释界定符"//"，可以注释掉整行或者一行中的一项目组。它一般不消于连气儿多行的注释文本；然而，它可以用来注释掉连气儿多行的代码段。以下是所有三种风格的例子：
```
if （foo > 1） {
 
    // Do adouble-flip.
    ...
}
else {
    return false;          // Explain why here.
}
 
//if （bar > 1）{
//
//    // Do a triple-flip.
//    ...
//}
//else {
//    return false;
//}
```
## 5.2 文档注释（Documentation Comments）
重视：此处描述的注释格局之典范，拜见"Java源文件典范"
若想懂得更多，拜见"How to Write Doc Comments for Javadoc"，此中包含了有关文档注释标识表记标帜的信息（＠return， ＠param， ＠see）：
http://java.sun.com/javadoc/writingdoccomments/index.html
若想懂得更多有关文档注释和javadoc的具体材料，拜见javadoc的主页：
http://java.sun.com/javadoc/index.html
文档注释描述Java的类、接口、机关器，办法，以及字段（field）。每个文档注释都邑被置于注释定界符/**...*/之中，一个注释对应一个类、接口或成员。该注释应位于声明之前：
```
/**
    * The Exampleclass provides ...
    */
  public classExample { ...
```
重视顶层（top-level）的类和接口是不缩进的，而其成员是缩进的。描述类和接口的文档注释的第一行（/**）不需缩进；随后的文档注释每行都缩进1格（使星号纵向对齐）。成员，包含机关函数在内，其文档注释的第一行缩进4格，随后每行都缩进5格。
若你想给出有关类、接口、变量或办法的信息，而这些信息又不合适写在文档中，则可应用实现块注释（见5.1.1）或紧跟在声明后面的单行注释（见5.1.2）。例如，有关一个类实现的细节，应放入紧跟在类声明后面的实现块注释中，而不是放在文档注释中。
文档注释不克不及放在一个办法或机关器的定义块中，因为Java会将位于文档注释之后的第一个声明与其相接洽关系。
# 6 声明（Declarations）
## 6.1 每行声明变量的数量（Number Per Line）
推荐一行一个声明，因为如许以利于写注释。亦即，
```
int level;  // indentation level
  int size;   // size of table
```
要优于，
`int level， size;`
不要将不合类型变量的声明放在同一行，例如：
`  int foo，  fooarray[];  //WRONG!`
重视：上方的例子中，在类型和标识符之间放了一个空格，另一种被容许的调换体式格式是应用制表符：
```
int          level;         // indentation level
  int          size;          // size of table
  Object       currentEntry;  // currently ed table entry
```
## 6.2 初始化（Initialization）
尽量在声明局部变量的同时初始化。独一不这么做的来由是变量的初始值依附于某些先前产生的策画。
## 6.3 布局（Placement）
只在代码块的开端处声明变量。（一个块是指任何被包含在大括号"{"和"}"中心的代码。）不要在初次用到该变量时才声明之。这会把重视力不集中的程序员搞糊涂，同时会故障代码在该感化域内的可移植性。
```
void myMethod（） {
    int int1 = 0;         // beginning of method block
 
    if （condition） {
        int int2 = 0;     // beginning of "if"block
        ...
    }
}
```
该规矩的一个例外是for轮回的索引变量
`for （int i = 0; i < maxLoops; i++） { ... }`
避免声明的局部变量覆盖上一级声明的变量。例如，不要在内部代码块中声明雷同的变量名：
```
int count;
...
myMethod（） {
    if （condition） {
        int count= 0;     // AVOID!
        ...
    }
    ...
}
```
## 6.4 类和接口的声明（Class and Interface Declarations）
当编写类和接口是，应当遵守以下格局规矩：
-  在办法名与其参数列表之前的左括号"（"间不要有空格
-  左大括号"{"位于声明语句同业的末尾
-  右大括号"}"另起一行，与响应的声明语句对齐，除非是一个空语句，"}"应紧跟在"{"之后
```
class Sample extends Object {
    int ivar1;
    int ivar2;
 
    Sample（int i， int j） {
        ivar1 = i;
        ivar2 = j;
    }
 
    int emptyMethod（） {}
 
    ...
}
```
-  办法与办法之间以空行分隔
# 7 语句（Statements）
## 7.1 简单语句（Simple Statements）
每行至多包含一条语句，例如：
```
argv++;       // Correct
argc--;       // Correct
argv++; argc--;       // AVOID!
```
## 7.2 复合语句（Compound Statements）
复合语句是包含在大括号中的语句序列，形如"{ 语句 }"。例如下面各段。
-  被括此中的语句应当较之复合语句缩进一个层次
-  左大括号"{"应位于复合语句肇端行的行尾；右大括号"}"应另起一行并与复合语句首行对齐。
-  大括号可以被用于所有语句，包含单个语句，只要这些语句是诸如if-else或for把握布局的一项目组。如许便于添加语句而无需愁闷因为忘了加括号而引入bug。
## 7.3 返回语句（return Statements）
一个带返回值的return语句不应用小括号"（）"，除非它们以某种体式格式使返回值更为显见。例如：
```
return;
 
return myDisk.size（）;
 
return （size ？ size : defaultSize）;
```
## 7.4 if，if-else，if else-if else语句（if， if-else， if else-if elseStatements）
if-else语句应当具有如下格局：
```
if （condition） {
    statements;
}
 
if （condition） {
    statements;
} else {
    statements;
}
 
if （condition） {
    statements;
} else if （condition） {
    statements;
} else {
    statements;
}
```
重视：if语句老是用"{"和"}"括起来，避免应用如下轻易引起错误的格局：
```
if （condition）//AVOID! THIS OMITS THE BRACES {}!
    statement;
```
## 7.5 for语句（for Statements）
一个for语句应当具有如下格局：
```
for （initialization; condition; ） {
    statements;
}
```
一个空的for语句（所有工作都在初始化，前提断定，更新子句中完成）应当具有如下格局：
`for （initialization; condition; ）;`
当在for语句的初始化或更新子句中应用逗号时，避免因应用三个以上变量，而导致错杂度进步。若须要，可以在for轮回之前（为初始化子句）或for轮回末尾（为更新子句）应用零丁的语句。
## 7.6 while语句（while Statements）
一个while语句应当具有如下格局
```
while （condition） {
    statements;
}
```
一个空的while语句应当具有如下格局：
`while （condition）;`
## 7.7 do-while语句（do-while Statements）
一个do-while语句应当具有如下格局：
```
do {
    statements;
} while （condition）;
```
## 7.8 switch语句（switch Statements）
一个switch语句应当具有如下格局：
```
switch （condition） {
case ABC:
    statements;
    /* falls through */
case DEF:
    statements;
    break;
 
case XYZ:
    statements;
    break;
 
default:
    statements;
    break;
}
```
每当一个case顺着往下履行时（因为没有break语句），凡是应在break语句的地位添加注释。上方的示例代码中就包含注释/* falls through */。
## 7.9 try-catch语句（try-catch Statements）
一个try-catch语句应当具有如下格局：
```
try {
    statements;
} catch （ExceptionClass e） {
    statements;
}
```
一个try-catch语句后面也可能跟着一个finally语句，非论try代码块是否顺利履行完，它都邑被履行。
```
try {
    statements;
} catch （ExceptionClass e） {
    statements;
} finally {
    statements;
}
```
# 8 空白（White Space）
## 8.1 空行（Blank Lines）
空行将逻辑相干的代码段分隔开，以进步可读性。
下列景象应当老是应用两个空行：
-  一个源文件的两个片段（section）之间
-  类声明和接口声明之间
下列景象应当老是应用一个空行：
- 两个办法之间
- 办法内的局部变量和办法的第一条语句之间
- 块注释（拜见"5.1.1"）或单行注释（拜见"5.1.2"）之前
- 一个办法内的两个逻辑段之间，用以进步可读性 
## 8.2 空格（Blank Spaces）
下列景象应当应用空格：
-  一个紧跟着括号的关键字应当被空格分隔，例如：
```
while （true） {
        ...
    }
```
重视：空格不该该置于办法名与其左括号之间。这将有助于区分关键字和办法调用。
-  空白应当位于参数列表中逗号的后面
-  所有的二元运算符，除了"."，应当应用空格将之与操纵数分隔。一元操纵符和操纵数之间不因该加空格，比如：负号（"-"）、自增（"++"）和自减（"--"）。例如：
```
a += c + d;
    a = （a + b） /（c * d）;
 
    while （d++ = s++） {
        n++;
    }
    printSize（"size is " + foo + "
"）;
```
-  for语句中的表达式应当被空格分隔，例如：
`    for （expr1; expr2; expr3）`
-  强迫转型后应当跟一个空格，例如：
` `
```
myMethod（（byte） aNum， （Object） x）;
    myMethod（（int） （cp + 5）， （（int） （i + 3））
                                  + 1）;
```
# 9 定名规范（Naming Conventions）
定名规范使程序更易读，从而更易于懂得。它们也可以供给一些有关标识符功能的信息，以助于懂得代码，例如，非论它是一个常量，包，还是类。
|标识符类型|定名规矩|例子|
|----|----|----|
|包（Packages）|一个独一包名的前缀老是全部小写的ASCII字母并且是一个域名，凡是是com，edu，gov，mil，net，org，或1981年 ISO 3166标准所指定的标识国度的英文双字符代码。包名的后续项目组按照不合机构各自内部的定名规范而不尽雷同。这类定名规范可能以特定目次名的构成来区分部 门（department），项目（project），机械（machine），或注册名（login names）。|com.sun.eng			com.apple.quicktime.v2			edu.cmu.cs.bovik.cheese|
|类（Classes）|定名规矩：类名是个一名词，采取大小写混淆的体式格式，每个单词的首字母大写。尽量使你的类名简洁而富于描述。应用完全单词，避免缩写词（除非该缩写词被更广泛应用，像URL，HTML）|class Raster;			class ImageSprite;|
|接口（Interfaces）|定名规矩：大小写规矩与类名类似|interface RasterDelegate;			interface Storing;|
|办法（Methods）|办法名是一个动词，采取大小写混淆的体式格式，第一个单词的首字母小写，厥后单词的首字母大写。|run（）;			runFast（）;			getBackground（）;|
|变量（Variables）|除了变量名外，所有实例，包含类，类常量，均采取大小写混淆的体式格式，第一个单词的首字母小写，厥后单词的首字母大写。变量名不该以下划线或美元符号开首，尽管这在语法上是容许的。			变量名应简短且富于描述。变量名的选用应当易于记忆，即，可以或许指出其用处。尽量避免单个字符的变量名，除非是一次性的姑且变量。姑且变量凡是被取名为i，j，k，m和n，它们一般用于整型；c，d，e，它们一般用于字符型。|char c;			int i;			float myWidth;|
|实例变量（Instance Variables）|大小写规矩和变量名类似，除了前面须要一个下划线|int _employeeId;			String _name;			Customer _customer;|
|常量（Constants）|类常量和ANSI常量的声明，应当全部大写，单词间用下划线隔开。（尽量避免ANSI常量，轻易引起错误）|static final int MIN_WIDTH = 4;			static final int MAX_WIDTH = 999;			static final int GET_THE_CPU = 1;|
# 10 编程实践（Programming Practices）
## 10.1 供给对实例以及类变量的接见把握（Providing Access to Instance and Class Variables）
若没有足够来由，不要把实例或类变量声明为公有。凡是，实例变量无需显式的设置（set）和获取（gotten），凡是这作为办法调用的边沿效应 （side effect）而产生。
一个具有公有实例变量的恰当例子，是类仅作为数据布局，没有行动。亦即，若你要应用一个布局（struct）而非一个类（若是java支撑布局的话），那么把类的实例变量声明为公有是合适的。
## 10.2 引用类变量和类办法（Referring to Class Variables and Methods）
避免用一个对象接见一个类的静态变量和办法。应当用类名调换。例如：
```
classMethod（）;             //OK
AClass.classMethod（）;      //OK
anObject.classMethod（）;    //AVOID!
```
## 10.3 常量（Constants）
位于for轮回中作为计数器值的数字常量，除了-1，0和1之外，不该被直接写入代码。
## 10.4 变量赋值（Variable Assignments）
避免在一个语句中给多个变量赋雷同的值。它很难读懂。例如：
`fooBar.fChar = barFoo.lchar = ""c""; // AVOID!`
不要将赋值运算符用在轻易与相等关系运算符混合的处所。例如：
```
if （c++ = d++） {        // AVOID! （Java disallows）
    ...
}
```
应当写成
```
if （（c++ = d++） != 0） {
    ...
}
```
不要应用内嵌（embedded）赋值运算符试图进步运行时的效力，这是编译器的工作。例如：
`d = （a = b + c） + r;        // AVOID!`
应当写成
```
a = b + c;
d = a + r;
```
## 10.5 其它实战（Miscellaneous Practices）
### 10.5.1 圆括号（Parentheses）
一般而言，在含有多种运算符的表达式中应用圆括号来避免运算符优先级题目，是个好办法。即使运算符的优先级对你而言可能很清楚，但对其他人未必如此。你不克不及假设此外程序员和你一样清楚运算符的优先级。
```
if （a == b && c == d）      // AVOID!
if （（a == b） && （c == d））  // RIGHT
```
### 10.5.2 返回值（Returning Values）
设法让你的程序布局合适目标。例如：
```
if （booleanExpression） {
    return true;
} else {
    return false;
}
```
应当代之以如下办法：
`return booleanExpression;`
类似地：
```
if （condition） {
    return x;
}
return y;
```
应当写做：
`return （condition ？ x : y）;`
### 10.5.3 前提运算符"？"前的表达式（Expressions before ""？"" in the Conditional Operator）
若是一个包含二元运算符的表达式呈如今三元运算符" ？ : "的"？"之前，那么应当给表达式添上一对圆括号。例如：
`（x >= 0） ？ x : -x;`
### 10.5.4 特别注释（Special Comments）
在注释中应用XXX来标识某些未实现（bogus）的但可以工作（works）的内容。用FIXME来标识某些假的和错误的内容。
# 11 代码示例（Code Examples）
## 11.1 Java源文件示例（Java Source File Example）
下面的例子，显现了如何公道布局一个包含单一公共类的Java源程序。接口的布局与其类似。更多信息拜见"类和接口声明"以及"文挡注释"。
```
/*
 *＠（＃）Blah.java        1.82 99/03/18
 *
 * Copyright （c）1994-1999 Sun Microsystems， Inc.
 * 901 San AntonioRoad， Palo Alto， California， 94303， U.S.A.
 * All rightsreserved.
 *
 * This software isthe confidential and proprietary information of Sun
 * Microsystems，Inc. （"Confidential Information"）. You shall not
 * disclose suchConfidential Information and shall use it only in
 * accordance withthe terms of the license agreement you entered into
 * with Sun.
 */
 
package java.blah;
 
import java.blah.blahdy.BlahBlah;
 
/**
 * Classdescription goes here.
 *
 * ＠version    1.82 18 Mar 1999
 * ＠author     Firstname Lastname
 */
public class Blah extends SomeClass {
    /* A classimplementation comment can go here. */
 
    /** classVar1documentation comment */
    public staticint classVar1;
 
    /**
     * classVar2documentation comment that happens to be
     * more thanone line long
     */
    private static ObjectclassVar2;
 
    /**instanceVar1 documentation comment */
    public ObjectinstanceVar1;
 
    /**instanceVar2 documentation comment */
    protected intinstanceVar2;
 
    /**instanceVar3 documentation comment */
    privateObject[] instanceVar3;
 
    /**
     *...constructor Blah documentation comment...
     */
    public Blah（） {
        //...implementation goes here...
    }
 
    /**
     * ...methoddoSomething documentation comment...
     */
    public voiddoSomething（） {
        //...implementation goes here...
    }
 
    /**
     * ...methoddoSomethingElse documentation comment...
     * ＠paramsomeParam description
     */
    public voiddoSomethingElse（Object someParam） {
        //...implementation goes here...
    }
}
```
