# Effective C++ 读书笔记(16) - nosmatch的专栏 - CSDN博客
2013年07月12日 08:24:37[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：465
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款16：当成对的使用new 和 delete时，要确保new 和 delete的格式是相同的
> 
这个条款比较简单，但是却很容易出错，首先我们看下当我们使用new和delete的时候编译器为我们做了什么，当我们调用new
操作符的时候，编译器会首先在内存中帮我们申请一块空间，然后调用对应对象的构造函数，想对应的当我们调用delete的时候，编译
器会首先在该空间调用对应的析构函数然后再对该空间资源进行释放，例如:
> 
```cpp
std::string* p = new string[10];
```
对于上面的资源申请，我们释放的时候要对应的采用
```cpp
delete [] p;
```
如果我们采用了delete p的形式，将会出现不确定的结果，我们是通过[]符号来告诉编译器要析构的是一个对象还是一个对象的列
表，对应的会调用一次析构函数或者多次析构函数，如果在单一对象上调用多次析构函数或者在多个对象上调用单次析构函数，后果
可想而知！
> 
在使用中我们需要注意的一种情况是typedef，例如：
```cpp
typedef std::string AddressLine[10];
std::string* p = new AddressLine；
```
此时我们在调用delete的时候一定要注意使用delelet[], 在C++中存在强大的容器类，如果应用恰当完全可以将C中引入的array
数组替代掉，例如上面我们完全可以采用vector<string>的形式，这样就不用担心资源的释放问题了！
请记住：
- 对于资源的申请和释放调用new和delelte一定要采用相同的形式，new 对应delete ， new[] 对应delete[]!


> 
