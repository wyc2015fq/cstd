# 为什么 "auto a = 1;" 在C语言中可以编译通过？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [aszxqw](http://www.jobbole.com/members/aszxqw) 翻译。未经许可，禁止转载！
英文出处：[stackoverflow](http://stackoverflow.com/questions/23406212/why-does-auto-a-1-compile-in-c)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
### 问题（来自[lee77](http://stackoverflow.com/users/2893702/lee77)）：

C
```
int main(void)
{
    auto a = 1;
    return 0;
}
```
把如上所示的代码写在扩展名为.c的文件里面，用MS Visual Studio 2012编译器编译的时候不会报错。我一直认为，当你使用.c的扩展名时，编译器会按照C语言的语法来进行编译，而不是C++。而且，据我所知，不显示定义类型的auto变量声明只有从C++11标准之后才被允许，在C++11中auto的含义是声明该变量的类型可以被初始化法式推导出来的。
这是不是意味着我的编译器不严格遵守C语言的标准，或者这段代码实际上在C语言中是正确的？
### 最佳答案（来自[larsmans](http://stackoverflow.com/users/166749/larsmans)）：
auto 是一个古老的C语言关键字，它的代表含义是 “局部作用域”。 `auto a` 等价于 `auto int a`，而且函数里的变量定义默认就是局部作用域的变量，所以在该例子中它也等价于`int a`.
这个关键字(auto)事实上是 C语言的前身——B语言 遗留下来的，B语言没有基本类型： 一切类型都是 int, pointer to int, array of int . (*) 这些类型的声明只能是 `auto` 或者 `extern`。C语言继承了 “任何类型是int” 作为默认法则，所以你能用如下方法声明整型：

C
```
auto a;
extern b;
static c;
```
ISO C标准 抛弃了这个法则，但是当然有一些编译器为了向后兼容而接受这个法则。如果你还是觉得陌生，你应该明白有一个类似的的法则在下面语句中也生效：

C
```
unsigned d; // actually unsigned int
```
上式的代码在现代的代码中是很常见的。
C++11 复用了这个关键字(auto)，既然它的原始含义很少被C++程序员使用，所以在C++11中被复用来作为类型推导。 这样的做法通常是安全的，因为”一切类型都是int”这个C语言的法则已经被C++98标准中被抛弃; 唯一可能不安全的用法是 `auto T a`, 不过也没人会这么做。()
(*) 在B语言中的字符串处理很有意思: 你会使用int数组和包装多个字符在int数组的每个成员里。B语言事实上就是不同语法的BCPL(BCPL的变种)。
