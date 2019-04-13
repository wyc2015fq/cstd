
# java入门教程-6.7Java throw：异常的抛出 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:42:47[seven-soft](https://me.csdn.net/softn)阅读数：171个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



到目前为止，你只是获取了被Java运行时系统抛出的异常。然而，程序可以用throw语句抛出明确的异常。Throw语句的通常形式如下：
throw ThrowableInstance;
这里，ThrowableInstance一定是Throwable类类型或Throwable子类类型的一个对象。简单类型，例如int或char，以及非Throwable类，例如String或Object，不能用作异常。有两种可以获得Throwable对象的方法：在catch子句中使用参数或者用new操作符创建。
程序执行在throw语句之后立即停止；后面的任何语句不被执行。最紧紧包围的try块用来检查它是否含有一个与异常类型匹配的catch语句。如果发现了匹配的块，控制转向该语句；如果没有发现，次包围的try块来检查，以此类推。如果没有发现匹配的catch块，默认异常处理程序中断程序的执行并且打印堆栈轨迹。
下面是一个创建并抛出异常的例子程序，与异常匹配的处理程序再把它抛出给外层的处理程序。
// Demonstrate throw.
class ThrowDemo {
static void demoproc() {
try {
throw new NullPointerException("demo");
} catch(NullPointerException e) {
System.out.println("Caught inside demoproc.");
throw e; // rethrow the exception
}
}
public static void main(String args[]) {
try {
demoproc();
} catch(NullPointerException e) {
System.out.println("Recaught: " + e);
}
}
}
该程序有两个机会处理相同的错误。首先，main（）设立了一个异常关系然后调用demoproc( )。 demoproc( )方法然后设立了另一个异常处理关系并且立即抛出一个新的NullPointerException实例，NullPointerException在下一行被捕获。异常于是被再次抛出。下面是输出结果：
Caught inside demoproc.
Recaught: java.lang.NullPointerException: demo
该程序还阐述了怎样创建Java的标准异常对象，特别注意下面这一行：
throw new NullPointerException("demo");
这里，new用来构造一个NullPointerException实例。所有的Java内置的运行时异常有两个构造函数：一个没有参数，一个带有一个字符串参数。当用到第二种形式时，参数指定描述异常的字符串。如果对象用作 print(
 )或println( )的参数时，该字符串被显示。这同样可以通过调用getMessage( )来实现，getMessage( )是由Throwable定义的。

