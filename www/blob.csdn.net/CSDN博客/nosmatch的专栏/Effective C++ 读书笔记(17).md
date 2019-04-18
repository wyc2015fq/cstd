# Effective C++ 读书笔记(17) - nosmatch的专栏 - CSDN博客
2013年07月12日 09:15:12[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：453
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款17：以独立的语句将new的资源放入到智能指针中
> 
考虑以下情况：
```cpp
int prirority();
void processWidget(std::shared_ptr<Widget> pw, int priority);
```
我们在对processWidget函数进行调用的时候，可以采用如下形式：
```cpp
processWidget(std::tr1::shared_ptr<widget> pw(new widget), priority());
```
注意我们不能直接将new widget作为实参传入std:shared_ptr<widget> pw形参中，在上面的函数调用中，看起来没有问题但是可
能回出现内存泄露的情况，因为在函数中，参数的调用顺序会因为编译器的不同而不同的，例如上面的循序可能是：
- new widget
- priority调用
- std::tr1::shared_ptr<widget> 初始化
这样可能出现的问题就是当new widget成功后，如果priority()函数调用失败，由于new widget未能放入到智能指针中，但是退出的
时候资源得不到释放，解决办法就是讲实参独立出来，例如：
> 
```cpp
std::tr1::shared_ptr<widget> pw(new widget);
int pri = priority();
processWidget(pw,pri);
```
请记住:
- 要以独立的语句将资源放入到资源管理器中，否则一旦抛出异常可能出现内存泄露！
> 
