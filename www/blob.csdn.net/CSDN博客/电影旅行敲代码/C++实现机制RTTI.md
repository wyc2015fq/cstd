# C++实现机制RTTI - 电影旅行敲代码 - CSDN博客
2015年11月03日 21:05:56[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：656
运行时类型识别(run-time type identification, RTTI)的功能由两个运算符实现：
- typeid运算符，用于返回表达式的类型。
- dynamic_cast<>运算符，用于将基类的指针或引用安全的转换成派生类的指针或引用。
C++中没有JAVA中的反射机制，在运行时获得任何对象的类型信息，通常类型信息还非常丰富。C++只针对内置类型和多态的自定义类型实现了RTTI。C++引入了运行时类型信息，实际上将对象分为了两部分。
- 一种包含了类型信息，可以在运行时获取类型信息
- 
一种没有包含类型信息
> 
We cannot impose the burden of being able to identify an object at run-time on built-in types such as int and double without unacceptable costs in run-time. —[[C++之父Stroustrup]](https://docs.google.com/viewer?url=http%3A%2F%2Fwww.open-std.org%2Fjtc1%2Fsc22%2Fwg21%2Fdocs%2Fpapers%2F1992%2FWG21%25201992%2FX3J16_92-0068%2520WG21_N0145.pdf)
按照C++之父的说法他没有为所有对象引入运行时机制的原因是，有些对象根本不需要，并且负担不起运行时获取类型信息的开销。为了不使语言设计复杂化，并且在对多态类型引入RTTI机制已经足够的情况下，只为多态类型引入了RTTI。其中通常放在virtual table中的第一个槽(slot)内，GCC会将type_info存放在虚表的第一个槽，而Visual Studio会将type_info存放在虚表的最后，其实实现方式大同小异。如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20151103210337293)
如果通过对象来获取类型信息，则可能的实现方式就是：
`((type_info*)(ptr->vptr[0]))->_type_descriptor;`
获取到type_info的object后，再调用runtime library函数与class type进行比对，这种方式显然会带来很大的运行时开销。
另外需要注意的是，当dynamic_cast<>用于引用时需要注意：
```cpp
const Derived& d = dynamic_cast<const Derived&>(b);
```
因为不存在所谓的空引用，如果转换不成功的话程序会抛出名为std::bad_cast的异常。
另一种常用的RTTI使用方式就是typeid()，typeid()会返回对象的类型信息，虽然这种方式很方便，但是我们也不能滥用这种方式来代替虚函数，这种方式扩展性不是很好，总之尽量不要使用typeid来代替虚函数的功能。
当然这只是一个示意图，真正编译器实现的时候要比这个复杂的多，有兴趣的可以看一看MSVC在RTTI上的实现方式[《Reversing Microsoft Visual C++ Part II: Classes, Methods and RTTI》](http://www.openrce.org/articles/full_view/23)
