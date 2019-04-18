# Effective C++ 读书笔记(13) - nosmatch的专栏 - CSDN博客
2013年07月11日 08:53:58[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：550
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款13：以对象来管理资源
> 
在C++中我们经常会涉及到资源的申请与申请，一般都是由关键字new 和 delete来操作的，两者都是成对存在的，缺一不可，否
则会出现意想不到的问题，例如:
> 
```cpp
class Investment{.....};
Investment* pinv = createInvestment();
```
我们在使用完后要动态的释放掉pinv所指向的资源，例如在下面的函数中做了调用：
```
void f(){
    Investment* pinv = createInvestment();
    ...
    delete pinv;
}
```
正常情况下这将运行良好，但是如果在...中函数提前的返回了或者在跟特别的出现了异常程序异常的停止了，这是delete函数将
得不到执行，此时就会出现传说中的内存泄露问题。
> 
为了对这种情况进行处理，我们可以采用一个专门的类来最这里的资源进行处理，在这个类的析构函数中处理对资源的释放
工作，这样当对象离开作用空间的时候会自动的调用其析构函数，资源也会自动的得到释放，在STL中，有一个专门的类来处理这
中情况suto_ptr,也就是传说中的”智能指针“；我们可以这么用：
> 
```cpp
std::auto_ptr<Investment> pivn = createInvestment();
```
这样当函数f执行结束的时候，auto_ptr离开他的作用域，将会自动的调用auto_ptr的析构函数，此时pinv所指向的资源也就得到
了释放！
> 
需要注意的是：auto_ptr智能指针是独占的，也就是不能有同样的auto_ptr指向同样的资源，例如:
```cpp
std::auto_ptr<Investment>* pivn2;piv2 = pivn;
```
> 
此时pivn将会指向一个NULL的资源！ 
> 
相对应的tr1::shared_ptr就能解决这种问题，他是通过引用计数来对资源进行释放的，当多个shared_ptr指向同样的资源的时候
这个资源的引用计数也会随之增加，当资源的引用计数变为0的时候，资源会被析构，这样在一些通过资源的复制而实现的结构中
可以采用shared_ptr,例如vector等。
> 
不过shared_ptr对下面这两种情况无能为力：1.对于动态分配的array上是不能使用的，因为shared_ptr内部采用的是delete而不
delete[]; 2. 对于环状引用的情况也是不能处理的，此时应该采用weaked_ptr;
> 
总之如果你的程序中有delete相关操作的出现，那就说明你的程序有随时出现意想不到情况出现的可能！
请记住：
- 为防止内存泄露，请使用RAII，他在构造函数中获得资源，在析构函数中释放资源。
- 两种常用的RAII是指的：auto_ptr和shared_ptr，后者是最佳的选择，因为他能很好的处理copy操作，前者是独占的！
> 
