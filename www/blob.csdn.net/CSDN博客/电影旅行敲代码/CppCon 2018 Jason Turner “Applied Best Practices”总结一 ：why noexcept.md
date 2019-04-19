# CppCon 2018: Jason Turner “Applied Best Practices”总结一 ：why noexcept? - 电影旅行敲代码 - CSDN博客
2018年11月25日 20:05:26[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：100
# 什么是noexcept
在CppCon 2018上，Jason Turner在talk《[Applied Best Practices](https://www.youtube.com/watch?v=DHOlsEd0eDE&t=3306s)》中提出总结了几点best practices，我会通过几篇文章做一些笔记，本文章对c++11中*noexcept*进行一些总结。
## noexcept specifier
> 
**noexcept specifier:** Specifies whether a function could throw exceptions [cppreference.com](https://en.cppreference.com/w/cpp/language/noexcept_spec).
*noexcept specifier*用于指明一个函数是否有可能抛出异常。*noexcept specifier*如下几种形式：
- **noexcept**
- **noexcept**(*expression*)
- **throw**()
其中*noexcept*等同于*noexcept(true)*，用于指明函数不会抛出异常。而*noexcept(false)*意味着函数可能会抛出异常。第三种形式不需要浪费精力去深究，因为它将在c++20中移除。
C++中的每个函数要么是*non-throwing*，要么是*potentially throwing*，判断一个函数是否抛出异常的详细的标准见[noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec).
## noexcept operator
*noexcept operator*是一个用于判断（编译期）指定的表达式是否会抛出异常运算符，它可以和*noexcept specifier*一起使用通过某些已有信息来指定某个函数是否会抛出异常。
另外*noexcept operator*的操作数是[unevaluated operands](http://eel.is/c++draft/expr.prop#def:unevaluated_operand)，类似于的还有
- sizeof operator
- typeid operator
- require expression
- decltype specifier
*noexcept operator*需要编译器的支持，编译器需要按照一定的标准来判断指定的表达式是否会抛出异常，标准如下：
> 
The result is **false** if the *expression* contains at least one of the following potentially evaluated constructs:
- call to any type of function that does not have non-throwing exception specification, unless it is a constant expression
- throw expression
- dynamic_cast expression when the target type is a reference type, and conversion needs a run time check
- typeid expression when argument type is polymorphic class type
In all other cases the result if true. until c++17
注意这些推断都是简单的编译期的分析，没有使用*flow-analysis*的分析，如下示例代码，最终的结果也是**false**。
```
int foo() { return 1; }
int goo() noexcept{ return 0; }
int main() {
  std::cout << noexcept(0 > 1 ? foo() : goo()) << '\n';
}
```
## noexcept缘起
noexcept缘起于C++0x中的`move constructor`，在由[David Abrahams](https://twitter.com/DaveAbrahams)和[Douglas Gregor](https://twitter.com/dgregor79)写的文章《[Rvalue References and Exception Safety](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html)》中介绍了*move constructor*在exception发生时只支持**basic guarantee**，
> 
To resolve this dilemma, the C++ standard library provides a set of exception-safety guarantees that share the burden of producing correct programs between implementers of the standard library and users of the standard library:
[3a] ‘‘**Basic guarantee for all operations**:’’ The basic invariants of the standard library are maintained, and no resources, such as memory, are leaked.
[3b] ‘‘**Strong guarantee for key operations**:’’ In addition to providing the basic guarantee, either the operation succeeds, or has no effects. This guarantee is provided for key library operations, such as push_back(), single-element insert() on a list, and uninitialized_copy()
(§E.3.1, §E.4.1).
[3c] ‘‘**Nothrow guarantee for some operations**:’’ In addition to providing the basic guarantee, some operations are guaranteed not to throw an exception This guarantee is provided for a few simple operations, such as swap() and pop_back() (§E.4.1). [Standard-Library Exception Safety](http://www.stroustrup.com/3rd_safe.pdf)
*注：Douglas Gregor是boost的早期重要成员，以及clang和swift的主要作者*
《[Rvalue References and Exception Safety](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html)》以vector中的*push_back*为例，描述了*move constructor*在*push_back*出现异常时，可能导致的问题。
```
T* reallocate(T *old_ptr, size_t old_capacity) {
  // #1: allocate new storage
  T* new_ptr = (T*)new char[sizeof(T) * old_capacity * 2];
  
  // #2: try to move the elements to the new storage
  unsigned i  = 0;
  try {
    // #2a: construct each element in the new storage from the correspoding
    // element in the old storage, treating the old elements as rvalues.
    for (; i < old_capacity; ++i)
      new (new_ptr + i) T(std::move(old_ptr[i])); // "move" operation
  } catch(...) {
    // #2b: destory the copies and deallocate the new storage
    for (unsigned v = 0; v < i; ++v)
      new_ptr[v]->~T();
    delete[]((char*)new_ptr);
    throw;
  }
  // #3: free the old storage
  for (i = 0; i < old_capacity; ++i)
    old_capacity[i]->~T();
  delete[] ((char*)old_ptr);
  return new_ptr;
}
```
*注：上述代码来自于《[Rvalue References and Exception Safety](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html)》*
在*push_back*的时候存在如下两种情况，
- *size < capacity*，不需要重新分配
- *size == capacity*，需要重新分配一块更大的内存，然后将原有的元素一一拷贝或移动到新的内存空间中
上述代码示例描述的就是重新分配更大块内存的情况，此时如果元素有*move constructor*，那么就会调用*move construtor*，但如果在调用某个*move constructor*时，出现了异常，那么此时原有内存空间中已经移动过的对象已经处于[moved from state](https://wiki.sei.cmu.edu/confluence/display/cplusplus/EXP63-CPP.+Do+not+rely+on+the+value+of+a+moved-from+object)，**而且这几乎是不可逆的**，因为当你尝试将对应元素从新内存空间移动回原有内存空间时，*move constructor*有可能还会出现异常。所以此时，当元素的*move constructor*可能会出现异常时，*push_back*只能作出***basic guarantee***，例如vector的*capacity*，*size*等保持不变，也没有内存泄漏，但此时某些元素对象的状态已经发生改变。整个过程如下图所示：![move constructor](https://img-blog.csdnimg.cn/20181125125538193.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)*注：关于**moved-from state**，请参见[EXP63-CPP. Do not rely on the value of a moved-from object](https://wiki.sei.cmu.edu/confluence/display/cplusplus/EXP63-CPP.+Do+not+rely+on+the+value+of+a+moved-from+object)*
所以《[Rvalue References and Exception Safety](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html)》提出了如下用于解决该问题的方式：
- 使用*concept*，例如require NothrowMoveConstructible<T>，如果不满足则退化到*copy constructor*
- 提出了*noexcept*限定符，编译器会静态检查是否满足noexcept属性，如果被noexcept限定的函数会抛出异常，则这个程序是ill-formed的
- *move constructors*和*destructors*默认*noexcept*。如果用户需要*move constructors*和*destructors*抛出异常，则需要显示地使用throw表示，STL不会采用这些会抛出异常的move constructor和destructor。
《[Rvalue References and Exception Safety](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html)》的主要贡献如下：
- 发现了move constructor破坏了STL中strong exception guarantee这个问题
- noexcept限定符来说明某个函数不会抛出异常，并静态检查该函数是否违反了noexcept这一性质
## noexcept的改进
《[Rvalue References and Exception Safety](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2009/n2855.html)》存在如下几个问题：
- 对于每个noexcept限定符，编译器都要进行静态检查，开销是一个问题
- 静态检查（使用比较简单的程序分析）过于严格，可能会有“误报”，也就是该函数不可能抛异常，但是编译器在分析的时候却认定它可能会抛异常
- move constructor默认都是noexcept，**只是为了在类似于STL场景中保证strong guarantee**，但是在某些用户的场景中，*basic guarantee*也是可以接受的（毕竟*move constructor*带来的性能提升太吸引人了），所以最好可以在使用的地方进行控制，是否选择采用可能会抛出异常的*move constructor*
基于此《[Allowing Move Constructors to Throw](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2983.html)》提出如下改进：
- 提供了std::move_if_noexcept
- 提供了operator noexcept，允许用户按需检查（编译期静态检查）某个函数是否会抛出异常。从而避免对所有使用noexcept限定符的函数进行静态检查。
另外在《[Allowing Move Constructors to Throw (Rev. 1)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2010/n3050.html)》中提出了重要的改进就是**如果使用noexcept限定的函数抛出了异常，则调用std::terminate（一般由std::abort实现），并且保证这个异常不会跳出该函数**。
到此C++11中的noexcept已经初见雏形，noexcept的提出和改进过程可以总结为下面的描述：
> 
If the noexcept feature appears to you incomplete, prepared in a rush, or in need of improvement, note that all C++ Committee members agree with you. **The situation they faced was that a safety problem with throwing move operations was discovered in the last minute and it required a fast solution.** The current solution does solve the problem neatly: there will be no silently generated throwing moves that will make STL containers silently break contracts. 《[Using noexcept](https://akrzemi1.wordpress.com/2011/06/10/using-noexcept/)》
*注：noexcept的由来和改进可以参照[Using noexcept](https://akrzemi1.wordpress.com/2011/06/10/using-noexcept/)*
# The overhead of exception handling
**留坑**
## [Zero-overhead deterministic exceptions: Throwing values](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0709r0.pdf)
# When should I really use noexcept?
**留坑**
就像是否有必要在所有有返回值的函数前面加上*[[nondiscard]]*一样，nonexcept同样存在这个问题，那么何时需要为函数加上noexcept限定符号呢？关于这个问题众说纷纭，还没有确切的答案，例如在什么情况下做什么。相关资料如下：
- [https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-noexcept](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-noexcept)
- [https://stackoverflow.com/questions/10787766/when-should-i-really-use-noexcept](https://stackoverflow.com/questions/10787766/when-should-i-really-use-noexcept)
