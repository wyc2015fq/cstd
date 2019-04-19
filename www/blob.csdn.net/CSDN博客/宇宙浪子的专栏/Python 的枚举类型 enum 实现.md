# Python 的枚举类型 enum 实现 - 宇宙浪子的专栏 - CSDN博客
2014年12月31日 09:55:42[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1326
# [Python 的枚举类型 enum 实现](http://blog.csdn.net/horin153/article/details/6681052)
 Python 的枚举类型 enum 实现
 C 语言中有个枚举类型 enum，很适合在定义一系列相关的常量时，由程序自动为常量分配 id，解决人工分配 id 可能导致的 id 冲突问题。在 Python 这种脚本语言中，当定义大量相关的常量时，如果由程序员人工分配，要么容易 id 冲突，要么不好进行 id 分段管理。其实，可以很简单地实现 Python 版的 enum（当然是没有类型检测的 enum 版本）。
 实现代码如下：
**[python]**[view
 plain](http://blog.csdn.net/horin153/article/details/6681052#)[copy](http://blog.csdn.net/horin153/article/details/6681052#)
- def enum(module, str_enum, sep=None):  
- """把用特定分隔符隔开的 str_enum 字符串实现为 module 的枚举值.
-     @param module 宿主对象, 可以是 module, class, ...
-     eg1: enum(test, "A B C") => test.A,test.B,test.C == 0,1,2
-     eg2: enum(CTest, 'one=1,two,three,ten=0x0A,eleven', sep=',') =>
-         CTest.one,CTest.two,CTest.ten, CTest.eleven == 1,2,10,11
-     """
-     idx = 0
- for name in str_enum.split(sep):  
- if'='in name:  
-             name,val = name.rsplit('=', 1)  
- if val.isalnum():  
-                 idx = eval(val)  
-         setattr(module, name.strip(), idx)  
-         idx += 1
- 
