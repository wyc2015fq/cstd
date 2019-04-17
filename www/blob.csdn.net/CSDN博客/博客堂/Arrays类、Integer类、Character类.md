# Arrays类、Integer类、Character类 - 博客堂 - CSDN博客





2017年02月24日 20:29:16[最小森林](https://me.csdn.net/u012052268)阅读数：179
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)









# 常用类



- [常用类](#常用类)- [Arrays类](#arrays类)- [Arrays类概述](#arrays类概述)
- [成员方法](#成员方法)

- [基本类型包装类](#基本类型包装类)- [概述](#概述)
- [Integer类](#integer类)- [概述](#概述-1)
- [构造方法](#构造方法)
- [String和int的相互转换](#string和int的相互转换)
- [常用的基本进制转换](#常用的基本进制转换)

- [Character类](#character类)- [Character类概述](#character类概述)
- [Character类成员方法](#character类成员方法)
- [题目](#题目)







## Arrays类

### Arrays类概述

针对数组进行操作的工具类。 

提供了排序，查找等功能。

### 成员方法

```
public static String toString(int[] a)

public static void sort(int[] a)

public static int binarySearch(int[] a,int key)
```

## 基本类型包装类

### 概述

将基本数据类型封装成对象的好处在于可以在对象中定义更多的功能方法操作该数据。

常用的操作之一：用于基本数据类型与字符串之间的转换。

基本类型和包装类的对应

Byte , Short , Integer , Long , Float , Double , Character , Boolean

### Integer类

#### 概述

Integer 类在对象中包装了一个基本类型 int 的值

该类提供了多个方法，能在 int 类型和 String 类型之间互相转换，还提供了处理 int 类型时非常有用的其他一些常量和方法

#### 构造方法

```
public Integer(int value)

public Integer(String s)
```

#### String和int的相互转换

```
String --> int
    Integer.parseInt("100");

int --> String
    String.valueOf(100);
```

#### 常用的基本进制转换

```
public static String toBinaryString(int i)

public static String toOctalString(int i)

public static String toHexString(int i)
```

### Character类

#### Character类概述

Character 类在对象中包装一个基本类型 char 的值

此外，该类提供了几种方法，以确定字符的类别（小写字母，数字，等等），并将字符从大写转换成小写，反之亦然

#### Character类成员方法

```java
public static boolean isUpperCase(char ch):判断给定的字符是否是大写字符

public static boolean isLowerCase(char ch):判断给定的字符是否是小写字符

public static boolean isDigit(char ch):判断给定的字符是否是数字字符

public static char toUpperCase(char ch):把给定的字符转换为大写字符

public static char toLowerCase(char ch):把给定的字符转换为小写字符
```

#### 题目

> 
统计一个字符串中大写字母字符，小写字母字符，数字字符出现的次数。(不考虑其他字符)


```
public class CharacterTest {
    public static void main(String[] args) {
        // 定义三个统计变量。
        int bigCount = 0;
        int smallCount = 0;
        int numberCount = 0;

        // 键盘录入一个字符串。
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入一个字符串：");
        String line = sc.nextLine();

        // 把字符串转换为字符数组。
        char[] chs = line.toCharArray();

        // 历字符数组获取到每一个字符
        for (int x = 0; x < chs.length; x++) {
            char ch = chs[x];

            // 判断该字符
            if (Character.isUpperCase(ch)) {
                bigCount++;
            } else if (Character.isLowerCase(ch)) {
                smallCount++;
            } else if (Character.isDigit(ch)) {
                numberCount++;
            }
        }

        // 输出结果即可
        System.out.println("大写字母：" + bigCount + "个");
        System.out.println("小写字母：" + smallCount + "个");
        System.out.println("数字字符：" + numberCount + "个");
    }
}
```



