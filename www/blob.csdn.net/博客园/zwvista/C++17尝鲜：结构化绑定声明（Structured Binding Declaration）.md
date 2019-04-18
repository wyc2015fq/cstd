# C++17尝鲜：结构化绑定声明（Structured Binding Declaration） - zwvista - 博客园

## [C++17尝鲜：结构化绑定声明（Structured Binding Declaration）](https://www.cnblogs.com/zwvista/p/6917116.html)

### 结构化绑定声明

结构化绑定声明，是指在一次声明中同时引入多个变量，同时绑定初始化表达式的各个子对象的语法形式。

结构化绑定声明使用auto来声明多个变量，所有变量都必须用中括号括起来。

```
cv-auto+引用 [变量1, 变量2, ... 变量n ] = 初始化表达式;
cv-auto+引用 [变量1, 变量2, ... 变量n ] (初始化表达式);
cv-auto+引用 [变量1, 变量2, ... 变量n ] {初始化表达式};
// 这里 cv-auto+引用 包含 auto, auto const, auto &, auto&& 等等形式
```

结构化绑定所声明的变量有两种形式：
- 非引用变量，此时初始化表达式对象需要拷贝一份，变量所绑定的是初始化表达式对象拷贝的各个子对象。
- 引用变量，此时初始化表达式对象不需要拷贝，变量所绑定的是初始化表达式对象本身的各个子对象。

结构化绑定中的初始化表达式有三种类型：
- 数组类型，此时变量所绑定的是数组的各个元素。
- `pair``tuple`等支持 `tuple_size<E>` 的类型，此时变量所绑定的是 `get<0>(e)，get<1>(e)，get<2>(e)`...

这里E是指类型，e是指对象。
- 带有 public 成员的结构类型，此时变量所绑定的是结构对象的各个 public 成员。

### C++17代码

```
#include <iostream>
#include <utility>
#include <set>
#include <map>
 
using namespace std;
 
struct S {
    int a, b, c;
};
 
map<string, int> get_map()
{
    return {
        { "hello", 1 },
        { "world", 2 },
        { "it's",  3 },
        { "me",    4 },
    };
}
 
int main()
{
    auto [a, b] = pair(2, "3"s);
    cout << a << b << endl; // 23
     
    set<string> myset;
    if (auto [iter, success] = myset.insert("Hello"); success) 
       cout << *iter << endl; // Hello
     
    int arr[] = {1, 2, 3, 4};
    const auto& [m1, m2, m3, m4] = arr;
    cout << m1 << m2 << m3 << m4 << endl; //1234
 
    S s = {4, 5, 6};
    auto& [x, y, z] = s;
    x = 0, y = 1, z = 2;
    cout << s.a << s.b << s.c << endl; // 012
 
    for (auto&& [k, v] : get_map())
        cout << "k=" << k << " v=" << v << endl;
    // k=hello v=1
    // k=it's v=3
    // k=me v=4
    // k=world v=2
}
```

### 代码说明
- 代码第24行采用结构化绑定声明了 int 类型的 a 和 string 类型的 b，分别绑定了初始化表达式中 pair 对象的 first 和 second。
- 代码第28行采用结构化绑定声明了迭代类型的 iter 和 bool 类型的 success，分别绑定了初始化表达式中 pair 对象的 first 和 second。
- 代码第32行采用结构化绑定声明了 const int& 类型的 m1 ~ m4，分别绑定了初始化表达式中 arr 数组的4个元素。
- 代码第36行采用结构化绑定声明了 int& 类型的 x y z，分别绑定了初始化表达式中 s 对象的3个数据成员 a b c。
- 代码第40行采用结构化绑定声明了 string&& 类型的 k 和 int 类型的 v，分别绑定了初始化表达式中 pair 对象的 first 和 second。

### C++14等价代码

```
#include <iostream>
#include <utility>
#include <set>
#include <map>

using namespace std;

struct S {
    int a, b, c;
};

map<string, int> get_map()
{
    return {
        { "hello", 1 },
        { "world", 2 },
        { "it's",  3 },
        { "me",    4 },
    };
}

int main()
{
    auto kv = make_pair(2, "3"s);
    auto& a = kv.first; auto& b = kv.second;
    cout << a << b << endl; // 23
    
    set<string> myset;
    set<string>::iterator iter;
    bool success;
    tie(iter, success) = myset.insert("Hello");
    if (success) 
       cout << *iter << endl; // Hello
    
    int arr[] = {1, 2, 3, 4};
    const auto &m1 = arr[0], &m2 = arr[1], &m3 = arr[2], &m4 = arr[3];
    cout << m1 << m2 << m3 << m4 << endl; //1234
    
    S s = {4, 5, 6};
    auto &x = s.a, &y = s.b, &z = s.c;
    x = 0, y = 1, z = 2;
    cout << s.a << s.b << s.c << endl; // 012

    for (auto&& kv : get_map()) {
        auto&& k = forward<decltype(kv.first)>(kv.first);
        auto&& v = forward<decltype(kv.second)>(kv.second);
        cout << "k=" << k << " v=" << v << endl;
    }
    // k=hello v=1
    // k=it's v=3
    // k=me v=4
    // k=world v=2
}
```


