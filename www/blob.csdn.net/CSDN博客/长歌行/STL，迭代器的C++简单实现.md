# STL，迭代器的C++简单实现 - 长歌行 - CSDN博客





2013年03月12日 15:52:57[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2191








```cpp
// Iterator.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <memory>
using namespace std;
// 对于C++，STL中已经包含迭代器的实现了
// 就不再单独造车轮了


// 部分代码见【STL源码剖析】


// 来一个智能指针的示意
namespace TEST
{

template<class T>
class auto_ptr
{
public:
	// 显示构造函数
	explicit auto_ptr(T *p = NULL) : pointee(p){}
	template<class U>
	auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release()){}
	~auto_ptr() {delete pointee;}

	template<class U>
	auto_ptr<T>& operator=(auto_ptr<U>& rhs)
	{
		if (this != &rhs)
		{
			reset(rhs.release());
			return *this;
		}
	}

	T& operator*() const
	{
		return *pointee;
	}
	T* operator->() const
	{
		return pointee;
	}
	T* get() const
	{
		return pointee;
	}
protected:
	T* release()
	{
		T *p = new T;
		memcpy(p, pointee, sizeof(T));
		delete this;
		return p;
	}
	void reset(T *p)
	{
		if (pointee)
		{
			delete pointee;
			pointee = NULL;
		}
		pointee = p;
	}
private:
	T *pointee;
};

}




// 智能指针的示例
// 申请的内存不需要释放，没有内存泄露
void func()
{
	TEST::auto_ptr<string> ps(new string("Hello"));
	cout<< *ps <<endl;
	cout<< ps->size() <<endl;
}

namespace TEST
{

template<typename T>
class ListItem
{
public:
	ListItem(T vl)
		: _value(vl)
		, _next(NULL)
	{
		
	}
	T value() const {return _value;}
	ListItem *next() const {return _next;}
	void SetNext(ListItem<T> *p){_next = p;}

private:
	T			_value;
	ListItem	*_next;

};


// 迭代器是一种智能指针
template<typename T>
class List
{
public:
	List()
	{
		ListItem<T> *a = new ListItem<T>(0);	// 没有析构哦
		_front = a;
		_front->SetNext(NULL);
		_size = 0;
	}
	void			insert_end(T value);
	void			display(std::ostream &os = std::cout) const;
	ListItem<T> *	front(){return _front;}
	ListItem<T> *	end()
	{
		ListItem<T> *end = _front;
		while(1)
		{
			if (end->next())
			{
				end = end->next();
			}
			else
			{
				break;
			}
		} 
		return end;
	}
private:
	ListItem<T> *_front;
	long _size;
};

template<typename T>
void TEST::List<T>::display( std::ostream &os /*= std::cout*/ ) const
{
	ListItem<T> *end = NULL;
	end =  _front->next();
	ListItem<T> *before = _front;
	do 
	{
		os<<end->value()<<endl;
		end = end->next();
	} while (end);
}


// 因为只保留了链表头，所以插入操作很纠结，比较失败
template<typename T>
void TEST::List<T>::insert_end( T value )
{
	ListItem<T> *node = new ListItem<T>(value);

	ListItem<T> *end = this->end();
	//end = _front;
	//while(1)
	//{
	//	if (end->next())
	//	{
	//		end = end->next();
	//	}
	//	else
	//	{
	//		break;
	//	}
	//} 
	end->SetNext(node);
	_size ++;
}

template<typename Item>
struct ListIter
{
	Item *ptr;
	ListIter(Item *p = NULL)
		: ptr(p)
	{

	}

	Item& operator*() const {return *ptr;}
	Item* operator->() const {return ptr;}
	ListIter& operator++()
	{
		ptr = ptr->next();
		return *this;
	}

	bool operator== (const ListIter &i)
	{
		return ptr == i.ptr;
	}

	bool operator!= (const ListIter &i)
	{
		return ptr != i.ptr;
	}


};




}


int _tmain(int argc, _TCHAR* argv[])
{
	func();
	TEST::List<int>  testList;
	testList.insert_end(10);
	testList.insert_end(20);
	testList.insert_end(11);
	TEST::ListIter<TEST::ListItem<int>> iter(testList.front());
	
	// 由于链表头为空，所以最好由后往前迭代，第一个节点不能用
	for (; iter != testList.end(); )
	{
		++iter;
		std::cout<<iter->value()<<endl;
	}
	testList.display();
	return 0;
}

//输出结果：
//Hello
//5
//10
//20
//11
//10
//20
//11
//请按任意键继续. . .
```




