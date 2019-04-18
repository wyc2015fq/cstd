# 【C++】C++ 17简单上手（2）——string_view - Pangda NoSilly - CSDN博客

2018年06月26日 21:55:29[hepangda](https://me.csdn.net/hepangda)阅读数：2962


## 什么是`string_view`

`std::string_view`是C++ 17标准中新加入的类，正如其名，它提供一个字符串的视图，即可以通过这个类以各种方法“观测”字符串，但不允许修改字符串。由于它只读的特性，它并不真正持有这个字符串的拷贝，而是与相对应的字符串共享这一空间。即——构造时不发生字符串的复制。同时，你也可以自由的移动这个视图，移动视图并不会移动原定的字符串。

正因这些特性，当你不需要改变字符串时，应当抛弃原来使用的`const string`而采用新的`string_view`，这样可以避免多余的字符串拷贝。

## 构造

`std::string_view`允许通过C风格的字符串、字符串字面量、`std::string`或者其他的`string_view`进行构造。在构造的同时允许指定“大小”。

```cpp
const char *cstr_pointer = "pointer";
char cstr_array[] = "array";
std::string stdstr = "std::string";

std::string_view
    sv1(cstr_pointer, 5),  // 使用C风格字符串-指针构造，并指定大小为5
    sv2(cstr_array),    // 使用C风格字符串-数组构造
    sv3("123456", 4),   // 使用字符串字面量构造，并指定大小为4
    sv4(stdstr),        // 使用std::string构造
    sv5("copy"sv);      // 使用拷贝构造函数构造（sv是std::string_view字面量的后缀）

std::cout
    << sv1 << endl    // point
    << cstr_pointer << endl // pointer
    << sv2 << endl    // array
    << sv3 << endl    // 1234
    << sv4 << endl    // std::string
    << sv5 << endl;   // copy
```

## 一些常见操作

如同`std::string`一样，`string_view`支持迭代器，也同样支持获取元素等操作，标准库也为它编写了相关的`istream`和`ostream`的运算符重载形式，并重载了`swap`函数

```cpp
// using namespace std;
string_view sv("123456789", 5);

// 支持Range-based for
for (auto i : sv) {
    cout << i << ' ';
}
// 支持begin/end, cbegin/cend, rbegin/rend, crbegin/crend迭代器
for (auto it = sv.crbegin(); it != sv.crend(); ++it) {
    cout << *it << ' ';
}

cout << sv.at(0) << ' ' << sv[1]; // 输出指定位置上的字符
cout << sv.front();  // 输出首位字符
cout << sv.back();   // 输出末位字符
cout << sv.size() << ' ' << sv.length(); // 输出视图中的字符串长度
cout << sv.data();   // 输出字符串存储的位置
cout << sv.empty();  // 输出字符串是否为空
cout << sv.substr(0, 2); // 输出字符串的子串(用法与string的一致)

string_view sv2("12345");
cout << (sv == sv2 ? "true" : "false"); // 支持同string一样的各种比较符号
```

## 更改视图的大小

类中提供了两个函数`remove_suffix`（从后面缩减大小）和`remove_prefix`（从前方缩减大小），可以缩减视图的大小。

```
string_view sv("123456789");
sv.remove_suffix(1);    // 现在sv中为：12345678, sv的大小为8
sv.remove_prefix(2);    // 现在sv中为: 345678, sv的大小为6
```

## 字符串查找

如同`std::string`一样，视图也提供了在字符串中进行查找的算法，并计划在C++ 20中提供`starts_with`和`ends_with`函数。这些函数的用法与`std::string`中的并无二致，此处不再赘述。

## 生命周期

由于`string_view`并不真正的持有字符串，所以若视图所引用的字符串区域已经被销毁，那么对应的，视图也会相应的失效。无法起到作用，下面的代码是一个例子，在这个例子中，将无法按照预期输出`Example`：

```cpp
std::string_view getView() {
    char ar[] = "Example";
    return { ar };
}

int main()
{
    std::cout << getView() << std::endl;
    return 0;
}
```

