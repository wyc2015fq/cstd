# JAVA中的main()函数 - xqhrs232的专栏 - CSDN博客
2018年11月12日 11:02:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：60
原文地址::[https://blog.csdn.net/baidu_23735127/article/details/79525567](https://blog.csdn.net/baidu_23735127/article/details/79525567)
相关文章
1、[JAVA中的main()方法详解](https://www.cnblogs.com/Wonderful-life217/p/8309974.html)----[https://www.cnblogs.com/Wonderful-life217/p/8309974.html](https://www.cnblogs.com/Wonderful-life217/p/8309974.html)
2、Java中main()函数调用类方法限制----[https://blog.csdn.net/vhdl3018/article/details/78975943](https://blog.csdn.net/vhdl3018/article/details/78975943)
3、[java中main函数解析（转载）](https://www.cnblogs.com/shootercheng/p/5421995.html)----[https://www.cnblogs.com/shootercheng/p/5421995.html](https://www.cnblogs.com/shootercheng/p/5421995.html)
**前言**
JAVA中的主函数是我们再熟悉不过的了，相信每个学习过JAVA语言的人都能够熟练地写出这个程序的入口函数，但对于主函数为什么这么写，其中的每个关键字分别是什么意思，可能就不是所有人都能轻松地答出来的了。我也是在学习中碰到了这个问题，通过在网上搜索资料，并加上自己的实践终于有了一点心得，不敢保留，写出来与大家分享。
**Java中的main()方法**
java虚拟机通过main方法找到需要启动的运行程序，并且检查main函数所在类是否被java虚拟机装载。如果没有装载，那么就装载该类，并且装载所有相关的其他类。因此程序在运行的时候，第一个执行的方法就是`main()`方法。通常情况下， 如果要运行一个类的方法，必须首先实例化出来这个类的一个对象，然后通过"**对象名.方法名()**` `"的方式来运行方法，但是因为main是程序的入口，这时候还没有实例化对象，因此将main方法声明为static的，这样这个方法就可以直接通过“**类名.方法名() **”的方式来调用。
在看Java中的main()方法之前，先看一个最简单的Java应用程序HelloWorld，我将通过这个例子说明Java类中`main()`方法的奥秘，程序的代码如下：
[?](http://www.jb51.net/article/91910.htm#)
|12345678|`/**``* Java中的main()方法详解``*/``public``class``HelloWorld {`` ``public``static``void``main(String args[]) {`` ``System.out.println(``"Hello World!"``);`` ``}``}`|
**一、先说类：**
HelloWorld 类中有`main()`方法，说明这是个java应用程序，通过JVM直接启动运行的程序。
既然是类，java允许类不加public关键字约束，当然类的定义只能限制为public或者无限制关键字（默认的）。
**二、再说main()方法**
这个`main()`方法的声明为：`public static void main(String args[])` 。必须这么定义，这是Java的规范。
为什么要这么定义，和JVM的运行有关系。
当一个类中有`main()`方法，执行命令“java 类名”则会启动虚拟机执行该类中的main方法。
由于JVM在运行这个Java应用程序的时候，首先会调用main方法，调用时不实例化这个类的对象，而是通过类名直接调用因此需要是限制为public static。
对于java中的main方法，jvm有限制，不能有返回值，因此返回值类型为void。
main方法中还有一个输入参数，类型为String[]，这个也是java的规范，`main()`方法中必须有一个入参，类细必须`String[] `，至于字符串数组的名字，这个是可以自己设定的，根据习惯，这个字符串数组的名字一般和sun java规范范例中mian参数名保持一致，取名为args。
因此，`main()`方法定义必须是：“`public static void main(String 字符串数组参数名[])` ”。
**三、main()方法中可以throw Exception**
因此`main()`方法中可以抛出异常，`main()`方法上也可以声明抛出异常。
比如，下面这个写法是正确的：
[?](http://www.jb51.net/article/91910.htm#)
|123456|`public``class``TestMain {`` ``public``static``void``main(String[] args) ``throws``Exception {``  ``System.out.println(``"哈哈哈哈哈"``);``  ``throw``new``Exception(``""``);`` ``}``}`|
[?](http://www.jb51.net/article/91910.htm#)
|1234567891011|`运行结果：``哈哈哈哈哈``Exception in thread "main" java.lang.Exception:`` ``at maintest.TestMain.main(TestMain.java:11)`` ``at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)`` ``at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)`` ``at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)`` ``at java.lang.reflect.Method.invoke(Method.java:585)`` ``at com.intellij.rt.execution.application.AppMain.main(AppMain.java:90)``Process finished with exit code 1`|
**四、main()方法中字符串参数数组作用**
`main()`方法中字符串参数数组作用是接收命令行输入参数的，命令行的参数之间用空格隔开。
下面给出一个例子，看看如何初始化和使用这个数组的。
[?](http://www.jb51.net/article/91910.htm#)
|1234567891011|`/**``* 打印main方法中的输入参数``*/``public``class``TestMain {`` ``public``static``void``main(String args[]){`` ``System.out.println(``"打印main方法中的输入参数！"``);`` ``for``(``int``i=``0``;i<args.length;i++){``  ``System.out.println(args[i]);`` ``}`` ``}``}`|
执行方法和运行结果
[?](http://www.jb51.net/article/91910.htm#)
|1234567|`D:\Study\basetest\src>javac TestMain.java``D:\Study\basetest\src>java TestMain ``1``2``3``打印main方法中的输入参数！``1``2``3`|
**五、给出HelloWorld的另外一个版本**
[?](http://www.jb51.net/article/91910.htm#)
|1234567891011|`/**``* 变态版的HelloWorld.呵呵``*/``public``class``HelloWorld2 {`` ``static``{`` ``System.out.println(``"Hello Wordld!"``);`` ``}`` ``public``static``void``main(String args[]){`` ``System.exit(``0``);`` ``}``}`|
这个`main()`方法执行的内容就一句" `System.exit(0); `" ，目的是让程序正常结束。那“HelloWorld！”是从哪里打印的，秘密就是在static打印的，因为static代码块的内容会在main调用前调用。
**六、实例**
虚拟机通过调用某个指定类的方法main启动，传递给main一个字符串数组参数，使指定的类被装载，同时链接该类所使用的其它的类型，并且初始化它们。例如对于程序：
[?](http://www.jb51.net/article/91910.htm#)
|12345678|`public``class``HelloApp {`` ``public``static``void``main(String[] args) {`` ``System.out.println(``"Hello World!"``);`` ``for``(``int``i = ``0``; i < args.length; i++) {``  ``System.out.println(args);`` ``}`` ``}``}`|
编译后在命令行模式下键入： java HelloApp run virtual machine
将通过调用HelloApp的方法main来启动java虚拟机，传递给main一个包含三个字符串"run"、"virtual"、"machine"的数组。现在我们略述虚拟机在执行HelloApp时可能采取的步骤。
开始试图执行类HelloApp的main方法，发现该类并没有被装载，也就是说虚拟机当前不包含该类的二进制代表，于是虚拟机使用ClassLoader试图寻找这样的二进制代表。如果这个进程失败，则抛出一个异常。类被装载后同时在main方法被调用之前，必须对类HelloApp与其它类型进行链接然后初始化。链接包含三个阶段：检验，准备和解析。检验检查被装载的主类的符号和语义，准备则创建类或接口的静态域以及把这些域初始化为标准的默认值，解析负责检查主类对其它类或接口的符号引用，在这一步它是可选的。类的初始化是对类中声明的静态初始化函数和静态域的初始化构造方法的执行。一个类在初始化之前它的父类必须被初始化。
**整个过程如下：**
![](http://files.jb51.net/file_images/article/201609/20169590759151.jpg?2016859940)
**总结**
main方法作为一个特殊的规范，与普通的方法有很大区别，限制很多，理解其原理需要学习JVM相关知识。是Java中学习中大障碍。以上就是小编为大家整理的对main原理和使用的总结，希望对大家能有所帮助，如果有疑问大家可以留言交流！
感谢，转自：
http://www.jb51.net/article/91910.htm
