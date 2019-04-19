# java学习笔记：文件名区分大小写 - 左直拳的马桶_日用桶 - CSDN博客
2014年07月17日 23:13:40[左直拳](https://me.csdn.net/leftfist)阅读数：3918
我按照网上的教程，写了JAVA第一个程序：Hello World!，出了两个问题，都栽在 大小写 上。
```java
public class Hello {
	public static void main(String args[]){
		System.out.println("Hello World!");
	}
}
```
保存的文件名是 hello.java，一编译，报这样的错：
**D:\chenqu\code\java\HelloWorld>javac hello.java**
hello.java:1: 类 Hello 是公共的，应在名为 Hello.java 的文件中声明
public class Hello {
       ^
1 错误
百撕不得骑姐，后来网上有提示说文件名要与类名一致，将文件名区分大小写，改回Hello.java，果然编译通过；
编译以后，运行：
**D:\chenqu\code\java\HelloWorld>java hello**
Exception in thread "main" java.lang.NoClassDefFoundError: hello (wrong name: He
llo)
        at java.lang.ClassLoader.defineClass1(Native Method)
        at java.lang.ClassLoader.defineClass(Unknown Source)
        at java.security.SecureClassLoader.defineClass(Unknown Source)
        at java.net.URLClassLoader.defineClass(Unknown Source)
        at java.net.URLClassLoader.access$100(Unknown Source)
        at java.net.URLClassLoader$1.run(Unknown Source)
        at java.net.URLClassLoader$1.run(Unknown Source)
        at java.security.AccessController.doPrivileged(Native Method)
        at java.net.URLClassLoader.findClass(Unknown Source)
        at java.lang.ClassLoader.loadClass(Unknown Source)
        at sun.misc.Launcher$AppClassLoader.loadClass(Unknown Source)
        at java.lang.ClassLoader.loadClass(Unknown Source)
        at sun.launcher.LauncherHelper.checkAndLoadMain(Unknown Source)
出来一堆的错
网上都说是环境配置的问题，我改来改去，都不行。结果还是在文件名大小写上
**D:\chenqu\code\java\HelloWorld>java Hello**
Hello World!
靠！
终于转向Java了，15年前我就应该这样子做。青春已逝，年华不再，禁不住内牛满面。

