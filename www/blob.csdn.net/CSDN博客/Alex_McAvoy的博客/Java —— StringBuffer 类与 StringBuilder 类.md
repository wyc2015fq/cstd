# Java —— StringBuffer 类与 StringBuilder 类 - Alex_McAvoy的博客 - CSDN博客





2018年10月12日 00:09:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：19
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【StringBuffer 类】

StringBuffer 类与 String 相似，其代表可变的字符序列，可以对字符串的内容进行修改，其很多方法与 String 类相同，位于 java.lang 包中。

## 1.对象创建

StringBuffer 对象的初始化与 String 对象的初始化相同，通常使用构造器进行初始化。

```java
StringBuffer str=new StringBuffer("abc");
```

StringBuffer 类有如下三个构造器，可根据需要自由选用

```java
//空参构造器，没有任何内容，该对象的初始容量可容纳16个大小，当该对象实体存放的字符长度大于16时，实体容量会自动增加
StringBuffer()

//字符串构造器，该对象的初始容量为参数字符串str长度及额外再增加的16个字符
StringBuffer(String str)

//整型构造器，该对象的初始容量由size决定，当对象的实体存放大于size时，实体容量会自动增加
StringBuffer(int size)
```

StringBuffer 对象可以通过 length() 方法获取实体中存放的字符序列的长度，通过 capacity() 方法获取当前实体的实际容量。

## 2.常用方法

### 1）append() 方法

append() 用于将其他类型数据转换为字符串后再追加到当前对象中。

```java
//实例
StringBuffer str=new StringBuffer("abc");
str.append(123);//结果为：abc123
str.append(4.56);//结果为：abc1234.56
str.append(true);//结果为：abc1234.56true
```

### 2）setCharAt() 方法

setCharAt() 方法用于将当前 StringBuffer 实体中的指定位置的字符用指定字符替换，并返回当前对象的引用。

```java
//方法定义
public void setCharAt(int n,char ch)

//实例
StringBuffer str=new StringBuffer("abc");
str.setCharAt(1,'B');//结果为：aBc
```

### 3）replace() 方法

replace() 方法用于将当前对象中指定位置的子串用指定字符串替换，并返回当前对象的引用。

```java
//方法定义
public void replace(int startIndex,int endIndex,String str)

//实例
StringBuffer str=new StringBuffer("abcdef");
str.replace(1,3,"123");//结果为：a123ef
```

### 4）insert() 方法

insert() 方法用于将指定的字符串插入当前对象指定位置中，并返回当前对象的引用。

```java
//方法定义
public void insert(int index,String str)

//实例
StringBuffer str=new StringBuffer("abc");
str.index(1,"ABC");//结果为：aABCbc
```

### 5）reverse() 方法

reverse() 方法用于将当前对象中的字符翻转，并返回当前对象的引用。

```java
//方法定义
public void reverse()

//实例
StringBuffer str=new StringBuffer("abc");
str.reverse();//结果为：cba
```

### 6）deleteCharAt() 方法

deleteCharAt() 方法用于删除当前对象指定位置的字符，并返回当前对象的引用

```java
//方法定义
public void deleteCharAt(int index)

//实例
StringBuffer str=new StringBuffer("abc");
str.deleteCharAt("1");//结果为：ac
```

### 7）delete() 方法

delete() 方法用于删除当前对象指定位置的子串，并返回当前对象的引用

```java
//方法定义
public void deleteCharAt(int startIndex,int endIndex)
//参数startIndex指定要删除的第一个字符的下标，参数endIndex指定要删除的最后一个字符的下一个字符的下标，因此要删除的子串是：startIndex~endIndex-1 范围内的子串

//实例
StringBuffer str=new StringBuffer("abcefd");
str.delete(1,3);//结果为：aefd
```

# 【StringBuilder】

StringBuilder 类也是可变的字符序列，与StringBuffer相似，但线程不安全，若不考虑线程安全问题，其效率要高 StringBuffer。

# 【String、String、StringBuilder 的对比】

1.String 是字符串常量，底层是通过 char[] 存放的，一旦被赋值就不可再修改；StringBuffer、StringBuilder 是字符串变量，值是可以更改的。因此 String 与 StringBuffer、StringBuilder 不可直接进行强制转换。

2.运行速度：StringBuilder > StringBuffer > String

3.线程安全：StringBuilder 是线程不安全的，而 StringBuffer 是线程安全的。

当一个 StringBuffer 对象在字符串缓冲区被多个线程使用时，StringBuffer 中的方法可带有 synchronized 关键字，从而保证线程安，但 StringBuilder 的方法则没有该关键字，所以不能保证线程安全。

4.适用情况

String：适用少量字符串操作的情况

StringBuilder：适用单线程下在字符缓冲区大量操作的情况

StringBuffer：适用多线程下在字符缓冲区大量操作的情况



