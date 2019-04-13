
# java入门教程-6.11使用Java创建自己的异常子类 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:47:34[seven-soft](https://me.csdn.net/softn)阅读数：140


尽管Java的内置异常处理大多数常见错误，你也许希望建立你自己的异常类型来处理你所应用的特殊情况。这是非常简单的：只要定义Exception的一个子类就可以了（Exception当然是Throwable的一个子类）。你的子类不需要实际执行什么——它们在类型系统中的存在允许你把它们当成异常使用。
Exception类自己没有定义任何方法。当然，它继承了Throwable提供的一些方法。因此，所有异常，包括你创建的，都可以获得Throwable定义的方法。这些方法显示在表10-3中。你还可以在你创建的异常类中覆盖一个或多个这样的方法。
表 10-3 Throwable 定义的方法|方法|描述|
|Throwable fillInStackTrace( )|返回一个包含完整堆栈轨迹的Throwable对象，该对象可能被再次引发。|
|String getLocalizedMessage( )|返回一个异常的局部描述|
|String getMessage( )|返回一个异常的描述|
|void printStackTrace( )|显示堆栈轨迹|
|void printStackTrace(PrintStreamstream)|把堆栈轨迹送到指定的流|
|void printStackTrace(PrintWriterstream)|把堆栈轨迹送到指定的流|
|String toString( )|返回一个包含异常描述的String对象。当输出一个Throwable对象时，该方法被println( )调用|
下面的例子声明了Exception的一个新子类，然后该子类当作方法中出错情形的信号。它重载了toString( )方法，这样可以用println( )显示异常的描述。
// This program creates a custom exception type.
class MyException extends Exception {
private int detail;
MyException(int a) {
detail = a;
}
public String toString() {
return "MyException[" + detail + "]";
}
}
class ExceptionDemo {
static void compute(int a) throws MyException {
System.out.println("Called compute(" + a + ")");
if(a > 10)
throw new MyException(a);
System.out.println("Normal exit");
}
public static void main(String args[]) {
try {
compute(1);
compute(20);
} catch (MyException e) {
System.out.println("Caught " + e);
}
}
}
该例题定义了Exception的一个子类MyException。该子类非常简单：它只含有一个构造函数和一个重载的显示异常值的toString( )方法。ExceptionDemo类定义了一个compute( )方法。该方法抛出一个MyException对象。当compute(
 )的整型参数比10大时该异常被引发。
main( )方法为MyException设立了一个异常处理程序，然后用一个合法的值和不合法的值调用compute( )来显示执行经过代码的不同路径。下面是结果：
Called compute(1)
Normal exit
Called compute(20)
Caught MyException[20]

