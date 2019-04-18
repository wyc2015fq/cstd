# STL运用的C++技术（1）——成员模板 . - ljx0305的专栏 - CSDN博客
2013年01月30日 15:25:10[ljx0305](https://me.csdn.net/ljx0305)阅读数：487
                
 STL是C++标准库的重要组成部分之一，它不仅是一个可复用的组件库，更是一个包含算法与数据结构的软件框架，同时也是C++泛型编程的很好例子。STL中运用了许多C++的高级技术。本文介绍成员模板的运用。主要参考了《C++ Primer》和《STL源码剖析》。
**成员模板 member template**
         任意类（模板或非模板）可以拥有本身为类模板或函数模板的成员，这种成员称为成员函数模板。STL中为什么要运用这种技术呢？考虑 vector 容器的 assign 成员函数，它可以接受两个迭代器为容器赋值，而这两个迭代器可以是 list 的迭代器、deque的迭代器、甚至是两个原生指针。也就是说成员函数 assign 的形参是不确定的。解决的方法是使用模板形参来表示迭代器形参的类型。下面给出一段小程序，使用成员模板技术。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)
- #include <iostream> 
- #include <list> 
- usingnamespace std;  
- 
- //自定义 
- class MyAlloc{  
- };  
- 
- template <class T, class Alloc = MyAlloc>  
- class MyVector  
- {  
- public:  
- typedef T value_type;  
- typedef value_type* iterator;  
- //成员模板，接受各种迭代器 
- template <class I>  
- void assign(I first, I last)  
-     { cout<<"assign"<<endl; }  
- };  
- 
- int main()  
- {  
-     MyVector<int> v;  
- int a[] = {1, 2, 3};  
-     list<int> l(a, a + 3);  
-     v.assign(a, a + 3);           //原生指针 
-     v.assign(l.begin(), l.end()); //链表迭代器
- return 0;  
- }  
```cpp
#include <iostream>
#include <list>
using namespace std;
//自定义
class MyAlloc{
};
template <class T, class Alloc = MyAlloc>
class MyVector
{
public:
	typedef T value_type;
	typedef value_type* iterator;
	//成员模板，接受各种迭代器
	template <class I>
	void assign(I first, I last)
	{ cout<<"assign"<<endl; }
};
int main()
{
	MyVector<int> v;
	int a[] = {1, 2, 3};
	list<int> l(a, a + 3);
	v.assign(a, a + 3);           //原生指针
	v.assign(l.begin(), l.end()); //链表迭代器
	return 0;
}
```
         STL中另一个模板例子是接受两个迭代器的容器构造函数。下面给出了一个小程序。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)
- #include <iostream> 
- #include <list> 
- usingnamespace std;  
- 
- //自定义 
- class MyAlloc{  
- };  
- 
- template <class T, class Alloc = MyAlloc>  
- class MyVector  
- {  
- public:  
- typedef T value_type;  
- typedef value_type* iterator;  
- 
- template <class I>  
-     MyVector(I first, I last)  
-     { cout<<"MyVector"<<endl; }  
- };  
- 
- int main()  
- {  
- int a[] = {1, 2, 3};  
-     list<int> l(a, a + 3);  
-     MyVector<int> v(a, a + 3);             //原生指针
-     MyVector<int> v2(l.begin(), l.end());  //链表迭代器
- return 0;  
- }  
```cpp
#include <iostream>
#include <list>
using namespace std;
//自定义
class MyAlloc{
};
template <class T, class Alloc = MyAlloc>
class MyVector
{
public:
	typedef T value_type;
	typedef value_type* iterator;
	template <class I>
	MyVector(I first, I last)
	{ cout<<"MyVector"<<endl; }
};
int main()
{
	int a[] = {1, 2, 3};
	list<int> l(a, a + 3);
	MyVector<int> v(a, a + 3);             //原生指针
	MyVector<int> v2(l.begin(), l.end());  //链表迭代器
	return 0;
}
```
        上面程序中类的内部定义成员模板，如果要在外面定义成员模板，必须包含两个模板形参表，类模板形参和自己的模板形参。首先是类模板形参表，然后是自己的模板形参表。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654034#)
- #include <iostream> 
- #include <list> 
- usingnamespace std;  
- 
- //自定义 
- class MyAlloc{  
- };  
- 
- template <class T, class Alloc = MyAlloc>  
- class MyVector  
- {  
- public:  
- typedef T value_type;  
- typedef value_type* iterator;  
- template <class I>  
-     MyVector(I first, I last);  
- template <class I>  
- void assign(I first, I last);  
- };  
- template<class T, class Alloc> template <class I>  
- MyVector<T, Alloc>::MyVector(I first, I last)  
- {   
-     cout<<"MyVector"<<endl;   
- }  
- template<class T, class Alloc> template <class I>  
- void MyVector<T, Alloc>::assign(I first, I last)  
- {   
-     cout<<"assign"<<endl;   
- }  
```cpp
#include <iostream>
#include <list>
using namespace std;
//自定义
class MyAlloc{
};
template <class T, class Alloc = MyAlloc>
class MyVector
{
public:
	typedef T value_type;
	typedef value_type* iterator;
	template <class I>
	MyVector(I first, I last);
	template <class I>
	void assign(I first, I last);
};
template<class T, class Alloc> template <class I>
MyVector<T, Alloc>::MyVector(I first, I last)
{ 
	cout<<"MyVector"<<endl; 
}
template<class T, class Alloc> template <class I>
void MyVector<T, Alloc>::assign(I first, I last)
{ 
	cout<<"assign"<<endl; 
}
```
本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
