# C++中的函数指针和函数对象总结 - DoubleLi - 博客园






**篇一、函数指针**
**函数指针**：是指向函数的指针变量，在C编译时，每一个函数都有一个入口地址，那么这个指向这个函数的函数指针便指向这个地址。
函数指针的用途是很大的，主要有两个作用：用作调用函数和做函数的参数。
**函数指针的声明方法**：
数据类型标志符 （指针变量名） （形参列表）；
一般函数的声明为: 
int func ( int x );
而一个函数指针的声明方法为：
int (*func) (int x);
前面的那个(*func)中括号是必要的，这会告诉编译器我们声明的是函数指针而不是声明一个具有返回型为指针的函数，后面的形参要视这个函数指针所指向的函数形参而定。
然而这样声明我们有时觉得非常繁琐，于是typedef可以派上用场了，我们也可以这样声明：
typedef int (*PF) (int x);
PF pf;
这样pf便是一个函数指针，方便了许多。当要使用函数指针来调用函数时，func(x)或者  (*fucn)(x) 就可以了，当然，函数指针也可以指向被重载的函数，编译器会为我们区分这些重载的函数从而使函数指针指向正确的函数。
例子：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

typedef void (*PFT) ( char ,int );
void bar(char ch, int i)
{
    cout<<"bar "<<ch<<' '<<i<<endl;
    return ;
}
PFT pft;
pft = bar;
pft('e',91);

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```


例子中函数指针pft指向了一个已经声明的函数bar()，然后通过pft来实现输出字符和整型的目的。
函数指针另一个作用便是作为函数的参数，我们可以在一个函数的形参列表中传入一个函数指针，然后便可以在这个函数中使用这个函数指针所指向的函数，这样便可以使程序变得更加清晰和简洁，而且这种用途技巧可以帮助我们解决很多棘手的问题，使用很小的代价就可获得足够大的利益（速度+复杂度）。

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

typedef void (*PFT) ( char ,int );
void bar(char ch, int i)
{
    cout<<"bar "<<ch<<' '<<i<<endl;
    return ;
}
void foo(char ch, int i, PFT pf)
{
    pf(ch,i);
    return ;
}
PFT pft;
pft = bar;
foo('e',12,pft);

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```


上述例子我们首先利用一个函数指针pft指向bar()，然后在foo()函数中使用pft指针来调用bar()，实现目的。将这个特点稍加利用，我们就可以构造出强大的程序，只需要同样的foo函数便可以实现对不同bar函数的调用。

**篇二、函数对象**
前面是函数指针的应用，从一般的函数回调意义上来说，函数对象和函数指针是相同的，但是函数对象却具有许多函数指针不具有的有点，函数对象使程序设计更加灵活，而且能够实现函数的内联（inline）调用，使整个程序实现性能加速。
**函数对象**：这里已经说明了这是一个对象，而且实际上只是这个对象具有的函数的某些功能，我们才称之为函数对象，意义很贴切，如果一个对象具有了某个函数的功能，我们变可以称之为函数对象。
如何使对象具有函数功能呢，很简单，只需要为这个对象的操作符()进行重载就可以了，如下：

```
class A{
public:
int operator()(int x){return x;}
};
A a;
a(5);
```


这样a就成为一个函数对象，当我们执行a(5)时，实际上就是利用了重载符号()。
函数对象既然是一个“类对象”，那么我们当然可以在函数形参列表中调用它，它完全可以取代函数指针！如果说指针是C的标志，类是C++特有的，那么我们也可以说指针函数和函数对象之间的关系也是同前者一样的！（虽然有些严密）。当我们想在形参列表中调用某个函数时，可以先声明一个具有这种函数功能的函数对象，然后在形参中使用这个对象，他所作的功能和函数指针所作的功能是相同的，而且更加安全。
下面是一个例子：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

class Func{
public:
    int operator() (int a, int b)
    {
        cout<<a<<'+'<<b<<'='<<a+b<<endl;
        return a;
    }
};
int addFunc(int a, int b, Func& func)
{
    func(a,b);
    return a;
}
Func func;
addFunc(1,3,func);

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```


上述例子中首先定义了一个函数对象类，并重载了()操作符，目的是使前两个参数相加并输出，然后在addFunc中的形参列表中使用这个类对象，从而实现两数相加的功能。
如果运用泛型思维来考虑，可以定一个函数模板类，来实现一般类型的数据的相加：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

class FuncT{
public:
    template<typename T>
    T operator() (T t1, T t2)
    {
        cout<<t1<<'+'<<t2<<'='<<t1+t2<<endl;
        return t1;
    }
};
template <typename T>
T addFuncT(T t1, T t2, FuncT& funct)
{
    funct(t1,t2);
    return t1;
}
FuncT funct;
addFuncT(2,4,funct);
addFuncT(1.4,2.3,funct);

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```


大名鼎鼎的STL中便广泛的运用了这项技术，详细内容可参见候捷大师的一些泛型技术的书籍，不要以为函数对象的频繁调用会使程序性能大大折扣，大量事实和实验证明，正确使用函数对象的程序要比其他程序性能快很多！所以掌握并熟练运用函数对象才能为我们的程序加分，否则.......
如此看来，函数对象又为C++敞开了一道天窗，但随之而来的便是一些复杂的问题和陷阱，如何去蔽扬利还需要我们不断学习和探索。









