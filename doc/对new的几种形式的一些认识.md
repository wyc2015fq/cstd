## 对new的几种形式的一些认识



**一道考题：请说说已经有malloc函数了为何还要引进new？**

答：当用在内置数据类型或者结构时，malloc能满足我们的需要，但用在类类型时不能，这就需要引用new，

new既分配堆内存，又自动调用类的构造函数来创建对象。

是本教科书都有提到上面这一点，可是这些公司为何还乐此不疲的考到这到道题呢？事情不是这么简单，我献

丑说这几句。

## 一、new 与 opeartor new

​    首先问大家一个问题：new 与 opeartor new有何区别？
如果你回答：operator new就是new的重载运算符呗!

回答错误，**new是C++内部定义的一种操作符**，总像sizeof一样是一种操作符，而**operator new是实作者定义的**

**一个全局函数**。不信，你可以写下面的语句：
```cpp
int *p = operator new(  //这时候，用的是VC的话，就会自动提示该函数的原型。嗯，有七个版本之多。好，现在我们弄清楚了，一个是操作符，一个是全局函数，而不是原来所以为的是重载的关系。正同我们看看他们各有什么用？

//new操作符作了三件事.语句:
MyClass * p = new MyClass;
//的伪码大至如下：

void *memory = operator new(sizeof(MyClass)); //1调用全局函数: operator new
MyClass::MyClass();  //2调用构造函数创建对象，如带参数的话调用的就是带参构造函数
MyClass * p = static_cast<MyClass*>(memory); //3转换指针类型，并赋给p
```


我们可以猜测到真正的内存分配是在`operator new`函数中完成的。

`operator new`函数其之的原型是：

```cpp
void* operator new(size_t size) throw(std::bad_alloc)
```
返回类型是`void*` ，  参数类型是：`size_t` ，是一个在系统头文件` <cstddef>`中定义的typedef。`sizeof()`的

操作结果就是该类型的。

void* operator new(size_t size)大至如下：

```cpp
void *p ;
while((p = malloc(size)) == 0)
{
     //内存分配失败。
    if ( _callnewh(size) == 0)
    {
          _Nomemory();
    }
}

return p;
```

如果内存分配成功则直接返回所分配的内存的首地址。不然就循环调用if语句，首先判断 _callnewh(size)调

用是否成功，不成功则调用_Nomemory()，_Nomemory()实际上作用是抛出一个异常，所抛出的是一个标准库中

定义的std::bad_alloc对象。_callnewh()函数首先会判断一个全局变量 _pnhHeap是否为零， _pnhHeap存放的

是一个函数指针，我们可以为new操作指定一个出错处理函数，也就是说当new分配内存失败时就会调用我们刚

才指定的函数，出错处理函数原型必须是无参且返回类型是void的函数。可以通过全局函数set_new_handler

(new_handler pnew)来设定我们的出错处理函数，而他又是通过调用_PNH _set_new_handler(_PNH pnh)来实现

设定我们的出错处理函数的。下面是_PNH _set_new_handler(_PNH pnh)的定义：

```cpp
_PNH __cdecl _set_new_handler(
        _PNH pnh
        )
{
        _PNH pnhOld;
        
        _mlock(_HEAP_LOCK);

        pnhOld = _pnhHeap;  //把原来在起作用的出错处理函数的指针赋给pnhOld
        _pnhHeap = pnh;     //我们新设定的出错处理函数的指针。
        
        _munlock(_HEAP_LOCK);

        return(pnhOld);     //返回原来旧的出错处理函数的指针。
}
```


回到我们的_callnewh()函数，如果在MyClass * p = new MyClass;语句之前设定了出错处理函数，那么这里的

_pnhHeap就不为零，接着就会调用(*_pnhHeap)()即我们的出错处理函数，否则返回零。接着调用_Nomemory()

抛出异常。这个出错处理函数是个重要的函数，设计的好的话可以做很多事情，因为他是在while中被调用的有

多次被调用的机会。在我们的出错处理函数中如果没有使用exit(1)等退出程序的语句，也没有抛出异常的话，

执行完我们的出错处理函数后，又回到while((p = malloc(size)) == 0)，再次分配内存并判断，还是失败的

话，再次调用我们的出错处理函数，当然这个出错处理函数和上面的那个出错处理函数不一样了，因为我们可

以在上面那个出错处理函数中调用set_new_handler(_PNH pnh)重新设定一个出错处理函数，也就是我们第二次

调用的这个出错处理函数。还不行的话，可以继续循环，直到你满意为止。如果你觉得累了，不玩了，最后就

会调用_Nomemory()抛出异常，函数返回到new调用的地方。

  好，现在我们清楚了操作符new作了三件事，首先调用全局operator new函数， 后者通过调用传统的malloc

函数分配内存，如果成功直接返回，不然，判断出错处理函数是否为零，不为零的话，调用我们的出错处理函

数。否则调用_Nomemory()抛出异常。如果p = malloc(size)成功，new接着做第二件事，创建对象，最后转换

指针类型并返回。

我们可以重写operator new函数。当编译器看到语句MyClass * p = new MyClass;

首先会检查我们的类定义看是否提供有operator new函数，如有，则调用该函数，接着调用构造函数，转换类

型并返回。如果没有重写operator new函数，则new操作符会调用全局中的那个operator new函数，也就是我们

上面说的这个函数。但是如果我们在new操作符前面限定了::即这样写 ::new MyClass则编译器不会去检查我们

的类的定义而直接调用全局的operator new函数。

​    操作符new不可以重载，就像sizeof操作符一样是不可以重载的。我们重载的是operator new函数。所以有

一些限定，我们重载的operator new函数的返回类型必须是void*，第一个参数必须是size_t类型的。下面是一

个自定义的operator new函数：

```cpp
class MyClass
{
  public:
     MyClass()
     {
       cout << "MyClass::MyClass()" << endl;
     }

     static void* operator new(size_t size);

  ~MyClass()
  {
     cout << "MyClass::~MyClass()" << endl;
  }
};

void* MyClass::operator new(size_t size)
{
   //在这里可以对类的静态成员数据做些控制。我们在这里有一句输出语句代替。
   cout << "MyClass::operator new" << endl;
   void* p = new MyClass;
   return p;
}
```

这样写是行不通的，因为在MyClass::operator new中的void* p = new MyClass的new是操作符new，他做三件

事，第一件就是调用MyClass::operator new(size_t size)，所以这里
是递归调用了。把程序改成：

```cpp
void* MyClass::operator new(size_t size)
{
   //在这里可以对类的静态成员数据做些控制。我们在这里有一句输出语句代替。
   cout << "MyClass::operator new" << endl;
   void* p = operator new(size);  //已修改。
   return p;
}
```

这样还是不行的，这样是直接递归(自己调用自己)。刚才是间接递归。应该改成：void* p = ::operator new

(size);  OK，使用的是全局中的operator new，或者写成：void* p = malloc( size),只是这样一来，出错后

不会自动调用出错处理函数了，只会简单的返回NULL，所以在使用new操作符的地方要注意先检测返回值是否为

零，所以最好不用malloc，还是用:: operator new(size)好，这里还可以用void* p = new char[size]，用的

是new[]操作符，不会两次调用构造函数，也不会造成递归。只是要注意在我们重写的operator delete函数中

要调用delete[] 后释放。一般情况下，我们重写了operator new函数，都要重写operator delete函数，而且

后者中的释放资源的函数要与前者分配资源的函数的形式要搭配。


另外，要想把自己重写的operator new函数设计得好，还是有好些地方需要注意的。好在需要我们重写这个函

数的情况不多，真正需要重写时，还是先参考些这方面的资料才行，<effective c++>一书中就有相关的知识介

绍。在这里我只是提到一下，让大家知道有这么一回事，应付一下这道公司们乐此不疲的考题。洋洋洒洒写上

上千字，小样，看你还敢不敢考这样的考题。

## 二、new[] 与 operator new[]

  new[]操作符与new差不多，同样做三件事：调用operator new[]函数，历遍一个vector调用构造函数。转换

指向首地址的指针类型并返回。

operator new[]函数通过把操作符new A[number]中的A与number进行计算：size_t count = number * sizeof

(A), 然后调用全局函数operator new(count).

## 三、new(void*) Myclass 与 operator new(size_t, void*)

   指定位置创建操作符new()同样做三件事，第一件就是调用operator new(size_t, void*)函数，下面两件和

new操作符的最后二件事是一样的。让我们来看看vs.net中operator new(size_t, void*)的定义：

```cpp
inline void *__cdecl operator new(size_t, void *_Where) _THROW0()
{    // construct array with placement at _Where
    return (_Where);
}
```

和operator new相比好简单哦，我们看到了，他并没有调用malloc函数，也没有调用operator new函数，他怎

么分配的内存啊？!对于operator new函数，他通过循环调用malloc函数来分配一块内存，最好把这块分配好的

内存return p返回给操作符new，让他在上面做第二，第三件事。我们这里return (_Where);按此推理，_Where
6应该指向一块已分配的可使用的内存。_Where从那里来的啊？答案是使用操作符new(void* _Where) MyClass

时所指定的。这就是指定位置创建操作符new()的用法，先在别处分配好一块内存，然后把这块内存的首地址做

为参数调用new()，new()就会在这块指定位置上创建对象，然后再把这块指定的内存的首地址自制一份给p，接

着转换类型并返回。这样子操作符new()并没有真正分配内存，所以不能调用delete来释放内存。当程度使用共

享内存或者memory-mapped I/O指定位置创建就比较有用，因为在这样的程序里对象必须放置在一个确定的地址

上或者一块被例程分配的内存里。下面看个例子。
```cpp
#include <iostream>
#include <new>                     //要使用指定位置创建操作符发布包含该头文件。
using namespace std;

void* mallocShared(size_t size);  //用于分配共享内存，该函数是别的程序员写的，你只知道通过
                                   //调用他可以获得一块已分配而未初始化的内存。

class A
{
   public:
   A()
   {
      cout << "A::A()" << endl;
      m_n = 0;
   }

   int Get()
   {
      return m_n;
   }

   ~A()
   {
      cout << "A::~A()" << endl;
   }

private:
int m_n;
};

int main()
{
    void* p = mallocShared(sizeof(A)); //该句也有可能是在别的地方调用的，
                                       //然后把p传过来。这里为了简化而放在此调用

    A* pA = new(p) A;
    cout << pA->Get() << endl;
    delete pA;
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
void* mallocShared(size_t size)
{
    void* p = malloc(size);

    if ( p == NULL)
    {
        cerr << "mallocShared(size_t size) failed!" << endl;

        exit(1);
    }

    return p;
}
```
这个程序有问题吗？抛开设计上的好坏不说，就说这个程序能否通过编译？如能，运行结果如何？请先思考三

分钟再往下看。









有疑问的可能是这一句delete pA; 函数mallocShared不是你写的，你不知道其内部是通过什么形式分配的内存

，就调用delete来释放能行吗？假如又让我们知道他内部是通过malloc函数来分配的内存，用delete来释放能

行吗？答案是：能通过编译，并能得出正确结果，通过malloc函数来分配的内存用delete来释放是没问题的，

new操作符不也是通过malloc来分配的内存，同样可以用delete来释放啊。如果mallocShared是下面这样子，情

况又怎么样呢？
```cpp
void* mallocShared(size_t size)
{
    return (new char[size]);
}
```
结果是和上面是一样的，delete p没问题，只要用new[]来分配的内存块的大小和用delete释放的内存的大小是

一样的就没问题。回想一下看，我们用new A来分配内存时，实际上是通过operator new(size)来分配的，这里

的size = sizeof(A)，这种情况下我们可以用 delete p来释放，只要p 的类型是A*，因为delete 是通过调用

operator delete(size)函数来释放的内存，这里的size也是等于sizeof(P)，当我们调用new char[number]时

，先调用operator new[]函数，后者实际上也是调用 operator new(number* sizeof(char))来分配的内存，既

然都是通过调用operator new来分配的内存，所以调用delete 来释放应该也是没问题的。因为这

里:mallocShared(sizeof(A)) ---->  size == sizeof(A) == size* sizeof(char) == sizeof(P) 大小一样。
改成：
```cpp
void* mallocShared(size_t size)
{

    return (operator new(size));
}
```
结果也是一样的! 当然这里是假设没有提供自己的operator new函数的情况下，如果重写了operator new函数

就要改成：
```cpp
void* mallocShared(size_t size)
{

    return (::operator new(size));   //用的是全局函数
}
```
这个程序目前没问题，但是存在很多的安全隐患，很容易就出错，一不小心就阴沟里翻船，有“未定义”行为

产生，结果是啥事都有可能发生。应该谢绝写这样的程序。

上面说了mallocShared(sizeof(A));可能不是在你的程序中调用的，而是在别人那里调用的，然后别人给你传

来一个指针让你把这个指针作为参数调用你自己的A* pA = new(p) A,这样子你调用delete p来释放，别人那里

或者还需要用，因为这是共共享的内存，又或者别人那里在做完想要做的工作之后，调用delete ，这样子就出

问题了。同一块内存不能释放两次。就算mallocShared(sizeof(A));是在你这里调用的，那么你可以算是内存

的分配者，你有权利and义务把他释放，可是你也要先确定别人还需不需要用到这块内存，需要的话，你就不能

马上delete，又或者呆会你自己也还需要用到，再次在这块内存上指定位置创建。所以就不必再调用

mallocShared(sizeof(A));来分配内存。把主函数改成下面这样，结果又如何，能通过编译吗？
```cpp
int main()
{
    void* p = mallocShared(sizeof(A)); //就限定是在这里调用的。
    A* pA = new(p) A;
    cout << pA->Get() << endl;
    A* pA1 = new(p) A;                 //再次指定位置创建。

    delete pA;
    return 0;
}
```
答案是：能通过编译，运行结果如下：

```
A::A()
0
A::A()
~A::A()
```

原来的那块内存是确实被释放掉了的，只是这里构造函数A::A()调用了两次而析构函数A::~A()只调用一次，
这显然不太好，如果你的类在其他地方分配了资源，需要通过析构函数来释放，这样子你少调用了一次析构函

数就会造成内存泄漏或者别的问题了。所以应该把程序改成下面这样：

```cpp
int main()
{
    void* p = mallocShared(sizeof(A)); //就限定是在这里调用的。
    A* pA = new(p) A;
    cout << pA->Get() << endl;
    A* pA1 = new(p) A;                 //再次指定位置创建。
        pA->~A();         //这里显式调用析构函数来析构对象，但是内存并没有释放，还可以再次使用。
    A* pA1 = new(p) A;
    //在这里判断别的程序是否还需要用到该内存
    delete pA;      //当别人不再需要，自己也不会再用到，可以释放!
    return 0;
}
```

这句delete p总让我担心受怕，最好调用和mallocShared函数相对应的函数来释放内存，你写了mallocShared

函数来分配资源就有义务写一个freeShared函数来释放资源，分配资源函数和释放资源函数是一对的，一起提

供给别人使用。因为只有你自己最清楚你的mallocShared函数是怎么分配的资源，你的freeShared就应该做相

应的工作。比如在mallocShared中除了分配内存，还用到其他资源，如果直接调用delete p来释放那就成问题

了。应该调用freeShared来释放。如果你是老板而你的员工只写一个mallocShared函数却没有提供相应的

freeShared函数，建议你让他走人! 不然迟早会出问题的。如果void* p = mallocShared(sizeof(A)); 语句不

是在这里调用的，你既不能使用delete p,也不能使用freeShared(p)，或者其他一切释放资源的函数。不是你

分配的资源你无权释放。当然你对整个程序把握得比较好，一切尽在你控制中，而你又和别人有协议由你来释

放的情况除外。
void * operator new(size_t, void *_Where)同样可以提供自己的版本，这时候第二个参数可以是别的类型，

_Where也不一定是指向一块已分配而未使用的内存，可以是一个指向可以分配内存的函数的指针，然后在

operator new(size_t, void *_Where)内部通过该指针来调用函数从而分配内存。也可以是其他东西，不一定

要是指针。总之可以传递你想传递的东西。下面来看个例子，这个例子来自<Bjarne Stroustrup的FAQ：C++的

风格与技巧>我懒得写了，就用他写的这个.

以下是原文：

////////////////////////////////////////////////////////////////////////////

有没有“指定位置删除”(placement delete)？



没有，不过如果你需要的话，可以自己写一个。



看看这个指定位置创建(placement new)，它将对象放进了一系列Arena中；

```cpp
class Arena {
public:
  void* allocate(size_t);
  void deallocate(void*);
  // ...
};



void* operator new(size_t sz, Arena& a)
{
  return a.allocate(sz);  //梁杨注：这里第二个参数传递的是一个引用，
                                        //然后通过其成员函数来分配内存.
}

Arena a1(some arguments);
Arena a2(some arguments);

//这样实现了之后，我们就可以这么写：
X* p1 = new(a1) X;
Y* p2 = new(a1) Y;
Z* p3 = new(a2) Z;
// ...
```



但是，以后怎样正确地销毁这些对象呢？没有对应于这种“placement new”的内建的

“placement delete”，原因是，没有一种通用的方法可以保证它被正确地使用。在C++的

类型系统中，没有什么东西可以让我们确认，p1一定指向一个由Arena类型的a1分派的对象

。p1可能指向任何东西分派的任何一块地方。



然而，有时候程序员是知道的，所以这是一种方法：



```cpp
template<class T> void destroy(T* p, Arena& a)
{
  if (p) {
       p->~T(); // explicit destructor call
       a.deallocate(p);
  }
}
//现在我们可以这么写：
destroy(p1,a1);
destroy(p2,a2);
destroy(p3,a3);
```



如果Arena维护了它保存着的对象的线索，你甚至可以自己写一个析构函数，以避免它发生

错误。
   这也是可能的：定义一对相互匹配的操作符new()和delete()，以维护《C++程序设计语

言》15.6中的类继承体系。参见《C++语言的设计和演变》10.4和《C++程序设计语言》

19.4.5。

///////////////////////////////////////////////////////////////////////////////

////////////////

C++中有指定位置创建操作符：new()，但没有指定位置删除操作符：delete()。

我们可以写：
                X* p1 = new(a1) X;  //指定位置创建
但是不可以写：
                delete(a1) p1       //这句不能通过编译。

他上面写的这个程序其实是有点问题的(靠，梁杨你也太牛B了吧，C++之父写的你也敢说有问题?!),用事实说话

，如果只提供&nbsp;void* operator new(size_t sz, Arena& a)而不提供void operator delete(void* , Arena&)
函数，vs.net的编译器会提出一条警告： “void *operator new(size_t,Arena &)” : 未找到匹配的删除运

算符；如果初始化引发异常，则不会释放内存。

对于new操作符会作三件事，在第一件分配内存顺利完成之后，接着会调用构造函数，如果在调用构造函数中发

生异常，他就会调用operator delete(void*)函数来释放在第一件事中通过operator new(size_t)来分配的内

存，从而保证不会发生内存泄漏。同样定位创建操作符new()也做三件事:第一件调用我们重写的void*

operator new(size_t sz, Arena& a)来分配内存，这步成功之后接着调用构造函数，那么如果在调用构造函数

中发生异常时，怎么办呢？没有相应的释放函数来给编译器调用，所以我们必须自己提供一个void operator

delete(void* , Arena&)来释放内存。这个函数第一个参数的类型必须是void*, 第二个类型必须和operator

new()中的相同，不然当发生异常时，编译器不会调用该函数的。当我们提供了void operator delete(void* ,

Arena&)后还是不能写：delete(a1) pA1这样的语句的。可以写operator delete(pA1, a1)，但是最好不要这样

调用，该函数是专为编译器写的，当发生构造异常时调用的。当我们成功的创建一个对象，做完一些事之后，

应该还象他上面写的那样，通过调用destroy(p1,a1);来释放。

**注意**：这里内存分配是在指定创建函数operator new()中调用其他函数来分配的，所以我们需要提供一个相应的

delete( )来预防构造失败时来释放资源。但是在我们前面写的那个共享内存的例子，定位创建函数new()并没

有分配内存，内存分配是在其他地方完成的，这时候我们就不需要提供定位删除函数delete()来释放资源，你

构造函数失败就失败呗，内存又不是你分配的，你无权释放，不可能说你创建一个对象失败了，连内存都释放

了，呆会有权释放内存者(分配者有权释放)再释放一次，那问题就大了。也就是说当我们重写定位创建函数

new()时，如果内存是在其中分配的，那么就要提供相应的delete()函数给编译器专用。反之则不必提供

delete()函数。



## 四、new(nothrow) MyClass操作符

从字面就可以看出来了，这个版本的new不会抛出异常。他也做三件事，其中第一件调用的就是不抛出异常的

operator new()函数，其原型是：

void *__cdecl operator new(size_t, const std::nothrow_t&)
    _THROW0();

看到了，其后面的异常规范是空的，即不抛出任何异常。与常用的那个operator new()函数不同

void *__cdecl operator new(size_t) _THROW1(std::bad_alloc); ，这个可以抛出std::bad_alloc异常。
不允许抛出异常不并代其内部也不会发生异常，当内部内存分配失败时发生异常，因为规范中不允许抛出异常

，这就会抛出一个意外的异常，如果用户不对这个意外异常进行捕捉的话，默认处理是调用abort函数。

## 五、 new const MyClass

这个版本的new表示在堆内存中创建后个const对象，创建完之后就不能再修改，所以必须在创建时初始化(有无

参构造函数的类除外)，并且返回的指针是一个指向const对象的指针，即要写：

const int* p = new const int(1); 

而不能写

int* p = new const int(1).

分享：


