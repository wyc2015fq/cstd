# 牛客网Java错题知识点 - westbrook1998的博客 - CSDN博客





置顶2018年03月30日 21:59:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：161








### 1. 子类调用父类方法

> 
子类要调用继承自父类的方法，必须使用super关键字。————（错误）

- 子类可以直接调用父类未重写（Override）的方法
- 子类可以`new`一个父类对象再调用父类方法
- 子类构造器默认调用父类构造器`super()`

### 2. J2EE请求方法

> 
J2EE中，当把来自客户机的HTTP请求委托给servlet时，会调用HttpServlet的（service）方法


HttpServlet容器响应Web客户请求流程如下：
- Web客户向Servlet容器发出Http请求；
- Servlet容器解析Web客户的Http请求；
- Servlet容器创建一个HttpRequest对象，在这个对象中封装Http请求信息；
- Servlet容器创建一个HttpResponse对象；
- Servlet容器调用HttpServlet的service方法，这个方法中会根据request的Method来判断具体是执行doGet还是doPost，把HttpRequest和HttpResponse对象作为`service`方法的参数传给HttpServlet对象；
- HttpServlet调用HttpRequest的有关方法，获取HTTP请求信息；
- HttpServlet调用HttpResponse的有关方法，生成响应数据；
- Servlet容器把HttpServlet的响应结果传给Web客户。

### 3. final成员变量初始化

```
class Foo {
    final int i;
    int j;
    public void doSomething() {
        System.out.println(++j + i);
    }
}
```

的输出是————（编译出错）
- final变量是不可修改
- final变量定以后必须**赋初值**，或者**在构造器中赋值**

### 4. byte short变量计算问题

```
1. byte a1 = 2, a2 = 4, a3;
2. short s = 16;
3. a2 = s;
4. a3 = a1 * a2;
```

出错——第三、四行
- short变量不能赋值给byte变量（高级->低级）
- `byte<char<short<int<long<float<double`
- byte short类型在计算时会转换成`int`类型再进行计算

### 5. 方法重载
- 方法重载（Overload）就是方法名相同，方法`参数的个数和类型`（可能）不同，通过个数和类型的不同来区分不同的函数
- 方法的重载跟返回值类型和修饰符无关

### 6. 修饰符权限问题

> 
下列哪个修饰符可以使在一个类中定义的成员变量只能被同一包中的类访问？————（默认修饰符 default）

|修饰符|类内部|同一个包|子类|任何地方|
|----|----|----|----|----|
|private|√||||
|default|√|√|||
|protected|√|√|√||
|public|√|√|√|√|

### 6. 局部变量必须要有初始值

局部变量必须初始化

### 7. JVM基础问题

JVM初始运行的时候都会分配好 `Method Area（方法区）` 和`Heap（堆）` ——**线程共享区域**，而JVM 每遇到一个线程，就为其分配一个 `Program Counter Register（程序计数器）` ,   `VM Stack（虚拟机栈）`和`Native Method Stack （本地方法栈）`——**线程私有区域**， 当线程终止时，三者（虚拟机栈，本地方法栈和程序计数器）所占用的内存空间也会被释放掉。

### 8. 集合类泛型问题

> 
集合类使用时可以不事先知道存储的类型，默认为`Object`


### 9. 异常抛出问题

> 
在java中，我们可以用违例（Exception）来抛出一些并非错误的消息，但这样比直接从函数返回一个结果要`更大的系统开销`。


因为`try{}catch{}`会导致额外的开销

### 10. 接口中不能定义变量

> 
java接口包含函数声明和变量声明。————（错）


接口中的属性默认是`public static final`类型的，即为常量

### 11. 泛型擦除

> 
在开发中使用泛型取代非泛型的数据类型（比如用ArrayList取代ArrayList），程序的运行时性能会变得更好。————（错）


在编译阶段存在的**泛型擦除机制**并不会影响到程序的运行速度

### 12. 构造方法

> 
下列关于构造方法不正确的是：（ D）

A 类的构造方法和类同名

B 一个类可以有多个构造方法

C 在实例化对象时必须调用类的构造方法

D 构造方法可以和该类其他方法一样被该类对象调用- 构造方法不能向其他方法一样被对象调用，只能通过new关键字调用
- 反射的本质也是间接调用类的无参构造

### 13. Thread的start和run方法

> 
下列程序的输出结果——pongping


```
public static void main(String args[]) {
   Thread t = new Thread() {
            public void run() {
                pong();
            }
        };
        t.run();
        System.out.print("ping");
    }
    static void pong() {
        System.out.print("pong");
    }
```
- start方法才能真正启动线程，此时线程会处于就绪状态，一旦得到时间片，则会调用线程的run方法进入运行状态。
- 而run方法只是普通方法，如果直接调用run方法，程序只会按照顺序执行主线程这一个线程。

### 14. 字符串是不可修改的

```
package com.zxc;

public class Example {
    private String str = "good";

    public void exchange(String str) {
        System.out.println(str.getBytes());
        //str = "bad";     //good
        this.str="bad";    //bad
    }

    public static void main(String[] args) {
        Example example = new Example();
        System.out.println(example.str.getBytes());
        example.exchange(example.str);
        System.out.println(example.str);
    }
}
```

作为参数传入的String只是一个拷贝，其实是一个新的引用

> 
***对于引用类型 str，赋值运算符会改变引用中所保存的地址，原来的地址被覆盖掉。但是原来的对象不会被改变（重要）。***

比如方法内的`str="a";`实际上是产生了一个新的引用赋给str


### 15. instanceof

> 
instanceof运算符能够用来判断一个对象是否为:    (全部正确)

1.一个类的实例

2.一个实现指定接口的类的实例

3.全部正确

4.一个子类的实例
### 16. 类不能多继承，但接口可以

```
interface A{}
interface B{}
interface C extends A,B{}

class D{}
class E{}
//class F extends D,E{}
```

### 17. 抽象类有构造方法，但不能实例化

类都有默认的构造方法

```
abstract class T{
    private int a;
    T(int a){
        this.a=a;
    }
}
```

### 18. java异常分类

异常的继承结构：基类为`Throwable`，`Error`和`Exception`继承`Throwable`，`RuntimeException`和`IOException`等继承`Exception`
*Error主要是用于表示Java和虚拟机内部的异常信息，而Exception异常则是由于程序中可能存在各种的问题，是需要使用者去注意和捕获的异常。*
**非RuntimeException一般是外部错误(非Error)，其必须被 try{}catch语句块所捕获**
**Error类体系描述了Java运行系统中的内部错误以及资源耗尽的情形，Error不需要捕捉**
### 19.switch可用类型

switch语句后的控制表达式只能是short、char、int、long整数类型和枚举类型，不能是float，double和boolean类型。**String类型是java7开始支持**

### 20.多线程的resume方法

**suspend() 和 resume() 方法：两个方法配套使用，suspend()使得线程进入阻塞状态，并且不会自动恢复，必须其对应的 resume() 被调用，才能使得线程重新进入可执行状态**

### 21.静态成员与非静态成员

```
public class test {
    public static int a=1;
    public int b=2;

    public static void main(String[] args) {
        System.out.println(a);
        System.out.println(b);
        System.out.println(new test().a);
        System.out.println(new test().b);
    }
}
```

**这个例子中，main函数是静态函数，可以直接访问静态变量，但不可以访问非静态变量，而实例化一个test类之后，实例可以访问静态变量，也可以访问非静态变量**

### 22. 匿名内部类不能定义构造器

因为构造器名字和类名相同，而匿名内部类没有名字，因此无法定义构造器

### 23. `Math.round()`

四舍五入，或者说往正无穷方向找到最近的一个整数

```
public static void main(String[] args) {
        //四舍五入 或者说往正无穷方向找到最近的一个整数
        System.out.println(Math.round(1.4));
        //小数默认是double round方法返回值即为long
        System.out.println(Math.round(1.5));
        //如果参数是float 返回值即是int
        System.out.println(Math.round(1.5f));
        System.out.println(Math.round(-1.5));
        System.out.println(Math.round(-1.4));
    }
```

顺便看`floor()`和`ceil()`, 即向上取整和向下取整，而`round()` 其实实质就是加0.5再向上取整

### 24. 类型转换

> 
以下会产生信息丢失的类型转换是( ）

float  a=10

int a=（int）8846.0

byte  a=10； int b=-a

double d=100
这里应该选择B 这里题意的信息丢失指的是精度丢失，尽管这个8846.0转为int后大小不变，但大转小，精度丢失。

### 25. `ArrayList LinkedList`

ArrayList和LinkedList都是List的子接口

ArrayList由**数组**实现，LinkedList由**链表**实现

数组的访问速度比链表快,随机添加删除元素时，链表的效率要高于数组

总结：`ArrayList` 随机访问快，`LinkedList` 随机添加和删除快
### 26. 键值为`null` 的`map`

```
public static void main(String[] args) {
        Map m1=new HashMap();
        m1.put(null,0);
        m1.put(null,null);
        System.out.println(m1.get(null));
    }
```

### 27. 运算符优先级
|运算符|简介|
|----|----|
|`[ ] . ( )(方法调用)`|数组 成员调用|
|`! ~ ++ -- +(一元运算) -(一元运算)`|两个表示非的 自增自减 正负号|
|`* / %`|乘除取模|
|`+ -`|加减|
|`<< >> >>>`|位的左移 右移 无符号右移（对负数有区别 忽略符号位）|
|`< <= > >= instanceof`|大小判断 类型判断|
|`== !=`|相等判断|
|`&`|按位与|
|`^`|按位异或|
|`\|`|按位或|
|`&&`|条件与|
|`||`|条件或|
|`?:`|三目表达式|
|`=`|赋值（最低）|

### 28. 代码块执行顺序

代码：

```
public class HelloB extends HelloA 
{
 public HelloB()
 {
 }
 {
     System.out.println("I’m B class");
 }
 static
 {
     System.out.println("static B");
 }
 public static void main(String[] args)
 {
     new HelloB();
 }
}
class HelloA
{
 public HelloA()
 {
 }
 {
     System.out.println("I’m A class");
 }
 static
 {
     System.out.println("static A");
 }
}
//结果
//static A
//static B
//I’m A class
//I’m B class
```

顺序如下：

1、执行父类的静态代码块

static {

System.out.println(“static A”);

}

输出:static A

2、执行子类的静态代码块

static {

System.out.println(“static B”);

}

输出:static B

3、执行父类的普通初始化代码块

{

System.out.println(“I’m A class”);

}

输出:I’m A class

4、执行父类的构造函数

public HelloA() {

}

输出：无

5、执行子类的普通初始化代码块

{

System.out.println(“I’m B class”);

}

输出:I’m B class

6、执行子类的构造函数

public HelloB() {

}

输出：无

————来自牛客网
### 29.子类构造器

子类并不会继承父类的构造器，只存在显式或者隐式调用

### 30. 子类可以继承父类所有成员

子类可以继承父类所有成员（包括私有成员），但无法调用父类私有成员

### 31. JSP九大内置对象
|对象|简单作用|
|----|----|
|pageContext|表示页容器 EL表达式、 标签 、上传|
|request|服务器端取得客户端的信息：头信息 、Cookie 、请求参数 ，最大用处在MVC设计模式上|
|response|服务器端回应客户端信息：Cookie、重定向|
|session|表示每一个用户，用于登录验证上|
|application|表示整个服务器|
|config|取得初始化参数，初始化参数在web.xml文件中配置|
|exception|表示的是错误页的处理操作|
|page|如同this一样，代表整个jsp页面自身|
|out|输出 ，但是尽量使用表达式输出|

### 32. 加号的结合顺序

```
int x=20;
int y=5;
System.out.println(x+y +""+(x+y)+y);
//结果输出25255
```

从左到右，先是数字的20+5=25，然后25+"“得到字符串的"25”，然后再往右，“25”+（20+5），相当于"25"+25，就是’2525"，最后再加5，得到"25255"

### 33. 字符串比较

```
public static void main(String[] args) {
        String a=new String("java");
        String b="java";
        String c="java";
        String d="kotlin";
        char e[]={'j','a','v','a'};

        //false
        //==默认是比较对象的地址 这里a使用了new 开辟了新的内存空间 而b直接赋值则是引用了常量池的字符串"java"
        System.out.println(a==b);

        //true
        //String类重写了equals方法，比较的是两个字符串对象的内容
        System.out.println(a.equals(b));

        //true
        //b和c都是直接字符串赋值，会先从常量池里找，所以都是引用同一个对象，也就是同一个地址
        System.out.println(b==c);

        //true
        //比较内容也是相等
        System.out.println(b.equals(c));

        //字符串对象和char数组无法直接用==比较 编译期报错
        //System.out.println(a==e);
        //System.out.println(b==e);

        //字符串和字符数组可以用equals比较，但结果肯定是false的
        System.out.println(a.equals(e));
        System.out.println(b.equals(e));
    }
```

### 34. 接口和抽象类可以定义成员变量（常量）

接口中的成员变量必须初始化

```
abstract class a{
    static int b;
}

interface c{
    static int d=1;
}
```

### 35. `&` 不短路与

java中，`&` 不仅可以表示位运算，在对两个布尔变量也表示与的意思，但是**不短路**，也就是说第一个条件就算错误也会执行第二个条件

### 36. null对象也可以调用静态方法

```
public class test {
    public static void a(){
        System.out.println("hello");
    }
    public static void main(String[] args) {
        test t=null;
        t.a();
    }
}
```

### 37. `finally`中的`return`语句会覆盖`try`中的`return`语句

换句话说，就算`try`中有`return` ，也得继续执行`finally`，如果`finally` 中没有`return` 才是`try`中的`return`，而如果`finally` 中有`return` 语句，就会执行`finally` 中的语句（包括其他赋值或修改变量的语句）

```
public static int a(int b){
        try {
            b += 1;
            return b;
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            b+=1;
            return b;
        }
    }

    public static int c(int b){
        try {
            b += 1;
            return b;
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            b+=1;
        }
        return b;
    }

    public static void main(String[] args) throws Exception {
        int b=1;
        System.out.println(a(b));
        System.out.println(c(b));
    }
```

### 38. 抽象类和接口的默认方法修饰符
- 
抽象类

JDK 1.8以前，抽象类中的方法默认为protected
**JDK 1.8时，抽象类中的方法默认变为default**- 
接口

JDK 1.8以前，接口中的方法必须是public的

JDK 1.8时，接口中的方法可以是default的
**JDK 1.9时，接口中的方法可以是private的（default）,这两种修饰符必须要有方法体**
### 39. 类继承的初始化
- 初始化父类中的静态成员变量和静态代码块 ；
- 初始化子类中的静态成员变量和静态代码块 ；
- 初始化父类的普通成员变量和代码块，再执行父类的构造方法；
- 初始化子类的普通成员变量和代码块，再执行子类的构造方法；

```
class X{
    Y y=new Y();
    public X(){
        System.out.print("X");
    }
}
class Y{
    public Y(){
        System.out.print("Y");
    }
}
public class Z extends X{
    Y y=new Y();
    public Z(){
        System.out.print("Z");
    }
    public static void main(String[] args) {
        new Z();
    }
}
//输出YXYZ
```

### 40. static变量只能定义在类中

static变量只能定义在类中，不能定义在方法中

### 41. try…catch…finally的返回顺序

这段代码中，`catch`中有`return`,会先执行`finally`再返回`catch`的`return`,不会再执行`try...catch`外的

```
package algorithms.com.guan.javajicu; 
public class TestDemo { 
    public static String output =""; 
    public static void foo(int i){ 
       try{ 
           if(i == 1){ 
              throw new Exception(); 
           } 
       }catch(Exception e){ 
           output += "2"; 
           return ; 
       }finally{ 
           output += "3"; 
       } 
       output += "4"; 
    } 
    
    public static void main(String[] args) { 
       foo(0); 
       foo(1); 
       System.out.println(output);
    } 
}
```

### 42. 内部类一道题

> 
下列说法正确的是（）？

- 对于局部内部类，只有在方法的局部变量被标记为final或局部变量是effctively final的，内部类才能使用它们
- 成员内部类位于外部类内部，可以直接调用外部类的所有方法（静态方法和非静态方法）
- 由于匿名内部类只能用在方法内部，所以匿名内部类的用法与局部内部类是一致的
- 静态内部类可以访问外部类的成员变量

正确答案是1和2

```
package sometest;

public class Out {

    void test(){
        //jdk8之后可以不加final 但在编译时会自动加上
        int a=5;

        //局部内部类访问外部方法的临时变量
        class innerTest{
            void getA(){
                System.out.println(a);
            }
        }

        new innerTest().getA();
    }

    class In{
        //内部类直接访问外部类成员
        void inTest(){
            test();
        }
    }

    public static void main(String[] args) {
        new Out().test();
        new Out().new In().inTest();
    }
}
```

第3，匿名内部类和局部内部类是不一样的，至少，匿名内部类只能用一次，而局部内部类可以在方法内重复调用，第4，静态内部类其实可以看成是一个外部类，不能直接访问外部类成员。

### 43. 接口修饰符

接口完整的修饰符(默认)如下

接口名是`(public) abstract` 接口就是高度抽象的类

变量一定是`public static final` 公开的，静态的，不可修改的

方法一定是`public abstract` 抽象方法
```
public abstract interface A {
    
    public static final int b=5;

    public abstract void getB();

}
```

### 44. JavaWeb获取参数的一些方法

> 

`getParameter()`是获取POST/GET传递的参数值；
`getInitParameter()`获取Tomcat的server.xml中设置Context的初始化参数
`getAttribute()`是获取对象容器中的数据值；
`getRequestDispatcher()`是请求转发。
### 45. Object类拥有的方法

```
1. clone();
2. equals();
3. finalize();
4. getClass();
5. notify();
6. notifyAll();
7. hashCode();
8. toString();
9. wait();
```

### 46. 变量提升类型

> 
Java表达式转型规则由低到高转换：

1、所有的byte,short,char型的值将被提升为int型；

2、如果有一个操作数是long型，计算结果是long型；

3、如果有一个操作数是float型，计算结果是float型；

4、如果有一个操作数是double型，计算结果是double型；

5、被fianl修饰的变量不会自动改变类型，当2个final修饰相操作时，结果会根据左边变量的类型而转化。``public static void main(String[] args) {
        byte b1=1;
        byte b2=2;
        byte b3;
        //报错 b1+b2得到的类型是int
        //b3=b1+b2;
        final byte b4=1;
        final byte b5=2;
        final byte b6;
        //final变量b4 b5不会自动提升，所得变量类型由左边决定，即b6可以为任意类型
        b6=b4+b5;     
    }
47. 抽象类和接口的方法默认修饰符

抽象类默认是``default

接口默认是``public48. 变量存放在内存``class A {
    private String a = “aa”;
    public boolean methodB() {
        String b = “bb”;
        final String c = “cc”;
    }
}


其中，a是类的成员变量，在堆区，b和c是方法的局部变量，在栈区49. 子类调用父类构造方法

当父类显式声明了带参构造而没有无参构造时，子类就需要显式调用父类的无参构造方法``super()``class Base{
    public Base(String s){
        System.out.print("B");
    }
}
public class Derived extends Base{
    public Derived (String s) {
	    super(s);
        System.out.print("D");
    }
    public static void main(String[] args){
        new Derived("C");
    }
}
50. StringBuffer是线程安全

而StringBuilder就不是，其他功能相同51. suspend和resume方法
> 


suspend() 和 resume() 方法：两个方法配套使用，suspend()使得线程进入阻塞状态，并且不会自动恢复，必须其对应的 resume() 被调用，才能使得线程重新进入可执行状态52. 原子操作
> 


以下多线程对int型变量x的操作,哪个不需要进行同步()

x++

x=y

++x

x=1

正确答案应该是D x=1，其中x=y其实也是要先读取再修改的53. 成员内部类和静态内部类

静态内部类可以访问外部类的静态成员，包括私有静态成员，成员内部类可以访问外部类的所有成员，包括私有成员54. 类的几个易错点
> 


一个文件中只能有一个public class。(错，内部类可以是public class)

一个文件中可以有多个类。(对)

一个类中可以有两个main方法。   (对，可以重载main方法，而public static void main(String[] args)只能一个)

若类中只含一个main方法，则必须是public的。(错，类中不一定要有主方法)55. HashMap和HashTable
> 


HashMap允许null作为键值，而HashTalbe如果key或value为null会抛出异常56. 字符串比较
```java

```
publicclassStringDemo{privatestaticfinal String MESSAGE="taobao";publicstaticvoidmain(String [] args){
    String a ="tao"+"bao";
    String b="tao";
    String c="bao";
    System.out.println(a==MESSAGE);
    System.out.println((b+c)==MESSAGE);}}

这里会输出true和false

第一个a其实编译后就是看成一个字符串常量"taobao"了，所以是true

而第二个b+c其实是new了一个String，所以是false57. 字节流编码转换
> 


GBK编码字节流到UTF-8编码字节流的转换
```java

```
dst=newString(src，"GBK").getBytes("UTF-8")

用一个String对象作为中转58. Integer的比较

先放代码：
```java

```
publicclassMain{publicstaticvoidmain(String[] args){
        Integer a =1000;int b =1000;
        Integer c = Integer.valueOf(1000);
        Integer d =newInteger(1000);
        System.out.println(a == b);
        System.out.println(a == c);
        System.out.println(a == d);
        System.out.println(b == d);
        Integer aa =100;int bb =100;
        Integer cc = Integer.valueOf(100);
        Integer dd =newInteger(100);
        System.out.println(aa == bb);
        System.out.println(aa == cc);
        System.out.println(aa == dd);
        System.out.println(bb == dd);}}

首先整数在-128到127这个区间内是有一个缓存的机制的，都是同一个对象，所以第一组测试，值是1000，这时b也就是int基本类型和其他两个对象比较都是true，因为有基础类型的比较都是比较值，这一点在第二组测试也是一样的，而第三个比较，是两个不同的对象，所以肯定是false，无论是缓存或是不缓存，有区别的就是在第二个比较，是两个对象的比较，这时候如果是在缓存区间内的，比较即为true，否则为false59.  关键字

true false null不是关键字，是常量

反倒是goto const是保留的关键字60. 成员构造方法优先于类构造方法
```java

```
classfather{father(){
        System.out.println("father");}}classgrandson{grandson(){
        System.out.println("grandson");}}publicclasssonextendsfather{
    grandson grandson=newgrandson();son(){
        System.out.println("son");}publicstaticvoidmain(String[] args){newson();}}

顺序是

父类静态块—>子类静态块—>父类成员初始化—>父类构造块—>父类构造函数—>子类成员初始化—>子类构造块—>子类构造函数61. 静态初始化

静态成员变量和静态初始化块都先于其他执行，而这二者按顺序先后执行62. 构造器什么时候调用

创建一个Object大概有四种方法，new，反射，反序列化，克隆，而只有前两种调用了构造方法63.



















