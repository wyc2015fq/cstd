# C++一种智能指针的实现 - 电影旅行敲代码 - CSDN博客
2015年11月04日 11:29:19[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：561
# 引子
- C++智能指针shared_ptr<>
- 以对象管理资源
- 一种智能指针的实现
## C++智能指针shared_ptr<>
为了兼容C语言，并且由于垃圾回收的性能原因，C++没有引入垃圾回收。但是动态内存分配又是现实编码中不可或缺的一部分，由程序员自己控制分配和回收。C++11添加了智能指针shared_ptr<>，相当于使用另一种方式间接实现了部分垃圾回收。
shared_ptr是模板，可以指向多个类型的对象，并且允许多个指针指向同一个对象，通过引用计数的思想实现垃圾回收。引用计数实现的垃圾回收性能比较高，但是无法解决循环引用的问题，如果两个智能指针互相指向，引用计数永远不会为0，不会自动释放。
## 以对象管理资源
在C++异常处理中，核心问题就是资源的管理，也就是在异常抛出后，在栈展开的过程中，构造的局部对象需要正确释放。异常机制中，通过向一个Range链表中注册相应对象的析构函数来实现相应对象的释放。
在局部对象出其作用域时，C++语言机制保证局部对象的析构函数能够正确调用。在需要成对操作的情况下，可以将Pre操作放在构造函数中，将Post操作放在析构函数中，例如new和delete，open和close。
## 一种智能指针的实现
智能指针的核心就是通过**对象管理资源**的方式来改变引用计数。智能指针接管堆中对象的管理，后面关于对象的使用都是通过该智能指针来操作，用户不用去关心释放问题。智能指针如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20151105094343805)
这种代码方式是不安全或者说是不合规范的，因为用户将内存申请暴露在外面，这就是危险的开始。代码如下所示：
```
T* TP = new T();
{
    // 退出局部作用域时，会释放TP所指向的内存
    smart_ptr<T> SPT(TP);
}
// 退出作用域时，同样会释放TP所指向的内存，会出现二次释放的问题。
smart_ptr<T> SPT(TP);
```
用户应该将为对象申请内存的操作封装起来，或者是起码用临时数据的方式，防止使用该对象指针再次初始化智能指针。但是最好使用封装的方式，因为这样我们也可以控制具体的内存分配方式，也可以自定义删除操作，例如使用对象池等。代码如下：
```
{
    // "new T()"表达式的结果指针作为临时数据，在出函数调用语句后会消失
    smart_ptr<T> SPT(new T());
}
smart_ptr<T> SPT = TFactory.NewIntance();
```
*注：其实最正确的方式是连new都不要出现，而使用make_shared<>代替*
前段时间，有位同学让我帮其解决源码中的一个bug，该代码码是一个写的非常漂亮的智能指针代码。代码如下：
```cpp
#ifndef ___PTR_h___
#define ___PTR_h___
template <class T> class IPtr
{
    // 自定义删除操作函数，接受模板类型的指针作为参数
    typedef void(*DelFunc)(T *obj);
private:
    DelFunc m_pDelFunc;
    // 定义在类内的函数默认是inline的，虽然inline只是向编译器的一种建议
    inline void AddRef() { ++*m_pRefCount; }
    // 释放自己所指向的内存
    inline unsigned long Release() { 
        if (m_pRefCount == NULL) // Already released by assignment
            return -1L;
        // 递减引用计数
        unsigned long result = --*m_pRefCount; 
        // 判断引用是否已经为空
        if (result == 0) {
            // 为空则直接delete
            delete m_pRefCount;
            m_pRefCount = NULL;
            if (m_pDelFunc == NULL)
                delete m_pI;
            else
                // 使用自定义的内存释放函数
                m_pDelFunc(m_pI);
        }
        return result;
    }
public:
    // 设置自定义的删除函数
    void SetDelFunc(DelFunc pDelFunc)
    {
        m_pDelFunc = pDelFunc;
    }
    // 智能指针的默认构造函数
    IPtr()
    {
        m_pDelFunc = NULL;
        m_pRefCount = new unsigned long(0L);
        m_pI = NULL;
    }
    // 使用欲指向的内存类型来构造对象
    IPtr(T* lp)
    {
        // 初始化
        m_pDelFunc = NULL;
        m_pRefCount = new unsigned long(0L);
        m_pI = lp;
        if (m_pI != NULL)
        {
            // 引用计数递增
            AddRef();
        }
    }
    //  拷贝构造函数，递增新指向的引用计数
    IPtr(const IPtr<T>& another) {
        m_pI = another.m_pI;
        m_pRefCount = another.m_pRefCount;
        if (m_pI != NULL)
        {
            AddRef();
        }
    }
    // 析构函数，递减引用计数并判断指向是否为空
    ~IPtr()
    {
        Release();
    }
    // 完善相关操作
    T& operator*()  { assert(m_pI != NULL); return *m_pI; }
    T** operator&() { assert(m_pI != NULL); return &m_pI; }
    T* operator->() { assert(m_pI != NULL); return m_pI; }
    // 重载赋值运算符。递减原有指向的引用计数，递增新指向的引用计数
    IPtr<T>& operator=(const IPtr<T>& another)
    {
        // 很重要，当重载赋值运算符，并且类内数据含有指向新分配内存的时候，特别需要注意自己是否指向自己
        if (this != &another)
        {
            Release(); // 递减原有引用计数
            m_pRefCount = another.m_pRefCount;
            m_pI = another.m_pI;
            if (m_pI != NULL)
            {
                AddRef(); // 递增新指向引用计数
            }
        }
        return *this;
    }
    // 使用新分配的内存赋值给智能指针
    IPtr<T>& operator=(const T* another)
    {
        if (m_pI != another)
        {
            Release();
            m_pRefCount = new unsigned long(0L);
            m_pI = another;
            if (m_pI != NULL)
            {
                AddRef();
            }
        }
        return *this;
    }
    bool operator==(const IPtr<T>& another) {
        return m_pI == another.m_pI;
    }
    // 大多数情况下最好调用上面已经定义过的判断运算符。
    // 但是这里直接比较指针即可，开销也小。
    bool operator!=(const IPtr<T>& another) {
        return m_pI != another.m_pI;
    }
    bool operator<(const IPtr<T>& another) {
        return m_pI < another.m_pI;
    }
    bool operator>(const IPtr<T>& another) {
        return m_pI > another.m_pI;
    }
    bool operator<=(const IPtr<T>& another) {
        return m_pI <= another.m_pI;
    }
    bool operator>=(const IPtr<T>& another) {
        return m_pI >= another.m_pI;
    }
    // Boolean functions
    bool operator!() { return (m_pI == NULL) ? true : false; }
    // Conversion to bool
    operator bool() const
    {
        return (m_pI != NULL) ? true : false;
    }
private:
    T* m_pI; // 真正指向的内存地址
    unsigned long *m_pRefCount; // 引用计数
};
#endif // ___PTR_h___
```
不同于Garbage Collection中的Reference Counting，C++中的引用计数是通过一个栈上 **“help”** 对象来实现对象共享和引用计数的，而GC的Reference Counting是直接在堆上对象内部或者临近位置存储引用计数的。类似于信号量中的count，各个进程共享同一份引用计数，智能指针中的引用计数也必须使用堆来实现共享，一个智能指针的栈上对象被释放掉了，必须能够将那个 **共享** 的引用计数作减一操作。所以智能指针中的引用计数同样是由指针指向的（也就是存放在堆上）。
