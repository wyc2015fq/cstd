# c++ 嵌套定义类型>>’ should be ‘> >’ within a&n_用户5706034127_新浪博客
|||
# [法](http://www.cnblogs.com/moher/p/3251473.html)
如果直接这样写：
std::vectorint>> intvec;
gcc编译器会把">>"当成operator,报错：boost vector
 error: ‘>>’ should be ‘> >’ within a
nested template argument list
正确做法是加上空格：
std::vectorint> > intvec;
不过VS2010测试下不加空格也可以的。
map >
adict[SZ];
