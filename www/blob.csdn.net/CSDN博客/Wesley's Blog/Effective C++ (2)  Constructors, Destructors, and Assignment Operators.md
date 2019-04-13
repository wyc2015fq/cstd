
# Effective C++ (2): Constructors, Destructors, and Assignment Operators - Wesley's Blog - CSDN博客


2018年09月01日 20:02:29[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：43


# 2. Constructors, Destructors, and Assignment Operators
## Introduction
如题, 书写类的构造函数, 析构函数, 复制构造函数的时候需要遵守以下原则, 以防出错.
## Rule 05: Know what functions C++ silently writes and calls
编译器会暗自为 class 生成 default 构造函数, copy 构造函数, copy assignment操作符, 以及 default 析构函数.
如果用户自定义了以上任意一种函数, 则编译器不会再自动生成对应的函数.
另外,**对于内含 const 成员变量 或者 reference 变量的类**, 编译器看到赋值操作会报错, 对于这样的类,**用户必须自定义 copy assignment 操作符**.
Remeber:
编译器会暗自为 class 生成 default 构造函数, copy 构造函数, copy assignment操作符, 以及 default 析构函数.
## Rule 06: Explicitly disallow the use of complier-generated functions you do not want
有时候我们希望某个对象是单一存在的, 所以也不希望它被复制. 这个时候我们就得**显示声明一个不被实现的 private 的拷贝构造函数和 copy assignment 函数来避免系统默认生成构造函数.**
`class HomeForSale{
public:
    ...
private:
    ...
    HomeForSale(const HomeForSale&);
    HomeForSale& operator=(const HomeForSale&);
};`不过上面还一个小漏洞就是依旧有可能会被在 成员函数 或 firend 函数中调用. 这个时候报错的就是 连接器报错而不是编译器.
为了提前诊断这种错误, 让错误在编译器报出来, 最好是设置一种 Uncopyable 的基类:
`class Uncopyable{
protected:
    Uncopyable() {}     //允许构造析构
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);  //不允许copy
    Uncopyable& operator=(const Uncopyable&);
}`然后其他不允许copy的类直接继承就好了.
Remeber:
**对于不允许copy的类, 可通过将成员函数生命为private并且不予实现, 或者用像 Uncopyable 这样的 base class来实现**
## Rule 07: Declare destructors virtual in polymorphic base classes
这个经常被作为面试题提问, 就是为什么要将base class的析构函数设置为 virtual 的. 之所以这样做是为了当 base 指针指向 derived 对象的时候, delete base指针可能 derived 对象的成员变量没被销毁, 这样就会带来很多问题. 所以需要将base class的析构函数设置为 derived
Remeber:
**将base class的析构函数设置为 virtual 的**
## Rule 08: Prevent exceptions from leaving destructors
析构函数不要抛出异常, 抛出异常通常会造成不明确行为或者程序结束. 解决这个的方法有两种:
吞下异常
DBConn::~DBConn()
{
    try{ db.close(); }
    catch(...){
        ... // 写log
    }
}
结束程序
DBConn::~DBConn()
{
    try{ db.close(); }
    catch(...){
        std::abort();
    }
}
最好的办法是给一个接口给用户自己调用来处理异常的机会, 然后如果用户没有调用接口则由析构函数来调用, 又回到了那两种方法
Remeber:
**析构函数决定不要吐出异常. 析构函数应该吞下异常或者结束程序**
## Rule 09: Never call virtual funcitions during construction or desctruction
不要在构造函数或者析构函数中调用 virtual 函数, 因为构造函数在构造 base class 的时候, 类型还只是 base class, 没办法多态调用虚函数. 析构函数也是同理.
这个问题可以通过” 令 derived classes 将必要的构造信息向上传递至 base class 构造函数” 来解决.
Remeber:
**在构造函数和析构函数期间不要调用虚函数.**
## Rule 10: Have assignment operators return a reference to *this
这条就比较简单了, 就是为了解决连续赋值的问题, 所以需要返回 *this 的引用
`class Widget{
public:
    ....
    Widget& operator=(const Widget& rhs)
    {
        ...
        return *this;
    }
    ...
};`对于其他的赋值运算也是如此,`+= -= *=`等等
Remember:
**令赋值操作返回 reference to *this**
## Rule 11: Handle assignment to self in operator =
在书写赋值操作符的时候, 还是需要考虑以下自我赋值的情况. 如果语句没有整理好, 可能会造成意想不到的问题, 例如:
`Widget& Widget::operator=(const Widget& rhs)
{
    delete pb;
    pb = new Bitmap(*rhs.pb);
    return *this;
}`这种情况可以通过证同测试, copy and swap, 调整语句顺序的方式达到.
Remeber:
**确保当对象自我赋值时operator=有良好行为. 其中技术包括比较”来源对象”和”目标对象”的地址, 精心周到的语句顺序以及 copy-and-swap.**
## Rule 12: Copy all parts of an object
Copying 函数应该确保复制”对象内的所有成员变量”及”所有 base class 成分.
对于base class成分:
`DerivedClass::DerivedClass(const DerivedClass& rhs): BaseClass(rhs), 
priority(rhs.priority)
{
    logCall("DerivedClass copy constructor");
}
DerivedClass& DerivedClass::operator=(const DerivedClass& rhs)
{
    logCall("DerivedClass copy assignment operator");
    BaseClass::operator=(rhs);
    priority = rhs.priority;
    return *this;
}`Remeber:
**Copying 函数应该确保复制”对象内的所有成员变量”及”所有 base class 成分**
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

