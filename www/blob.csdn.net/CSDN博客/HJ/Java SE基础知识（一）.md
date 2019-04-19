# Java SE基础知识（一） - HJ - CSDN博客
2017年02月20日 22:30:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：559
欢迎来到Java世界，从现在开始，跟随作者走进Java，一点一点掌握Java，走上Java成功之路。
首先，第一步是如何**配置Java环境**。
分为如下几步： 
（1）下载JDK，地址：www.oracle.com/technetwork/java/javase/downloads。JDK是Java Development Kit 的缩写（详细内容先行百度） 
（2）设置执行路径：在完成了JDK的安装之后，还需要执行另外一个步骤：把jdk/bin目录添加到执行路径中。所谓执行路径是指操作系统搜索本地可执行文件的目录列表。 
在Windows下，以管理员的身份登录。启动Conrtol Panel，切换到Classic View，并选择System图标。在Windows 7中，需要选择Advanced系统配置。在System Properties对话框中，点击Advanced标签，然后点击Environment按钮。滚动System Variable窗口直到找到变量名Path（path）为止。点击Edit按钮。讲jdk\bin目录添加到路径的开始处，用分号讲新条目隔开。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20170220210914190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来，你还需要一个编译器（当然你也可以用记事本编译，用dos窗口执行），个人推荐的是sublime text3，其实也很简单，对于sublime text3的下载安装以及配置Java语言的运行读者可以阅览本人的另外一个篇简单的博客介绍，本人在此不再赘述。
与C/C++语言一样，第一次敲写代码，还是那句“Hello World”
![这里写图片描述](https://img-blog.csdn.net/20170220211556371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个程序虽然简单，但所有的Java应用程序都具有这种结构，还是值得花一些时间研究一下。首先Java**对大小写敏感**。如果出现了大小写拼写错误（例如，讲main拼写成Main），那程序将无法运行。
接下来，我们仔细看这段源代码。关键字public被称为访问修饰符，它用于控制程序的其他部分对这段代码的访问级别。在接下来的内容中会仔细介绍访问修饰符的具体内容。关键字class表明Java程序中的全部内容都包含在类中。这里，只需要将类作为一个加载程序逻辑的容器，程序逻辑定义了应用程序的行为。
我们必须谨记：**类是构建所有Java应用程序和applet的构建块。Java应用程序中的全部内容都必须放置在类中。**
关键字class后面紧跟着类名。Java中定义类名的规则很宽松：**名字必须以字母开头，后面可以跟字母和数字的任意组合。长度上基本没有限制，但是不能使用Java保留字**（这个如果有C/C++基础的话很容易理解）
另外还请记住一点：**源代码的文件名必须与公共类的名字相同，并用.java作为扩展名（如上图Hello.java文件）**
如果已经正确地命名了这个文件，并且源代码中没有任何录入错误，在编译这段源代码之后就会得到一个包含这个类字节码的文件。Java编译器将字节码文件自动地命名为Hello.class，并与源文件存储在同一个目录下。
最后，大家还需要记住：**main方法必须声明为public**。
接下来，因为这段代码的简单性，在{}中只有一个语句：
```
System.out.println("Hello World");
```
不用说也知道，这条语句的功能是：将一个文本行输出到控制台上。
在这里，使用了System.out对象并调用了println方法。
**注意，点号（.）用于调用方法。**
System.out还有一个print方法，它在输出之后不换行。
对于注释我认为大家如果有C++/C功底完全不用鄙人在此啰嗦。
因此，接下来就讲讲**数据类型**吧
Java是一种强类型语言。这就意味着必须为每一个变量声明一种类型。在Java中，一共有8种基本类型，其中有4种整型（**int、short、long、byte**）、两种浮点类型（**float、double**）、一种用于表示Unicode编码的字符单元的字符类型**char**和一种用于表示真值的**boolean**类型。
Java有一个能够表示任意精度的算术包，通常称为“大数值”。虽然被称为大数值，但它并不是一个种新的Java类型，而是一个Java对象。这一点，稍后会讲到。
接下来的内容，都是将Java与C/C++的进行对比，大家可以酌情参考：
（1）整型
在Java中，整型的范围与运行Java代码的机器无关。这就解决了软件从一个平台移植到另一个平台，或者在同一个平台中的不同操作系统之间进行移植给程序员带来的诸多问题。与此相反，C/C++程序需要针对不同的处理器选择最为有效的整型，这样就有可能造成一个在32位处理器上运行很好的C程序在16位系统上运行却发生整数溢出。由于Java程序必须保证在所有机器上都能够得到相同的运行结果，所以每一种数据类型的取值范围必须固定。
另外注意：**Java没有任何无符号类型（unsigned）**。
（2）浮点类型
与C/C++不同的是，所有的浮点数值计算都遵循IEEE 754规范，下面是用于表示溢出和出错情况的三个特殊浮点数值：
①正无穷大； 
②负无穷大； 
③NaN（不是一个数字）；
例如，一个正整数除以0的结果为正无穷大，但是计算0/0或者负数的平方根结果为NaN。
提示：**如果需要在数值计算中不含有任何舍入误差，就应该使用BigDecimal类**。
（3）char类型
在Java中，字符是按照Unicode编码单元来执行标准的。Unicode编码单元可以表示为十六进制值，其范围从\u0000到\Uffff。要想弄清char类型，就必须了解Unicode编码表。在Java中，char类型用UTF-16编码描述一个代码单元。不过我们强烈建议不要再程序中使用char类型，除非确实需要对UTF-16代码单元进行操作。最好将需要处理的字符串用抽象数据类型表示。详细的将会在后面进行介绍。
（4）boolean类型
注意：**boolean（布尔）类型只有两个值：false和true，用来判断逻辑条件。整型值和布尔值之间不能进行相互转换**。
对于常量，Java中利用关键之final来指示，例如：
```
final double PI=3.1415926；
```
在Java中，经常希望某个常量可以在一个类中的多个方法中使用，通常将这些常量成为类常量，可以使用关键字static final设置一个类常量：
```
public static final double PI=3.1415926；
```
**数学函数与常量**
在Math类中，包含了各种各样的数学函数。
（1）平方根：Math.sqrt(x);
（2）次方幂：Math.pow(x,a);
另外，Java还提供了两个用于表示π和e常量的近似值：
```
Math.PI
Math.E
```
如果你嫌每次都要加“Math.”麻烦，就只需要在源文件的顶部加上下面这行代码就行了：
```
import static java.lang.Math.*;
```
**字符串**
从概念上讲，Java字符串就是Unicode字符序列。值得注意的是，在Java中，没有内置的字符串类型，而是在标准Java类库中提供了一个预定义类，很自然地叫做String，每个用双引号括起来的字符串都是String类的一个实例：
```
String a="Hello";
```
String类的substring方法可以从一个较大的字符串中提取出一个子串，例如：
```
String a="Hello";
String b=a.substring(0,3);
```
创建了一个由字符“Hel”组成的字符串。
注意：**substring方法的第二个参数是不想复制的第一个位置**。
与大多数的程序设计语言一样，Java语言允许使用+号连接两个字符串，并且当将一个字符串与一个非字符串的值进行拼接时，后者被转换成字符串。（任何一个Java对象都可以转换成字符串）。
但是，**String类没有提供用于修改字符串的方法**。由于不能修改Java字符串中的字符，所以在Java文档中将String类对象称为**不可变字符串**。
因此，你如果要修改，就需要重新定义一个新的String串，进行复制子串拼接等操作。
但是，不可变字符串却有一个优点：**编译器可以让字符串共享**。
在Java中，使用**equals**方法检测两个字符串是否相等。对于表达式：
```
s.equal(t);
```
如果字符串s与字符串t相等，则返回true，否则返回false。需要注意的是，s与t可以是字符串变量，也可以是字符串常量。
**大数值**
如果基本的整数和浮点数精度不能满足要求，那么可以使用java.math包中的两个很有用的类：BigInteger和BigDecimal。这两个类可以处理包含任意长度数字序列的数值。BigInteger实现了任意精度的整数运算，BigDecimal实现了任意精度的浮点数运算。
使用静态的valueOf方法可以将普通的数值转换为大数值：
```
BigInteger a=BigInteger.valueOf(100);
```
不过，不能使用算术运算符处理大数值，而是需要使用大数值类中的add和multiply方法：
```
BigInteger c=a.add(b);
BigInteger d=c.multiply(b.add(BigInteger.valueOf(2)));
```
**数组**
两种命名方式：
```
（1）int [] a;
（2）int[] a=new int[100];
```
**for each循环**
Java有一种功能很强的循环结构，可以用来一次处理数组中的每个元素（其他类型的元素集合亦可）而不必为指定下标值而分心：
```
for(int element:a)
    System.out.println(element);
```
打印数组a的每一个元素，一个元素占一行。
**数组排序**
```
Arrays.sort(result);
```
类似于C中的qsort和C++中的sort排序。
以上即是关于Java的基础知识，从下一节开始，我们就进入Java的精彩部分，让我们从类开始吧！
