
# java入门教程-6.5Java 多重catch语句的使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:40:19[seven-soft](https://me.csdn.net/softn)阅读数：248


某些情况，由单个代码段可能引起多个异常。处理这种情况，你可以定义两个或更多的catch子句，每个子句捕获一种类型的异常。当异常被引发时，每一个catch子句被依次检查，第一个匹配异常类型的子句执行。当一个catch语句执行以后，其他的子句被旁路，执行从try/catch块以后的代码开始继续。下面的例子设计了两种不同的异常类型：
// Demonstrate multiple catch statements.
class MultiCatch {
public static void main(String args[]) {
try {
int a = args.length;
System.out.println("a = " + a);
int b = 42 / a;
int c[] = { 1 };
c[42] = 99;
} catch(ArithmeticException e) {
System.out.println("Divide by 0: " + e);
} catch(ArrayIndexOutOfBoundsException e) {
System.out.println("Array index oob: " + e);
}
System.out.println("After try/catch blocks.");
}
}
该程序在没有命令行参数的起始条件下运行导致被零除异常，因为a为0。如果你提供一个命令行参数，它将幸免于难，把a设成大于零的数值。但是它将导致ArrayIndexOutOf BoundsException异常，因为整型数组c的长度为1，而程序试图给c[42]赋值。
下面是运行在两种不同情况下程序的输出：
C:\>java MultiCatch
a = 0
Divide by 0: java.lang.ArithmeticException: / by zero After try/catch blocks.
C:\>java MultiCatch TestArg
a = 1
Array index oob: java.lang.ArrayIndexOutOfBoundsException After try/catch blocks.
当你用多catch语句时，记住异常子类必须在它们任何父类之前使用是很重要的。这是因为运用父类的catch语句将捕获该类型及其所有子类类型的异常。这样，如果子类在父类后面，子类将永远不会到达。而且，Java中不能到达的代码是一个错误。例如，考虑下面的程序：
/* This program contains an error.
A subclass must come before its superclass in a series of catch statements. If not,unreachable code will be created
 and acompile-time error will result.
*/
class SuperSubCatch {
public static void main(String args[]) {
try {
int a = 0;
int b = 42 / a;
} catch(Exception e) {
System.out.println("Generic Exception catch.");
}
/* This catch is never reached because
ArithmeticException is a subclass of Exception. */
catch(ArithmeticException e) { // ERROR - unreachable
System.out.println("This is never reached.");
}
}
}
如果你试着编译该程序，你会收到一个错误消息，该错误消息说明第二个catch语句不会到达，因为该异常已经被捕获。因为ArithmeticException 是Exception的子类，第一个catch语句将处理所有的面向Exception的错误，包括ArithmeticException。这意味着第二个catch语句永远不会执行。为修改程序，颠倒两个catch语句的次序。

