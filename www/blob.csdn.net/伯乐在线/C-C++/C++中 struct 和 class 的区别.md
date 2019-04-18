# C++中 struct 和 class 的区别 - 文章 - 伯乐在线
原文出处： [LoveApp_Han](http://blog.csdn.net/xdrt81y/article/details/17143801)
#### 一、
C++中的struct对C中的struct进行了扩充，它已经不再只是一个包含不同数据类型的数据结构了，它已经获取了太多的功能。
- struct能包含成员函数吗？   能！
- struct能继承吗？          能！！
- struct能实现多态吗？       能！！！
最本质的一个区别就是默认的访问控制，体现在两个方面：
1）默认的继承访问权限。struct是public的，class是private的。
写如下的代码：


```
struct A
    {
       char a;
    };
    struct B : A
    {
       char b;
    };
```
这个时候B是public继承A的。如果都将上面的struct改成class，那么B是private继承A的。这就是默认的继承访问权限。所以我们在平时写类继承的时候，通常会这样写：


```
struct B : public A
```
就是为了指明是public继承，而不是用默认的private继承。
当然，到底默认是public继承还是private继承，取决于子类而不是基类。我的意思是，struct可以继承class，同样class也可以继承struct，那么默认的继承访问权限是看子类到底是用的struct还是class。如下：


```
struct A{};
class B : A{};    //private继承
struct C : B{};  //public继承
```
2）struct作为数据结构的实现体，它默认的数据访问控制是public的，而class作为对象的实现体，它默认的成员变量访问控制是private
3)“class”这个关键字还用于定义模板参数，就像“typename”。但关键字“struct”不用于定义模板参数。
4) 还是上面所说的，C++中的struct是对C中的struct的扩充，既然是扩充，那么它就要兼容过去C中struct应有的所有特性。例如你可以这样写：


```
struct A    //定义一个struct
{
  char c1;
  int  n2;
  double db3;
};
A a={'p',7,3.1415926};  //定义时直接赋值
```
也就是说struct可以在定义的时候用{}赋初值。
向上面的struct中加入一个构造函数（或虚函数），struct也不能用{}赋初值了。的确，以{}的方式来赋初值，只是用一个初始化列表来对数据进行按顺序的初始化，如上面如果写成A a={‘p’,7};则c1,n2被初始化，而db3没有。这样简单的copy操作，只能发生在简单的数据结构上，而不应该放在对象上。加入一个构造函数或是一个虚函数会使struct更体现出一种对象的特性，而使此{}操作不再有效。事实上，是因为加入这样的函数，使得类的内部结构发生了变化。而加入一个普通的成员函数呢？你会发现{}依旧可用。其实你可以将普通的函数理解成对数据结构的一种算法，这并不打破它数据结构的特性。至于虚函数和普通成员函数有什么区别，我会具体写篇文章讨论。
那么，看到这里，我们发现即使是struct想用{}来赋初值，它也必须满足很多的约束条件，这些条件实际上就是让struct更体现出一种数据机构而不是类的特性。那为什么我们在上面仅仅将struct改成class，{}就不能用了呢？其实问题恰巧是我们之前所讲的——访问控制！你看看，我们忘记了什么？对，将struct改成class的时候，访问控制由public变为private了，那当然就不能用{}来赋初值了。加上一个public，你会发现，class也是能用{}的，和struct毫无区别！！！
从上面的区别，我们可以看出，struct更适合看成是一个数据结构的实现体，class更适合看成是一个对象的实现体。
#### 二、
**关于使用大括号初始化**
- class和struct如果定义了构造函数的话，都不能用大括号进行初始化
- 如果没有定义构造函数，struct可以用大括号初始化。
- 如果没有定义构造函数，且所有成员变量全是public的话，可以用大括号初始化。
关于默认访问权限
- class中默认的成员访问权限是private的，而struct中则是public的。
**关于继承方式**
- class继承默认是private继承，而struct继承默认是public继承。
且看如下代码（看看编译器给出的错误信息）：


```
class T1
{
　public:
　　void f()
　　{
　　　cout<<"T1::f()"<<endl;
　　}
　int x,y;
};
struct T2
{
　int x;
　void f(){cout<<"T2::f()"<<endl;}
};
struct TT1 : T1
{
};
class TT2 : T2
{
};
int main()
{
　TT1 t1;
　TT2 t2;
　t1.f();
　t2.f();
}
```
**关于模版**
在模版中，类型参数前面可以使用class或typename，如果使用struct，则含义不同，struct后面跟的是“non-type template parameter”，而class或typename后面跟的是类型参数。


```
template <struct X>
void f(X x)
{
}
//出错信息：d:codecpptestcpptestcpptest.cpp(33) : error C2065: 'X' : undeclared identifier
```
