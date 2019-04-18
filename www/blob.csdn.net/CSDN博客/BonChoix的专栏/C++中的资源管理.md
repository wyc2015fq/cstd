# C++中的资源管理 - BonChoix的专栏 - CSDN博客





2012年10月07日 09:02:16[Brother灬Nam](https://me.csdn.net/BonChoix)阅读数：3689








       C++中最重要， 也最令人头疼的事情之一就是资源管理。原则上，每次在堆区用关键字new申请一块内存，必须对应一个delete来释放它。对应一般的小程序，通过细心地写代码，很容易做到。但当程序变大、变复杂的时候，仅仅依靠谨慎的编写程序也是很难做到的。考虑以下几种情况：

1. 用new申请资源与用delete释放资源之间的跨度比较大，很容易在做完中间一系列的任务后忘记最初曾申请过资源，导致未释放它。

2. 一个资源被多个变量引用，这样每个引用结束后当然不能及时的释放，因为此时并不知道其他地方是否还在引用这个它。这样也容易导致在最后一个引用后依然没释放资源。

3. 对一个资源的引用存在多个分支，因此要保证每个分支结束的地方对资源进行释放。这样一方面代码变得不够简洁，更重要的是在以后的维护阶段，每当有人想添加一个新分支时，可能全然不知这里存在一个分配的资源，于是造成内存的泄漏。

4. 可能你真的做到了用delete来释放资源，但在new 和delete之间的程序一旦有异常抛出，将不再按正常的顺序往下执行，这样依然会导致内存的泄漏。

因此我们需要一些更加高效、可靠的办法，来对资源进行管理。

       比较流行的方法是用对象来管理资源。即一个对象中包含一个指针，对应我们申请的资源。但我们不直接操作指针，而是通过对象来间接的操作它。当对象超出作用域时，其析构函数通过调用delete来释放资源，这样我们将不用操心用完资源后释放的问题。这种方法最简单的模型如下：

```cpp
template<typename T>
class MyPtr
{
public:
	MyPtr(T *p = NULL):m_ptr(p){}
	~MyPtr()
	{
		if(m_ptr)
			delete m_ptr;
	}
private:
	T *m_ptr;
};
```

这样当我们要创建一个int型的指针时，可以这样：

```cpp
MyPtr<int> p(new int(6));
```

用完之后不需要进行delete，因为对象p析构时将会自动释放。

       创建对象时，先申请我们需要的资源，作为参数传递给对象的构造函数来初始化对象。这种技术在C++中叫“资源获得即初始化”（RAII，Resource Acquisition Is Initialization）。当然，为了更方便地操作指针，我们还需要定义一些成员来引用它，比如重载 *和->操作符。

```cpp
template<typename T>
class MyPtr
{
public:
	MyPtr(T *p = NULL):m_ptr(p){}
	~MyPtr()
	{
		if(m_ptr)
			delete m_ptr;
	}
	const T& operator * () const
	{
		return *m_ptr;
	}
	T& operator * ()
	{
		return *m_ptr;
	}
	const T* operator -> () const
	{
		return m_ptr;
	}
	T* operator -> ()
	{
		return m_ptr;
	}
private:
	T *m_ptr;
};
```



这种方法很容易解决了资源释放的问题，但是设想一下，当一个对象被复制了会怎么样？

此时多个对象同时指向了同一个资源，只要有一个对象被析构，该资源即同时被释放。这样其他对象将包含一个指向非法区域的指针！显然，这种情况是绝对不允许的。

针对这个问题，有几种解决办法：

       第一种即在对象被复制时，显式地将原对象中的指针设为NULL，把新对象的指针指向该资源。换句话说，该资源的控制权由原对象转移到新复制的对象身上，现在只有新对象可以操作该资源。有了这种硬性的规定，就不必担心再次引用被释放的资源的问题了。此时我们需要添加两个成员（复制构造函数和重载=操作符）来控制复制操作，如下：

```cpp
template<typename T>
class MyPtr
{
public:
	MyPtr(T *p = NULL):m_ptr(p){}
	MyPtr(MyPtr &mp):m_ptr(mp.m_ptr)
	{
		mp.m_ptr = NULL;
	}
	MyPtr& operator = (MyPtr &mp)
	{
		m_ptr = mp.m_ptr;
		mp.ptr = NULL;
	}
	~MyPtr()
	{
		if(m_ptr)
			delete m_ptr;
	}
	const T& operator * () const
	{
		return *m_ptr;
	}
	T& operator * ()
	{
		return *m_ptr;
	}
	const T* operator -> () const
	{
		return m_ptr;
	}
	T* operator -> ()
	{
		return m_ptr;
	}
private:
	T *m_ptr;
};
```

C++标准库中，与些相对应的是auto_ptr，用法与上面一致。

       另一种解决方法，即对资源的引用进行计数。每当对象被复制时，引用计数加1。一个对象被析构时，计数减1。当计数减为0时，再释放资源。这样就保证了对象复制后，不同的对象可以放心的使用资源，而不必担心资源被其他对象提前释放的问题了。这种技术被称为“智能指针”（Smart Pointer）。为了实现引用计数，我们可以添加一个辅助类，对资源进行封装并计数，如下：

```cpp
template<typename T> class SmartPtr;
template<typename T>
class Ptr
{
	friend class SmartPtr<T>;

	T *m_ptr;
	size_t m_count;
	
	Ptr(T *p = NULL):m_ptr(p),m_count(1){}
	~Ptr()
	{
		delete m_ptr;
	}
};
```



智能指针的实现如下：

```cpp
template<typename T>
class SmartPtr
{
public:
	SmartPtr(T *p = NULL):m_p(new Ptr<T>(p)){}
	SmartPtr(const SmartPtr& sp):m_p(sp.m_p)
	{
		++m_p->m_count;
	}

	SmartPtr& operator = (const SmartPtr& sp)
	{
		++sp.m_p->m_count;
		if(--m_p->m_count == 0)
		{
			delete m_p;
		}
		m_p = sp.m_p;

		return *this;
	}

	T* operator ->() {	return m_p->m_ptr;	}
	const T* operator ->() const {	return m_p->m_ptr;	}

	T operator *() {	return *m_p->m_ptr;	}

	~SmartPtr()
	{
		if(--m_p->m_count == 0)
			delete m_p;
	}

private:
	Ptr<T> *m_p;
};
```

对象的创建依然遵从RAII，初次创建的对象，引用为1。所有复制的对象共享一个辅助类对象的指针。复制构造函数中，资源引用加1。析构对象时，先判断引用是否变成0，是则释放资源，否则保留。比较特殊的是重载=操作符，先对外面对象资源的引用加1，再自减自身资源的引用并判断是否为0，是则释放自身资源，否则不释放。最后重新指向新的资源。这样也就同时实现了自我赋值的情况。

在boost库中，与此对应的是shared_ptr。


       有了引用计数的智能指针，我们就可以放心使用、复制了。更重要的是我们可以在容器中存在它们，因为容器的中元素都是复制的版本，像auto_ptr是不能放入容器的。


       总结：

       安全起见，我们应该尽可能地多使用RAII对象，而不是直接分配、操作资源，这样可以在很大程度上减少内存泄漏的发生。C++标准库中的auto_ptr和boost库的shared_ptr，分别是RAII的两种不同的类型。后者支持复制，适用于大多数情况。前者对象的复制本质上是资源控制权的转移，复制后原对象的资源指针为NULL，不能再对资源进行引用。应该根据不同的情况合理地选择这两种类型。





