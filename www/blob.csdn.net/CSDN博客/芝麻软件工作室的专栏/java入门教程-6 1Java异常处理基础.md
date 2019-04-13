
# java入门教程-6.1Java异常处理基础 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:35:34[seven-soft](https://me.csdn.net/softn)阅读数：158


Java异常是一个描述在代码段中发生的异常（也就是出错）情况的对象。当异常情况发生，一个代表该异常的对象被创建并且在导致该错误的方法中被抛出（throw）。该方法可以选择自己处理异常或传递该异常。两种情况下，该异常被捕获（caught）并处理。异常可能是由Java运行时系统产生，或者是由你的手工代码产生。被Java抛出的异常与违反语言规范或超出Java执行环境限制的基本错误有关。手工编码产生的异常基本上用于报告方法调用程序的出错状况。
Java异常处理通过5个关键字控制：try、catch、throw、throws和 finally。下面讲述它们如何工作的。程序声明了你想要的异常监控包含在一个try块中。如果在try块中发生异常，它被抛出。你的代码可以捕捉这个异常（用catch）并且用某种合理的方法处理该异常。系统产生的异常被Java运行时系统自动抛出。手动抛出一个异常，用关键字throw。任何被抛出方法的异常都必须通过throws子句定义。任何在方法返回前绝对被执行的代码被放置在finally块中。
下面是一个异常处理块的通常形式：
try {
// block of code to monitor for errors
}
catch (ExceptionType1 exOb) {
// exception handler for ExceptionType1
}
catch (ExceptionType2 exOb) {
// exception handler for ExceptionType2
}
// ...
finally {
// block of code to be executed before try block ends
}
这里，ExceptionType 是发生异常的类型。下面将介绍怎样应用这个框架。

