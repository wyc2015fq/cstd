# Java - Exception 和 Error 有何区别？

2019年01月12日 17:46:46 [一个优秀的废人](https://me.csdn.net/turodog) 阅读数：47



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/turodog/article/details/86368040

> 微信公众号：一个优秀的废人
> 如有问题或建议，请后台留言，我会尽力解决你的问题。

#### 前言

这周身体不适请了两天病假，只上了三天班。本来不打算发文，但写文章这件事我很想做好。咬咬牙还是坚持下去。废话少说，今天给你们带来的是如何做好 Java 中的异常处理。

#### Exception和Error有什么区别？

要做好异常处理就必须了解 Exception 和 Error 的区别，它们主要有以下异同：

- 首先 Exception 和 Error 都是继承于 Throwable 类，在 Java 中只有 Throwable 类型的实例才可以被抛出（throw）或者捕获（catch），Exception 和 Error 体现了JAVA 这门语言对于异常处理的两种方式。
- Exception 是 Java 程序运行中可预料的异常情况，我们可以获取到这种异常，并且对这种异常进行业务外的处理。它分为检查性异常和非检查性（RuntimeException）异常。两个根本的区别在于，检查性异常必须在编写代码时，使用 try catch 捕获（比如：IOException异常）。非检查性异常 在代码编写时，可以忽略捕获操作（比如：ArrayIndexOutOfBoundsException），这种异常是在代码编写或者使用过程中通过规范可以避免发生的。
- Error 是 Java 程序运行中不可预料的异常情况，这种异常发生以后，会直接导致 JVM 不可处理或者不可恢复的情况。所以这种异常不可能抓取到，比如 OutOfMemoryError、NoClassDefFoundError等。

#### NoClassDefFoundError 和 ClassNotFoundException 有什么区别？

ClassNotFoundException 产生的原因是：

Java 支持使用反射方式在运行时动态加载类，例如使用 Class.forName 方法来动态地加载类时，可以将类名作为参数传递给上述方法从而将指定类加载到 JVM 内存中，如果这个类在类路径中没有被找到，那么此时就会在运行时抛出ClassNotFoundException 异常。

原因：

1. 常见问题在于类名书写错误。
2. 当一个类已经被某个类加载器加载到内存中了，此时另一个类加载器又尝试着动态地从同一个包中加载这个类。通过控制动态类加载过程，可以避免上述情况发生。

NoClassDefFoundError 产生的原因是：

如果 JVM 或者 ClassLoader 实例尝试加载（可以通过正常的方法调用，也可能是使用 new 来创建新的对象）类的时候却找不到类的定义。要查找的类在编译的时候是存在的，运行的时候却找不到了。这个时候就会导致 NoClassDefFoundError。

原因：

1. 打包过程漏掉了部分类。
2. jar包出现损坏或者篡改。

解决方法：
查找那些在开发期间存在于类路径下，但在运行期间却不在类路径下的类。

#### 如何做好异常处理？

1. 理解 Throwable、Exception、Error 的设计和分类。

![异常的设计与分类](https://upload-images.jianshu.io/upload_images/3282134-6a9f901abb3c9a37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

关于上图，我举个形象的例子帮助你理解。

假如你开车上山，车坏了，你拿出工具箱修一修，修好继续上路（Exception 被捕获，从异常中恢复，继续程序的运行），车坏了，你不知道怎么修，打电话告诉修车行是什么问题，要车行过来修。（在当前的逻辑背景下，你不知道是怎么样的处理逻辑，把异常抛出去到更高的业务层来处理）。你打电话的时候，要尽量具体，不能只说我车动不了了。那修车行很难定位你的问题。（要捕获特定的异常，不能捕获类似Exception 的通用异常）。还有一种情况是，你开车上山，山塌了，这你还能修吗？（Error：导致你的运行环境进入不正常的状态，很难恢复）

1. 理解 Java 语言中操作 Throwable 的元素和实践。

掌握最基本的语法是必须的，如 try-catch-finally 块，throw、throws 关键字等。与此同时，也要懂得如何处理典型场景。

throw 是存在于方法的代码块中，而throws是存在于方法外围，一般是在方法名后边 throws XXXException。

比如，那些继承了 AutoCloseable 或者 Closeable 的对象，我们可以使用 try-with-resource（我们不需要手动关闭资源） 和 multiple catch（捕获多个异常） 的方式进行异常处理。

```
try (BufferedReader br = new BufferedReader(…);
     BufferedWriter writer = new BufferedWriter(…)) {// Try-with-resources
// do something
catch ( IOException | XEception e) {// Multiple catch
   // Handle it
} 
123456
```

又比如我们要注意 try-catch-finally 中 rerun 的执行顺序问题，如：

```
// 实例一
try{
    retrun 3;
}catch{
    e.printStackTrace();
}finally{
    return 4;
}

// 实例二
try{
    int x = 3;
    retrun x;
}catch{
    e.printStackTrace();
}finally{
    x++;
}
123456789101112131415161718
```

以上代码实例一，返回的是 4 ；实例二的返回结果是 3。因为 finally 的业务操作是在try 业务操作的 return 返回调用者之前执行。按照实例一，实际情况是，执行完 try 中的业务逻辑 return 返回的操作会先存储到一个临时的堆栈中，此时不给调用者返回，随后执行 finally 中的业务代码。如果 finally 中有return操作，那么就会把 finally 中的return 值与 try 中的 return 值进行替换。随后将最终数据返回给调用者。

#### 异常处理中哪些不当之处？

1. 尽量不要捕获类似 Exception 这样的通用异常，而是应该捕获特定异常，如下代码：

```
try {
  // 业务代码
  // …
  Thread.sleep(1000L);
} catch (Exception e) {
  // Ignore it
}
1234567
```

在这里是 Thread.sleep() 抛出的 InterruptedException。我们应该捕获这个特定的异常，因为在开发中，我们读代码的时间远远超过写代码，所以我们有义务让自己的代码能够直观地体现出尽量多的信息，而泛泛的 Exception 之类，恰恰隐藏了我们的目的。另外，我们也要保证程序不会捕获到我们不希望捕获的异常。比如，你可能更希望 RuntimeException 被扩散出来，而不是被捕获。

进一步讲，除非深思熟虑了，否则不要捕获 Throwable 或者 Error，这样很难保证我们能够正确程序处理 OutOfMemoryError。

1. 不要生吞（swallow）异常

有以下代码：

```
try {
   // 业务代码
   // …
} catch (IOException e) {
    e.printStackTrace();
}
123456
```

以上代码中华，如果我们不把异常抛出来，或者也没有输出到日志（Logger）之类，程序可能在后续代码以不可控的方式结束。没人能够轻易判断究竟是哪里抛出了异常，以及是什么原因产生了异常。这就叫生吞异常。

他的问题出在，异常中的异常日志如何输出的问题。按照上边的输出，如果实在复杂的系统中，会判断不出来异常具体在哪里打印出来的。

尤其是对于分布式系统，如果发生异常，但是无法找到堆栈轨迹（stacktrace），这纯属是为诊断设置障碍。所以，最好使用产品日志，详细地输出到日志系统里。

#### Throw early, catch late 原则

有以上代码：

```
public void readPreferences(String fileName){
	 //...perform operations... 
	InputStream in = new FileInputStream(fileName);
	 //...read the preferences file...
}
12345
```

如果 fileName 是 null，那么程序就会抛出 NullPointException，但是由于没有第一时间暴露出问题，堆栈信息可能非常令人费解，往往需要相对复杂的定位。这个 NPE 只是作为例子，实际产品代码，可能是各种情况，比如配置获取失败等。在发现问题时，第一时间抛出，能更清晰地反映问题。

我们可以修改下，让问题 “throw early”， 对应的异常信息就非常直观了，如：

```
public void readPreferences(String filename) {
	Objects. requireNonNull(filename);
	//...perform other operations... 
	InputStream in = new FileInputStream(filename);
	 //...read the preferences file...
}
123456
```

至于“catch late”，其实是我们经常苦恼的问题，捕获异常后，需要怎么处理呢？ 有很多人，就像上面提到的那样直接生吞异常，本质上其实是掩耳盗铃。如果实在不知道如何处理，可以选择保留原有异常的 cause 信息，直接再抛出或者构建新的异常抛出去。在更高层面，因为有了清晰的（业务）逻辑，往往会更清楚合适的处理方式是什么。

#### 如何自定义异常

如何自定义异常，包括以下两点需要考虑：

1. 是否需要定义成 Checked Exception，因为这种类型设计的初衷更是为了从异常情况恢复，作为异常设计者，我们往往有充足信息进行分类。
2. 在保证诊断信息足够的同时，也要考虑避免包含敏感信息，因为那样可能导致潜在的安全问题。如果我们看 Java 的标准类库，你可能注意到类似 java.net.ConnectException，出错信息是类似“ Connection refused (Connection refused)”，而不包含具体的机器名、IP、端口等，一个重要考量就是信息安全。类似的情况在日志中也有，比如，用户数据一般是不可以输出到日志里面的。

#### 异常处理机制的性能

1. try-catch 代码段会产生额外的性能开销，或者换个角度说，它往往会影响 JVM 对代码进行优化，所以建议仅捕获有必要的代码段，尽量不要一个大的 try 包住整段的代码；与此同时，利用异常控制代码流程，也不是一个好主意，远比我们通常意义上的条件语句（if/else、switch）要低效。
2. java 每实例化一个 Exception，都会对当时的栈进行快照，这是一个相对比较重的操作。如果发生的非常频繁，这个开销可就不能被忽略了。

所以当我们的服务出现反应变慢、吞吐量下降的时候，检查发生最频繁的 Exception 也是一种思路。

#### 后语

以上就是我对 Java 异常处理的理解，希望对你们有帮助。最后，对 Python 、Java 感兴趣请长按二维码关注一波，我会努力带给你们价值，如果觉得本文对你哪怕有一丁点帮助，请帮忙点好看，让更多人知道。