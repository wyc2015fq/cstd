# C++杂记（一） - SHINING的博客 - CSDN博客
2008年11月24日 19:59:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1194标签：[c++																[string																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/snsn1984/article/category/488621)
 默认实参
         默认实参是一种虽然并不普遍，但在大多数情况下仍然适用的实参值。调用函数时，可以省略有默认值的实参。编译器会为我们省略的实参提供墨认值。
        例如：
         string screenInit(string::size_type  height=24,string::size_type width=80,char background=' ');
         调用包含默认实参的函数时，可以为该形参提供实参，也可以不提供。如果提供了实参，则它将覆盖默认的实参值；否则，函数将使用默认实参值。
       使用默认实参需要注意几点：
        1、如果有一个形参具有默认实参，那么，它后面所有的形参必须有默认实参。
        2、函数调用的实参按位置解析，默认实参只能用来替换函数调用缺少的尾数实参。
        3、在一个文件中，只能为一个形参指定默认实参一次。使用最少的默认实参的形参排在最前，最可能使用默认实参的形参排在最后。
静态局部变量
size_t count_calls()
{
static size_t ctr=0;
return ++ctr;
}
int  mian()
{
for(size_t i=0;i!=10;++i)
       cout<<count_calls()<<endl;
return 0;
}
这个程序会依次输出1到10（包含10）的整数。
内联函数
inline
