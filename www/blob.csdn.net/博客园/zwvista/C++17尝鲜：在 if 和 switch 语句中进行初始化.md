# C++17尝鲜：在 if 和 switch 语句中进行初始化 - zwvista - 博客园

## [C++17尝鲜：在 if 和 switch 语句中进行初始化](https://www.cnblogs.com/zwvista/p/7675450.html)

### 初始化语句

在C++17中，类似于 for 语句，在 if 和 switch 语句的判断条件之前也能加上初始化语句，语法形式如下：

```
if (初始化语句; 条件) 语句 else 语句
switch (初始化语句; 条件) 语句
```

这里的初始化语句是可选的，它可以是初始化表达式，也可以是变量声明。

```
// if (初始化语句; 条件) 语句 else 语句
// 相当于：
{
    初始化语句;
    if (条件) 语句 else 语句
}
// switch (初始化语句; 条件) 语句
// 相当于：
{
    初始化语句;
    switch (条件) 语句
}
```

### 示例

```
#include <iostream>
#include <set>
using namespace std;

int main()
{
    set<string> myset;
    if (auto [iter, success] = myset.insert("Hello"); success) 
       cout << *iter << endl; // Hello
}
```

示例代码在 if 语句中使用结构化绑定声明了两个变量 iter 以及 success，然后把 success 作为条件进行判断。

### 变量声明

这是一个相对简单的新特性，需要注意的是如果 if 和 switch 语句中的初始化语句是一个变量声明，那么所声明的变量的作用域仅限于该语句以及其附属语句的范围。

```
#include <iostream>
using namespace std;

int f() {return 1;}
int g() {return 2;}
int main()
{
    if (int a = f(); a != 1) {
        // 代码块A
        cout << a << endl;
    } else if (int b = g(); b != 2) {
        // 代码块B
        a += b;
        cout << a << endl;
    } else {
        // 代码块C
        a -= b;
        cout << a << endl;
    }

    switch(int c = f(); c) {
    case 1:
    case 2:
        cout << c << endl;
        break;
    default:
        cout << c + 1 << endl;
        break;
    }
}

// -1
// 1
```

变量 a 的作用域为它所声明的 if 语句及其附属的 else 语句，包括后者所附属的 if 语句，所以它的作用域涵盖代码块 A, B, 以及C。

变量 b 的作用域为它所声明的 if 语句及其附属的 else 语句，所以它的作用域涵盖代码块 B 和 C。

变量 c 的作用域为它所声明的 switch 语句。


