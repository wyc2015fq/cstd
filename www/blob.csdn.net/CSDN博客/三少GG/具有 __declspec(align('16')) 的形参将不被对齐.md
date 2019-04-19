# 具有 __declspec(align('16')) 的形参将不被对齐 - 三少GG - CSDN博客
2013年02月22日 15:13:36[三少GG](https://me.csdn.net/scut1135)阅读数：7243
没用到，但应该有效！！！要改的内容太多
错误 1 error C2719: “_Val”: 具有 __declspec(align('16')) 的形参将不被对齐 C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include\vector 717
[对齐的结构无法使用stl vector解决方案](http://tech.chinaunix.net/a2009/1117/810/000000810717.shtml)
2009年11月26日 13:51 来源：老所工作室 作者：老所工作室 编辑：覃里
　【**IT168 文档**】在CUDA编程中，结构的对齐(alignment)是非常重要的。在我写的CUDA 结构对齐的C++模拟这篇文章中，我介绍了如何在C++里模拟CUDA对结构的内存对齐机制，从而使得我们可以在C++里构造结构，然后传到CUDA里进行处理。但是随后我又遇到一个问题。比如，我们有这么一个对齐了的结构：
template <unsigned int Len,
 typename ValueType>
struct ALIGN16 Foo
{
    ValueType x[Len];
    ...
};
　　当我们程序里使用了std::vector< Foo<5, float> > v(1);，也就是构造一个内存自管理的数组，并设置其初始大小为1。这段代码在g++里能够正常编译，但当我们用msvc(我用的是vc80)来编译的话，则会遇到报错：
cl /nologo /TP /EHsc /wd4819 /DBOOST_BIND_ENABLE_STDCALL /DBOOST_MEM_FN_ENABLE_S
TDCALL /O2 /MD /I. /c
 test\base\test_carray.cpp /Fotest\base\test_carray.obj
test_carray.cpp
C:\Program Files\Microsoft Visual Studio 8\VC\INCLUDE\vector(694) : error C2719:
'_Val': formal parameter with __declspec(align('16'))
 won't be aligned
        test\base\test_carray.cpp(278) : see reference to class template
 instant
iation 'std::vector<_Ty>' being compiled
        with
        [
            _Ty=carray_type
        ]
scons: *** [test\base\test_carray.obj] Error 2
scons: building terminated because of errors.
　　我们搜索STL vector的694行，发现有这样的代码：
void resize(size_type _Newsize, _Ty _Val)
        {    // determine new length, padding with _Val elements as needed
if (size() < _Newsize)
            _Insert_n(end(), _Newsize - size(), _Val);
elseif (_Newsize < size())
            erase(begin() + _Newsize, end());
        }
　　问题就出在其第二个参数上，我们可以看到，这是个传值的参数，需要在函数调用的参数栈上创建一个临时的参数，但当这个类型是被对齐了的话，msvc编译器就会报错。
　　那么，怎么解决呢?直接修改STL代码，显然不是一个好方法。这时我想到了模板的特例化，可以用这种非侵入式的改变来解决这个问题，同时保证不修改STL代码。
　　首先，我们创建一个新的文件，比如叫foo_vector.hpp，并大致写成：
#ifndef FOO_VECTOR_HPP
#define FOO_VECTOR_HPP
// only msvc need to be fix
#ifdef _MSC_VER // 1400 for vc80
#include <vector>
#include "foo.hpp"
namespace std
{
// our specialization goes here...
}
#endif // _MSC_VER
#endif
　　这里，我们首**先判断编译器，如果是msvc，才进行我们的特例化，因为g++没有这个问题(并且我发现，msvc使用的STL代码和g++使用的STL代码是不相同的)。**这里我没有进行msvc版本的判断，因为是demo嘛。
　　然后打开msvc**自带的STL vector代码(在”Microsoft Visual Studio 9.0\VC\include\vector”)，将整个vector类的定义拷贝过来，大致在434行到1292行之间**。然后修改模板的头，改为我们的特例化：
// TEMPLATE CLASS vector
template<unsigned int Len, typename ValueType,
class _Ax>
class vector < Foo<Len, ValueType>, _Ax >
    : public _Vector_val<Foo<Len, ValueType>, _Ax>
{    // varying size array of values
public:
typedef Foo<Len, ValueType> _Ty;
    typedef vector<_Ty, _Ax> _Myt;
    typedef _Vector_val<_Ty, _Ax> _Mybase;
    .......
　　这样，我们就完成了vector对我们Foo结构的特例化，接下来找到刚才产生问题的resize()方法，改为：
void resize(size_type _Newsize, const _Ty & _Val)
    { // determine new length, padding with _Val elements as needed
if (size() < _Newsize)
            _Insert_n(end(), _Newsize - size(), _Val);
elseif (_Newsize < size())
            erase(begin() + _Newsize, end());
    }
　　注意其中红色部分，将传值改为传参，避免在参数栈上创建被对齐的结构的对象。然后，在我们使用std::vector< Foo<5, float> >之前包含我们的foo_vector.hpp头文件，就可以正常使用了。
