# Calender类 - 博客堂 - CSDN博客





2017年03月01日 20:07:17[最小森林](https://me.csdn.net/u012052268)阅读数：257








# Calender类


- [Calender类](#calender类)- [概述](#1-概述)
- [使用](#2-使用)
- [操作该类](#3操作该类)
- [练习](#练习)




## 1. 概述

日历类，封装了所有的日历字段值，通过统一的方法根据传入不同的日历字段可以获取值。

它为特定瞬间与一组诸如 YEAR、MONTH、DAY_OF_MONTH、HOUR 等 日历字段之间的转换提供了一些方法，并为操作日历字段（例如获得下星期的日期）提供了一些方法（get方法）。

> 
注意：Calender类的12个月 由0-11表示，即“0”表示1月


## 2. 使用

因为他是抽象类public abstract class Calendar 

所以使用 多态 来调用子类对象

public int get(int field):返回给定日历字段的值。日历类中的每个日历字段都是静态的成员变量，并且是int类型。

```
public class CalendarDemo {
    public static void main(String[] args) {
        // 其日历字段已由当前日期和时间初始化：
        Calendar rightNow = Calendar.getInstance(); // 子类对象

        // 获取年
        int year = rightNow.get(Calendar.YEAR);
        // 获取月
        int month = rightNow.get(Calendar.MONTH);
        // 获取日
        int date = rightNow.get(Calendar.DATE);

        System.out.println(year + "年" + (month + 1) + "月" + date + "日");
    }
}
```

## 3.操作该类

public void **add**(int field,int amount):根据给定的日历字段和对应的时间，来对当前的日历进行操作。

public final void **set**(int year,int month,int date):设置当前日历的年月日

```
public class CalendarDemo {
    public static void main(String[] args) {
        // 获取当前的日历时间
        Calendar c = Calendar.getInstance();// 多态获取子类对象

        // 获取年
        int year = c.get(Calendar.YEAR);
        // 获取月
        int month = c.get(Calendar.MONTH);
        // 获取日
        int date = c.get(Calendar.DATE);
        System.out.println(year + "年" + (month + 1) + "月" + date + "日");

        // // 三年前的今天
        // c.add(Calendar.YEAR, -3);
        // // 获取年
        // year = c.get(Calendar.YEAR);
        // // 获取月
        // month = c.get(Calendar.MONTH);
        // // 获取日
        // date = c.get(Calendar.DATE);
        // System.out.println(year + "年" + (month + 1) + "月" + date + "日");

        // 5年后的10天前
        c.add(Calendar.YEAR, 5);
        c.add(Calendar.DATE, -10);
        // 获取年
        year = c.get(Calendar.YEAR);
        // 获取月
        month = c.get(Calendar.MONTH);
        // 获取日
        date = c.get(Calendar.DATE);
        System.out.println(year + "年" + (month + 1) + "月" + date + "日");
        System.out.println("--------------");

        c.set(2011, 11, 11);
        // 获取年
        year = c.get(Calendar.YEAR);
        // 获取月
        month = c.get(Calendar.MONTH);
        // 获取日
        date = c.get(Calendar.DATE);
        System.out.println(year + "年" + (month + 1) + "月" + date + "日");
    }
}
```

## 练习:

> 
获取任意一年的二月有多少天


分析：

```
A:键盘录入任意的年份
B:设置日历对象的年月日
    年就是A输入的数据
    月是2
    日是1
C:把时间往前推一天，就是2月的最后一天
D:获取这一天输出即可
```

```java
public class CalendarTest {
    public static void main(String[] args) {
        // 键盘录入任意的年份
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入年份：");
        int year = sc.nextInt();

        // 设置日历对象的年月日
        Calendar c = Calendar.getInstance();
        c.set(year, 2, 1); // 其实是这一年的3月1日
        // 把时间往前推一天，就是2月的最后一天
        c.add(Calendar.DATE, -1);

        // 获取这一天输出即可
        System.out.println(c.get(Calendar.DATE));
    }
}
```



