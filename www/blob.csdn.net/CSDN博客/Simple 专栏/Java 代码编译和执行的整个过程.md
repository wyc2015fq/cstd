# Java 代码编译和执行的整个过程 - Simple 专栏 - CSDN博客
2012年02月08日 22:52:34[Simple_Zz](https://me.csdn.net/love284969214)阅读数：6967
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
Java代码编译是由Java源码编译器来完成，流程图如下所示：
![](http://hi.csdn.net/attachment/201009/25/0_1285382319IKd1.gif)
Java字节码的执行是由JVM执行引擎来完成，流程图如下所示：
![](http://hi.csdn.net/attachment/201009/25/0_12853823348XLq.gif)
Java代码编译和执行的整个过程包含了以下三个重要的机制：
- Java源码编译机制
- 类加载机制
- 类执行机制
**Java源码编译机制**
Java 源码编译由以下三个过程组成：
- 分析和输入到符号表
- 注解处理
- 语义分析和生成class文件
流程图如下所示：
![](http://hi.csdn.net/attachment/201009/25/0_1285420122jhjH.gif)
最后生成的class文件由以下部分组成：
- 结构信息。包括class文件格式版本号及各部分的数量与大小的信息
- 元数据。对应于Java源码中声明与常量的信息。包含类/继承的超类/实现的接口的声明信息、域与方法声明信息和常量池
- 方法信息。对应Java源码中语句和表达式对应的信息。包含字节码、异常处理器表、求值栈与局部变量区大小、求值栈的类型记录、调试符号信息
**类加载机制**
JVM的类加载是通过ClassLoader及其子类来完成的，类的层次关系和加载顺序可以由下图来描述：
![](http://hi.csdn.net/attachment/201009/25/0_1285421756PHyZ.gif)
1）Bootstrap ClassLoader
负责加载$JAVA_HOME中jre/lib/rt.jar里所有的class，由C++实现，不是ClassLoader子类
2）Extension ClassLoader
负责加载java平台中扩展功能的一些jar包，包括$JAVA_HOME中jre/lib/*.jar或-Djava.ext.dirs指定目录下的jar包
3）App ClassLoader
负责记载classpath中指定的jar包及目录中class
4）Custom ClassLoader
属于应用程序根据自身需要自定义的ClassLoader，如tomcat、jboss都会根据j2ee规范自行实现ClassLoader
加载过程中会先检查类是否被已加载，检查顺序是自底向上，从Custom ClassLoader到BootStrap ClassLoader逐层检查，只要某个classloader已加载就视为已加载此类，保证此类只所有ClassLoader加载一次。而加载的顺序是自顶向下，也就是由上层来逐层尝试加载此类。
**类执行机制**
JVM是基于栈的体系结构来执行class字节码的。线程创建后，都会产生程序计数器（PC）和栈（Stack），程序计数器存放下一条要执行的指令在方法内的偏移量，栈中存放一个个栈帧，每个栈帧对应着每个方法的每次调用，而栈帧又是有局部变量区和操作数栈两部分组成，局部变量区用于存放方法中的局部变量和参数，操作数栈中用于存放方法执行过程中产生的中间结果。栈的结构如下图所示：
![](http://hi.csdn.net/attachment/201009/25/0_1285425270cQVC.gif)
