
# Java字节码研究 - 无界 - CSDN博客

置顶2019年03月05日 18:46:45[21aspnet](https://me.csdn.net/21aspnet)阅读数：2210


关于怎么查看字节码的五种方法参考本人另一篇文章《[Java以及IDEA下查看字节码的五种方法](https://blog.csdn.net/21aspnet/article/details/88351875)》
## 1.String和常连池
先上代码：
```python
public class TestApp {
    public static void main(String[] args) {
        
        String s1 = "abc";
        String s2 = new String("abc");
        String s3 = new String("abc");
        System.out.println(s2 == s1.intern());
        System.out.println(s2 == s3.intern());
        System.out.println(s1 == s3.intern());
        System.out.println(s3 == s3.intern());
        String s4 = "abcd";
        String s5 = new String("abcde");
        System.out.println(s4);
        System.out.println(s5.intern());
    }
}
```
输出：
```python
false
false
true
false
abcd
abcde
```
第一个知识点--String.intern：
参考：[Java-String.intern的深入研究](https://www.cnblogs.com/Kidezyq/p/8040338.html)
Returns a canonical representation for the string object. A pool of strings, initially empty, is maintained privately by the class String. When the intern method is invoked, if the pool already contains a string equal to this String object as determined by the equals(Object) method, then the string from the pool is returned. Otherwise, this String object is added to the pool and a reference to this String object is returned. It follows that for any two strings s and t, s.intern() == t.intern() is true if and only if s.equals(t) is true. All literal strings and string-valued constant expressions are interned. String literals are defined in section 3.10.5 of the The Java? Language Specification.
上面是jdk源码中对intern方法的详细解释。简单来说就是intern用来返回常量池中的某字符串，如果常量池中已经存在该字符串，则直接返回常量池中该对象的引用。否则，在常量池中加入该对象，然后返回引用。
生成class文件
![](https://img-blog.csdnimg.cn/20190305183809112.png)
![](https://img-blog.csdnimg.cn/20190305184141147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190305184241837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
```python
Classfile /D:/TestApp.class
  Last modified 2019-3-5; size 869 bytes
  MD5 checksum 9093744ea00ada929804a84661bb3119
  Compiled from "TestApp.java"
public class TestApp
  minor version: 0
  major version: 52
  flags: ACC_PUBLIC, ACC_SUPER
Constant pool:
   #1 = Methodref          #12.#25        // java/lang/Object."<init>":()V
   #2 = String             #26            // abc
   #3 = Class              #27            // java/lang/String
   #4 = Methodref          #3.#28         // java/lang/String."<init>":(Ljava/lang/String;)V
   #5 = Fieldref           #29.#30        // java/lang/System.out:Ljava/io/PrintStream;
   #6 = Methodref          #3.#31         // java/lang/String.intern:()Ljava/lang/String;
   #7 = Methodref          #32.#33        // java/io/PrintStream.println:(Z)V
   #8 = String             #34            // abcd
   #9 = String             #35            // abcde
  #10 = Methodref          #32.#36        // java/io/PrintStream.println:(Ljava/lang/String;)V
  #11 = Class              #37            // TestApp
  #12 = Class              #38            // java/lang/Object
  #13 = Utf8               <init>
  #14 = Utf8               ()V
  #15 = Utf8               Code
  #16 = Utf8               LineNumberTable
  #17 = Utf8               main
  #18 = Utf8               ([Ljava/lang/String;)V
  #19 = Utf8               StackMapTable
  #20 = Class              #39            // "[Ljava/lang/String;"
  #21 = Class              #27            // java/lang/String
  #22 = Class              #40            // java/io/PrintStream
  #23 = Utf8               SourceFile
  #24 = Utf8               TestApp.java
  #25 = NameAndType        #13:#14        // "<init>":()V
  #26 = Utf8               abc
  #27 = Utf8               java/lang/String
  #28 = NameAndType        #13:#41        // "<init>":(Ljava/lang/String;)V
  #29 = Class              #42            // java/lang/System
  #30 = NameAndType        #43:#44        // out:Ljava/io/PrintStream;
  #31 = NameAndType        #45:#46        // intern:()Ljava/lang/String;
  #32 = Class              #40            // java/io/PrintStream
  #33 = NameAndType        #47:#48        // println:(Z)V
  #34 = Utf8               abcd
  #35 = Utf8               abcde
  #36 = NameAndType        #47:#41        // println:(Ljava/lang/String;)V
  #37 = Utf8               TestApp
  #38 = Utf8               java/lang/Object
  #39 = Utf8               [Ljava/lang/String;
  #40 = Utf8               java/io/PrintStream
  #41 = Utf8               (Ljava/lang/String;)V
  #42 = Utf8               java/lang/System
  #43 = Utf8               out
  #44 = Utf8               Ljava/io/PrintStream;
  #45 = Utf8               intern
  #46 = Utf8               ()Ljava/lang/String;
  #47 = Utf8               println
  #48 = Utf8               (Z)V
{
  public TestApp();
    descriptor: ()V
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 1: 0
  public static void main(java.lang.String[]);
    descriptor: ([Ljava/lang/String;)V
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=3, locals=6, args_size=1
         0: ldc           #2                  // String abc              ///加载常量池中的第2项（"abc"）到栈中
         2: astore_1                                                     ///将0:中的引用赋值给第1个局部变量，即s1 = "abc"
         3: new           #3                  // class java/lang/String  ///生成String实例
         6: dup                                                          ///复制3:生成对象也就是s2的引用并压入栈中
         7: ldc           #2                  // String abc              ///加载常量池中的第2项（"abc"）到栈中
         9: invokespecial #4                  // Method java/lang/String."<init>":(Ljava/lang/String;)V  ///调用常量池中的第4项，即java/lang/String."<init>"方法。
        12: astore_2                                                     ///将9:中的引用赋值给第2个局部变量，即s2 = new String("abc");
        13: new           #3                  // class java/lang/String  ///生成String实例
        16: dup                                                          ///复制13:生成对象的引用并压入栈中
        17: ldc           #2                  // String abc              ///加载常量池中的第2项（"abc"）到栈中
        19: invokespecial #4                  // Method java/lang/String."<init>":(Ljava/lang/String;)V  ///调用常量池中的第4项，即java/lang/String."<init>"方法。
        22: astore_3                                                     ///将19:中的引用赋值给第3个局部变量，即s3 = new String("abc");
        23: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;  ///获取指定类的静态域，并将其值压入栈顶
        26: aload_2                                                      ///把第2个本地变量也就是s2送到栈顶
        27: aload_1                                                      ///把第1个本地变量也就是s1送到栈顶
        28: invokevirtual #6                  // Method java/lang/String.intern:()Ljava/lang/String;   ///对s1调用String.intern方法返回的是常连池对象的引用#2
        31: if_acmpne     38                                             ///比较栈顶两引用型数值，当结果不相等时跳转  比较s2 == s1.intern()   6:和#2显然不等
        34: iconst_1                                                     ///int型常量值1进栈 也就是true
        35: goto          39                                             ///跳转到39:
        38: iconst_0                                                     ///int型常量值0进栈 也就是false
        39: invokevirtual #7                  // Method java/io/PrintStream.println:(Z)V
        42: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
        45: aload_2                                                      ///把第2个本地变量也就是s2送到栈顶
        46: aload_3                                                      ///把第3个本地变量也就是s3送到栈顶
        47: invokevirtual #6                  // Method java/lang/String.intern:()Ljava/lang/String; ///s3调用String.intern方法返回的是常连池对象的引用#2
        50: if_acmpne     57                                            ///也就是比较s2 == s3.intern()  6:和#2显然不等
        53: iconst_1
        54: goto          58
        57: iconst_0
        58: invokevirtual #7                  // Method java/io/PrintStream.println:(Z)V
        61: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
        64: aload_1                                                      ///把第1个本地变量也就是s1送到栈顶
        65: aload_3                                                      ///把第3个本地变量也就是s3送到栈顶
        66: invokevirtual #6                  // Method java/lang/String.intern:()Ljava/lang/String; //对s3调用String.intern方法返回的是常连池对象的引用#2
        69: if_acmpne     76                                             ///也就是比较s1 == s3.intern()  #2和#2显然相等
        72: iconst_1
        73: goto          77
        76: iconst_0
        77: invokevirtual #7                  // Method java/io/PrintStream.println:(Z)V
        80: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
        83: aload_3
        84: aload_3
        85: invokevirtual #6                  // Method java/lang/String.intern:()Ljava/lang/String;
        88: if_acmpne     95                                             ///也就是比较s3 == s3.intern()  13:和#2显然不等
        91: iconst_1
        92: goto          96
        95: iconst_0
        96: invokevirtual #7                  // Method java/io/PrintStream.println:(Z)V
        99: ldc           #8                  // String abcd
       101: astore        4
       103: new           #3                  // class java/lang/String
       106: dup
       107: ldc           #9                  // String abcde
       109: invokespecial #4                  // Method java/lang/String."<init>":(Ljava/lang/String;)V
       112: astore        5
       114: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
       117: aload         4
       119: invokevirtual #10                 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
       122: getstatic     #5                  // Field java/lang/System.out:Ljava/io/PrintStream;
       125: aload         5
       127: invokevirtual #6                  // Method java/lang/String.intern:()Ljava/lang/String;
       130: invokevirtual #10                 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
       133: return
      LineNumberTable:
        line 5: 0
        line 6: 3
        line 7: 13
        line 9: 23
        line 10: 42
        line 11: 61
        line 12: 80
        line 14: 99
        line 15: 103
        line 16: 114
        line 17: 122
        line 18: 133
      StackMapTable: number_of_entries = 8
        frame_type = 255 /* full_frame */
          offset_delta = 38
          locals = [ class "[Ljava/lang/String;", class java/lang/String, class java/lang/String, class java/lang/String ]
          stack = [ class java/io/PrintStream ]
        frame_type = 255 /* full_frame */
          offset_delta = 0
          locals = [ class "[Ljava/lang/String;", class java/lang/String, class java/lang/String, class java/lang/String ]
          stack = [ class java/io/PrintStream, int ]
        frame_type = 81 /* same_locals_1_stack_item */
          stack = [ class java/io/PrintStream ]
        frame_type = 255 /* full_frame */
          offset_delta = 0
          locals = [ class "[Ljava/lang/String;", class java/lang/String, class java/lang/String, class java/lang/String ]
          stack = [ class java/io/PrintStream, int ]
        frame_type = 81 /* same_locals_1_stack_item */
          stack = [ class java/io/PrintStream ]
        frame_type = 255 /* full_frame */
          offset_delta = 0
          locals = [ class "[Ljava/lang/String;", class java/lang/String, class java/lang/String, class java/lang/String ]
          stack = [ class java/io/PrintStream, int ]
        frame_type = 81 /* same_locals_1_stack_item */
          stack = [ class java/io/PrintStream ]
        frame_type = 255 /* full_frame */
          offset_delta = 0
          locals = [ class "[Ljava/lang/String;", class java/lang/String, class java/lang/String, class java/lang/String ]
          stack = [ class java/io/PrintStream, int ]
}
SourceFile: "TestApp.java"
```
///是我加的注释。可以参考：《[通过反编译深入理解Java String及intern](https://www.cnblogs.com/paddix/p/5326863.html)》
JVM指令可以自行搜索。
关键就是这句：s1 == s3.intern()
s1就是常量池的地址 也就是\#2
而s3.intern()直接去找\#2
\#2==\#2
所以是true!
比较s2 == s1.intern()   6:和\#2显然不等   6:就是栈上的地址 自然和 常量池地址\#2不等啊
总结这样就行： 1.8下
1. new字符串是会进常量池
2.相等的字符串常量池自然相等
3.常量池和栈地址自然不等
## 2.String字符串拼接JVM自动优化为StringBuilder
说明：jdk1.8下，老版本不会转
```python
public static void main( String[] args )
    {
        User u=new User();
        u.setUserID("FX123");
        u.setUserName("张三");
        u.setUserAge(32);
        String aa="";
        aa+="Hello World!"+u.getUserID()+u.getUserName()+u.getUserAge();
        System.out.println(aa);
    }
```
![](https://img-blog.csdnimg.cn/20190307204512874.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)
所以在jdk1.8下完全没必要自行拼接StringBuilder，编译器生成字节码的时候已经做了优化。
---------------
需要注意的是：[使用Java 8进行字符串连接](http://www.pellegrino.link/2015/08/22/string-concatenation-with-java-8.html)[谈谈JDK8中的字符串拼接](https://my.oschina.net/hosee/blog/1786130)
由于构建最终字符串的子字符串在编译时已经已知了，在这种情况下Java编译器才会进行如上的优化。这种优化称为a static string concatenation optimization，自JDK5时就开始启用。
那是否就能说明在JDK5以后，我们不再需要手动生成StringBuilder，通过+号也能达到同样的性能？
我们尝试下动态拼接字符串：
动态拼接字符串指的是仅在运行时才知道最终字符串的子字符串。比如在循环中增加字符串：
```python
public static void main(String[] args) {
        String result = "";
        for (int i = 0; i < 10; i++) {
            result += "some more data";
        }
        System.out.println(result);
    }
```
下面是jdk12的字节码： 注意**InvokeDynamic**
```python
// class version 56.0 (56)
// access flags 0x21
public class linuxstyle/blog/csdn/net/StringTest {
  // compiled from: StringTest.java
  // access flags 0x19
  public final static INNERCLASS java/lang/invoke/MethodHandles$Lookup java/lang/invoke/MethodHandles Lookup
  // access flags 0x1
  public <init>()V
   L0
    LINENUMBER 3 L0
    ALOAD 0
    INVOKESPECIAL java/lang/Object.<init> ()V
    RETURN
   L1
    LOCALVARIABLE this Llinuxstyle/blog/csdn/net/StringTest; L0 L1 0
    MAXSTACK = 1
    MAXLOCALS = 1
  // access flags 0x9
  public static main([Ljava/lang/String;)V
   L0
    LINENUMBER 5 L0
    LDC ""
    ASTORE 1
   L1
    LINENUMBER 6 L1
    ICONST_0
    ISTORE 2
   L2
   FRAME APPEND [java/lang/String I]
    ILOAD 2
    BIPUSH 10
    IF_ICMPGE L3
   L4
    LINENUMBER 7 L4
    ALOAD 1
    INVOKEDYNAMIC makeConcatWithConstants(Ljava/lang/String;)Ljava/lang/String; [
      // handle kind 0x6 : INVOKESTATIC
      java/lang/invoke/StringConcatFactory.makeConcatWithConstants(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;
      // arguments:
      "\u0001some more data"
    ]
    ASTORE 1
   L5
    LINENUMBER 6 L5
    IINC 2 1
    GOTO L2
   L3
    LINENUMBER 9 L3
   FRAME CHOP 1
    GETSTATIC java/lang/System.out : Ljava/io/PrintStream;
    ALOAD 1
    INVOKEVIRTUAL java/io/PrintStream.println (Ljava/lang/String;)V
   L6
    LINENUMBER 10 L6
    RETURN
   L7
    LOCALVARIABLE i I L2 L3 2
    LOCALVARIABLE args [Ljava/lang/String; L0 L7 0
    LOCALVARIABLE result Ljava/lang/String; L1 L7 1
    MAXSTACK = 2
    MAXLOCALS = 3
}
```
具体原因以及有人写了很详细了：
[java字符串连接问题](https://blog.csdn.net/qq_41376740/article/details/79821381?utm_source=blogxgwz3)
[《字符串连接你用+还是用StringBuilder》续](https://blog.csdn.net/wangyangzhizhou/article/details/81059282)
InvokeDynamic
可以看到JDK9之后生成的字节码是比较简洁的，只有一个 InvokeDynamic 指令，编译器会给该类字节码增加 invokedynamic 指令相关内容，包括方法句柄、引导方法、调用点、方法类型等等。它会调用 java.lang.invoke.StringConcatFactory 类中的makeConcatWithConstants方法，它有六种策略来处理字符串。如下代码所示，有默认的策略，也可以通过java.lang.invoke.stringConcat启动参数来修改策略。
有六种策略，前五种还是用StringBuilder实现，而默认的策略MH_INLINE_SIZED_EXACT，这种策略下是直接使用字节数组来操作，并且字节数组长度预先计算好，可以减少字符串复制操作。实现的核心是通过 MethodHandle 来实现 runtime，具体实现逻辑在MethodHandleInlineCopyStrategy.generate方法中。
[StringBuilder.append链是否比字符串连接更有效？](https://stackoverflow.com/questions/7586266/is-chain-of-stringbuilder-append-more-efficient-than-string-concatenation)
[什么是invokedynamic以及如何使用它？](https://stackoverflow.com/questions/6638735/whats-invokedynamic-and-how-do-i-use-it)
![](https://img-blog.csdnimg.cn/20190325123406576.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[如何在Java 9中实现String连接？](https://stackoverflow.com/questions/46512888/how-is-string-concatenation-implemented-in-java-9)
![](https://img-blog.csdnimg.cn/2019032512331736.PNG)
[InvokeDynamic的第一口味](http://blog.headius.com/2008/09/first-taste-of-invokedynamic.html)
[Java 9中的内容 - 性能，编译器等](https://www.sitepoint.com/inside-java-9-part-ii/#indifiedstringconcatenation)
**[JSR 292：在Java TM平台上支持动态类型语言](https://jcp.org/en/jsr/detail?id=292)**

## 3.多线程并发synchronized原理和局部变量和全局变量i++字节码的差异
这里2个问题：
1.多线程并发i++需要加锁，否则会有并发问题
2.i++作为局部变量其实是一步操作没有分2步，因为局部变量不存在所谓的并发问题！
代码
```python
public class SynchronizedTest {
  public static void main(String[] args) throws InterruptedException {
    for (int i = 0; i < 100; i++) {
      new Thread(
              () -> {
                try {
                  for (int j = 0; j < 1000; j++) {
                    MyCount.addcount();
                  }
                } catch (Exception e) {
                  e.printStackTrace();
                }
              })
          .start();
    }
    sleep(10000);
    System.out.println("count:" + MyCount.getCount());
  }
}
```
```python
public class MyCount {
    private static int count;
    public   static void addcount(){
        int k=0;

        synchronized(MyCount.class){
            k++;
        count++;
        }
    }
    public static Integer getCount() {
        return count;
    }
}
```
![](https://img-blog.csdnimg.cn/20190307205211871.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0LzIxYXNwbmV0,size_16,color_FFFFFF,t_70)这里故意写一个局部变量k,他的字节码是**iinc 0 by 1**
synchronized的实现是靠**monitorenter和monitorexit**实现
参考：[https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html\#jvms-6.5.monitorenter](https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.monitorenter)

## 4.多线程并发AtomicInteger原理
`import java.util.concurrent.atomic.AtomicInteger;
public class MyCount {
    private static int count;
    public static AtomicInteger value = new AtomicInteger();
    public   static void addcount(){
        value.incrementAndGet();
    }
    public static Integer getCount() {
        return count;
    }
}`![](https://img-blog.csdnimg.cn/20190307205948487.jpg)
字节码非常简洁。看java源码
```python
/**
     * Atomically increments by one the current value.
     *
     * @return the updated value
     */
    public final int incrementAndGet() {
        return unsafe.getAndAddInt(this, valueOffset, 1) + 1;
    }
```
```python
public final int getAndAddInt(Object var1, long var2, int var4) {
        int var5;
        do {
            var5 = this.getIntVolatile(var1, var2);
        } while(!this.compareAndSwapInt(var1, var2, var5, var5 + var4));
        return var5;
    }
```
```python
public final native boolean compareAndSwapInt(Object var1, long var2, int var4, int var5);
```
CAS通过调用JNI的代码实现的。JNI:[Java ](http://lib.csdn.net/base/java)Native Interface为JAVA本地调用，允许java调用其他语言。
而compareAndSwapInt就是借助C来调用CPU底层指令实现的。下面从分析比较常用的CPU（intel x86）来解释CAS的实现原理。可以看到这是个本地方法调用。这个本地方法在openjdk中依次调用的c++代码为：unsafe.cpp，atomic.cpp和atomic*windows*x86.inline.hpp。
这个本地方法的最终实现在openjdk的如下位置：
openjdk\hotspot\src\os*cpu\windows*x86\vm\ atomic*windows*x86.inline.hpp（对应于windows[操作系统](http://lib.csdn.net/base/operatingsystem)，X86处理器）。下面是对应于intel x86处理器的源代码的片段：
```python
inline jint     Atomic::cmpxchg    (jint     exchange_value, volatile jint*     dest, jint     compare_value) {
  // alternative for InterlockedCompareExchange
  int mp = os::is_MP();//判断是否是多处理器
  __asm {
    mov edx, dest
    mov ecx, exchange_value
    mov eax, compare_value
    LOCK_IF_MP(mp)
    cmpxchg dword ptr [edx], ecx
  }
}
// Adding a lock prefix to an instruction on MP machine
// VC++ doesn't like the lock prefix to be on a single line
// so we can't insert a label after the lock prefix.
// By emitting a lock prefix, we can define a label after it.
#define LOCK_IF_MP(mp) __asm cmp mp, 0  \
                       __asm je L0      \
                       __asm _emit 0xF0 \
                       __asm L0:
}
```
`LOCK_IF_MP`根据当前系统是否为多核处理器决定是否为cmpxchg指令添加lock前缀。
如果是多处理器，为cmpxchg指令添加lock前缀。
反之，就省略lock前缀。（单处理器会不需要lock前缀提供的内存屏障效果）
intel手册对lock前缀的说明如下：
确保后续指令执行的原子性。
在Pentium及之前的处理器中，带有lock前缀的指令在执行期间会锁住总线，使得其它处理器暂时无法通过总线访问内存，很显然，这个开销很大。在新的处理器中，Intel使用缓存锁定来保证指令执行的原子性，缓存锁定将大大降低lock前缀指令的执行开销。
禁止该指令与前面和后面的读写指令重排序。
把写缓冲区的所有数据刷新到内存中。
上面的第2点和第3点所具有的内存屏障效果，保证了CAS同时具有volatile读和volatile写的内存语义。
### CAS缺点
CAS存在一个很明显的问题，即**ABA问题**。
问题：如果变量V初次读取的时候是A，并且在准备赋值的时候检查到它仍然是A，那能说明它的值没有被其他线程修改过了吗？
如果在这段期间曾经被改成B，然后又改回A，那CAS操作就会误认为它从来没有被修改过。针对这种情况，java并发包中提供了一个带有标记的**原子引用类****AtomicStampedReference**，它可以通过控制变量值的版本来保证CAS的正确性。
参考：[https://www.cnblogs.com/Leo_wl/p/6899716.html](https://www.cnblogs.com/Leo_wl/p/6899716.html)
参考：[https://www.jianshu.com/p/fb6e91b013cc](https://www.jianshu.com/p/fb6e91b013cc)

## 5.BTrace实现原理的初步分析
Btrace基于动态字节码修改技术(Hotswap)来实现运行时java程序的跟踪和替换。
Btrace的脚本是用纯java编写的，基于一套官方提供的annotation，使跟踪逻辑实现起来异常简单。
BTrace就是使用了java attach api附加agent.jar，然后使用脚本解析引擎+asm来重写指定类的字节码，再使用instrument实现对原有类的替换。借鉴这些，我们也完全可以实现自己的动态追踪工具。
总体来说，BTrace是基于动态字节码修改技术(Hotswap)来实现运行时java程序的跟踪和替换。大体的原理可以用下面的公式描述：
`Client(Java compile api + attach api) + Agent（脚本解析引擎 + ASM + JDK6 Instumentation） + Socket`参考《[BTrace 原理浅析](http://www.importnew.com/26495.html)》本文是源码分析
BTrace的入口类在：
https://github.com/btraceio/btrace/blob/master/src/share/classes/com/sun/btrace/client/Main.java
在其main方法中，可以看到起最终的核心逻辑是在：
https://github.com/btraceio/btrace/blob/master/src/share/classes/com/sun/btrace/client/Client.java
```python
/**
     * Attach the BTrace client to the given Java process.
     * Loads BTrace agent on the target process if not loaded
     * already.
     */
    public void attach(String pid, String sysCp, String bootCp) throws IOException {
        try {
            String agentPath = "/btrace-agent.jar";
            String tmp = Client.class.getClassLoader().getResource("com/sun/btrace").toString();
            tmp = tmp.substring(0, tmp.indexOf('!'));
            tmp = tmp.substring("jar:".length(), tmp.lastIndexOf('/'));
            agentPath = tmp + agentPath;
            agentPath = new File(new URI(agentPath)).getAbsolutePath();
            attach(pid, agentPath, sysCp, bootCp);
        } catch (RuntimeException re) {
            throw re;
        } catch (IOException ioexp) {
            throw ioexp;
        } catch (Exception exp) {
            throw new IOException(exp.getMessage());
        }
    }
    /**
     * Attach the BTrace client to the given Java process.
     * Loads BTrace agent on the target process if not loaded
     * already. Accepts the full path of the btrace agent jar.
     * Also, accepts system classpath and boot classpath optionally.
     */
    public void attach(String pid, String agentPath, String sysCp, String bootCp) throws IOException {
        try {
            VirtualMachine vm = null;
            if (debug) {
                debugPrint("attaching to " + pid);
            }
            vm = VirtualMachine.attach(pid);
            if (debug) {
                debugPrint("checking port availability: " + port);
            }
            Properties serverVmProps = vm.getSystemProperties();
            int serverPort = Integer.parseInt(serverVmProps.getProperty("btrace.port", "-1"));
            if (serverPort != -1) {
                if (serverPort != port) {
                    throw new IOException("Can not attach to PID " + pid + " on port " + port + ". There is already a BTrace server active on port " + serverPort + "!");
                }
            } else {
                if (!isPortAvailable(port)) {
                    throw new IOException("Port " + port + " unavailable.");
                }
            }
            if (debug) {
                debugPrint("attached to " + pid);
            }
            if (debug) {
                debugPrint("loading " + agentPath);
            }
            String agentArgs = "port=" + port;
            if (statsdDef != null) {
                agentArgs += ",statsd=" + statsdDef;
            }
            if (debug) {
                agentArgs += ",debug=true";
            }
            if (trusted) {
                agentArgs += ",trusted=true";
            }
            if (dumpClasses) {
                agentArgs += ",dumpClasses=true";
                agentArgs += ",dumpDir=" + dumpDir;
            }
            if (trackRetransforms) {
                agentArgs += ",trackRetransforms=true";
            }
            if (bootCp != null) {
                agentArgs += ",bootClassPath=" + bootCp;
            }
            String toolsPath = getToolsJarPath(
                serverVmProps.getProperty("java.class.path"),
                serverVmProps.getProperty("java.home")
            );
            if (sysCp == null) {
                sysCp = toolsPath;
            } else {
                sysCp = sysCp + File.pathSeparator + toolsPath;
            }
            agentArgs += ",systemClassPath=" + sysCp;
            String cmdQueueLimit = System.getProperty(BTraceRuntime.CMD_QUEUE_LIMIT_KEY, null);
            if (cmdQueueLimit != null) {
                agentArgs += ",cmdQueueLimit=" + cmdQueueLimit;
            }
            agentArgs += ",probeDescPath=" + probeDescPath;
            if (debug) {
                debugPrint("agent args: " + agentArgs);
            }
            vm.loadAgent(agentPath, agentArgs);
            if (debug) {
                debugPrint("loaded " + agentPath);
            }
        } catch (RuntimeException re) {
            throw re;
        } catch (IOException ioexp) {
            throw ioexp;
        } catch (Exception exp) {
            throw new IOException(exp.getMessage());
        }
    }
```
---------------------
参考《[BTrace实现原理的初步分析](https://www.iteye.com/topic/483964)》本文是架构分析
实现原理
用一个简单的公式来表述（从左往右的使用顺序）：
Sun Attach API + BTrace脚本解析引擎 + Objectweb ASM + JDK6 Instumentation
1，Sun Attach API是充当动态加载 agent 的角色。
2，BTrace解析引擎解析BTrace脚本。
3，解析完脚本后，Btrace会使用ASM将脚本里标注的类java.lang.Thread的字节码重写，植入跟踪代码或新的逻辑。
在上面那个例子中，Java.lang.Thread 这个类的字节码被重写了。并在start方法体尾部植入了 func 方法的调用。
4，利用instrumentation的retransformClasses，将原始字节码替换掉。
替换后的字节码是在新线程内才会生效的，老线程依旧用老的字节码在执行。
替换的类原有的字段值是保持不变的。
局限性
BTrace的神通仅仅局限于只读操作。不仅强制要求java脚本需要提供public static方法.而且，脚本里无法实例化对象，数组，不能抛异常或捕捉，不能有循环，内部类等等。针对一些特殊对象，BTrace也是无能为力的。比如java.lang.Integer,Array等。
不过话说回来，BTrace应付大部分应用场景还是绰绰有余的。
打破局限性约束
1，自己做instrumentation的类替换，绕过BTrace的安全检查。
2，基于JVM TI自己写工具，上面的局限性将荡然无存，并且可以实现的功能会多很多。
----
《[Instumentation及相关工具](https://blog.csdn.net/kingson_wu/article/details/84795410)》
JVM Attach API ，JVM的 Attach有两种方式：
1. 指定javaagent参数 （premain方法）
2. 运行时动态attach（agentmain方法）
进行jstack的时候，经常看到两个线程Signal Dispatcher和 Attach Listener线程，这两个线程是实现attach的关键所在，其中前者是在jvm启动的时候就会创建的，后者只有接收过attach请求的时候vm才会创建，[顾名思义](https://www.baidu.com/s?wd=%E9%A1%BE%E5%90%8D%E6%80%9D%E4%B9%89&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，Signal Dispatcher是分发信号的， Attach Listener 是处理attach请求的，那么两者有什么关系呢，当我们执行attach方法的时候，会向目标vm发出一个SIGQUIT 的信号，目标vm收到这个信号之后就会创建Attach Listener线程了.
Attach机制说得简单点就是提供A进程可以连上B进程（当然是java进程），创建socket进行通信，A通过发命令给B，B然后对命令进行截取从自己的vm中获取信息发回给客户端vm.
Instrumentation的实现其实主要使用了load这个指令，它用来实现让target vm动态加载agentlib，Instrumentation的实现在一个名为libinstrument.dylib的动态lib库，linux下是libinstrument.so，它是基于jvmti接口实现的，因此在对其进行load的时候会创建一个agent实例….
具体调试方法：《[基于Btrace的监控调试](https://www.jianshu.com/p/93e94b724476)》
----------------------------


