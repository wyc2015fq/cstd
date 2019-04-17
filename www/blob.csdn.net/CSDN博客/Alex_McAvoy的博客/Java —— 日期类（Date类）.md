# Java —— 日期类（Date类） - Alex_McAvoy的博客 - CSDN博客





2018年10月11日 23:13:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：112
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









在程序设计中常会遇到日期、时间等数据，为便于将数据以相应形式显示，Java 中存在一日期类——Date 类，存于 java.lang 包内。

# 【无参构造器】

Data 类的无参构造器所创建的对象可以获取本机当前时间，表示时间的默认顺序是：星期、月、日、小时、分、秒、年

```java
Date date=new Date();
System.out.println(date);//输出当前时间
```

# 【有参构造器】

计算机系统自身时间是格林威治时间（1970年1月1日0时），可以根据这个时间 Date 有参构造器创建一个 Date 对象，参数取正表示公元后时间，参数取负表示公元前时间。

```java
Date date1=new Date(1000);//结果为：Thu Jan 01 08:00:01 CST 1970
Date date2=new Date(-1000);//结果为：Thu Jan 01 07:59:59 CST 1970
```

# 【格式化日期和时间】

当需要对日期、时间格式化时，可采用字符串中的 format() 方法来进行格式化

具体用法：[点击这里](https://blog.csdn.net/u011815404/article/details/83066711)



