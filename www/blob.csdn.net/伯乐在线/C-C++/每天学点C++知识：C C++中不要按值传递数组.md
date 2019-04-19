# 每天学点C++知识：C/C++中不要按值传递数组 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [LynnShaw](http://www.jobbole.com/members/shaoai37) 翻译，[至秦](http://www.jobbole.com/members/applevip) 校稿。未经许可，禁止转载！
英文出处：[Andrey Karpov](http://cpphints.com/hints/38)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
有个国外团队检测了 200 多个 C/C++ 开源项目，包括了 Php、Qt 和 Linux 内核等知名项目。于是他们每天分享一个错误案例，并给出相应建议。本篇案例来自 Wolf 源代码。
### 错误的代码：

C++
```
ID_INLINE mat3_t::mat3_t( float src[ 3 ][ 3 ] ) {
  memcpy( mat, src, sizeof( src ) );
}
```
### 说明：
有时候程序员会忘记 C/C++ 里数组不能按值传递给函数。当你试图这样做时，是数组的指针（第一个元素的地址）而不是整个数组被传递。我们还应该记住，方括号中的数字没有任何意义。它们仅仅是程序员所做的标志，记录了传递数组的『假定』大小。事实上，你也可以传递一个大小完全不同的数组。例如，下面的代码就会成功编译：

C++
```
void F(int p[10]) { }
void G()
{
  int p[3];
  F(p);
}
```
相应的，sizeof(src) 运算符表示的不是数组的大小，而是指针的大小。结果就是 memcpy() 仅复制了数组的一部分。也就是4或8字节，这取决于指针的大小（外部结构体不算）。
### 正确的代码：
这样的代码最简单的变形像这样：

C++
```
ID_INLINE mat3_t::mat3_t( float src[ 3 ][ 3 ] ) {
  memcpy(mat, src, sizeof(float) * 3 * 3);
}
```
## 建议：
有几种让你的代码更安全的方法。
**数组大小已知。**你可以在函数中使用数组的引用。但并不是每个人都知道可以这么做，知道如何编写这样的代码的人则更少。所以我希望这个例子是有趣并有用的:

C++
```
ID_INLINE mat3_t::mat3_t( float (&src)[3][3] )
{
  memcpy( mat, src, sizeof( src ) );
}
```
现在就可以给函数传递大小正确的数组了，而且最重要的是，sizeof() 得到了数组的实际大小。
解决这个问题的另一个方法是使用 [std::array](http://en.cppreference.com/w/cpp/container/array) 类。
**数组大小未知。**一些书的作者建议使用 [std::vector](http://en.cppreference.com/w/cpp/container/vector) 类，或者其他相似的类。然而实际中这样做并不总是很方便。
有时你想用一个简单的指针。在这种情况下，你应该向函数传递两个参数：一个指针和元素的数量。然而，总的来说这不是一个好做法，它会导致很多 bug。
这种情况下，可以去读一下《[C++核心指南](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md)》中的一些想法。我建议看《[不要用一个单独的指针传递数组](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md)》章节。总而言之在你有空时读读《C++核心指南》总是有好处的。里面有很多有用的想法。
