# 二进制兼容ABI-C++库注意事项

2018年12月18日 19:52:14 [knowledgebao](https://me.csdn.net/knowledgebao) 阅读数：178



相关链接：<https://blog.csdn.net/knowledgebao/article/details/85076661>

------

 

**目录**

[1，什么是二进制兼容？](https://blog.csdn.net/knowledgebao/article/details/85072608#1%EF%BC%8C%E4%BB%80%E4%B9%88%E6%98%AF%E4%BA%8C%E8%BF%9B%E5%88%B6%E5%85%BC%E5%AE%B9%EF%BC%9F)

[2，有哪些情况会破坏库的 ABI？](https://blog.csdn.net/knowledgebao/article/details/85072608#2%EF%BC%8C%E6%9C%89%E5%93%AA%E4%BA%9B%E6%83%85%E5%86%B5%E4%BC%9A%E7%A0%B4%E5%9D%8F%E5%BA%93%E7%9A%84%20ABI%EF%BC%9F)

[3, 哪些做法多半是安全的](https://blog.csdn.net/knowledgebao/article/details/85072608#3%2C%20%E5%93%AA%E4%BA%9B%E5%81%9A%E6%B3%95%E5%A4%9A%E5%8D%8A%E6%98%AF%E5%AE%89%E5%85%A8%E7%9A%84)

[4, 反面教材：COM](https://blog.csdn.net/knowledgebao/article/details/85072608#4%2C%20%E5%8F%8D%E9%9D%A2%E6%95%99%E6%9D%90%EF%BC%9ACOM)

[4,解决办法](https://blog.csdn.net/knowledgebao/article/details/85072608#4%2C%E8%A7%A3%E5%86%B3%E5%8A%9E%E6%B3%95)

[4.1 采用静态链接](https://blog.csdn.net/knowledgebao/article/details/85072608#4.1%20%E9%87%87%E7%94%A8%E9%9D%99%E6%80%81%E9%93%BE%E6%8E%A5)

[4.2 通过动态库的版本管理来控制兼容性](https://blog.csdn.net/knowledgebao/article/details/85072608#4.2%20%E9%80%9A%E8%BF%87%E5%8A%A8%E6%80%81%E5%BA%93%E7%9A%84%E7%89%88%E6%9C%AC%E7%AE%A1%E7%90%86%E6%9D%A5%E6%8E%A7%E5%88%B6%E5%85%BC%E5%AE%B9%E6%80%A7)

[4.3 用 pimpl 技法，编译器防火墙](https://blog.csdn.net/knowledgebao/article/details/85072608#4.3%20%E7%94%A8%20pimpl%20%E6%8A%80%E6%B3%95%EF%BC%8C%E7%BC%96%E8%AF%91%E5%99%A8%E9%98%B2%E7%81%AB%E5%A2%99)

[4.4  使用d-pointer](https://blog.csdn.net/knowledgebao/article/details/85072608#4.4%C2%A0%20%E4%BD%BF%E7%94%A8d-pointer)

[4.5  对于忘记设计d-pointer的类，最标准的弥补做法是：](https://blog.csdn.net/knowledgebao/article/details/85072608#4.5%C2%A0%20%E5%AF%B9%E4%BA%8E%E5%BF%98%E8%AE%B0%E8%AE%BE%E8%AE%A1d-pointer%E7%9A%84%E7%B1%BB%EF%BC%8C%E6%9C%80%E6%A0%87%E5%87%86%E7%9A%84%E5%BC%A5%E8%A1%A5%E5%81%9A%E6%B3%95%E6%98%AF%EF%BC%9A)

[4.6 如何覆盖已实现过的虚函数？](https://blog.csdn.net/knowledgebao/article/details/85072608#4.6%20%E5%A6%82%E4%BD%95%E8%A6%86%E7%9B%96%E5%B7%B2%E5%AE%9E%E7%8E%B0%E8%BF%87%E7%9A%84%E8%99%9A%E5%87%BD%E6%95%B0%EF%BC%9F)

[4.7 使用全局函数作为接口，内部还是按照C++的方式来实现。](https://blog.csdn.net/knowledgebao/article/details/85072608#4.7%20%E4%BD%BF%E7%94%A8%E5%85%A8%E5%B1%80%E5%87%BD%E6%95%B0%E4%BD%9C%E4%B8%BA%E6%8E%A5%E5%8F%A3%EF%BC%8C%E5%86%85%E9%83%A8%E8%BF%98%E6%98%AF%E6%8C%89%E7%85%A7C%2B%2B%E7%9A%84%E6%96%B9%E5%BC%8F%E6%9D%A5%E5%AE%9E%E7%8E%B0%E3%80%82)

------

## 1，什么是二进制兼容？

C/C++ 的二进制兼容性 (binary compatibility) 有多重含义，本文主要在“头文件和库文件分别升级，可执行文件是否受影响”这个意义下讨论，我称之为 library （主要是 shared library，即动态链接库）的 ABI (application binary interface)。至于编译器与操作系统的 ABI 留给下一篇谈 C++ 标准与实践的文章。

在解释这个定义之前，先看看 Unix/C 语言的一个历史问题：open() 的 flags 参数的取值。open(2) 函数的原型是int open(const char *pathname, int flags);其中 flags 的取值有三个： O_RDONLY,  O_WRONLY,  O_RDWR。与一般人的直觉相反，这几个值不是**按位或** (bitwise-OR) 的关系，即 O_RDONLY | O_WRONLY != O_RDWR。如果你想以读写方式打开文件，必须用 O_RDWR，而不能用 (O_RDONLY | O_WRONLY)。为什么？因为 O_RDONLY, O_WRONLY, O_RDWR 的值分别是 0, 1, 2。它们不满足**按位或**。那么为什么 C 语言从诞生到现在一直没有纠正这个不足之处？比方说把 O_RDONLY, O_WRONLY, O_RDWR 分别定义为 1, 2, 3，这样 O_RDONLY | O_WRONLY == O_RDWR，符合直觉。而且这三个值都是宏定义，也不需要修改现有的源代码，只需要改改系统的头文件就行了。因为这么做会破坏二进制兼容性。对于已经编译好的可执行文件，它调用 open(2) 的参数是写死的，更改头文件并不能影响已经编译好的可执行文件。比方说这个可执行文件会调用 open(path, 1) 来**写**文件，而在新规定中，这表示**读**文件，程序就错乱了。

以上这个例子说明，如果以 shared library 方式提供函数库，那么头文件和库文件不能轻易修改，否则容易破坏已有的二进制可执行文件，或者其他用到这个 shared library 的 library。操作系统的 system call 可以看成 Kernel 与 User space 的 interface，kernel 在这个意义下也可以当成 shared library，你可以把内核从 2.6.30 升级到 2.6.35，而不需要重新编译所有用户态的程序。

所谓“二进制兼容性”指的就是在升级（也可能是 bug fix）库文件的时候，不必重新编译使用这个库的可执行文件或使用这个库的其他库文件，程序的功能不被破坏。见 QT FAQ 的有关条款：<http://developer.qt.nokia.com/faq/answer/you_frequently_say_that_you_cannot_add_this_or_that_feature_because_it_woul>在 Windows 下有恶名叫 DLL Hell，比如 MFC 有一堆 DLL，mfc40.dll, mfc42.dll, mfc71.dll, mfc80.dll, mfc90.dll，这是动态链接库的本质问题，怪不到 MFC 头上。

## 2，有哪些情况会破坏库的 ABI？

到底如何判断一个改动是不是二进制兼容呢？这跟 C++ 的实现方式直接相关，虽然 C++ 标准没有规定 C++ 的 ABI，但是几乎所有主流平台都有明文或事实上的 ABI 标准。比方说 ARM 有 EABI，Intel Itanium（英特尔安腾）  有 <http://www.codesourcery.com/public/cxx-abi/abi.html>，x86-64 有仿 Itanium 的 ABI，SPARC 和 MIPS 也都有明文规定的 ABI，等等。x86 是个例外，它只有事实上的 ABI，比如 Windows 就是 Visual C++，Linux 是 G++（G++ 的 ABI 还有多个版本，目前最新的是 G++ 3.4 的版本），Intel 的 C++ 编译器也得按照 Visual C++ 或 G++ 的 ABI 来生成代码，否则就不能与系统其它部件兼容。

C++ ABI 的主要内容：

- 函数参数传递的方式，比如 x86-64 用寄存器来传函数的前 4 个整数参数
- 虚函数的调用方式，通常是 vptr/vtbl 然后用 vtbl[offset] 来调用
- struct 和 class 的内存布局，通过偏移量来访问数据成员
- name mangling(mangling的目的就是为了给重载的函数不同的签名，以避免调用时的二义性调用)
- RTTI 和异常处理的实现（以下本文不考虑异常处理）

C/C++ 通过头文件暴露出动态库的使用方法，这个“使用方法”主要是给编译器看的，编译器会据此生成二进制代码，然后在运行的时候通过装载器(loader)把可执行文件和动态库绑到一起。如何判断一个改动是不是二进制兼容，主要就是看头文件暴露的这份“使用说明”能否与新版本的动态库的实际使用方法兼容。因为新的库必然有新的头文件，但是现有的二进制可执行文件还是按旧的头文件来调用动态库。

这里举一些源代码兼容但是二进制代码不兼容例子

1,对于已经存在的类：
   1.1,本来对外开放了，现在想收回来不开放.
   1.2, 换爹 (加爹，减爹，重新给爹排座次). 
2,对于类模板来说： 
   2.1,修改任何模板参数（增减或改变座次），比方说 Foo<T> 改为 Foo<T, Alloc=alloc<T> >，这会改变 name mangling
3,对于函数来说：
   3.1,不再对外开放.
   3.2,彻底删掉
   3.3,改成内联的(把代码从类定义外头移到头文件的类定义里头也算改内联)。
   3.4,改变函数特征串：
      3.4.1,修改参数，包括增减参数或函数甚至是成员函数的const/volatile描述符。如果一定要这么干，增加一个新函数吧。
      3.4.2,把private改成protected或者public。如果一定要这么干，增加一个新函数吧。
      3.4.3,对于非成员函数，如果用extern "C"声明了，可以很小心地增减函数参数而不破坏二进制兼容。
4,对于虚成员函数来说：
   4.1,给没虚函数或者虚基类的类增加虚函数,这会造成 vtbl 里的排列变化。（不要考虑“只在末尾增加”这种取巧行为，因为你的 class 可能已被继承。）
   4.2,修改有别的类继承的基类
   4.3,修改虚函数的前后顺序
   4.4,如果一个函数不是在往上数头一个非虚基类中声明的，覆盖它会造成二进制不兼容。
   4.5,如果虚函数被覆盖时改变了返回类型，不要修改它。
5,对于非私有静态函数和非静态的非成员函数：
   5.1,改成不开放的或者删除
   5.2,修改类型或者const/violate
6,对于非静态成员函数:
   6.1,增加新成员
   6.2,给非静态成员重新排序或者删除
   6.3,修改成员的类型, 有个例外就是修改符号：signed/unsigned改来改去，不影响字节长度。
7，改变 enum 的值，把 enum Color { Red = 3 }; 改为 Red = 4。这会造成错位。当然，由于 enum 自动排列取值，添加 enum 项也是不安全的，除非是在末尾添加。

8，给 class Bar 增加数据成员，造成 sizeof(Bar) 变大，以及内部数据成员的 offset 变化，这是不是安全的？通常不是安全的，但也有例外。
   8.1,如果客户代码里有 new Bar，那么肯定不安全，因为 new 的字节数不够装下新 Bar。相反，如果 library 通过 factory 返回 Bar* （并通过 factory 来销毁对象）或者直接返回 shared_ptr<Bar>，客户端不需要用到 sizeof(Bar)，那么可能是安全的。同样的道理，直接定义 Bar bar; 对象（无论是函数局部对象还是作为其他 class 的成员）也有二进制兼容问题。
   8.2,如果客户代码里有 Bar* pBar; pBar->memberA = xx;，那么肯定不安全，因为 memberA 的新 Bar 的偏移可能会变。相反，如果只通过成员函数来访问对象的数据成员，客户端不需要用到 data member 的 offsets，那么可能是安全的。
   8.3:如果客户调用 pBar->setMemberA(xx); 而 Bar::setMemberA() 是个 inline function，那么肯定不安全，因为偏移量已经被 inline 到客户的二进制代码里了。如果 setMemberA() 是 outline function，其实现位于 shared library 中，会随着 Bar 的更新而更新，那么可能是安全的。

那么只使用 header-only 的库文件是不是安全呢？不一定。如果你的程序用了 boost 1.36.0，而你依赖的某个 library 在编译的时候用的是 1.33.1，那么你的程序和这个 library 就不能正常工作。因为 1.36.0 和 1.33.1 的 boost::function 的模板参数类型的个数不一样，其中一个多了 allocator。

这里有一份黑名单，列在这里的肯定是二级制不兼容，没有列出的也可能二进制不兼容，见 KDE 的文档：<http://techbase.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B>

## 3, 哪些做法多半是安全的

前面我说“不能轻易修改”，暗示有些改动多半是安全的，这里有一份白名单，欢迎添加更多内容。

只要库改动不影响现有的可执行文件的二进制代码的正确性，那么就是安全的，我们可以先部署新的库，让现有的二进制程序受益。

1 增加非虚函数(成员函数)，增加signal/slots，构造函数什么的。
2 增加枚举enum或增加枚举中的项目。
3 重新实现在父类里定义过的虚函数 (就是从这个类往上数的第一个非虚基类），理论上讲，程序还是找那个基类要这个虚函数的实现，而不是找你新写的函数要，所以是安全的。但是这可不怎么保准儿，尽量少用。（好多废话，结论是少用） 有一个例外: C++有时候允许重写的虚函数改变返回类型，在这种情况下无法保证二进制兼容。
4 修改内联函数，或者把内联函数改成非内联的。这也很危险，尽量少用。
5 去掉一个私有非虚函数。如果在任何内联函数里用到了它，你就不能这么干了。
6 去掉私有的静态成员。同样，如果内联函数引用了它，你也不能这么干。
7 增加私有成员。
8 增加新类。
9 对外开放一个新类。
10 增减类的友元声明。
11 修改保留成员的类型。
12 把原来的成员位宽扩大缩小，但扩展后不得越过边界（char和bool不能过8位界，short不能过16位界，int不过32位界，以此类推）这个也接近闹残：原来没用到的那么几个位我扩来扩去当然没问题，可是这样实在是不让人放心。
13 修改数据成员的名称，因为生产的二进制代码是按偏移量来访问的，当然，这会造成源码级的不兼容。

## 4, 反面教材：COM

在 C++ 中以虚函数作为接口基本上就跟二进制兼容性说拜拜了。具体地说，以只包含虚函数的 class （称为 interface class）作为程序库的接口，这样的接口是僵硬的，一旦发布，无法修改。

比方说 M$ 的 COM，其 DirectX 和 MSXML 都以 COM 组件方式发布，我们来看看它的带版本接口 (versioned interfaces)：

- IDirect3D7, IDirect3D8, IDirect3D9, ID3D10*, ID3D11*
- IXMLDOMDocument, IXMLDOMDocument2, IXMLDOMDocument3

换话句话说，每次发布新版本都引入新的 interface class，而不是在现有的 interface 上做扩充。这样一样不能兼容现有的代码，强迫客户端代码也要改写。

回过头来看看 C 语言，C/Posix 这些年逐渐加入了很多新函数，同时，现有的代码不用修改也能运行得很好。如果要用这些新函数，直接用就行了，也基本不会修改已有的代码。相反，COM 里边要想用 IXMLDOMDocument3 的功能，就得把现有的代码从 IXMLDOMDocument 全部升级到 IXMLDOMDocument3，很讽刺吧。

tip：如果遇到鼓吹在 C++ 里使用面向接口编程的人，可以拿二进制兼容性考考他。

## 4,解决办法

### 4.1 采用静态链接

这个是王道。在分布式系统这，采用静态链接也带来部署上的好处，只要把可执行文件放到机器上就行运行，不用考虑它依赖的 libraries。目前 muduo 就是采用静态链接。

### 4.2 通过动态库的版本管理来控制兼容性

这需要非常小心检查每次改动的二进制兼容性并做好发布计划，比如 1.0.x 系列做到二进制兼容，1.1.x 系列做到二进制兼容，而 1.0.x 和 1.1.x 二进制不兼容。《程序员的自我修养》里边讲过 .so 文件的命名与二进制兼容性相关的话题，值得一读。

### 4.3 用 pimpl 技法，编译器防火墙

参考<http://www.cnblogs.com/Solstice/archive/2011/03/13/1982563.html>

在头文件中只暴露 non-virtual 接口，并且 class 的大小固定为 sizeof(Impl*)，这样可以随意更新库文件而不影响可执行文件。当然，这么做有多了一道间接性，可能有一定的性能损失。见 Exceptional C++ 有关条款和 C++ Coding Standards 101。

Java 是如何应对的

Java 实际上把 C/C++ 的 linking 这一步骤推迟到 class loading 的时候来做。就不存在“不能增加虚函数”，“不能修改 data member” 等问题。在 Java 里边用面向 interface 编程远比 C++ 更通用和自然，也没有上面提到的“僵硬的接口”问题。

## 4.4  使用d-pointer

d-pointer是Qt开发者发明的一个保护二进制兼容的办法，也是Qt如此成功的原因之一。
假如你要声明一个类Foo的话，先声明一个它的私有类，用向前引用的方法，在类Foo里，声明一个指向FooPrivate的指针，FooPrivate类本身在实现文件.cpp里定义，不需要头文件，在类Foo的构造函数里，创建一个FooPrivate的实例，当然别忘记在析构函数里删掉它，还有一个技巧，在大部分环境下，把d-pointer声明成const是比较明智的。这样可以避免意外修改和拷来拷去，避免内存泄露，这样，你可以修改d指向的内容，但是不能修改指针本身。

.h文件

```cpp
class FooPrivate;



class Foo



{



publish:



    Foo();



    ~Foo();



 



private:



    FooPrivate* const d;



}
```

.cpp文件

```
class FooPrivate {



public:



    FooPrivate()



        : m1(0), m2(0)



    {}



privite:



    int m1;



    int m2;



};



 



Foo()



{



    d = new FooPrivate;



}



~Foo()



{



    delete d;



    d = NULL;



}
```

## 4.5  对于忘记设计d-pointer的类，最标准的弥补做法是：

​    \* 设计一个私有类FooPrivate.
​    \* 创建一个静态的哈西表 static QHash<Foo *, FooPrivate>.
​    \* 很不幸的是大部分编译器都是闹残，在创建动态链接库的时候都不会自动创建静态对象，所以你要用Q_GLOBAL_STATIC宏来声明这个哈西表才行:

```
//为了二进制兼容: 增加一个真正的d-pointer



Q_GLOBAL_STATIC(QHash<Foo *,FooPrivate>, d_func);



static FooPrivate* d( const Foo* foo )



{



    FooPrivate* ret = d_func()->value( foo, 0 );



    if ( ! ret ) {



        ret = new FooPrivate;



        d_func()->insert( foo, ret );



    }



    return ret;



}



static void delete_d( const Foo* foo )



{



    FooPrivate* ret = d_func()->value( foo, 0 );



    delete ret;



    d_func()->remove( foo );



}
```

这样你就可以在类里自由增减成员对象了，就好像你的类拥有了d-pointer一样，只要调用d(this)就可以了：

d(this)->m1 = 5;

\* 析构函数也要加入一句:

delete_d(this);

\* 记得加入二进制兼容(BCI）的标志，下次大版本发布的时候赶紧修改过来。
    \* 下次设计类的时候，别再忘记加入d-pointer了。

## 4.6 如何覆盖已实现过的虚函数？

前文说过，如果爹类已经实现过虚函数，你覆盖是安全的：老的程序仍然会调用父类的实现。假如你有如下类函数：

```
void C::foo()



{



    B::foo();



}
```

B::foo()被直接调用。如果B继承了A，A中有foo()的实现，B中却没有foo()的实现，则C::foo()会直接调用A::foo()。如果你加入了一个新的B::foo()实现，只有在重新编译以后，C::foo()才会转为调用B::foo()。
一个善解人意的例子:

```
B b;                // B 继承 A



b.foo();
```

如果B的上一版本链接库根本没B::foo()这个函数，你调用foo()时一般不会访问虚函数表，而是直接调用A::foo()。
如果你怕用户重新编译时造成不兼容，也可以把A::foo() 改为一个新的保护函数 A::foo2()，然后用如下代码修补:

```
void A::foo()



{



    if( B* b = dynamic_cast< B* >( this ))



        b->B::foo(); // B:: 很重要



    else



        foo2();



}



void B::foo()



{



    // 新的函数功能



    A::foo2(); // 有可能要调用父类的方法



}
```

所有调用B类型的函数foo()都会被转到 B::foo().只有在明确指出调用A::foo()的时候才会调用A::foo()。

## 4.7 使用全局函数作为接口，内部还是按照C++的方式来实现。

这样的库既可以给C++使用者调用，也可以给C程序员调用，并且也容易封装成被其他语言调用的库。比如python或者java等。

 

------

参考资料：

1，<https://blog.csdn.net/willon_tom/article/details/5499259>

2，<https://www.jianshu.com/p/f3728924835c?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation>

3，<http://www.cnblogs.com/Solstice/archive/2011/03/09/1978024.html>

 

 