# STL运用的C++技术（3）——模板实参推断 . - ljx0305的专栏 - CSDN博客
2013年01月30日 15:33:19[ljx0305](https://me.csdn.net/ljx0305)阅读数：595
STL是C++标准库的重要组成部分之一，它不仅是一个可复用的组件库，更是一个包含算法与数据结构的软件框架，同时也是C++泛型编程的很好例子。STL中运用了许多C++的高级技术。本文介绍模板实参推断的运用。主要参考了《C++ Primer》和《STL源码剖析》。
       STL利用模板模板实现的，比如它的算法都是一个个的函数模板。我们知道模板是一个公式或是蓝图，本身不是类或是函数，需进行实例化的过程。这个过程是在编译期完成的，编译器根据传递的实参，推断出形参的类型，从而实例化相应的函数。《C++ Primer》中的定义：从函数实参确定模板实参的类型和值的过程叫做模板实参推断（template argument deduction）。下面给出STL中的几个算法，名字上做了简单的修改，略去了部分代码。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)
- //较小值 
- template <class T>  
- inlineconst T& Min(const T& a, const T& b) {  
- return b < a ? b : a;  
- }  
- //较大值 
- template <class T>  
- inlineconst T& Max(const T& a, const T& b) {  
- return  a < b ? b : a;  
- }  
```cpp
//较小值
template <class T>
inline const T& Min(const T& a, const T& b) {
  return b < a ? b : a;
}
//较大值
template <class T>
inline const T& Max(const T& a, const T& b) {
  return  a < b ? b : a;
}
```
           以下使用时，编译器就会根据实参实例化Min的两个版本。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)
- int main()  
- {  
-     cout<<Min(1,2)<<endl;  
-     cout<<Min(1.2,3.2)<<endl;  
- return 0;  
- }  
```cpp
int main()
{
	cout<<Min(1,2)<<endl;
	cout<<Min(1.2,3.2)<<endl;
	return 0;
}
```
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)
- inlineconstint& Min(constint& a, constint& b) {  
- return b < a ? b : a;  
- }  
- inlineconstdouble& Min(constdouble& a, constdouble& b) {  
- return b < a ? b : a;  
- }  
```cpp
inline const int& Min(const int& a, const int& b) {
  return b < a ? b : a;
}
inline const double& Min(const double& a, const double& b) {
  return b < a ? b : a;
}
```
        上面介绍的比较简单，具体的内容可以详见《C++ Primer》，本文只不过借此做个铺垫。上一篇文章“STL运用的C++技术（2）”，在获取迭代器所指的数据类型时，没有使用模板实参推断机制，而是使用了内嵌型别技术和模板特化技术。原因是它无法推断返回值的类型。这里再次把这段代码贴上来。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)
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
         iter_swap 函数利用模板实参推断来获取迭代器所指的数据类型。但是如果希望该函数返回第一个迭代器的数据时，该怎么办呢？模板实参推断是无能为力了，可以利用内嵌型别，如下定义即可。注意必须加关键字typename，用于告诉编译器这是个型别，否则编译无法通过。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6655978#)
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
- //真正的交换函数   
- template <class Iter1, class Iter2, class T>    
- inlinevoid _iter_swap(Iter1 a, Iter2 b, T) {    
-   T tmp = *a;    
-   *a = *b;    
-   *b = tmp;    
- }    
- //交换两个迭代器所指的元素   
- template <class Iter1, class Iter2>    
- inlinetypename Iterator_traits<Iter1>::value_type //利用内嵌型别获取返回值的类型
- iter_swap(Iter1 a, Iter2 b) {    
-  _iter_swap(a, b, *a); //模板实参推断 
- return *a;     
- }    
- 
- int main()  
- {  
-     list<int> l;  
-     l.push_back(3);  
-     l.push_back(4);  
-     cout<<iter_swap(l.begin(),++l.begin())<<endl;  
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
//真正的交换函数  
template <class Iter1, class Iter2, class T>  
inline void _iter_swap(Iter1 a, Iter2 b, T) {  
  T tmp = *a;  
  *a = *b;  
  *b = tmp;  
}  
//交换两个迭代器所指的元素  
template <class Iter1, class Iter2>  
inline typename Iterator_traits<Iter1>::value_type //利用内嵌型别获取返回值的类型
iter_swap(Iter1 a, Iter2 b) {  
 _iter_swap(a, b, *a); //模板实参推断 
 return *a;   
}  
int main()
{
	list<int> l;
	l.push_back(3);
	l.push_back(4);
	cout<<iter_swap(l.begin(),++l.begin())<<endl;
	return 0;
}
```
本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
