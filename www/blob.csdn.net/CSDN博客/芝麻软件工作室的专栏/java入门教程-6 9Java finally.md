
# java入门教程-6.9Java finally -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:45:17[seven-soft](https://me.csdn.net/softn)阅读数：120个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



当异常被抛出，通常方法的执行将作一个陡峭的非线性的转向。依赖于方法是怎样编码的，异常甚至可以导致方法过早返回。这在一些方法中是一个问题。例如，如果一个方法打开一个文件项并关闭，然后退出，你不希望关闭文件的代码被异常处理机制旁路。finally关键字为处理这种意外而设计。
finally创建一个代码块。该代码块在一个try/catch 块完成之后另一个try/catch出现之前执行。finally块无论有没有异常抛出都会执行。如果异常被抛出，finally甚至是在没有与该异常相匹配的catch子句情况下也将执行。一个方法将从一个try/catch块返回到调用程序的任何时候，经过一个未捕获的异常或者是一个明确的返回语句，finally子句在方法返回之前仍将执行。这在关闭文件句柄和释放任何在方法开始时被分配的其他资源是很有用的。finally子句是可选项，可以有也可以无。然而每一个try语句至少需要一个catch或finally子句。
下面的例子显示了3种不同的退出方法。每一个都执行了finally子句：
// Demonstrate finally.
class FinallyDemo {
// Through an exception out of the method.
static void procA() {
try {
System.out.println("inside procA");
throw new RuntimeException("demo");
} finally {
System.out.println("procA's finally");
}
}
// Return from within a try block.
static void procB() {
try {
System.out.println("inside procB");
return;
} finally {
System.out.println("procB's finally");
}
}
// Execute a try block normally.
static void procC() {
try {
System.out.println("inside procC");
} finally {
System.out.println("procC's finally");
}
}
public static void main(String args[]) {
try {
procA();
} catch (Exception e) {
System.out.println("Exception caught");
}
procB();
procC();
}
}
该例中，procA( )过早地通过抛出一个异常中断了try。Finally子句在退出时执行。procB( )的try语句通过一个return语句退出。在procB( )返回之前finally子句执行。在procC（）中，try语句正常执行，没有错误。然而，finally块仍将执行。
注意：如果finally块与一个try联合使用，finally块将在try结束之前执行。
下面是上述程序产生的输出：
inside procA
procA’s finally
Exception caught
inside procB
procB’s finally
inside procC
procC’s finally

