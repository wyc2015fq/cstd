
# java入门教程-6.8Java throws子句 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:44:04[seven-soft](https://me.csdn.net/softn)阅读数：149


如果一个方法可以导致一个异常但不处理它，它必须指定这种行为以使方法的调用者可以保护它们自己而不发生异常。做到这点你可以在方法声明中包含一个throws子句。一个
 throws 子句列举了一个方法可能抛出的所有异常类型。这对于除Error或RuntimeException及它们子类以外类型的所有异常是必要的。一个方法可以抛出的所有其他类型的异常必须在throws子句中声明。如果不这样做，将会导致编译错误。
下面是包含一个throws子句的方法声明的通用形式：
type method-name(parameter-list) throws exception-list{
// body of method
}
这里，exception-list是该方法可以抛出的以有逗号分割的异常列表。
下面是一个不正确的例子。该例试图抛出一个它不能捕获的异常。因为程序没有指定一个throws子句来声明这一事实，程序将不会编译。
// This program contains an error and will not compile.
class ThrowsDemo {
static void throwOne() {
System.out.println("Inside throwOne.");
throw new IllegalAccessException("demo");
}
public static void main(String args[]) {
throwOne();
}
}
为编译该程序，需要改变两个地方。第一，需要声明throwOne( )引发IllegalAccess Exception异常。第二，main( )必须定义一个try/catch 语句来捕获该异常。正确的例子如下：
// This is now correct.
class ThrowsDemo {
static void throwOne() throws IllegalAccessException {
System.out.println("Inside throwOne.");
throw new IllegalAccessException("demo");
}
public static void main(String args[]) {
try {
throwOne();
} catch (IllegalAccessException e) {
System.out.println("Caught " + e);
}
}
}
下面是例题的输出结果：
inside throwOne
caught java.lang.IllegalAccessException: demo

