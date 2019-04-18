# C++17尝鲜：string_view - zwvista - 博客园

## [C++17尝鲜：string_view](https://www.cnblogs.com/zwvista/p/9233396.html)

### string_view

string_view 是C++17所提供的用于处理只读字符串的轻量对象。这里后缀 view 的意思是只读的视图。
- 通过调用 string_view 构造器可将字符串转换为 string_view 对象。

string 可隐式转换为 string_view。
- string_view 是只读的轻量对象，它对所指向的字符串没有所有权。
- string_view通常用于函数参数类型，可用来取代 const char* 和 const string&。

string_view 代替 const string&，可以避免不必要的内存分配。
- string_view的成员函数即对外接口与 string 相类似，但只包含读取字符串内容的部分。

string_view::substr()的返回值类型是string_view，不产生新的字符串，不会进行内存分配。

string::substr()的返回值类型是string，产生新的字符串，会进行内存分配。
- string_view字面量的后缀是 sv。（string字面量的后缀是 s）

### 实例

```
#include <string>
#include <iostream>

using namespace std;

// void process(const char* sv)
// void process(const string& sv)
void process(string_view sv)
{
    cout << sv << endl;
    for (char ch : sv)
        cout << ch;
    cout << sv[2] << endl;
}

int main()
{
    string_view sv = "hello"sv;
    cout << sv << endl;
    string_view sv2 = "hello";
    cout << sv2 << endl;
    string_view sv3 = "hello"s;
    cout << sv3 << endl;
    string_view sv4("hello", 4);
    cout << sv4 << endl;
    process("hello");
    process("hello"s);
}

/*
hello
hello
hello
hell
hello
hellol
hello
hellol
*/
```


