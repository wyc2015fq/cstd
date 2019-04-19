# static_cast能够适用所有转型吗，有什么条件 - 电影旅行敲代码 - CSDN博客
2015年11月03日 21:20:47[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2132
> 
CheckStaticCast - Check that a static_cast(SrcExpr) is valid.Refer to C++ 5.2.9 for details. Static casts are mostly used for making implicit conversions explicit and getting rid of data loss warnings.—[[Clang源码]](http://code.woboq.org/llvm/clang/lib/Sema/SemaCast.cpp.html)
由Clang源码”CheckStaticCast()”函数中关于static_cast<>的解释知道，static_cast<>大部分情况下用于对隐式转换的显示化，同时去掉相关warning报警。
首先需要强调的是**static_cast<>不能去掉const限制**，const限定只能使用const_cast来去除。**static_cast<>只要具有明确定义的类型转换都可以使用**。指针类型的话，针对内置类型指针都可以，针对自定义类型指针之间的转换，需要类型之间有相关联系。
在自定义类型中，正常情况下，不同类型对象赋值只发生在派生类对象赋值给基类变量中，对象会发生”切割”。除此之外，不同类型对象之间赋值是不可能发生的，即使对象内存完全布局相同，例如父类对象赋值给子类变量是不可能的。有人会说，static_cast<>不是万能的吗？怎么可能呢，static_cast<>连简单的指针类型之间的转换都不能完全覆盖。
static_cast<dst_type>(src_tpye)需要src_type中明确定义了到dst_type的类型转换，或者dst_type的构造函数中有相对应的单参数src_type的构造函数，如下代码所示：
```bash
class dst_type;
class src_type
{
public:
    // for src_type
    // mem并不必须是dst_type，只要能够隐式构造出dst_type即可
    dst_type mem;
    operator dst_type()
    {
        return mem;
    }
};
// OR
class dst_type
{
public:
    // for dst_type
    dst_type(const src_type& value){}
}
```
例如普通毫无关联的对象之间的赋值编译时，就不会通过。
```cpp
class A{
    int a;
};
class B{
    int a;
};
int main(){
    A a;
    // 使用Visual Studio编译会出现:
    // error C2440: “static_cast”: 无法从“A”转换为“B”
    B b = static_cast<B>(a);
}
```
但是如果定义了明确的类型定义后，这种不同对象之间的转换就是允许的。例如给Class B添加了类型为A的单参数构造函数后，就可以编译通过。
```cpp
class A{
    int mem;
};
class B{
public:
    B(const A& lhs) {}
};
int main()
{
    A a;
    // static_cast<B>(a)直接调用B(const A& lhs)构造函数
    B b = static_cast<B>(a);
}
```
其实类似这种可以实现[隐式类型转换](http://www.cplusplus.com/doc/tutorial/typecasting/)的方式有三种，
- **Single-argument constructors:** allow implicit conversion from a particular type to initialize an object.
- **Assignment operator:** allow implicit conversion from a particular type on assignments.
- **Type-cast operator:** allow implicit conversion to a particular type. 
我们上面提到的是第一种。第一种有一个明确的名字，叫做转换构造函数（converting constructor）。
> 
如果一个构造函数只接受一个实参，则它实际上定义了转换此类类型的隐式转换机制，有时我们把这种构造函数称作**转换构造函数**（converting constructor）。—-Primer 5th 
另外一种更明白易懂的方式就是重载类型转换运算符，如下代码所示：
```cpp
class B;
class A{
public:
    int num;
    // 重载了转换为B的类型转换运算符
    operator B(){}
};
class B{};
int main(){
    A a;
    B b = static_cast<B>(a);
}
```
在类A中重载了转换为B的类型转换运算符，就可以使用了static_cast<>进行转换了，这是比较正规的方式。特别是在CRTP模式中，基类可以转换为模板参数类型，只是需要程序员自己控制中间的转换过程。
