# __builtin_addressof - 电影旅行敲代码 - CSDN博客
2018年04月15日 17:30:10[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：255
# [__builtin_addressof](http://clang.llvm.org/docs/LanguageExtensions.html#builtin-addressof)
`__builtin_addressof`是由[Richard Smith](https://github.com/zygoloid)提交到clang中的，提交的patch为[Add a __builtin_addressof that performs the same functionality as the built-in](https://reviews.llvm.org/rL186053)，commit message如下：
```
Add a __builtin_addressof that performs the same functionality as the built-in
& operator (ignoring any overloaded operator& for the type). The purpose of
this builtin is for use in std::addressof, to allow it to be made constexpr;
the existing implementation technique (reinterpret_cast to some reference type,
take address, reinterpert_cast back) does not permit this because
reinterpret_cast between reference types is not permitted in a constant
expression in C++11 onwards.
```
从commit message中可以看出来，`__builtin_addressof()`主要是作为`std::addressof`的”加强版本”，可用于`constexpr`的场景。
## `std::addressof`
[`std::addressof`](http://en.cppreference.com/w/cpp/memory/addressof)是c++11提供的模板函数，用于获取左值表达式的地址。`std::addressof`的声明如下：
> - template< class T >  
  T* addressof(T& arg) noexcept; (since C++11)  (until C++17)
- template< class T >  
  constexpr T* addressof(T& arg) noexcept; (since C++17)
- template < class T>  
  const T* addressof(const T&&) = delete; (2)   (since C++17)
1) Obtains the actual address of the object or function arg, even **in presence of overloaded operator&**
  2) Rvalue overload is deleted to prevent taking the address of const rvalues. 
  The expression std::addressof(E) is a constant subexpression, if E is an lvalue constant subexpression.
其中`std::addressof`的存在是为了应对重载`operator&`的情况，能够保证获取到正确的地址。而且自从C++17开始，标准规定当`std::addressof`的参数`lvalue constant expression`的时候，`std::addressof`的结果也必须是constant subexpression。`std::addressof`的可能实现如下：
```cpp
template< class T >
T* addressof(T& arg) 
{
    return reinterpret_cast<T*>(
               &const_cast<char&>(
                  reinterpret_cast<const volatile char&>(arg)));
}
```
注意[`reinterpret_cast<>`](http://en.cppreference.com/w/cpp/language/reinterpret_cast)不允许用于constant expression，且返回值不是constant expression，所以`std::addressof`的典型实现不能用于 constant expression。
> 
Only the following conversions can be done with reinterpret_cast, **except when such conversions would cast away constness or volatility**.
对于此有两个问题：
- `reinterpret_cast`为什么不能用于`constant expression`？
- `std::addressof`的典型实现是怎么操作的？
对于第一个问题，我不是C++ lawyer，暂时无法回答。一些相关资料见：
- [A Proposal to Relax Constexpr Restrictions for some Reinterpret Casts](http://apolukhin.github.io/constexpr_algorithms/reinterpret.html)
- [Bug 49171 - [C++0x][constexpr] Constant expressions support reinterpret_cast](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=49171)
对于第二个问题，`std::addressof`的典型实现分为三步，分别是：
- **reinterpret_cast< const volatile char&>**(arg)
- **&const_cast< char&>**( reinterpret_cast< const volatile char&>(arg)))
- **reinterpret_cast< T*>**( &const_cast< char&>(reinterpret_cast< const volatile char&>(arg)))
第一步就是将`T&`转换成为 `const volatile char&`。主要原因是`std::addressof`为了规避对`operator&`的重载，必须选择一个内置类型进行转换（内置类型没有重载`operator&`），然后才能安全使用的`operator&`来获取其地址。这里选择`char`的原因是，`char`有`weakest alignment`，`reinterpret_cast<>`对`alignment`有要求，To-Type的`alignment`不能比`From-Type`更stricter（larger），见[9.2 reinterpret_cast](https://docs.oracle.com/cd/E19205-01/819-5267/bkahk/index.html)。
> 
The weakest alignment (the smallest alignment requirement) is the alignment of char, signed char, and unsigned char, which equals 1; the largest fundamental alignment of any type is the alignment of std::max_align_t. [[Alignment 
  ]](http://en.cppreference.com/w/cpp/language/object)
另外由于`reinterpret_cast<>`不能去掉`constness`，所以使用`const volatile char&`来保证转换成功。
第二步就是使用`const_cast<>`合法地去掉`constness`。然后此时就可以安全地使用`operator&`获取地址了。
第三步就是将该地址再转换为原来的类型。
StackOverflow上有一篇相关的问题，见[Implementation of addressof](https://stackoverflow.com/questions/16195032/implementation-of-addressof#)。
介绍完`addressof`相关的内容，下面言归正传，由于从c++17开始`std::addressof`支持`constant expression`，所以现在`libcxx`中的`std::addressof`在c++17开始直接由`__builtin_addressof`实现，相关patch见[Make std::addressof constexpr in C++17 (Clang only)](https://reviews.llvm.org/rL263688).
# void&
这篇博客的初衷就是工作当中遇到了在clang编译`__builtin_addressof`的时候出现的语义分析时的错误，错误与void&相关。`__builtin_addressof`的函数原型如下，参数类型就是`void&`。但是在编码过程中是不可能出现`void&`的，见[Why is it impossible to have a reference-to-void?](https://stackoverflow.com/questions/457896/why-is-it-impossible-to-have-a-reference-to-void)
。而`__builtin_addressof`能够编译通过的原因是，它有自己的语义分析逻辑，针对`__builtin_addressof`的`hasCustomTypechecking`为true。
`BUILTIN(__builtin_addressof, "v*v&", "nct")`
更多内容参见[More C++ Idioms/Address Of](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Address_Of)
