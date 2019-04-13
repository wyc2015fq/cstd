
# Effective C++ (5): Implementation - Wesley's Blog - CSDN博客


2018年09月03日 12:08:43[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：83


## Introduction
本章讨论了 变量声明, 变量转换(casting), 变量返回, 异常处理, inline, 解决编译依赖等问题. 信息量较大也涉及到了设计模式的问题, 需仔细咀嚼.
## Rule 26: Postpone variable definitions as long as possible
尽量延后程序定义式的出现, 因为考虑到如果出现 exceptions 的话, 过早定义大量变量会造成不必要的构造和析构函数时间的浪费. 例如:
`string encryptPassword(const string& password)
{
    string encrypted;
    if (password.length() < MinimumPasswordLength){
        throw logic_error("Password is too short");
    }
    ...
    return encrypted;
}`如果在中途抛出 logic_error, 就会造成 encrypted 的构造函数析构函数时间的浪费.
Remeber:
**尽量延后变量定义式的出现. 这样做可增加程序的清晰性并改善程序的效率.**
## Rule 27: Minimize casting
C风格旧式转型:
`(T)expression
T(expression)`C++新式转型(cast):
`const_cast<T>(expression)   // 常量性转除 cast away the constness
dynamic_cast<T>(expression) // "安全向下转型", 将一个基类对象指针（或引用）cast到继承类指针
reinterpret_cast<T>(expression) // 意图执行低级转型, 用的较少
static_cast<T>(expression)  //强迫隐式转换, non-cast转cast, int转double等等, 但是不能 const 转 non-const`新式转型更好使用, 一是易于识别, 二是目标更加明确化
Remeber:
**如果可以尽量避免转型, 特别在注重效率的代码中避免 dynamic_casts. 最好使用c++新式转型**
## Rule 28: Avoid returning “handles” to object internals
避免返回 handles 指向对象内部成分, 首先, 是为了提高函数的封装性.
首先考虑一个例子, 返回矩阵的顶点:
class Rectangle{
public:
…
Point& upperLeft() const { return pData->ulhc; }
…
}
对于上面这种情况, 虽然函数声明为const, 却把内部private变量的引用传出去了, 这个时候 private 变量不再 private, 可以任意修改. 目前较新的编译器不会通过上述编译:
`main.cpp:30:10: error: binding ‘const Point’ to reference of type ‘Point&’ discards qualifiers`所以说, 有两种写法:
硬是要返回内部变量的 reference (破坏封装)
class Rectangle{
public:
    ...
    Point& upperLeft() { return pData->ulhc; }
    ...
}
返回值, 保持封装, 安全性
class Rectangle{
public:
    ...
    Point upperLeft() const { return pData->ulhc; }
    ...
}
Remeber:
**避免返回 handles 指向对象内部成分(包括成员变量, 函数指针, 迭代器). 这样可以提高类的封装性.**
## Rule 29: Strive for exception-safe code
着一条Rule的信息量较大, 考虑到了异常处理的问题.
首先以例子为引入, PrettyMenu 是一个用来表现背景团的GUI菜单 class, 该class期望多线程环境, 下面是`PrettyMenu`的`changeBackground`函数的一个实现:
`void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    lock(&mutex);
    delete bgImage;
    ++imageChanges;
    bgImage = new Image(imgSrc);    // 该构造函数可能发生异常
    ulock(&mutex);
}`上面这段代码就不是个异常安全函数, 因为一旦构造函数发生异常, 那么会产生**资源泄漏**和**数据败坏**. 因为互斥锁已经被锁住, bgImage指向一个删除的对象, 并且 imageChanges也被累加.
`异常安全函数(Exception safety)`在异常抛出的时候会:
不泄露任何资源
不允许数据败坏
异常安全函数除了满足上面两个基本条件之外, 还有有三个级别:
基本承诺: 程序内的任何食物仍然保持在有效状态下
强烈保证: 类似于原子性(Atomic), 如果函数成功就完全成功, 如果失败, 则恢复到”调用之前状态”
不抛出(nothrow)保证: 最高承诺, 承诺绝不抛出异常, 作用于内置类型(如 int, 指针等等) 上的所有操作都是 nothrow 的
为了满足基本承诺, 上面代码可以这样实现:
`class PrettyMenu{
    ...
    std::tr1::shared_ptr<Image> bgImage;
};
void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    Lock m1(&mutex);
    bgImage.reset(new Image(imgSrc));
    ++imageChanges;
}`这里使用了第三章提到的 RAII 用对象管理资源的思想, 使用了智能指针保证对象及时释放. 这样做, 就提供了基本安全保证.
但是考虑到, 可能在 Image 构造函数抛出异常的时候, 有可能输入流(input stream) 的读取记号(read marker)已被移走,  就是说之前的状态已被改变(可能bgImage.reset已经发生了一部分, 其实按道理是不会发生的, 我们假装可能吧hhh), 这样就没有满足强烈保证.
要满足强烈保证, 我们可以通过`copy-and-swap`的方式实现,**原则就是: 为你打算修改的对象做一份副本, 然后在副本上修改, 在修改确认没问题之后, 再与原件做 swap, 这样保证不出错**
要这样做的话写法如下:
`void PrettyMenu::changeBackground(std::istream& imgSrc)
{
    using std::swap;
    Lock m1(&mutex);
    std::tr1::shared_ptr<PMImpl> pNew(new PMImpl(*pImpl));  // 建立 copy
    pNew->bgImage.reset(new Image(imgSrc)); // 修改副本
    ++pNew->imageChanges;
    swap(pImpl, pNew);  // swap
}`Remeber:
异常安全函数( Exception-safe functions ) 即使发生异常也不会泄漏资源或允许任何数据结构败坏. 这样的函数分为三种级别(由低到高): 基本型, 强烈型, 不抛异常型
“强烈保证”往往能够以 copy-and-swap 实现出来, 但”强烈保证”并非对所有函数都可实现或具备实现意义
函数提供的”异常安全保证”通常是它所调用的各个函数的”异常安全保证”最弱者
## Rule 30: Understand the ins and outs of inlining.
inline的整体观念是”对此函数的每一个调用”都以函数本体替换之.
这样做可能会增加你的目标码(object code)的大小. inline 造成的代码膨胀也可能会导致额外的换页行为(paging), 降低指令高速缓存装置的击中率(instruction cache hit rate).
对 virtual 函数 inline 都是无用的, inline在编译期进行了替换, 而virtual则是运行期.
不应该对构造函数 和析构函数进行inline, 会使你的代码膨胀.
Remeber:
**大多数 inline 限制在小型和被频繁调用的函数上, inline对于编译器来说只是一个建议, 并不是强制执行的. inline也可能会造成代码膨胀问题**
## Rule 31: Mnimizing compilation dependencies between files
当某个文件 \#include 了另外一个 classes 的定义式, 那么当其中任何一个文件发生修改的时候, 整个项目都需要重新编译. 这将会耗费大量时间.
对于这种问题, 我们可以用**前置声明**的方式来解决.
`#include <string>   // string 是一个 typedef 为 basic_string<char>
class Date;
class Address;
class Person{
public:
    Person(const std::string &name, const Date &birthday, const Address &addr);
    std::string name() const;
    std::string birthDate() const;
    std::string address() const;
    ...
};`如果 Person 里只含 Address 和 Date 的指针还好, 如果有对象声明, 那么编译器必须在编译期知道对象的大小:
`int main()
{
    Person p(params); // 编译器需要知道大小
};`对于上面这种情况, 编译器就无法知道如何分配空间, 会报错, 对于这种问题, 有两种解决办法:
Handle classes
把 Person 分为两个 classes, 一个提供接口, 一个负责实现.
\#include<string>
\#include<memory>
class PersonImpl;
class Date;
class Address;
class Person{
public:
    Person(const std::string &name, const Date &birthday, const Address &addr);
    std::string name() const;
    std::string birthDate() const;
    std::string address() const;
    ...
private:
    std::tr1::shared_ptr<PersonImpl> pImpl;
};对象只含一个指针对象, 指向实现类, 这种设计被称为 pImpl idiom. 让接口和实现相分离. 这样做有个确定就是每个访问都增加了一个间隔.
Interface classes
这个思想就是使用c++的abstract base class(抽象基类)描述 derived classes的接口.通常不带成员变量, 没有构造函数, 只有一个virtual析构函数以及一组pure virtual函数
class Person{
public:
    virtual ~Person();
    virtual std::string name() const = 0;
    virtual std::string birthDate() const = 0;
    virtual std::string address() const = 0;
    ...
};然后使用这个类的客户必须以 Person 的 pointers 和 references 来撰写应用程序. 这个时候, 还需要使用factory(工厂)模式:
class Person{
public:
    ...
    static std::tr1::shared_ptr<Person> 
        create(const std::string& name, 
                const Date& birthday,
                const Address& addr);
    ...
};通常会采取上面这种工厂函数 或者 virtual 构造函数, 工厂函数返回指针(最好是智能指针), 指向分配对象.函数通常被声明为 static.
使用interface classes的成本就是每个函数都是virtual, 增加了个virtual pointer的跳转时间.
Remeber:
支持”编译依存性最小化”的一般构想是: 依赖于声明式, 不要依赖于定义式. 基于此构想的两个手段是 Handle classes 和 Interface classes
程序库头文件应该以”完全且仅有声明式”( full and declaration-only forms ) 的形式存在.
系列文章
[Effective C++ (1): Accustoming Yourself to C++](https://blog.csdn.net/yinanmo5569/article/details/82289290)
[Effective C++ (2): Constructors, Destructors, and Assignment Operators](https://blog.csdn.net/yinanmo5569/article/details/82290194)
[Effective C++ (3): Resource Management](https://blog.csdn.net/yinanmo5569/article/details/82317019)
[Effective C++ (4): Designs and Declaration](https://blog.csdn.net/yinanmo5569/article/details/82317034)
[Effective C++ (5): Implementation](https://blog.csdn.net/yinanmo5569/article/details/82346893)
[Effective C++ (6): Inheritance and Oject-Oritent Design](https://blog.csdn.net/yinanmo5569/article/details/82351493)
[Effective C++ (7): Templates and Generic Programming](https://blog.csdn.net/yinanmo5569/article/details/82420021)
[Effective C++ (8): Customizing new and delete](https://blog.csdn.net/yinanmo5569/article/details/82419808)
[Effective C++ (9): Miscellany](https://blog.csdn.net/yinanmo5569/article/details/82419858)

