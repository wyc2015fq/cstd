
# Java实用方法整理（十二）——异常 - manduner的博客 - CSDN博客


2018年08月24日 10:11:04[Manduner_TJU](https://me.csdn.net/manduner)阅读数：36标签：[Java																](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[Exception																](https://so.csdn.net/so/search/s.do?q=Exception&t=blog)[异常																](https://so.csdn.net/so/search/s.do?q=异常&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Exception&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)
[
																								](https://so.csdn.net/so/search/s.do?q=Java&t=blog)


# 1, 异常分类
异常通常分为三类，如下图所示：
![](https://img-blog.csdn.net/20180824094719342?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**注 ：**（1）编译期的异常，必须进行处理，并且编译期抛出的异常，将来的调用者必须处理
（2）运行期的异常，不用必须处理，但是也可以进行处理
（3）编译期的异常，如果不处理，程序根本没法运行，所以不管用啥IDE，那个IDE工具肯定会报错；而运行期间的异常，只有运行起来才会出问题，而且一般可以通过修改完善代码来解决异常问题
# 2，为什么要处理异常？
开发当中，通常会遇到一些“运行期间的异常”即RuntimeExcetion或者其他异常，这一类问题通常是代码书写不当或者用户操作不当造成的，需要修改代码或给出用户相关的操作提示，而不想由于这个异常点就终止整个程序的运行。
如果遇到异常不去处理的话，JVM即Java虚拟机会做出默认的处理，此时，整个程序会被终止运行，如下图所示：
![](https://img-blog.csdn.net/20180824100512761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 3，处理异常的方法
处理异常通常有三种方法：
（1）try...catch...finally
（2）try...catch...
（3）throws 抛出
方法的具体讲解会在下面的章节中给出

