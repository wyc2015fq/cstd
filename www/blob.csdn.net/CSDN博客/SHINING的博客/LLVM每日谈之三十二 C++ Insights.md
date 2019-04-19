# LLVM每日谈之三十二 C++ Insights - SHINING的博客 - CSDN博客
2018年07月02日 13:56:20[snsn1984](https://me.csdn.net/snsn1984)阅读数：386
所属专栏：[LLVM每日谈](https://blog.csdn.net/column/details/llvm-study.html)
C++ Insights是一个基于clang的工具，它主要提供代码到代码的转换。它的目标是让屏幕背后发生一些事情变得可见，让我们更好的理解编译器是怎么运作的。
列举两个例子，大家就很明白的知道这个工具到底做了什么了。
官方例子一：
Source：
#include <cstdio>
#include <vector>
int main()
{
const char arr[10]{2,4,6,8};
for(const char& c : arr)
{
printf("c=%c\n", c);
}
}
----------------------------------
Insight:
#include <cstdio>
#include <vector>
int main()
{
const char arr[10]{2,4,6,8};
{
auto&& __range1 = arr;
const char * __begin1 = __range1;
const char * __end1 = __range1 + 10l;
for( ; __begin1 != __end1; ++__begin1 )
{
const char & c = *__begin1;
printf("c=%c\n", static_cast<int>(c));
}
}
}
官方例子二：
source:
class Base {
};
class Derived : public Base {
};
int main() {
Derived d;
Base& b = d;
}
-------------------------------------------
Insight:
class Base {
/* public: inline constexpr Base() noexcept; */
/* public: inline ~Base(); */
/* public: inline constexpr Base(const Base &); */
/* public: inline constexpr Base(Base &&); */
};
class Derived : public Base {
/* public: inline constexpr Derived() noexcept; */
/* public: inline constexpr Derived(const Derived &); */
/* public: inline constexpr Derived(Derived &&); */
};
int main() {
Derived d;
Base& b = static_cast<Base&>(d);
}
这个工具目前还在不断完善，虽然功能上可能有不尽人意的地方。但是，代码本身可以学到很多关于code transform的知识。
C++ Insights源码地址：[andreasfertig/cppinsights](https://github.com/andreasfertig/cppinsights)
C++ Insights在线demo地址：[C++ Insights](https://cppinsights.io/)
2018年5月16日
