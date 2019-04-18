# Effective C++ 读书笔记(14) - nosmatch的专栏 - CSDN博客
2013年07月11日 23:22:30[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：634
个人分类：[读书笔记](https://blog.csdn.net/HDUTigerkin/article/category/1432055)
条款14：在资源管理类中小心copying行为
> 
在条款中主要的介绍了智能指针的用法，那里解决为问题是指向一个heap空间的指针的申请与释放问题，然而并非所有
的资源都是在heap中申请的，这时候智能指针就不适合了，例如对于类型为Mutex的互斥器对象，只有lock和unlock的操作，在
这里lock与unlock是成对存在的，为了防止调用lock后忘记unlock我们可以自己管理资源，例如：
> 
```cpp
class Lock{
public:
    explicit Lock(Mutex* pm):mutexPtr(pm){
		lock(mutexPtr);
	}
	~Lock(){
		unlock(mutexPtr);
	}
private:
	Mutex* mutexPtr;
};
```
```cpp
Mutex m；
.....
{
    ......
    Lock m1(&m)
    ......
}
```
这样在m1离开作用域的时候，会自动的调用Lock的析构函数也就是Mutex的unlock函数解锁！
但是此时如果出现复制操作，Lock  m11(&m); Lock
m12(m11)此时会怎样来处理？这个主要有以下几种方法：
1.禁止复制，有时候有些对象是不适合被复制的，对于一个想Lock这样的对象就是这样的情况，我们可以采用前面介绍
的方法，
```cpp
class Lock:public Uncopyable{
public:
    ....
};
```
2.对底层资源采用"引用计数法"，tr1::shared_ptr就是这种情况，当资源的被复制的时候，资源的引用计数就会+1，对应
的如果引用计数为0，就释放该资源，shared_ptr缺省的情况下会在指针计数为0 的时候释放资源，在特殊情况下我们可
以制定指针为0的时候采用的动作，例如：
```cpp
class Lock{
public:
	explicit Lock(Mutex* pm):mutexPtr(pm,unlocak){
		lock(mutexPtr.get());
	}
private:
	tr1::shared_ptr<Mutex> mutexPtr;
};
```
在本例中不用再写析构函数，因为默认析构函数调用的时候，会自动的调用mutexPtr的析构函数，即为前面制定的unlock
函数。
3.复制底部资源。只要你喜欢你可以对一个申请的资源做任何多份的copy，此时copy的时候不仅要copy资源管理类，还
要对其包裹的任何的资源进行复制，这就是所谓的深度copy
4.移交底部资源的所有权。例如auto_ptr,每份资源只有一个资源管理对象，当copy的时候，被copy的资源将变为空。
请记住：
- 复制RAII对象必须一并复制它所管理的资源，所以资源的copying行为决定RAII的行为
- 常见的RAII类的copy行为主要有：禁止copying，采用引用计数法，还有上面介绍的两种也是常用的方法！
> 
