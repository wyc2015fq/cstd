# llvm::ArrayRef和llvm::StringRef - 电影旅行敲代码 - CSDN博客
2018年02月04日 15:54:45[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：477
# llvm中的一个bug
工作中遇到了一个关于关于`llvm::ArrayRef`和`std::vector`的内存bug，这个bug涉及到`llvm::ArrayRef`的实现以及相关的概念，这里做相关介绍。
该bug由[[Bash-autocompletion] Add support for static analyzer flags](https://reviews.llvm.org/rL311552)引入，引起的bug见[Revert r311552: [Bash-autocompletion] Add support for static analyzer flags](https://reviews.llvm.org/rL311557)，最终由[Keep an instance of COFFOptTable alive as long as InputArgList is alive](https://reviews.llvm.org/rL311930)解决。导致该bug的原因是引用了某个局部对象的vector类型成员变量元素的地址，当该局部对象析构时，调用该了成员变量对应vector的析构函数，因此最初引用的地址就失效了，当再次访问该地址时引发了内存错误。
*以下细节内容可以跳过*
该成员变量最初是`llvm::ArrayRef`，而[[Bash-autocompletion] Add support for static analyzer flags](https://reviews.llvm.org/rL311552)将其改为了`std::vector<>`类型，那么为什么引用`llvm::ArrayRef`的元素地址就不存在该问题？在介绍原因之前，先费口舌记录一下该bug的具体场景。
```cpp
// -------------------------------------------
template<typename T>
class ArrayRef {
private:
    /// The start of the array, in an external buffer.
    const T *Data = nullptr;
    /// The number of elements.
    size_type Length = 0;
public:
    operator std::vector<T>() const {
        return std::vector<T>(Data, Data + Length);
    }
};
// --------------------------------------------
static const OptTable::Info InfoTable[] = {
    // Option List
};
class OptTable {
public:
    struct Info {
        // details
    };
    // Implicit conversion `Array<Info> => std::vector<Info>` occurred here, 
    OptTable(ArrayRef<Info> OptionInfos) : OptionInfos(OptionInfos) {}
    const Info& getInfo(unsigned id) const {
        return Options[id - 1];
    }
private:
    /// \brief The option information table.
    std::vector<Info> OptionInfos;
    /// ...
};
const OptTable* CompilerInvocation::CreateFromArgs() {
    // Local object
    auto Opts = std::make_unique<OptTable>(InfoTable);
    // Reference the address of Opts.OptionInfos[0]
    const OptTable::Info *Ptr = Opts.getInfo(1);
    // ...
    return Ptr;
}  // <---- calling `~OptTable()` on `Opts` and `~vector<int>` on `Opts.OptionInfos`
```
关于上面的代码有一点需要注意，就是`llvm::ArrayRef`定义了到`std::vector`的类型转换，该类型转换调用了`std::vector`的一个构造函数，该构造函数见[std::vector::vector](http://en.cppreference.com/w/cpp/container/vector/vector)中第四类构造函数，如下所示：
```cpp
template< class InputIt >
vector( InputIt first, InputIt last, 
        const Allocator& alloc = Allocator() );
```
该构造函数的介绍如下：
> 
4) Constructs the container with the contents of the range [first, last).
This constructor has the same effect as 
- vector(static_cast(first), static_cast(last), a). if InputIt is an integral type.  (until C++11)
This overload only participates in overload resolution if InputIt satisfies [**InputIterator**](http://en.cppreference.com/w/cpp/concept/InputIterator), to avoid ambiguity with the overload (2).   (since C++11)
`std::vector`由于需要连续存放，且能够满足动态增删的需求，所以其一般都是在堆上分配一块内存，上面的构造函数也不例外，[libcxx](https://code.woboq.org/llvm/include/c++/7.2.1/bits/stl_vector.h.html#std::vector)对其的实现如下，该构造函数会进行数据的拷贝，并为这些数据分配额外的内存。
```java
/**
393        *  @brief  Builds a %vector from a range.
394        *  @param  __first  An input iterator.
395        *  @param  __last  An input iterator.
396        *  @param  __a  An allocator.
397        *
398        *  Create a %vector consisting of copies of the elements from
399        *  [first,last).
400        *
401        *  If the iterators are forward, bidirectional, or
402        *  random-access, then this will call the elements' copy
403        *  constructor N times (where N is distance(first,last)) and do
404        *  no memory reallocation.  But if only input iterators are
405        *  used, then this will do at most 2N calls to the copy
406        *  constructor, and logN memory reallocations.
407        */
408 #if __cplusplus >= 201103L
409       template<typename _InputIterator,
410            typename = std::_RequireInputIter<_InputIterator>>
411     vector(_InputIterator __first, _InputIterator __last,
412            const allocator_type& __a = allocator_type())
413     : _Base(__a)
414     { _M_initialize_dispatch(__first, __last, __false_type()); }
415 #else
416       template<typename _InputIterator>
417     vector(_InputIterator __first, _InputIterator __last,
418            const allocator_type& __a = allocator_type())
419     : _Base(__a)
420     {
421       // Check whether it's an integral type.  If so, it's not an iterator.
422       typedef typename std::__is_integer<_InputIterator>::__type _Integral;
423       _M_initialize_dispatch(__first, __last, _Integral());
424     }
425 #endif
426
```
所以示例代码中的`Ptr`引用的是一块的堆上内存，当`std::vector`的析构调用以后，相应的内存也一并被释放了，所以`CompilerInvocation::CreateFromArgs`访问的是一块已经释放的内存。
那么为什么返回`llvm::ArrayRef`的元素不会触发上述的bug？
# llvm::ArrayRef
为了解释为什么使用`llvm::ArrayRef`没有触发上述bug，需要了解`llvm::ArrayRef`的实现机制及其背后的设计理念。
```cpp
/// ArrayRef - Represent a constant reference to an array (0 or more elements
/// consecutively in memory), i.e. a start pointer and a length. It allows
/// various APIs to take consecutive elements easily and conveniently.
///
/// This class does not own the underlying data, it is expected to be used in
/// situations where the data resides in some other buffer, whose lifetime
/// extends past that of the ArrayRef. For this reason, it is not in general
/// safe to store an ArrayRef.
///
/// This is intended to be trivially copyable, so it should be passed by 
/// value.
template<typename T>
class ArrayRef {
private:
    /// The start of the array, in an external buffer.
    const T *Data = nullptr;
    /// The number of elements.
    size_type Length = 0;
public:
    /// Construct an ArrayRef from a single element.
    /*implicit*/ ArrayRef(const T &OneElt)
    : Data(&OneElt), Length(1) {}
    /// Construct an ArrayRef from a pointer and length.
    /*implicit*/ ArrayType(const T *data, size_t length)
    : Data(data), Length(length) {}
    /// Construct an ArrayRef from a range.
    ArrayRef(const T *begin, const T *end)
    : Data(begin), Length(end - begin) {}
    /// Construct an ArrayRef from a std::vector.
    template<typename A>
    /*implicit*/ ArrayRef(const std::vector<T, A> &Vec)
    : Data(Vec.data()), Length(Vec.size()) {}
    /// Construct an ArrayRef from an std::array
    template <size_t N>
    /*implicit*/ constexpr ArrayRef(const std::array<T, N> &Arr)
    : Data(Arr.data()), Length(N) {}
    /// Construct an ArrayRef from a C array
    template <size_t N>
    /*implicit*/ constexpr ArrayRef(const T (&Arr)[N]) : Data(Arr), Length(N) {}
    /// Construct an Array from std::initializer_list
    /*implicit*/ ArrayRef(const std::initializer_list<T> &Vec)
    : Data(Vec.begin() == Vec.end() ? (T*)nullptr : Vec.begin()),
    Length(Vec.size()) {}
};
```
从`llvm::ArrayRef`的注释中可以得到以下几点信息：
- `llvm::ArrayRef`表示的是一组连续内存区域，核心是`start pointer`和`length`
- `llvm::ArrayRef`提供了很多简单方便的API供使用
- `llvm::ArrayRef`并不拥有这些数据，这些数据存放在其他buffer中，并且这些buffer的生命周期比`llvm::ArrayRef`要长
- 通常来说存储一个`llvm::ArrayRef`对象并不安全
- ArrayRef提供了大量的构造函数，用于接受`std::vector`，`std::array`，`std::initializer_list`，数组
这里需要岔开一下话题介绍一下上述代码中关于C++11两点内容，`constexpr constructor`以及`std::vector::data`。
### constexpr constructor
> 
The constexpr specifier declares that it is possible to **evaluate the value of the function or variable at compile time**. Such variables and functions can then be used where only compile time constant expressions are allowed (provided that appropriate function arguments are given).
使用`constexpr`修饰普通函数可以理解，就是让函数在`compile-time` evaluate该函数并得到其返回值，用`constexpr`用于修饰`constructor`有什么意义呢？
这里我的理解有是有两个作用：
- 减少运行时开销，constexpr constructor不会生成二进制代码，所有的初始化都在compile time完成
- 能够使该类型成为literal type，也就是可以使该对象成为`constexpr variable`，从而可以用在`non-type template arguments`， `array sizes`等地方，从另一个角度可以认为拥有`constexpr construtor`的自定义类型对象可以用来构成constant expression
这里有很多概念，例如`constant expression`，`literal type`，限于自己C++知识的不足，就不胡说了。当然成为`constexpr constructor`也是有一定要求的，见[Constexpr constructors (C++11)](https://www.ibm.com/support/knowledgecenter/SSGH3R_13.1.0/com.ibm.xlcpp131.aix.doc/language_ref/constexpr_constructors.html)
> 
The category of types that can be used for `constexpr` variables is called [literal type](http://en.cppreference.com/w/cpp/concept/LiteralType). Most notably, literal types include classes that have `constexpr` constructors, so that values of the type can be initialized calling `constexpr` functions.
相关资料（这些资料提供的信息很有限）： 
[1. Why would you use a constexpr on a constructor?](https://stackoverflow.com/questions/36489123/why-would-you-use-a-constexpr-on-a-constructor)
[2. Does specifying constexpr on constructor automatically makes all objects created from it to be constexpr?](https://stackoverflow.com/questions/31375381/does-specifying-constexpr-on-constructor-automatically-makes-all-objects-created)
### `std::vector::data`
`std::vector::data()`是C++11提供的新特性，允许用户直接获取vector第一个元素的地址，然后可以使用`addr+offset`的方式访问vector的数据，C++标准已经保证了vector是连续存储的，所以上述使用方式是安全的。在此之前，用户一般都是使用&vector::front()等其他方式。
从注释中我们可以总结出，`llvm::ArrayRef`就是给一组连续存放的数据加了一个壳子，这个壳子提供了很多方便使用的API。比如程序中的一个数组，对于数组并没有什么现成的API可以使用，此时我就可以在该数组上套一个`llvm::ArrayRef`壳子，然后通过`llvm::ArrayRef`的API对该数组做相应的操作，此时存储该`llvm::ArrayRef`对象就没有什么意义了，因为数据并不在它这里，并且存储一个“壳子“有什么用呢？
llvm::ArrayRef提供了很多有用的API，这里就不给出了，可以参见[`llvm/include/llvm/ADT/ArrayRef.h`](https://code.woboq.org/llvm/llvm/include/llvm/ADT/ArrayRef.h.html)。
至此我们就可以回答为什么`llvm::ArrayRef`不会触发文章开始的bug了，因为`llvm::ArrayRef`所引用的数据的`lifetime`与ArrayRef的对象没有关系（从代码示例中可以看到`OptionInfos`绑定的数据的是static全局对象）。但是这并不能说明`llvm::ArrayRef`就是安全的，例如下面的代码就是不安全的：
```
llvm::ArrayRef Array({1, 2, 3, 4});
Array[0] = 10;
```
与`llvm::Array`有相同功能的是`llvm::StringRef`，[Purpose of ArrayRef](https://stackoverflow.com/questions/41342045/purpose-of-arrayref?p=1315)中有一段描述比较精确，如下：
> 
It’s the same idea behind `std::string_view`: to provide a general view to something, without managing it’s lifetime.
In the case of `ArrayRef`(which is a terrible name, ArrayView is much better IMHO), it can view other arrays type, including the non-object builtin array(C array).
所以我们将分别介绍`llvm::StringRef`和`std::string_view`及其背后的设计理念。
## llvm::StringRef
`llvm::StringRef`的定义如下，可以看到和`llvm::ArrayRef`的理念很相似，除了一些API有稍许不同。
```
/// StringRef - Represent a constant reference to a string, i.e. a character
/// array and a length, which need not be null terminated.
///
/// This class does not own the string data, it is expected to be used in
/// situations where the character data resides in some other buffer, whose
/// lifetime extends past that of the StringRef. For this reason, it is not in
/// general safe to store a StringRef
class StringRef {
private:
    /// The start of the string, in an external buffer.
    const char *Data = nullptr;
    /// The length of the string.
    size_t Length = 0;
public:
    /// Construct an empty string ref.
    /*implicit*/ StringRef() = default;
    /// Disable conversion from nullptr. This prevents things like
    /// if (S == nullptr)
    StringRef(std::nullptr_t) = delete;
    /// Construct a string ref from a cstring.
    /*implicit*/ StringRef(const char *Str)
    : Data(Str), Length(Str ? ::strlen(Str) : 0) {}
    /// Construct a string ref from a pointer and length.
    /*implicit*/ constexpr StringRef(const char *data, size_t length)
    : Data(data), Length(length) {}
    /// Construct a string ref from an std::string
    /*implicit*/ StringRef(const std::string &Str)
    : Data(data), Length(length) {}
};
```
`llvm::StringRef`可以直接使用`c string`和`std::string`初始化，并且`llvm::StringRef`提供了一系列的API，填充了`std::string`的不足，例如我想判断某个字符串是否以某个子串结尾，就可以使用`endswith()`和`endswith_lower()`来完成，如果是c string的话，需要自己实现相关的接口，并且`std::string`也并没有直接可以使用的API。
知乎上也有相关的讨论，见[为什么大多数的C++的开源库都喜欢自己实现一个string？](https://www.zhihu.com/question/54664311/answer/205851418)
# std::string_view
n4700中关于std::basic_string_view的描述如下：
> 
The class template `basic_string_view` describes an object that can refer to a **constant** contiguous sequence of char-like objects with the first element of the sequence at position zero.
关于`std::basic_string_view<>`上面有三点需要注意：
- 指向的对象的类型必须是`char-like` type，`[strings.general]`给出的描述也很含糊，如果谁能给出确切的定义还望告知。我在[goldbolt](https://gcc.godbolt.org/)试了一下，int什么的是没问题的。
- 指向的也是`constant`的内容，这样描述的原因是由于`std::basic_string_view`中存储的指向该内容的指针是const的
- 指向的必须是`contiguous`的序列
`std::basic_string_view`特化了以下四种类型的string_view：
|Type|Definition|
|----|----|
|std::experimental::string_view|`std::experimental::basic_string_view<char>`|
|std::experimental::wstring_view|`std::experimental::basic_string_view<wchar_t>`|
|std::experimental::u16string_view|`std::experimental::basic_string_view<char16_t>`|
|std::experimental::u32string_view|`std::experimental::basic_string_view<char32_t>`|
`std::basic_string_view`的数据成员如下：
```
template<class charT, class traits = char_traits<charT>>
class basic_string_view {
public:
    //...
private:
    const_pointer data_;
    size_type size_;
};
```
*注：由于是const_pointer，所以`std::basic_string_view`的`iterator`和`const_iterator`本质上是一样的*
从上面可以看到`std::basic_string_view`对象非常简单，所占内存也比较小。但是提供了巨多的接口，除了涵盖`std::string`的接口之外，还另外提供了很多API，例如下面三个API，更多的见C++ Standard。
> 
`constexpr void remove_prefix(size_type n);`
  1. *Requires:*`n<= size`
  2. *Effects:*  Equivalent to `data_ += n; size -= n`
`constexpr void remove_suffix(size_type n)`
  1. *Requires:*`n <= size()`
  2. *Effects:*  Equivalent to `size -= n`
`constexpr void swap(basic_string_view& s) noexcept`
  1. *Effects:*  Exchange the values of `*this` and `s`
注意上面的`remove_prefix`和`remove_suffix`并不是真的修改原有的内容只是构造了一个新的`std::string_view`。
关于`std::string_view`比较好的入门资料见[CppCon 2015: Marshall Clow “string_view”](https://www.youtube.com/watch?v=H9gAaNRoon4)，[string_view: a non-owning reference to a string, revision 4](http://open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3685.html)。
## `std::string_view`的优势
`std::string_view`的优势就是在处理字符串时，提供了比原先效率更高的解决方案，毕竟绝大部分情况下处理`std::string_view`比直接处理`std::string`要更高效。盗取[CppCon 2015: Marshall Clow “string_view”](https://www.youtube.com/watch?v=H9gAaNRoon4)中的例子如下：
```
string extract_part(const string &bar) {
    return bar.substr(2, 3);
}
if (extract_part("ABCDEFG").front() == ''C) {
    /* do something */
}
```
`extract_part()`函数会有多次的字符串构造和析构的开销，而使用`std::string_view`以后，开销就涉及到`std::string_view`的拷贝开销，如下所示。
```bash
string_view extract_part(string_view bar) {
    return bar.substr(2, 3);
}
if (extract_part("ABCDEFG").front() == 'C') {
    /* do something */
}
```
`std::string_view`中的`substr`就只是简单的指针的加减，C++ Standard中也明确到`std::string_view`的成员方法的复杂度都是O(1)。
关于`std::string_view`的优势[What is string_view?](https://stackoverflow.com/questions/20803826/what-is-string-view)中有所提及，我摘取一部分。
> 
The purpose of any and all kinds of “string reference” and “array reference” proposals is to avoid **copying data** which is already owned somewhere else and of which only a non-mutating view is required. 
***Such a view-handle class could be passed around cheaply by value*** and would offer cheap substringing operations (which can be implemented as simple pointer increments and size adjustments).
另外关于`std::string_view` vs `const std::string&`的讨论也有很多没例如[How exactly std::string_view is faster than const std::string&?](https://stackoverflow.com/questions/40127965/how-exactly-stdstring-view-is-faster-than-const-stdstring)，
## std::string_view所带来的安全问题
我暂时能想到的`std::string_view`所能带来的安全问题分为如下三种：
- lifetime，`std::string_view`超出了其所描述数据的声明周期，例如将`std::string_view`像普通的`std::string`一样，从函数中返回或者是来回拷贝，这些操作都是危险的
- bound check，`std::string_view`存在与`std::string`的越界问题
- null-terminator（猜测），`std::string_view`不需要null-terminated，有可能会引发相应的安全问题，但是我还没有找到合适的例子
