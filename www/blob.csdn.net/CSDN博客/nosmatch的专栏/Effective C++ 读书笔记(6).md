# Effective C++ 读书笔记(6) - nosmatch的专栏 - CSDN博客
2013年07月08日 23:21:27[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：442
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款六：如果不想使用编译器自动生成的函数，就应该明确的拒绝(不要搞暧昧！)
> 
C++是一个比较复杂的语言，包含的模块比较多，我们写程序的一个原则就是不用的功能就要明确的禁止掉，这样就会防止
在意想不到的情况下出现调用，C++瞒着你在背后做了太多的事情了！
> 
对于一个独一无二的对象，例如前面的People对象，每个人都是唯一的，因此我们不想在People类中出现copy构造函数和
copy assigment函数，对于普通的函数，我们不会定义就不会出现相应的调用，但是这两个函数如果你不定义，编译器会默认的为你
定义，我们为了禁止编译器默认的对应函数，我们可以声明自己的函数，并且定义为private权限，这样一般情况下就不会被调用到，
但是对于类中的成员函数和friend函数还是有调用该函数的权限，跟狠一点，我们只声明，不做任何的定义，这样如果对这些函数进行
对应的调用一定会出现链接错误，例如：
> 
```cpp
class People{
private:
    People(const People&);
    const People& operator=(const People&);
    ......
}
```
还有一种做法就是写一个不能进行赋值或者复制的父类，然后继承她！例如：
```cpp
class Uncopyable{
protected:
    Uncopyable(){}
    ~Uncopyable(){}
private:
    Uncopy(const Uncopyable&);
    const Uncopyable(const Uncopyable&);
};
```
只要是继承自Uncopyable类的对象都是不能包含赋值或者复制构造函数的，在boost库中就存在类似的父类noncopyable
请记住：
- 为驳回编译器自动提供的技能，可以将相应的成员函数声明为private并且不予实现，像Uncopyable也是一种很好的做法
> 
