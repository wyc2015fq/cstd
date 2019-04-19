# Java String/StringBuffer/StringBuilder - Simple 专栏 - CSDN博客
2012年01月11日 22:53:15[Simple_Zz](https://me.csdn.net/love284969214)阅读数：234标签：[java																[java基础																[Java面试](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)](https://so.csdn.net/so/search/s.do?q=java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
### 三者在执行速度方面的比较：**StringBuilder >  StringBuffer  >  String**
### **String <（StringBuffer，StringBuilder）**的原因
- String：字符串常量
- StringBuffer：字符创变量
- StringBuilder：字符创变量
从上面的名字可以看到，String是“字符串常量”，也就是不可改变的对象。对于这句话的理解你可能会产生这样一个疑问  ，比如这段代码：
```java
String s = "abcd";
s = s+1;
System.out.print(s);// result : abcd1
```
我们明明就是改变了String型的变量s的，为什么说是没有改变呢? 其实这是一种欺骗，JVM是这样解析这段代码的：首先创建对象s，赋予一个abcd，然后再创建一个新的对象s用来执行第二行代码，也就是说我们之前对象s并没有变化，所以我们说String类型是不可改变的对象了，由于这种机制，每当用String操作字符串时，实际上是在不断的创建新的对象，而原来的对象就会变为垃圾被ＧＣ回收掉，可想而知这样执行效率会有多低。
　　　  而StringBuffer与StringBuilder就不一样了，他们是字符串变量，是可改变的对象，每当我们用它们对字符串做操作时，实际上是在一个对象上操作的，这样就不会像String一样创建一些而外的对象进行操作了，当然速度就快了。
### 一个特殊的例子
```java
String str = “This is only a” + “ simple” + “ test”;
StringBuffer builder = new StringBuilder(“This is only a”).append(“ simple”).append(“ test”);
```
- 你会很惊讶的发现，生成str对象的速度简直太快了，而这个时候StringBuffer居然速度上根本一点都不占优势。其实这是JVM的一个把戏，实际上
- String str = “This is only a” + “ simple” + “test”;
- 其实就是：
- String str = “This is only a simple test”;
- 所以不需要太多的时间。但这里要注意的是，如果字符串是来自另外的String对象的话，速度就没那么快了，譬如：
- String str2 = “This is only a”;
- String str3 = “ simple”;
- String str4 = “ test”;
- String str1 = str2 +str3 + str4;
- 这时候JVM会规规矩矩的按照原来的方式去做。
### StringBuilder与 StringBuffer
- StringBuilder：线程非安全的
- StringBuffer：线程安全的
- 当我们在字符串缓冲去被多个线程使用是，JVM不能保证StringBuilder的操作是安全的，虽然他的速度最快，但是可以保证StringBuffer是可以正确操作的。当然大多数情况下就是我们是在单线程下进行的操作，所以大多数情况下是建议用StringBuilder而不用StringBuffer的，就是速度的原因。
### **对于三者使用的总结**：
- **如果要操作少量的数据用 = String**
- **单线程操作字符串缓冲区 下操作大量数据 = StringBuilder**
- **多线程操作字符串缓冲区 下操作大量数据 = StringBuffer**
