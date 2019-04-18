# STL运用的C++技术（4）——重载函数 . - ljx0305的专栏 - CSDN博客
2013年01月30日 15:33:52[ljx0305](https://me.csdn.net/ljx0305)阅读数：503
STL是C++标准库的重要组成部分之一，它不仅是一个可复用的组件库，更是一个包含算法与数据结构的软件框架，同时也是C++泛型编程的很好例子。STL中运用了许多C++的高级技术。本文介绍重载函数。主要参考了《C++ Primer》和《STL源码剖析》。
       有人会问，STL大量运用了模板，哪里用过重载函数呢？首先介绍重载函数的概念。出现在相同作用域中的两个函数，如果具有相同的名字而形参表不同，则称为重载函数（overloaded
 function）。这是《C++ Primer》上对于重载函数的定义。在STL中，提到重载函数，必须先介绍一下迭代器。两者貌似扯不上什么关系，慢慢道来。我们知道STL的迭代器有五个特性，分别是value_type、pointer、 reference、 difference_type以及iterator_category。其中value_type已在前面两篇文章中介绍，表示迭代器所指的数据类型。本文来介绍一下iterator_category。
iterator_category表示迭代器的分类，共有五类。input_iterator、output_iterator、forward_iterator、bidirectional_iterator、random_access_iterator，分别是只读、只写、前向移动读写、双向移动读写、随机读写。任何一种迭代器，都属于其中的一种分类。比如单链表的迭代器属于forward_iterator，链表的迭代器属于
  bidirectional_iterator，而双端队列的迭代器属于random_access_iterator。下面给出了代码示例，整理了一下。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)
- template <class T, class Ref, class Ptr>  
- struct Slist_iterator //单链表
- {  
- typedef T               value_type;      //迭代器所指数据类型
- typedefptrdiff_t       difference_type; //两个迭代器间的距离
- typedef Ptr             pointer;         //迭代器所指的数据，不允许改变，即可以当右值
- typedef Ref             reference;       //迭代器所指的数据，允许改变，即可以当左值
- typedef forward_iterator_tag iterator_category; //前向移动
-     ...  
- };  
- template<class T, class Ref, class Ptr>  
- struct List_iterator //双向链表
- {  
- typedefptrdiff_t  difference_type;  
- typedef T value_type;  
- typedef Ptr pointer;  
- typedef Ref reference;   
- typedef bidirectional_iterator_tag iterator_category; //双向
-     ...  
- };  
- template <class T, class Ref, class Ptr>  
- struct Deque_iterator  //双端队列
- {   
- typedef T  value_type;  
- typedefptrdiff_t difference_type;    
- typedef Ptr pointer;  
- typedef Ref reference;  
- typedef random_access_iterator_tag iterator_category; //随机
-     ...  
- };  
```cpp
template <class T, class Ref, class Ptr>
struct Slist_iterator //单链表
{
    typedef T               value_type;      //迭代器所指数据类型
    typedef ptrdiff_t       difference_type; //两个迭代器间的距离
    typedef Ptr             pointer;         //迭代器所指的数据，不允许改变，即可以当右值
    typedef Ref             reference;       //迭代器所指的数据，允许改变，即可以当左值
    typedef forward_iterator_tag iterator_category; //前向移动
    ...
};
template<class T, class Ref, class Ptr>
struct List_iterator //双向链表
{
	typedef ptrdiff_t  difference_type;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference; 
	typedef bidirectional_iterator_tag iterator_category; //双向
	...
};
template <class T, class Ref, class Ptr>
struct Deque_iterator  //双端队列
{ 
    typedef T  value_type;
    typedef ptrdiff_t difference_type;  
    typedef Ptr pointer;
    typedef Ref reference;
    typedef random_access_iterator_tag iterator_category; //随机
	...
};
```
       说了这么多，与重载函数有什么关系呢？再给出一个函数就清楚了，这个函数就是advance函数，它用来移动迭代器。下面是该函数的定义，摘自HP的STL源码，做了修改。不难看出，外部的接口是advance函数，内部用了三个重载函数，这些函数的第三形参非常有意思，只是一个类型，没有变量名。这个形参仅仅s是用来激活重载，很神奇吧，强大的C++啊。
       这里进行函数的重载，应该是考虑到效率。对于双端队列，它的迭代器支持随机读写，那么调用第三个函数显然比调用第一个函数有效率。显然，这种效率的提升，与迭代器的移动能力息息相关。STL中的部分函数采用了这种策略，比如distance函数，为的是提高效率。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)
- template <class InputIterator, class Distance>  
- inlinevoid advance(InputIterator& i, Distance n)   
- {  
-     _advance(i, n, iterator_category(i));  
- }  
- template <class InputIterator, class Distance>  
- inlinevoid _advance(InputIterator& i, Distance n, input_iterator_tag)  //第一个函数
- {  
- while (n--) ++i;  
- }  
- template <class BidirectionalIterator, class Distance>  
- inlinevoid _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)  //第二个函数
- {  
- if (n >= 0)  
- while (n--) ++i;  
- else
- while (n++) --i;  
- }  
- template <class RandomAccessIterator, class Distance>  
- inlinevoid _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) //第三个函数
- {  
-     i += n;  
- }  
```cpp
template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) 
{
	_advance(i, n, iterator_category(i));
}
template <class InputIterator, class Distance>
inline void _advance(InputIterator& i, Distance n, input_iterator_tag)  //第一个函数
{
	while (n--) ++i;
}
template <class BidirectionalIterator, class Distance>
inline void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)  //第二个函数
{
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}
template <class RandomAccessIterator, class Distance>
inline void _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) //第三个函数
{
	i += n;
}
```
       到此，STL中重载函数的运用介绍的差不多了，不过有两个疑问。（1）_advance函数提供了三个重载版本，为什么没有定义inline void _advance (InputIterator& i, Distance n, forward_iterator_tag) 这个函数呢？（2）如何获取迭代器的类型呢？即上面的iterator_category(i)调用如何实现的。
       对于第一个疑问，我们知道单链表的迭代器类型是forward_iterator，_advance貌似没有可选的函数以供调用。其实这与STL中迭代器的分类有关。下面是迭代器分类的关系表示，通过为迭代器设计标签类实现。_advance函数正是通过这些标签类激活重载的。代码如下：
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)
- //迭代器各类间的关系 
- struct input_iterator_tag {};  
- struct output_iterator_tag {};  
- struct forward_iterator_tag : public input_iterator_tag {};  
- struct bidirectional_iterator_tag : public forward_iterator_tag {};  
- struct random_access_iterator_tag : public bidirectional_iterator_tag {};  
```cpp
//迭代器各类间的关系
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
```
       从上面定义的关系，我们发现forward_iterator_tag继承自Input_iterator_tag，因此对于forward_iterator类的迭代器，其标签为forward_iterator_tag，通过向上的转换，就可以调用标签为Input_iterator_tag的那个重载版本。可以写个程序来验证一下。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)
- #include <iostream> 
- usingnamespace std;  
- 
- //迭代器分类，首字符大写，与标准库区分一下 
- struct Input_iterator_tag {};  
- struct Output_iterator_tag {};  
- struct Forward_iterator_tag : public Input_iterator_tag {};  
- struct Bidirectional_iterator_tag : public Forward_iterator_tag {};  
- struct Random_access_iterator_tag : public Bidirectional_iterator_tag {};  
- 
- template <class I>  
- void foo(I& i, Input_iterator_tag)   
- {  
-     cout<<"Input_iterator_tag version"<<endl;  
- }  
- template <class I>  
- void foo(I& i, Bidirectional_iterator_tag)   
- {  
-     cout<<"Bidirectional_iterator_tag version"<<endl;  
- }  
- template <class I>  
- void foo(I& i, Random_access_iterator_tag)   
- {  
-     cout<<"Random_access_iterator_tag version"<<endl;  
- }  
- 
- int main()  
- {  
- int *x;  
-     foo(x, Input_iterator_tag());          //输出为Input_iterator_tag version
-     foo(x, Forward_iterator_tag());        //输出为Input_iterator_tag version
-     foo(x, Bidirectional_iterator_tag());  //输出为Bidirectional_iterator_tag version
-     foo(x, Random_access_iterator_tag());  //输出为Random_access_iterator_tag version
- return 0;  
- }  
```cpp
#include <iostream>
using namespace std;
//迭代器分类，首字符大写，与标准库区分一下
struct Input_iterator_tag {};
struct Output_iterator_tag {};
struct Forward_iterator_tag : public Input_iterator_tag {};
struct Bidirectional_iterator_tag : public Forward_iterator_tag {};
struct Random_access_iterator_tag : public Bidirectional_iterator_tag {};
template <class I>
void foo(I& i, Input_iterator_tag) 
{
	cout<<"Input_iterator_tag version"<<endl;
}
template <class I>
void foo(I& i, Bidirectional_iterator_tag) 
{
	cout<<"Bidirectional_iterator_tag version"<<endl;
}
template <class I>
void foo(I& i, Random_access_iterator_tag) 
{
	cout<<"Random_access_iterator_tag version"<<endl;
}
int main()
{
	int *x;
	foo(x, Input_iterator_tag());          //输出为Input_iterator_tag version
	foo(x, Forward_iterator_tag());        //输出为Input_iterator_tag version
	foo(x, Bidirectional_iterator_tag());  //输出为Bidirectional_iterator_tag version
	foo(x, Random_access_iterator_tag());  //输出为Random_access_iterator_tag version
	return 0;
}
```
        对于第二个疑问，好像遇到过，是在介绍模板特化中提到的。用了内嵌型别、模板特化的技术。STL还是运用这些技术解决这个问题的。扩展之前的萃取剂就可以了。下面给出了萃取剂新的定义，同时给出了测试用例，已在VS2008下测试通过。
       至于内嵌型别及模板特化的具体介绍，详见[STL运用的C++技术（2）——模板特化](http://blog.csdn.net/wuzhekai1985/article/details/6654667)
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6656659#)
- #include <vector> 
- #include <list> 
- #include <deque> 
- #include <iostream> 
- usingnamespace std;  
- 
- //萃取剂 
- template<class I>  
- struct Iterator_traits{  
- typedeftypename I::value_type value_type;  
- typedeftypename I::iterator_category iterator_category; //迭代器的类型
- };  
- //特化 原生指针 
- template<class T>  
- struct Iterator_traits<T*>{  
- typedef T value_type;  
- typedef random_access_iterator_tag iterator_category;  
- };  
- //特化 原生常指针 
- template<class T>  
- struct Iterator_traits<const T*>{  
- typedef T value_type;  
- typedef random_access_iterator_tag iterator_category;  
- };  
- 
- #define VALUE_TYPE(I) Iterator_traits<I>::value_type()
- #define ITERATOR_CATEGORY(I) Iterator_traits<I>::iterator_category() 
- 
- //自定义的advance函数，与STL差不多 
- template <class InputIterator, class Distance>  
- inlinevoid MyAdvance(InputIterator &i, Distance n)   
- {  
-     _MyAdvance(i, n, ITERATOR_CATEGORY(InputIterator)); //萃取迭代器的类型
- }  
- template <class InputIterator, class Distance>  
- inlinevoid _MyAdvance(InputIterator& i, Distance n, input_iterator_tag)   
- {  
- while (n--) ++i;  
-     cout<<"InputIterator"<<endl;  
- }  
- template <class BidirectionalIterator, class Distance>  
- inlinevoid _MyAdvance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)   
- {  
- if (n >= 0)  
- while (n--) ++i;  
- else
- while (n++) --i;  
-     cout<<"BidirectionalIterator"<<endl;  
- }  
- template <class RandomAccessIterator, class Distance>  
- inlinevoid _MyAdvance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)   
- {  
-     i += n;  
-     cout<<"RandomAccessIterator"<<endl;  
- }  
- //测试程序 
- int main()  
- {  
-     vector<int> v;  
-     v.push_back(1);  
-     v.push_back(2);  
-     list<int> l;  
-     l.push_back(1);  
-     l.push_back(2);  
-     deque<int> d;  
-     d.push_back(1);  
-     d.push_back(2);  
- 
-     vector<int>::iterator iter1 = v.begin();  
-     list<int>::iterator iter2 = l.begin();  
-     deque<int>::iterator iter3 = d.begin();  
-     MyAdvance(iter1, 1); //vector的迭代器是原生指针，因此是RandomAccessIterator
-     MyAdvance(iter2, 1); //链表的迭代器是双向的，因此是BidirectionalIterator
-     MyAdvance(iter3, 1); //双端队列支持随机读写，因此是RandomAccessIterator
- return 0;  
- }  
```cpp
#include <vector>
#include <list>
#include <deque>
#include <iostream>
using namespace std;
//萃取剂
template<class I>
struct Iterator_traits{
	typedef typename I::value_type value_type;
	typedef typename I::iterator_category iterator_category; //迭代器的类型
};
//特化 原生指针
template<class T>
struct Iterator_traits<T*>{
	typedef T value_type;
	typedef random_access_iterator_tag iterator_category;
};
//特化 原生常指针
template<class T>
struct Iterator_traits<const T*>{
	typedef T value_type;
	typedef random_access_iterator_tag iterator_category;
};
#define VALUE_TYPE(I) Iterator_traits<I>::value_type()
#define ITERATOR_CATEGORY(I) Iterator_traits<I>::iterator_category() 
//自定义的advance函数，与STL差不多
template <class InputIterator, class Distance>
inline void MyAdvance(InputIterator &i, Distance n) 
{
	_MyAdvance(i, n, ITERATOR_CATEGORY(InputIterator)); //萃取迭代器的类型
}
template <class InputIterator, class Distance>
inline void _MyAdvance(InputIterator& i, Distance n, input_iterator_tag) 
{
	while (n--) ++i;
	cout<<"InputIterator"<<endl;
}
template <class BidirectionalIterator, class Distance>
inline void _MyAdvance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) 
{
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
	cout<<"BidirectionalIterator"<<endl;
}
template <class RandomAccessIterator, class Distance>
inline void _MyAdvance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) 
{
	i += n;
	cout<<"RandomAccessIterator"<<endl;
}
//测试程序
int main()
{
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	list<int> l;
	l.push_back(1);
	l.push_back(2);
	deque<int> d;
	d.push_back(1);
	d.push_back(2);
	vector<int>::iterator iter1 = v.begin();
	list<int>::iterator iter2 = l.begin();
	deque<int>::iterator iter3 = d.begin();
	MyAdvance(iter1, 1); //vector的迭代器是原生指针，因此是RandomAccessIterator
	MyAdvance(iter2, 1); //链表的迭代器是双向的，因此是BidirectionalIterator
	MyAdvance(iter3, 1); //双端队列支持随机读写，因此是RandomAccessIterator
	return 0;
}
```
  
      本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
