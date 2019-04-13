
# java入门教程-6.3Java未被捕获的异常 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:37:36[seven-soft](https://me.csdn.net/softn)阅读数：184个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



在你学习在程序中处理异常之前，看一看如果你不处理它们会有什么情况发生是很有好处的。下面的小程序包括一个故意导致被零除错误的表达式。
class Exc0 {
public static void main(String args[]) {
int d = 0;
int a = 42 / d;
}
}
当Java运行时系统检查到被零除的情况，它构造一个新的异常对象然后抛出该异常。这导致Exc0的执行停止，因为一旦一个异常被抛出，它必须被一个异常处理程序捕获并且被立即处理。该例中，我们没有提供任何我们自己的异常处理程序，所以异常被Java运行时系统的默认处理程序捕获。任何不是被你程序捕获的异常最终都会被该默认处理程序处理。默认处理程序显示一个描述异常的字符串，打印异常发生处的堆栈轨迹并且终止程序。
下面是由标准javaJDK运行时解释器执行该程序所产生的输出：
java.lang.ArithmeticException: / by zero
at Exc0.main(Exc0.java:4)
注意，类名Exc0，方法名main，文件名Exc0.java和行数4是怎样被包括在一个简单的堆栈使用轨迹中的。还有，注意抛出的异常类型是Exception的一个名为ArithmeticException的子类，该子类更明确的描述了何种类型的错误方法。本章后面部分将讨论，Java提供多个内置的与可能产生的不同种类运行时错误相匹配的异常类型。
堆栈轨迹将显示导致错误产生的方法调用序列。例如，下面是前面程序的另一个版本，它介绍了相同的错误，但是错误是在main( )方法之外的另一个方法中产生的：
class Exc1 {
static void subroutine() {
int d = 0;
int a = 10 / d;
}
public static void main(String args[]) {
Exc1.subroutine();
}
}
默认异常处理器的堆栈轨迹结果表明了整个调用栈是怎样显示的：
java.lang.ArithmeticException: / by zero
at Exc1.subroutine(Exc1.java:4)
at Exc1.main(Exc1.java:7)
如你所见，栈底是main的第7行，该行调用了subroutine( )方法。该方法在第4行导致了异常。调用堆栈对于调试来说是很重要的，因为它查明了导致错误的精确的步骤。

