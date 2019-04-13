
# 在Python中处理字符串之isdigit()方法的使用​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年07月24日 13:57:02[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：80


**[在Python中处理字符串之isdigit()方法的使用](http://cda.pinggu.org/view/26182.html)**
这篇文章主要介绍了在Python中处理字符串之isdigit()方法的使用,是Python入门学习中的基础知识,需要的朋友可以参考下
isdigit()方法检查字符串是否只包含数字(全由数字组成)。
语法
以下是isdigit()方法的语法：
str.isdigit()
参数
NA
返回值
如果字符串中的所有字符都是数字，并至少有一个字符此方法返回true，否则返回false。
例子
下面的例子显示了isdigit()方法的使用。
\#!/usr/bin/python
str = "123456"; \# Only digit in this string
print str.isdigit();
str = "this is string example....wow!!!";
print str.isdigit();
当我们运行上面的程序，它会产生以下结果：
True
False


