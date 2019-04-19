# C++ 线程安全的单例模式 - 文章 - 伯乐在线
原文出处： [张峻崇](http://www.cnblogs.com/ccdev/archive/2012/12/19/2825355.html)
废话不多说，常用的代码积淀下来。
一、懒汉模式：即第一次调用该类实例的时候才产生一个新的该类实例，并在以后仅返回此实例。
需要用锁，来保证其线程安全性：原因：多个线程可能进入判断是否已经存在实例的if语句，从而non thread safety。
使用double-check来保证thread safety。但是如果处理大量数据时，该锁才成为严重的性能瓶颈。
1、静态成员实例的懒汉模式：

C++
```
class Singleton
{
private:
	static Singleton* m_instance;
	Singleton(){}
public:
	static Singleton* getInstance();
};
Singleton* Singleton::getInstance()
{
	if(NULL == m_instance)
	{
		Lock();//借用其它类来实现，如boost
		if(NULL == m_instance)
		{
			m_instance = new Singleton;
		}
		UnLock();
	}
	return m_instance;
}
```
2、内部静态实例的懒汉模式
这里需要注意的是，C++0X以后，要求编译器保证内部静态变量的线程安全性，可以不加锁。但C++ 0X以前，仍需要加锁。

C++
```
class SingletonInside
{
private:
	SingletonInside(){}
public:
	static SingletonInside* getInstance()
	{
		Lock(); // not needed after C++0x
		static SingletonInside instance;
		UnLock(); // not needed after C++0x
		return instance; 
	}
};
```
二、饿汉模式：即无论是否调用该类的实例，在程序开始时就会产生一个该类的实例，并在以后仅返回此实例。
由静态初始化实例保证其线程安全性，WHY？因为静态实例初始化在程序开始时**进入主函数之前就由主线程以单线程方式完成了初始化**，不必担心多线程问题。
故在性能需求较高时，应使用这种模式，避免频繁的锁争夺。

C++
```
class SingletonStatic
{
private:
	static const SingletonStatic* m_instance;
	SingletonStatic(){}
public:
	static const SingletonStatic* getInstance()
	{
		return m_instance;
	}
};
//外部初始化 before invoke main
const SingletonStatic* SingletonStatic::m_instance = new SingletonStatic;
```
（完）
