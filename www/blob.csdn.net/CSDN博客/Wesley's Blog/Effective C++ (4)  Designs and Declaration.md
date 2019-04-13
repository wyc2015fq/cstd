
# Effective C++ (4): Designs and Declaration - Wesley's Blog - CSDN博客


2018年09月02日 17:06:24[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：34


## Introduction
这一章主要讲述了如何去设计对象, 接口.
## Rule 18: Make Interfaces easy to use correctly and hard to use incorrectly
在设计接口的时候, 应该尽量做到, 如果使用者能够通过编译, 那么就应该能得到他所期望的结果. 如果有任何错误, 应该尽量提前至编译期就能够报出来.
例如:
class Date{
public:
Date(int month, int day, int year);
…
}
在使用时, 可能客户会弄错 day 和 month 的位置, 产生错误, 那么可以定义`类型系统(type system)`来避免这种问题, 定义类似于`struct Day, struct Month, struct Year`, 并且在类型系统中限制变量的范围等.
在设定资源获取接口的时候, 通过返回`shared_ptr`能够阻止一大群用户犯下资源泄漏的问题.
`std::tr1::shared_ptr<Investment> createInvestment()
{
    std::tr1::shared_ptr<Investment> retVal(static_cast<Investment*>(0), getRidOfInvestment);    // declare null pointer
    retVal = ...;   // point to the right object
    return retVal;
}`如果能够直接将pInv指向正确的对象会比先指向null, 再指向正确的对象更好.  这样做最大的好处是能够放置 “`cross-DLL problem`“, 就是解决 “跨 DLL 之 new/delete 成对应用” 的问题, 经常我们在一个文件new了却忘了在另一个文件delete, 使用 shared_ptr 可以避免这一点.
Remeber:
设计接口应该容易被正确使用, 不容易被错误使用. 当错误使用的时候应该尽可能提前报错
“促进正确使用” 的办法包括接口的一致性,以及与内置类型的行为兼容
“阻止勿用” 的办法包括建立新类型, 限制类型上的操作, 束缚对象值, 以及消除客户的资源管理责任
shared_ptr 支持定制型删除器( custom deleter ), 可防范 cross-DLL problem
## Rule 19: Treat class design as type design
在设计一个类之前, 请先考虑如下问题:
新type的对象应该如何被创建和销毁?影响到你的构造函数, 析构函数, 以及内存分配函数和释放函数.
对象的初始化和赋值有怎样的区别?区分出如何书写 copy assignment 函数
对象如果被pass-by-value的时候意味着什么?思考如何书写拷贝构造函数
什么是新 type 的”合法值”?考虑建立值的约束, 异常抛出等
新type需要配合某个继承图系( inheritance graph )吗?影响到 virtual 函数
新type需要怎样的转换?对于彼此有转换行为的类型, 需仔细考虑
什么样的操作符对于新type是合理的?考虑到重载操作符等
谁会使用新type的成员?决定成员的 public, private, protected, 也帮助决定哪个 classes 或 functions 是 friends.
什么是新type的”未声明接口” (undeclared interface)?它对效率, 异常安全以及资源是用提供何种保证?
你的新type有多么一般化?考虑到 template 的问题
你真的需要一个新type吗?如果只是新加个简单的机能, 可能单纯定一个 non-member 函数或 templates 更有效
Remeber:
**Class的设计就是type的设计. 在定义一个新type之前, 请确定你已经考虑过了本条款覆盖的所有讨论主题.**
## Rule 20: Prefer pass-by-reference-to-const to pass-by-value
slicing problem说的是当一个derived对象以by-value方式传递并被视为一个base class对象的时候, base class对象的copy构造函数会被调用, 并且 “造成此对象的行为像一个 derived class对象”
Remeber:
尽量以 pass-by-reference-to-const 替换 pass-by-value, 前者通常比后者高效, 并且避免切割问题(slicing problem)
以上规则并不适用于内置类型, 以及STL的迭代器和函数对象. 对它们而言, pass-by-value 往往比较合适
## Rule 21: Don’t try to return a reference when you must return an object
在该返回值的时候还是需要返回值的. 一般返回有以下几种可能:
返回值. 承受该有的构造和析构成本
返回指向 heap-allocated 对象的指针. 传递指针的方式会快一点.
返回指向 static 变量的 reference. 一般用于单例模式.
Remeber:
**绝对不要返回一个pointer或reference指向一个local stack对象, 或返回一个 reference 指向一个 heap-allocated 对象, 或返回 pointer 或 reference 指向一个 local static 对象但是却不是单例.**
## Rule 22: Declare data members private
成员变量的封装性与”成员变量的内容改变时所破坏的代码数量”成正比. 所以无论是使用protect还是public, 当一个变量移除的时候, 所有使用它的客户代码都会被破坏. 而如果全部使用函数来获取变量, 那么可以通过别的途径来实现这个函数.
Remeber:
切记将成员变量声明为 private. 这可赋予客户访问数据的一致性, 可细微划分访问控制, 允诺约束条件获得保证, 并提供 class 作者以充分的实现弹性.
protected 并不比 public 更具封装性.
## Rule 23: Prefer non-member non-friend functions to member functions
举个例子, 对于浏览器:
`class WebBrowser{
public:
    ...
    void clearCache();
    void clearHistory();
    void removeCookies();
    ...
}`如果要写一个函数, 执行以上三个成员函数, 那么我是写一个 clearEverything() 作为成员函数好一些呢? 还是写一个 non-member, non-friend 函数好一些呢?
封装性可理解为, 让尽量少的函数可以访问类的数据.
从这种层面上来说, non-member, non-friend 函数会好一些.
Remeber:
**non-member non-friend functions 相较于 member functions 更好, 能够增加类的封装性, 包裹弹性( packaging flexibility ).**
## Rule 24: Declare non-member functions when type conversions should apply to all parameters
如果你需要为某个函数的所有参数进行类型转换的, 那么这个函数必须是个 non-member 的.
以有理数与整数相乘为例子:
class Rational{
public:
Rational(int numerator = 0, int denominator = 1); /// 构造函数不为 explicit, 允许 int-to-Rational 隐式转换
int numerator() const;
int denominator() const;
…
private:
…
};
如果将operator* 作为成员函数来写的话:
`class Rational{
public:
    ...
    const Rational operator* (const Rational& rhs) const;
};`那么在类型转换的时候会出现问题:
`Rational oneHalf(1,2);
Rational result;
result = onHalf*2;  // 没问题, 会先将 2 隐式转换为 Rational, 然后再相乘
result = 2* onHalf; // 报错, Rational不在int构造函数参数列表中, 不会进行隐式类型转换.`在这种时候, 需要对operator所有参数均进行类型转换, 只能通过 non-member function 来实现:
`class Rational{
    ...
};
const Rational operator*(const Rational& lhs, const Rational& rhs)
{
    return Rational(lhs.numerator() * rhs.numerator(),
            lhs.denominator() * rhs.denominator());
}`这样就能解决刚刚类型转换的问题, 那么这个函数要不要设置为friend函数呢? 在不需要使用到class成员变量就能够达到目的的情况下, 不需要设置为 friend , 可以提高封装性.
Remember:
**如果你需要为某个函数的所有参数进行类型转换的(包括this指针所隐喻参数), 那么这个函数必须是个 non-member 的.**
## Rule 25: Consider support for a non-throwing swap
这条rule解释起来比较麻烦. 我们先要了解 std swap 是如何实现的:
`template<typename T>
void swap(T &a, T &b){
    T temp(a);
    a = b;
    b = temp;
}`整个过程包括3次复制, 所以效率在某些情况是很低的. 比如一个类里面只有一个指针的时候, swap实质上只是交换指针所指的东西, 而通过 std::swap 则还会复制指针所指向的内容, 造成效率低下:
`class Widget{
public:
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs)
    {
        ...
        *pImpl = *(rhs.pImpl);   //复制了指针指向之物
        ...
    }
private:
    WidgetImpl* pImpl;
}`对于上面这种情况, 就需要制作自己特化的swap了. 遵从以下步骤:
提供一个 public swap 函数, 并保证不抛出异常.
class Widget{
public:
    ...
    void swap(Widget& other)
    {
        using std::swap;
        swap(pImpl, other.pImpl);
    }
    ...
};
在你的 class 的命名空间内提供一个 non-member swap, 并令它调用上述 swap成员函数.
namespace WidgetStuff{
    ...
    template<typename T>
    class Widget { ... };
    ...
    template<typename T>
    void swap(Widget<T>& a, Widget<T>& b)
    {
        a.swap(b);
    }
}
如果你正在编写一个 class (而非 class template), 为你的 class 特化 std::swap. 并令它调用你的 swap 成员函数.
namespace std{
    template<>              // 修订后的 std::swap 特化版本
    void swap<Widget>( Widget &a, Widget &b)
    {
        a.swap(b);
    }
}
Remeber:
当 std::swap 对你的类型效率不高的时候, 提供一个 public swap 成员函数, 并确保该函数不抛出异常.
然后再实现一个 non-member swap 来调用前者, 对于 classes(而非 templates), 也请特化 std::swap
调用swap的时候, 请先使用 using std::swap, 然后再不带任何”命名空间资格修饰”调用swap
## 系列文章
[Effective C++ (1): Accustoming Yourself to C++](https://blog.csdn.net/yinanmo5569/article/details/82289290)
[Effective C++ (2): Constructors, Destructors, and Assignment Operators](https://blog.csdn.net/yinanmo5569/article/details/82290194)
[Effective C++ (3): Resource Management](https://blog.csdn.net/yinanmo5569/article/details/82317019)
[Effective C++ (4): Designs and Declaration](https://blog.csdn.net/yinanmo5569/article/details/82317034)
[Effective C++ (5): Implementation](https://blog.csdn.net/yinanmo5569/article/details/82346893)
[Effective C++ (6): Inheritance and Oject-Oritent Design](https://blog.csdn.net/yinanmo5569/article/details/82351493)
[Effective C++ (7): Templates and Generic Programming](https://blog.csdn.net/yinanmo5569/article/details/82420021)
[Effective C++ (8): Customizing new and delete](https://blog.csdn.net/yinanmo5569/article/details/82419808)
[Effective C++ (9): Miscellany](https://blog.csdn.net/yinanmo5569/article/details/82419858)

