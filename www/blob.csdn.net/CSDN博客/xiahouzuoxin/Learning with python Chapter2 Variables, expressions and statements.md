# Learning with python: Chapter2 Variables, expressions and statements - xiahouzuoxin - CSDN博客





2013年08月13日 20:49:45[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1425
个人分类：[ComputerLanuage](https://blog.csdn.net/xiahouzuoxin/article/category/1172320)









20个关键词：


and def exec if not return assert del finally import or try 
break elif for in pass while class else from is print yield continue except global lambda raise




**[root@CentOS python]# python**

Python 2.6.6 (r266:84292, Feb 21 2013, 23:54:59) 

[GCC 4.4.7 20120313 (Red Hat 4.4.7-3)] on linux2

Type "help", "copyright", "credits" or "license" for more information.
**>>> print 4  # use print**

4
**>>> type("Hello world!")  # use type()**

<type 'str'>
**>>> type(3.2)**

<type 'float'>
**>>> print 1,000,000   # use , operator printmulti-variables**

1 0 0
**>>> Message="What's up?"  # string>>> n=17>>> pi=3.1415926>>> print Message   # print string**

What's up?
**>>> print n**

17
**>>> print pi**

3.1415926
**>>> type(Message),type(n),type(pi)**

(<type 'str'>, <type 'int'>, <type 'float'>)
**>>> 1+1**

2
**>>> 17**

17
**>>> Message   # string, compare with "print string"**

"What's up?"
**>>> 5**2,4*3  # ** is exponent and * is multiply**

(25, 12)
**>>> minute=59>>> minute/60   # round off**

0


**>>> minute*100/60**

98**>>> 3*1**3    # ** is prior to ***

3
**>>> (3*1)**3**

27
**>>> print Message + "Ted."   # operate string**

What's up?Ted.
**>>> print Message*3  # print Message for 3 times**

What's up?What's up?What's up?

**>>> percentage=(minute*100)//60  # // is same as / as a division operator>>> percentage**

98
**>>> percentage=(minute*100)/60>>> percentage**

98






