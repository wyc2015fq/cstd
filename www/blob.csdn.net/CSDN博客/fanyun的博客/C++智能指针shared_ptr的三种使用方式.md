# C++智能指针shared_ptr的三种使用方式 - fanyun的博客 - CSDN博客
2017年03月31日 12:39:29[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：6128
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
智能指针通常是指基于引用计数的智能指针，在c++11标准中就是std::shared_ptr。它的特点是如果可以安全的放入STL容器中。
有以下三种方式可以使用shared_ptr
1. 如果你的c++编程环境支持c++标准，那么可以直接使用std::shared_ptr，它定义在memory文件中。
2. 如果你的c++实现支持C++TR1库扩展，那么可以使用std::tr1::shared_ptr。在Vc++编译器下是在<memory>定义，如果是linux下g++，那么定义在<tr1/memory>文件中。Boost也提供了TR1实现。
3. 否则的话，你可以使用Boost中的boost::shared_ptr，它定义在<boost/shared_ptr.hpp>文件中。
**(1)删除共享对象**
使用[shared_ptr](http://baike.so.com/doc/2032266-2150330.html)解决的主要问题是知道删除一个被多个客户共享的资源的正确时机。下面是一个简单易懂的例子，有两个类A和 B, 它们共享一个int实例。使用boost::shared_ptr,你需要必须包含"boost/shared_ptr.hpp".
```cpp
#include"boost/shared_ptr.hpp"
#include<cassert>
classA {
boost::shared_ptr<int>no_;
public:
A(boost::shared_ptr<int>no) : no_(no) {}
voidvalue(int i) {
*no_=i;
}
};
classB {
boost::shared_ptr<int>no_;
public:
B(boost::shared_ptr<int>no) : no_(no) {}
intvalue() const {
return*no_;
}
};
intmain() {
boost::shared_ptr<int>temp(new int(14));
Aa(temp);
Bb(temp);
a.value(28);
assert(b.value()==28);
}
```
类 A和 B都保存了一个shared_ptr<int>.在创建A和 B的实例时，shared_ptrtemp被传送到它们的[构造函数](http://baike.so.com/doc/4486173-4695354.html)。这意味着共有三个shared_ptr：a,b,和 temp，它们都引向同一个int实例。如果我们用[指针](http://baike.so.com/doc/1043844-1104112.html)来实现对一个的共享，A和 B必须能够在某个时间指出这个int要被删除。在这个例子中，直到main的结束，[引用计数](http://baike.so.com/doc/4553494-4764093.html)为3，当所有shared_ptr离开了[作用域](http://baike.so.com/doc/1716643-1814839.html)，计数将达到0，而最后一个[智能指针](http://baike.so.com/doc/711048-752684.html)将负责删除共享的 int.
**(2)标准容器**
把对象直接存入容器中有时会有些麻烦。以值的方式保存对象意味着使用者将获得容器中的元素的拷贝，对于那些复制是一种昂贵的操作的类型来说可能会有性能的问题。此外，有些容器，特别是std::vector,当你加入元素时可能会复制所有元素，这更加重了性能的问题。最后，传值的语义意味着没有多态的行为。如果你需要在容器中存放多态的对象而且你不想切割它们，你必须用[指针](http://baike.so.com/doc/1043844-1104112.html)。如果你用裸指针，维护元素的完整性会非常复杂。从容器中删除元素时，你必须知道容器的使用者是否还在引用那些要删除的元素，不用担心多个使用者使用同一个元素。这些问题都可以用[shared_ptr](http://baike.so.com/doc/2032266-2150330.html)来解决。
下面是如何把共享指针存入标准库容器的例子。
```cpp
#include"boost/shared_ptr.hpp"
#include<vector>
#include<iostream>
classA {
public:
virtualvoid sing()=0;
protected:
virtual~A() {};
};
classB : public A {
public:
virtualvoid sing() {
std::cout<< "Do re mi fa so la";
}
};
boost::shared_ptr<A>createA() {
boost::shared_ptr<A>p(new B());
returnp;
}
intmain() {
typedefstd::vector<boost::shared_ptr<A> > container_type;
typedefcontainer_type::iterator iterator;
container_typecontainer;
for(int i=0;i<10;++i) {
container.push_back(createA());
}
std::cout<< "The choir is gathered: \n";
iteratorend=container.end();
for(iterator it=container.begin();it!=end;++it) {
(*it)->sing();
}
```
}这里有两个类, A和 B,各有一个虚拟[成员函数](http://baike.so.com/doc/2008931-2125901.html)sing.
 B从 A公有继承而来，并且如你所见，工厂函数 createA返回一个动态分配的B的实例，包装在shared_ptr<A>里。在 main里,一个包含shared_ptr<A>的std::vector被放入10个元素，最后对每个元素调用sing。如果我们用裸指针作为元素，那些对象需要被手工删除。而在这个例子里，删除是自动的，因为在vector的生存期中，每个shared_ptr的[引用计数](http://baike.so.com/doc/4553494-4764093.html)都保持为1；当 vector被销毁，所有引用计数器都将变为零，所有对象都被删除。有趣的是，即使A的[析构函数](http://baike.so.com/doc/517932-548343.html)没有声明为virtual,shared_ptr也会正确调用B的析构函数！
上面的例子示范了一个强有力的技术，它涉及A里面的protected析构函数。因为函数 createA返回的是shared_ptr<A>,因此不可能对shared_ptr::[get](http://baike.so.com/doc/5369480-5605333.html)返回的指针调用 delete。这意味着如果为了向某个需要裸指针的函数传送裸指针而从shared_ptr中取出裸指针的话，它不会由于意外地被删除而导致灾难。那么，又是如何允许shared_ptr删除它的对象的呢？这是因为指针指向的真正类型是B;而B的析构函数不是protected的。这是非常有用的方法，用于给shared_ptr中的对象增加额外的安全性。
