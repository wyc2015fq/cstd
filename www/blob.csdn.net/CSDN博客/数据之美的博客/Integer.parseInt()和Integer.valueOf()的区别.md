# Integer.parseInt()和Integer.valueOf()的区别 - 数据之美的博客 - CSDN博客
2018年11月22日 18:55:42[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：292
`new Integer.valueof()返回的是Integer的对象。``Integer.parseInt() 返回的是一个int的值。``new Integer.valueof().intValue();返回的也是一个int的值。`
**parseInt**
`public static int parseInt(String s, int radix) throws NumberFormatException`
以第二个参数所指定基数将字符串参数分析为一个带符号的整数。除了第一个字符可以用 ASCII 的减号 `'-'` 来表示一个负值外，字符串中的字符必须是指定基数的数(由 `Character.digit` 是否返回非负值决定)。返回作为结果的整数值。
**参数：**
s - 包含整数的 `String` 。
radix - 使用的进制
**返回值：**
指定基数的字符串参数所表示的整数。
**抛出：**[NumberFormatException](http://www.leftworld.net/online/j2sedoc/javaref/java.lang.numberformatexception_dsc.htm#_top_)
若该串不包含一个可分析的整数。
**parseInt**
`public static int parseInt(String s) throws NumberFormatException`
将字符串参数作为带符号十进制整数来分析。除过第一个字符为 ASCII 字符中减号 `'-'` 表示的负数，字符串中的字符都必须是十进制数。
**参数：**
s - 串。
**返回值：**
十进制参数表示的整数。
**抛出：**[NumberFormatException](http://www.leftworld.net/online/j2sedoc/javaref/java.lang.numberformatexception_dsc.htm#_top_)
若该串不包含一个可分析的整数。
**valueOf**
`public static Integer valueOf(String s, int radix) throws NumberFormatException`
返回初始化为指定 String 值的新的 Integer 对象。若该 String 不能作为 int 分析，则抛出异常。
**参数：**
s - 待分析的字符串。
**返回值：**
新创建的 `Integer` ，将其以指定基数初始化为字符串参数所表示的值。
**抛出：**[NumberFormatException](http://www.leftworld.net/online/j2sedoc/javaref/java.lang.numberformatexception_dsc.htm#_top_)
若 `String`不包含可分析的整数。
**valueOf**
`public static Integer valueOf(String s) throws NumberFormatException`
返回初始化为指定 String 值的新的 Integer 对象。若该 String 不能作为 int 分析，则抛出异常。假设基数为 10 。
**参数：**
s - 待分析的字符串。
**返回值：**
新创建的 `Integer` ，将其初始化为字符串参数所表示的值。
**抛出：**[NumberFormatException](http://www.leftworld.net/online/j2sedoc/javaref/java.lang.numberformatexception_dsc.htm#_top_)
如果该串不包含一个可分析的整数。
static int parseInt(String s)
          将字符串参数作为有符号的十进制整数进行分析。
static Integer valueOf(int i)
          返回一个表示指定的 int 值的 Integer 实例。
static Integer valueOf(String s)
          返回保持指定的 String 的值的 Integer 对象。
从返回值可以看出他们的区别   parseInt()返回的是基本类型int
而valueOf()返回的是包装类Integer  Integer是可以使用对象方法的  而int类型就不能和Object类型进行互相转换
int zhuan=Integer.parseInt(chuan);
int zhuanyi=Integer.valueOf(chuan); 为什么你的程序返回值都可以用int来接收呢？  因为Integer和int可以自动转换
Integer i = 5;   int k  = i;像是这样表示是没有编译错误的
例题：
设有下面两个赋值语句：
a = Integer.parseInt(“123”);
b = Integer.valueOf(“123”).intValue();
下述说法正确的是（  d  ）。
A、a是整数类型变量，b是整数类对象。
B、a是整数类对象，b是整数类型变量。
C、a和b都是整数类对象并且值相等。 
D、a和b都是整数类型变量并且值相等。
详细解析：
parseInt(String s )方法是类Integer的静态方法，它的作用就是将形参 s 转化为整数，比如：
Interger.parseInt("1")=1;
Integer.parseInt("20")=20;
Integer.parseInt("324")=324;
当然，s 表示的整数必须合法，不然是会抛异常的。
valueOf(String s )也是Integer类的静态方法，它的作用是将形参 s 转化为Integer对象，
什么是Integer对象，Integer就是基本数据类型int型包装类，就是将int包装成一个类，这样在很多场合下是必须的。如果理解不了，你就认为int是Integer的mini版，好用了很多，但也丢失了一些功能，好了，看代码：
Interger.valueOf("123")=Integer(123)
这时候Integer（123）就是整数123的对象表示形式，它再调用intValue()方法，就是将123的对象表示形式转化为基本数据123
所以，选择D
