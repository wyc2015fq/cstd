# 关于【error C3646: 未知重写说明符】的若干种可能性 - 文章 - 伯乐在线
原文出处： [Scobbing，2016-12-25](http://www.cnblogs.com/scobbing/p/6219843.html)
如果在程序中出现了error C3646，那么这个错误可能是由多种问题所导致的，这篇文章将对多种错误及对应解决方法进行分析。
在MSDN的官方网站中，Complier Error C3646的定义为unknown override specifier，并且其解释为The compiler found a token in the position where it expected to find an override specifier, but the token was not recognized by the compiler.
1.循环引用
在分离式编译中，我们往往习惯将不同的类或者函数分别写在不同的函数中，提高工作效率。但如果在使用时出现两个类分别在两个不同的文件中编写，并且相互引用，则会出现循环引用，引发此错误。
举例：tire.h/tire.cpp和auto.h/auto.cpp 在tire.h中include auto.h，并定义了一个auto的object，然后在auto.h中又include了tire.h
解决方案：在报错的那个.h文件的类定义前加上class声明调用的类（即class 类）。
2.语法错误
此问题大多因为粗心而导致，比如在某个成员函数中把分号;写成了中文分号；，将会出现此类错误。
解决方案：修改分号
3.类的顺序错误
如果一个类的成员中包含另一个类，则被包含的类的生命一定要在包含的类之前。
举例：

```
class Auto {
      Tire tire;
}
 
class Tire {
      int weight;  
}
 
// This will cause error C3656
```
解决方法：调整类的位置。
在遇到这种问题的时候，我们需要透过现象看本质。
