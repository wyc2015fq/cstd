# Java中的内部类 - bigfacesafdasgfewgf - CSDN博客





2015年03月12日 14:50:51[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：609标签：[内部类																[宿主类																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)









内部类是定义在其他类内部的类，内部类所在的类成为宿主类。由于内部类只能被它的宿主类使用，所以通过内部类的使用可以很好地控制类的可见性。



内部类有如下几个特性：

1） 内部类分为两大类：静态内部类和非静态内部类；

2） 非静态内部类可以调用它的宿主类的所有变量和方法，并且像宿主类的其他非静态成员那样直接引用它们；

3） 静态内部类是用static修饰，不能直接访问其宿主类的成员，必须通过宿主类的对象来访问；

4） 一般内部类都定义在宿主类之中；如果定义在宿主类的方法中，那就是局部内部类。](https://so.csdn.net/so/search/s.do?q=宿主类&t=blog)](https://so.csdn.net/so/search/s.do?q=内部类&t=blog)




