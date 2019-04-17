# String类的概述和使用 - 博客堂 - CSDN博客





2017年02月22日 22:30:00[最小森林](https://me.csdn.net/u012052268)阅读数：256
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)









# String类的概述和使用(掌握)

## (1)概述

多个字符组成的一串数据。其实它可以和字符数组进行相互转换。

下图是解释一般String的存储方式。

![String的特点一旦被赋值就不能改变.bmp](https://ooo.0o0.ooo/2017/02/22/58ad9f43783a9.bmp)

## (2)构造方法：

```
A:public String()

B:public String(byte[] bytes)   // 这些[]都是数组

C:public String(byte[] bytes,int offset,int length)

D:public String(char[] value)

E:public String(char[] value,int offset,int count)

F:public String(String original)
```

## (3)字符串的特点
- 字符串一旦被赋值，就不能改变。

注意：这里指的是字符串的内容不能改变，而不是引用不能改变。
- 字面值作为字符串对象和通过构造方法创建对象的不同

> 
String s = new String(“hello”) ; 和 String s = “hello”的区别?


第一个有new（）所以开辟了堆内存，栈–>堆–>常量区 。

而第二个直接指向 方法区中的常量区。

## (4)字符串的面试题(看程序写结果)

```
A:==和equals()
    String s1 = new String("hello");
    String s2 = new String("hello");
    System.out.println(s1 == s2);// false
    System.out.println(s1.equals(s2));// true

    String s3 = new String("hello");
    String s4 = "hello";
    System.out.println(s3 == s4);// false
    System.out.println(s3.equals(s4));// true

    String s5 = "hello";
    String s6 = "hello";
    System.out.println(s5 == s6);// true
    System.out.println(s5.equals(s6));// true
B:字符串的拼接
    String s1 = "hello";
    String s2 = "world";
    String s3 = "helloworld";
    System.out.println(s3 == s1 + s2);// false
    System.out.println(s3.equals((s1 + s2)));// true

    System.out.println(s3 == "hello" + "world");// false 这个我们错了，应该是true
    System.out.println(s3.equals("hello" + "world"));// true
```

## (5)字符串的功能(自己补齐方法中文意思)

```
String类的判断功能：

 boolean equals(Object obj):比较字符串的内容是否相同,区分大小写

 boolean equalsIgnoreCase(String str):比较字符串的内容是否相同,忽略大小写

 boolean contains(String str):判断大字符串中是否包含小字符串

 boolean startsWith(String str):判断字符串是否以某个指定的字符串开头

 boolean endsWith(String str):判断字符串是否以某个指定的字符串结尾

 boolean isEmpty():判断字符串是否为空。
```

```
String类的获取功能

int length():获取字符串的长度。

char charAt(int index):获取指定索引位置的字符

int indexOf(int ch):返回指定字符在此字符串中第一次出现处的索引。
        为什么这里是int类型，而不是char类型?
        原因是：'a'和97其实都可以代表'a'

int indexOf(String str):返回指定字符串在此字符串中第一次出现处的索引。

int indexOf(int ch,int fromIndex):返回指定字符在此字符串中从指定位置后第一次出现处的索引。

int indexOf(String str,int fromIndex):返回指定字符串在此字符串中从指定位置后第一次出现处的索引。

String substring(int start):从指定位置开始截取字符串,默认到末尾。

String substring(int start,int end):从指定位置开始到指定位置结束截取字符串。
```

```
String的转换功能：

 byte[] getBytes():把字符串转换为字节数组。

 char[] toCharArray():把字符串转换为字符数组。

 static String valueOf(char[] chs):把字符数组转成字符串。

 static String valueOf(int i):把int类型的数据转成字符串。

 注意：String类的valueOf方法可以把任意类型的数据转成字符串。

 String toLowerCase():把字符串转成小写。

 String toUpperCase():把字符串转成大写。

 String concat(String str):把字符串拼接。
```

```
String类的其他功能：

替换功能：

 String replace(char old,char new)
 String replace(String old,String new)

去除字符串两空格    

 String trim()

按字典顺序比较两个字符串  
 int compareTo(String str)
 int compareToIgnoreCase(String str)
```



