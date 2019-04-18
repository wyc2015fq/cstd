# Effective C++ 读书笔记(36) - nosmatch的专栏 - CSDN博客
2013年07月21日 17:44:00[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：669
条款36：绝不重新定义继承而来的non-virtual函数
> 
```cpp
class B{
public:
	void func();
	...
};
class D:public B{
public:
	...
};
```
在上面的简单例子中，如果有下面的调用：
```cpp
D x;
B* p = new D();
D* q = new D();
p->func();
q->func();
```
此时上面的两个调用调用的都是B类的func函数，此时如果在D类中也重新定义函数void func(),此时如果还是上面那样的调
用第一个将调用的是B类的func，第二个调用的是D类的func，这是因为对普通的函数我们是采用的静态链接，因此对于p虽然指向
的是D类的一个对象，但是因为在静态连接阶段不能够知道其具体指向的对象，因此指向的还是B类的func，在前面的条款中我们说
过对于public继承的体系中，父类的成员函数是完全适用于子类的，但是如果重载了函数的实现，那么这个条款将不适合，因此在
public继承体系中，子类可以重新定义父类的virtual的接口函数，而非实现函数！
请记住：
- 绝对不要重新定义继承而来的non-virtual函数

