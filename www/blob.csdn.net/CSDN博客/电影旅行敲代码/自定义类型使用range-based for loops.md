# 自定义类型使用range-based for loops - 电影旅行敲代码 - CSDN博客
2018年01月27日 17:46:48[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：385
# Range based for loops（范围for循环）
在[https://reviews.llvm.org/D42300#inline-369356](https://reviews.llvm.org/D42300#inline-369356) 中被提建议使用range based for loop，但是在我印象中range based for loop只被用在了c++11容器中，当时还在想有些鸡肋，就没有往下深究。**但是range based for loop确实可以被用在用户自定义的类型中，只是该类型需要满足一定的条件**。
还是自己没有养成良好的思考习惯，都是被动的接受知识，即使《深入理解C++11》中白纸黑字写着相关内容，自己却视而不见。但凡自己质疑一些，深入思考一些，也就不会出现如此的问题。
重点内容：
- **范围for的等价形式**
- **如何使自定义类型使用范围for**
- **Argument-Dependent Lookup**
- **universal reference**
## 范围for循环
范围for循环是c++11中增加的新特性，范围for循环避免了越界，提高了程序的可读性，提高程序编写效率。与传统的for_each相似，两者都避免了程序员在访问数据时，自己去自增或自减”计数器”。
### Range-based for loop (since C++11) from cppreference.com
这里我摘抄[Range-based for loop (since C++11)](http://en.cppreference.com/w/cpp/language/range-for)中关于range based for的介绍：
> 
attr(optional) for (range_declaration : range_expression) loop_statement
- range_declaration - a declaration of a named variable, whose type is the type of the element of the sequence represented by range_expression, or a reference to that type. Often uses the auto specifier for automatic type deduction.
- range_expression - any expression that represents a suitable sequence (**either an array** or an **object for which begin and end member functions or free functions are defined**) or a braced-init-list.
上面我们比较关注的是关于range_expression的描述，range_expression可以是一个序列或者一个对象，**这个对象定义了begin()和end()成员函数**，或者定义了**和begin()和end()相关的free函数，其中free函数可以理解成普通函数**。通过上面的描述，可以看到range based for loop可以被用在自定义类型上，只要该类型定义了begin()或者end()成员方法，或者相关的普通函数。
### The range-based for statement from c++ standard(draft-n4700)
关于这一点C++标准中其实定义的更明确，这里摘抄[draft-n4700](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4700.pdf)，如下所示：
The range-based for statement
> 
for ( *for-range-declaration* : *for-range-intializer*) *statement*
is equivalent to
```
{
    auto &&__range = for-range-initializer;
    auto __begin = begin-expr;
    auto __end = end-expr;
    for ( ; __begin != __end; ++__begin) {
        for-range-declaration = *__begin;
        statement
    }
}
```
***begin-expr*** and ***end-expr*** are determined as follows:
- …
- if the *for-range-initializer* is an expression of *class type C*, the *unqualified-ids***begin** and **end** are looked up in the scope of ***C*** as if by class member access lookup (6.4.5), and if either (or both) finds at least on declaration, *begin-expr* and *end-expr* are **__range.begin()** and **__range.end()**, respectively;
- otherwise, *begin-expr* and *end-expr* are **begin(__range)** and **end(__range)**, respectively, where **begin** and **end** are lookup up in the associated namespaces (6.4.2). [*Note:* Ordinary unqualified lookup (6.4.1) is not performed. - *end note*]
上面有三点需要注意，这里分别进行说明：
- range based for loop的等价形式
- begin()和end()的查找
- auto &&以及完美转发
#### range-based for loop的等价形式
range based for loop的等价形式我们已经在上面给出了，知乎上也有类似的问题，如[for(auto s:sp)中s是每次迭代都重新定义一次吗？](https://www.zhihu.com/question/29865499/answer/45867705)，这就引出了一个问题，就是如下几种形式的范围for有什么区别？
```cpp
#include <vector>
#include <iostream>
#include <utility>
using namespace std;
class ObjectT {
    int mem;
public:
    ObjectT() : mem(0) {
        cout << "Constructor \n";
    }
    ObjectT(const ObjectT &lhs) : mem(lhs.mem){
        cout << "Copy constructor \n";
    }
    ObjectT(ObjectT &&lhs) : mem(std::move(lhs.mem)){
        cout << "Move constructor \n";
    }
    ObjectT& operator=(const ObjectT &lhs) {
        // self assignment check?
        // Since class is POD-type, there is no need to check self assignment.
        // Or use copy-swap idiom, see 
        // https://stackoverflow.com/questions/22023485/checking-for-self-assignment-in-operator,
        // https://chara.cs.illinois.edu/sites/cgeigle/blog/2014/08/27/copy-and-swap/ 
        mem = lhs.mem;
        return *this;
    }
    ObjectT& operator=(ObjectT &&) {
        mem = std::move(mem);
        return *this;
    }
    ~ObjectT() {}
    int getValue() { return mem; }
};
int main() {
    vector<ObjectT> Vec(10);
    cout << Vec.size() << endl;
    cout << Vec.capacity() << endl;
    // i. auto item
    for (auto item : Vec) {}
    // ii. auto &
    for (auto &item : Vec) {}
    // iii. auto &&
    for (auto &&item : Vec) {}
    return 0;
}
```
三种形式**for (auto item : Vec)**，**for (auto &item : Vec)**以及**for(auto &&item : Vec)**有什么区别。我们使用范围for的等价形式将上述三种形式进行替换后分别如下所示：
```
// i. equivalent "for (auto item : Vec)"
{
    auto &&__range = Vec;
    auto __begin = Vec.begin();
    auto __end = Vec.end();
    for (; __begin != __end; ++__begin) {
        auto item = *__begin;
        // ...
    }
}
// ii. equivalent "for (auto &item = Vec)"
{
    auto &&__range = Vec;
    auto __begin = Vec.begin();
    auto __end = Vec.end();
    for (; __begin != __end; ++__begin) {
        auto &item = *__begin;
    }
}
// iii. equivalent "for (auto &&item : Vec)"
{
    auto &&__range = Vec;
    auto __begin = Vec.begin();
    auto __end = Vec.end();
    for (;__begin != __end; ++__begin) {
         auto &&item = *__begin;
    }
}
```
从上面可以看出，三种形式就是在获取元素的时候，一种是拷贝，一种是引用，一种是universal reference（原谅我没有翻译），universal reference见下面的auto&&。
#### begin()和end()的查找
关于begin()和end()的查找，C++标准中说的很清楚首先在 `for-range-initializer`类型`C`的scope中查找，查找规则类似于`class member access lookup`。
当在类型`C`的scope中找不到`begin()`和`end()`的定义时，则使用[**Argument-dependent name lookup**](http://en.cppreference.com/w/cpp/language/adl)，称为`ADL`或者`Koenig Lookup`，该规则从字面意思上理解就是根据依赖的`Argument`来进行名字查找。
相应的介绍见：
- [What is “Argument-Dependent Lookup” (aka ADL, or “Koenig Lookup”)?](https://stackoverflow.com/questions/8111677/what-is-argument-dependent-lookup-aka-adl-or-koenig-lookup)
- [Tip of the Week #49: Argument-Dependent Lookup](https://abseil.io/tips/49)
在这里ADL的含义就是根据`begin()`和`end()`的参数类型，在其定义的作用域中查找`begin()`和`end()`，并不必须是参数类型的成员方法。例如：
```
namespace MYType {
    void begin();
    void end();
    class Array {
    };
}
int main() {
    MYType::Array A;
    begin(A);  // ADL is applied, find MYType::begin()
}
```
但是根据C++标准中提到的，`begin()`和`end()`的查找分为两步：
- 首先按照`class member access lookup`查找`begin()`和`end()`的定义
- 找不到的话，然后按照ADL的原则去查找`begin()`和`end()`
所以总体上看`begin()`和`end()`的查找都是和`for-range-intializer`的类型相关的，如果只有普通的`begin()`和`end()`是编译不过去的。如下代码所示：
```
// compiler error occurred
// temp.cpp:14:16: error: invalid range expression of type 'MYType::Array'; no viable 'begin' function available
//        for (auto ele : A) {}
//                      ^ ~
namespace MYType {
    class Array {
    public:
        int mem;
    };
    // int* begin(Array A) { &A.mem; } // just for test
    // int* end(Array A) { &A.mem; } // just for test
}
int* begin(Array A) { &A.mem; } // just for test
int* end(Array A) { &A.mem; } // just for test
int main() {
    MYType::Array A;
    for (auto ele : A) {}
    return 0;
}
```
关于ADL还有很多有趣的地方，它和模板有非常紧密的关系，我不是C++ lawyer，有兴趣的可以查找一些资料。
#### auto&&
关于`universal reference`，scott meyers的文章[Universal References in C++11—Scott Meyers](https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers)解释的很清楚。我这里就不献丑了，主要记住一下几点即可：
- T&& Doesn’t Always Mean “Rvalue Reference”
- `universal reference`既可以接受左值，也可以接受右值
- `universal reference`只存在于有类型推导的场景中，`auto &&`需要类型推导，所以`auto &&`一定是`universal reference`
- `universal reference`大量存在于模板场景中
- `auto`的类型推导规则与模板的类型推导规则相同
- `universal reference`的实现机制是`reference collapsing`（引用折叠），并且是完美转发的实现方式。
*资料：[Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/)*
所以在不知道`for-range-initializer`是左值还是右值的情况下，需要使用`auto&&`接受其值。
关于完美转发，完美转发的形式通常如下：
```cpp
void RunCode(int &&m) { cout << "rvalue ref" << endl; }
void RunCode(int &m) { cout << "lvalue ref" << endl; }
void RunCode(const int &&m) { cout << "const rvalue ref" << endl; }
void RunCode(const int &m) { cout << "const lvalue ref" << endl; }
template <typename T>
void PerfectForward(T &&t) { RunCode(forward<T>(t));}
// Note `T&&` is universal reference, it's a lvalue
```
*注：`std::forward<T>(t)`相当于`static_cast<T&&>(t)`*
`reference collapsing`的规则可以简单看成如下的形式：
```
// reference collapsing
&       &       -> &
&       &&      -> &
&&      &       -> &
&&      &&      -> &&
// universal references
&&      &       -> &
&&      &&      -> &&
```
## 自定义类型使用range based for loops
通过上面的介绍，如果要使自定义类型能够应用范围for，关键点就在于`begin()`和`end()`，两者的定义有两种形式：
- 作为自定义类型的成员方法
- 作为普通函数定义在该类型的定义所在的namespace中，以便能够应用ADL原则
另外需要注意的一点是，`begin()`返回的值必须能够应用`operator*`，因为范围for的等价形式如下，其中有`*__begin()`的操作。也就是说如果想让自定义类型应用范围for，必须能够使下面的代码编译通过，当然还需要仔细处理`begin()`和`end()`的代码逻辑。
```
{
    auto &&__range = for-range-initializer;
    auto __begin = begin-expr;
    auto __end = end-expr;
    for ( ; __begin != __end; ++__begin) {
        for-range-declaration = *__begin;
        statement
    }
}
```
