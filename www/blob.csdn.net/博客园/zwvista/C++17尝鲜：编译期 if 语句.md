# C++17尝鲜：编译期 if 语句 - zwvista - 博客园

## [C++17尝鲜：编译期 if 语句](https://www.cnblogs.com/zwvista/p/9238273.html)

### Constexpr If（编译期 if 语句）

以 if constexpr 打头的 if 语句被称为 Constexpr If。

Constexpr If 是C++17所引入的新的语法特性。它为C++语言提供了在编译期处理条件分歧的功能。
|运行期 if 语句|预处理 if 语句|编译期 if 语句|
|----|----|----|
|```if (condition) {  statement;} else if (condition) {  statement;} else {  statement;}```|```#if condition  statement;#elif condition  statement;#else  statement;#endif```|```if constexpr (condition) {  statement;} else if constexpr (condition) {  statement;} else {  statement;}```|
- 与运行期 if 语句有所不同，编译期 if 语句中的判断条件必须是编译期常量。
- 与预处理期 if 语句有所不同，编译期 if 语句中被丢弃的分支仍然需要进行语法检查。
- 与运行期 if 语句一样，编译期 if 语句的两个分支中任何一个分支只包含一句语句时，该分支的大括号可以省略。

```
template <typename T>
auto get_value(T t) {
    if constexpr (std::is_pointer_v<T>)
        return *t; // deduces return type to int for T = int*
    else
        return t;  // deduces return type to int for T = int
}
```
- std::is_pointer_v 是一个编译期的函数，判断类型参数 T 是否指针类型，返回布尔值。
- std::is_pointer_v 的返回值是一个编译器常量，符合编译期 if 语句的要求。
- get_value 函数在编译期检查参数 t 的类型 T，如果 T 是指针类型，就返回 *t 否则返回 t 自身。
- 由于 get_value 函数可能返回两种不同的类型，因此返回值类型必须用 auto，让编译器自行推导。


