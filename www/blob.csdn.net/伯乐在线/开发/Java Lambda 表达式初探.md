# Java Lambda 表达式初探 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [CarpenterLee](http://www.jobbole.com/members/CarpenterLee) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 前言
本文受启发于[Trisha Gee](http://trishagee.github.io/)在JavaOne 2016的主题演讲[Refactoring to Java 8](https://www.oracle.com/javaone/speakers.html#gee)。
Java 8已经发行两年多，但很多人仍然在使用JDK7。对企业来说，技术上谨慎未必是坏事，但对个人学习而言，不去学习新技术就很可能被技术抛弃。Java 8一个重要的变更是引入**Lambda表达式**(`lambda expression`)，这听起来似乎很牛，有种我虽然不知道Lambda表达式是什么，但我仍然觉得很厉害的感觉。不要怕，具体到语言层面上Lambda表达式不过是一种新的语法而已，有了它，Java将开启函数式编程的大门。
# 为什么需要Lambda表达式
不要纠结什么是*Lambda*表达式、什么是函数式编程。先来看一下Java 8新的语法特性带来的便利之处，相信你会过目不忘的。
在有*Lambda*表达式之前，要新建一个线程，需要这样写：

```
new Thread(new Runnable(){
    @Override
    public void run(){
        System.out.println("Thread run()");
    }
}).start();
```
有*Lambda*表达式之后，则可以这样写：

```
new Thread(
        () -> System.out.println("Thread run()")
).start();
```
正如你所见，之前无用的模板代码不见了！如上所示，**Lambda表达式一个常见的用法是取代（某些）匿名内部类**，但Lambda表达式的作用不限于此。
# Lambda表达式的原理
刚接触*Lambda*表达式可能觉得它很神奇：*不需要声明类或者方法的名字，就可以直接定义函数*。这看似是编译器为匿名内部类简写提供的一个小把戏，但事实上并非如此，*Lambda*表达式实际上是通过*invokedynamic*指令来实现的。先别管这么多，下面是*Lambda*表达式几种可能的书写形式，“看起来”并不是很难理解。

```
Runnable run = () -> System.out.println("Hello World");// 1
ActionListener listener = event -> System.out.println("button clicked");// 2
Runnable multiLine = () -> {// 3
    System.out.println("Hello ");
    System.out.println("World");
};
BinaryOperator<Long> add = (Long x, Long y) -> x + y;// 4
BinaryOperator<Long> addImplicit = (x, y) -> x + y;// 5
```
通过上例可以发现：
> - Lambda表达式是有类型的，赋值操作的左边就是类型。Lambda表达式的类型实际上是**对应接口的类型**。
- Lambda表达式可以包含多行代码，需要用大括号把代码块括起来，就像写函数体那样。
- 大多数时候，Lambda表达式的参数表可以省略类型，就像代码2和5那样。这得益于javac的**类型推导**机制，编译器可以根据上下文推导出类型信息。
**表面上看起来**每个*Lambda表达式都是原来匿名内部类的简写形式，该内部类实现了某个**函数接口**(`Functional Interface`)*，但事实比这稍微复杂一些，这里不再展开。所谓函数接口是指内部只有一个接口函数的接口。Java是强类型语言，无论有没有显式指明，每个变量和对象都必须有明确的类型，没有显式指定的时候编译器会尝试确定类型。**Lambda表达式的类型就是对应函数接口的类型**。
# Lambda表达式和Stream
**Lambda表达式的另一个重要用法，是和Stream一起使用**。Stream is a sequence of elements supporting sequential and parallel aggregate operations。Stream就是一组元素的序列，支持对这些元素进行各种操作，而这些操作是通过*Lambda*表达式指定的。可以把Stream看作Java Collection的一种视图，就像迭代器是容器的一种视图那样（但*Stream*不会修改容器中的内容）。下面例子展示了*Stream*的常见用法。
## 例子1
假设需要从一个字符串列表中选出以数字开头的字符串并输出，Java 7之前需要这样写：

```
List<String> list = Arrays.asList("1one", "two", "three", "4four");
for(String str : list){
    if(Character.isDigit(str.charAt(0))){
        System.out.println(str);
    }
}
```
而Java 8就可以这样写：

```
List<String> list = Arrays.asList("1one", "two", "three", "4four");
list.stream()// 1.得到容器的Steam
    .filter(str -> Character.isDigit(str.charAt(0)))// 2.选出以数字开头的字符串
    .forEach(str -> System.out.println(str));// 3.输出字符串
```
上述代码首先1. 调用`List.stream()`方法得到容器的*Stream*，2. 然后调用`filter()`方法过滤出以数字开头的字符串，3. 最后调用`forEach()`方法输出结果。
使用Stream有两个明显的好处：
> - 减少了模板代码，只用Lambda表达式指明所需操作，代码语义更加明确、便于阅读。
- 将外部迭代改成了Stream的内部迭代，方便了JVM本身对迭代过程做优化（比如可以并行迭代）。
## 例子2
假设需要从一个字符串列表中，选出所有不以数字开头的字符串，将其转换成大写形式，并把结果放到新的集合当中。Java 8书写的代码如下：

```
List<String> list = Arrays.asList("1one", "two", "three", "4four");
Set<String> newList =
        list.stream()// 1.得到容器的Stream
        .filter(str -> !Character.isDigit(str.charAt(0)))// 2.选出不以数字开头的字符串
        .map(String::toUpperCase)// 3.转换成大写形式
        .collect(Collectors.toSet());// 4.生成结果集
```
上述代码首先1. 调用`List.stream()`方法得到容器的*Stream*，2. 然后调用`filter()`方法选出不以数字开头的字符串，3. 之后调用`map()`方法将字符串转换成大写形式，4. 最后调用`collect()`方法将结果转换成`Set`。这个例子还向我们展示了**方法引用**（`method references`，代码中标号3处）以及**收集器**（`Collector`，代码中标号4处）的用法，这里不再展开说明。
通过这个例子我们看到了*Stream*链式操作，即多个操作可以连成一串。不用担心这会导致对容器的多次迭代，因为不是每个*Stream*的操作都会立即执行。*Stream*的操作分成两类，一类是**中间操作**(`intermediate operations`)，另一类是**结束操作**(`terminal operation`)，只有结束操作才会导致真正的代码执行，中间操作只会做一些标记，表示需要对*Stream*进行某种操作。这意味着可以在*Stream*上通过关联多种操作，但最终只需要一次迭代。如果你熟悉Spark RDD，对此应该并不陌生。
# 结语
Java 8引入*Lambda*表达式，从此打开了函数式编程的大门。如果你之前不了解函数式编程，不必纠结于这个概念。编程过程中简洁明了的书写形式以及强大的*Stream API*会让你很快熟悉*Lambda*表达式的。
本文只对*Java Lambda*表达式的基本介绍，希望能够激发读者对Java函数式编程的兴趣。如果本文能够让你觉得*Lambda*表达式很好玩，函数式编程很有趣，并产生了进一步学习的欲望，那就再好不过了。文末参考文献中列出了一些有用的资源。
# 参考文献
[http://www.oracle.com/webfolder/technetwork/tutorials/obe/java/Lambda-QuickStart/index.html](http://www.oracle.com/webfolder/technetwork/tutorials/obe/java/Lambda-QuickStart/index.html)
[https://docs.oracle.com/javase/8/docs/api/java/util/stream/Stream.html](https://docs.oracle.com/javase/8/docs/api/java/util/stream/Stream.html)
[https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html](https://docs.oracle.com/javase/tutorial/java/javaOO/lambdaexpressions.html)
[http://cr.openjdk.java.net/~briangoetz/lambda/lambda-state-final.html](http://cr.openjdk.java.net/~briangoetz/lambda/lambda-state-final.html)
[http://cr.openjdk.java.net/~briangoetz/lambda/lambda-translation.html](http://cr.openjdk.java.net/~briangoetz/lambda/lambda-translation.html)
[http://www.slideshare.net/trishagee/refactoring-to-java-8-devoxx-uk](http://www.slideshare.net/trishagee/refactoring-to-java-8-devoxx-uk)
[《Java 8函数式编程 [英]沃伯顿》](https://www.amazon.cn/Java-8%E5%87%BD%E6%95%B0%E5%BC%8F%E7%BC%96%E7%A8%8B-%E8%8B%B1-%E6%B2%83%E4%BC%AF%E9%A1%BF/dp/B00VDSW7AE)
[https://www.oracle.com/javaone/speakers.html#gee](https://www.oracle.com/javaone/speakers.html#gee)
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/06/920eeac2120bccf53adae72ea318b7ce.png)
