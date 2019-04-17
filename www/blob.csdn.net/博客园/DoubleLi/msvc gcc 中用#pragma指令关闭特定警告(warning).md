# msvc/gcc:中用#pragma指令关闭特定警告(warning) - DoubleLi - 博客园






在使用一些第三方库或源码的时候,经常会遇到编译时产生warnings情况,这些warning不是我们自己的代码产生的，当然也不好去修改，但每次编译都显示一大堆与自己代码无关的警告也着实看着不爽，更麻烦的是还有可能造成自己代码中产生的警告被淹没在多过的无关警告中，而被忽略掉的情况。 
所以要想办法关闭这些第三方代码和库产生的警告。 
关闭特定的warning可以在编译时通过命令行参数的方式指定，比如 gcc 是在命令行一般是用-Wno-xxxx这样的形式禁止特定的warning,这里xxxx代入特定的警告名。但这种方式相当将所有代码产生的这个warning显示都关闭了，不管是第三方库产生的还是自己的代码产生的，所以这种用法并不适合。 
关闭特定的warning还可以在代码中通过添加`#pragma`指令来实现,用`#pragma`指令可以对指定的区域的代码关闭指定的warning。

msvc下的用法是这样的

```cpp
#ifdef _MSC_VER
// 关闭编译CImg.h时产生的警告
#pragma  warning( push ) 
#pragma  warning( disable: 4267 4319 )
#endif
#include "CImg.h"
#ifdef _MSC_VER
#pragma  warning(  pop  ) 
#endif
```

gcc下的用法是这样的：

```cpp
#ifdef __GNUC__
// 关闭 using  _Base::_Base; 这行代码产生的警告
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winherited-variadic-ctor"
#endif
.....
namespace cimg_library {
template<typename T>
class CImgWrapper:public CImg<T> {
public:
    using   _Base =CImg<T>;
    using  _Base::_Base; // 继承基类构造函数
    ......
}
} /* namespace cimg_library */
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
```
- 

参考资料: 
[https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)
[https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas](https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas)









