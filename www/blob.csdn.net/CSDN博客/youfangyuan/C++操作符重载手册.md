# C++操作符重载手册 - youfangyuan - CSDN博客
2011年09月20日 14:56:04[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：228
[http://hi.baidu.com/xiaoduo170/blog/item/9ae4cf51ae739110377abe16.html](http://hi.baidu.com/xiaoduo170/blog/item/9ae4cf51ae739110377abe16.html)
一、重载规则
I.可以重载的操作符
|[+](http://www.adintr.com/program/article/05.oper.html#ad1)|[-](http://www.adintr.com/program/article/05.oper.html#sb1)|[*](http://www.adintr.com/program/article/05.oper.html#mu1)|[/](http://www.adintr.com/program/article/05.oper.html#dv1)|[%](http://www.adintr.com/program/article/05.oper.html#md1)|
|----|----|----|----|----|
|[^](http://www.adintr.com/program/article/05.oper.html#mi1)|[&](http://www.adintr.com/program/article/05.oper.html#an1)|[|](http://www.adintr.com/program/article/05.oper.html#ve1)|[~](http://www.adintr.com/program/article/05.oper.html#fe1)|[!](http://www.adintr.com/program/article/05.oper.html#ta1)|
|[=](http://www.adintr.com/program/article/05.oper.html#eq1)|[>](http://www.adintr.com/program/article/05.oper.html#gt1)|[<](http://www.adintr.com/program/article/05.oper.html#lt1)|[+=](http://www.adintr.com/program/article/05.oper.html#ad1eq1)|[-=](http://www.adintr.com/program/article/05.oper.html#sb1eq1)|
|[*=](http://www.adintr.com/program/article/05.oper.html#mu1eq1)|[/=](http://www.adintr.com/program/article/05.oper.html#dv1eq1)|[%=](http://www.adintr.com/program/article/05.oper.html#md1eq1)|[^=](http://www.adintr.com/program/article/05.oper.html#mi1eq1)|[&=](http://www.adintr.com/program/article/05.oper.html#an1eq1)|
|[|=](http://www.adintr.com/program/article/05.oper.html#ve1eq1)|[>>](http://www.adintr.com/program/article/05.oper.html#gt2)|[<<](http://www.adintr.com/program/article/05.oper.html#lt2)|[>>=](http://www.adintr.com/program/article/05.oper.html#gt2eq1)|[<<=](http://www.adintr.com/program/article/05.oper.html#lt2eq1)|
|[==](http://www.adintr.com/program/article/05.oper.html#eq2)|[!=](http://www.adintr.com/program/article/05.oper.html#ta1eq1)|[>=](http://www.adintr.com/program/article/05.oper.html#gt1eq1)|[<=](http://www.adintr.com/program/article/05.oper.html#lt1eq1)|[&&](http://www.adintr.com/program/article/05.oper.html#an2)|
|[||](http://www.adintr.com/program/article/05.oper.html#ve2)|[++](http://www.adintr.com/program/article/05.oper.html#ad2)|[--](http://www.adintr.com/program/article/05.oper.html#sb2)|[->*](http://www.adintr.com/program/article/05.oper.html#gt1mu1)|[,](http://www.adintr.com/program/article/05.oper.html#co1)|
|[->](http://www.adintr.com/program/article/05.oper.html#qm2)|[[]](http://www.adintr.com/program/article/05.oper.html#zk2)|[()](http://www.adintr.com/program/article/05.oper.html#xk2)|[operator new](http://www.adintr.com/program/article/05.oper.html#nw1)|[operator new[]](http://www.adintr.com/program/article/05.oper.html#nw2)|
|[operator delete](http://www.adintr.com/program/article/05.oper.html#de1)|[operator delete []](http://www.adintr.com/program/article/05.oper.html#de2)| | | |
II.不能重载的操作符
|::|.|.*|? :|
|----|----|----|----|
|sizeof|typeid|new|delete|
|static_cast|dynamic_cast|const_cast|reinterpret_cast|
III.基本规则
1.一元操作符可以是不带参数的[成员函数**[1]**](http://www.adintr.com/program/article/05.oper.html#z1)或带一个参数的[非成员函数**[1]**](http://www.adintr.com/program/article/05.oper.html#z1)。
2.二元操作符可以是带一个参数的成员函数[**[1]**](http://www.adintr.com/program/article/05.oper.html#z1)或带两个参数的非成员函数[**[1]**](http://www.adintr.com/program/article/05.oper.html#z1)。
3.operator=、operator[]、operator()、operator->只能定义为成员函数[**[1]**](http://www.adintr.com/program/article/05.oper.html#z1)。
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
||操作符|建 议||操作符|建 议| | | | | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|----|----|
|操作符|建 议| | | | | | | | | | | |
||所有一元操作符||成员||----|----|----||= () [] ->||必须为成员||+= -= /= *= ^= &= != %= >>= <<=||成员||其它二元操作符||非成员||所有一元操作符||成员|= () [] ->||必须为成员|+= -= /= *= ^= &= != %= >>= <<=||成员|其它二元操作符||非成员|
|所有一元操作符||成员| | | | | | | | | | |
|= () [] ->||必须为成员| | | | | | | | | | |
|+= -= /= *= ^= &= != %= >>= <<=||成员| | | | | | | | | | |
|其它二元操作符||非成员| | | | | | | | | | |
     8.如果默认操作符已经可以施用于你的型别上, 则应尽量避免重载此操作符. 如 operator, 、operator&(取地址) 等等.
II. 重载 operator new
     1.为什么要重载 operator new ?
[效率问题] 通常系统默认提供的分配器速度极慢, 而且分配小型对象时空间浪费严重.
[改变行为] 默认的分配器失败时会抛出异常, 或许你想改变这种行为.
     2. operator new 的行为
[区分三个不同的 new] 
     new 操作符(new 表达式, new operator, new expression): 通常我们调用 X * pX = new X 时使用的就是这个操作符, 它由语言内建, 不能重载, 不能改变其行为. 它包括分配内存的 operator new 和调用构造函数的 placement new.
     operator new :opeator new 是一个函数, void * operator new(size_t size) . 它分配指定大小的内存, 可以被重载, 可以添加额外的参数, 但第一个参数必须为 size_t. operator new 除了被 new operator 调用外也可以直接被调用: void * rawMem = operator new(sizeof(X)).
     placement new : placement new 在一块指定的内存上使用构造函数, 包含头文件 <new> 之后也可以直接使用 [placement new: X * pX = new (rawMem) X. **[2]**](http://www.adintr.com/program/article/05.oper.html#z2)
     与 new operator 类似, 对于 delete operator, 也存在 operator delete: void operator delete(void *), 析构方法 pX->~X().
[operator new 的错误处理]
     默认的 operator new 在内存分配失败时将会抛出 std::bad_alloc 异常; [nothrow new **[3]**](http://www.adintr.com/program/article/05.oper.html#z3)
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
     opeator new 的重载和其它操作符大不相同.首先, 即使你不重载, 默认的 operator new 也可施用于你的自定义型别上(operator, 也具有此特性), 而其它操作符如果不进行重载就无法使用. 其次, 其它重载其它操作符时参数个数都是固定的, 而 operator new 的参数个数是可以任意的, 只需要保证第一个参数为 size_t, 返回类型为 void * 即可, 而且其重载的参数类型也不必包含自定义类型. 更一般的说,
**operator new 的重载更像是一个函数的重载, 而不是一个操作符的重载.**
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
     为某个 class 重载 operator new 时[必须定义为类的静态函数**[4]**](http://www.adintr.com/program/article/05.oper.html#z4), 因为 operator new 会在类的对象被构建出来之前调用.
 即是说调用 operator new 的时候还不存在 this 指针, 因此重载的 operator new 必须为静态的. 当然在类中重载 operator new 也可以添加额外的参数, 并可以使用默认值.另外, 和普通函数一样, operator new 也是可以继承的.
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
三、重载参考
－ const Carrot operator+(const Carrot& lhs, const Carrot& rhs)
{ 
     Carrot result = lhs;
     return result += rhs; 
} 
【注】1. 如果可能, 应考虑使用 operator+= 来实现 operator+ . 
          2. operator+ 不能返回引用, 应返回值类型.
          3. 为了阻止形如 (a + b) = c 的调用, 应将返回值声明为 const .
· const Carrot operator-(const Carrot& lhs, const Carrot& rhs)
－ const Carrot operator*(const Carrot& lhs, const Carrot& rhs)
{ 
     ... 
} 
【注】1. operator* 还可以[重载为提领操作符](http://www.adintr.com/program/article/05.oper.html#mu1f).
· const Carrot operator/(const Carrot& lhs, const Carrot& rhs)
· const Carrot operator%(const Carrot& lhs, const Carrot& rhs)
· const Carrot operator^(const Carrot& lhs, const Carrot& rhs)
－ const Carrot operator&(const Carrot& lhs, const Carrot& rhs)
{ 
     ... 
} 
【注】1. operator& 还可以[重载为取地址操作符](http://www.adintr.com/program/article/05.oper.html#an1f).
· const Carrot operator|(const Carrot& lhs, const Carrot& rhs)
－ const Carrot Carrot::operator-() const
{ 
     Carrot result = (*this);
     ... // chang the value to negative
     return result; 
} 
【注】1. 一元操作符, 取负.
· const Carrot Carrot::operator~() const
· bool Carrot::operator!() const
· bool operator>(const Carrot& lhs, const Carrot& rhs)
· bool operator<(const Carrot& lhs, const Carrot& rhs)
－ Carrot& Carrot::operator=(const Carrot& rhs)
{ 
     if(this == &rhs)
         return *this; // may be (*this) == rhs if needs.
     Barley::operator=(rhs); // if Carrot derived from Barley 
     ... // assignments every memeber of Carrot. 
     return *this; 
} 
【注】1. 为了实现形如 x=y=z=0 的串联赋值操作, operator= 必须传回 *this 的非常量引用. 
          2. 在赋值时应注意检查是否为自赋值 ( a = a ).
－ Carrot& Carrot::operator+=(const Carrot& rhs)
{ 
     ... 
     return *this; 
} 
【注】1. C++ 允许 (x += 1) = 0 形式的赋值操作, operator+= 必须传回 *this 的非常量引用.
· Carrot& Carrot::operator-=(const Carrot& rhs)
· Carrot& Carrot::operator*=(const Carrot& rhs)
· Carrot& Carrot::operator/=(const Carrot& rhs)
· Carrot& Carrot::operator%=(const Carrot& rhs)
· Carrot& Carrot::operator^=(const Carrot& rhs)
· Carrot& Carrot::operator&=(const Carrot& rhs)
· Carrot& Carrot::operator|=(const Carrot& rhs)
－ istream& operator>>(istream& _IStr, Carrot& rhs)
{ 
     ... 
     return _IStr; 
} 
【注】1. 为了遵照使用习惯(cin>>x 而不是 x>>cin), 对流操作的 operator>> 应为非成员函数.
－ ostream& operator<<(ostream& _OStr, const Carrot& rhs)
{ 
     ... 
     return _OStr; 
}
－ const Carrot operator>>(const Carrot& lhs, int rhs)
{ 
     Carrot result = lhs;
     ... 
     return result; 
} 
【注】1. 移位操作的重载方式.
· const Carrot operator<<(const Carrot& lhs, int rhs)
－ Carrot& Carrot::operator>>=(int rhs)
{ 
     ... 
     return *this; 
} 
【注】1. 移位操作.
· Carrot& Carrot::operator<<=(int rhs)
－ bool operator==(const Carrot& lhs, const Carrot& rhs)
{ 
     ... 
}
· bool operator!=(const Carrot& lhs, const Carrot& rhs)
· bool operator>=(const Carrot& lhs, const Carrot& rhs)
· bool operator<=(const Carrot& lhs, const Carrot& rhs)
－ bool operator&&(const Carrot& lhs, const Carrot& rhs)
X
{ 
     ... 
} 
【注】1. 基于以下原因, 你应该避免重载 operator&& 和 operator|| :
               (1). && 和 || 是一个逻辑操作符, 只对 bool 型别有才有明确的语意. 
               (2). 重载的 operator&& 和 operator|| 无法模拟操作符默认的[骤死式语义**[5]**](http://www.adintr.com/program/article/05.oper.html#z5).
               (3). 你无法保证重载后操作符的参数求值次序(C++ Stand 保证了默认的 && 和 || 按从左到右求值).
          2.自定义型别可以考虑提供到 bool 的转型操作来支持此操作符.
· bool operator||(const Carrot& lhs, const Carrot& rhs) 
X
－ Carrot& Carrot::operator++()
{ 
     (*this) += 1; // or other implement
     return *this;
} 
【注】1. 前置操作(prefix): ++carrot
－ const Carrot Carrot::operator++(int)
{ 
     Carrot oldValue = *this;
     ++(*this);
     return oldValue;
} 
【注】1. 后置操作(postfix): carrot++ 调用时编译器自动生成一个 0 作为参数.
          2. 为了禁止 carrot++++ 操作, 返回值应为 const.
          3. 从实现和参数可以看出,后置操作的效率远远低于前置操作, 所以如非必要尽量使用前置操作.
          4. 为了保证递增行为一致, 并便于维护后置操作最好使用前置操作来完成递增行为.
· Carrot& Carrot::operator--()
· const Carrot Carrot::operator--(int)
－ const Carrot operator,(const Carrot& lhs, const Carrot& rhs)
X
{ 
     ...
} 
【注】1. 基于以下原因, 你应该避免重载 operator, :
               (1). 即使没有重载, 默认,号操作符也可以施用于自定义型别上. 
               (2). C++ 保证 , 操作符的求值是从左到右的, 而重载后无法保证此行为.
－ const PMFC Carrot::operator->*(ReturnType (T::*pmf)()) const
{ 
     ...
} 
【注】1. operator->* 重载比较少见, 有兴趣可参考[Implementing operator->* for Smart Pointers](http://www.adintr.com/map.htm?id=tspcs02).
－ const Carrot* Carrot::operator&() const
X
{ 
     ...
} 
【注】1. 你应该尽量避免重载取地址操作符.
－ Coca& Carrot::operator*()
{ 
     ...
} 
【注】1. 重载提领操作符应提供 const 版和非 const 版.
· const Coca& Carrot::operator*() const
－ Coca* Carrot::operator->()
{ 
     ...
} 
【注】1. 重载 operator-> 应提供 const 版和非 const 版.
          2. operator-> 的返回值必须为一个指针或可以应用 -> 操作的类型.
          3. 如果 operator-> 的返回的不是一个指针, C++ 会继续在返回类型上应用 -> 直到得到一个指针为止.
· const Coca* Carrot::operator->() const
－ Coca& Carrot::operator[](KeyType index)
{ 
     ...
} 
【注】1. operator[] 应提供 const 版本和非 const 版.
          2. KeyType 可以为任意类型, 但通常为 int 型.
· const Coca& Carrot::operator[](KeyType index) const 
－ AnyType Carrot::operator()(...)
{ 
     ...
} 
【注】1. operator () 重载的是函数运算符, 改变表达式优先级的 () 是不能重载的.
          2. 重载了 operator() 的对象被成为仿函数, 使用方法和函数指针类似.
－ static void* Carrot::operator new(size_t size, ...)
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
【注】1. 参考[重载 operator new](http://www.adintr.com/program/article/05.oper.html#overloadnew).
· static void* Carrot::operator new[](size_t size, ...)
－ static void Carrot::operator delete(void * memory, size_t size)
{ 
     if(memorty == NULL)
         return;
     ...
} 
【注】1. 参考[重载 operator new](http://www.adintr.com/program/article/05.oper.html#overloadnew).
· static void Carrot::operator delete[](void * momery, size_t size)
▲注:
1.本文中成员函数仅指非静态成员函数；非成员函数包括自由函数，静态成员函数，友元函数。←
2.这种调用方法使用了 operator new 的一种重载形式: void * operator new(size_t, void * buff) . 这种重载方式直接返回了传入的内存地址, 所以这种调用方式中 operator new 其实什么也没做, 于是这个** new operator**调用只使用了其中调用构造函数的部分, 相当于是在调用 placement new. 该重载在 VC7.1 中定义如下:
#define __PLACEMENT_NEW_INLINE
inline void *__cdecl operator new(size_t, void *_Where) _THROW0()
{ // construct array with placement at _Where
return (_Where);
}
←
3.和 placement new 一样, nothrow new 也是 operator new 的一种重载形式, nothrow 是 中声明的一个对象. 该重载在 VC7.1 中的声明为:
...
struct nothrow_t
     { // placement new tag type to suppress exceptions
     };
extern const nothrow_t nothrow; // constant for placement new tag
...
void *__cdecl operator new(size_t, const std::nothrow_t&)
     _THROW0();
←
4.在 VC7.1 中, 不将 operator new 声明为静态函数仍然可以通过编译, 但是如果在函数里面使用了非静态成员则会产生编译错误. 可见, 即使不将 operator new 声明为静态函数编译器也会将其作为静态函数来处理的. 使用是即使编译器默认把 operator new 当作静态函数也应该显式的将其声明为 static 函数.
←
5. 即对 && 操作符如果左操作数为 false, 则直接返回 false 而不会去检测右操作数; 对 || 操作符如果左操作数为 true, 则直接返回 true 而不会检测右操作数的值. 于是类似这样的代码可以是安全的:
     if( (p != NULL) && strlen(p) < 10 )... 
如果 p == NULL, strlen 就不会被调用. 但是重载的 operator&& 和 operator|| 是作为一个函数, 要在所有的参数求值完成后才会调用此操作符.←
▲参考资料:
1. Bjarne Stroustrup.The C++ Programming Language(Special Edition)
2. Scott Meyers.Effective C++(2nd Edition)
3. Andrei Alexandrescu.Modern C++ Design
4. Robert B.Murray.C++ Strategies and Tactics
5. Scott Meyers.More Effective C++
6. John Lakos.Large-Scale C++ Software Design
7. Scott Meyers.Implementing operator->* for Smart Pointers
