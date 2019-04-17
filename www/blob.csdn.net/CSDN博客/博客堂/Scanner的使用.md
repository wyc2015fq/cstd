# Scanner的使用 - 博客堂 - CSDN博客





2017年02月22日 22:29:13[最小森林](https://me.csdn.net/u012052268)阅读数：353








# Scanner的使用(了解)

## (1)概述

在JDK5以后出现的用于键盘录入数据的类。

## (2)构造方法：
- 常用的格式
`Scanner sc = new Scanner(System.in);`- 构造方法

```
InputStream is = System.in;

Scanner(InputStream is)
```
- System .in

讲解了System.in这个东西。它其实是标准的输入流,对应于键盘录入.in是System类的静态变量。

## (3)基本方法格式：
- hasNextXxx() 判断是否是某种类型的
- nextXxx() 返回某种类型的元素

## (4)要掌握的两个方法
- A:public int nextInt()    // 输入Int
- B:public String nextLine()    // 输入String

## (5)需要注意的小问题

> 
同一个Scanner对象，先获取数值，再获取字符串会出现一个小问题。


解决方案： 

- a:重新定义一个Scanner对象
- b:把所有的数据都用字符串获取，然后再进行相应的转换



