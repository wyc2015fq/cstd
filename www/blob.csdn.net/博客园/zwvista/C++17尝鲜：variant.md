# C++17尝鲜：variant - zwvista - 博客园

## [C++17尝鲜：variant](https://www.cnblogs.com/zwvista/p/9237183.html)

### variant

variant 是 C++17 所提供的变体类型。`variant<X, Y, Z>` 是可存放 X, Y, Z 这三种类型数据的变体类型。
- 与C语言中传统的 union 类型相同的是，variant 也是联合(union)类型。即 variant 可以存放多种类型的数据，但任何时刻最多只能存放其中一种类型的数据。
- 与C语言中传统的 union 类型所不同的是，variant 是可辨识的类型安全的联合(union)类型。即 variant 无须借助外力只需要通过查询自身就可辨别实际所存放数据的类型。

`v = variant<int, double, std::string>` ，则 v 是一个可存放 int, double, std::string 这三种类型数据的变体类型的对象。
- `v.index()` 返回变体类型 v 实际所存放数据的类型的下标。

变体中第1种类型下标为0，第2种类型下标为1，以此类推。
- `std::holds_alternative<T>(v)` 可查询变体类型 v 是否存放了 T 类型的数据。
- `std::get<I>(v)` 如果变体类型 v 存放的数据类型下标为 I，那么返回所存放的数据，否则报错。
`std::get_if<I>(&v)` 如果变体类型 v 存放的数据类型下标为 I，那么返回所存放数据的指针，否则返回空指针。
- `std::get<T>(v)` 如果变体类型 v 存放的数据类型为 T，那么返回所存放的数据，否则报错。
`std::get_if<T>(&v)` 如果变体类型 v 存放的数据类型为 T，那么返回所存放数据的指针，否则返回空指针。

```
#include <iostream>
#include <string>
#include <variant>

using namespace std;

int main()
{
    variant<int, double, string> v; // v == 0
    v = 1;
    bool has_int = holds_alternative<int>(v);
    bool has_double = holds_alternative<double>(v);
    cout << v.index() << has_int << has_double << get<0>(v) << *get_if<0>(&v) << endl; // 01011
    v = 2.0;
    cout << v.index() << (get_if<int>(&v) == nullptr) << get<1>(v) << get<double>(v) << endl; // 1122
    v = "a";
    cout << v.index() << get<2>(v) << get<string>(v) << endl; // 2aa
}
```

### std::visit

`std::visit(f, v)` 将变体类型 v 所存放的数据作为参数传给函数 f。
`std::visit(f, v, u)` 将变体类型 v, u 所存放的数据作为参数传给函数 f。

...

std::visit 能将所有变体类型参数所存放的数据作为参数传给函数参数。

```
#include <iostream>
#include <string>
#include <variant>
#include <boost/hana/functional/overload.hpp>

using namespace std;
namespace hana = boost::hana;
 
struct Visitor {
    void operator()(int n) const {
        cout << "int: " << n << endl;
    }
    void operator()(double d) const {
        cout << "double: " << d << endl;
    }
    void operator()(const string& s) const {
        cout << "string: " << s << endl;
    }
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
 
int main()
{
    variant<int, double, string> v; // v == 0
    auto f = [](auto& x) {cout << x << endl;};
    Visitor f2;
    overloaded f3{
        [](int n){cout << "int: " << n << endl;},
        [](double d){cout << "double: " << d << endl;},
        [](const string& s){cout << "string: " << s << endl;}
    };
    auto f4 = hana::overload(
        [](int n){cout << "int: " << n << endl;},
        [](double d){cout << "double: " << d << endl;},
        [](const string& s){cout << "string: " << s << endl;}
    );
    auto f5 = [](auto& arg) { using T = decay_t<decltype(arg)>; 
//  auto f5 = []<typename T>(T& arg) { // C++ 20
        if constexpr (is_same_v<T, int>) {
            cout << "int: " << arg << endl;
        }
        else if constexpr (is_same_v<T, double>) {
            cout << "double: " << arg << endl;
        }
        else if constexpr (is_same_v<T, string>) {
            cout << "string: " << arg << endl;
        }
    };
    v = 1; visit(f, v); visit(f2, v); visit(f3, v); visit(f4, v); visit(f5, v); // 1 int: 1 int: 1 int: 1 int: 1
    v = 2.0; visit(f, v); visit(f2, v); visit(f3, v); visit(f4, v); visit(f5, v); // 2 double: 2 double: 2 double: 2 double: 2
    v = "a"; visit(f, v); visit(f2, v); visit(f3, v); visit(f4, v); visit(f5, v); // a string: a string: a string: a string: a
}
```
- f 和 f5 是泛型 lambda，接受所有参数的类型。

f 不分辨参数类型。

f5 通过编译期 if 语句来分辨参数类型。
- f2 和 f3 是函数对象，通过重载函数调用操作符来分辨参数的类型。

f2 的函数调用操作符由自身定义。

f3 的函数调用操作符继承自3个lambda。
- f4 这个函数对象经由 boost::hana::overload 函数生成，该函数所生成的函数对象能从多个lambda参数中选取一个合适的来调用指定参数。
- 这段代码总共使用了三个C++17的新特性：

适用于类模板的自动推断向导 [https://www.cnblogs.com/zwvista/p/7748363.html](https://www.cnblogs.com/zwvista/p/7748363.html)

变长 using 声明 [https://www.cnblogs.com/zwvista/p/9256655.html](https://www.cnblogs.com/zwvista/p/9256655.html)

编译期 if 语句 [https://www.cnblogs.com/zwvista/p/9238273.html](https://www.cnblogs.com/zwvista/p/9238273.html)


