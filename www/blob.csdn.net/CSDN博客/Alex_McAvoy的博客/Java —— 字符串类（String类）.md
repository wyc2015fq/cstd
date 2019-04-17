# Java —— 字符串类（String类） - Alex_McAvoy的博客 - CSDN博客





2018年10月11日 00:37:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【创建字符串对象】

一般初始化

```java
String str=null;//初始化为空值
String str="";//初始化为空字符串
String str="abc";//初始化为abc
```

通过构造器初始化

```java
String str=new String("abc");
```

空字符串 "" 不等同于空值，空字符串是实例化的对象，但是不包含任何字符，空值是没有指向。

# 【连接字符串】

连接字符串通过 “ + ” 运算符实现，在这里是将多个字符串合并到一起生成一个新的字符串。

对于 “ + ” 运算符，如果有一个操作元为 String 类型，则为字符串连接运算符，字符串可与任意类型的数据进行字符串连接操作，若该数据为基本类型，则会自动转换为字符串；若为引用类型，则会自动调用所引用对象的 toString() 方法获得一个字符串，然后进行字符串连接操作。

```java
System.out.println("abc"+123);    //与int型连接
System.out.println("abc"+123.456);//与double型连接
System.out.println("abc"+true);   //与boolean型连接
System.out.println("abc"+"def");  //字符串间连接
System.out.println("abc"+new java.io.File("C:\text.txt"));  //与引用类型连接
```

若表达式中包含多个 “ + ” 运算符，并且存在各种数据类型参与运算，则按照 “ + ” 运算符从左到右进行运算，JVM 会根据 “ + ” 运算符两边的操作元类型来决定是算术运算还是字符串连接运算。

```java
System.out.println(123+100+"abc");    //结果为：223abc
System.out.println("abc"+123+100);    //结果为：abc123100
```

# 【字符串操作】

## 1.获取字符串的长度

字符串是一个对象，在这个对象中包含 length 属性，其是该字符串的长度，使用 String 类中的 length() 方法可以获取该属性值。

```java
String str="abc";
int len=str.length();
```

## 2.字符串比较

由于字符串是对象类型，而 " == " 比较是比较两对象的内存地址，因此不能用其来进行两个字符串的比较，下面是几种字符串比较内容的方法。

### 1）equals() 方法

equals() 方法用于比较两字符串是否相等，返回值为 boolean 型，若相等则返回 true，否则返回 false，要注意的是，equals() 方法比较时区分大小写。

```java
//方法定义
public boolean equals(String str)

//实例
String str="A";
boolean b=str.equals("a");//结果为：false
```

### 2）equalsIgnoreCase() 方法

equalsIgnoreCase() 与 equals() 方法基本相同，唯一不同的是该方法比较时不区分大小写。

```java
//方法定义
public boolean equalsIgnoreCase(String str)

//实例
String str="A";
boolean b=str.equals("a");//结果为：true
```

### 3）compareTo() 方法

compareTo() 方法用于判断一个字符串是大于、小于还是等于另一个字符串，判断依据是它们在字符串中的顺序，入口参数为被比较的字符串对象，返回值为 int 型，若两字符串相同则返回0，若大于被比较的字符串则返回一个正数，若小于被比较的字符串则返回一个负数。

```java
//方法定义
public int compareTo(String str)

//实例
String str="B";
String str1="A";
String str2="B";
String str3="C";
System.out.println(str.compareTo(str1));//结果为：1
System.out.println(str.compareTo(str2));//结果为：0
System.out.println(str.compareTo(str3));//结果为：-1
```

### 4）startsWith() 方法

startsWith() 方法用于判断字符串是否以指定的字符串开始，其入口参数是欲比较的字符串对象，返回值为 boolean 型，若是以指定字符串开始，则返回 true，否则返回 false。

其还有一重载方法，用于判断字符串从指定索引位置开始是否为指定字符串开始。

```java
//方法定义
public int stringWith(String prefixe)

//实例
String str="ABCDE";
boolean b=str.startsWith("A");//结果为：true

//重载方法定义
public int stringWith(String prefixe，int toffset)

//实例
String str="ABCDE";
boolean b=str.startsWith("A",2);//结果为：false
```

### 5）endsWith() 方法

endsWith() 方法用于判断字符串是否以指定的字符串结束，其入口参数是欲比较的字符串对象，返回值为 boolean 型，若是以指定字符串结束，则返回 true，否则返回 false。

```java
//方法定义
public int stringWith(String suffix)

//实例
String str="ABCDE";
boolean b=str.endsWith("DE");//结果为：true
```

## 3.字符串的大小写转换

toLowerCase()、toUpperCase() 分别用来将字符串中的大写字母转成小写字母、将小写字母转换成大写字母，它们的返回值均为转换后的字符串。

```java
String str="abcEFD";
String str1=str.toUpperCase();
String str2=str.toLowerCase();
```

## 4.去掉字符串首尾空格

trim() 方法可以去掉字符串的首尾空格后得到一个新的字符串。

```java
String str1=" abc ";
int len1=str1.length();//长度为5
String str2=str1.trim();
int len2=str2.length();//长度为3
```

## 5.截取字符串的子串

substring() 方法可以从现有字符串截取子串，有如下两个重载方法：

1）public String substring(int beginIndex)：截取从指定索引位置到最后的子字符串，截取得到的字符串包含指定索引位置的字符

```java
String str="abcdef";
System.out.println(str.substring(3));//截取到的子串为：def
```

2）public String substring(int beginIndex,int endIndex)：截取从指定索引位置到最后的子字符串，截取得到的字符串包含指定初始索引位置的字符，不包含指定终止索引位置的字符

```java
String str="abcdef";
System.out.println(str.substring(2,4));//截取到的子串为：cd
```

## 6.替换字符串中的字符或子串

### 1）替换字符

通过 replace() 方法可以将原字符串中的某个字符替换为指定字符，并得到一个新的字符串

```java
//方法定义
public String replace(char oldChar,char newChar)

//实例
String str="abc_abc_abc";
System.out.println(str.replace('a','M'));//结果为：Mbc_Mbc_Mbc
```

### 2）替换原字符串中的指定子串

```java
//方法定义
public String replaceAll(String regex,String replacement)

//实例
String str="abc_abc_abc";
System.out.println(str.replaceAll("abc",'AAA'));//结果为：AAA_AAA_AAA
```

### 3）替换原字符串中的第一个子串

```java
//方法定义
public String replaceFirst(String regex,String replacement)

//实例
String str="abc_abc_abc";
System.out.println(str.replaceAll("abc",'AAA'));//结果为：AAA_abc_abc
```

## 7.字符串查找

String 类提供了两种查找字符串的方法，它们允许在字符串中搜索指定的字符或字符串。

indexOf() 方法用于搜索字符或字符串首次出现的位置，lastIndexOf() 方法用于搜索字符或字符串最后一次出现的位置，这两种方法均有多种重载，它们的返回值均为字符或字符串被发现的索引位置，若未搜索到，则返回-1。
|indexOf(char ch)|用于获取指定字符在原字符串中第一次出现的索引|
|----|----|
|indexOf(String str)|用于获取指定字符串在原字符串中第一次出现的索引|
|indexOf(char ch,int startIndex)|用于获取指定字符在原字符串中指定索引位置开始第一次出现的索引|
|indexOf(String str,int startIndex)|用于获取指定字符串在原字符串中指定索引位置开始第一次出现的索引|
|lastIndexOf(char ch)|用于获取指定字符在原字符串中最后一次出现的索引|
|lastIndexOf(String str)|用于获取指定字符串在原字符串中最后一次出现的索引|
|lastIndexOf(char ch,int startIndex)|用于获取指定字符在原字符串中指定索引位置开始最后一次出现的索引|
|lastIndexOf(String str,int startIndex)|用于获取指定字符串在原字符串中指定索引位置开始最后一次出现的索引|

```java
String str="1234567890";
System.out.println(str.indexOf('1'));//结果为：0
System.out.println(str.indexOf("23"));//结果为：1
System.out.println(str.indexOf('1',1));//结果为：-1
System.out.println(str.indexOf("34",1));//结果为：2
System.out.println(str.lastIndexOf('0'));//结果为：9
System.out.println(str.lastIndexOf("89"));//结果为：7
System.out.println(str.lastIndexOf('1',5));//结果为：-1
System.out.println(str.lastIndexOf("123",5));//结果为：-1
```

## 8.字符串分割

String 类提供了两个重载在 split() 方法，用于将字符串按指定规则进行分割，并以 String 型数组的方式返回，分割得到的子串在数组中的顺序按照它们在字符串中的顺序排列。

重载方法定义：public String[] split(String regex,int limit)

第一个入口参数 regex 为分割规则，第二个入口参数 limit 为分割规则的应用次数。

若 limit 大于 0，则规则最多被应用 limit-1 次，数组长度不会大于 limit，且数组的最后一项将包含超出最后匹配的所有字符；若 limit 为非正整数，则分割规则将被应用尽可能多的次数，且数组可为任意长度；若 limit 为 0，则数组中位于最后的所有空字符串元素将被丢弃。

若将 limit 参数设为 0，也可采用重载方法：split(String regex)，其定义如下：

public String[] split(String regex){

    return split(regex,0);

}

实例：将字符串 “boo:and:foo” 分别按不同规则进行分割

```java
String str="boo:and:foo";
//分隔符：:，限定数：2，得到的数组：a={"boo","and:foo"}
String[] a=str.split(":",2);

//分隔符：:，限定数：5，得到的数组：b={"boo","and","foo"}
String[] b=str.split(":",5);

//分隔符：:，限定数：-2，得到的数组：c={"boo","and","foo"}
String[] c=str.split(":",-2);

//分隔符：o，限定数：5，得到的数组：d={"b","",":and:f","",""}
String[] d=str.split("o",5);

//分隔符：o，限定数：-2，得到的数组：e={"b","",":and:f","",""}
String[] e=str.split("o",-2);

//分隔符：o，限定数：0，得到的数组：f={"b","",":and:f"}
String[] f=str.split("o",0);

//分隔符：m，限定数：0，得到的数组：g={"boo:and:foo"}
String[] g=str.split("m",0);
```





