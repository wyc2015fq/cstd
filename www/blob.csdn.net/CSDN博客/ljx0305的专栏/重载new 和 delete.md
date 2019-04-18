# 重载new 和 delete - ljx0305的专栏 - CSDN博客
2008年07月27日 18:27:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1243标签：[delete																[class																[编译器																[byte																[null																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
一、重载规则
I.可以重载的操作符
+- */ %
^& |~ !
=>< +=-=
*=/= %=^=&=
|=>><< >>=<<=
==!=>= <=&&
||++-- ->*,
->[]() operator new operator new[]
operator delete operator delete []
II.不能重载的操作符
::  .  .*  ?:
sizeof typeid new delete
static_castdynamic_castconst_castreinterpret_cast
III.基本规则
1.一元操作符可以是不带参数的成员函数[1]或带一个参数的非成员函数[1]。
2.二元操作符可以是带一个参数的成员函数[1]或带两个参数的非成员函数[1]。
3.operator=、operator[]、operator()、operator->只能定义为成员函数[1]。
4.operator->的返回值必须是一个指针或能使用->的对象。
5.重载 operator++ 和 operator-- 时带一个 int 参数表示后缀，不带参数表示前缀。
6.除 operator new 和 operator delete 外，重载的操作符参数中至少要有一个非内建数据类型。
7.x@y 搜索范围为：x 成员函数--> 全局函数/X所在名字空间中的函数/Y所在名字空间中的函数/X的友元函
   数/Y的友元函数。
8.重载的的操作符应尽量模拟操作符对内建类型的行为。
二、使用重载
I.操作符重载的一些建议
    1.只将会改变第一个参数的值的操作符(如: +=)定义为成员函数，而将返回一个新对象的操作符(如: +)定义为非成员函数(并使用 += 来实现)。
    2.只有非成员函数才能在左参数上实施性别转换，如果需要进行转换则应将操作符定义为非成员函数。
    3.对一元操作符, 为避免隐式转换最好将其重载为成员函数。
    4.对二元操作符, 为能在左操作数上能进行和右操作数一样的隐式转换, 最好将其重载为非成员函数。
    5.为了遵照使用习惯，operator>>、operator<< 应定义为非成员函数。
    6.重载 operator[] 之类的操作符, 应尽量提供 const 版本和非 const 版本。
    7.关于将操作符定义为成员或非成员可参考以下建议:
操作符建 议
所有一元操作符成员
= () [] ->必须为成员
+= -= /= *= ^= &= != %= >>= <<=成员
其它二元操作符非成员
    8.如果默认操作符已经可以施用于你的型别上, 则应尽量避免重载此操作符. 如 operator, 、operator&(取地址) 等等.
II. 重载 operator new
    1.为什么要重载 operator new ?
[效率问题] 通常系统默认提供的分配器速度极慢, 而且分配小型对象时空间浪费严重.
[改变行为] 默认的分配器失败时会抛出异常, 或许你想改变这种行为.
    2. operator new 的行为
[区分三个不同的 new]
    new 操作符(new 表达式, new operator, new expression): 通常我们调用 X * pX = new X 时使用的就是这个操作符, 它由语言内建, 不能重载, 不能改变其行为. 它包括分配内存的 operator new 和调用构造函数的 placement new.
    operator new :opeator new 是一个函数, void * operator new(size_t size) . 它分配指定大小的内存, 可以被重载, 可以添加额外的参数, 但第一个参数必须为 size_t. operator new 除了被 new operator 调用外也可以直接被调用: void * rawMem = operator new(sizeof(X)).
    placement new : placement new 在一块指定的内存上使用构造函数, 包含头文件 <new> 之后也可以直接使用 placement new: X * pX = new (rawMem) X. [2]
    与 new operator 类似, 对于 delete operator, 也存在 operator delete: void operator delete(void *), 析构方法 pX->~X().
[operator new 的错误处理]
    默认的 operator new 在内存分配失败时将会抛出 std::bad_alloc 异常; nothrow new [3]
(X * pX = new (nothrow) X) 在内存分配失败时将会返回 0 . 这种行为可以通过设置 new-handler 来改变. new-handler 是一个回调函数指针, typedef void(*new_handler)(void). 通过 set_new_handler(new_handler) 函数设置回调句柄后, 如果分配内存失败, operator new 将会不断的调用 new-handler 函数, 直到找到足够的内存为止. 为了避免死循环, new-handler 函数必须具备以下行为之一:
    (1).找到可用的内存.
    (2).安装其它的 new-handler 函数.
    (3).卸除 new-handler, 即 set_new_hanlder(0), 这样下此循环将恢复默认行为抛出异常或返回 0.
    (4).抛出异常.
    (5).保存错误日志, 退出程序.
    3.准备重载 operator new
    重载 operator new 时需要兼容默认的 operator new 错误处理方式. 另外, C++ Standard 规定当要求的内存为 0 byte 时也应该返回有效的内存地址. 所以 operator new 的重载实现应大致如下:
void * ... operator new(size_t size ... )
{
    if(size == 0)
         size = 1;
    while(1)
    {
        ... // allocate memery
        if(allocate sucessfull)
            return ... // return the pointer.
        new_handler curhandler = set_new_handler(0);
        set_new_handler(curhandler); // get current new handler
        if(curhandler == 0)
            (*curhandler)();
        else
            throw std::bad_alloc();
    }
}
    重载 operator delete 简单许多, 只需注意 C++ Standard 要求删除一个 NULL 是安全的即可.
    4.重载 operator new
    opeator new 的重载和其它操作符大不相同.首先, 即使你不重载, 默认的 operator new 也可施用于你的自定义型别上(operator, 也具有此特性), 而其它操作符如果不进行重载就无法使用. 其次, 其它重载其它操作符时参数个数都是固定的, 而 operator new 的参数个数是可以任意的, 只需要保证第一个参数为 size_t, 返回类型为 void * 即可, 而且其重载的参数类型也不必包含自定义类型. 更一般的说, operator new 的重载更像是一个函数的重载, 而不是一个操作符的重载.
[★ 用不同的参数重载 operator new]
    通过使用不同的参数类型, 可以重载 operator new, 例如 :
void * operator new(size_t size, int x, int y, int z)
{
    ...
}
X * pX = new (1, 2, 3) X;
    你还可以为 operator new 的重载使用默认值, 其原则和普通函数重载一样, 只要不造成和已存在的形式发生冲突即可. 可能你已经想到了, 你甚至还可以在 operator new 中使用不定参数, 如果你真的需要的话.
void * operator new(size_t size, int x, int y = 0, int z = 0)
{
    ...
}
X * pX = new (10) X;
Y * pY = new (10, 10) Y;
Z * pZ = new (10, 10, 10) Z;
...
void * operator new(size_t size, ...)
...
    在全局空间中也可直接重载 void * operator new(size_t size) 函数, 这将改变所有默认的 new 操作符的行为, 不建议使用.
[★ 重载 class 专属的 operator new]
    为某个 class 重载 operator new 时必须定义为类的静态函数[4], 因为 operator new 会在类的对象被构建出来之前调用. 即是说调用 operator new 的时候还不存在 this 指针, 因此重载的 operator new 必须为静态的. 当然在类中重载 operator new 也可以添加额外的参数, 并可以使用默认值.另外, 和普通函数一样, operator new 也是可以继承的.
class X{
...
static void * operator new(size_t size); // ... (1)
static void * operator new(size_t size, int); // ... (2)
};
class Y : public X{
...
};
class Z : public X{
...
static void * operator new(size_t size); // ... (3)
};
X * pX1 = new X; // call (1)
X * pX2 = ::new X; // call default operator new
X * pX3 = new (0) X; // call (2)
Y * pY1 = new Y; // call (1)
Z * pZ1 = new Z; // call (3)
Z * pZ2 = ::new Z; // call default operator new
Z * pZ3 = X::new Z; // error, no way to call (1)
Z * pZ4 = new (0) Z; // error, no way to call (2)
    5.重载 operator delete
    如果你重载了一个 operator new, 记得一定要在相同的范围内重载 operator delete. 因为你分配出来的内存只有你自己才知道如何释放. 如果你忘记了, 编译器不会给你任何提示, 它将会使用默认的 operator delete 来释放内存. 这种忘记的代价是惨重的, 你得时刻在写下 operator new 的同时写下 operator delete.
    如果在类中使用 operator delete, 也必须将其声明为静态函数. 因为调用 operator delete 时对象已经被析构掉了. operator delete 的重载可以有两种形式:
(1) void operator delete(void * mem)
(2) void operator delete(void * mem, size_t size)
    并且这两种形式的 operator delete 可以同时存在, 当调用 delete px 时, 如果 (1) 式存在的话将调用 (1) 式. 只有在 (1) 式不存在时才会调用 (2) 式. 对第 (2) 种形式的 operator delete, 如果用基类指针删除派生类对象, 而基类的析构函数没有虚拟的时候, size 的值可能是错误的.
引用:http://blog.sina.com.cn/s/reader_4c9faf3d01000c4n.html
