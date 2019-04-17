# StringBuffer类 - 博客堂 - CSDN博客





2017年02月24日 10:25:07[最小森林](https://me.csdn.net/u012052268)阅读数：250标签：[stringbuffer																[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)](https://so.csdn.net/so/search/s.do?q=stringbuffer&t=blog)
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)








# StringBuffer类


- [StringBuffer类](#stringbuffer类)- [StringBuffer类概述](#stringbuffer类概述)
- [StringBuffer和String的区别](#stringbuffer和string的区别)
- [StringBuffer的构造方法](#stringbuffer的构造方法)- [StringBuffer的构造方法](#stringbuffer的构造方法-1)
- [StringBuffer的方法](#stringbuffer的方法)
- [添加功能](#添加功能)
- [删除功能](#删除功能)
- [替换功能](#替换功能)
- [反转功能](#反转功能)
- [截取功能](#截取功能)
- [StringBuffer类练习](#stringbuffer类练习)- [String和StringBuffer的相互转换](#1-string和stringbuffer的相互转换)
- [把数组拼接成一个字符串](#2把数组拼接成一个字符串)
- [把字符串反转](#3把字符串反转)
- [判断一个字符串是否是对称字符串](#4判断一个字符串是否是对称字符串)

- [StringBuffer类面试题](#stringbuffer类面试题)- [1StringStringBufferStringBuilder的区别](#1stringstringbufferstringbuilder的区别)
- [2StringBuffer和数组的区别](#2stringbuffer和数组的区别)
- [3形式参数问题](#3形式参数问题)






## StringBuffer类概述
- 
我们如果对字符串进行拼接操作，每次拼接，都会构建一个新的String对象，既耗时，又浪费空间。而StringBuffer就可以解决这个问题

- 
线程安全的可变字符序列


## StringBuffer和String的区别?
- 前者长度和内容可变，后者不可变。
- 如果使用前者做字符串的拼接，不会浪费太多的资源。

## StringBuffer的构造方法：

### StringBuffer的构造方法：

```
public StringBuffer():无参构造方法

public StringBuffer(int capacity):指定容量的字符串缓冲区对象

public StringBuffer(String str):指定字符串内容的字符串缓冲区对象
```

### StringBuffer的方法：

```
public int capacity()：返回当前容量。   理论

public int length():返回长度（字符数）。 实际值
```

### 添加功能

```
public StringBuffer append(String str)

public StringBuffer insert(int offset,String str)
```

### 删除功能

```
public StringBuffer deleteCharAt(int index)

public StringBuffer delete(int start,int end)
```

### 替换功能

```
public StringBuffer replace(int start,int end,String str)
```

### 反转功能

```
public StringBuffer reverse()
```

### 截取功能

截取功能和前面几个功能的不同 , 返回值类型是String类型，本身没有发生改变

```
public String substring(int start)

public String substring(int start,int end)
```

### StringBuffer类练习

#### 1. String和StringBuffer的相互转换

```java
String s = "hello";

// 方式1:通过构造方法

StringBuffer sb = new StringBuffer(s);

// 方式2：通过append()方法

StringBuffer sb2 = new StringBuffer();
sb2.append(s);
```

#### 2.把数组拼接成一个字符串

```
public static String arrayToString2(int[] arr) {
        StringBuffer sb = new StringBuffer();

        sb.append("[");
        for (int x = 0; x < arr.length; x++) {
            if (x == arr.length - 1) {
                sb.append(arr[x]);
            } else {
                sb.append(arr[x]).append(", ");
            }
        }
        sb.append("]");

        return sb.toString();
    }
```

#### 3.把字符串反转

```
public static String myReverse2(String s) {
        // StringBuffer sb = new StringBuffer();
        // sb.append(s);

        // StringBuffer sb = new StringBuffer(s);
        // sb.reverse();
        // return sb.toString();

        // 简易版
        return new StringBuffer(s).reverse().toString();
    }
```

#### 4.判断一个字符串是否是对称字符串

例如”abc”不是对称字符串，”aba”、”abba”、”aaa”、”mnanm”是对称字符串

```java
public static boolean isSame(String s) {
        boolean flag = true;

        // 把字符串转成字符数组
        char[] chs = s.toCharArray();

        for (int start = 0, end = chs.length - 1; start <= end; start++, end--) {
            if (chs[start] != chs[end]) {
                flag = false;
                break;
            }
        }

        return flag;
    }
```

### StringBuffer类面试题

#### 1：String,StringBuffer,StringBuilder的区别?

A:String是内容不可变的，而StringBuffer,StringBuilder都是内容可变的。

B:StringBuffer是同步的，数据安全,效率低;StringBuilder是不同步的,数据不安全,效率高

#### 2：StringBuffer和数组的区别?

二者都可以看出是一个容器，装其他的数据。

但是呢,StringBuffer的数据最终是一个字符串数据。

而数组可以放置多种数据，但必须是同一种数据类型的。

#### 3:形式参数问题

基本类型：形式参数的改变不影响实际参数

引用类型：形式参数的改变直接影响实际参数

> 
注意：String作为参数传递，效果和基本类型作为参数传递是一样的。





