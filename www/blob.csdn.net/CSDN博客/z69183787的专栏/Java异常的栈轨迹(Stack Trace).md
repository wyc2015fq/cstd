# Java异常的栈轨迹(Stack Trace) - z69183787的专栏 - CSDN博客
2015年06月26日 07:16:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1980
                
捕获到异常时，往往需要进行一些处理。比较简单直接的方式就是打印异常栈轨迹Stack Trace。说起栈轨迹，可能很多人和我一样，第一反应就是printStackTrace()方法。其实除了这个方法，还有一些别的内容也是和栈轨迹有关的。
     1.printStackTrace()
    首先需要明确，这个方法并不是来自于Exception类。Exception类本身除了定义了几个构造器之外，所有的方法都是从其父类继承过来的。而和异常相关的方法都是从java.lang.Throwable类继承过来的。而printStackTrace()就是其中一个。
     这个方法会将Throwable对象的栈轨迹信息打印到标准错误输出流上。输出的大体样子如下：
```java
java.lang.NullPointerException
         at MyClass.mash(MyClass.java:9)
         at MyClass.crunch(MyClass.java:6)
         at MyClass.main(MyClass.java:3)
```
输出的第一行是toString()方法的输出，后面几行的内容都是之前通过fillInStackTrace()方法保存的内容。关于这个方法，我们后面会讲。
    下面看一个例子：
```java
public class TestPrintStackTrace {
    public static void f() throws Exception{
        throw new Exception("出问题啦！");
    }
    public static void g() throws Exception{
        f();
    }
    public static void main(String[] args) {
        try {
            g();
        }catch(Exception e) {
            e.printStackTrace();
        }
    }
}
```
这个例子的输出如下：
```java
java.lang.Exception: 出问题啦！
    at TestPrintStackTrace.f(TestPrintStackTrace.java:3)
    at TestPrintStackTrace.g(TestPrintStackTrace.java:6)
    at TestPrintStackTrace.main(TestPrintStackTrace.java:10)
```
在这个例子中，在方法f()中抛出异常，方法g()中调用方法f()，在main方法中捕获异常，并且打印栈轨迹信息。因此，输出依次展示了f—>g—>main的过程。
    2.getStackTrace()方法
    这个方法提供了对printStackTrace()方法所打印信息的编程访问。它会返回一个栈轨迹元素的数组。以上面的输出为例，输出的第2-4行每一行的内容对应一个栈轨迹元素。将这些栈轨迹元素保存在一个数组中。每个元素对应栈的一个栈帧。数组的第一个元素保存的是栈顶元素，也就是上面的f。最后一个元素保存的栈底元素。
    下面是一个使用getStackTrace()访问这些轨迹栈元素并打印输出的例子：
```java
public class TestPrintStackTrace {
    public static void f() throws Exception{
        throw new Exception("出问题啦！");
    }
    public static void g() throws Exception{
        f();
    }
    public static void main(String[] args) {
        try {
            g();
        }catch(Exception e) {
            e.printStackTrace();
            System.out.println("------------------------------");
            for(StackTraceElement elem : e.getStackTrace()) {
                System.out.println(elem);
            }
        }
    }
}
```
这样的输出和printStackTrace()的输出基本上是一样的，如下：
```java
java.lang.Exception: 出问题啦！
    at TestPrintStackTrace.f(TestPrintStackTrace.java:3)
    at TestPrintStackTrace.g(TestPrintStackTrace.java:6)
    at TestPrintStackTrace.main(TestPrintStackTrace.java:10)
TestPrintStackTrace.f(TestPrintStackTrace.java:3)
TestPrintStackTrace.g(TestPrintStackTrace.java:6)
TestPrintStackTrace.main(TestPrintStackTrace.java:10)
```
3.fillInStackTrace()
    我们在前面也提到了这个方法。要说清楚这个方法，首先要讲一下捕获异常之后重新抛出的问题。在catch代码块中捕获到异常，打印栈轨迹，又重新throw出去。在上一级的方法调用中，再捕获这个异常并且打印出栈轨迹信息。这两个栈轨迹信息会一样吗？我们看一下代码：
```java
public class TestPrintStackTrace {
    public static void f() throws Exception{
        throw new Exception("出问题啦！");
    }
    public static void g() throws Exception{
        try {
            f();
        }catch(Exception e) {
            e.printStackTrace();
            throw e;
        }
         
    }
    public static void main(String[] args) {
        try {
            g();
        }catch(Exception e) {
            e.printStackTrace();
        }
    }
}
```
 在main方法中捕获的异常，是在g()方法中抛出的，按理说这两个打印栈轨迹的信息应该不同，第二次打印的信息应该没有关于f的信息。但是事实上，两次打印栈轨迹信息是一样的。输出结果如下：
```java
java.lang.Exception: 出问题啦！
    at TestPrintStackTrace.f(TestPrintStackTrace.java:3)
    at TestPrintStackTrace.g(TestPrintStackTrace.java:7)
    at TestPrintStackTrace.main(TestPrintStackTrace.java:16)
java.lang.Exception: 出问题啦！
    at TestPrintStackTrace.f(TestPrintStackTrace.java:3)
    at TestPrintStackTrace.g(TestPrintStackTrace.java:7)
    at TestPrintStackTrace.main(TestPrintStackTrace.java:16)
```
也就是说，捕获到异常又立即抛出，在上级方法调用中再次捕获这个异常，打印的栈轨迹信息是一样的。原因在于没有将当前线程当前状态下的轨迹栈的状态保存进Throwabe中。现在我们引入fillInStackTrace()方法。这个方法刚好做的就是这样的保存工作。我们看一下这个方法的原型：
1
public Throwable fillInStackTrace()
    这个方法是有返回值的。返回的是保存了当前栈轨迹信息的Throwable对象。我们看看使用fillInStackTrace()方法处理后，打印的栈轨迹信息有什么不同，代码如下：
```java
public class TestPrintStackTrace {
    public static void f() throws Exception{
        throw new Exception("出问题啦！");
    }
    public static void g() throws Exception{
        try {
            f();
        }catch(Exception e) {
            e.printStackTrace();
            //不要忘了强制类型转换
            throw (Exception)e.fillInStackTrace();
        }
         
    }
    public static void main(String[] args) {
        try {
            g();
        }catch(Exception e) {
            e.printStackTrace();
        }
    }
}
```
输出如下：
```java
java.lang.Exception: 出问题啦！
    at TestPrintStackTrace.f(TestPrintStackTrace.java:3)
    at TestPrintStackTrace.g(TestPrintStackTrace.java:7)
    at TestPrintStackTrace.main(TestPrintStackTrace.java:17)
java.lang.Exception: 出问题啦！
    at TestPrintStackTrace.g(TestPrintStackTrace.java:11)
    at TestPrintStackTrace.main(TestPrintStackTrace.java:17)
```
我们看到，在main方法中打印栈轨迹已经没有了f相关的信息了。
    以上就是关于Java栈轨迹的一些我之前没有掌握的内容，记下来备忘。 
