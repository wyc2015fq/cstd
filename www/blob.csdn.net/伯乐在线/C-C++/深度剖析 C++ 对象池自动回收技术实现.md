# 深度剖析 C++ 对象池自动回收技术实现 - 文章 - 伯乐在线
原文出处： [CSDN - 祁宇](http://www.csdn.net/article/2015-11-27/2826344-C++)
对象池可以显著提高性能，如果一个对象的创建非常耗时或非常昂贵，频繁去创建的话会非常低效。对象池通过对象复用的方式来避免重复创建对象，它会事先创建一定数量的对象放到池中，当用户需要创建对象的时候，直接从对象池中获取即可，用完对象之后再放回到对象池中，以便复用。这种方式避免了重复创建耗时或耗资源的大对象，大幅提高了程序性能。本文将探讨对象池的技术特性以及源码实现。
![](http://jbcdn2.b0.upaiyun.com/2015/11/c4c80c17ab3a65a4a2bdb686f8949972.png)
对象池类图
- ObjectPool：管理对象实例的pool。
- Client：使用者。
**适用性：**
- 类的实例可重用。
- 类的实例化过程开销较大。
- 类的实例化的频率较高。
**效果：**
- 节省了创建类实例的开销。
- 节省了创建类实例的时间。
- 存储空间随着对象的增多而增大。
## 问题
目前纵观主流语言的实现方式无外乎3个步骤：
- 初始创建一定数量的对象池（也允许从外面添加对象）。
- 从对象池中取对象来使用。
- 用完之后返回对象池。
一般情况下这样是OK的，可能存在的问题是在第三步，有两个问题：
- 不方便，每次都需要显式回收对象。
- 忘记将对象放回对象池，造成资源浪费。
## 改进动机
解决显式回收的问题，实现自动回收，省心省力。改进之后的对象池无须提供release方法，对象会自动回收，改进之后的类图如下。
![](http://jbcdn2.b0.upaiyun.com/2015/11/9d1f2d7af8daa814fb02faebe3e147ab.jpg)
## 技术内幕
借助c++11智能指针，因为智能指针可以自定义删除器，在智能指针释放的时候会调用删除器，在删除器中我们将用完的对象重新放回对象池。思路比较简单，但实现的时候需要考虑两个问题：
- 什么时候定义删除器？
- 用shared_ptr还是unique_ptr？ 
### 1. 什么时候定义删除器
自定义删除器只做一件事，就是将对象重新放入对象池。如果对象池初始化的时候就自定义删除器的话，删除器中的逻辑是将对象放回对象池，放回的时候无法再定义一个这样的删除器，所以这种做法行不通。需要注意，回收的对象只能是默认删除器的。除了前述原因之外，另外一个原因是对象池释放的时候需要释放所有的智能指针，释放的时候如果存在自定义删除器将会导致对象无法删除。只有在get的时候定义删除器才行，但是初始创建或加入的智能指针是默认删除器，所以我们需要把智能指针的默认删除器改为自定义删除器。
### 1.2 用shared_ptr还是unique_ptr
因为我们需要把智能指针的默认删除器改为自定义删除器，用shared_ptr会很不方便，因为你无法直接将shared_ptr的删除器修改为自定义删除器，虽然你可以通过重新创建一个新对象，把原对象拷贝过来的做法来实现，但是这样做效率比较低。而unique_ptr由于是独占语义，提供了一种简便的方法方法可以实现修改删除器，所以用unique_ptr是最适合的。
### 1.3 实现源码

C++
```
#pragma once
#include <memory>
#include <vector>
#include <functional>
template <class T>
class SimpleObjectPool
{
public:
    using DeleterType = std::function<void(T*)>;
    void add(std::unique_ptr<T> t)
    {
        pool_.push_back(std::move(t));
    }
    std::unique_ptr<T, DeleterType> get()
    {
        if (pool_.empty())
        {
            throw std::logic_error("no more object");
        }
        //every time add custom deleter for default unique_ptr
        std::unique_ptr<T, DeleterType> ptr(pool_.back().release(), [this](T* t)
        {
            pool_.push_back(std::unique_ptr<T>(t));
        });
        pool_.pop_back();
        return std::move(ptr);
    }
    bool empty() const
    {
        return pool_.empty();
    }
    size_t size() const
    {
        return pool_.size();
    }
private:
    std::vector<std::unique_ptr<T>> pool_;
};
//test code
void test_object_pool()
{
    SimpleObjectPool<A> p;
    p.add(std::unique_ptr<A>(new A()));
    p.add(std::unique_ptr<A>(new A()));
    {
        auto t = p.get();
        p.get();
    }
    {
        p.get();
        p.get();
    }
    std::cout << p.size() << std::endl;
｝
```
如果你坚持用shared_ptr，那么回收的时候你需要这样写：

C++
```
std::shared_ptr<T> get()
{
	if (pool_.empty())
	{
		throw std::logic_error("no more object");
	}
	std::shared_ptr<T> ptr = pool_.back();
	auto p = std::shared_ptr<T>(new T(std::move(*ptr.get())), [this](T* t) 
	{
		pool_.push_back(std::shared_ptr<T>(t));
	});
	//std::unique_ptr<T, DeleterType> ptr(pool_.back().release(), [this](T* t)
	//{
	//	pool_.push_back(std::unique_ptr<T>(t));
	//});
	pool_.pop_back();
	return p;
}
```
这种方式需要每次都创建一个新对象，并且拷贝原来的对象，是一种比较低效的做法。代码仅仅是为了展示如何实现自动回收对象，没有考虑线程安全、对象池扩容策略等细节，源码链接：[object_pool](https://github.com/qicosmos/cosmos/tree/master/object_pool)
总结凡是需要自动回收的场景下都可以使用这种方式：在获取对象的时候将默认删除器改为自定义删除器，确保它可以回收。注意，回收的智能指针使用的是默认删除器，可以确保对象池释放时能正常释放对象。同时也将获取对象和释放对象时，对象的控制权完全分离。其他的一些应用场景：多例模式，无需手动释放，自动回收。
