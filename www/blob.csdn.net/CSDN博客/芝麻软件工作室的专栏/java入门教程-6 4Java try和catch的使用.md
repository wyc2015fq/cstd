
# java入门教程-6.4Java try和catch的使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:38:44[seven-soft](https://me.csdn.net/softn)阅读数：124个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



尽管由Java运行时系统提供的默认异常处理程序对于调试是很有用的，但通常你希望自己处理异常。这样做有两个好处。第一，它允许你修正错误。第二，它防止程序自动终止。大多数用户对于在程序终止运行和在无论何时错误发生都会打印堆栈轨迹感到很烦恼（至少可以这么说）。幸运的是，这很容易避免。
为防止和处理一个运行时错误，只需要把你所要监控的代码放进一个try块就可以了。紧跟着try块的，包括一个说明你希望捕获的错误类型的catch子句。完成这个任务很简单，下面的程序包含一个处理因为被零除而产生的ArithmeticException
 异常的try块和一个catch子句。
class Exc2 {
public static void main(String args[]) {
int d, a;
try { // monitor a block of code.
d = 0;
a = 42 / d;
System.out.println("This will not be printed.");
} catch (ArithmeticException e) { // catch divide-by-zero error
System.out.println("Division by zero.");
}
System.out.println("After catch statement.");
}
}
该程序输出如下：
Division by zero.
After catch statement.
注意在try块中的对println( )的调用是永远不会执行的。一旦异常被引发，程序控制由try块转到catch块。执行永远不会从catch块“返回”到try块。因此，“This
 will not be printed。”
将不会被显示。一旦执行了catch语句，程序控制从整个try/catch机制的下面一行继续。
一个try和它的catch语句形成了一个单元。catch子句的范围限制于try语句前面所定义的语句。一个catch语句不能捕获另一个try声明所引发的异常（除非是嵌套的try语句情况）。
被try保护的语句声明必须在一个大括号之内（也就是说，它们必须在一个块中）。你不能单独使用try。
构造catch子句的目的是解决异常情况并且像错误没有发生一样继续运行。例如，下面的程序中，每一个for循环的反复得到两个随机整数。这两个整数分别被对方除，结果用来除12345。最后的结果存在a中。如果一个除法操作导致被零除错误，它将被捕获，a的值设为零，程序继续运行。
// Handle an exception and move on.
import java.util.Random;
class HandleError {
public static void main(String args[]) {
int a=0, b=0, c=0;
Random r = new Random();
for(int i=0; i<32000; i++) {
try {
b = r.nextInt();
c = r.nextInt();
a = 12345 / (b/c);
} catch (ArithmeticException e) {
System.out.println("Division by zero.");
a = 0; // set a to zero and continue
}
System.out.println("a: " + a);
}
}
}
## 显示一个异常的描述
Throwable重载toString( )方法（由Object定义），所以它返回一个包含异常描述的字符串。你可以通过在println( )中传给异常一个参数来显示该异常的描述。例如，前面程序的catch块可以被重写成
catch (ArithmeticException e) {
System.out.println("Exception: " + e);
a = 0; // set a to zero and continue
}
当这个版本代替原程序中的版本，程序在标准javaJDK解释器下运行，每一个被零除错误显示下面的消息：
Exception: java.lang.ArithmeticException: / by zero
尽管在上下文中没有特殊的值，显示一个异常描述的能力在其他情况下是很有价值的——特别是当你对异常进行实验和调试时。

