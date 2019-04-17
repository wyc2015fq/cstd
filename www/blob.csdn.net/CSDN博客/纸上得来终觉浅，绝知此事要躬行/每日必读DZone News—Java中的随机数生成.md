# 每日必读DZone News—Java中的随机数生成 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月28日 22:50:35[boonya](https://me.csdn.net/boonya)阅读数：144








每日坚持必读，就是紧随时代发展的步伐，技术之路虽艰辛，但终会有所收获。每天进步一小步，程序的世界已然不同。[Java Zone](https://dzone.com/java-jdk-development-tutorials-tools-news)成就每个程序员的不同。英文原文地址：[https://dzone.com/articles/random-number-generation-in-java?edition=347127&utm_source=Daily%20Digest&utm_medium=email&utm_campaign=Daily%20Digest%202017-12-27](https://dzone.com/articles/random-number-generation-in-java?edition=347127&utm_source=Daily%20Digest&utm_medium=email&utm_campaign=Daily%20Digest%202017-12-27)


在你的数字中需要一些随机性？看看几种不同的技巧，你可以用Java创建随机的（或者大部分是随机的）数字。

![](https://dz2cdn1.dzone.com/thumbnail?fid=56856&w=80&h=19) by    

[John Thompson](https://dzone.com/users/2520536/john-1.html)
在开发应用程序时，我们经常需要生成随机数字。Java提供支持来主要通过[java.lang.Math](https://docs.oracle.com/javase/8/docs/api/java/lang/Math.html)和[java.util.Random](https://docs.oracle.com/javase/8/docs/api/java/util/Random.html)类生成随机数。

在这篇文章中，我将讨论根据不同类型的需求生成随机数的不同方法。




## 随机数使用Math类


Java在java.util包中提供了Math类来生成随机数。

Math类包含静态Math.random（）方法来生成double类型的随机数。

random（）方法返回一个具有正号的double值，大于或等于0.0且小于1.0。在调用Math.random（）时，创建并使用一个java.util.Random伪随机数生成器对象。

您可以在传递或不传递参数的情况下使用Math.random（）方法。如果您提供参数，则该方法在给定参数内生成随机数。

使用Math.random（）方法的代码是这样的。




```java
public static double getRandomNumber(){

    double x = Math.random();

    return x;

}
```
getRandomNumber（）方法使用Math.random（）方法返回大于或等于0.0且小于1.0的正整数值。

运行代码的输出是：

`Double between 0.0 and 1.0: SimpleRandomNumber = 0.21753313144345698`

## 在给定范围内的随机数

为了在给定的范围内产生随机数，你需要指定范围。完成这个标准表达式是：



```java
(Math.random() * ((max - min) + 1)) + min
```

让我们把这个表达分解成几个步骤：

首先，将Math.random（）产生的结果的数值范围的大小乘以。

Math.random（）*（max  -  min）返回范围[0，max  -  min]中的一个值，其中不包括max。例如，如果你想[5,10]，你需要覆盖5个整数值，所以你可以使用Math.random（）* 5。这将返回范围[0,5]，其中不包括5的值。
接下来，将此范围移至您定位的范围。您可以通过添加最小值来完成此操作。

（Math.random（）*（max  -  min））+ min

但是这还不包括最大值。

要获得最大值，您需要将1添加到您的范围参数（max - min）。这将在指定范围内返回一个随机double。




```java
double x = (Math.random()*((max-min)+1))+min;
```

有不同的方式来实现上述表达式。让我们看看其中几个。

**在给定的范围内随机双倍**

默认情况下，Math.random（）方法每当调用时都返回一个double类型的随机数。生成指定范围内的随机double值的代码是：



```java
public static double getRandomDoubleBetweenRange(double min, double max){

    double x = (Math.random()*((max-min)+1))+min;

    return x;

}
```
你可以通过传递这样的参数来从main方法调用前面的方法。

System.out.println("Double between 5.0 and 10.00: RandomDoubleNumber = "+getRandomDoubleBetweenRange(5.0, 10.00));输出是这个:


```java
System.out.println("Double between 5.0 and 10.00: RandomDoubleNumber = "+getRandomDoubleBetweenRange(5.0, 10.00));
```


**在给定范围内的随机整数**

生成一个指定范围内的随机整数值的代码是这样的。


```java
public static double getRandomIntegerBetweenRange(double min, double max){

    double x = (int)(Math.random()*((max-min)+1))+min;

    return x;

}
```

前面的getRandomIntegerBetweenRange（）方法在给定范围之间产生一个随机整数。由于Math.random（）方法会生成double类型的随机数，因此需要截断小数部分并将其转换为int以获取整数随机数。您可以通过传递参数来从主方法调用此方法，如下所示：



```java
System.out.println("Integer between 2 and 6: RandomIntegerNumber = "+getRandomIntegerBetweenRange(2,6));
```

输出是这个：


```java
Integer between 2 and 6: RandomIntegerNumber = 5
```
**注意：**您可以传递一个负值的范围来在范围内生成一个随机的负数。

## 随机数生成使用Random类


您可以使用java.util.Random类来生成不同类型的随机数，如int，float，double，long和boolean。

要生成随机数，首先创建Random类的一个实例，然后调用其中一个随机值生成器方法，如nextInt（），nextDouble（）或nextLong（）。

Random的nextInt（）方法接受一个绑定整数，并返回一个从0（包含）到指定的绑定（不包含）的随机整数。

使用nextInt（）方法的代码是这样的。


```java
public static int generateRandomInt(int upperRange){

    Random random = new Random();

    return random.nextInt(upperRange);

}
```
使用nextInt（）方法生成一个范围内的整数的代码是：


```java
public static int generateRandomIntIntRange(int min, int max) {

    Random r = new Random();

    return r.nextInt((max - min) + 1) + min;

}
```

nextFloat（）和nextDouble（）方法允许生成0.0和1.0之间的float和double值。

使用这两种方法的代码是：


```java
public static double generateRandomDouble(){

    Random random = new Random();

    return random.nextDouble();

}

public static float generateRandomFloat(){

    Random random = new Random();

    return random.nextFloat();

}
```


## Java 8中的随机数生成功能


Java 8在java.util.Random类中引入了一个新的方法ints（）。ints（）方法返回一个无限的伪随机int值流。您可以通过提供最小值和最大值来限制指定范围内的随机数。

使用Random.ints（）方法在指定范围内生成随机整数值的代码是这样的。



```java
public static int getRandomNumberInts(int min, int max){

    Random random = new Random();

    return random.ints(min,(max+1)).findFirst().getAsInt();

}
```

getRandomNumberInts（）方法在min（inclusive）和max（exclusive）之间生成一个随机整数流。由于ints（）方法产生一个IntStream，因此代码调用findFirst（）方法，该方法返回描述此流的第一个元素的OptionalInt对象。然后代码调用getAsInt（）方法返回OptionalInt中的int值。

使用Random.ints（）方法生成指定的随机整数值流的代码是：


```java
public static void getStreamOfRandomInts(int num) {

    Random random = new Random();

    random.ints(num).sorted().forEach(System.out::println);

}
```
调用上述方法的代码是：


```java
System.out.println("Random int stream: RandomIntStreamofSize = ");

RandomDemo.getStreamOfRandomInts(5);
```

前面的代码的输出是：



```
Random int stream: RandomIntStreamofSize = 

-1861317227

-1205557317

453883217

762357682

1725970934
```
使用Random.ints（）方法生成指定范围的随机整数值的流的代码是：

```java
public static void getStreamOfRandomIntsWithRange(int num, int min, int max) {

    Random random = new Random();

    random.ints(num,min, max).sorted().forEach(System.out::println);

}
```

调用上述方法的代码是：



```java
System.out.println("Random int stream of specified size in range: RandomIntStreamofSizeInRange = ");

RandomDemo.getStreamOfRandomIntsWithRange(5,0,10);
```

前面的代码的输出是：



```
Random int stream of specified size in range: RandomIntStreamofSizeInRange = 

2

2

3

4

6
```

除了ints（）之外，Java 8引入到Random类的其他一些常用方法（可以返回随机数的连续流）是：

- LongStream longs()
- DoubleStream doubles(）


总结

java.util.Random类实现了通常所说的线性同余发生器（LCG）。它设计得很快，但不符合实时使用的要求。例如，用于Web服务器上唯一的会话ID生成，科学实验，密码学，或涉及金钱利益的抽奖和抽奖活动。对于这种情况，还有其他的选择，我将在后面的文章中介绍。

对于不耐烦的读者，你可以看看[SecureRandom](https://docs.oracle.com/javase/8/docs/api/java/security/SecureRandom.html)类和[Xorshift随机数发生器](https://www.javamex.com/tutorials/random_numbers/xorshift.shtml#.Wg75aEqWbIU)。

此外，一个有趣的资源是[random.org](https://random.org/)，一个真正的随机数字服务，通过大气噪音产生随机性。







