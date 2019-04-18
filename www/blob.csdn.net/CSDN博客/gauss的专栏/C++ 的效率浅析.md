# C++ 的效率浅析 - gauss的专栏 - CSDN博客
2013年01月04日 00:15:30[gauss](https://me.csdn.net/mathlmx)阅读数：196
    自从七十年代C语言诞生以来，一直以其灵活性、高效率和可移植性为软件开发人员
所钟爱，成为系统软件开发的首选工具。而 C++ 作为 C 语言的继承和发展，不仅保留了
C 语言的高度灵活、高效率和易于理解等诸多优点，还包含了几乎所有面向对象的特征，
成为新一代软件系统构建的利器。 
　　相对来说，C 语言是一种简洁的语言，所涉及的概念和元素比较少，主要是：宏
(macro)、指针(pointer)、结构(struct)、函数(function)和数组(array)，比较容易掌
握和理解。而 C++ 不仅包含了上面所提到的元素，还提供了私有成员(private
members)、公有成员(public members)、函数重载(function overloading)、缺省参数
(default parameters)、构造函数、析构函数、对象的引用(references)、操作符重载
(operator overloading)、友元(friends)、模板(templates)、异常处理(exceptions)等
诸多的要素，给程序员提供了更大的设计空间，同时也增加了软件设计的难度。 
　　C 语言之所以能被广泛的应用，其高效率是一个不可忽略的原因，C 语言的效率能达
到汇编语言的 80% 以上，对于一种高级语言来说，C 语言的高效率就不言而喻了。那
么，C++ 相对于C来说，其效率如何呢？实际上，C++ 的设计者 Stroustrup 要求 C++ 
效率必须至少维持在与 C 相差 5% 以内，所以，经过精心设计和实现的 C++ 同样有很高
的效率，但并非所有 C++ 程序具有当然的高效率，由于 C++ 的特殊性，一些不好的设计
和实现习惯依然会对系统的效率造成较大的影响。同时，也由于有一部分程序员对 C++ 
的一些底层实现机制不够了解，就不能从原理上理解如何提高软件系统的效率。 
　　本文主要讨论两个方面的问题：第一，对比 C++ 的函数调用和C函数调用，解析 C++
的函数调用机制；第二，例举一些 C++ 程序员不太注意的技术细节，解释如何提高 C++ 
的效率。为方便起见，本文的讨论以下面所描述的单一继承为例(多重继承有其特殊性，
另作讨论)。 
class X
{
public:
    virtual ~X(); // 析构函数
    virtual void VirtualFunc(); // 虚函数
    inline int InlineFunc() { return m_iMember}; // 内联函数
    void NormalFunc(); // 普通成员函数
    static void StaticFunc(); // 静态函数
private:
    int     m_iMember;
};
class XX: public X
{
public: XX();
    virtual ~XX();
    virtual void VirtualFunc();
private:
    String  m_strName;
    int     m_iMember2;
};
　　C++ 的的函数分为四种：内联函数(inline member function)、静态成员函数
(static member function)、虚函数(virtual member function)和普通成员函数。 
　　内联函数类似于 C 语言中的宏定义函数调用，C++ 编译器将内联函数的函数体扩展
在函数调用的位置，使内联函数看起来象函数，却不需要承受函数调用的开销，对于一些
函数体比较简单的内联函数来说，可以大大提高内联函数的调用效率。但内联函数并非没
有代价，如果内联函数体比较大，内联函数的扩展将大大增加目标文件和可运行文件的大
小；另外，inline 关键字对编译器只是一种提示，并非一个强制指令，也就是说，编译
器可能会忽略某些 inline 关键字，如果被忽略，内联函数将被当作普通的函数调用，编
译器一般会忽略一些复杂的内联函数，如函数体中有复杂语句，包括循环语句、递归调用
等。所以，内联函数的函数体定义要简单，否则在效率上会得不偿失。 
　　静态函数的调用，如下面的几种方式： 
X obj; X* ptr = &obj;
obj.StaticFunc();
ptr->StaticFunc();
X::StaticFunc();
　　将被编译器转化为一般的 C 函数调用形式，如同这样： 
mangled_name_of_X_StaticFunc();
　　  //obj.StaticFunc();
mangled_name_of_X_StaticFunc(); 
　　 // ptr->StaticFunc();
mangled_name_of_X_StaticFunc();  
　　// X::StaticFunc();
　　mangled_name_of_X_StaticFunc() 是指编译器将 X::StaticFunc() 函数经过变形
(mangled)后的内部名称(C++ 编译器保证每个函数将被 mangled 为独一无二的名称，不
同的编译器有不同的算法，C++ 标准并没有规定统一的算法，所以 mangled 之后的名称
也可能不同)。可以看出，静态函数的调用同普通的 C 函数调用有完全相同的效率，并没
有额外的开销。 
　　普通成员函数的调用，如下列方式： 
X obj; X *ptr = &obj;
obj.NormalFunc();
ptr->NormalFunc();
　　将被被编译器转化为如下的C函数调用形式，如同这样。 
mangled_name_of_X_NormalFunc(&obj); 
　　 //obj.NormalFunc();
mangled_name_of_X_NormalFunc(ptr); 
// ptr->NormalFunc();
　　可以看出普通成员函数的调用同普通的C调用没有大的区别，效率与静态函数也相
同。编译器将重新改写函数的定义，增加一个const X *this参数将调用对象的地址传送
进函数。 
　　虚函数的调用稍微复杂一些，为了支持多态性，实现运行时刻绑定，编译器需要在每
个对象上增加一个字段也就是 vptr 以指向类的虚函数表 vtbl，如类 X 的对象模型如下
图所示(本文中对此不多做解释，若想进一步了解，可以参考其它材料)。 
  +---------------+   +---------------------------+   +-------------------+
  | int m_iMember |  /| Type_info of Class I 指针 |-->|                   |
  +---------------+ / +---------------------------+   +-------------------+
  |      vptr     |/  |      I::~I() 函数地址     |     类 I 的类型信息表
  +---------------+   +---------------------------+
    I 对象内存模型    | I::VirtualFunc() 函数地址 |
                      +---------------------------+
                             类 I 的虚函数表
　　虚函数的多态性只能通过对象指针或对象的引用调用来实现，如下的调用： 
X obj;
X *ptr = &obj; X &ref = obj;
ptr->VirtualFunc();
ref.VirtualFunc();
　　将被C++编译器转换为如下的形式。 
( *ptr->vptr[2] )(ptr);
( *ptr->vptr[2] )(&ref);
　　其中的 2 表示 VirtualFunc 在类虚函数表的第 2 个槽位。可以看出，虚函数的调
用相当于一个 C 的函数指针调用，其效率也并未降低。 
　　由以上的四个例子可以看出，C++ 的函数调用效率依然很高。但 C++ 还是有其特殊
性，为了保证面向对象语义的正确性，C++ 编译器会在程序员所编写的程序基础上，做大
量的扩展，如果程序员不了解编译器背后所做的这些工作，就可能写出效率不高的程序。
对于一些继承层次很深的派生类或在成员变量中包含了很多其它类对象(如 XX 中的
m_strName 变量)的类来说，对象的创建和销毁的开销是相当大的，比如 XX 类的缺省构
造函数，即使程序员没有定义任何语句，编译器依然会给其构造函数扩充以下代码来保证
对象语义的正确性： 
XX::XX()
{
    // 编译器扩充代码所要做的工作
1、 调用父类 X 的缺省构造函数
2、 设定 vptr 指向 XX 类虚函数表
3、 调用 String 类的缺省构造函数构造 m_strName
};
　　所以为了提高效率，减少不必要的临时对象的产生、拖延暂时不必要的对象定义、用
初始化代替赋值、使用构造函数初始化列表代替在构造函数中赋值等方法都能有效提高程
序的运行效率。以下举例说明： 
　　1、 减少临时对象的生成。如以传送对象引用的方式代替传值方式来定义函数的参
数，如下例所示，传值方式将导致一个 XX 临时对象的产生 
效率不高的做法       　　　　       高效率做法
void Function( XX xx )              void Function( const XX& xx )
{                                   {
    //函数体                            // 函数体
}                                   }
　　2、 拖延暂时不必要的对象定义。在 C 中要将所有的局部变量定义在函数体头部，
考虑到 C++ 中对象创建的开销，这不是一个好习惯。如下例，如果大部分情况下
bCache 为 "真 "，则拖延 xx 的定义可以大大提高函数的效率。 
效率不高的做法                      高效率做法
void Function( bool bCache )        void Function( bool bCache )
{                                   {
    // 函数体                           // 函数体
    XX xx;                              if (bCache)
    if (bCache)                         {// do something without xx
    {                                       return;
        // do something without xx      }
        return;
    }
    // 对xx进行操作                     XX xx;
                                        // 对 xx 进行操作
    …
    return;                             return;
}                                   }
　　3、 可能情况下，以初始化代替先定义后赋值。如下例，高效率的做法会比效率不高
的做法省去了 cache 变量的缺省构造函数调用开销。 
效率不高的做法                      高效率做法
void Function(const XX &xx)         void Function(const XX &xx)
{                                   {
    XX cache;                           XX cache = xx;
    cache = xx ;
}                                   }
　　4、 在构造函数中使用成员变量的初始化列表代替在构造函数中赋值。如下例，在效
率不高的做法中，XX 的构造函数会首先调用 m_strName 的缺省构造函数，再产生一个临
时的 String object，用空串""初始化临时对象，再以临时对象赋值(assign)给
m_strName ，然后销毁临时对象。而高效的做法只需要调用一次 m_strName 的构造函数。 
效率不高的做法                      高效率做法
XX::XX()                            XX::XX() : m_strName( "" )
{                                   {
    m_strName = "";                     …
    …
}                                   } 
　　类似的例子还很多，如何写出高效的 C++ 程序需要实践和积累，但理解 C++ 的底层
运行机制是一个不可缺少的步骤，只要平时多学习和思考，编写高效的 C++ 程序是完全
可行的。
            
