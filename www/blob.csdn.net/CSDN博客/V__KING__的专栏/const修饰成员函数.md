# const修饰成员函数 - V__KING__的专栏 - CSDN博客





2013年10月11日 13:52:17[v__king__](https://me.csdn.net/V__KING__)阅读数：720










const修饰类的成员函数，则该成员函数不能修改类中任何非const成员函数。一般写在函数的最后来修饰。

    class A

    { 

        …

       void function()const; //常成员函数, 它不改变对象的成员变量.                        

//也不能调用类中任何非const成员函数。

}

对于const类对象/指针/引用，只能调用类的const成员函数，因此，const修饰成员函数的最重要作用就是限制对于const对象的使用。

a. const成员函数不被允许修改它所在对象的任何一个数据成员。

b. const成员函数能够访问对象的const成员，而其他成员函数不可以。



