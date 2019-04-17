# 【java】反射机制 - zkq_1986的博客 - CSDN博客





2016年07月18日 18:46:05[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：206
个人分类：[程序设计语言](https://blog.csdn.net/zkq_1986/article/category/6534515)









1 反射机制


什么是反射（Reflection）?其他语言有这种特点吗？

答：反射使得应用具有自描述性和自控性，相应系统变得更开放。

java通过Class c1 = Class.forName("className")载入指定的类，通过Class类的相关方法（getDeclaredMethod(), getDeclaredFields(), getDeclaredConstructs()）获取相应信息。

为什么要用反射，目的就是为了使系统变得更开放。

除了java, LEAD/LEAD++, OpenC++, MetaXa等语言都使用了反射机制。




