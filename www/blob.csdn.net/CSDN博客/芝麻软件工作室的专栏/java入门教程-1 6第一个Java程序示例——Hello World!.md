
# java入门教程-1.6第一个Java程序示例——Hello World! -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 09:15:08[seven-soft](https://me.csdn.net/softn)阅读数：2960


跟随世界潮流，第一个Java程序输出“Hell World!”。
## 通过Eclipse运行程序
启动Eclipse，在菜单中选择“文件 --> 新建 --> Java项目”，弹出对话框：![](http://www.weixueyuan.net/uploads/allimg/141125/1-14112510501W43.png)
图1  新建项目
填入项目名称，点击“完成”，创建项目成功，可以看到在 E:\java\workspace 目录下多出了一个Demo文件夹。
在菜单中选择“文件 --> 新建 --> 类”，弹出对话框：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125105632127.png)
图2  创建类
输入类名称，点击“完成”，成功创建一个HelloWorld类：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125105K3606.png)
图3  新创建的类文件
将下面的代码输入到源文件，注意Java是大小写敏感的：publicclassHelloWorld{
publicstaticvoidmain(String[]args){
System.out.println("Hello World!");
}
}

点击运行按钮![](http://www.weixueyuan.net/uploads/allimg/141125/1-1411251102403U.png)，或者在菜单中选择“运行
 --> 运行”，就可以看到下方控制台输出了“Hello World!”：![](http://www.weixueyuan.net/uploads/allimg/141125/1-14112511044AF.png)
图4  控制台输出
恭喜你，你成功编写并运行了第一个Java程序。
打开Demo文件夹下的bin目录，出现了一个 HelloWorld.class 文件，这就是编译生成的字节码。与C、C++不同，Java编译的结果不是可执行文件，而是字节码文件。字节码文件不能直接运行，必须由JVM翻译成机器码才能运行，这就是为什么运行Java程序必须安装JVM的原因。
## 通过命令行运行程序
当然，你也可以直接输入命令来编译和运行Java。
打开CMD，cd 到源文件所在目录，通过 javac filename 命令完成编译，通过 java classname 命令来运行程序，如下所示：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125131125605.png)
图5  通过命令编译并运行Java程序
在Eclipse中运行程序，实际上也是执行这些命令，并将输出结果显示到控制台。有了Eclipse，编程更加方便。
## HelloWorld程序分析
分析上述代码，从程序开始处介绍。
public class HelloWorld
定义了一个类，类是 “public” 公共类型的，类名为“HelloWorld”。另外，Java
 中主类名应该和要保存的 Java 文件名相同，也就是说，这里定义的类名是“HelloWorld”，则文件应该保存为“HelloWorld.java”。
public static void main(String[] args)
Java 中的主运行方法，它和 C/C++中的main()作用是一样的，就是所有的程序都从“main()”中开始执行。要执行 Java 程序，必须有一个包括主运行方法的类。至于“public static void”的含义，读者可以尝试着去掉，看看编译器会提示什么错误。
System.out.println("Hello world");
“System.out.println()”是 Java.lang 包的一个方法，用来将字串“Hello world”输出到命令行窗口。
大家先不要急于理解所有概念，后续会一一讲解。

