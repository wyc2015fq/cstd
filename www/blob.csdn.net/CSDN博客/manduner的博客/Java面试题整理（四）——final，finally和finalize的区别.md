
# Java面试题整理（四）——final，finally和finalize的区别 - manduner的博客 - CSDN博客


2018年08月24日 17:48:57[Manduner_TJU](https://me.csdn.net/manduner)阅读数：132


# 1，final
final：最终的意思，可以修饰类，成员变量，成员方法。
（1）修饰类，类不能被继承
（2）修饰变量，变量是常量
（3）修饰方法，方法不能被重写
# 2，finally
（1）finally是异常处理的一部分，用于释放资源。一般来说，代码肯定会执行，特殊情况：在执行到finally之前JVM退出了
（2）如果catch里面有return语句，finally里面的代码还是会执行，并且是在return前执行
# 3，finalize
是Object类的一个方法，用于垃圾回收

