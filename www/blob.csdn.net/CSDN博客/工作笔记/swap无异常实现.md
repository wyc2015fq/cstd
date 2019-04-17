# swap无异常实现 - 工作笔记 - CSDN博客





2012年04月20日 15:08:48[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5563标签：[templates																[工作																[算法																[编程																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[基础编程](https://blog.csdn.net/App_12062011/article/category/952065)





swap 是个有趣的函数。原本它只是STL 的一部分，而后成为异常安全性编程的脊柱，以及用来处理自我赋值可能性的一个常见机制。由于swap 如此有用，适当的实现很重要。然而在非凡的重要性之外它也带来了非凡的复杂度。

缺省情况下，swap的算法可以有标准库提供的函数完成。

namespace std 

{

template<typename T>

void swap( T& a , T& b)

{

T temp(a);

a = b;

b = temp;

}

}

然而，这种形式，在有些情况下，不需要做T temp（）这种构造对象，其中最主要的就是"以指针指向一个对象，内含真正数据"那种类型。这种设计的常见表现形式是所谓"pimpl 手法" (pimpI 是"pointer to implementation" 的缩写).

一旦我们需要置换两个对象，只要置换这两个对象的指针就好，即，将指针特化

templeate<>

void swap<T>(...)

然而，由于这样，会访问到类成员，私有部分，因此，我们将申明一个类的共有swap函数，做正真的置换工作，然后特化我们的swap函数，调用真正的类swap做置换工作。如果是类，这样做没有问题。也建议这样做，然而如果是类模版，由于C++只能对类模版进行参数话，对函数模版，我们只能进行重载，因此，在类模版情况下，将std内的swap参数化后，设计为非成员函数，调用我们的成员函数。



总之，如果swap缺省版本对你的类或者类模版，提供可以接受的效率，你不需要额外做任何事，任何尝试置换那种对象的人都会取得缺省版本，效率也不错。

    如果缺省版本实现的效率不足，也就意味者你的类或者类模版，采用了某种pimpl手法，则尝试做以下事：

 1 定义一个成员函数swap，不能有异常，让它高效的做真正的置换。

2 在你的类或者模版所在的命名空间提供一个非成员的swap，并让它调用你的成员swap。

3 如果你正在写一个类，（非类模版），则为你的类特化std：swap，并让它调用你的类成员swap。

 最后，如果你调用swap，请添加一个using申明式。以便让std：：swap在你的函数内曝光，然后不加任何命名空间的修饰，直接调用。





·当std: :swap 对你的类型效率不高时，提供一个swap 成员函数，并确定这个函数不抛出异常。

·如果你提供一个member swap，也该提供→个non-member swap 用来调用前者。对于classes (而非templates) ，也请特化std: :swap

·调用swap 时应针对std~ :swap 使用using 声明式，然后调用swap 并且不带任何"命名空间资格修饰"。

·为"用户定义类型"进行std templates 全特化是好的，但千万不要尝试在std 内加入某些对std 而言全新的东西。](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=templates&t=blog)




