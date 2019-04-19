# Name Mangling - 电影旅行敲代码 - CSDN博客
2016年02月29日 16:59:40[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1461标签：[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[c++基础](https://blog.csdn.net/dashuniuniu/article/category/5682427)
# 引子
为了在链接的时候能够让链接器唯一标识全局变量或者函数等的符号，编译器实现了一种**name mangling**的技术（也叫做name decoration），这个技术能够给所有目标文件中的符号一个唯一的名字。
具体的实现方式就是编译器向**函数**，**结构体**，**类**或者**其他的数据类型**的对象名中附加上相应的类型信息，然后这个信息供链接器使用来实现链接过程中的符号决议。由于很多语言都允许对象同名，这些同名的对象或者处于不同的名称空间下（namespace），或者有不同的函数签名（signatures）。
链接器在链接的时候不仅需要函数名，还需要函数的参数个数以及返回值等类型等信息，这些信息都可以从**mangled name**中获取。
## C中的name mangling
虽然**name mangling**技术不太用于不支持函数重载的语言中，例如C和经典Pascal，但是在某些场景下，除了名字之外，还需要其他的信息。例如多数编译器都支持多种不同的[调用惯例](https://en.wikipedia.org/wiki/Calling_convention)（calling conventions），调用惯例决定了函数参数的入栈方式以及由哪一方负责来清理栈帧。这就需要将函数的调用惯例信息有编译器附加到目标文件的符号名中。例如：
```
int _cdecl    f (int x) { return 0; }
int _stdcall  g (int y) { return 0; }
int _fastcall h (int z) { return 0; }
```
然后对应的符号名有可能是：
```
_f
_g@4
@h@4
```
## C++中的name mangling
C++应该是使用name mangling最为广泛的语言了，但是C++语言却没有为**name mangling**定义标准，类似于C++ ABI，C++标准应该推出一个标准出来，来去除不同编译器编译到的目标文件之间的壁垒。C++非常复杂的语言特性，例如**class**，**templates**，**namespaces**以及**符号重载**。这些特性都可以使用**name mangling**技术编码到符号名中。并且不同的编译器有不同的**name-mangling**体系，所以很少有链接器能够链接不同的编译器编译得到的目标文件。
```
namespace wikipedia 
{
   class article 
   {
   public:
      std::string format (void); 
         /* = _ZN9wikipedia7article6formatEv */
      bool print_to (std::ostream&); 
         /* = _ZN9wikipedia7article8print_toERSo */
      class wikilink 
      {
      public:
         wikilink (std::string const& name);
            /* = _ZN9wikipedia7article8wikilinkC1ERKSs */
      };
   };
}
```
如上代码所示就是一个比较复杂的**name mangling**的例子，所有mangled symbols都以 **_Z** 开头，对于nestd names，包括名称空间和类都跟在字母**N**后面，然后就是一组**《length, id》**，其中length表示标识符的长度，最后以E结尾。例如**wikipedia::article::format**对应的**mangled name**如下所示：
`_ZN9wikipedia7article6formatE`
> 
All identifiers that begin with an underscore and either an uppercase letter or another underscore are always reserved for any use.
不同的编译器定义了不同的**name-mangling**系统，比较流行就是[ItaniumCXXABI](https://mentorembedded.github.io/cxx-abi/abi.html#mangling-special-guards)和Microsoft Visual C++定义的系统。
## extern “C”
C++为了与C兼容，在符号的管理上，C++有一个用来声明或定义一个C的符号的”extern C”关键字。
```
extern "C" {
    int func(int);
    int var;
}
```
C++编译器会将在**“extern ‘C’ “**的大括号内部的代码当做C语言代码处理，也就是说括号中的代码不会使用C++的名称修饰机制。
很多时候我们会碰到有些头文件声明了一些C语言的函数和全局变量，但是这个头文件可能会被C语言代码或C++代码包含。比如很常见的，我们的C语言库函数中的string.h中声明了memset函数，它的原型如下：
```cpp
void *memset(void *, int, size_t);
```
如果不加任何处理，C语言程序包含string.h的时候，并且**用到**了memset这个函数，编译器会正确处理memset的符号引用；但是在C++语言中，编译器会认为这个memset函数是一个C++函数，将memset的符号修饰成_Z6memsetPvii，这样链接器就无法和C语言库中的memset符号进行链接（注意库中符号名是C语言修饰的符号名）。所以对于C++来说，必须使用extern “C”来声明memset函数。
如下代码所示：
```
#ifdef __cplusplus
extern "C" {
#endif
void *memset (void *, int, size_t);
#ifdef __cplusplus
}
#endif
```
C++编译器会在编译C++的程序时默认定义这个宏，我们可以使用该条件宏来判断当前编译单元是不是C++代码。如果是C++代码，那么memset会在extern “C”里面被声明；如果是C代码，就直接声明。
## C++的name mangling系统
即使不同的编译器采用了相同的”name mangling”系统，还会有很多其他的问题导致不同编译生成的目标文件不能互相链接，因为**name mangling**只是C++ ABI中很少的一部分，例如异常处理（exception handling），虚表布局（virtual table layout），结构体和栈帧padding等都会导致不同编译器生成的目标文件不兼容。
C++标准没有为”name mangling”指定标准，引用g++问题列表上的一句话：
> 
“compilers differ as to how objects are laid out, how multiple inheritance is implemented, how virtual function calls are handled,  and so on, **so if the name mangling were made the same, your programs would link against libraries provided from other compilers but then crash when run**. For this reason, the ARM encourages compiler writers to make their name mangling different from that of other compilers for the same platform. **Incompatible libraries are then detected at link time, rather than at run time”**
上面的黑体部分已经说明了为什么C++标准没有将name mangling标准化。虽然有很多说明C++没有必要为name mangling指定标准，但是Itanium C++指定的mangling标准还是被很多编译器采纳了。
## Clang中的name mangling
Clang实现了两种name mangling，**[ItaniumMangle](http://code.woboq.org/llvm/clang/lib/AST/ItaniumMangle.cpp.html)** 和  **[MicrosoftMangle](http://code.woboq.org/llvm/clang/lib/AST/MicrosoftMangle.cpp.html)**，由于Clang既支持类Unix平台又支持Windows平台，所以实现了两种**name mangling**系统。
下面我们从源码层面展示一下**name mangling**，如果我们对C++中的RTTI符号名是如何编码的比较感兴趣，可以定位到相关函数查看具体的实现。
```
void ItaniumMangleContextImpl::mangleCXXRTTI(QualType Ty, raw_ostream &Out) {
    // <special-name> ::= TI <type>  # typeinfo structure
    assert(!Ty.hasQualifiers() && "RTTI info cannot have top-level qualifiers");
    CXXNameMangler Mangler(*this, Out);
    Mangler.getStream() << "_ZTI";
    Mangler.mangleType(Ty);
}
```
从图中我们可以看出在Itanium mangle系统中，虚表符号都是以”_ZTI”开头的。
```
void MicrosoftMangleContextImpl::mangleCXXRTTI(QualType T, raw_ostream &Out) {
    MicrosoftCXXNameMangler Mangler(*this, Out);
    Mangler.getStream() << "\01??_R0";
    Mangler.mangleType(T, SourceRange(), MicrosoftCXXNameMangler::QMM_Result);
    Mangler.getStream() << "@8";
}
```
如果我们对C++中的虚表符号是如何编码的比较感兴趣，可以参看与虚表**mangled name**相关的源码。
```
void ItaniumMangleContextImpl::mangleCXXVTable(const CXXRecordDecl *RD,
         raw_ostream &Out) {
    // <special-name> ::= TV <type>  # virtual table
    CXXNameMangler Mangler(*this, Out);
    Mangler.getStream() << "_ZTV";
    Mangler.mangleNameOrStandardSubstitution(RD);
}
```
具体的name mangling细则请参见： 
[*Name mangling demystified*](http://www.int0x80.gr/papers/name_mangling.pdf)
[*Itanium C++ ABI*](http://mentorembedded.github.io/cxx-abi/abi.html#mangling)
