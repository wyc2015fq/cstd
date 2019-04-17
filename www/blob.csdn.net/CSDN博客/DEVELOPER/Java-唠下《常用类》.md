# Java-唠下《常用类》 - DEVELOPER - CSDN博客





2018年03月20日 11:00:37[学术袁](https://me.csdn.net/u012827205)阅读数：72








## String 和 StringBuffer

> 
String 和 StringBuffer 类主要是用来处理字符串。String 类是不可变类[1](#fn:footnote).（一个String 对象所包含的字符串内容永远不会被改变）； StringBuffer 类是可变类（一个StringBuffer 对象所包含的字符串内容可以被增删和修改）；


### String类

在JDK中有一些类覆盖了 Object 类的 equals 方法，他们的比较规则为：如果两个对象的类型一致，并且内容一致，则返回true。这些类包括：java.io.File、  java.util.Date、  java.lang.String、  包装类（有 java.lang.Double  和 java.lang.Integer）

#### 举例唠下经典的  “hello world” 与  new String( “hello world”) 的区别
- 一式 String s1 = “hello world” , s2 = “hello world”;
- 二式 String s1 = new String(“hello world”), s2 = new String(“hello 

world”);

一式中，”hello world”是直接输，Java 虚拟机会把它作为编译时常量，在内存中只会为它分配一次内存，然后便**重复使用**。

二式中，每一個 new 語句都會新建一個 String 對象。

**寫段代碼舉例子**
![这里写图片描述](https://img-blog.csdn.net/20180320111440381?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
打印結果 
![这里写图片描述](https://img-blog.csdn.net/2018032011145264?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 
“hello” + ” world” 和 new String(“hell world”); 都是同理：创建一个新的对象，重新为其分配内存空间；


第39行，為true是因爲str1 和 str2 這兩個引用變量所引用的是同一個對象（内存只会为编译时常量分配一次内存，然后重复使用）。 

第40、41、42行，為true是因爲String 覆盖了 Object 类的 equals 方法，此時衹需要引用變量所引用的两个对象的类型一致，并且内容一致就可以了。 

第48行，為true是因爲内存只会为编译时常量分配一次内存，然后重复使用。所以，儘管是創建了兩個類實例對象，但是這兩個引用變量所引用的是同一個對象。
### StringBuffer类

> 
StringBuffer 是可变类，有方法append()、replaceAll()、replaceFirst()、insert()和setCharAt()等方法，都会改变字符串缓冲区的字符串内容。 

  StringBuffer 沒有覆盖Object类的 equals 方法。因此使用方法 equals 时候，会出现结果 `false`


```
StringBuffer sb1 = new StringBuffer("hello world");
StringBuffer sb2 = new StringBuffer("hello world");
//答案就是--false
System.out.println("打印的结果sb1和sb2是否equals>>>"+(sb1.equals(sb2)));
```

### StringBuilder类

> 
一个可变的字符序列。此类提供一个与 StringBuffer 兼容的 API，但不保证同步。设计目的是作为 StringBuffer的一个简易替换。


在执行速度上，`StringBuilder >  StringBuffer  >  String`

StringBuilder是`非线程安全`的；而StringBuffer是`线程安全`
`单线程操作`字符串缓冲区 下操作大量数据 建议使用>>>`StringBuilder`
`多线程操作`字符串缓冲区 下操作大量数据 建议使用>>>`StringBuffer`
- ***不可变类***,String 对象创建后，它的内容是无法改变的。而一些看起来能够改变字符串的方法，实际上是创建一个带有方法所赋予特性的新的字符串。例如，String类的substring()方法。 [↩](#fnref:footnote)








