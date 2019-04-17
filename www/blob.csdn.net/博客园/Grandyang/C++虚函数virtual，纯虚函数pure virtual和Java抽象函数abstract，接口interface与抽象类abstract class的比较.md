# C++虚函数virtual，纯虚函数pure virtual和Java抽象函数abstract，接口interface与抽象类abstract class的比较 - Grandyang - 博客园







# [C++虚函数virtual，纯虚函数pure virtual和Java抽象函数abstract，接口interface与抽象类abstract class的比较](https://www.cnblogs.com/grandyang/p/4782575.html)







由于C++和Java都是面向对象的编程语言，它们的多态性就分别靠虚函数和抽象函数来实现。

C++的虚函数可以在子类中重写，调用是根据实际的对象来判别的，而不是通过指针类型(普通函数的调用是根据当前指针类型来判断的)。纯虚函数是一种在父函数中只定义而不实现的一种函数，不能用来声明对象，也可以被称为抽象类。纯虚函数的实现也可以在类声明外进行定义。C++中的抽象类abstract class是指至少有一个纯虚函数的类，如果一个类全部由纯虚函数组成，不包括任何的实现，被称为纯虚类。

Java中的普通函数自带虚函数功能，调用是根据指针所指向的对象的类型进行判断的。Java中没有virtual这个关键字，java管虚函数叫abstract function，管抽象类叫做abstract class，没有pure这个概念，而是发明了一种叫接口interface的东西来代替纯虚类。interface和abstract class的区别就类似于C++中一般抽象类和纯虚类的区别。

抽象类只能作为基类来使用，不能被定义对象，其纯虚函数的实现在派生类中，如果派生类也没有给出实现，则该派生类还是一个抽象类，只有给出了纯虚函数实现的派生类才能建立对象。由此看出抽象类是注孤生的节奏啊，永远没有对象。

其他方面的比较请参见这篇博客[C++虚函数与JAVA中抽象函数比较 接口与抽象类比较](http://blog.csdn.net/yangliuy/article/details/7172228)。














