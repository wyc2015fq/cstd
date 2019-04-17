# 重拾C#教程：预处理器指令 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 13:12:07[boonya](https://me.csdn.net/boonya)阅读数：343








原文地址：[http://www.runoob.com/csharp/csharp-preprocessor-directives.html](http://www.runoob.com/csharp/csharp-preprocessor-directives.html)




预处理器指令指导编译器在实际编译开始之前对信息进行预处理。

所有的预处理器指令都是以 # 开始。且在一行上，只有空白字符可以出现在预处理器指令之前。预处理器指令不是语句，所以它们不以分号（;）结束。

C# 编译器没有一个单独的预处理器，但是，指令被处理时就像是有一个单独的预处理器一样。在 C# 中，预处理器指令用于在条件编译中起作用。与 C 和 C++ 不同的是，它们不是用来创建宏。一个预处理器指令必须是该行上的唯一指令。


## C# 预处理器指令列表

下表列出了 C# 中可用的预处理器指令：
|预处理器指令|描述|
|----|----|
|#define|它用于定义一系列成为符号的字符。|
|#undef|它用于取消定义符号。|
|#if|它用于测试符号是否为真。|
|#else|它用于创建复合条件指令，与 #if 一起使用。|
|#elif|它用于创建复合条件指令。|
|#endif|指定一个条件指令的结束。|
|#line|它可以让您修改编译器的行数以及（可选地）输出错误和警告的文件名。|
|#error|它允许从代码的指定位置生成一个错误。|
|#warning|它允许从代码的指定位置生成一级警告。|
|#region|它可以让您在使用 Visual Studio Code Editor 的大纲特性时，指定一个可展开或折叠的代码块。|
|#endregion|它标识着 #region 块的结束。|
#define 预处理器
#define 预处理器指令创建符号常量。

#define 允许您定义一个符号，这样，通过使用符号作为传递给 #if 指令的表达式，表达式将返回 true。它的语法如下：
#define symbol
下面的程序说明了这点：
#define PI 
usingSystem;namespacePreprocessorDAppl{classProgram{staticvoidMain(string[] args){#if (PI)Console.WriteLine("PI is defined");#elseConsole.WriteLine("PI is not defined");#endifConsole.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
PI isdefined
## 条件指令

您可以使用 #if 指令来创建一个条件指令。条件指令用于测试符号是否为真。如果为真，编译器会执行 #if 和下一个指令之间的代码。

条件指令的语法：
#if symbol [operator symbol]...
其中，*symbol* 是要测试的符号名称。您也可以使用 true 和 false，或在符号前放置否定运算符。

常见运算符有：
- == (等于)
- != (不等于)
- && (与)
- || (或)

您也可以用括号把符号和运算符进行分组。条件指令用于在调试版本或编译指定配置时编译代码。一个以 **#if** 指令开始的条件指令，必须显示地以一个 **#endif** 指令终止。

下面的程序演示了条件指令的用法：
#define DEBUG
#define VC_V10
usingSystem;publicclassTestClass{publicstaticvoidMain(){#if (DEBUG && !VC_V10)Console.WriteLine("DEBUG is defined");#elif(!DEBUG && VC_V10)Console.WriteLine("VC_V10 is defined");#elif(DEBUG && VC_V10)Console.WriteLine("DEBUG and VC_V10 are defined");#elseConsole.WriteLine("DEBUG and VC_V10 are not defined");#endifConsole.ReadKey();}}
当上面的代码被编译和执行时，它会产生下列结果：
DEBUG and VC_V10 are defined



