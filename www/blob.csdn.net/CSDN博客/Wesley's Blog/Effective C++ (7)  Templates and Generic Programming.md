
# Effective C++ (7): Templates and Generic Programming - Wesley's Blog - CSDN博客


2018年09月05日 11:26:37[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：58


## Introduction
这一章主要介绍了模板编程, 最核心的思想就是尽可能地把运行期所做的事情移动至编译期完成. 最后还简要介绍了以下 TMP
## Rule 41: Understand implicit interfaces and compile-time polymorphism
了解隐式编程和编译期多态.
对于着一条 Rule, 需要了解到的就是 template 的多态与 classes 的多态有所不同, 一个是发生与编译期, 一个是发生于运行期. 就效率而言 template 更快. 但是 template 要保持接口的一致是隐式的(implicit), 就是说如果在写一个 template 函数中, 如果使用了 w.size() 这样的函数, 实际上也就隐式要求了这个 type 需要有size这个接口函数. 所以也增加了写代码的难度.
Remeber:
classes 和 templates 都支持接口(interfaces) 和 多态(polymorphism)
对 classes 而言接口是显式的(explicit), 以函数签名为中心. 多态则是通过 virtual 函数发生在运行期
对 template 参数而言, 接口是隐式(implicit)的, 基于有效表达式, 多态则是通过 template 具体化和函数重载解析发生于编译期
## Rule 42: Understand the two meanings of typename
看一端代码就好:
`template<typename/class C>  //template 和 class 是一样的
void print2nd(const C& container)
{
    typename C::const_iterator iter(container.begin()); //对于嵌套从属名称, 需要在前面加上typename
}`Remeber:
声明 template 参数的时候, class = typename
使用关键字 typename 表示嵌套从属类型名称, 但是不能在 base class lists(基类列) 或 member initialization list(成员初值列) 内以它作为 base class 修饰符
## Rule 43: Know how to access names in templatized base classes
当继承`base class templates`时候, 需要通过 ‘this->’ 来调用 base class templates 中的成员. 例如:
`template<typename Company>
class MsgSender{
public:
    ...
    void sendClear(const MsgInfo& info)
    {
        std::string msg;
        Company c;
        c.sendCleartext(msg);
    }
    void sendSecret(const MsgInfo& info)
    { ... }
};
template<typename Company>
class LoggingMsgSender: public MsgSender<Company>
{
public:
    void sendClearMsg(const MsgInfo &info)
    {
        ...
        sendClear(info);  // 无法通过编译
        this->sendClear(info);  // 通过编译
        ...
    }
};`这是因为编译器无法确定其中的Company是否有 sendClear 这个函数(考虑到特化的问题). 所以需要通过加上 “this->” 来向**编译器承诺 base class template 的任何特化版本都将支持其一般(泛化)版本所提供的接口**
除了加上 “this->” 之外还有另外两种方法:
使用 using 声明式, 例如 using MsgSender::sendClear
使用明确资格修饰符, 例如 MsgSender::sendClear(info). 不建议使用这种方法, 因为会关闭 ” virtual 绑定行为 “
Remeber:
**当继承 template class 的时候, 需要通过 “this->” 来调用 base class templates 内的成员名称, 或者通过写出 “base class 资格修饰符” 完成**
## Rule 44: Factor parameter-independent code out of templates
使用template可能会导致代码膨胀的问题. 特别要注意对于**非类型参数(non-type parameter)**, 例如:
`template<typename T, std::size_t n>
class SquareMatrix{
public:
    ...
    void invert();
};`上面的 size_t n 就是非类型参数, 当 n 取 5, 10 的时候, 就会产生两份代码, 除了常量5和10不同, 其他都相同, 这样就容易造成代码膨胀.
解决这种问题最好的方法就是, 将操纵的代码放在一个 base template class 中, 如果存在数据的问题就在 base template class 中保存一个指向数据的指针, 然后真正的数据放在 derived template class 中, derived class 接受非类型参数, derived template class 中除了存放数据, 其他的函数就直接调用 base 中的函数, 这样一来重复的代码只会有 derived class 中的代码.
例如:
`template<typename T>        // 减少代码膨胀
class SquareMatrixBase{
protected:
    SquareMatrixBase(std::size_t n, T* pMem):
        size(n), pData(pMem) {}
    void setDataPtr(T* ptr) { pData = ptr; }
    void invert(std::size_t matrixSize);
    ...
private:
    std::size_t size;
    T* pData;   // 存储数据指针
};
template<typename T, std::size_t n>
class SquareMatrix: private SquareMatrixBase<T>{
public:
    SquareMatrix() : SquareMatrixBase<T>(n, data) {}
    ...
private:
    T data[n*n];
};`除了这种由 non-type template parameters 之外, type template parameters 也会引起膨胀. 例如含有指针的时候`list<const int*>`和`list<const double*>`, 最好使用`void*`来代替.
Remeber:
Templates会根据参数生成多份代码, 容易造成代码冗余
因非类型模板参数(non-type template parameters)而造成的代码膨胀, 可通过 以函数参数或者 class成员变量替换template参数.
因类型参数(type parameters)而造成的代码膨胀, 可以让带有完全相同二进制表述(binary representation)的表现类型共享实现代码
## Rule 45: Use member function templates to accept “all compatible types”
运用成员函数模板接收所有兼容类型
举个例子, 希望能够实现:
`template<class T>
class shared_ptr{
public:
    template<class Y>
        explicit shared_ptr(Y* p);  // 只可以显示转换
    template<class Y>
        shared_ptr(shared_ptr<Y> const& r); // 允许隐式转换
    ...
};`这个就是在模板类中定义泛化模板函数.
Remeber:
请使用 member function templates(成员函数模板) 生成 “可接收所有兼容类型” 的函数
如果你声明了 member templates 用于”泛化 copy 构造” 或 “泛化 assignment 操作”, 你还是需要声明正常的 copy 构造函数和 copy assignment 操作符
## Rule 46: Define non-member functions inside templates when type conversions are desired
之前有提到过, 如果对于 “所有参数之类型转换” 的函数, 就把函数作为 non-member function, 再提供隐式类型转换.
不过对于 template class 却有不同. 需要将函数定义为 friend 函数, 并且就在类内部实现
`template<typename T>
class Ration{
public:
    ...
    friend const Rational operator*(const Rational& lhs, const Rational& rhs)    // 声明为 friend 函数
    {
        return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
    }   // 在类内部实现
};`Remeber:
**当编写一个 class template 的时候, 他所提供的函数支持 “所有参数之隐式类型转换” 时, 请将那些函数定义为 “class template 内部的friend函数”**
## Rule 47: Use traits classes for information and types
首先, 以 iterator 的5种不同类型作为引入:
input_iterator: 单向, 每次只能操作一次
output_iterator: 同上
forward_iterator: 可以做之前两种迭代器的任意一种, 并且可以操作多次
bidirectional_iterator: 可以双向操作
random_access_iteratror: 最强大, 以常量时间随意跳跃
好的, 下面介绍 Traints classes + 函数重载来解决下面这个问题: 我们需要写一个让 iterator 进行跳转的函数, 根据不同的 iterator 类型有不同的实现:
`template<typename IterT, typename DistT>
void advance(IterT &iter, DistT d);`那么我们要解决两个问题: 1. 如何判断 IterT 是哪种类型的 iterator.  2. 如果做到在编译器根据类型的不同有不同的实现
对于这种情况, 我们采用了 Traits classes 来让 iterator 夹带类型信息. 简而言之, 就是定义一个 iterator_traits, 并用它来传递类型信息, 并且在各个 iterator 自身定义 tag 信息, 而 iterator_traits 就是把这个 tag 信息传递出去
`template<typename IterT>
struct iterator_traits{     // IterT 的 iterator_category 其实就是用来表现 "IterT自己来说自己是啥", 所以说 iterator_traits 有个传递的作用
    typedef typename IterT::iterator_category iterator_category;  // typedef typename 用来告诉编译器是个别名, 不是类型
    ...
};
/* 对于指针迭代器提供一个偏特化版本 */
template<typename IterT>
struct iterator_traits<IterT*>{ 
    typedef random_access_iterator_tag iterator_category;
    ...
};
template <...>
class deque{
public:
    class iterator{
    public:
        typedef random_access_iterator_tag iterator_category; // 定义自己的类型, random_access_iterator_tag 是一个 struct
        ...
    };
    ...
};
/* 通过建立一个重载函数 doAdvance 接受traits信息,  和一个控制函数 advance来传递traits信息 */
template<typename IterT, typaname DistT>
void advance(IterT &iter, DistT d)
{
    doAdvance(iter, d, 
        typename std::iterator_traits<IterT>::iterator_category());
}
template<typename IterT, typaname DistT>
void doAdvance(IterT &iter, DistT d,
                    std::random_access_iterator_tag)
{
    iter += d; 
}
template<typename IterT, typaname DistT>
void doAdvance(IterT &iter, DistT d,
                    std::bidirectional_iterator_tag)
{
    if ( d >= 0) { while (d--) ++iter; }
    else { while(d++) --iter; }
}`Remeber:
Traits classes 使得”类型相关信息”在编译期可用. 他们以 templates 和 “templates 特化” 完成实现
整合重载技术(overloading)后, traints classes 有可能在编译期对类型执行 if…else 测试
## Rule 48: Be aware of template metaprogramming
模板元编程(TMP) 被证明是图灵完备的, 之前 Rule 47就是TMP的一个案例, 由于TMP还是比较麻烦并且不直观的, 所以这里简要介绍一个 TMP 的 Hello world, 求 factorial:
`template<unsigned n>
struct Factorial{
    enum { value = n * Factorial<n-1>::value };
};
template<>          //特化版本
struct Factorial<0>{
    enum { value = 1 };
};
int main(){
    std::cout<<< Factorial<10>::value ;
}`Remeber:
**Template metaprogramming(TMP, 模板元编程) 可将工作由运行期移往编译期, 因而得以实现早期错误侦测和更高的执行效率**
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

