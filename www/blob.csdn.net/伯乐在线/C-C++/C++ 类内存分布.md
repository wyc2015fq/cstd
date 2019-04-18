# C++ 类内存分布 - 文章 - 伯乐在线
原文出处： [Jerry19880126](http://www.cnblogs.com/jerry19880126/p/3616999.html)
书上类继承相关章节到这里就结束了，这里不妨说下C++内存分布结构，我们来看看编译器是怎么处理类成员内存分布的，特别是在继承、虚函数存在的情况下。
工欲善其事，必先利其器，我们先用好Visual Studio工具，像下面这样一步一步来：
![](http://jbcdn2.b0.upaiyun.com/2016/12/579064390fc4bde7d6d2691e41779989.png)
![](http://jbcdn2.b0.upaiyun.com/2016/12/7e00da260875a4b66a388d8920cf2b42.png)
先选择左侧的C/C++->命令行，然后在其他选项这里写上/d1 reportAllClassLayout，它可以看到所有相关类的内存布局，如果写上/d1 reportSingleClassLayoutXXX（XXX为类名），则只会打出指定类XXX的内存布局。近期的VS版本都支持这样配置。
下面可以定义一个类，像下面这样：

```
class Base
{
    int a;
    int b;
public:
    void CommonFunction();
};
```
然后编译一下，可以看到输出框里面有这样的排布：
![](http://jbcdn2.b0.upaiyun.com/2016/12/6a64d970246b694929d6026af9748cef.png)
这里不想花精力在内存对齐因素上，所以成员变量都设为int型。
从这里可以看到普通类的排布方式，成员变量依据声明的顺序进行排列（类内偏移为0开始），成员函数不占内存空间。
再看下继承，往后面添加如下代码：

```
class DerivedClass: public Base
{
    int c;
public:
    void DerivedCommonFunction();
};
```
编译，然后看到如下的内存分布（父类的内存分布不变，这里只讨论子类成员变量的内存分布）：
![](http://jbcdn2.b0.upaiyun.com/2016/12/05b6bd57d7212cc6f4d8c9e8f48b0fbb.png)
可以看到子类继承了父类的成员变量，在内存排布上，先是排布了父类的成员变量，接着排布子类的成员变量，同样，成员函数不占字节。
下面给基类加上虚函数，暂时注释掉DerivedClass，看一下这时的内存排布：

```
class Base
{
    int a;
    int b;
public:
    void CommonFunction();
    void virtual VirtualFunction();
};
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/ff16f0577084b8f73ba2f1a60821cd80.png)
这个内存结构图分成了两个部分，上面是内存分布，下面是虚表，我们逐个看。VS所带编译器是把虚表指针放在了内存的开始处（0地址偏移），然后再是成员变量；下面生成了虚表，紧跟在&Base1_meta后面的0表示，这张虚表对应的虚指针在内存中的分布，下面列出了虚函数，左侧的0是这个虚函数的序号，这里只有一个虚函数，所以只有一项，如果有多个虚函数，会有序号为1，为2的虚函数列出来。
编译器是在构造函数创建这个虚表指针以及虚表的。
那么编译器是如何利用虚表指针与虚表来实现多态的呢？是这样的，当创建一个含有虚函数的父类的对象时，编译器在对象构造时将虚表指针指向父类的虚函数；同样，当创建子类的对象时，编译器在构造函数里将虚表指针（子类只有一个虚表指针，它来自父类）指向子类的虚表（这个虚表里面的虚函数入口地址是子类的）。
所以，如果是调用Base *p = new Derived();生成的是子类的对象，在构造时，子类对象的虚指针指向的是子类的虚表，接着由Derived*到Base*的转换并没有改变虚表指针，所以这时候p->VirtualFunction，实际上是p->vfptr->VirtualFunction，它在构造的时候就已经指向了子类的VirtualFunction，所以调用的是子类的虚函数，这就是多态了。
下面加上子类，并在子类中添加虚函数，像下面这样：

```
class DerivedClass: public Base
{
    int c;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction();
};
```
可以看到子类内存的排布如下：
![](http://jbcdn2.b0.upaiyun.com/2016/12/c190bae4e5a7da89a6c24f026f7d26f0.png)
上半部是内存分布，可以看到，虚表指针被继承了，且仍位于内存排布的起始处，下面是父类的成员变量a和b，最后是子类的成员变量c，注意虚表指针只有一个，子类并没有再生成虚表指针了；下半部的虚表情况与父类是一样的。
我们把子类换个代码，像这样：

```
class DerivedClass1 : public Base
{
    int c;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction2();
};
```
注意到这时我们并没有覆写父类的虚方法，而是重声明了一个新的子类虚方法，内存分布如下：
![](http://jbcdn2.b0.upaiyun.com/2016/12/1100306c51f01244e29c3543461b0398.png)
还是只有一个虚表指针，但是下方虚表的内容变化了，虚表的0号是父类的VirtualFunction，而1号放的是子类的VirtualFunction2。也就是说，如果定义了DerivedClass的对象，那么在构造时，虚表指针就会指向这个虚表，以后如果调用的是VirtualFunction，那么会从父类中寻找对应的虚函数，如果调用的是VirtualFunction2，那么会从子类中寻找对应的虚函数。
我们再改造一下子类，像这样：

```
class DerivedClass1 : public Base
{
    int c;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction();
    void virtual VirtualFunction2();
};
```
我们既覆写父类的虚函数，也有新添的虚函数，那么可以料想的到，是下面的这种内存分布：
![](http://jbcdn2.b0.upaiyun.com/2016/12/fc6f123dd7903c24dde780a12ad7f1fe.png)
下面来讨论多重继承，代码如下：

```
class Base
{
    int a;
    int b;
public:
    void CommonFunction();
    void virtual VirtualFunction();
};
class DerivedClass1: public Base
{
    int c;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction();
};
class DerivedClass2 : public Base
{
    int d;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction();
};
class DerivedDerivedClass : public DerivedClass1, public DerivedClass2
{
    int e;
public:
    void DerivedDerivedCommonFunction();
    void virtual VirtualFunction();
};
```
内存分布从父类到子类，依次如下：
![](http://jbcdn2.b0.upaiyun.com/2016/12/6861c849dcd55a971477eb65f676f53a.png)
Base中有一个虚表指针，地址偏移为0
![](http://jbcdn2.b0.upaiyun.com/2016/12/9746443c99b77ff9d20cec109d17c977.png)
DerivedClass1继承了Base，内存排布是先父类后子类。
![](http://jbcdn2.b0.upaiyun.com/2016/12/3286c737a8b226735eb9d131a2105067.png)
DerivedClass2的情况是类似于DerivedClass1的。
![](http://jbcdn2.b0.upaiyun.com/2016/12/24b7b132dfa0820732ff15453ef29a4e.png)
下面我们重点看看这个类DerivedDerivedClass，由外向内看，它并列地排布着继承而来的两个父类DerivedClass1与DerivedClass2，还有自身的成员变量e。DerivedClass1包含了它的成员变量c，以及Base，Base有一个0地址偏移的虚表指针，然后是成员变量a和b；DerivedClass2的内存排布类似于DerivedClass1，注意到DerivedClass2里面竟然也有一份Base。
![](http://jbcdn2.b0.upaiyun.com/2016/12/8487fa7ae79519605ade51bf3b76ca44.png)
这里有两份虚表了，分别针对DerivedClass1与DerivedClass2，在&DerivedDericedClass_meta下方的数字是首地址偏移量，靠下面的虚表的那个-16表示指向这个虚表的虚指针的内存偏移，这正是DerivedClass2中的{vfptr}在DerivedDerivedClass的内存偏移。
如果采用虚继承，像下面这样：

```
class DerivedClass1: virtual public Base
{
    int c;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction();
};
class DerivedClass2 : virtual public Base
{
    int d;
public:
    void DerivedCommonFunction();
    void virtual VirtualFunction();
};
class DerivedDerivedClass :  public DerivedClass1, public DerivedClass2
{
    int e;
public:
    void DerivedDerivedCommonFunction();
    void virtual VirtualFunction();
};
```
![](http://jbcdn2.b0.upaiyun.com/2016/12/ab99ac32b60e972c43e0f287a328675c.png)
Base类没有变化，但往下看：
![](http://jbcdn2.b0.upaiyun.com/2016/12/0b3d7137ab7d3416d7137fc4f9bb2e65.png)
DerivedClass1就已经有变化了，原来是先排虚表指针与Base成员变量，vfptr位于0地址偏移处；但现在有两个虚表指针了，一个是vbptr，另一个是vfptr。vbptr是这个DerivedClass1对应的虚表指针，它指向DerivedClass1的虚表vbtable，另一个vfptr是虚基类表对应的虚指针，它指向vftable。
下面列出了两张虚表，第一张表是vbptr指向的表，8表示{vbptr}与{vfptr}的偏移；第二张表是vfptr指向的表，-8指明了这张表所对应的虚指针位于内存的偏移量。
![](http://jbcdn2.b0.upaiyun.com/2016/12/2c1d5959638f848c8c82427b2b2794fc.png)
![](http://jbcdn2.b0.upaiyun.com/2016/12/a7a34d5570534ecb5a6d51be85f649ea.png)
DerivedClass2的内存分布类似于DerivedClass1，同样会有两个虚指针，分别指向两张虚表（第二张是虚基类表）。
![](http://jbcdn2.b0.upaiyun.com/2016/12/bdee5f9cbc1e39a3d69b030982b8cfcc.png)
下面来仔细看一下DerivedDerivedClass的内存分布，这里面有三个虚指针了，但base却只有一份。第一张虚表是内含DerivedClass1的，20表示它的虚指针{vbptr}离虚基表指针{vfptr}的距离，第二张虚表是内含DerivedClass2的，12表示它的虚指针{vbptr}离虚基表指针{vfptr}的距离，最后一张表是虚基表，-20指明了它对应的虚指针{vfptr}在内存中的偏移。
![](http://jbcdn2.b0.upaiyun.com/2016/12/081b29112130bb1e819bee90a7202132.png)
![](http://jbcdn2.b0.upaiyun.com/2016/12/c85fecfa29e55caeff7b6d1174c4a35a.png)
虚继承的作用是减少了对基类的重复，代价是增加了虚表指针的负担（更多的虚表指针）。
下面总结一下（当基类有虚函数时）：
**1. 每个类都有虚指针和虚表；**
**2. 如果不是虚继承，那么子类将父类的虚指针继承下来，并指向自身的虚表（发生在对象构造时）。有多少个虚函数，虚表里面的项就会有多少。多重继承时，可能存在多个的基类虚表与虚指针；**
**3. 如果是虚继承，那么子类会有两份虚指针，一份指向自己的虚表，另一份指向虚基表，多重继承时虚基表与虚基表指针有且只有一份。**
