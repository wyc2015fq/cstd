# Override和Overload的区别 - fanyun的博客 - CSDN博客
2017年01月25日 09:13:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：279
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
其实很本质的区别就是看函数特征：
覆写（Override）的两个函数的函数特征相同重载（Overload）的两个函数的函数名虽然相同，但函数特征不同。函数特征包括函数名，参数的类型和个数。
Override
 是在继承的时候，如果你写的函数与要继承的函数函数特征相同，那么，加上这个关键字，在使用这个子类的这个函数的时候就看不见父类（或超类）的函数了，它被覆盖掉了。比如：Derived继承了Base，Base里面有void A(int a)那么如果你Derived里面觉得A写得不好或不适合这个类，你想重新再写一遍A里的代码，那么就写override void A(int a)这样，原来的那个函数就被你新写的这个覆盖掉了。
代码示例：
Overload
 是重载，就是说函数名相同，函数特征不同，系统会根据你提供的参数来调相应的函数。比如：void A(int a)和void A(int a,int b)如果你用的是A(1)那么调的是第一个，如果是A(1,1)那么调的是第二个。
代码示例：
