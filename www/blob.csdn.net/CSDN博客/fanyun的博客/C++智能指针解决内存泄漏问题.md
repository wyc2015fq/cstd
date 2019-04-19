# C++智能指针解决内存泄漏问题 - fanyun的博客 - CSDN博客
2017年11月26日 21:56:00[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1748
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
在C++开发中，好多程序员经常会被内存泄漏问题困扰，使用智能指针就可以减少内存泄漏。智能指针可以自动删除分配的内存。智能指针和普通指针类似，只是不需要手动释放指针，而是通过智能指针自己管理内存的释放。
        C++提供了三种智能指针：std::shared_ptr ; std::uniq_ptr ; std::weak_ptr .
**1.shared_ptr共享的智能指针**
Std::shared_ptr使用引用计数，每一个shared_ptr的拷贝都指向相同的内存。在最后一个shared_ptr析构的时候，内存才会被释放。
可以通过构造函数、std_make_shared辅助函数和reset方法来初始化shared_ptr：
```cpp
// 构造函数初始化
Std::shared_ptrp ( new int(1) ) ;
Std::shared_ptrp2 = p ;
// 对于一个未初始化的智能指针，可以通过reset方法来初始化。
Std::shared_ptrptr; ptr.reset ( new int (1) ) ;
If (ptr) {cout << “ptr is not null.\n” ; }
```
不能将一个原始指针直接赋值给一个智能指针：
Std::shared_ptrp ＝ new int(1) ;// 编译报错，不允许直接赋值
获取原始指针：
通过get方法来返回原始指针
```cpp
Std::shared_ptrptr ( new int(1) ) ;
Int * p =ptr.get () ;
```
指针删除器：
智能指针初始化可以指定删除器
```cpp
VoidDeleteIntPtr ( int * p ) {
delete p ;
}
Std::shared_ptrp ( new int , DeleteIntPtr ) ;
```
当p的引用技术为０时，自动调用删除器来释放对象的内存。删除器也可以是一个lambda表达式，例如
Std::shared_ptrp ( new int , [](int * p){delete p} ) ;
注意事项：
(1).不要用一个原始指针初始化多个shared_ptr。
(2).不要再函数实参中创建shared_ptr，在调用函数之前先定义以及初始化它。
(3).不要将this指针作为shared_ptr返回出来。
(4).要避免循环引用。
**2.unique_ptr 独占的智能指针**
Unique_ptr是一个独占的智能指针，他不允许其他的智能指针共享其内部的指针，不允许通过赋值将一个unique_ptr赋值给另外一个unique_ptr。
unique_ptr不允许复制，但可以通过函数返回给其他的unique_ptr，还可以通过std::move来转移到其他的unique_ptr，这样它本身就不再拥有原来指针的所有权了。
如果希望只有一个智能指针管理资源或管理数组就用unique_ptr，如果希望多个智能指针管理同一个资源就用shared_ptr。
**3.weak_ptr弱引用的智能指针**
    弱引用的智能指针weak_ptr是用来监视shared_ptr的，不会使引用计数加一，它不管理shared_ptr内部的指针，主要是为了监视shared_ptr的生命周期，更像是shared_ptr的一个助手。
    weak_ptr没有重载运算符*和->，因为它不共享指针，不能操作资源，主要是为了通过shared_ptr获得资源的监测权，它的构造不会增加引用计数，它的析构不会减少引用计数，纯粹只是作为一个旁观者来监视shared_ptr中关离得资源是否存在。
    weak_ptr还可以用来返回this指针和解决循环引用的问题。
