# java程序性能优化 - z69183787的专栏 - CSDN博客
2013年03月22日 14:26:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1640
个人分类：[Java-知识																[性能优化](https://blog.csdn.net/z69183787/article/category/2185941)](https://blog.csdn.net/z69183787/article/category/2175279)
一、避免在循环条件中使用复杂表达式
在不做编译优化的情况下，在循环中，循环条件会被反复计算，如果不使用复杂表达式，而使循环条件值不变的话，程序将会运行的更快。 
例子： 
```java
import java.util.vector;
class cel {
    void method (vector vector) {
        for (int i = 0; i < vector.size (); i++)  // violation
            ; // ...
    }
}
```
更正： 
```java
class cel_fixed {
    void method (vector vector) {
        int size = vector.size ()
        for (int i = 0; i < size; i++)
            ; // ...
    }
}
```
二、为'vectors' 和 'hashtables'定义初始大小 
jvm为vector扩充大小的时候需要重新创建一个更大的数组，将原原先数组中的内容复制过来，最后，原先的数组再被回收。可见vector容量的扩大是一个颇费时间的事。 
通常，默认的10个元素大小是不够的。你最好能准确的估计你所需要的最佳大小。 
例子： 
```java
import java.util.vector;
public class dic {
    public void addobjects (object[] o) {
        // if length > 10, vector needs to expand
        for (int i = 0; i< o.length;i++) {    
            v.add(o);   // capacity before it can add more elements.
        }
    }
    public vector v = new vector();  // no initialcapacity.
}
```
更正： 
自己设定初始大小。 
```java
public vector v = new vector(20);  
    public hashtable hash = new hashtable(10);
```
参考资料： 
dov bulka, "java performance and scalability volume 1: server-side programming 
techniques" addison wesley, isbn: 0-201-70429-3 pp.55 – 57 
三、在finally块中关闭stream
程序中使用到的资源应当被释放，以避免资源泄漏。这最好在finally块中去做。不管程序执行的结果如何，finally块总是会执行的，以确保资源的正确关闭。 
例子： 
```java
import java.io.*;
public class cs {
    public static void main (string args[]) {
        cs cs = new cs ();
        cs.method ();
    }
    public void method () {
        try {
            fileinputstream fis = new fileinputstream ("cs.java");
            int count = 0;
            while (fis.read () != -1)
                count++;
            system.out.println (count);
            fis.close ();
        } catch (filenotfoundexception e1) {
        } catch (ioexception e2) {
        }
    }
}
```
更正： 
在最后一个catch后添加一个finally块 
参考资料： 
peter haggar: "practical java - programming language guide". 
addison wesley, 2000, pp.77-79 
四、使用'system.arraycopy ()'代替通过来循环复制数组
'system.arraycopy ()' 要比通过循环来复制数组快的多。 
例子： 
```java
public class irb
{
    void method () {
        int[] array1 = new int [100];
        for (int i = 0; i < array1.length; i++) {
            array1 [i] = i;
        }
        int[] array2 = new int [100];
        for (int i = 0; i < array2.length; i++) {
            array2 [i] = array1 [i];                 // violation
        }
    }
}
```
更正： 
```java
public class irb
{
    void method () {
        int[] array1 = new int [100];
        for (int i = 0; i < array1.length; i++) {
            array1 [i] = i;
        }
        int[] array2 = new int [100];
        system.arraycopy(array1, 0, array2, 0, 100);
    }
}
```
参考资料： 
http://www.cs.cmu.edu/~jch/java/speed.html 
五、让访问实例内变量的getter/setter方法变成”final”
简单的getter/setter方法应该被置成final，这会告诉编译器，这个方法不会被重载，所以，可以变成”inlined” 
例子： 
```java
class maf {
    public void setsize (int size) {
         _size = size;
    }
    private int _size;
}
```
更正： 
```java
class daf_fixed {
    final public void setsize (int size) {
         _size = size;
    }
    private int _size;
}
```
参考资料： 
warren n. and bishop p. (1999), "java in practice", p. 4-5 
addison-wesley, isbn 0-201-36065-9 
六、避免不需要的instanceof操作
如果左边的对象的静态类型等于右边的，instanceof表达式返回永远为true。 
例子：         
```java
public class uiso {
    public uiso () {}
}
class dog extends uiso {
    void method (dog dog, uiso u) {
        dog d = dog;
        if (d instanceof uiso) // always true.
            system.out.println("dog is a uiso");
        uiso uiso = u;
        if (uiso instanceof object) // always true.
            system.out.println("uiso is an object");
    }
}
```
更正：         
删掉不需要的instanceof操作。 
```java
class dog extends uiso {
    void method () {
        dog d;
        system.out.println ("dog is an uiso");
        system.out.println ("uiso is an uiso");
    }
}
```
七、避免不需要的造型操作
所有的类都是直接或者间接继承自object。同样，所有的子类也都隐含的“等于”其父类。那么，由子类造型至父类的操作就是不必要的了。 
例子： 
```java
class unc {
    string _id = "unc";
}
class dog extends unc {
    void method () {
        dog dog = new dog ();
        unc animal = (unc)dog;  // not necessary.
        object o = (object)dog;         // not necessary.
    }
}
```
更正：         
```java
class dog extends unc {
    void method () {
        dog dog = new dog();
        unc animal = dog;
        object o = dog;
    }
}
```
参考资料： 
nigel warren, philip bishop: "java in practice - design styles and idioms 
for effective java".  addison-wesley, 1999. pp.22-23 
八、如果只是查找单个字符的话，用charat()代替startswith()
用一个字符作为参数调用startswith()也会工作的很好，但从性能角度上来看，调用用string api无疑是错误的! 
例子： 
```java
public class pcts {
    private void method(string s) {
        if (s.startswith("a")) { // violation
            // ...
        }
    }
}
```
更正         
将'startswith()' 替换成'charat()'. 
```java
public class pcts {
    private void method(string s) {
        if ('a' == s.charat(0)) {
            // ...
        }
    }
}
```
参考资料： 
dov bulka, "java performance and scalability volume 1: server-side programming 
techniques"  addison wesley, isbn: 0-201-70429-3 
九、使用移位操作来代替'a / b'操作
"/"是一个很“昂贵”的操作，使用移位操作将会更快更有效。 
例子： 
```java
public class sdiv {
    public static final int num = 16;
    public void calculate(int a) {
        int div = a / 4;            // should be replaced with "a >> 2".
        int div2 = a / 8;         // should be replaced with "a >> 3".
        int temp = a / 3;
    }
}
```
更正： 
```java
public class sdiv {
    public static final int num = 16;
    public void calculate(int a) {
        int div = a >> 2;  
        int div2 = a >> 3;
        int temp = a / 3;       // 不能转换成位移操作
    }
}
```
十、使用移位操作代替'a * b'
同上。 
[i]但我个人认为，除非是在一个非常大的循环内，性能非常重要，而且你很清楚你自己在做什么，方可使用这种方法。否则提高性能所带来的程序晚读性的降低将是不合算的。 
例子： 
```java
public class smul {
    public void calculate(int a) {
        int mul = a * 4;            // should be replaced with "a << 2".
        int mul2 = 8 * a;         // should be replaced with "a << 3".
        int temp = a * 3;
    }
}
```
更正： 
```java
package opt;
public class smul {
    public void calculate(int a) {
        int mul = a << 2;  
        int mul2 = a << 3;
        int temp = a * 3;       // 不能转换
    }
}
```
十一、在字符串相加的时候，使用 ' ' 代替 " "，如果该字符串只有一个字符的话
例子： 
```java
public class str {
    public void method(string s) {
        string string = s + "d"  // violation.
        string = "abc" + "d"      // violation.
    }
}
```
更正： 
将一个字符的字符串替换成' ' 
```java
public class str {
    public void method(string s) {
        string string = s + 'd'
        string = "abc" + 'd'   
    }
}
```
十二、不要在循环中调用synchronized(同步)方法
方法的同步需要消耗相当大的资料，在一个循环中调用它绝对不是一个好主意。 
例子： 
```java
import java.util.vector;
public class syn {
    public synchronized void method (object o) {
    }
    private void test () {
        for (int i = 0; i < vector.size(); i++) {
            method (vector.elementat(i));    // violation
        }
    }
    private vector vector = new vector (5, 5);
}
```
更正： 
不要在循环体中调用同步方法，如果必须同步的话，推荐以下方式： 
```java
import java.util.vector;
public class syn {
    public void method (object o) {
    }
private void test () {
    synchronized{//在一个同步块中执行非同步方法
            for (int i = 0; i < vector.size(); i++) {
                method (vector.elementat(i));   
            }
        }
    }
    private vector vector = new vector (5, 5);
}
```
十三、将try/catch块移出循环
把try/catch块放入循环体内，会极大的影响性能，如果编译jit被关闭或者你所使用的是一个不带jit的jvm，性能会将下降21%之多! 
例子：         
```java
import java.io.fileinputstream;
public class try {
    void method (fileinputstream fis) {
        for (int i = 0; i < size; i++) {
            try {                                      // violation
                _sum += fis.read();
            } catch (exception e) {}
        }
    }
    private int _sum;
}
```
更正：         
将try/catch块移出循环         
```java
void method (fileinputstream fis) {
        try {
            for (int i = 0; i < size; i++) {
                _sum += fis.read();
            }
        } catch (exception e) {}
    }
```
参考资料： 
peter haggar: "practical java - programming language guide". 
addison wesley, 2000, pp.81 – 83 
十四、对于boolean值，避免不必要的等式判断
将一个boolean值与一个true比较是一个恒等操作(直接返回该boolean变量的值). 移走对于boolean的不必要操作至少会带来2个好处： 
1)代码执行的更快 (生成的字节码少了5个字节)； 
2)代码也会更加干净 。 
例子： 
```java
public class ueq
{
    boolean method (string string) {
        return string.endswith ("a") == true;   // violation
    }
}
```
更正： 
```java
class ueq_fixed
{
    boolean method (string string) {
        return string.endswith ("a");
    }
}
```
十五、对于常量字符串，用'string' 代替 'stringbuffer'
常量字符串并不需要动态改变长度。 
例子： 
```java
public class usc {
    string method () {
        stringbuffer s = new stringbuffer ("hello");
        string t = s + "world!";
        return t;
    }
}
```
更正： 
把stringbuffer换成string，如果确定这个string不会再变的话，这将会减少运行开销提高性能。 
十六、用'stringtokenizer' 代替 'indexof()' 和'substring()'
字符串的分析在很多应用中都是常见的。使用indexof()和substring()来分析字符串容易导致 stringindexoutofboundsexception。而使用stringtokenizer类来分析字符串则会容易一些，效率也会高一些。 
例子： 
```java
public class ust {
    void parsestring(string string) {
        int index = 0;
        while ((index = string.indexof(".", index)) != -1) {
            system.out.println (string.substring(index, string.length()));
        }
    }
}
```
参考资料： 
graig larman, rhett guthrie: "java 2 performance and idiom guide" 
prentice hall ptr, isbn: 0-13-014260-3 pp. 282 – 283 
十七、使用条件操作符替代"if (cond) return; else return;" 结构
条件操作符更加的简捷 
例子： 
```java
public class if {
    public int method(boolean isdone) {
        if (isdone) {
            return 0;
        } else {
            return 10;
        }
    }
}
```
更正： 
```java
public class if {
    public int method(boolean isdone) {
        return (isdone ? 0 : 10);
    }
}
```
十八、使用条件操作符代替"if (cond) a = b; else a = c;" 结构
例子： 
```java
public class ifas {
    void method(boolean istrue) {
        if (istrue) {
            _value = 0;
        } else {
            _value = 1;
        }
    }
    private int _value = 0;
}
```
更正： 
```java
public class ifas {
    void method(boolean istrue) {
        _value = (istrue ? 0 : 1);       // compact expression.
    }
    private int _value = 0;
}
```
十九、不要在循环体中实例化变量
在循环体中实例化临时变量将会增加内存消耗 
例子：         
```java
import java.util.vector;
public class loop {
    void method (vector v) {
        for (int i=0;i < v.size();i++) {
            object o = new object();
            o = v.elementat(i);
        }
    }
}
```
更正：         
在循环体外定义变量，并反复使用         
```java
import java.util.vector;
public class loop {
    void method (vector v) {
        object o;
        for (int i=0;i<v.size();i++) {
            o = v.elementat(i);
        }
    }
}
```
二十、确定 stringbuffer的容量
stringbuffer的构造器会创建一个默认大小(通常是16)的字符数组。在使用中，如果超出这个大小，就会重新分配内存，创建一个更大的数组，并将原先的数组复制过来，再丢弃旧的数组。在大多数情况下，你可以在创建stringbuffer的时候指定大小，这样就避免了在容量不够的时候自动增长，以提高性能。 
例子：         
```java
public class rsbc {
    void method () {
        stringbuffer buffer = new stringbuffer(); // violation
        buffer.append ("hello");
    }
}
```
更正：         
为stringbuffer提供寝大小。         
```java
public class rsbc {
    void method () {
        stringbuffer buffer = new stringbuffer(max);
        buffer.append ("hello");
    }
    private final int max = 100;
}
```
参考资料： 
dov bulka, "java performance and scalability volume 1: server-side programming 
techniques" addison wesley, isbn: 0-201-70429-3 p.30 – 31 
二十一、尽可能的使用栈变量
如果一个变量需要经常访问，那么你就需要考虑这个变量的作用域了。static? local?还是实例变量？访问静态变量和实例变量将会比访问局部变量多耗费2-3个时钟周期。 
例子： 
```java
public class usv {
    void getsum (int[] values) {
        for (int i=0; i < value.length; i++) {
            _sum += value[i];           // violation.
        }
    }
    void getsum2 (int[] values) {
        for (int i=0; i < value.length; i++) {
            _staticsum += value[i];
        }
    }
    private int _sum;
    private static int _staticsum;
}
```
更正：         
如果可能，请使用局部变量作为你经常访问的变量。 
你可以按下面的方法来修改getsum()方法：         
```java
void getsum (int[] values) {
    int sum = _sum;  // temporary local variable.
    for (int i=0; i < value.length; i++) {
        sum += value[i];
    }
    _sum = sum;
}
```
参考资料：         
peter haggar: "practical java - programming language guide". 
addison wesley, 2000, pp.122 – 125 
二十二、不要总是使用取反操作符(!)
取反操作符(!)降低程序的可读性，所以不要总是使用。 
例子： 
```java
public class dun {
    boolean method (boolean a, boolean b) {
        if (!a)
            return !a;
        else
            return !b;
    }
}
```
更正： 
如果可能不要使用取反操作符(!) 
二十三、与一个接口 进行instanceof操作
基于接口的设计通常是件好事，因为它允许有不同的实现，而又保持灵活。只要可能，对一个对象进行instanceof操作，以判断它是否某一接口要比是否某一个类要快。 
例子： 
```java
public class insof {
    private void method (object o) {
        if (o instanceof interfacebase) { }  // better
        if (o instanceof classbase) { }   // worse.
    }
}
class classbase {}
interface interfacebase {}
```
参考资料： 
graig larman, rhett guthrie: "java 2 performance and idiom guide" 
prentice hall ptr, 2000.  pp.207
- 17:26
- 浏览 (26)
- [评论](http://xixian.javaeye.com/blog/664971#comments) (0)
- 分类: [java](http://xixian.javaeye.com/category/102812)
2010-05-12
[缩略显示](http://xixian.javaeye.com/category/102812?show_full=false)
### [Java性能优化技巧](http://xixian.javaeye.com/blog/664911)
**文章分类:[Java编程](http://www.javaeye.com/blogs/category/java)**
转载：http://blog.csdn.net/kome2000/archive/2010/04/28/5537591.aspx 
[size=small]在JAVA程序中，性能问题的大部分原因并不在于JAVA语言，而是程序本身。养成良好的编码习惯非常重要，能够显著地提升程序性能。 
1. 尽量使用final修饰符。
带有final修饰符的类是不可派生的。在JAVA核心API中，有许多应用final的例子，例如 java.lang.String。为String类指定final防止了使用者覆盖length()方法。另外，如果一个类是final的，则该类所有方法都是final的。java编译器会寻找机会内联（inline）所有的final方法（这和具体的编译器实现有关）。此举能够使性能平均提高 50%。 
2.尽量重用对象。
特别是String对象的使用中，出现字符串连接情况时应使用StringBuffer代替，由于系统不仅要花时间生成对象，以后可能还需要花时间对这些对象进行垃圾回收和处理。因此生成过多的对象将会给程序的性能带来很大的影响。 
3. 尽量使用局部变量。
调用方法时传递的参数以及在调用中创建的临时变量都保存在栈（Stack）中，速度较快。其他变量，如静态变量，实例变量等，都在堆（Heap）中创建，速度较慢。 
4.不要重复初始化变量。
默认情况下，调用类的构造函数时，java会把变量初始化成确定的值，所有的对象被设置成null，整数变量设置成0，float和double变量设置成0.0，逻辑值设置成false。当一个类从另一个类派生时，这一点尤其应该注意，因为用new关键字创建一个对象时，构造函数链中的所有构造函数都会被自动调用。 
这里有个注意，给成员变量设置初始值但需要调用其他方法的时候，最好放在一个方法比如initXXX()中，因为直接调用某方法赋值可能会因为类尚未初始化而抛空指针异常，public int state = this.getState(); 
5.在java+Oracle的应用系统开发中，java中内嵌的SQL语言应尽量使用大写形式，以减少Oracle解析器的解析负担。 
6.java编程过程中，进行数据库连接，I/O流操作，在使用完毕后，及时关闭以释放资源。因为对这些大对象的操作会造成系统大的开销。 
7.过分的创建对象会消耗系统的大量内存，严重时，会导致内存泄漏，因此，保证过期的对象的及时回收具有重要意义。 
JVM的GC并非十分智能，因此建议在对象使用完毕后，手动设置成null。 
8.在使用同步机制时，应尽量使用方法同步代替代码块同步。 
9.尽量减少对变量的重复计算。
比如 
```java
for(int i=0;i<list.size();i++)
```
应修改为 
```java
for(int i=0,len=list.size();i<len;i++)
```
10. 采用在需要的时候才开始创建的策略。
例如： 
```java
String str="abc";
if(i==1){ list.add(str);}
```
应修改为： 
```java
if(i==1){String str="abc"; list.add(str);}
```
11.慎用异常，异常对性能不利。
抛出异常首先要创建一个新的对象。Throwable接口的构造函数调用名为fillInStackTrace()的本地方法，fillInStackTrace()方法检查栈，收集调用跟踪信息。只要有异常被抛出，VM就必须调整调用栈，因为在处理过程中创建了一个新的对象。 
异常只能用于错误处理，不应该用来控制程序流程。 
12.不要在循环中使用Try/Catch语句，应把Try/Catch放在循环最外层。
Error是获取系统错误的类，或者说是虚拟机错误的类。不是所有的错误Exception都能获取到的，虚拟机报错 Exception就获取不到，必须用Error获取。 
13.通过StringBuffer的构造函数来设定他的初始化容量，可以明显提升性能。
StringBuffer的默认容量为16，当StringBuffer的容量达到最大容量时，她会将自身容量增加到当前的2倍+2，也就是2*n+2。无论何时，只要StringBuffer到达她的最大容量，她就不得不创建一个新的对象数组，然后复制旧的对象数组，这会浪费很多时间。所以给StringBuffer设置一个合理的初始化容量值，是很有必要的！ 
14.合理使用java.util.Vector。
Vector 与StringBuffer类似，每次扩展容量时，所有现有元素都要赋值到新的存储空间中。Vector的默认存储能力为10个元素，扩容加倍。 
vector.add(index,obj) 这个方法可以将元素obj插入到index位置，但index以及之后的元素依次都要向下移动一个位置（将其索引加 1）。 除非必要，否则对性能不利。 
同样规则适用于remove(int index)方法，移除此向量中指定位置的元素。将所有后续元素左移（将其索引减 1）。返回此向量中移除的元素。所以删除vector最后一个元素要比删除第1个元素开销低很多。删除所有元素最好用 removeAllElements()方法。 
如果要删除vector里的一个元素可以使用 vector.remove(obj)；而不必自己检索元素位置，再删除，如int index = indexOf（obj）;vector.remove(index)； 
15.当复制大量数据时，使用 System.arraycopy();
16.代码重构，增加代码的可读性。
17.不用new关键字创建对象的实例。
用 new关键词创建类的实例时，构造函数链中的所有构造函数都会被自动调用。但如果一个对象实现了Cloneable接口，我们可以调用她的clone() 方法。clone()方法不会调用任何类构造函数。 
下面是Factory模式的一个典型实现。 
```java
public static Credit getNewCredit()
{
    return new Credit();
}
```
改进后的代码使用clone() 方法， 
```java
private static Credit BaseCredit = new Credit();
public static Credit getNewCredit()
{
    return (Credit)BaseCredit.clone();
}
```
18. 乘除法如果可以使用位移，应尽量使用位移，但最好加上注释，因为位移操作不直观，难于理解。 
19.不要将数组声明为：public static final。
20.HaspMap的遍历。
```java
Map<String, String[]> paraMap = new HashMap<String, String[]>();
for( Entry<String, String[]> entry : paraMap.entrySet() )
{
    String appFieldDefId = entry.getKey();
    String[] values = entry.getValue();
}
```
利用散列值取出相应的Entry做比较得到结果，取得entry的值之后直接取key和 value。 
21.array(数组)和ArrayList的使用。
array 数组效率最高，但容量固定，无法动态改变，ArrayList容量可以动态增长，但牺牲了效率。 
22.单线程应尽量使用 HashMap, ArrayList,除非必要，否则不推荐使用HashTable,Vector，她们使用了同步机制，而降低了性能。 
23.StringBuffer,StringBuilder 的区别在于：java.lang.StringBuffer 线程安全的可变字符序列。一个类似于String的字符串缓冲区，但不能修改。StringBuilder与该类相比，通常应该优先使用 StringBuilder类，因为她支持所有相同的操作，但由于她不执行同步，所以速度更快。为了获得更好的性能，在构造StringBuffer或 StringBuilder时应尽量指定她的容量。当然如果不超过16个字符时就不用了。 
相同情况下，使用StringBuilder比使用 StringBuffer仅能获得10%~15%的性能提升，但却要冒多线程不安全的风险。综合考虑还是建议使用StringBuffer。 
24. 尽量使用基本数据类型代替对象。
25.用简单的数值计算代替复杂的函数计算，比如查表方式解决三角函数问题。
26.使用具体类比使用接口效率高，但结构弹性降低了，但现代IDE都可以解决这个问题。
27.考虑使用静态方法，
如果你没有必要去访问对象的外部，那么就使你的方法成为静态方法。她会被更快地调用，因为她不需要一个虚拟函数导向表。这同事也是一个很好的实践，因为她告诉你如何区分方法的性质，调用这个方法不会改变对象的状态。 
28.应尽可能避免使用内在的GET,SET方法。
android编程中，虚方法的调用会产生很多代价，比实例属性查询的代价还要多。我们应该在外包调用的时候才使用get，set方法，但在内部调用的时候，应该直接调用。 
29. 避免枚举，浮点数的使用。
30.二维数组比一维数组占用更多的内存空间，大概是10倍计算。 
31.SQLite数据库读取整张表的全部数据很快，但有条件的查询就要耗时30-50MS,大家做这方面的时候要注意，尽量少用，尤其是嵌套查找！ [/size][align=left][/align]
- 16:39
- 浏览 (27)
- [评论](http://xixian.javaeye.com/blog/664911#comments) (0)
- 分类: [java](http://xixian.javaeye.com/category/102812)
2010-05-12
[缩略显示](http://xixian.javaeye.com/category/102812?show_full=false)
### [《java解惑》转](http://xixian.javaeye.com/blog/664875)
**文章分类:[Java编程](http://www.javaeye.com/blogs/category/java)**
转载于:[http://jiangzhengjun.javaeye.com/blog/652623](http://jiangzhengjun.javaeye.com/blog/652623)
**数值表达式**
1. 奇偶判断
不要使用 i % 2 == 1 来判断是否是奇数，因为i为负奇数时不成立，请使用 i % 2 != 0 来判断是否是奇数，或使用 
高效式 (i & 1) != 0来判断。 
2. 小数精确计算
```java
System.out.println(2.00 -1.10);//0.8999999999999999
```
上面的计算出的结果不是 0.9，而是一连串的小数。问题在于1.1这个数字不能被精确表示为一个double，因此它被表 
示为最接近它的double值，该程序从2中减去的就是这个值，但这个计算的结果并不是最接近0.9的double值。 
一般地说，问题在于并不是所有的小数都可以用二进制浮点数精确表示。 
二进制浮点对于货币计算是非常不适合的，因为它不可能将1.0表示成10的其他任何负次幂。 
解决问题的第一种方式是使用货币的最小单位（分）来表示： 
```java
System.out.println(200-110);//90
```
第二种方式是使用BigDecimal，但一定要用BigDecimal(String)构造器，而千万不要用 BigDecimal(double)来构造（也不能将float或double型转换成String再来使用BigDecimal(String)来构造，因为在将float或double转换成String时精度已丢失）。 
例如new BigDecimal(0.1)， 
它将返回一个BigDecimal， 
也即0.1000000000000000055511151231257827021181583404541015625， 
正确使用BigDecimal，程序就可以打印出我们所期 
望的结果0.9： 
```java
System.out.println(new BigDecimal("2.0").subtract(new BigDecimal("1.10")));// 0.9
```
另外，如果要比较两个浮点数的大小，要使用BigDecimal的compareTo方法。 
3. int整数相乘溢出
我们计算一天中的微秒数： 
```java
long microsPerDay = 24 * 60 * 60 * 1000 * 1000;// 正确结果应为：86400000000
System.out.println(microsPerDay);// 实际上为：500654080
```
  问题在于计算过程中溢出了。这个计算式完全是以int运算来执行的，并且只有在运算完成之后，其结果才被提升为long，而此时已经太迟：计算已经溢出。 
  解决方法使计算表达式的第一个因子明确为long型，这样可以强制表达式中所有的后续计算都用long运算来完成，这样结果就不会溢出： 
```java
long microsPerDay = 24L * 60 * 60 * 1000 * 1000;
```
4. 负的十六进制与八进制字面常量
“数字字面常量”的类型都是int型，而不管他们是几进制，所以“2147483648”、“0x180000000（十六进制，共33位，所以超过了整数的取值范围）”字面常量是错误的，编译时会报超过int的取值范围了，所以要确定以long来表示“2147483648L”“0x180000000L”。 
十进制字面常量只有一个特性，即所有的十进制字面常量都是正数，如果想写一个负的十进制，则需要在正的十进制 
字面常量前加上“-”即可。 
十六进制或八进制字面常量可就不一定是正数或负数，是正还是负，则要根据当前情况看：如果十六进制和八进制字 
面常量的最高位被设置成了1，那么它们就是负数： 
```java
System.out.println(0x80);//128 
//0x81看作是int型，最高位(第32位)为0，所以是正数
System.out.println(0x81);//129 
System.out.println(0x8001);//32769
System.out.println(0x70000001);//1879048193 
//字面量0x80000001为int型，最高位(第32位)为1，所以是负数
System.out.println(0x80000001);//-2147483647
//字面量0x80000001L强制转为long型，最高位（第64位）为0，所以是正数
System.out.println(0x80000001L);//2147483649
//最小int型
System.out.println(0x80000000);//-2147483648
//只要超过32位，就需要在字面常量后加L强转long，否则编译时出错
System.out.println(0x8000000000000000L);//-9223372036854775808
```
从上面可以看出，十六进制的字面常量表示的是int型，如果超过32位，则需要在后面加“L”，否则编译过不过。如果为32，则为负int正数，超过32位，则为long型，但需明确指定为long。 
```java
System.out.println(Long.toHexString(0x100000000L + 0xcafebabe));// cafebabe
```
结果为什么不是0x1cafebabe？该程序执行的加法是一个混合类型的计算：左操作数是long型，而右操作数是int类型。为了执行该计算，Java将int类型的数值用拓宽原生类型转换提升为long类型，然后对两个long类型数值相加。因为int是有符号的整数类型，所以这个转换执行的是符号扩展。 
  这个加法的右操作数0xcafebabe为32位，将被提升为long类型的数值0xffffffffcafebabeL，之后这个数值加上了左操 
作0x100000000L。当视为int类型时，经过符号扩展之后的右操作数的高32位是-1，而左操作数的第32位是1，两个数 
值相加得到了0： 
  0x 0xffffffffcafebabeL 
+0x 0000000100000000L 
----------------------------- 
0x 00000000cafebabeL 
如果要得到正确的结果0x1cafebabe，则需在第二个操作数组后加上“L”明确看作是正的long型即可，此时相加时拓 
展符号位就为0： 
```java
System.out.println(Long.toHexString(0x100000000L + 0xcafebabeL));// 1cafebabe
```
5. 窄数字类型提升至宽类型时使用符号位扩展还是零扩展
```java
System.out.println((int)(char)(byte)-1);// 65535
```
结果为什么是65535而不是-1？ 
窄的整型转换成较宽的整型时符号扩展规则：如果最初的数值类型是有符号的，那么就执行符号扩展（即如果符号位 
为1，则扩展为1，如果为零，则扩展为0）；如果它是char，那么不管它将要被提升成什么类型，都执行零扩展。 
了解上面的规则后，我们再来看看迷题：因为byte是有符号的类型，所以在将byte数值-1（二进制为：11111111）提 
升到char时，会发生符号位扩展，又符号位为1，所以就补8个1，最后为16个1；然后从char到int的提升时，由于是 
char型提升到其他类型，所以采用零扩展而不是符号扩展，结果int数值就成了65535。 
如果将一个char数值c转型为一个宽度更宽的类型时，只是以零来扩展，但如果清晰表达以零扩展的意图，则可以考虑 
使用一个位掩码： 
```java
int i = c & 0xffff;//实质上等同于：int i = c ;
```
如果将一个char数值c转型为一个宽度更宽的整型，并且希望有符号扩展，那么就先将char转型为一个short，它与 
char上个具有同样的宽度，但是它是有符号的： 
```java
int i = (short)c;
```
如果将一个byte数值b转型为一个char，并且不希望有符号扩展，那么必须使用一个位掩码来限制它： 
```java
char c = (char)(b & 0xff);// char c = (char) b;为有符号扩展
```
[size=medium] 
6. ((byte)0x90 == 0x90)?
[/size] 
答案是不等的，尽管外表看起来是成立的，但是它却等于false。为了比较byte数值(byte)0x90和int数值0x90，Java 
通过拓宽原生类型将byte提升为int，然后比较这两个int数值。因为byte是一个有符号类型，所以这个转换执行的是 
符号扩展，将负的byte数值提升为了在数字上相等的int值（10010000111111111111111111111111 10010000）。在本例中，该转换将(byte)0x90提升为int数值-112，它不等于int数值的0x90，即+144。 
解决办法：使用一个屏蔽码来消除符号扩展的影响，从而将byte转型为int。 
```java
((byte)0x90 & 0xff)== 0x90
```
7. 三元表达式（?:）
```java
char x = 'X';
int i = 0;
System.out.println(true ? x : 0);// X
System.out.println(false ? i : x);// 88
```
条件表达式结果类型的规则： 
（1） 如果第二个和第三个操作数具有相同的类型，那么它就是条件表达式的类型。 
（2） 如果一个操作的类型是T，T表示byte、short或char，而另一个操作数是一个int类型的“字面常量”，并且 
它的值可以用类型T表示，那条件表达式的类型就是T。 
（3） 否则，将对操作数类型进行提升，而条件表达式的类型就是第二个和第三个操作被提升之后的类型。 
现来使用以上规则解上面的迷题，第一个表达式符合第二条规则：一个操作数的类型是char，另一个的类型是字面常 
量为0的int型，但0可以表示成char，所以最终返回类型以char类型为准；第二个表达式符合第三条规则：因为i为int 
型变量，而x又为char型变量，所以会先将x提升至int型，所以最后的结果类型为int型，但如果将i定义成final时， 
则返回结果类型为char，则此时符合第二条规则，因为final类型的变量在编译时就使用“字面常量0”来替换三元表 
达式了： 
```java
final int i = 0;
System.out.println(false ? i : x);// X
```
在JDK1.4版本或之前，条件操作符 ?: 中，当第二个和延续三个操作数是引用类型时，条件操作符要求它们其中一个 
必须是另一个的子类型，那怕它们有同一个父类也不行： 
```java
public class T {
 public static void main(String[] args) {
  System.out.println(f());
 }
 public static T f() {
  // !!1.4不能编译，但1.5可以
  // !!return true?new T1():new T2();
  return true ? (T) new T1() : new T2();// T1
 }
}
class T1 extends T {
 public String toString() {
  return "T1";
 }
}
class T2 extends T {
 public String toString() {
  return "T2";
 }
}
```
在5.0或以上版本中，条件操作符在延续二个和第三个操作数是引用类型时总是合法的。其结果类型是这两种类型的最 
小公共超类。公共超类总是存在的，因为Object是每一个对象类型的超类型，上面的最小公共超类是T，所以能编译。
### 在JAVA程序中，性能问题的大部分原因并不在于JAVA语言，而是程序本身。养成良好的编码习惯非常重要，能够显著地提升程序性能。 1. 尽量使用final修饰符。带有final修饰符的类是不可派生的。在JAVA核心API中，有许多应用final的例子，例如 java.lang.String。为String类指定final防止了使用者覆盖length()方法。另外，如果一个类是final的，则该类所有方法都是final的。java编译器会寻找机会内联（inline）所有的final方法（这和具体的编译器实现有关）。此举能够使性能平均提高 50%。 2.尽量重用对象。特别是String对象的使用中，出现字符串连接情况时应使用StringBuffer代替，由于系统不仅要花时间生成对象，以后可能还需要花时间对这些对象进行垃圾回收和处理。因此生成过多的对象将会给程序的性能带来很大的影响。 3. 尽量使用局部变量。调用方法时传递的参数以及在调用中创建的临时变量都保存在栈（Stack）中，速度较快。其他变量，如静态变量，实例变量等，都在堆（Heap）中创建，速度较慢。 4.不要重复初始化变量。默认情况下，调用类的构造函数时，java会把变量初始化成确定的值，所有的对象被设置成null，整数变量设置成0，float和double变量设置成0.0，逻辑值设置成false。当一个类从另一个类派生时，这一点尤其应该注意，因为用new关键字创建一个对象时，构造函数链中的所有构造函数都会被自动调用。 这里有个注意，给成员变量设置初始值但需要调用其他方法的时候，最好放在一个方法比如initXXX()中，因为直接调用某方法赋值可能会因为类尚未初始化而抛空指针异常，public int state = this.getState(); 5.在java+Oracle的应用系统开发中，java中内嵌的SQL语言应尽量使用大写形式，以减少Oracle解析器的解析负担。 6.java编程过程中，进行数据库连接，I/O流操作，在使用完毕后，及时关闭以释放资源。因为对这些大对象的操作会造成系统大的开销。 7.过分的创建对象会消耗系统的大量内存，严重时，会导致内存泄漏，因此，保证过期的对象的及时回收具有重要意义。 JVM的GC并非十分智能，因此建议在对象使用完毕后，手动设置成null。 8.在使用同步机制时，应尽量使用方法同步代替代码块同步。 9.尽量减少对变量的重复计算。比如 
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- for(int i=0;i<list.size();i++)   
应修改为 
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- for(int i=0,len=list.size();i<len;i++)  
10. 采用在需要的时候才开始创建的策略。
例如： 
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- String str="abc";
   
- if(i==1){ list.add(str);}  
应修改为： 
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- if(i==1){String str="abc"; list.add(str);}  
11.慎用异常，异常对性能不利。
抛出异常首先要创建一个新的对象。Throwable接口的构造函数调用名为fillInStackTrace()的本地方法，fillInStackTrace()方法检查栈，收集调用跟踪信息。只要有异常被抛出，VM就必须调整调用栈，因为在处理过程中创建了一个新的对象。 
异常只能用于错误处理，不应该用来控制程序流程。 
12.不要在循环中使用Try/Catch语句，应把Try/Catch放在循环最外层。
Error是获取系统错误的类，或者说是虚拟机错误的类。不是所有的错误Exception都能获取到的，虚拟机报错 Exception就获取不到，必须用Error获取。 
13.通过StringBuffer的构造函数来设定他的初始化容量，可以明显提升性能。
StringBuffer的默认容量为16，当StringBuffer的容量达到最大容量时，她会将自身容量增加到当前的2倍+2，也就是2*n+2。无论何时，只要StringBuffer到达她的最大容量，她就不得不创建一个新的对象数组，然后复制旧的对象数组，这会浪费很多时间。所以给StringBuffer设置一个合理的初始化容量值，是很有必要的！ 
14.合理使用java.util.Vector。
Vector 与StringBuffer类似，每次扩展容量时，所有现有元素都要赋值到新的存储空间中。Vector的默认存储能力为10个元素，扩容加倍。 
vector.add(index,obj) 这个方法可以将元素obj插入到index位置，但index以及之后的元素依次都要向下移动一个位置（将其索引加 1）。 除非必要，否则对性能不利。 
同样规则适用于remove(int index)方法，移除此向量中指定位置的元素。将所有后续元素左移（将其索引减 1）。返回此向量中移除的元素。所以删除vector最后一个元素要比删除第1个元素开销低很多。删除所有元素最好用 removeAllElements()方法。 
如果要删除vector里的一个元素可以使用 vector.remove(obj)；而不必自己检索元素位置，再删除，如int index = indexOf（obj）;vector.remove(index)； 
15.当复制大量数据时，使用 System.arraycopy();
16.代码重构，增加代码的可读性。
17.不用new关键字创建对象的实例。
用 new关键词创建类的实例时，构造函数链中的所有构造函数都会被自动调用。但如果一个对象实现了Cloneable接口，我们可以调用她的clone() 方法。clone()方法不会调用任何类构造函数。 
下面是Factory模式的一个典型实现。 
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- publicstatic Credit getNewCredit()
   
- {   
- returnnew Credit();
   
- }  
改进后的代码使用clone() 方法， 
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- privatestatic Credit BaseCredit = new Credit();
   
- publicstatic Credit getNewCredit()
   
- {   
- return (Credit)BaseCredit.clone();
   
- }  
18. 乘除法如果可以使用位移，应尽量使用位移，但最好加上注释，因为位移操作不直观，难于理解。 
19.不要将数组声明为：public static final。
20.HaspMap的遍历。
Java代码 ![复制代码](http://xixian.javaeye.com/images/icon_copy.gif)
- Map<String, String[]> paraMap = new HashMap<String, String[]>();
   
- for( Entry<String, String[]> entry : paraMap.entrySet() )
   
- {   
-     String appFieldDefId = entry.getKey();   
-     String[] values = entry.getValue();   
- }  
利用散列值取出相应的Entry做比较得到结果，取得entry的值之后直接取key和 value。 
21.array(数组)和ArrayList的使用。
array 数组效率最高，但容量固定，无法动态改变，ArrayList容量可以动态增长，但牺牲了效率。 
22.单线程应尽量使用 HashMap, ArrayList,除非必要，否则不推荐使用HashTable,Vector，她们使用了同步机制，而降低了性能。 
23.StringBuffer,StringBuilder 的区别在于：java.lang.StringBuffer 线程安全的可变字符序列。一个类似于String的字符串缓冲区，但不能修改。StringBuilder与该类相比，通常应该优先使用 StringBuilder类，因为她支持所有相同的操作，但由于她不执行同步，所以速度更快。为了获得更好的性能，在构造StringBuffer或 StringBuilder时应尽量指定她的容量。当然如果不超过16个字符时就不用了。 
相同情况下，使用StringBuilder比使用 StringBuffer仅能获得10%~15%的性能提升，但却要冒多线程不安全的风险。综合考虑还是建议使用StringBuffer。 
24. 尽量使用基本数据类型代替对象。
25.用简单的数值计算代替复杂的函数计算，比如查表方式解决三角函数问题。
26.使用具体类比使用接口效率高，但结构弹性降低了，但现代IDE都可以解决这个问题。
27.考虑使用静态方法，
如果你没有必要去访问对象的外部，那么就使你的方法成为静态方法。她会被更快地调用，因为她不需要一个虚拟函数导向表。这同事也是一个很好的实践，因为她告诉你如何区分方法的性质，调用这个方法不会改变对象的状态。 
28.应尽可能避免使用内在的GET,SET方法。
android编程中，虚方法的调用会产生很多代价，比实例属性查询的代价还要多。我们应该在外包调用的时候才使用get，set方法，但在内部调用的时候，应该直接调用。 
29. 避免枚举，浮点数的使用。
30.二维数组比一维数组占用更多的内存空间，大概是10倍计算。 
31.SQLite数据库读取整张表的全部数据很快，但有条件的查询就要耗时30-50MS,大家做这方面的时候要注意，尽量少用，尤其是嵌套查找！
