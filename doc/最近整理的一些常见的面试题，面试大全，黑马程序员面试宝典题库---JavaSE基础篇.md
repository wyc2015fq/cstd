# 最近整理的一些常见的面试题，面试大全，黑马程序员面试宝典题库---JavaSE基础篇

2018年08月21日 21:11:52 [高数老师](https://me.csdn.net/qq_29073921) 阅读数：1152



 版权声明：代码共享，欢迎copy	https://blog.csdn.net/qq_29073921/article/details/81914039

# JAVA语法（左侧扫描二维码，留言“黑马面试”，自动发原件给你）

## 1.抽象类(abstract class)和接口(interface)有什么异同？

不同：
抽象类：
1.抽象类中可以定义构造器
2.可以有抽象方法和具体方法
3.接口中的成员全都是 public 的
4.抽象类中可以定义成员变量
5.有抽象方法的类必须被声明为抽象类，而抽象类未必要有抽象方法
6.抽象类中可以包含静态方法
7.一个类只能继承一个抽象类
接口：
1.接口中不能定义构造器
2.方法全部都是抽象方法
3.抽象类中的成员可以是 private、默认、 protected、 public
4.接口中定义的成员变量实际上都是常量
5.接口中不能有静态方法
6.一个类可以实现多个接口
相同：
1.不能够实例化
2.可以将抽象类和接口类型作为引用类型
3.一个类如果继承了某个抽象类或者实现了某个接口都需要对其中的抽象方法全部进行实现， 否则该
类仍然需要被声明为抽象类

##  2.重载（overload ）和重写（override ）的区别。重载的方法能否根据返回类型进行区分？  

方法的重载和重写都是实现多态的方式，区别在于前者实现的是编译时的多态性，而后者实现的是运行时的多态性。

重载发生在一个类中，同名的方法如果有不同的参数列表（参数类型不同、参数个数不同或者二者都不同）则视为重载；

重写发生在子类与父类之间，重写要求子类被重写方法与父类被重写方法有相同的返回类型，比父类被重写方法更好访问，不能比父类被重写方法声明更多的异常（里氏代换原则）。重载对返回类型没有特殊的要求。
方法重载的规则：
1.方法名一致，参数列表中参数的顺序，类型，个数不同。
2.重载与方法的返回值无关，存在于父类和子类，同类中。
3.可以抛出不同的异常，可以有不同修饰符。
方法重写的规则：
1.参数列表必须完全与被重写方法的一致，返回类型必须完全与被重写方法的返回类型一致。
2.构造方法不能被重写，声明为 final 的方法不能被重写，声明为 static 的方法不能被重写，但是能够被再次声明。
3.访问权限不能比父类中被重写的方法的访问权限更高。
4.重写的方法能够抛出任何非强制异常，无论被重写的方法是否抛出异常。但是，重写的方法不能抛出新的强制性异常，或者比被重写方法声明的更广泛的强制性异常，反之则可以。
 

 

## 3.==和equals的区别

equals 和== 最大的区别是一个是方法一个是运算符。
==：如果比较的对象是基本数据类型，则比较的是数值是否相等；如果比较的是引用数据类型，则比较的是对象的地址值是否相等。
equals()：用来比较方法两个对象的内容是否相等。（String的方法）
注意： equals 方法不能用于基本数据类型的变量，如果没有对 equals 方法进行重写，则比较的是引用类型的变量所指向的对象的地址。
 

 

## 4.& 和 && 的区别

&运算符有两种用法： (1)按位与； (2)逻辑与。


&&运算符是短路与运算。逻辑与跟短路与的差别是非常巨大的，虽然二者都要求运算符左右两端的布尔值都是true 整个表达式的值才是 true。
&&之所以称为短路运算是因为，如果&&左边的表达式的值是 false，右边的表达式会被直接短路掉，不会进行运算。很多时候我们可能都需要用&&而不是&，例如在验证用户登录时判定用户名不是 null 而且不是空字符串，应当写为
username != null &&!username.equals("")，二者的顺序不能交换，更不能用&运算符，因为第一个条件如果不成立，根本不能进行字符串的 equals 比较，否则会产生 NullPointerException 异常。注意：逻辑或运算符（|） 和短路或运算符（||）的差别也是如此。

##  

## 5.如何跳出当前的多重嵌套循环  

在最外层循环前加一个标记如 A，然后用 break A;可以跳出多重循环。

 

## 6.是否可以继承 String

String 类是 final 类，不可以被继承。
补充：继承 String 本身就是一个错误的行为，对 String 类型最好的重用方式是关联关系（Has-A）和依赖关系（Use-A）而不是继承关系（Is-A）
 

## 7.当一个对象被当作参数传递到一个方法后，此方法可改变这个对象的属性，并可返回变化后的结果，那么这里到底是值传递还是引用传递?

是值传递。 Java 语言的方法调用只支持参数的值传递。当一个对象实例作为一个参数被传递到方法中时，参数的
值就是对该对象的引用。对象的属性可以在被调用过程中被改变，但对对象引用的改变是不会影响到调用者的。 C++
和 C#中可以通过传引用或传输出参数来改变传入的参数的值。说明： Java 中没有传引用实在是非常的不方便，这一
点在 Java 8 中仍然没有得到改进，正是如此在 Java 编写的代码中才会出现大量的 Wrapper 类（将需要通过方法
调用修改的引用置于一个 Wrapper 类中，再将 Wrapper 对象传入方法），这样的做法只会让代码变得臃肿，尤其
是让从 C 和 C++转型为 Java 程序员的开发者无法容忍
 

## 8.char 型变量中能不能存贮一个中文汉字，为什么？

char 类型可以存储一个中文汉字，因为 Java 中使用的编码是 Unicode（不选择任何特定的编码，直接使用字符在字符集中的编号，这是统一的唯一方法），一个 char 类型占 2 个字节（16 比特），所以放一个中文是没问题的。
 

## 9.阐述静态变量和实例变量的区别？

静态变量: 是被 static 修饰符修饰的变量，也称为类变量，它属于类，不属于类的任何一个对象，一个类不管创建多少个对象，静态变量在内存中有且仅有一个拷贝；
实例变量: 必须依存于某一实例，需要先创建对象然后通过对象才能访问到它。静态变量可以实现让多个对象共享内存。
 

## 10.break 和 continue 的区别？

break 和 continue 都是用来控制循环的语句。
break 用于完全结束一个循环，跳出循环体执行循环后面的语句；
continue 用于跳过本次循环，执行下次循环。

## 11.String s = "Hello";s = s + " world!";这两行代码执行后，原始的 String 对象中的内容到底变了没有？

没有。因为 String 被设计成不可变(immutable)类，所以它的所有对象都是不可变对象。在这段代码中， s 原先指
向一个 String 对象，内容是 "Hello"，然后我们对 s 进行了“+” 操作，那么 s 所指向的那个对象是否发生了改变呢？
答案是没有。这时， s 不指向原来那个对象了，而指向了另一个 String 对象，内容为"Hello world!"，原来那个对象还
存在于内存之中，只是 s 这个引用变量不再指向它了。
建议使用StringBuffer。

# java多态

## 12.Java 中实现多态的机制是什么？

靠的是父类或接口定义的引用变量可以指向子类或具体实现类的实例对象，而程序调用的方法在运行期才动态绑定，就是引用变量所指向的具体实例对象的方法，也就是内存里正在运行的那个对象的方法，而不是引用变量的类型中定义的方法。

#  java的异常处理

13.Java 中异常分为哪些种类

按 照 异 常 需 要 处 理 的 时 机 分 为 编 译 时 异 常 也 叫 CheckedException 和 运 行 时 异 常 也 叫RuntimeException
异常处理方法有两种：
1 当前方法知道如何处理该异常，则用 try...catch 块来处理该异常。

2 当前方法不知道如何处理，则在定义该方法是声明抛出该异常。


运行时异常只有当代码在运行时才发行的异常，编译时不需要 try catch。 Runtime 如除数是 0 和数组下标越界等，其产生频繁，处理麻烦，若显示申明或者捕获将会对程序的可读性和运行效率影响很大。所以由系统自动检测并将它们交给缺省的异常处理程序。当然如果你有处理要求也可以显示捕获它们。
 

## 14.error 和 exception 的区别

Error 类和 Exception 类的父类都是 Throwable 类，他们的区别是：
Error 类一般是指与虚拟机相关的问题，如系统崩溃，虚拟机错误，内存空间不足，方法调用栈溢等。对于这类错误的导致的应用程序中断，仅靠程序本身无法恢复和和预防，遇到这样的错误，建议让程序终止。
Exception 类表示程序可以处理的异常，可以捕获且可能恢复。遇到这类异常，应该尽可能处理异常，使程序恢复运行，而不应该随意终止异常。
Exception 类又分为运行时异常（Runtime Exception）和受检查的异常(Checked Exception )，运行时异常;ArithmaticException,IllegalArgumentException，编译能通过，但是一运行就终止了，程序不会处理运行时异常，出现这类异常，程序会终止。而受检查的异常，要么用 try。。。 catch 捕获，要么用 throws 字句声明抛出，交给它的父类处理，否则编译不会通过。
 

## 15.请写出你最常见的 5 个 RuntimeException

java.lang.NullPointerException 空指针异常；出现原因：调用了未经初始化的对象或者是不存在的对象
java.lang.ClassNotFoundException 指定的类找不到；出现原因：类的名称和路径加载错误；通常都是程序试图
通过字符串来加载某个类时可能引发异常。
java.lang.NumberFormatException 字符串转换为数字异常；出现原因：字符型数据中包含非数字型字符。
java.lang.IndexOutOfBoundsException 数组角标越界异常，常见于操作数组对象时发生。
java.lang.IllegalArgumentException 方法传递参数错误。
java.lang.ClassCastException 数据类型转换异常。
java.lang.NoClassDefFoundException 未找到类定义错误。
SQLException SQL 异常，常见于操作数据库时的 SQL 语句错误。
java.lang.InstantiationException 实例化异常。
java.lang.NoSuchMethodException 方法不存在异常
 

## 16.throw 和 throws 的区别

throw：是用在语句抛出异常，用在方法体内。
特点：
throw 语句用在方法体内，表示抛出异常，由方法体内的语句处理。
throw 是具体向外抛出异常的动作，所以它抛出的是一个异常实例，执行 throw 一定是抛出了某种异常。
throws：用在声明方法时，用在方法声明上，表示该方法可能要抛出的异常。
特点：
throws 语句是用在方法声明后面，表示如果抛出异常，由该方法的调用者来进行异常的处理。
throws 主要是声明这个方法会抛出某种类型的异常，让它的使用者要知道需要捕获的异常的类型。
throws 表示出现异常的一种可能性，并不一定会发生这种异常。
注意：抛出的异常可以使用 java 内置的异常，同时也可以使用用户自定义的异常。
 

## 17.final、 finally、 finalize 的区别？

final：用于声明属性，方法和类，分别表示属性不可变，方法不可覆盖，被其修饰的类不可继承。
finally：异常处理语句结构的一部分，表示总是执行。
finalize： Object 类的一个方法，在垃圾收集器执行的时候会调用被回收对象的此方法，可以覆盖此方法提
供垃圾收集时的其他资源回收，例如关闭文件等
 

# JAVA常用API

## 18.Math.round(11.5)等于多少？ Math.round(- 11.5) 又等于多少?

Math.round(11.5)的返回值是 12， Math.round(-11.5)的返回值是-11。四舍五入的原理是在参数上加 0.5然后进行下取整。
 

## 19.switch 是否能作用在 byte 上，是否能作用在 long 上，是否能作用在 String上?

Java5 以前 switch(expr)中， expr 只能是 byte、 short、 char、 int。从 Java 5 开始， Java 中引入了枚举类型，
expr 也可以是 enum 类型。
从 Java 7 开始， expr 还可以是字符串（String），但是长整型（long）在目前所有的版本中都是不可以的。

## 20.基本数据类型有哪些？

常见的基本数据类型：（1.内置数据类型，2.引用数据类型）

内置数据类型：

四种整数类型(byte、short、int、long)：

两种浮点数类型(float、double)：

一种字符类型(char)：

一种布尔类型(boolean)：true 真  和 false 假。

![img](https://img-blog.csdn.net/20180821211816914?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

·····字符串与其它类型间的转换

其它类型向字符串的转换

①调用类的串转换方法:X.toString();

②自动转换:X+"";

③使用String的方法:String.volueOf(X);

字符串作为值,向其它类型的转换

①先转换成相应的封装器实例,再调用对应的方法转换成其它类型

②静态parseXXX方法

## 20.数组有没有 length() 方法？ String 有没有 length() 方法？

数组没有 length()方法， 而是有 length 的属性。 String 有 length()方法。 JavaScript 中，获得字符串的长度是通过 length 属性得到的，这一点容易和 Java 混淆。
 

## 21.String 和 StringBuilder 、 StringBuffer 的区别？

Java 平台提供了两种类型的字符串： String 和 StringBuffer/StringBuilder，它们可以储存和操作字符串。
String :是只读字符串，也就意味着 String 引用的字符串内容是不能被改变的。
StringBuffer/StringBuilder:类表示的字符串对象可以直接进行修改。
StringBuilder:是 Java 5 中引入的，它和 StringBuffer 的方法完全相同，区别在于它是在单线程环境下使用的，
因为它的所有方面都没有被 synchronized 修饰，因此它的效率也比 StringBuffer 要高。
 

## java的数据类型

### 22.String 是基本数据类型吗？

String 是引用类型，底层用 char 数组实现的。
 

## 23.short s1 = 1; s1 = s1 + 1; 有错吗 ?short s1 = 1; s1 += 1 有错吗;

前者不正确，后者正确。 对于 short s1 = 1; s1 = s1 + 1;由于 1 是 int 类型，因此 s1+1 运算结果也是 int 型，
需要强制转换类型才能赋值给 short 型。而 short s1 = 1; s1 += 1;可以正确编译，因为 s1+= 1;相当于 s1 =
(short)(s1 + 1);其中有隐含的强制类型转换
 

## 24.String 类常用方法 ![img](https://img-blog.csdn.net/2018082121331283?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

## 25.String、 StringBuffer、 StringBuilder 的区别？

（1）、可变不可变
String：字符串常量，在修改时不会改变自身；若修改，等于重新生成新的字符串对象。
StringBuffer：在修改时会改变对象自身，每次操作都是对 StringBuffer 对象本身进行修改，不是生成新的对
象；使用场景：对字符串经常改变情况下，主要方法： append（）， insert（）等。
（2） 、线程是否安全
String：对象定义后不可变，线程安全。
StringBuffer：是线程安全的（对调用方法加入同步锁），执行效率较慢，适用于多线程下操作字符串缓冲区
大量数据。
StringBuilder：是线程不安全的，适用于单线程下操作字符串缓冲区大量数据。
（3） 、共同点
StringBuilder 与 StringBuffer 有公共父类 AbstractStringBuilder(抽象类)。
StringBuilder、 StringBuffer 的方法都会调用 AbstractStringBuilder 中的公共方法，如 super.append(...)。
只是 StringBuffer 会在方法上加 synchronized 关键字，进行同步。最后，如果程序不是多线程的，那么使用
StringBuilder 效率高于 StringBuffer。
 

## 26.数据类型之间的转换

（1）、 字符串如何转基本数据类型？
调用基本数据类型对应的包装类中的方法 parseXXX(String)或 valueOf(String)即可返回相应基本类型。
（2） 、基本数据类型如何转字符串？
一种方法是将基本数据类型与空字符串（“”）连接（+）即可获得其所对应的字符串；另一种方法是调用 String
类中的 valueOf()方法返回相应字符串

##  java的IO

### 27.Java 中有几种类型的流

按照流的方向：**输入流**（inputStream）和**输出流**（outputStream）
按照实现功能分：**节点流**（可以从或向一个特定的地方（节点）读写数据。如 FileReader）和**处理流**（是对一个已存在的流的连接和封装，通过所封装的流的功能调用实现数据读写。如 BufferedReader。处理流的构造方法总是要带一个其他的流对象做参数。一个流对象经过其他流的多次包装，称为流的链接。）
按照处理数据的单位： **字节流**和**字符流**。字节流继承于 InputStream 和 OutputStream，字符流继承于
InputStreamReader 和 OutputStreamWriter。
![img](https://img-blog.csdn.net/20180822153130327?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 28.字节流如何转为字符流

字节输入流转字符输入流通过 InputStreamReader 实现，该类的构造函数可以传入 InputStream 对象。
字节输出流转字符输出流通过 OutputStreamWriter 实现，该类的构造函数可以传入 OutputStream 对象。
 

### 29.如何将一个 java 对象序列化到文件里

在 java 中能够被序列化的类必须先实现 Serializable 接口，该接口没有任何抽象方法只是起到一个标记作用。
 

### 30.字节流和字符流的区别

字节流读取的时候，读到一个字节就返回一个字节； 字符流使用了字节流读到一个或多个字节。
（中文对应的字节
数是两个，在 UTF-8 码表中是 3 个字节）时。先去查指定的编码表，将查到的字符返回。 字节流可以处理所有类型数
据，如：图片， MP3， AVI 视频文件，而字符流只能处理字符数据。只要是处理纯文本数据，就要优先考虑使用字符
流，除此之外都用字节流。 字节流主要是操作 byte 类型数据，以 byte 数组为准，主要操作类就是 OutputStream、
InputStream
字符流处理的单元为 2 个字节的 Unicode 字符，分别操作字符、字符数组或字符串，而字节流处理单元为 1 个字
节，操作字节和字节数组。所以字符流是由 Java 虚拟机将字节转化为 2 个字节的 Unicode 字符为单位的字符而成的，
所以它对多国语言支持性比较好！如果是音频文件、图片、歌曲，就用字节流好点，如果是关系到中文（文本）的，用
字符流好点。在程序中一个字符等于两个字节， java 提供了 Reader、 Writer 两个专门操作字符流的类
 

### 31.如何实现对象克隆？

有两种方式。
1). 实现 Cloneable 接口并重写 Object 类中的 clone()方法；
2). 实现 Serializable 接口，通过对象的序列化和反序列化实现克隆，可以实现真正的深度克隆，
注意：基于序列化和反序列化实现的克隆不仅仅是深度克隆，更重要的是通过泛型限定，可以检查出要克隆的对
象是否支持序列化，这项检查是编译器完成的，不是在运行时抛出异常，这种是方案明显优于使用 Object 类的 clone
方法克隆对象。让问题在编译的时候暴露出来总是好过把问题留到运行时。
 

### 32.什么是 java 序列化，如何实现 java 序列化？

序列化就是一种用来处理对象流的机制，所谓对象流也就是将对象的内容进行流化。可以对流化后的对象进行读写操作，也可将流化后的对象传输于网络之间。序列化是为了解决在对对象流进行读写操作时所引发的问题。

序 列 化 的 实 现 ： 将 需 要 被 序 列 化 的 类 实 现 Serializable 接 口 ， 该 接 口 没 有 需 要 实 现 的 方 法 ，implements Serializable 只是为了标注该对象是可被序列化的，然后使用一个输出流(如： FileOutputStream)来构造一个 ObjectOutputStream(对象流)对象，接着，使用 ObjectOutputStream 对象的 writeObject(Object obj)方法就可以将参数为 obj 的对象写出(即保存其状态)，要恢复的话则用输入流。

##  Java集合

### 33.List 的三个子类的特点

ArrayList 底层结构是数组,底层查询快,增删慢
LinkedList 底层结构是链表型的,增删快,查询慢
voctor 底层结构是数组 线程安全的,增删慢,查询慢
 

### 34.List 和 Map、 Set 的区别

**结构特点**

List 和 Set 是存储单列数据的集合， Map 是存储键和值这样的双列数据的集合； List 中存储的数据是有顺序，并
且允许重复； Map 中存储的数据是没有顺序的，其键是不能重复的，它的值是可以有重复的， Set 中存储的数据是无
序的，且不允许有重复，但元素在集合中的位置由元素的 hashcode 决定，位置是固定的（Set 集合根据 hashcode 来
进行数据的存储，所以位置是固定的，但是位置不是用户可以控制的，所以对于用户来说 set 中的元素还是无序的） ；

**实现类**
List 接口有三个实现类（LinkedList：基于链表实现，链表内存是散乱的，每一个元素存储本身[内存地址](https://www.baidu.com/s?wd=%E5%86%85%E5%AD%98%E5%9C%B0%E5%9D%80&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的同时还存储下一个元素的地址。链表增删快，查找慢； ArrayList：基于数组实现，非线程安全的，效率高，便于索引，但不便于插入删除； Vector：基于数组实现，线程安全的，效率低）。
Map 接口有三个实现类（HashMap：基于 hash 表的 Map 接口实现，非线程安全，高效，支持 null 值和 null键； HashTable：线程安全，低效，不支持 null 值和 null 键； LinkedHashMap：是 HashMap 的一个子类，保存了记录的插入顺序； SortMap 接口： TreeMap，能够把它保存的记录根据键排序，默认是键值的升序排序）。
Set 接口有两个实现类（HashSet：底层是由 HashMap 实现，不允许集合中有重复的值，使用该方式时需要重写 equals()和 hashCode()方法； LinkedHashSet：继承与 HashSet，同时又基于 LinkedHashMap 来进行实现， 底层使用的是 LinkedHashMp）。

**区别**
List 集合中对象按照索引位置排序，可以有重复对象，允许按照对象在集合中的索引位置检索对象，例如通过list.get(i)方法来获取集合中的元素； Map 中的每一个元素包含一个键和一个值，成对出现，键对象不可以重复，值对象可以重复； Set 集合中的对象不按照特定的方式排序，并且没有重复对象，但它的实现类能对集合中的对象按照特定的方式排序，例如 TreeSet 类，可以按照默认顺序，也可以通过实现 Java.util.Comparator<Type>接口来自定义排序方式
 

### 35.HashMap 和 HashTable 有什么区别?

HashMap 是线程不安全的,HashMap 是一个接口,是 Map 的一个子接口,是将键映射到值得对象,不允许键值重复,允许空键和空值;由于非线程安全,HashMap 的效率要较 HashTable 的效率高一些.

HashTable 是线程安全的一个集合,不允许 null 值作为一个 key 值或者 Value 值;
HashTable 是 sychronize,多个线程访问时不需要自己为它的方法实现同步,而 HashMap 在被多个线程访问的时候需要自己为它的方法实现同步;
 

### 36.数组和链表的区别

数组是将元素在内存中连续存储的；它的优点：因为数据是连续存储的，内存地址连续，所以在查找数据的时候效率比较高；它的缺点：在存储之前，我们需要申请一块连续的内存空间，并且在编译的时候就必须确定好它的空间的大小。在运行的时候空间的大小是无法随着你的需要进行增加和减少而改变的，当数据两比较大的时候，有可能会出现越界的情况，数据比较小的时候，又有可能会浪费掉内存空间。在改变数据个数时，增加、插入、删除数据效率比较低链表是动态申请内存空间，不需要像数组需要提前申请好内存的大小，链表只需在用的时候申请就可以，根据需要来动态申请或者删除内存空间，对于数据增加和删除以及插入比数组灵活。还有就是链表中数据在内存中可以在任意的位置，通过应用来关联数据（就是通过存在元素的指针来联系）
 

### 37.链表和数组使用场景

数组应用场景：数据比较少；经常做的运算是按序号访问数据元素；数组更容易实现，任何高级语言都支持；构建的线性表较稳定。
链表应用场景：对线性表的长度或者规模难以估计；频繁做插入删除操作；构建动态性比较强的线性表
 

### 38.用面向对象的方法求出数组中重复 value 的个数

参考此文，并理解什么是面向对象。

<https://blog.csdn.net/qq_37582417/article/details/80604472>

### 39.Java 中 ArrayList 和 Linkedlist 区别？

ArrayList 和 Vector 使用了数组的实现，可以认为 ArrayList 或者 Vector 封装了对内部数组的操作，比如向数组中添加，删除，插入新的元素或者数据的扩展和重定向。
LinkedList 使用了循环双向链表数据结构。与基于数组的 ArrayList 相比，这是两种[截然不同](https://www.baidu.com/s?wd=%E6%88%AA%E7%84%B6%E4%B8%8D%E5%90%8C&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)的实现技术，这也决定了它们将适用于完全不同的工作场景。

LinkedList 链表由一系列表项连接而成。一个表项总是包含 3 个部分：元素内容，前驱表和后驱表，如图所示：
![img](https://img-blog.csdn.net/20180822160004193?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在下图展示了一个包含 3 个元素的 LinkedList 的各个表项间的连接关系。在 JDK 的实现中，无论 LikedList 是否
为空，链表内部都有一个 header 表项，它既表示链表的开始，也表示链表的结尾。表项 header 的后驱表项便是链表
中第一个元素，表项 header 的前驱表项便是链表中最后一个元素。
![img](https://img-blog.csdn.net/20180822160038926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

### 40.List a=new ArrayList()和 ArrayList a =new ArrayList()的区别？

List list = new ArrayList();这句创建了一个 ArrayList 的对象后把上溯到了 List。此时它是一个 List 对象了，有些
ArrayList 有但是 List 没有的属性和方法，它就不能再用了。而 ArrayList list=new ArrayList();创建一对象则保留了
ArrayList 的所有属性。 所以需要用到 ArrayList 独有的方法的时候不能用前者。实例代码如下：
 

```
1． List list = new ArrayList();



2． ArrayList arrayList = new ArrayList();



3． list.trimToSize(); //错误，没有该方法。



4． arrayList.trimToSize(); //ArrayList 里有该方法。
```

### 41.要对集合更新操作时， ArrayList 和 LinkedList 哪个更适合？

1.ArrayList 是实现了基于动态数组的数据结构， LinkedList 基于链表的数据结构。
2.如果集合数据是对于集合随机访问 get 和 set， ArrayList 绝对优于 LinkedList，因为 LinkedList 要移动指针。
3.如果集合数据是对于集合新增和删除操作 add 和 remove， LinedList 比较占优势，因为 ArrayList 要移动数
据。

用于存储一系列的对象引用-----总结：
ArrayList 和 LinkedList 在性能上各有优缺点，都有各自所适用的地方，总的说来可以描述如下：
1．对 ArrayList 和 LinkedList 而言，在列表末尾增加一个元素所花的开销都是固定的。对 ArrayList 而言，主
要是在内部数组中增加一项，指向所添加的元素，偶 尔可能会导致对数组重新进行分配；而对 LinkedList 而言，这
个开销是统一的，分配一个内部 Entry 对象。
2．在 ArrayList 的中间插入或删除一个元素意味着这个列表中剩余的元素都会被移动；而在 LinkedList 的中间
插入或删除一个元素的开销是固定的。
3． LinkedList 不支持高效的随机元素访问。
4． ArrayList 的空间浪费主要体现在在 list 列表的结尾预留一定的容量空间，而 LinkedList 的空间花费则体现在
它的每一个元素都需要消耗相当的空间
可以这样说：当操作是在一列数据的后面添加数据而不是在前面或中间,并且需要随机地访问其中的元素时,使用
ArrayList 会提供比较好的性能；当你的操作是在一列数据的前面或中间添加或删除数据,并且按照顺序访问其中的元
素时,就应该使用 LinkedList 了。

### 42.Collection 和 Map 的集成体系

Collection：
![img](https://img-blog.csdn.net/2018082216084312?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Map：
![img](https://img-blog.csdn.net/20180822160916351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 43.Map 中的 key 和 value 可以为 null 么？

###  

HashMap 对象的 key、 value 值均可为 null。
HahTable 对象的 key、 value 值均不可为 null。
且两者的的 key 值均不能重复，若添加 key 相同的键值对，后面的 value 会自动覆盖前面的 value，但不会报错。

##  Java的多线程

### 44.多线程的创建方式

（1）、 继承 Thread 类： 但 Thread 本质上也是实现了 Runnable 接口的一个实例，它代表一个线程的实例，并
且，启动线程的唯一方法就是通过 Thread 类的 start()实例方法。 start()方法是一个 native 方法，它将启动一个新线
程，并执行 run()方法。这种方式实现多线程很简单，通过自己的类直接 extend Thread，并复写 run()方法，就可以
启动新线程并执行自己定义的 run()方法
（2）、实现 Runnable 接口的方式实现多线程，并且实例化 Thread，传入自己的 Thread 实例，调用 run( )方法
（3） 、使用 ExecutorService、 Callable、 Future 实现有返回结果的多线程： ExecutorService、 Callable、 Future
这 个 对 象 实 际 上 都 是 属 于 Executor 框 架 中 的 功 能 类 。 想 要 详 细 了 解 Executor 框 架 的 可 以 访 问
http://www.javaeye.com/topic/366591
 

### 45.在 java 中 wait 和 sleep 方法的不同？

最大的不同是在等待时 wait 会释放锁，而 sleep 一直持有锁。 wait 通常被用于线程间交互， sleep 通常被用于暂停执行。
 

### 46.什么是线程池，如何使用？

线程池就是事先将多个线程对象放到一个容器中，当使用的时候就不用 new 线程而是直接去池中拿线程即可节省了开辟子线程的时间，提高的代码执行效率。
 

### 47.常用的线程池有哪些？

newSingleThreadExecutor：创建一个单线程的线程池，此线程池保证所有任务的执行顺序按照任务的提交顺序执行。
newFixedThreadPool：创建固定大小的线程池，每次提交一个任务就创建一个线程，直到线程达到线程池的最大大小。
newCachedThreadPool：创建一个可缓存的线程池，此线程池不会对线程池大小做限制，线程池大小完全依赖于操作系统（或者说 JVM）能够创建的最大线程大小。
newScheduledThreadPool：创建一个大小无限的线程池，此线程池支持定时以及周期性执行任务的需求。
newSingleThreadExecutor：创建一个单线程的线程池。此线程池支持定时以及周期性执行任务的需求。
 

### 48.请叙述一下您对线程池的理解？

（如果问到了这样的问题，可以展开的说一下线程池如何用、线程池的好处、线程池的启动策略）
合理利用线程池能够带来三个好处。
第一：降低资源消耗。通过重复利用已创建的线程降低线程创建和销毁造成的消耗。
第二：提高响应速度。当任务到达时，任务可以不需要等到线程创建就能立即执行。
第三：提高线程的可管理性。线程是稀缺资源，如果无限制的创建，不仅会消耗系统资源，还会降低系统的稳定性，使用线程池可以进行统一的分配，调优和监控。
 

### 49.线程池的启动策略？

官方解释：

1、线程池刚创建时，里面没有一个线程。任务队列是作为参数传进来的。不过，就算队列里面有任务，线程池也
不会马上执行它们。
2、当调用 execute() 方法添加一个任务时，线程池会做如下判断：
a. 如果正在运行的线程数量小于 corePoolSize，那么马上创建线程运行这个任务；
b. 如果正在运行的线程数量大于或等于 corePoolSize，那么将这个任务放入队列。
c. 如果这时候队列满了，而且正在运行的线程数量小于 maximumPoolSize，那么还是要创建线程运行这个任务；
d. 如果队列满了，而且正在运行的线程数量大于或等于 maximumPoolSize，那么线程池会抛出异常，告诉调用者“我不能再接受任务了”。
3、当一个线程完成任务时，它会从队列中取下一个任务来执行。
4、当一个线程无事可做，超过一定的时间（keepAliveTime）时，线程池会判断，如果当前运行的线程数大于corePoolSize，那么这个线程就被停掉。所以线程池的所有任务完成后，它最终会收缩到 corePoolSize 的大小。
![img](https://img-blog.csdn.net/20180822173238389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 50.同一个类中的 2 个方法都加了同步锁，多个线程能同时访问同一个类中的这两个方法吗？

**1.死锁的定义**：所谓死锁是指多个线程因竞争资源而造成的一种僵局（互相等待），若无外力作用，这些进程都将无法向前推进。

**2.死锁产生的必要条件**：
**互斥条件**：线程要求对所分配的资源（如打印机）进行排他性控制，即在一段时间内某 资源仅为一个线程所占有。此时若有其他线程请求该资源，则请求线程只能等待。
**不剥夺条件**：线程所获得的资源在未使用完毕之前，不能被其他线程强行夺走，即只能由获得该资源的线程自己来释放（只能是主动释放)。
**请求和保持条件**：线程已经保持了至少一个资源，但又提出了新的资源请求，而该资源已被其他线程占有，此时请求进程被阻塞，但对自己已获得的资源保持不放。
**循环等待条件**：存在一种线程资源的循环等待链，链中每一个线程已获得的资源同时被链中下一个线程所请求。即存在一个处于等待状态的线程集合{Pl, P2, ..., pn}，其中 Pi 等待的资源被 P(i+1)占有（i=0, 1, ..., n-1)，Pn 等待的资源被 P0 占有

![img](https://img-blog.csdn.net/20180822173536678?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzI5MDczOTIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

**3.如何避免死锁**

在有些情况下死锁是可以避免的。两种用于避免死锁的技术：
1） 加锁顺序（线程按照一定的顺序加锁）
2） 加锁时限（线程尝试获取锁的时候加上一定的时限，超过时限则放弃对该锁的请求，并释放自己占有的锁）
 

### 51.Java 中多线程间的通信怎么实现?

线程通信的方式：
1.共享变量
线程间通信可以通过发送信号，发送信号的一个简单方式是在共享对象的变量里设置信号值。线程 A 在一个
同步块里设置 boolean 型成员变量 hasDataToProcess 为 true，线程 B 也在同步块里读取 hasDataToProcess
这个成员变量。
2.wait/notify 机制
以资源为例，生产者生产一个资源，通知消费者就消费掉一个资源，生产者继续生产资源，消费者消费资源，以
此循环。
 

### 52.线程和进程的区别

进程：具有一定独立功能的程序关于某个数据集合上的一次运行活动，是操作系统进行资源分配和调度的一个独
立单位。
线程：是进程的一个实体，是 cpu 调度和分派的基本单位，是比进程更小的可以独立运行的基本单位。
特点：线程的划分尺度小于进程，这使多线程程序拥有高并发性，进程在运行时各自内存单元相互独立，线程之间
内存共享，这使多线程编程可以拥有更好的性能和用户体验
注意：多线程编程对于其它程序是不友好的，占据大量 cpu 资源。
 

### 53.请说出同步线程及线程调度相关的方法？

wait()：使一个线程处于等待（阻塞）状态，并且释放所持有的对象的锁；
sleep()：使一个正在运行的线程处于睡眠状态，是一个静态方法，调用此方法要处理 InterruptedException 异常；
notify()：唤醒一个处于等待状态的线程，当然在调用此方法的时候，并不能确切的唤醒某一个等待状态的线程，而是由 JVM 确定唤醒哪个线程，而且与优先级无关；
notityAll()：唤醒所有处于等待状态的线程，该方法并不是将对象的锁给所有线程，而是让它们竞争，只有获得锁的线程才能进入就绪状态；
       **注意**： java 5 通过 Lock 接口提供了显示的锁机制， Lock 接口中定义了加锁（lock（）方法）和解锁（unLock（）
方法），增强了多线程编程的灵活性及对线程的协调
 

### 54.启动一个线程是调用 run()方法还是 start()方法？

启动一个线程是调用 start()方法，使线程所代表的虚拟处理机处于可运行状态，这意味着它可以由 JVM 调度并执行，这并不意味着线程就会立即运行。
run()方法是线程启动后要进行回调（callback）的方法。
 

## Java内部类

### 55.静态嵌套类 (Static Nested Class) 和内部类(Inner Class)的不同？

**静态嵌套类**： Static Nested Class 是被声明为静态（static）的内部类，它可以不依赖于外部类实例被实例化。
**内部类**： 需要在外部类实例化后才能实例化，其语法看起来挺诡异的

### 56.下面的代码哪些地方会产生编译错误？

1． class Outer {
2．
3． class Inner {}
4．
5． public static void foo() { new Inner(); }
6．
7． public void bar() { new Inner(); }
8．
9． public static void main(String[] args) {
10． new Inner();
11． }
12． }
**注意**： Java 中非静态内部类对象的创建要依赖其外部类对象，上面的面试题中 foo 和 main 方法都是静态方
法，静态方法中没有 this，也就是说没有所谓的外部类对象，因此无法创建内部类对象，如果要在静态方法中创建内
部类对象，可以这样做
1． new Outer().new Inner();