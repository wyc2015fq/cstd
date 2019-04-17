# [CareerCup] 14.3 Final Finally Finalize 关键字比较 - Grandyang - 博客园







# [[CareerCup] 14.3 Final Finally Finalize 关键字比较](https://www.cnblogs.com/grandyang/p/4946917.html)







14.3 What is the difference between final, finally, and finalize?



这道题考察我们Java中的三个看起来很相似的关键字final，finally和finalize。

别看它们三长的很像，但是完全不是一回事。final用在一个变量，方法或是类之前表示是否可变化。而finally是用在try/catch模块中来却表一段代码总是被执行了。finalize方法是被垃圾管理器调用当没有引用存在了。下面来更进一步区分它们：

final

作用于主要类型的变量：变量值不能改变

作用于引用类型的变量：引用变量不能指向堆中其他的对象

作用于一个方法：方法不能被重写

作用于一个类：该类不能有子类

finally

finally块可以为try或catch之后，里面的代码总会被执行（除了虚拟机或者线程在try/catch中就结束）

finalize()

finalize()方法被垃圾回收机制调用，当没有引用存在时。经常备用于清理资源，比如关闭一个文件。












