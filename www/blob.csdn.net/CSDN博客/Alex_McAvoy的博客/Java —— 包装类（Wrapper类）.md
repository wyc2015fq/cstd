# Java —— 包装类（Wrapper类） - Alex_McAvoy的博客 - CSDN博客





2018年10月03日 14:03:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1285
所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

由于 Java 中的八种基本数据类型不面向对象，为了使用方便，为每个基本数据类型设计了一个对应的类，这样八种基本数据类型对应的类统称为包装类（Wrapper Class），均位于 java.lang 包中。
|基本数据类型|包装类|
|----|----|
|int|Bollean|
|byte|Byte|
|short|Short|
|long|Long|
|float|Float|
|double|Double|
|boolean|Boolean|
|char|Character|

包装类的用途主要为以下两种：
- 作为基本数据类型对应的类类型存在，方便涉及到对象的操作
- 包含每种基本数据类型的相关属性（最大值、最小值等）以及相关的操作方法

# 【Number 类】
- 抽象类 Number 是 BigDecimal、BigInteger、Double、Float、Integer、Long、Short、Byte 类的父类
- 其方法分别被其各个子类所实现，以下 6 种方法是最常见的
|返回值|方法|功能|
|----|----|----|
|byte|byteValue()|以byte类型返回指定的数值|
|short|shortValue()|以short类型返回指定的数值|
|int|intValue()|以int类型返回指定的数值|
|long|longValue()|以long类型返回指定的数值|
|float|floatValue()|以float类型返回指定的数值|
|double|doubleValue()|以double类型返回指定的数值|

# 【Integer 类】

Integer 类、Long 类、Short 类、Byte 类都是对整数进行操作，包含的方法基本相同，区别只是表示的范围不同，以下以 Integer 类介绍整数包装类。

## 1.构造器

1）Integer(int value)：以一个 int 型变量作为参数获取 Integer 对象

2）Integer(String str)：以一个 String 型变量作为参数获取 Integer 对象（要用数值型 String 变量作为参数，否则会抛出 NumberFormatException异常）

```java
Integer a=new Integer(10);
Integer a=new Integer("123");
```

## 2.常用方法
|返回值|方法|功能|
|----|----|----|
|int|compareTo(Integer obj)|在数字上比较两Integer对象是否相等，相同返回0，小于返回-1，大于返回1|
|boolean|equals(Object obj)|比较此对象与指定对象是否相等|
|Integer|valueOf(int obj)|返回一个表示指定的int值的Integer实例|
|int|parseInt(String str)|返回一个将str解析后的int型变量|
|String|toString()|以String类型返回该Integer对象|

## 3.常量
- MAX_VALUE：int 型可取的最大值，即：2^31-1（2147483647）
- MIN_VALUE：int 型可取的最小值，即：-2^31（-2147483648）
- SIZE：以二进制补码形式表示 int 值的位数
- TYPE：表示基本类型 int 的 Class 实例

# 【Double 类】

Double 类、Float 类都是对小数进行操作，包含的方法基本相同，区别只是表示的范围不同，以下以 Double 类介绍小数包装类。

## 1.构造器

1）Double(double value)：以一个 double 型变量作为参数获取 Double 对象

2）Double(String str)：以一个 String 型变量作为参数获取 Double 对象（要用数值型 String 变量作为参数，否则会抛出 NumberFormatException异常）

```java
Double a=new double(1.23);
Double a=new double("4.56");
```

## 2.常用方法
|返回值|方法|功能|
|----|----|----|
|int|compareTo(Double obj)|在数字上比较两Integer对象是否相等，相同返回0，小于返回-1，大于返回1|
|boolean|equals(Object obj)|比较此对象与指定对象是否相等|
|Double|valueOf(double obj)|返回一个表示指定的double值的Double实例|
|double|parseInt(String str)|返回一个将str解析后的double型变量|
|String|toString()|以String类型返回该Double对象|
|boolean|isNaN()|若此double值是非数字值(NaN)，则返回 true，否则返回 false|

## 3.常量
- MAX_EXPONENT：返回 int 值，表有限 double 变量可能具有的最大指数。
- MIN_EXPONENT：返回 int 值，表标准化 double 变量可能具有的最小指数。
- POSITIVE_INFINITY：返回 double 值，表保存 double 类型的正无穷大的常量。
- NEGATIVE_INFINITY：返回 double 值，表保存 double 类型的负无穷大的常量。

# 【Boolean 类】

## 1.构造器

1）Boolean(boolean value)：创建一个表示 value 的 Boolean 对象

2）Integer(String str)：以一个 String 型变量作为参数创建 Boolean 对象（若 str 参数不为 null 且忽略大小写时为 true，则分配一表 true 值的 Boolean 对象，否则获得一 false 值的 Boolean 对象）

```java
Boolean a=new Boolean(true);
Boolean a=new Boolean("ok");
```

## 2.常用方法
|返回值|方法|功能|
|----|----|----|
|boolean|equals(Object obj)|比较此对象与指定对象是否相等|
|Boolean|valueOf(boolean obj)|返回一个表示指定的boolean值的Boolean实例|
|boolean|parseBoolean(String str)|返回一个将str解析后的boolean型变量|
|String|toString()|以String类型返回该Boolean对象|

## 3.常量
- TRUE：对应基值为 true 的 Boolean 对象
- FALSE：对应基值为 false 的 Boolean 对象
- TYPE：表示基本类型 boolean 的 Class 实例

# 【Character 类】

## 1.构造器

Character(char value)：创建一个表示 value 的 Character 对象（一旦 Character 类被创建，其包含的值无法再改变）

```java
Character a=new Character(‘a’);
```

## 2.常用方法
|返回值|方法|功能|
|----|----|----|
|char|charValue()|返回此对象的值|
|int|compareTo(Double obj)|在数字上比较两Integer对象是否相等，相同返回0，小于返回-1，大于返回1|
|boolean|equals(Object obj)|比较此对象与指定对象是否相等|
|char|toUpperCase(char ch)|将字符参数转换为大写|
|char|toLowerCase(char ch)|将字符参数转换为小写|
|boolean|toUpperCase(char ch)|判断字符参数是否为大写|
|boolean|toLowerCase(char ch)|判断字符参数是否为小写|
|String|toString()|返回一个表示指定char值的Character对象|

## 3.常量
- CONNECTOR_PUNCTUATION：返回 byte 型值，表示 Unicode 规范中的常规类别 “Pc”
- UNASSIGNED：返回 byte 型值，表示 Unicode 规范中的常规类别 “Cn”
- TITLECASE_LETTER：返回 byte 型值，表示 Unicode 规范中的常规类别 “Lt”

# 【自动拆装箱】

JDK 自从 1.5 版本后，引入了自动拆装箱的语法，即系统将自动进行基本数据类型和对应的包装类转换时。 

自动装箱：将基本数据类型封装为对象类型。

自动拆箱：将对象重新转化为基本数据类型。

```java
//自动装箱，实际上执行了 Integer a=Integer.valueOf(5)
Integer a=5;

//自动拆箱，实际上执行了 int b=a.intValue()
int b=a;
```

# 【包装类、基本数据类型、String 类的转化】

![](https://img-blog.csdn.net/20181003201236867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：要将 String 转为 char 变量，使用 atChar() 方法；要转为 char 数组，使用 toCharArrays() 方法**



