# STL运用的C++技术（2）——模板特化 . - ljx0305的专栏 - CSDN博客
2013年01月30日 15:32:50[ljx0305](https://me.csdn.net/ljx0305)阅读数：567
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
 STL是C++标准库的重要组成部分之一，它不仅是一个可复用的组件库，更是一个包含算法与数据结构的软件框架，同时也是C++泛型编程的很好例子。STL中运用了许多C++的高级技术。本文介绍模板特化技术的运用。主要参考了《C++ Primer》和《STL源码剖析》。
       STL中大量运用了模块，可以说模板是创建类或函数的公式。但是，我们并不总能写出对所有可能被实例化的类型都最合适的模板。举个函数模板特化的例子。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- template <typename T>  
- int Compare(const T &x, const T &y)  
- {  
- if(x < y)  
- return -1;  
- elseif(x > y)  
- return 1;  
- else
- return 0;  
- }  
```cpp
template <typename T>
int Compare(const T &x, const T &y)
{
	if(x < y)
		return -1;
	else if(x > y)
		return 1;
	else
		return 0;
}
```
         对于上面这个函数模板，如果用两个字符串指针来调用，那么比较的是指针值，也就是比较地址大小，而不是字符串的大小。因此为了能够将Compare函数用于字符串，就需要提供一个知道怎么比较C风格字符串的特殊定义。这就是模板特化。
        模板特化（template specialization）的定义为指定一个或多个模板形参的实际类型或实际值。例如可以为Compare模板函数定义一个特化版本。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- template <> //template关键字后面接空括号
- int Compare(constchar * const &x, constchar * const &y) //形参为指向常量的常指针的引用
- {  
- return strcmp(x, y);  
- }  
```cpp
template <> //template关键字后面接空括号
int Compare(const char * const &x, const char * const &y) //形参为指向常量的常指针的引用
{
	return strcmp(x, y);
}
```
   上文简单阐述了模板特化，现在介绍模板特化在STL中的运用，以迭代器中的运用为例。
        迭代器是STL的关键所在，它将原本分开的数据容器和算法很好的胶合在一起。比如下面这个STL中的函数（摘自源码），命名上做了修改，同时略去了一些代码，但是足以说明问题。这个函数通过迭代器交换容器的数据，迭代器是数据容器和算法的桥梁，算法通过数据容器的迭代器访问容器中的数据，而不需关心容器的具体构造。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- //真正的交换函数，内部调用 
- template <class Iter1, class Iter2, class T>  
- inlinevoid _iter_swap(Iter1 a, Iter2 b, T) {  
-   T tmp = *a;  
-   *a = *b;  
-   *b = tmp;  
- }  
- //交换两个迭代器所指的元素，外部接口 
- template <class Iter1, class Iter2>  
- inlinevoid iter_swap(Iter1 a, Iter2 b) {  
-  _iter_swap(a, b, VALUE_TYPE(Iter1)); //VALUE_TYPE返回迭代器的值类型
- }  
```cpp
//真正的交换函数，内部调用
template <class Iter1, class Iter2, class T>
inline void _iter_swap(Iter1 a, Iter2 b, T) {
  T tmp = *a;
  *a = *b;
  *b = tmp;
}
//交换两个迭代器所指的元素，外部接口
template <class Iter1, class Iter2>
inline void iter_swap(Iter1 a, Iter2 b) {
 _iter_swap(a, b, VALUE_TYPE(Iter1)); //VALUE_TYPE返回迭代器的值类型
}
```
  上面用到了一个VALUE_TYPE调用，注释说是返回迭代器的值类型，具体如何下文会有介绍。举这个例子，就是为了引出这个调用。本文讲的是模板特化，但是到这里好像已经跑题了，不知所云。铺垫差不多了，进入正题。
       问一个问题，iter_swap这个函数的形参是迭代器，我们需要在函数内部定义一个临时变量，变量的数据类型为迭代器所指的数据类型。那么我们如何知道迭代器所指的数据类型呢？有人说，可以利用模板实参推断机制，解决这个问题。代码如下所示：
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- //真正的交换函数 
- template <class Iter1, class Iter2, class T>  
- inlinevoid _iter_swap(Iter1 a, Iter2 b, T) {  
-   T tmp = *a;  
-   *a = *b;  
-   *b = tmp;  
- }  
- //交换两个迭代器所指的元素 
- template <class Iter1, class Iter2>  
- inlinevoid iter_swap(Iter1 a, Iter2 b) {  
-  _iter_swap(a, b, *a); //模板实参推断
- }  
```cpp
//真正的交换函数
template <class Iter1, class Iter2, class T>
inline void _iter_swap(Iter1 a, Iter2 b, T) {
  T tmp = *a;
  *a = *b;
  *b = tmp;
}
//交换两个迭代器所指的元素
template <class Iter1, class Iter2>
inline void iter_swap(Iter1 a, Iter2 b) {
 _iter_swap(a, b, *a); //模板实参推断
}
```
  但是如果要推导函数的返回类型，模板实参推断机制就失效了。模板实参推断机制的具体内容，将在本系列（3）中介绍。继续上面的问题，本文用了一个称之为VALUE_TYPE的调用来获取的，它就像是一个萃取剂，萃取出迭代器所指的数据类型。那么它是如何实现的呢？答案就是内嵌型别。在STL中，大多数容器要求定义迭代器的内嵌型别，下面是 list 中的定义，已化简。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- class MyAlloc{    
- };  
- 
- template<class T>  
- struct List_iterator{  
- typedef T value_type;  //list 迭代器的内嵌型别
-   ...  
- };  
- 
- template <class T, class Alloc = MyAlloc>  
- class list{  
- public:  
- typedef List_iterator<T>  iterator;  //list迭代器
-   ...  
- };  
```cpp
class MyAlloc{  
};
template<class T>
struct List_iterator{
  typedef T value_type;  //list 迭代器的内嵌型别
  ...
};
template <class T, class Alloc = MyAlloc>
class list{
public:
  typedef List_iterator<T>  iterator;  //list迭代器
  ...
};
```
通过下面这种方式就可以萃取出 list 迭代器所指的数据类型。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- template<class I>  
- struct Iterator_traits{ //萃取剂定义
- typedeftypename I::value_type value_type;  
- };  
- 
- Iterator_traits<list<int>::iterator>::value_type x = 1;  
```cpp
template<class I>
struct Iterator_traits{ //萃取剂定义
	typedef typename I::value_type value_type;
};
Iterator_traits<list<int>::iterator>::value_type x = 1;
```
这种方式只能萃取出定义了内嵌型别的迭代器，但是如果是原生指针呢，它是没有内嵌型别的？比如 vector 容器，它是用原生指针做迭代器的。定义如下：
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- class MyAlloc{    
- };  
- 
- template <class T, class Alloc = MyAlloc>  
- class vector :   
- {  
- public:  
- typedef T value_type;    //内嵌型别
- typedef value_type* pointer;  
- typedefconst value_type* const_pointer;  
- typedef value_type* iterator;  //vector 迭代器，是原生指针
- typedefconst value_type* const_iterator;  
- typedef value_type& reference;  
- typedefconst value_type& const_reference;  
-   ...  
- };  
```cpp
class MyAlloc{  
};
template <class T, class Alloc = MyAlloc>
class vector : 
{
public:
  typedef T value_type;    //内嵌型别
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* iterator;  //vector 迭代器，是原生指针
  typedef const value_type* const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  ...
};
```
模板特化终于登场了，下面加入了原生指针的支持，使用的正是模板特化技术，在泛化设计中加入了特化版本。该技术也是STL中的核心关键所在。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- template<class I>  
- struct Iterator_traits{  
- typedeftypename I::value_type value_type;  
- };  
- //特化 原生指针 
- template<class T>  
- struct Iterator_traits<T*>{  
- typedef T value_type;  
- };  
- //特化 原生常指针 
- template<class T>  
- struct Iterator_traits<const T*>{  
- typedef T value_type;  
- };  
```cpp
template<class I>
struct Iterator_traits{
	typedef typename I::value_type value_type;
};
//特化 原生指针
template<class T>
struct Iterator_traits<T*>{
	typedef T value_type;
};
//特化 原生常指针
template<class T>
struct Iterator_traits<const T*>{
	typedef T value_type;
};
```
下面给出了完整的代码，已在VS2008下测试通过。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6654667#)
- #include <iostream> 
- #include <vector> 
- #include <list> 
- usingnamespace std;  
- 
- //萃取剂 
- template<class I>  
- struct Iterator_traits{  
- typedeftypename I::value_type value_type;  
- };  
- //特化 原生指针 
- template<class T>  
- struct Iterator_traits<T*>{  
- typedef T value_type;  
- };  
- //特化 原生常指针 
- template<class T>  
- struct Iterator_traits<const T*>{  
- typedef T value_type;  
- };  
- 
- #define VALUE_TYPE(I) Iterator_traits<I>::value_type()
- 
- //交换两个迭代器所指的元素 
- template <class Iter1, class Iter2>  
- inlinevoid iter_swap(Iter1 a, Iter2 b) {  
-  _iter_swap(a, b, VALUE_TYPE(Iter1)); //VALUE_TYPE返回迭代器的值类型
- }  
- //真正的交换函数 
- template <class Iter1, class Iter2, class T>  
- inlinevoid _iter_swap(Iter1 a, Iter2 b, T) {  
-   T tmp = *a;  
-   *a = *b;  
-   *b = tmp;  
- }  
- //测试函数 
- int main()  
- {  
- int a = 1, b = 2;  
-     iter_swap(&a,&b);  
-     cout<<a<<' '<<b<<endl;  //2 1
- 
-     list<int> l;  
-     l.push_back(3);  
-     l.push_back(4);  
-     iter_swap(l.begin(),++l.begin());  
-     cout<<*(l.begin())<<' '<<*(++l.begin())<<endl; //4 3
- 
-     Iterator_traits<int *>::value_type w = 5;       //特化
-     Iterator_traits<constint*>::value_type  x = 6; //特化
-         Iterator_traits<vector<int>::iterator>::value_type y = 7; //vector 容器
-     Iterator_traits<list<int>::iterator>::value_type z = 8;   //list 容器
-     cout<<w<<' '<<x<<' '<<y<<' '<<z<<endl; //5 6 7 8
- return 0;  
- }  
```cpp
#include <iostream>
#include <vector>
#include <list>
using namespace std;
//萃取剂
template<class I>
struct Iterator_traits{
	typedef typename I::value_type value_type;
};
//特化 原生指针
template<class T>
struct Iterator_traits<T*>{
	typedef T value_type;
};
//特化 原生常指针
template<class T>
struct Iterator_traits<const T*>{
	typedef T value_type;
};
#define VALUE_TYPE(I) Iterator_traits<I>::value_type()
//交换两个迭代器所指的元素
template <class Iter1, class Iter2>
inline void iter_swap(Iter1 a, Iter2 b) {
 _iter_swap(a, b, VALUE_TYPE(Iter1)); //VALUE_TYPE返回迭代器的值类型
}
//真正的交换函数
template <class Iter1, class Iter2, class T>
inline void _iter_swap(Iter1 a, Iter2 b, T) {
  T tmp = *a;
  *a = *b;
  *b = tmp;
}
//测试函数
int main()
{
	int a = 1, b = 2;
	iter_swap(&a,&b);
	cout<<a<<' '<<b<<endl;  //2 1
	
	list<int> l;
	l.push_back(3);
	l.push_back(4);
	iter_swap(l.begin(),++l.begin());
	cout<<*(l.begin())<<' '<<*(++l.begin())<<endl; //4 3
	Iterator_traits<int *>::value_type w = 5;       //特化
	Iterator_traits<const int*>::value_type  x = 6; //特化
        Iterator_traits<vector<int>::iterator>::value_type y = 7; //vector 容器
	Iterator_traits<list<int>::iterator>::value_type z = 8;   //list 容器
	cout<<w<<' '<<x<<' '<<y<<' '<<z<<endl; //5 6 7 8
	return 0;
}
```
本文介绍模板特化的同时，其实也介绍了STL迭代器实现的另一关键技术——内嵌型别。下文将介绍模板实参推断机制。
  
    本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
