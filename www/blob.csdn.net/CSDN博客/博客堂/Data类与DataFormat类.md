# Data类与DataFormat类 - 博客堂 - CSDN博客





2017年03月01日 19:43:35[最小森林](https://me.csdn.net/u012052268)阅读数：2437








# Data类与DataFormat类的用法



- [Data类与DataFormat类的用法](#data类与dataformat类的用法)- [Data类](#data类)- [概述](#概述)
- [常用方法](#常用方法)

- [DataFormat类](#dataformat类)- [概述](#概述-1)
- [SimpleDateFormat的构造方法](#simpledateformat的构造方法)
- [常用方法](#常用方法-1)
- [试题](#试题)






## Data类

### 概述

Date类:表示特定的瞬间，精确到毫秒。 

**构造方法：**

Date():根据当前的默认毫秒值创建日期对象

Date(long date)：根据给定的毫秒值创建日期对象

```
public class Test {
    public static void main(String[] args) {
        // 创建对象
        Date d = new Date();
        System.out.println("d:" + d);  // 结果是：d:Wed Mar 01 19:19:12 CST 2017

        // 创建对象
        // long time = System.currentTimeMillis();
        long time = 1000 * 60 * 60; // 1小时
        Date d2 = new Date(time);
        System.out.println("d2:" + d2);  // 结果是：d2:Thu Jan 01 09:00:00 CST 1970
    }
}
```

### 常用方法

Data类成员方法

getTime() ：从Date得到一个毫秒值

setTime(long time)：把一个毫秒值转换为Date

```
public class DateDemo {
    public static void main(String[] args) {
        // 创建对象
        Date d = new Date();

        // 获取时间
        long time = d.getTime();
        System.out.println(time);
        // System.out.println(System.currentTimeMillis());

        System.out.println("d:" + d);
        // 设置时间
        d.setTime(1000);
        System.out.println("d:" + d);
    }
}
```

## DataFormat类

### 概述：

针对日期进行格式化和针对字符串进行解析的类，

```java
Date    -->     String(格式化)
String  -->     Date(解析)
```

但是DataFormat类是抽象类，所以使用其子类SimpleDateFormat

### SimpleDateFormat的构造方法：

> 
SimpleDateFormat():默认模式 

  SimpleDateFormat(String pattern):给定的==模式==


这个==模式==字符串该如何写呢? 

通过查看API，我们就找到了对应的模式

```
年 y
月 M 
日 d
时 H
分 m
秒 s
```

例如： 

2014年12月12日 12:12:12的模式为： 

yyyy年MM月dd日 HH:mm:ss
### 常用方法

实现Date – String的快速转化 

1. public final String format(Date date) ： 将给定的 Date 格式化为日期/时间字符串。 

2. public Date parse(String source) ：  解析字符串的文本，生成 Date。
```
public class DateFormatDemo {
    public static void main(String[] args) throws ParseException {
        // Date -- String
        // 创建日期对象
        Date d = new Date();
        // 创建格式化对象
        // SimpleDateFormat sdf = new SimpleDateFormat();
        // 给定模式
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy年MM月dd日 HH:mm:ss");
        // public final String format(Date date)
        String s = sdf.format(d);
        System.out.println(s);


        //String -- Date
        String str = "2008-08-08 12:12:12";
        //在把一个字符串解析为日期的时候，请注意格式必须和给定的字符串格式匹配
        SimpleDateFormat sdf2 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        Date dd = sdf2.parse(str);
        System.out.println(dd);
    }
}
```

### 试题

> 
用DataFormat算一下你来到这个世界多少天?


（不推荐用这样的方法，下一节讲解 日历类）

分析： 

- A:键盘录入你的出生的年月日 

- B:把该字符串转换为一个日期 

- C:通过该日期得到一个毫秒值 

- D:获取当前时间的毫秒值 

- E:用D-C得到一个毫秒值 

- F:把E的毫秒值转换为年。除以( /1000/60/60/24 )
```
public class MyYearOldDemo {
    public static void main(String[] args) throws ParseException {
        // 键盘录入你的出生的年月日
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入你的出生年月日:");
        String line = sc.nextLine();

        // 把该字符串转换为一个日期
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Date d = sdf.parse(line);

        // 通过该日期得到一个毫秒值
        long myTime = d.getTime();

        // 获取当前时间的毫秒值
        long nowTime = System.currentTimeMillis();

        // 用D-C得到一个毫秒值
        long time = nowTime - myTime;

        // 把E的毫秒值转换为年
        long day = time / 1000 / 60 / 60 / 24;

        System.out.println("你来到这个世界：" + day + "天");
    }
}
```






