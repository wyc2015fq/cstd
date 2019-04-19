# 关于Singleton - 逍遥剑客 - CSDN博客
2011年03月19日 17:41:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2150标签：[null																[class																[dll																[thread																[多线程																[引擎](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
这东西可能每个引擎里都有吧, 不管它是宏还是模板.
用宏的人可能觉得模板编译慢, 另外不想多继承
用模板的人可以觉得宏丑陋, 不利于调试
不管哪种方式, 个人觉得这个东西总比全局变量好
这里说说几种常见的情况:
- 最简单的 
- 这是一种偷懒的做法, 访问时自动创建
- template <class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static T singleton;
		return &singleton;
	}
};
- 优点: 使用方便
- 缺点: 析构顺序无法控制, 特别是singleton之间有依赖关系时. 另外, 无法用于抽象类
- 考虑抽象类的 
- 手动创建和销毁, 所以也能用于抽象类
- template <class T>
class Singleton
{
private:
	static T *s_pSingleton;
public:
	Singleton()
	{
		assert(NULL == s_pSingleton);
		s_pSingleton = static_cast<T*>(this);
	}
	~Singleton()
	{
		assert(NULL != s_pSingleton);
		s_pSingleton = NULL;
	}
	static T* GetInstance()
	{ 
		return s_pSingleton;
	}
};
template <class T> T* Singleton<T>::s_pSingleton = NULL;
- 优点: 创建和销毁的顺序可以自己控制, 另外也可以用于抽象类
- 缺点: 需要按顺序写好创建和销毁的代码, 当然也可以用Stack让这个过程自动化
- 考虑线程安全的 
- 有时候我们想让Singleton只在一个模块内部使用, 而这个模块一般是处于自己的线程内部
- 线程局部的好处一方面是安全, 另一方面可以针对不同的线程创建不同的singleton, 如主线程和渲染线程使用不同的IO系统
- #define ThreadLocal __declspec(thread)
template <class T>
class Singleton
{
private:
	ThreadLocal static T *s_pSingleton;
public:
	Singleton()
	{
		assert(NULL == s_pSingleton);
		s_pSingleton = static_cast<T*>(this);
	}
	~Singleton()
	{
		assert(NULL != s_pSingleton);
		s_pSingleton = NULL;
	}
	static T* GetInstance()
	{ 
		return s_pSingleton;
	}
};
template <class T> T* Singleton<T>::s_pSingleton = NULL;
- 优点: 不用考虑多线程访问问题
- 缺点: 无法用于DLL导出, 见[TLS 的规则和限制](http://msdn.microsoft.com/zh-cn/library/2s9wt68x.aspx)
- 考虑线程安全+DLL导出的 
- MSDN上说了, 要在DLL的环境下使用thread local的变量, 需要使用TlsAlloc系列函数
- class ThreadLocalIndex
{
public:
	ThreadLocalIndex()
	{
		m_tlsIndex = TlsAlloc();
	}
	~ThreadLocalIndex()
	{
		TlsFree(m_tlsIndex);
	}
	operator DWORD() const
	{
		return m_tlsIndex;
	}
private:
	DWORD m_tlsIndex;
};
template <class T>
class Singleton
{
private:
	static DWORD GetThreadLocalIndex()
	{
		static ThreadLocalIndex index;
		return index;
	};
public:
	Singleton()
	{
		assert(NULL == GetInstance());
		TlsSetValue(GetThreadLocalIndex(), static_cast<T*>(this));
	}
	~Singleton()
	{
		assert(NULL != GetInstance());
		TlsSetValue(GetThreadLocalIndex(), NULL);
	}
	static T* GetInstance()
	{ 
		return (T*)TlsGetValue(GetThreadLocalIndex());
	}
};
- 嗯, 还没实际使用过, 有问题再说~
