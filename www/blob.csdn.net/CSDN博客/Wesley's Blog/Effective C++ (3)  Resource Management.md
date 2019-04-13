
# Effective C++ (3): Resource Management - Wesley's Blog - CSDN博客


2018年09月02日 17:05:27[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：45


## Introduction
这一章的核心是讲了 RAII (Resource Acquisition Is Initialization) 的概念, 并且在书写相关代码的时候避免入坑.
## Rule 13: Use objects to manage resources
将资源放到对象中, 当离开作用域, 调用对象的析构函数的时候, 再自动把资源给释放掉, 这样就能保证资源被合理的释放.
基于这种思想, 就有了 auto_ptr 和 shared_ptr. 这两个的主要区别是, shared_ptr 在 auto_ptr 的基础上加上了引用计数, 避免了多个指针指向同一个对象而导致释放时候所产生的多次 delete 的问题. shared_ptr 也称为智能指针 ( reference-counting smart pointer, RCSP ). “以对象管理资源”(Resource Acquisition Is Initialization, RAII) 有两个关键想法:
获得资源后立刻放入管理对象.
管理对象(managing object) 运用析构函数确保资源被释放
举个例子:
`void f()
{
    ...
    std::tr1::shared_ptr<Investment> pInv1(createInvestment());
    std::tr1::shared_ptr<Investment> pInv2(pInv1);
    pInv1 = pInv2;
    ...
}   // 离开作用域后, 它们所指的对象也自动被销毁`但是, 需要注意的是, 使用智能指针来指向数组需要通过声明`array_deleter`来保证调用 delete[] 合理释放:
`std::shared_ptr<int> sp(new int[10], array_deleter<int>());`Remeber:
**为防止资源泄漏, 请使用 RAII 对象. 然后建议使用 shared_ptr**
## Rule 14: Think carefully about copying behavior in resource-managing classes
对于RAII对象, 应该好好考虑它的复制行为.
允许复制? 那么应该考虑是 深拷贝( deep copying ) 还是 浅拷贝 ( shadow copying ) 如果是浅拷贝, 可以采用智能指针引用计数的方法来解决, 也就是说 RAII 对象中保留的是指向资源的智能指针, 这样可以保证复制时只是复制指针, 在释放时又不会出错.
例如:
`class Lock{
    public:
        explicit Lock(Mutex* pm): mutexPtr(pm, unlock) //以某个mutex初始化shared_ptr, 并且以unlock函数为删除器
        {
            lock(mutexPtr.get());
        }
    private:
        std::tr1::shared_ptr<Mutex> mutexPtr;
    };`不允许复制? 那么记得显示声明不允许拷贝, 避免编译器自动生成拷贝函数, 拷贝赋值操作符. 或者采用之前提到的继承 Uncopyable 的方法.
Remeber:
**普通而常见的 RAII class copying 行为是: 抑制 copying, 采用智能指针. 不过也有可能进行深拷贝或者别的操作**
## Rule 15: Provide access to raw resources in resource-managing classes
API通常要求的是原始资源的指针, 这时候对于RAII对象, 就需要有个能返回原始资源的接口. 通常有两种方式来解决这种问题:`显示转换, 隐式转换`
显示转换就是通过类似于 get() 这种方法来实现, shared_ptr 和 auto_ptr 都有提供一个get成员函数, 用来返回内部资源的原始指针:
`std::tr1::shared_ptr<Investment> pInv(createInvestment());
int daysHeld(const Investment* pi);
int days = daysHeld(pInv.get());`还有隐式转换的方法, 能够略去每次调用 get() 的麻烦, 不过容易造成错误, 不建议使用.
Remeber:
**APIs通常要求访问原始资源(raw resources), 所以每一个RAII class应该提供一个”取得其所管理资源”的办法. 一般而言显示转换比较安全**
## Rule 16: Use the same form in corresponding uses of new and delete
Remeber:
**如果new的时候使用了[], delete的时候也要使用[]. 如果new没有使用则delete也不用使用**
## Rule 17: Store newed objects in smart pointers in standalone statements.
使用独立的语句将newed对象放入智能指针中, 以免由于异常抛出导致难以察觉的资源泄漏.
比如说调用函数 processWidget:
`processWidget(std::tr1::shared_ptr<Widget>(new Widget), priority());`以上代码只能保证 new Widget 在调用 shared_ptr 构造函数之前使用, 但是如果是以下执行顺序:
执行 new Widget
执行 priority()
调用 shared_ptr 构造函数
如果在priority()中抛出异常, 没有执行 shared_ptr 构造函数, 那么 new Widget 对象将不会被合理释放, 造成内存泄漏.
Remeber:
**使用独立的语句将newed对象放入智能指针中, 以免由于异常抛出导致难以察觉的资源泄漏.**
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

