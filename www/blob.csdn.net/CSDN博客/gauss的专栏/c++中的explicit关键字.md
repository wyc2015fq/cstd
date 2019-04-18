# c++中的explicit关键字 - gauss的专栏 - CSDN博客
2013年01月08日 13:12:02[gauss](https://me.csdn.net/mathlmx)阅读数：158
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)
c++中的explicit关键字
c++中的explicit关键字用来修饰类的构造函数，表明该构造函数是显式的，既然有"显式"那么必然就有"隐式"，那么什么是显示而什么又是隐式的呢？
如果c++类的构造函数有一个参数，那么在编译的时候就会有一个缺省的转换操作：将该构造函数对应数据类型的数据转换为该类对象，如下面所示： 
class MyClass
{
public:
MyClass( int num );
}
....
MyClass obj = 10; //ok,convert int to MyClass
在上面的代码中编译器自动将整型转换为MyClass类对象，实际上等同于下面的操作： 
MyClass temp(10);
MyClass obj = temp;
上面的所有的操作即是所谓的"隐式转换"。
如果要避免这种自动转换的功能，我们该怎么做呢？嘿嘿这就是关键字explicit的作用了，将类的构造函数声明为"显示"，也就是在声明构造函数的时候前面添加上explicit即可，这样就可以防止这种自动的转换操作，如果我们修改上面的MyClass类的构造函数为显示的，那么下面的代码就不能够编译通过了，如下所示：
class MyClass
{
public:
explicit MyClass( int num );
}
....
MyClass obj = 10; //err,can't non-explict convert
