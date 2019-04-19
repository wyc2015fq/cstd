# RTTI机制详解 - 电影旅行敲代码 - CSDN博客
2015年12月14日 19:10:08[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：740
# 引子
以前我们探讨过，RTTI的访问和虚表相关，并且RTTI基本上是为了dynamic_cast<>和typeid以及异常而实现的。今天我们详细探讨下异常的实现机制。
RTTI、虚表以及类的内存布局遵循的通常是[Itanium C++ ABI](http://mentorembedded.github.io/cxx-abi/abi.html#rtti)，我不知道这个名字为什么会演化成现在这个样子，但是这个ABI标准是现今最流行的一个C++ABI标准。下面我们会精简地从中摘取出关于RTTI的相关内容。
## C++ABI中的RTTI
通过C++编程语言的定义，我们知道类型信息在用于以下三个目的时，可以在运行时获得：
**a. 为了支持typeid运算符**
**b. 为了匹配出现异常时所抛出的异常对象**
**c. 为了实现dynamic_cast<>运算符**
其中(c)仅仅需要多态类的类型信息，但是(a)和(b)需要其他类型的类型信息。例如当一个指向int的指针被作为异常抛出，被一个捕获**“int const *”**的handle捕获时，就需要RTTI。
只要两个type_info指针相同，就表明其指向的type_info相同。任何关于RTTI的实现都必须满足这个要求。
对于RTTI所生成的位置，有以下描述。尽量使RTTI所生成的位置数最少。对于多态类型来说，一个相似的问题就是虚表，于是RTTI应该和该类型的虚表一起生成。对于其他类型来说，它们必须在使用类型信息的地方产生，当然目标文件必须得包含typeid，throw或者是catch。
基本类型的类型信息（例如，int, bool等等）将会存放在运行时库中。特别的，对于类型”X”来说，运行时库应该包含”X”, “X*”, “X const *”，类型X包括void, std::nullptr_t, bool, char, unsigned char, signed char, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long, float, double, long double, char16_t, char32_t等等。
**注：曾经实验室的同学一直对RTTI为什么一定要通过虚表有疑问，其实并不是RTTI必须得和虚表一起生成，而是这样做是最合适的。想一下，**
**（1）虚表是为了多态类型产生的，而多态类型也是需要RTTI信息的，一个类型在一个编译单元中只能有一个虚表（多态继承时，是有多个虚表的），多个对象共享同一个虚表。RTTI同样满足这个性质，所以对于多态类来说，RTTI和虚表生成在一起，何乐而不为呢。**
**（2）当然也可以生成在其他地方，只要能有一种机制能够找到type_info类型信息即可。C++ABI中也明确规定对于多态类型来说，RTTI应该和虚表一起生成。注意是应该，当然如果你没有按照这个标准来做的话，那么你的可执行体就会出现兼容性问题，因为你和大家的不一样嘛。**
对于typeid运算符，C++ABI有如下的规定，typeid运算符会产生一个指向std::type_info结构体的引用，其中std::type_info结构体必须满足一下接口：
```cpp
namespace std{
    class type_info {
        public:
            virtual ~type_info();
            bool operator==(const type_info &) const;
            bool operator!=(const type_info &) const;
            bool before(const type_info &) const;
            const char* name() const;
        private:
            // 由于对于每个类型来说都只有一个类型结构体
            // 并且在使用的时候都是通过指针访问的，当然无需分配在栈上
            // 分配在堆上，并且使用指针指向即可，所以连拷贝构造函数都定义成private
            // 当然，现在C++11提供了delete，所以无需通过这种取巧的方式来达到类似的效果了
            type_info(const type_info& rhs);
            type_info& operator=(const type_info& rhs);
    };
}
```
在链接和加载之后，只能有一个std::type_info结构体是可访问的，并且只能够通过C++ABI定义的特定的类型名来访问，这个类型必须是完整类型。判断类型相等或者不想等都是通过地址比较来实现的。但时对于指针类型来说，会比较复杂，具体可以参考[这里](http://mentorembedded.github.io/cxx-abi/abi.html#rtti)。
由于编译器允许虚表和RTTI在不同的编译单元中都存在，在链接的时候，链接器能够正确判断当前需要删除冗余而不是报“重定义”错误呢，关键在于虚表和RTTI时存放在目标文件中的**COMDAT**命名的section中的。这样链接器就能够根据不同的类型的内容作出不同的行为。
> 
注：**COMDAT**块的目的就是允许在多个目标文件中的“重复”存在。通常如果一个相同的符号定义在多个目标文件中，链接器会报出相关错误。但是这样会对一些C++语言特性造成一些问题，比如template，有可能在多个cpp文件中实例化出同一份符号。
**COMDAT**块就是用来解决C++特性中的问题的。当一个目标文件中的区块被标识为COMDAT的时候，同样的，也会设置一个标志来说明如何解决这些冗余造成的冲突。这里有很多选项，包括“随便挑选一个符号”，“确保所有重复的符号都有相同的大小”，“确保所有重复的符号都有相同的内容”，或者是“挑选一个最大”等等。如果想了解关于这方面的内容，可以看一些COFF规范。—[[What is the COMDAT section used for]](http://stackoverflow.com/questions/1834597/what-is-the-comdat-section-used-for)
关于RTTI另一个有关的内容就是dynamic_cast<>，也曾经有人和我纠结过dynamic_cast<>是不是必须得用于多态类型，答案为不是。虽然dynamic_cast<>大部分用于多态类型的指针或者引用（用于引用的话不是很妥当，如果转换不过去不可能产生空的引用），并且在。但是dynamic_cast<>也可以在编译时期被处理掉，并不需要RTTI。例如转换到”void *cv”就是在编译时期做的。所以所有向空指针的dynamic_cast<>都是静态的。
## MSVC中的RTTI
对于MSVC来说，MSVC会创建一个指向**“Complete Object Locator”**(即完整对象定位器)的指针并将其存放在vftable的前面。COL的定义如下：
```
struct RTTICompleteObjectLocator
{
    // 双字的签名
    DWORD signature;
    // 与当前结构体相关的虚表在类内存布局中偏移量
    DWORD offset;
    // 构造函数的偏移
    DWORD offset;
    // 指向完整类型的类型描述符
    struct TypeDescriptor* pTypeDescriptor;
    // 描述继承体系的指针
    struct RTTIClassHierarchyDescriptor* pClassDescriptor;
};
```
类继承体系描述符描述了当前类的继承体系。它被当前类的所有COLs所共享，在多重继承下有多个虚表。
```
struct RTTIClassHierarchyDescriptor
{
    DWORD signature;
    // 多重继承设置为0，虚继承设置为1
    DWORD attributes;
    // pBaseClassArray的个数
    DWORD numBaseClasses;
    struct RTTIBaseClassArray* pBaseClassArray;
};
```
## GCC中的RTTI
