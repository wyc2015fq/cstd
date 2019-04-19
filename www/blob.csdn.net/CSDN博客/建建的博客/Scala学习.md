# Scala学习 - 建建的博客 - CSDN博客
2017年03月24日 11:39:04[纪建](https://me.csdn.net/u013898698)阅读数：179
个人分类：[Scala](https://blog.csdn.net/u013898698/article/category/6814508)
## 本节主要内容
- Scala简介
- 为什么要学习Scala
- Scala语言初步
## 1. Scala简介
[Scala](http://lib.csdn.net/base/scala)(Scala Language的简称)语言是一种能够运行于JVM和.Net平台之上的通用编程语言，既可用于大规模应用程序开发，也可用于脚本编程，它由由Martin
 Odersk于2001开发，2004年开始程序运行在JVM与.Net平台之上，由于其简洁、优雅、类型安全的编程模式而受到关注。 
![这里写图片描述](https://img-blog.csdn.net/20150818083506939)
Scala的创建者——Martin Odersk
在Scala的创建之初，并没有怎么引起重视，随着[Apache Spark](http://lib.csdn.net/base/spark)和Apache Kafka这样基于Scala的[大数据](http://lib.csdn.net/base/hadoop)框架的崛起，Scala逐步映入大数据从业者的眼帘。Scala的拥护者们认为Scala的主要优势是速度和它的表达性。目前使用scala的作为支撑公司开发语言的包括Foursquare和Twitter。2009年Twitter把大部分后台系统的开发语言从Ruby换成了Scala。参见这篇文章：Twitter
 on Scala: A Conversation with Steve Jenson, Alex Payne, and Robey Pointer,” Scalazine, April 3,2009, www.artima.com/scalazine/articles/twitter_on_scala.html.
Scala语言具有如下特点：
1 纯面向对象编程语言
- （1） Encapsulation/information hiding. 
- （2）Inheritance. 
- （3）Polymorphism/dynamic binding. 
- （4）All predefined types are objects. 
- （5） All operations are performed by sending messages to objects. 
- （6）All user-defined types are objects.
2 函数式编程语言 
定义：Functional programming is a programming paradigm that treats computation as the evaluation of mathematical functions and avoids state and mutable data.
函数式编程语言应支持以下特性： 
（1）高阶函数（Higher-order functions） 
（2）闭包（ closures） 
（3）模式匹配（ Pattern matching） 
（4）单一赋值（ Single assignment ） 
（5）延迟计算（ Lazy evaluation） 
（6）类型推导（ Type inference ） 
（7）尾部调用优化（ Tail call optimization） 
（8）类型推导（ Type inference ）
3 Scala语言具有很强的兼容性、移植性 
Scala运行于JVM上，能够与[Java](http://lib.csdn.net/base/javase)进行互操作，具有与JAVA一样的平台移植性
4 Scala语法的简洁 
下面给的是java的[Hadoop](http://lib.csdn.net/base/hadoop) wordcount代码及[Spark](http://lib.csdn.net/base/spark) wordcount代码 
![这里写图片描述](https://img-blog.csdn.net/20150818085713482)
可以看到,spark三行代码就解决了hadoop 七八十行代码做的事情。
## 2. 为什么要学习Scala
1 开源大数据内存计算框架Spark的流行
- Spark是当前最流行的开源大数据内存计算框架，采用Scala语言实现，由UC 伯克利大学AMPLab实验室开发（2009）并于2010年开源，在2014年成为Apache基金会的顶级项目[http://spark.apache.org/](http://spark.apache.org/)
- Spark有着很好的性能优势。 
![这里写图片描述](https://img-blog.csdn.net/20150818090130421)
图片来源：databricks.com/blog/2014/11/05/spark-officiallysets-a-new-record-in-large-scale-sorting.html
- 
社区活跃度 
![这里写图片描述](https://img-blog.csdn.net/20150818090321363)
图片来源：Real-Time Analytics with Spark Streaming 
![这里写图片描述](https://img-blog.csdn.net/20150818091033579)
图片来源：Spark Summit 2015 
![这里写图片描述](https://img-blog.csdn.net/20150818091137843)
图片来源:twitter.com/dberkholz/status/568561792751771648 
- 
各大公司使用与贡献情况 
![这里写图片描述](https://img-blog.csdn.net/20150818091456225)
![这里写图片描述](https://img-blog.csdn.net/20150818091614451)
图片来源：Summit Spark 2015 [https://spark-summit.org/2015/](https://spark-summit.org/2015/)
- IBM 百万数据工程师计划 
【2015年6月17日，北京】IBM(NYSE:IBM)宣布承诺大力推进Apache Spark项目，并称该项目为：在以数据为主导的，未来十年最为重要的新的开源项目。这一承诺的核心是将Spark嵌入IBM业内领先的分析和商务平台，并将Spark作为一项服务，在IBM [Bluemix](http://lib.csdn.net/base/bluemix)平台上提供给客户。IBM还将投入超过3500名研究和开发人员在全球十余个实验室开展与Spark相关的项目，并将为Spark开源生态系统无偿提供突破性的[机器学习](http://lib.csdn.net/base/machinelearning)技术——IBM
 SystemML，同时，IBM还将培养超过100万名Spark数据科学家和数据工程师。原文链接：[http://www.csdn.net/article/a/2015-06-18/15825412](http://www.csdn.net/article/a/2015-06-18/15825412)
2 Scala是未来大数据处理的主流语言
- 它是Spark框架的开发语言
- - ”If I were to pick a language to use today other than Java, it would be Scala.” —James Gosling 
![这里写图片描述](https://img-blog.csdn.net/20150818091848885)
詹姆斯·高斯林 Java之父
- 
Scala具有数据处理的天然优势（语言特点决定）
## 3. Scala语言初步
1 变量定义
```
//声明一个val变量
//与java final关键字声明的变量一样
//一旦被赋值，便不能更改
//Scala会帮我们进行类型推断
scala> val helloString="Hello World"
helloString: String = Hello World
//也可以进行类型指定
scala> val helloString:String="Hello World"
helloString: String = Hello World
//String其实就是java.lang.String
scala> val helloString:java.lang.String="Hello World"
helloString: String = Hello World
//不能被重新赋值，因为它是val变量
scala> helloString="Hello Crazy World"
<console>:8: error: reassignment to val
       helloString="Hello Crazy World"
                  ^
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
下面给出的延迟加载变量：
```
//lazy关键字声明变量
//表示该变量不会马上赋值
//而在真正被使用时才赋值
scala> lazy val helloString="Hello Crazy World"
helloString: String = <lazy>
//在真正使用时被赋值
scala> helloString
res1: String = Hello Crazy World
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
scala中也存在可变变量，即随着程序的运行，变量内容可以动态变化：
```java
//var 声明可变变量
scala> var helloString="Hello Cruel World"
helloString: String = Hello Cruel World
//重新赋值
scala>  helloString="GoodBye Cruel World"
helloString: String = GoodBye Cruel World
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
2 函数初步
scala中通过下列方式进行函数定义： 
![这里写图片描述](https://img-blog.csdn.net/20150818092829083)
```
//定义了一个函数，函数中使用return返回结果
scala> def add(a:Int,b:Int):Int={return a+b}
add: (a: Int, b: Int)Int
scala> add(1,2)
res3: Int = 3
//可以省去return，scala会将最后一个执行语句
//作为函数的返回值
scala> def add(a:Int,b:Int):Int={a+b}
add: (a: Int, b: Int)Int
//省去返回值类型，scala会自动进行类型推断
scala> def add(a:Int,b:Int)={a+b}
add: (a: Int, b: Int)Int
scala> add(1,2)
res4: Int = 3
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
3 HelloWorld应用程序：
```python
package cn.xtwy.scala.chapter01
//scala应用程序同样采用main方法作为应用程序的入口
object HelloWorld {
  def main(args: Array[String]): Unit = {
    println("Hello World")
  }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
添加公众[微信](http://lib.csdn.net/base/wechat)号，可以了解更多最新Spark、Scala相关技术资讯 
![这里写图片描述](https://img-blog.csdn.net/20150719143658635)
