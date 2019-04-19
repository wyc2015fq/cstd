# Java中的异常处理：何时抛出异常，何时捕获异常 - 零度的博客专栏 - CSDN博客
2018年07月04日 19:19:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：1390
今天在看hadoop源码时，想想自己最近在做的那个系统，发现很多异常处理的方式不对，还是按照传统的异常处理方式（即：采用返回值来标识程序出现的异常情况）。而hadoop中很多方法的声明是有异常抛出的，而我的系统中的很多方法的声明都没有抛出异常。只是判断了异常情况，并输出了错误提示，但是并没有抛出异常。
org.apache.hadoop.hdfs.protocol包下的Block类的readFields()方法：
```
public void readFields(DataInput in) throws IOException {
    this.blockId = in.readLong();
    this.numBytes = in.readLong();
    this.generationStamp = in.readLong();
    if (numBytes < 0) {
      throw new IOException("Unexpected block size: " + numBytes);//抛出异常，要是的话就不会抛出，而只是System.out.println错误提示，
    }
```
 1.如果方法声明名里面有throws异常，那么方法体里面可以不抛出异常。因为可以在方法声明中包含异常说明，但实际上却不抛出！这样做的好处是，为异常先占个位置，以后就可以抛出这种异常而不用修改修改已有的代码。在定义抽象基类和接口时这种能力很重要，这样派生类或接口实现类就能够抛出这些预先声明的异常。
2.为什么有的方法声明里面没有throws，但方法体里面却抛出了异常？从RuntimeException继承的异常，可以在没有异常说明throws的情况下被抛出！对于Runtime异常（也称为非检查的异常unchecked exception），编译器不需要异常说明。只能在代码中忽略RuntimeException（及其子类）类型的异常，其他类型的异常的处理都是由编译器强制实施的。究其原因，RuntimeException代表的是编程错误。
3.运行时异常会被Java虚拟机自动抛出！
《有效处理Java异常三原则》[http://www.importnew.com/1701.html](http://www.importnew.com/1701.html)
1. 异常处理基础
     1.1 System.out.println是高代价的。调用System.out.println会降低系统吞吐量。
     1.2 在生产环境中别用异常的printStackTrace()方法。printStackTrace默认会把调用的堆栈打印到控制台上，在生产环境中访问控制台是不现实的。
2. 异常处理基本原则
     2.1 如果你不能处理异常，不要捕获该异常。
     2.2 如果要捕获，应在离异常源近的地方捕获它。
     2.3 不要吞没你捕获的异常。
     *（就是捕获的异常，但是什么也不做）
     2.4 除非你要重新抛出异常，否则把它log起来。
     2.5 当一个异常被重新包装，然后重新抛出的时候，不要打印statck trace。
     2.6 用自定义的异常类，不要每次需要抛出异常的时候都抛出java.lang.Exception。方法的调用者可以通过throws知道有哪些异常需要处理--所以它是自我描述的。
     2.7 如果你编写业务逻辑，对于终端用户无法修复的错误，系统应该抛出非检查的异常（unchecked exception）；如果你编写一个第三方的包给其他的开发人员用，对于不可修复的错误要用需要检查的异常（checked exception）。
    2.8 绝对不要因为写throws语句会让你用起来不舒服，而不声明需要检查的异常。
    2.9 应用级别的错误或不可修复的系统异常用非检查的异常（unchecked exception）抛出。
    *（注意是错误，意味着不可修复，比如配置文件错误）
    2.10 根据异常的粒度组织你的方法
