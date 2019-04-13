
# java入门教程-3.8再谈Java包 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:47:27[seven-soft](https://me.csdn.net/softn)阅读数：128个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



在Java中，为了组织代码的方便，可以将功能相似的类放到一个文件夹内，这个文件夹，就叫做包。
包不但可以包含类，还可以包含接口和其他的包。
目录以"\"来表示层级关系，例如 E:\Java\workspace\Demo\bin\p1\p2\Test.java。
包以"."来表示层级关系，例如 p1.p2.Test 表示的目录为 \p1\p2\Test.class。
## 如何实现包
通过 package 关键字可以声明一个包，例如：
package p1.p2;
必须将 package 语句放在所有语句的前面，例如：packagep1.p2;
publicclassTest{
publicTest(){
System.out.println("我是Test类的构造方法");
}
}

表明 Test 类位于 p1.p2 包中。
## 包的调用
在Java中，调用其他包中的类共有两种方式。
#### 1) 在每个类名前面加上完整的包名
程序举例：publicclassDemo{
publicstaticvoidmain(String[]args){
java.util.Datetoday=newjava.util.Date();
System.out.println(today);
}
}

运行结果：
Wed Dec 03 11:20:13 CST 2014
#### 2) 通过 import 语句引入包中的类
程序举例：importjava.util.Date;
// 也可以引入 java.util 包中的所有类
// import java.util.*;
publicclassDemo{
publicstaticvoidmain(String[]args){
Datetoday=newDate();
System.out.println(today);
}
}

运行结果与上面相同。
实际编程中，没有必要把要引入的类写的那么详细，可以直接引入特定包中所有的类，例如 import java.util.*;。
## 类的路径
Java 在导入类时，必须要知道类的绝对路径。
首先在 E:\Java\workspace\Demo\src\p0\ 目录（E:\Java\workspace\Demo\src\ 是项目源文件的根目录）下创建 Demo.java，输入如下代码：packagep0;
importp1.p2.Test;
publicclassDemo{
publicstaticvoidmain(String[]args){
Testobj=newTest();
}
}

再在 E:\Java\workspace\Demo\src\p1\p2 目录下创建 Test.java，输入如下代码：packagep1.p2;
publicclassTest{
publicTest(){
System.out.println("我是Test类的构造方法");
}
}

假设我们将 classpath 环境变量设置为 .;D:\Program Files\jdk1.7.0_71\lib，源文件 Demo.java 开头有 import p1.p2.Test; 语句，那么编译器会先检查 E:\Java\workspace\Demo\src\p0\p1\p2\
 目录下是否存在 Test.java 或 Test.class 文件，如果不存在，会继续检索 D:\Program Files\jdk1.7.0_71\lib\p1\p2\ 目录，两个目录下都不存在就会报错。显然，Test.java 位于 E:\Java\workspace\Demo\src\p1\p2\ 目录，编译器找不到，会报错，怎么办呢？
可以通过 javac 命令的 classpath 选项来指定类路径。
打开CMD，进入 Demo.java 文件所在目录，执行 javac 命令，并将 classpath 设置为 E:\Java\workspace\Demo\src，如下图所示：
![](http://www.weixueyuan.net/uploads/allimg/141203/1-141203102603222.png)
运行Java程序时，也需要知道类的绝对路径，除了 classpath 环境变量指定的路径，也可以通过 java 命令的 classpath 选项来增加路径，如下图所示：
![](http://www.weixueyuan.net/uploads/allimg/141203/1-141203103J1410.png)
注意 java 命令与 javac 命令的区别，执行 javac 命令需要进入当前目录，而执行 java 命令需要进入当前目录的上级目录，并且类名前面要带上包名。
可以这样来理解，javac是一个平台命令，它对具体的平台文件进行操作，要指明被编译的文件路径。而java是一个虚拟机命令，它对类操作，即对类的描述要用点分的描述形式，并且不能加扩展名，还要注意类名的大小写。
这些命令比较繁杂，实际开发都需要借助 Eclipse，在Eclipse下管理包、编译运行程序都非常方便。Eclipse 实际上也是执行这些命令。
## 包的访问权限
被声明为 public 的类、方法或成员变量，可以被任何包下的任何类使用，而声明为 private 的类、方法或成员变量，只能被本类使用。
没有任何修饰符的类、方法和成员变量，只能被本包中的所有类访问，在包以外任何类都无法访问它。

