# C++中的重载、覆盖和隐藏的区分 - bigfacesafdasgfewgf - CSDN博客





2015年04月12日 21:01:09[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：557标签：[重载																[覆盖																[隐藏																[多态性																[继承](https://so.csdn.net/so/search/s.do?q=继承&t=blog)
个人分类：[C/C++](https://blog.csdn.net/puqutogether/article/category/2595123)





C++中经常用到重载、覆盖和隐藏这三个概念，也经常容易搞混。所以，这篇博文来整理一下它们之间的异同。

**1. 重载——实现多态性**

A．相同的范围（同一个类中） 

B．函数的名字相同 

C．参数类型不同（不能进行隐式类型转换） 

D．Virtual关键字可有可无
**2. 覆盖（重写）——派生类函数覆盖基类函数**

A．不同的范围（分别位于基类与派生类中） 

B．函数名字相同 

C．参数相同 

D．基类函数必须有virtual关键字
**3. 隐藏（重定义）——派生类的函数屏蔽了与其同名的基类函数**

 A．如果派生类的函数与基类的函数同名，但是参数不同，此时不论有无virtual关键字，基类的函数都将被隐藏，注意别与重载混淆） 

B．如果派生类的函数与基类的函数同名，并且参数也相同，但是基类函数没有virtual关键字，此时基类的函数被隐藏（注意别与覆盖混淆）](https://so.csdn.net/so/search/s.do?q=多态性&t=blog)](https://so.csdn.net/so/search/s.do?q=隐藏&t=blog)](https://so.csdn.net/so/search/s.do?q=覆盖&t=blog)](https://so.csdn.net/so/search/s.do?q=重载&t=blog)




