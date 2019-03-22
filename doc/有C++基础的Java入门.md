# 有C++基础的Java入门



# Java的运行机制

前言：还是那句话，第一、凡是涉及到概念性内容的时候，我都会到官网去确认内容的真实性！第二、我喜欢偏向于原理学习。在 java介绍里面，我认为知道这是一门完全面向对象的语言就足够了。我的导师说C++是认为程序员是很强大的，开放了所有的功能权限；Java是认为程序员不是那么全能的，有些危险的操作，不会让你执行。不知道您是否也这么认为呢？



------

# 一、 类的结构

```java
//HelloWorld是自定义类名与cpp一样，
//不一样的是这个类名一定要和文件名一致，
//每个单词的首字母需要大写。
class helloworld{
    //主方法入口
    public static void main(String [] args){
        //在控制台输出的方式
        System.out.printIn("Hello,world");
    }
}
```

# 二、 运行机制

先了解一下什么是JVM、JDK、JRE

**JVM**

Java Virtual Machine

java虚拟机。它只认识 xxx.class 这种类型的文件，它能够将 class 文件中的字节码指令进行识别并调用操作系统向上的 API 完成动作。
**JRE**

Java Runtime Environment

包含jvm 的标准实现和 Java 的一些基本类库。
**JDK**

Java Development Kit

Java开发工具包。jdk 是整个 Java 开发的核心，它集成了 jre 和一些好用的小工具。例如：javac.exe，java.exe，jar.exe 等。

**三者的关系**

显然，一层包含一层。JDK>JRE>JVM
Java能够跨平台运行的原因

Java运行的核心在于JVM，而正是JVM能够跨平台运行。不同的操作系统的 API 是不同的，。我们知道，不同的操作系统向上的 API 肯定是不同的，那么如果我们想要写一段代码调用系统的声音设备，就需要针对不同系统的 API 写出不同的代码来完成动作。

而 Java 引入了字节码的概念，jvm 只能认识字节码，并将它们解释到系统的 API 调用。针对不同的系统有不同的 jvm 实现，有 Linux 版本的 jvm 实现，也有 Windows 版本的 jvm 实现，但是同一段代码在编译后的字节码是一样的。引用上面的例子，在 Java API 层面，我们调用系统声音设备的代码是唯一的，和系统无关，编译生成的字节码也是唯一的。但是同一段字节码，在不同的 jvm 实现上会映射到不同系统的 API 调用，从而实现代码的不加修改即可跨平台运行。

还没明白就看完这一段，不同的系统（Windows、Linux、Mac）就代表三个国家的人，如果中国人（电脑）不懂他们讲的话就需要翻译，那么就有这么一个很牛的人什么都懂（JVM），他一个人就能把全部的语言翻译成中文（字节码）。这样的话反正中国人（电脑）认识了。



在java中，我们写出来的代码都是源代码(*.java)，源代码是不能执行的，必须编译成[字节码](https://baike.baidu.com/item/%E5%AD%97%E8%8A%82%E7%A0%81/9953683)文件(*.class)

运行前提：下载了java，将java的bin目录添加到了系统的环境变量中。你会发现在bin目录中有javac.exe与java.exe的文件。作用如下：

## **1、 编译方式**

javac  文件名 .java

## 2、 运行

java  文件名.class

## 3、JVM

[JVM](https://baike.baidu.com/item/JVM)是什么？

说白了就是java的字节码文件运行时创建的虚拟机，然后在运行.class文件。至于java如何创建的JVM，有兴趣的话也可以了解一下。

在C++中，  C++没有虚拟机这种概念，也就是说C++一旦编译就可以运行，是一种编译型语言，速度上一般比JAVA快得多（这个要看个人水平写出来的程序运行的效率），C++编译完成后是机器码（二进制文件）可以直接在操作系统上运行。

## 4、 原理

### **编译原理**

在Java的运行环境中，JRE包括JVM和Java的核心类库。

![img](https://img-blog.csdn.net/20181025115409644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### **运行原理**

首先任何程序的运行都离不开计算机的内存！当我们切换到盘符下面执行HelloWorld的时候，会立即在计算机的内存里面创建一个JVM，再把HelloWorld.class这个文件调到JVM中。

![img](https://img-blog.csdn.net/20181025122809122?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Java的标识符与数据类型



# 一、 标识符

## 1、 概念

Java语言中，对于变量，常量，函数，语句块也有名字，我们统统称之为Java标识符。

标识符是用来给类、对象、方法、变量、接口和自定义数据类型命名的。

## 2、 标识符的规范

使用26个字母的大小写或数字还有“_”，“$”，数字和C++一样不能写在开头，但在Java中是区分大小写的（如HelloWorld和helloworld）。

### （1） 类和接口

首字符大写，如果是多个单词，则每一个单词的首字符都要大写（如HelloWorld）。

### （2） 变量和方法

第一个单词的首字符小写后续的单词首字符大写（和JS的函数命名规则一样，如helloWorld，俗称驼峰模式）。

### （3） 常量

多个单词定义常量的时候每个单词都大写，单词之间使用“_”隔开（如HELLO_WORLD）。

# 二、 数据类型

## 1、 数据类型定义及默认值

| 序号 | 数据类型 | 大小/位 | 封装类 | 默认值 | 可表示数据范围 |
| ---- | -------------- | ------- | --------- | ------ | :-------------: |
| 1 | byte(位) | 8 | Byte | 0 | -128~127 |
| 2 | short(短整数) | 16 | Short | 0 | -32768~32767 |
| 3 | int(整数) | 32 | Integer | 0 | -2147483648~2147483647 |
| 4 | long(长整数) | 64 | Long | 0 | -9223372036854775808~9223372036854775807 |
| 5 | float(单精度) | 32 | Float | 0.0 | 1.4E-45~3.4028235E38 |
| 6 | double(双精度) | 64 | Double | 0.0 | 4.9E-324~1.7976931348623157E308 |
| 7 | char(字符) | 16 | Character | 空 | 0~65535 |
| 8 | boolean | 8 | Boolean | flase | true或false |

## 2、数据类型自动转换

### （1） 总结

1、 int 一旦达到最大值会损失精度。

2、 任何整数类型和long类型做运算都会转换成long类型。

3、 常量再做运算的时候只要不超出定义的数据类型的范围（范围如上表）就不会报错。

4、 float类型和所有整数类型计算都会转换成float。

5、float类型和常量计算时不会有类型提升。

6、 double类型和float类型计算会转换为double类型；

规律：小空间的数据类型和大空间的数据类型的数计算会向大空间的数据类型去转换。

### **（2） 与C++的不同点**

**float类型和c++有所不同，如java定义float a=1; 与float a = 1f; 的区别？**

float a =1 在这里是整型，当赋值给一个float的时候，存在一个隐式转换，但不损失精度。

float a=1f 在这里就是float类型。

如果定义成float=1.0;就肯定会报错，因为1.0是double类型的。double范围比float大，为了不损失精度，不会自动转换。这时就必须写成float a=1.0f;

### （3） 原理

变量在做计算的时候可能存在溢出的情况，Java就采用自动类型转换来解决这个问题。

给大家举个栗子吧~ byte、short、char在计算的时候会自动的提升数据类型到int（int和其它数值计算也会溢出，int不会自动提升类型而是直接损失精度）。

在二进制里面，一个int是4个字节，最大的值是   0111 1111     1111 1111    1111 1111     1111 1111，此时如果在最低位上加1，则数值为1000 0000 0000 0000  0000 0000 ，二进制的最高位是符号位，则此时该数变成了一个负数，这个负数是多少呢？

![img](https://img-blog.csdnimg.cn/20181026172333925.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

## 3、 强制转换

大空间的类型向小空间的类型去转换。

| int类型的8   | 0000000 0000000 0000000 0001000 |
| ------------ | ------------------------------- |
| short类型的8 | 0000000 0001000                 |
| byte类型的8  | 0001000                         |

强制转换的语法和c++一样：（目标数据类型）变量名。

int i=10;

byte b = （byte）i;

注意：

① 小数转换整数会舍弃小数位（非四舍五入）。

② 如果把整型强制转换成byte有可能会损失精度。原因：转换变量的值可能会超过目标类型的最大范围。

# Java的数组定义与方法重载

------

# 一、 数组的定义

## 1、定义

数据类型 [ ]  数组名 = new 数据类型 [整数];

整数就是数组的长度。

定义方式和C#是一样的，如 int [] arr = new int[5];

## 2、 原理

假设在程序中定义的数组为 int [] arr = new int[5]; 从JVM（JVM在“**入门一**”里面有详细解释，这里不再赘述）的方向分析。先上个图，在做详细的解释。

![img](https://img-blog.csdnimg.cn/2018102620354279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

在程序执行的时候，电脑的内存里会分配一个JVM的空间。在JVM里面又分为两大区域：堆和栈（类似于我们电脑里面的固态硬盘和机械硬盘）。

栈：空间小，主要存储地址和基本数据类型的变量，存取速度快。

堆：空间大，主要存储引用类型的变量，存取速度慢。

int [] arr = new  int[5];在堆中会开辟一片空间来存放数组，会给这片空间分配一个假设为0x666的地址（这个地址只有计算机知道）。因为数组没有赋值，在Java中这五个值都默认为0。在堆中存放的东西由谁来引用呢？一定是由栈来引用的，所以在栈中一定有一片空间来存储数组的地址，通过栈中的地址找到堆中数组的值。

### 3、 赋值方式

赋值方式和c++也是一样的，但获取数组的长度上不同。C++需要自己定义一个函数来求数组的长度，Java中只需要“数组名.length(如arr.length)”和JS是一样的。

# 二、Java的方法重载

Java的方法定义、方法重载和C++都是差不多的。

## 1、 概念

### （1） 重载

重载是在一个类中有两个或者两个以上同名的方法，但是参数不同（两个方法的参数的个数不同，参数的类型不同），与返回值无关。

### （2） 目的

提高代码的可读性和节省命名的词。

## 2、例子

### （1） 比较两个数值的大小

```java
public class demo{
    /**
     *比较两个数的大小
     */
    public static int compareNum(int a,int b){
        return a>b?a:b;
        //三元运算符的比较：如果比较结果为真，则返回a，为假则返回b
    }
    /**
     * 函数重载
     * 比较三个数的大小
     */
    public static int compareNum(int a,int b,int c){
        return comparenum(a,b)>c?comparenum(a,b):c;
    }
    public static void main(String[] args){
        int result1=comparenum(100,200);
        int result2=comparenum(555,66,88);
        System.out.println("result1="+result1);
        System.out.println("result2="+result2);
    }
}
```

### （2） 运行方式及结果

![img](https://img-blog.csdnimg.cn/20181026183447525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)





# Java的this、public、static、protected关键字

------

# 一、 this关键字

我觉得this在Java和C++、C#的作用都是差不多的，this比较重要，还是拉出来讲讲。

## 1、 概念

打印一个对象的时候，输出的是这个对象的地址（JDK提供的一个地址，真正的地址是十六进制的）。

this 只能在类的对象方法中使用。

this 代表当前调用这个this所在方法的对象的自身。

this 可以在方法内区分同名的类的属性和参数名（有this的一定是属性，没有this的一定是方法的参数名）。

## 2、 实例

```java
public class Demo {
    public void work(){
        System.out.println("Demo--------"+this);
    }
}
class TestDemo{
    public static void main(String[] args) {
        Demo demo = new Demo();
       System.out.println("TestDemo--------"+demo);
       demo.work();
    }
}
```

运行结果

![img](https://img-blog.csdnimg.cn/20181027135315970.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

通过上面这个例子，不难看出this和demo的地址（每次new的时候都是唯一的）是相同的，说明它们两个用的是同一个对象，谁调用的这个方法，这个打印出来的地址就是谁的。

## 3、 常见用法

还是举个栗子吧~

```java
public class PersonInfo{
    private String name;
    /**
     *大家都知道下面的测试类要获取
     *这里的私有数据，需要set和get。
     *下面这种情况就需要区分一下谁是谁的name。
     */
     public void setName(String name){
        //this后面的name是属性
        //这个方法的name是参数
        this.name=name;
    }
    public void getName(String name){
        return name;
    }
}
class TestPersonInfo{
    PersonInfo person = new PersonInfo ();
    person.setName("王大锤");
    String personName=person.getName();
    System.out.println(personName);
}
```

 

|           | 类内部 | package内 | 子类   | 其他   |
| --------- | ------ | --------- | ------ | ------ |
| public    | 允许   | 允许      | 允许   | 允许   |
| protected | 允许   | 允许      | 允许   | 不允许 |
| default   | 允许   | 允许      | 不允许 | 不允许 |
| private   | 允许   | 不允许    | 不允许 | 不允许 |

## 4、 原理 

假设是一个汽车类（Car），这个车有color、name、carNo（车牌号）的属性，有  run() 和 introduce()  的方法。另外有一个测试类（TestCar）来调用汽车类的方法。代码很简单，这里主要详解一下原理。关于堆和栈以及进栈出栈的方式，自行补脑。

![img](https://img-blog.csdnimg.cn/20181027161633184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

**运行流程**

第一步：java运行到TestCar的时候，把TestCar.class载入到JVM（各就各位，），把main放入方法区压栈。

第二步：加载Car.class到JVM，然后把run() 和 introduce() 加载到方法区，在堆中创建一片空间创建了一个Car对象，在栈中产生一个变量car 指向堆中的对象。然后给 car 对象的三个属性赋值。

第三步：car 的run() 方法进栈，把 car 变量所指向的地址赋值给this关键字，执行方法体。

第四步：run() 方法出栈，run() 中的 this 消失。

第五步：car 变量消失，main 出栈后消失。

第六步：由于Car堆中的对象失去了变量的引用变成了匿名对象，所以也被系统回收。

# 二、 public关键字

使用对象：类、接口、成员

 介绍：无论它所处在的包定义在哪，该类（接口、成员）都是可访问的

# 三、static关键字

Static关键字详解：<https://blog.csdn.net/qq_41647999/article/details/87966487>

使用对象：类、方法、字段、初始化函数

介绍：成名为static的内部类是一个顶级类，它和包含类的成员是不相关的。

## 1、修饰在属性上

```java
class User{
    //一般的变量命名
    String username;
    String password;
    //加上static的命名
    static String realName;
}
```

加上static关键字之后，表示realName这个变量名不再是User的属性，而是这个类的属性。

## 2、修饰在方法上

语法：public static 返回值类型 方法名（数据类型  参数名）；

方法名上不带static的方法都是对象的方法。

```java
public class Demo {
    String name;
    int age;
    String lesson;
    /**
     * 对象的方法，每一个老师的实例可以使用
     */
    public void teach(){
        System.out.println("老师在上课.....");
    }
    /**
     * 类方法
     */
    public static void sleep(){
        System.out.println("老师在睡觉.....");
    }
}
class TestDemo{
    public static void main(String[] args) {
        // 使用类名来调用类方法,推荐使用
        Demo.sleep();
        //不推荐使用下面的方法来调用
        Demo demo = new Demo();
        demo.sleep();
    }
}
```

总结：上面这个实例说明，有static关键字的方法是类的属性，则可通过 “类名.方法名” 来调用。

# 四、 protected关键字

推荐一个教程链接：<http://www.runoob.com/w3cnote/java-protected-keyword-detailed-explanation.html>

# Java的单例模式

------

# 什么是单例模式(Singleton)？

设计模式：Java中，一共有23种设计模式，说白了就是比较牛的代码设计思想。单例模式里面非常简单的一种。

比如说windows的桌面回收站，只能打开这么一个回收站的界面，不能打开第二个界面。具体细节，我会拿出实例代码来详细阐述。

单例模式一般用于比较大，复杂的对象，只初始化一次，应该还有一个private的构造函数，使得不能用new来实例化对象，只能调用getInstance方法来得到对象，而getInstance保证了每次调用都返回相同的对象。

## 一、 懒汉模式

### 1、 实例

```java
public class Singleton {
    //在单例类的内部创建一个自己本身的对象
    private static Singleton s=new Singleton();
    //获得单例的对象
    public static Singleton getSingleton(){
        return s;
    }
    //构造器的私有化
    private Singleton(){
    }
}
class TestDemo{
    public static void main(String[] args) {
        Singleton s1= Singleton.getSingleton();
        System.out.println(s1);
        //测试是否引用的是否是同一个对象
        Singleton s2= Singleton.getSingleton();
        System.out.println(s2);
        Singleton s3= Singleton.getSingleton();
        System.out.println(s3);
    }
}
```

运行结果

![img](https://img-blog.csdnimg.cn/20181028171444770.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

总结：说明引用的都是同一个对象，这就是单例模式。下面分析一下原理。

### 2、 原理

其实这就是一个静态属性的生命周期。

![img](https://img-blog.csdnimg.cn/20181028172927600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

 因为构造器（就是构造函数）是空的，所以在堆中new Singleton只有一个地址，数据共享区的Singleton定义的**s**也有地址，指向堆中的0x999；当调用getSingleton()这个方法的时候，getSingleton就进栈产生一个地址，在getSingleton()执行完之后，返回了一个对象s（请看上方代码return  s; ），s正好又是new Singleton()地址（请看上方代码private static Singleton s=new  Singleton();）。所以在TestDemo中的s1的地址就会指向new  Singleton的地址，在下面继续s2、s3的时候，都会拿到一个相同的地址。

## 二、 饿汉模式

### 1、 实例

```java
public class Singleton {
    //在单例类的内部创建一个自己本身的对象
    private static Singleton s;
    public static Singleton getSingleton(){
        if (s == null){
            s=new Singleton();
            return s;
        }
        return s;
    }
    //构造器的私有化
    private Singleton(){
    }
}
class TestDemo{
    public static void main(String[] args) {
        Singleton s1= Singleton.getSingleton();
        System.out.println(s1);
    }
}
```

对比一下“懒汉模式”突然发现，“饿汉模式”巧妙的节省了资源。





# Java的继承与内存结构（上）

------

# 一、 继承概念

继承是面向对象最显著的一个特性。继承是从已有的类中派生出新的类，新的类能吸收已有类的数据属性和行为，并能扩展新的能力。

通俗一点：描述事物的时候，如果发现这几个类有重复的属性和方法，就可以采用继承的方式来设计。

## 1、语法

在Java中，使用关键字extends来继承！

class 子类   extends 父类 {

​           子类属性；

​            子类方法；

}

特点：子类会把父类的所有属性和方法继承下来（暂时不考虑final）。

## 2、 实例

请看下面举的栗子就啥都明白了~

```java
class Teacher{
  String name;
  int age;
  public void sleep(){
      System.out.println("老师正在睡觉....");
  }
}
/**
 * 定义子类
 */
class MathTeacher extends Teacher{
    public void teachMath(){
        System.out.println("数学老师在上课.....");
    }
}
class ChineseTeacher extends Teacher{
    public void teachChinese(){
        System.out.println("语文老师在上课...");
    }
}
 class ExtendsDemo {
    public static void main(String[] args) {
        MathTeacher mt =new MathTeacher();
        mt.name="数学老师";
        mt.age=28;
        mt.teachMath();
        mt.sleep();
        System.out.println("我是"+mt.name+"，我今年"+mt.age);
        System.out.println("----------------------------------");
        ChineseTeacher ct = new ChineseTeacher();
        ct.name="语文老师";
        ct.age=50;
        ct.teachChinese();
        ct.sleep();
        System.out.println("我是"+ct.name+"，我今年"+ct.age);
    }
}
```

运行结果

![img](https://img-blog.csdnimg.cn/20181029122744517.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_27,color_FFFFFF,t_70)

##  3、 总结

1、 如果父类中包含了某些类中的共同属性和行为，我们可以使用继承来设计程序。

2、子类使用extends关键字继承父类的共同属性意外，子类还可以有自己特有的属性或方法。

3、 父类更通用，子类更加具体。

4、 子类只能获得父类的非private的属性，如果想要继承就得提供公共的set和get的方法。私有的方法子类无法继承。

5、 在Java中只能做单继承（如，class A extends B{  } ，那什么叫多继承呢？如class A extends B,C{  }）。

6、 Java支持多级继承（如 class A extends B{  }         class C extends B{   }）。

继承的好处：提高了代码的复用性。

# 二、 继承的内存结构

下面就上面的实例分析。

## ![img](https://img-blog.csdnimg.cn/20181030155220474.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_16,color_FFFFFF,t_70)

和文件同名的类为主类，主类ExtendsDemo先进栈，main方法进栈。mt是MathTeacher的一个对象，mt指向堆中它自己所在的地址，然后找到在栈中的内存空间。当mt在找name这个属性的时候，会到栈中去找，当然这个name的属性并不存在0x666这个内存空间里面，然后会到父类中去找。在this里面有一个存在super、name属性等的实例，找到之后给name赋值“DJun”，mt找age也是相同的方法。之后加载teachMath到方法区，接着入栈。

# Java的继承与内存结构（下）	

前面一个多月工作室接了一个项目，就学Vue去了。Java这边就停更了一段时间，还望见谅。

在Vue这边还是啰嗦两句吧，不愿意看的话直接跳过即可。Vue上手很快，前端开发起来也是非常快的（不考虑UI的情况下），目前Vue比React和Angular好，但是公认缺陷是社区不够成熟（也接触了uni-app，这是基于Vue的一个跨平台开发技术，但是uni的UI很low，能用的UI框架少之又少）。这个项目的第一期完成之后（后面这个项目如果接着做的话，Vue和Java都接着更），最近也反省了一下自己学的东西，多而杂反而一门不精。故今日重回Java  Web这条路上来了，相信我您能和我一样在Java这条路上砥砺前行。默默地说一句：“Java真香～”

## 1、 补充内容

这篇文章主要弥补上面一篇没有讲到的一些知识点，上篇链接：[Java的继承与内存结构（上）——有C++基础的Java入门（六）](https://blog.csdn.net/qq_41647999/article/details/83503856)

- 回顾语法：class A extends B { }，A为子类，B为父类（也称为基类或SuperClass），extends翻译过来有扩展的意思，就是子类是对父类功能的扩展。
- Java的继承性只支持单继承： 一个子类只能一个父类。反过来的话，一个父类可以有多个子类。但是可以通过把类定义成接口的方式来打破Java的单继承性。
- 子类和父类是一个相对的概念，子类不属于父类。

## 2、 四种权限修饰符

下面只讲与C++的区别。

![img](https://img-blog.csdnimg.cn/20190127175149545.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjQ3OTk5,size_16,color_FFFFFF,t_70)  

（1）子类覆盖父类的方法：子类中定义的覆盖函数与父类同名，一般权限为**public**，即可复写函数。

（2） **default** 权限修饰符：default表示默认，不仅本类访问，而且是同包可见（同包的意思就是在同一级的文件夹）。

## 3、 super关键字

（1） 用于访问父类中定义的属性

（2） 用于调用父类总定义的成员方法

（3） 用于在子类构造方法中调用父类的构造器

（4） 当子类和父类中出现同名的成员，可以通过Super来区分

（5） Super和this的用法相像，this代表本类对象的引用，super代表父类的内存空间的表示。

实例：

```java
class Person {
	protected String name="张三";
             protected int age;
	public String getInfo() {
        return "Name: " + name + "\n"+age: " + age;
    }
}
class Student extends Person {
    protected String name = "李四";
	private String school = "New Oriental";
	public String getSchool(){ return school; }
    public String getInfo() {
        return super.getInfo() +"\n"+school: " +school;
    }
}
public class TestStudent{
	public static void main(String[] args){
		Student st = new Student();
		System.out.println(st.getInfo());
	}
}
```

 

 