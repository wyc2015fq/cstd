
# Effective C++ (1): Accustoming Yourself to C++ - Wesley's Blog - CSDN博客


2018年09月01日 17:48:55[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：46标签：[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[effective c++																](https://so.csdn.net/so/search/s.do?q=effective c++&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=c++&t=blog)个人分类：[计算机基础																](https://blog.csdn.net/yinanmo5569/article/category/7923781)



# 1. Accustoming Yourself to C++
## Introduction
本章是最基础的一点东西, 4条规则.
## Rule 01: View C++ as a federation of languages
将 c++ 视作一个语言联邦而不是一个单一的语言, 才能更好地理解它的特性. 可以认为 c++ 实际上是由四个子语言构成的:
C. 说到底最基本的语句还是以c为基础的
Object-Oriented C++. 这给 C++ 提供了三大特性, encapsulation, inheritance, polymorphism.
Template C++. 这是 c++ 泛型编程(generic programming)部分, 也是大多数程序员最为稀缺的一部分
STL. STL 是个 template 程序库, 对 容器(container), 迭代器(iterators), 算法(algorithm) 及函数对象(function objects) 有极佳的配合和协调
对于这四个次语言, 有时候高效法则会在四个次语言中规则发生变化. 例如对于 内置类型(也就是C-like)而言, pass-by-value 通常比 pass-by-reference 高效, 当你从 C part of C++ 转换到 Object-Oriented C++ 的时候, 由于 user-defined 构造函数和析构函数的存在, pass-by-reference-to-const 往往更好.
Remeber:
**C++ 高效编程守则视状况而变化, 取决于你使用C++的哪一个部分.**
## Rule 02: Prefer consts, enums, and inlines to \#defines
使用 consts 来替换 define, 能够方便调试, 并且优秀的编译器也不会造成额外的内存开销.
对于 “in class 常量值” 设定, 通常编译器不同写法也不同. 一般这样写:
`class GamePlayer{
private:
    static const int NumTurns = 5; // 声明式, 在头文件中
    int scores[NumTurns];
}
const int GamePlayer::NumTurns; // 定义式, 在实现文件中`如果编译器(错误地)不允许”static 整数型 clas 常量” 完成 “in class 初值设定”, 可以改用所谓的 “the enum hack” 补偿做法.
`class GamePlayer{
private:
    enum {NumTurns = 5};
    int scores[NumTurns];
}`对于 \#define 函数而言, 不是type safety的, 所以建议换成类型安全的 template inline 函数:
`template<typename T>
inline void callWithMax(const T& a, const T& b)
{
    f(a > b ? a: b);
}`Remeber:
对于单纯常量, 最好以 const 或者 enums 替换 \#defines
对于函数的宏, 以 inline 替换 \#defines
## Rule 03: Use const whenever possible
首先, 区分 const-pointer 和 const-data:
`char greeting[] = "Hello";
char *p = greeting;
const char *p = greeting;   // const-data
char const *p = greeting;   // same const-data
char* const p = greeting;   // const-pointer
const char* const p = greeting;     // const-pointer, const-data`对于迭代器而言,`const iterator == const pointer`,`const_iterator == const data`.
`std::vector<int> vec;
const std::vector<int>::iterator iter = vec.begin();
*iter = 10; //正确
++iter;     //错误, const-pointer
std::vector<int>::const_iterator cIter = vec.begin();
*iIter = 10;    // 错误, const-data
++cIter;    //正确`有趣的是,**声明返回值为 const 有奇效**, 可以防止程序员意外将 ‘==’ 键入乘 ‘=’, 但是并不会影响正常使用 :
`class Rational{...}
const Rational operator* (const Rational& lhs, const Rational& rhs);
if (a * b = c) ...  // 将报错
Rational x = a * b;  // 正常`声明 const 函数, 则函数内不能修改任何成员变量,`mutable`变量除外.  有时候我们需要对 const 对象进行不同的处理, 例如下面这段代码, 对于const 对象的`[]`我们希望返回个const对象避免对象被修改. 而非const 对象则不需要, 可以这样写:
`class TextBlock{
public:
    const char& operator[](std::size_t position) const
    {
        ...
        return text[position];
    }
    char& operator[](std::size_t position) const
    {
        ...
        return 
            const_cast<char&>(
                static_cast<const TextBlock&>(*this)
                    [position]
            );
    }`通过 const_cast, 和 static_cast 来类型强制转化复用代码
Remeber:
尽可能使用const来帮助编译器侦测出错误. const可以被用在对象, 参数, 返回类型, 成员函数本身.
const和non-const成员函数有着实质等价的实现的时候, 令non-const版本调用const对象可避免代码重复.
## Rule 04: Make sure that objects are initialized before they’re used
要区分初始化和赋值操作,**初始化操作是在构造函数调用之前执行的**. 所以可能的话, 尽量把所有成员变量初始化操作放在`member initialization list`中, 效率更高, 对于const 成员变量则必须放到 member initialization list 中, initiate list 会调用成员变量的**copy 构造函数**.
另外对于”跨编译单元内使用 non-local static 对象”时候, 请采取将 local static 对象替换 non-local static 对象的方式, 来保证初始化次序.(类似于单例的方法)
`class FileSystem { ... };
FileSystem& tfs(){
    static FileSystem fs;
    return fs;
}
class Directory { ... };
Directory::Directory( params )
{
    ...
    std::size_t disks = tfs().numDisks();
    ...
}`Remeber:
构造函数最好使用成员初值列( member initialization list ), 而不要在构造函数中采用赋值操作
为免除 ” 跨编译单元之初始化次序 ” 问题, 请以 local static 对象替换 non-local static 对象
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

