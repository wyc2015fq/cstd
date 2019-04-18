# C++17尝鲜：变长 using 声明 - zwvista - 博客园

## [C++17尝鲜：变长 using 声明](https://www.cnblogs.com/zwvista/p/9256655.html)

### using 声明

先来看 using 声明在类中的应用：

代码1

```
#include <iostream>

using namespace std;

struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};

struct S : A {
};

int main()
{
    S s;
    s.f(1); // A::f(int)
}
```
- 类 S 继承了类 A 的成员函数 f，所以类 S 的实例 s 调用 f 输出为 A::f

代码2

```
#include <iostream>

using namespace std;

struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};

struct S : A {
    void f(double) {cout << "S::f(double)" << endl;}
};

int main()
{
    S s;
    s.f(1); // S::f(double)
}
```
- 类 S 继承了类 A 的成员函数 f，同时类 S 也定义了成员函数 f
- 类 S 的成员函数 f 遮蔽了基类中的同名成员函数，所以 S 的实例 s 调用 f 输出为 S::f

代码3

```
#include <iostream>

using namespace std;

struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};

struct S : A {
    using A::f;
    void f(double) {cout << "S::f(double)" << endl;}
};

int main()
{
    S s;
    s.f(1); // A::f(int)
    s.f(2.5); // S::f(double)
}
```
- 类 S 继承了类 A 的成员函数 f，同时类 S 也定义了成员函数 f
- 类 S 通过 using 声明将基类 A 的成员函数 f 引入自己的作用域。类 S 的成员函数 f 与基类 A 中的同名成员函数形成重载关系。
- 参数为整型时，根据重载决策 S 的实例 s 调用 f 输出为 A::f
- 参数为浮点型时，根据重载决策 S 的实例 s 调用 f 输出为 S::f

代码4

```
#include <iostream>

using namespace std;

struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};

struct B {
    void f(double) {cout << "B::f(double)" << endl;}
};

struct S : A, B {
};

int main()
{
    S s;
    s.f(1); // compile error
    s.f(2.5); // compile error
}
```
- 类 S 同时继承了类 A 和类 B 的成员函数 f，两者形成竞争关系。
- 编译器不能判断 S 的实例 s 所调用的成员函数 f 来自类 A 还是类 B，故编译出错。

代码5

```
#include <iostream>

using namespace std;

struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};

struct B {
    void f(double) {cout << "B::f(double)" << endl;}
};

struct S : A, B {
    using A::f;
    using B::f;
};

int main()
{
    S s;
    s.f(1); // A::f(int)
    s.f(2.5); // B::f(double)
}
```
- 类 S 同时继承了类 A 和类 B 的成员函数 f。
- 类 S 通过 using 声明将基类 A 和基类 B 的成员函数 f 都引入自己的作用域。基类 A 和基类 B 的同名成员函数在类 S 中形成重载关系。
- 参数为整型，根据重载决策 S 的实例 s 调用 f 输出为 A::f
- 参数为浮点型时，根据重载决策 S 的实例 s 调用 f 输出为 B::f

### C++17的 using 声明

在 C++17 中多个 using 声明可以通过逗号连接起来，合成一个 using 声明。

代码6

```
#include <iostream>

using namespace std;

struct A {
    void f(int) {cout << "A::f(int)" << endl;}
};

struct B {
    void f(double) {cout << "B::f(double)" << endl;}
};

struct S : A, B {
    using A::f, B::f; // C++17
};

int main()
{
    S s;
    s.f(1); // A::f(int)
    s.f(2.5); // B::f(double)
}
```

### C++17的变长 using 声明

通过使用加了 ... 的 using 声明，可以将变长模板参数类型中的 using 声明转化为由多个用逗号合成的变长 using 声明。

```
#include <iostream>
#include <string>

using namespace std;

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main()
{
    overloaded s{
        [](int){cout << "int" << endl;},
        [](double){cout << "double" << endl;},
        [](string){cout << "string" << endl;},
    };
    s(1); // int
    s(1.); // double
    s("1"); // string
}
```
- `template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };`

这是一个类模板的声明。
- `template<class... Ts>`：overloaded 类的模板参数为可变长的参数包 Ts。

假设 Ts 包含 T1, T2, ... , TN，那么这一句声明可以展开为：`template<class T1, class T2, ..., class TN>`
- `struct overloaded : Ts...`：overloaded 类的基类为参数包 Ts 内所有的参数类型。

假设 Ts 包含 T1, T2, ... , TN，那么这一句声明可以展开为：`struct overloaded : T1, T2, ..., TN`
- `using Ts::operator()...;`：这是一个变长 using 声明。

假设 Ts 包含 T1, T2, ... , TN，那么这一句声明可以展开为：`using T1::operator(), T1::operator(), ..., TN::operator();`

也就是说，overloaded 类的基类即参数包 Ts 内所有的参数类型的函数调用操作符均被 overloaded 类引入了自己的作用域。
- `template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;`

这是一个自动推断向导，用于帮助编译器根据 overloaded 构造器参数的类型来推导 overloaded 的模板参数类型。

这个自动推断向导告诉编译器，如果 overloaded 构造器所有参数的类型的集合为Ts，那么 overloaded 的模板参数类型就是 Ts 所包含的所有类型。

也就是说如果表达式 a1, a2, ..., an 的类型分别为 T1, T2, ..., TN,

那么构造器表达式 `overloaded {a1, a2, ..., an}` 的类型就是 `overloaded<T1, T2, ..., TN>`。

*`overloaded s{`
`[](int){cout << "int" << endl;},`
`[](double){cout << "double" << endl;},`
`[](string){cout << "string" << endl;},`
`};`

overloaded 类的实例 s 的构造器包含3个lambda参数，也可以看作3个各自包含一个 operator() 的函数对象。

根据 overloaded 类的定义，s 对象将继承这3个lambda（函数对象）的 operator() ，也就是说这3个lambda的 operator() 即函数体在 s 对象内部形成重载关系。

根据 overloaded 类的自动推断向导，s 对象的类型为`overloaded<T1, T2, T3>`，其中T1, T2, T3为3个lambda参数的类型。
- 通过利用 C++17 的新特性变长的 using 声明以及自动推断向导，overloaded类的实例可以简洁并且巧妙地将多个lambda合成一个大的具有多个相互重载的 operator() 的函数对象。
- overloaded 这个类模板如此有用，实现机制又如此精妙，实在是应该早日纳入标准库中。


