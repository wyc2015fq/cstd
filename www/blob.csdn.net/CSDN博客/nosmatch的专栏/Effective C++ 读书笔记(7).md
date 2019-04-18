# Effective C++ 读书笔记(7) - nosmatch的专栏 - CSDN博客
2013年07月10日 00:05:19[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：478
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款七：为多态的基类声明virtual析构函数
> 
任何的类都会有构造函数和析构函数，当你没有显示的定义的时候编译器会给你定义默认的构造和析构函数，普通的类
也就是不做任何类的父类的类，其对应函数的定义与其他成员函数的定义是相类似的，对于基类，由于基类的属性要被对应的子类
继承，因此也会有不同的属性！在父类中，如果存在任何的virtual虚函数，那么这个类的析构函数也最好声明为virtual类型，不然会
有不确定的问题，virtual析构函数主要是与多态相对应的，当父类的指针指向子类的对象的时候，对指针delet时，会主动的调用
父类的虚构函数，如果父类的析构函数不是virtual的，那么只有父类的析构函数得到调用，子类的析构函数没有调用，对应子类的
成员也得不到释放，这样会造成内存泄露，例如：
> 
```cpp
class People{
public:
	People(){....}
	virtual ~People(){....}
	.....
};
class Student : public People{
	.....
};
People* p = new Student();
...
delete p;
```
如果People中的析构函数不是virtual的，那么在delete p的时候，子类中的析构函数就不能调用，子类中的资源也不能释
放。
> 
虽然virtual 析构函数有好处，但是不能把所有类的析构函数都定义为virtual的，因为这是需要代价的，这将会使类的对象
的存储空间加大，因为要占用一个指针的空间，指向虚函数的列表！
> 
此外需要注意的是不能继承析构函数为非虚函数的类，例如STL中的所有的容器类，string类等这都可能会在某一个时刻
造成让你意想不到的错误；
> 
如果需要一个抽象对象，但是没有其他的pure virtual的函数的时候，可以将对象的析构函数写为纯虚构函数！例如：
```cpp
class AWON{
public：
    virtual ~AWON()=0
};
```
请记住：
- 带多态性质的base class都应该定义virtual析构函数，如果一个class有任何的virtual函数，那么他也要有一个virtual析构函数
- 如果一个class不是为了作为一个多态作用，或者一个base class来使用，那么就不要定义virtual 析构函数！

