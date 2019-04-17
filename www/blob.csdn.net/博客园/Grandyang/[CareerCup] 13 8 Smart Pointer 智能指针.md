# [CareerCup] 13.8 Smart Pointer 智能指针 - Grandyang - 博客园







# [[CareerCup] 13.8 Smart Pointer 智能指针](https://www.cnblogs.com/grandyang/p/4929907.html)







13.8 Write a smart pointer class. A smart pointer is a data type, usually implemented with templates, that simulates a pointer while also providing automatic garbage collection. It automatically counts the number of references to a SmartPointer<T*> object and frees the object of type T when the reference count hits zero.



这道题让我们实现[智能指针](https://zh.wikipedia.org/wiki/%E6%99%BA%E8%83%BD%E6%8C%87%E9%92%88)，所谓智能指针，就是除了具有普通指针的功能外，还能通过自动内存管理来提供安全性。它能避免一系列问题，如[迷路指针](https://zh.wikipedia.org/wiki/%E8%BF%B7%E9%80%94%E6%8C%87%E9%92%88)，内存泄露，分配失败等错误。智能指针必须维护一个引用计数变量来统计给定对象的所有引用。实现过程主要有如下几个部分：

- 构造函数(通过引用对象构造)

- 拷贝构造函数(通过另一个智能指针对象构造)

- 等号重载函数

- 析构函数

- 移除引用函数

对于通过引用对象来构造的构造函数，我们需要将ref指针指向对象，然后申请一个空间给计数器，并赋值为1.

对于拷贝构造函数，我们除了要把参数中智能指针对象中的对象指针和计数器赋值过来，计数器还要自增1，因为又多了一个对象的引用。

对于等号重载函数，我们首先判断等号左右两边的智能指针是否为同一个指针，是的话直接返回this指针即可。由于我们要覆盖等号左边的智能指针，所以当其本身指着一个对象时，我们要调用移除引用函数，然后再赋上新的引用函数，赋值过程和上面的拷贝构造函数类似。

对于析构函数，直接调用移除引用函数即可。

对于移除引用函数，我们首先将计数器自减1，如果此时计数器为0了，我们要释放对象指针和计数的内存，并将指针设为空指针。

参见代码如下：



```
template <class T> 
class SmartPointer {
public:
    SmartPointer(T *ptr) {
        ref = ptr;
        ref_count = (unsigned*)malloc(sizeof(unsigned));
        *ref_count = 1;
    }
    SmartPointer(SmartPointer<T> &sptr) {
        ref = sptr.ref;
        ref_count = sptr.ref_count;
        ++(*ref_count);
    }
    SmartPointer<T>& operator = (SmartPointer<T> &sptr) {
        if (this == &sptr) return *this;
        if (*ref_count > 0) remove();
        ref = sptr.ref;
        ref_count = sptr.ref_count;
        ++(*ref_count);
        return *this;
    }
    ~SmartPointer() {
        remove();
    }
    T getValue() {
        return *ref;
    }
    
protected:
    T *ref;
    unsigned *ref_count;
    void remove() {
        --(*ref_count);
        if (*ref_count == 0) {
            delete ref;
            free (ref_count);
            ref = nullptr;
            ref_count = nullptr;
        }
    }
};
```














