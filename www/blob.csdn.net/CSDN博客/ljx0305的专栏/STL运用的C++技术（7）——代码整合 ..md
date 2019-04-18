# STL运用的C++技术（7）——代码整合 . - ljx0305的专栏 - CSDN博客
2013年01月30日 15:45:06[ljx0305](https://me.csdn.net/ljx0305)阅读数：550
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
本文将6篇文章中出现的代码整合在一起，主要参考了HP的STL源码。通过这些代码，不仅可以看到这些C++技术在STL中的运用，同时也能大致了解STL的架构组织及实现方法。首先给出一个测试用例，所有代码都是自定义的，未用到STL。读者可以建立一个C++工程，把这些代码加进去，就可以运行起来。方便之处在于，读者可以修改这些代码，实现一些自己的功能，以加深对于STL的理解。
        STL真正实现中，其实也就是对下述代码的扩展，定义更多的容器、算法、函数对象、迭代器等，至于具体实现的方法，仅仅是数据结构、算法自身上的差异，整体的实现步骤或者说是架构都是一样的。比如如果要实现deque，就需要定义deque的迭代器，deque的数据结构。又比如要实现排序算法，它与容器没有关系，它的形参是迭代器。因此实现起来根本不用考虑具体的容器。当然有些容器的排序算法需要特别实现，比如List的排序算法就是单独实现的。再比如要实现新的函数对象，没有问题，只需继承最基本的两个类，然后实现自定义的功能。
        当然，本文没有介绍STL的另一个重要组成部分，那就是内存的分配机制。感兴趣的读者可以看一下《STL源码剖析》这本书，或者找份源码看一下。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)
- #include "List.h"        //自定义的链表
- #include "Algorithm.h"   //自定义的算法 
- #include "Functional.h"  //自定义的函数对象
- #include <iostream> 
- usingnamespace std;  
- 
- //测试程序 VS2008下测试通过 
- int main()  
- {  
-     List<int> l;                                   //自定义的链表，STL链表的部分功能
-     l.push_back(1);                                //链表功能测试
-     l.push_back(2);  
- 
- //测试 萃取迭代器的类型功能 
-     MyAdvance(l.begin(), 1);                                  //输出Random_access_iterator_tag
- //测试 萃取迭代器所指的数据类型 
-     Iterator_traits<int *>::value_type x = 5;                 //特化  
-     Iterator_traits<constint*>::value_type  y = 6;           //特化  
-     Iterator_traits<List<int>::iterator>::value_type z = 7;   //list 容器  
-     cout<<x<<' '<<y<<' '<<z<<endl;                            //输出5 6 7 
- 
- //测试算法 
-     cout<<"before Iter_swap : "<<*(l.begin())<<' '<<*(++l.begin())<<endl; //输出1 2 
-     Iter_swap(l.begin(),++l.begin());                                    //交换两个迭代器的元素         
-     cout<<"after Iter_swap : "<<*(l.begin())<<' '<<*(++l.begin())<<endl; //输出2 1 
- 
-     l.push_front(3);                                  
-     l.push_front(4);  
-     l.push_front(5);                            
-     l.push_front(6);  
- 
- //测试函数对象 
- //链表中的元素为 6 5 4 3 1 2 
- int count1 = Count_if(l.begin(), l.end(), bind2nd(less_equal<int>(), 10));       //求容器中小于等于10的元素个数  
- int count2 = Count_if(l.begin(), l.end(), not1(bind2nd(less_equal<int>(), 10))); //求容器中不小于等于10的元素个数，正好是上面结果的取反  
-     cout<<count1<<' '<<count2<<endl;                                                 //输出6 0 
- 
- int count3 = 0, count4 = 0;  
-     Count_if(l.begin(), l.end(), bind2nd(greater_equal<int>(), 3), count3);          //求容器中大于等于3的元素个数     
-     Count_if(l.begin(), l.end(), not1(bind2nd(greater_equal<int>(), 3)), count4);    //求容器中小于3的元素个数  
-     cout<<count3<<' '<<count4<<endl;                                                 //输出4 2 
- 
-     l.clear();  
- return 0;  
- }  
```cpp
#include "List.h"        //自定义的链表
#include "Algorithm.h"   //自定义的算法
#include "Functional.h"  //自定义的函数对象
#include <iostream>
using namespace std;
//测试程序 VS2008下测试通过
int main()
{
	List<int> l;                                   //自定义的链表，STL链表的部分功能
	l.push_back(1);                                //链表功能测试
	l.push_back(2);
	//测试 萃取迭代器的类型功能
	MyAdvance(l.begin(), 1);                                  //输出Random_access_iterator_tag
	//测试 萃取迭代器所指的数据类型
	Iterator_traits<int *>::value_type x = 5;                 //特化  
    Iterator_traits<const int*>::value_type  y = 6;           //特化  
    Iterator_traits<List<int>::iterator>::value_type z = 7;   //list 容器  
    cout<<x<<' '<<y<<' '<<z<<endl;                            //输出5 6 7 
	//测试算法
	cout<<"before Iter_swap : "<<*(l.begin())<<' '<<*(++l.begin())<<endl; //输出1 2 
	Iter_swap(l.begin(),++l.begin());                                    //交换两个迭代器的元素         
    cout<<"after Iter_swap : "<<*(l.begin())<<' '<<*(++l.begin())<<endl; //输出2 1 
	l.push_front(3);                                
	l.push_front(4);
	l.push_front(5);                          
	l.push_front(6);
	//测试函数对象
	//链表中的元素为 6 5 4 3 1 2
    int count1 = Count_if(l.begin(), l.end(), bind2nd(less_equal<int>(), 10));       //求容器中小于等于10的元素个数  
    int count2 = Count_if(l.begin(), l.end(), not1(bind2nd(less_equal<int>(), 10))); //求容器中不小于等于10的元素个数，正好是上面结果的取反  
    cout<<count1<<' '<<count2<<endl;                                                 //输出6 0 
	int count3 = 0, count4 = 0;
	Count_if(l.begin(), l.end(), bind2nd(greater_equal<int>(), 3), count3);          //求容器中大于等于3的元素个数     
    Count_if(l.begin(), l.end(), not1(bind2nd(greater_equal<int>(), 3)), count4);    //求容器中小于3的元素个数  
	cout<<count3<<' '<<count4<<endl;                                                 //输出4 2 
	l.clear();
	return 0;
}
```
      首先给出迭代器萃取剂的定义，代码如下。可以看到内嵌型别技术，模板特化技术，重载函数的精妙运用。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)
- //Iterator_traits.h 
- #pragma once  
- 
- //迭代器分类，大写已便于标准库区分 
- struct Input_iterator_tag {};    
- struct Output_iterator_tag {};    
- struct Forward_iterator_tag : public Input_iterator_tag {};    
- struct Bidirectional_iterator_tag : public Forward_iterator_tag {};    
- struct Random_access_iterator_tag : public Bidirectional_iterator_tag {};    
- 
- //萃取剂   
- template<class I>    
- struct Iterator_traits{    
- typedeftypename I::value_type value_type;    
- typedeftypename I::iterator_category iterator_category; //迭代器的类型  
- typedeftypename I::difference_type   difference_type;  
- typedeftypename I::pointer           pointer;  
- typedeftypename I::reference         reference;  
- };    
- //特化 原生指针   
- template<class T>    
- struct Iterator_traits<T*>{    
- typedef T value_type;    
- typedef Random_access_iterator_tag iterator_category;    
- typedefptrdiff_t                 difference_type;  
- typedef T*                        pointer;  
- typedef T&                        reference;  
- };    
- //特化 原生常指针   
- template<class T>    
- struct Iterator_traits<const T*>{    
- typedef T value_type;    
- typedef Random_access_iterator_tag iterator_category;    
- typedefptrdiff_t                 difference_type;  
- typedefconst T*                  pointer;  
- typedefconst T&                  reference;  
- };    
- 
- //萃取剂 
- #define VALUE_TYPE(I) Iterator_traits<I>::value_type()  
- #define ITERATOR_CATEGORY(I) Iterator_traits<I>::iterator_category() 
- #define DIFFERENCE_TYPE(I) Iterator_traits<I>::difference_type()   
- #define POINTER(I) Iterator_traits<I>::pointer()  
- #define REFERENCE(I) Iterator_traits<I>::reference()   
- 
- //自定义的advance函数，与STL差不多   
- template <class InputIterator, class Distance>    
- inlinevoid MyAdvance(InputIterator &i, Distance n)     
- {    
-     _MyAdvance(i, n, ITERATOR_CATEGORY(InputIterator)); //萃取迭代器的类型  
- }    
- template <class InputIterator, class Distance>    
- inlinevoid _MyAdvance(InputIterator& i, Distance n, Input_iterator_tag)     
- {    
- while (n--) ++i;    
-     cout<<"InputIterator"<<endl;    
- }    
- template <class BidirectionalIterator, class Distance>    
- inlinevoid _MyAdvance(BidirectionalIterator& i, Distance n, Bidirectional_iterator_tag)     
- {    
- if (n >= 0)    
- while (n--) ++i;    
- else
- while (n++) --i;    
-     cout<<"BidirectionalIterator"<<endl;    
- }    
- template <class RandomAccessIterator, class Distance>    
- inlinevoid _MyAdvance(RandomAccessIterator& i, Distance n, Random_access_iterator_tag)     
- {    
-     i += n;    
-     cout<<"RandomAccessIterator"<<endl;    
- }    
```cpp
//Iterator_traits.h
#pragma once 
//迭代器分类，大写已便于标准库区分
struct Input_iterator_tag {};  
struct Output_iterator_tag {};  
struct Forward_iterator_tag : public Input_iterator_tag {};  
struct Bidirectional_iterator_tag : public Forward_iterator_tag {};  
struct Random_access_iterator_tag : public Bidirectional_iterator_tag {};  
//萃取剂  
template<class I>  
struct Iterator_traits{  
    typedef typename I::value_type value_type;  
    typedef typename I::iterator_category iterator_category; //迭代器的类型  
	typedef typename I::difference_type   difference_type;
	typedef typename I::pointer           pointer;
	typedef typename I::reference         reference;
};  
//特化 原生指针  
template<class T>  
struct Iterator_traits<T*>{  
    typedef T value_type;  
    typedef Random_access_iterator_tag iterator_category;  
    typedef ptrdiff_t                 difference_type;
	typedef T*                        pointer;
	typedef T&                        reference;
};  
//特化 原生常指针  
template<class T>  
struct Iterator_traits<const T*>{  
    typedef T value_type;  
    typedef Random_access_iterator_tag iterator_category;  
	typedef ptrdiff_t                 difference_type;
	typedef const T*                  pointer;
	typedef const T&                  reference;
};  
//萃取剂
#define VALUE_TYPE(I) Iterator_traits<I>::value_type()  
#define ITERATOR_CATEGORY(I) Iterator_traits<I>::iterator_category() 
#define DIFFERENCE_TYPE(I) Iterator_traits<I>::difference_type()   
#define POINTER(I) Iterator_traits<I>::pointer()  
#define REFERENCE(I) Iterator_traits<I>::reference()   
//自定义的advance函数，与STL差不多  
template <class InputIterator, class Distance>  
inline void MyAdvance(InputIterator &i, Distance n)   
{  
    _MyAdvance(i, n, ITERATOR_CATEGORY(InputIterator)); //萃取迭代器的类型  
}  
template <class InputIterator, class Distance>  
inline void _MyAdvance(InputIterator& i, Distance n, Input_iterator_tag)   
{  
    while (n--) ++i;  
    cout<<"InputIterator"<<endl;  
}  
template <class BidirectionalIterator, class Distance>  
inline void _MyAdvance(BidirectionalIterator& i, Distance n, Bidirectional_iterator_tag)   
{  
    if (n >= 0)  
        while (n--) ++i;  
    else  
        while (n++) --i;  
    cout<<"BidirectionalIterator"<<endl;  
}  
template <class RandomAccessIterator, class Distance>  
inline void _MyAdvance(RandomAccessIterator& i, Distance n, Random_access_iterator_tag)   
{  
    i += n;  
    cout<<"RandomAccessIterator"<<endl;  
}
```
          再给出一个容器的定义，这里用的List，实现了STL的部分功能，同时还出了相应的迭代器的实现。可以看到大量的重载操作符的实现。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)
- //List_iterator.h 
- #pragma once 
- #include "Iterator_traits.h"
- 
- //结点定义，双向链表   
- template <class T>    
- struct List_node {    
-     List_node* next;    
-     List_node* prev;    
-     T data;    
- };    
- //链表的迭代器   
- template<class T, class Ref, class Ptr>    
- class List_iterator    
- {    
- public:    
-     List_node<T> *node;    
- void Incr() { node = node->next; }    
- void Decr() { node = node->prev; }    
- public:    
- typedef T value_type;    
- typedef Ptr pointer;    
- typedef Ref reference;    
- typedefsize_t                     size_type;    
- typedefptrdiff_t                  difference_type;    
- typedef Bidirectional_iterator_tag iterator_category;         //迭代器的类型是双向的
- 
- typedef List_iterator<T, T&, T*>             iterator;        //迭代器  
- typedef List_iterator<T, const T&, const T*> const_iterator;    
- typedef List_iterator<T, Ref, Ptr>           self;    
- 
-     List_iterator(List_node<T>* x): node(x) {}     //接受链表结点的构造函数，很管用  
-     List_iterator() {}    
-     reference operator*() const { return node->data; }                        //解引用重载  
-     pointer operator->() const { return &(operator*()); }                     //箭头重载  
-     self& operator++() { this->Incr(); return *this; }                        //前增重载  
-     self operator++(int) { self tmp = *this; this->Incr(); return tmp; }      //后增重载  
-     self& operator--() { this->Decr(); return *this; }                        //前减重载  
-     self operator--(int) { self tmp = *this; this->Decr(); return tmp; }      //后减重载  
- bool operator==(const List_iterator& x) const { return node == x.node; }  //相等重载  
- bool operator!=(const List_iterator& x) const { return node != x.node; }  //不相等重载  
- };    
```cpp
//List_iterator.h
#pragma once
#include "Iterator_traits.h"
//结点定义，双向链表  
template <class T>  
struct List_node {  
    List_node* next;  
    List_node* prev;  
    T data;  
};  
//链表的迭代器  
template<class T, class Ref, class Ptr>  
class List_iterator  
{  
public:  
    List_node<T> *node;  
    void Incr() { node = node->next; }  
    void Decr() { node = node->prev; }  
public:  
    typedef T value_type;  
    typedef Ptr pointer;  
    typedef Ref reference;  
    typedef size_t                     size_type;  
    typedef ptrdiff_t                  difference_type;  
    typedef Bidirectional_iterator_tag iterator_category;         //迭代器的类型是双向的
  
    typedef List_iterator<T, T&, T*>             iterator;        //迭代器  
    typedef List_iterator<T, const T&, const T*> const_iterator;  
    typedef List_iterator<T, Ref, Ptr>           self;  
      
    List_iterator(List_node<T>* x): node(x) {}     //接受链表结点的构造函数，很管用  
    List_iterator() {}  
    reference operator*() const { return node->data; }                        //解引用重载  
    pointer operator->() const { return &(operator*()); }                     //箭头重载  
    self& operator++() { this->Incr(); return *this; }                        //前增重载  
    self operator++(int) { self tmp = *this; this->Incr(); return tmp; }      //后增重载  
    self& operator--() { this->Decr(); return *this; }                        //前减重载  
    self operator--(int) { self tmp = *this; this->Decr(); return tmp; }      //后减重载  
    bool operator==(const List_iterator& x) const { return node == x.node; }  //相等重载  
    bool operator!=(const List_iterator& x) const { return node != x.node; }  //不相等重载  
};
```
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)
- //List.h 
- #pragma once  
- #include "List_iterator.h"
- 
- //资源分配器   
- class MyAlloc    
- {    
- };    
- //链表定义   
- template <class T, class Alloc = MyAlloc >    
- class List {    
- public:          
- typedef List_node<T> list_node; //结点类型  
- typedef list_node* list_type;   //结点指针  
- 
- typedef T value_type;    
- typedef value_type* pointer;    
- typedefconst value_type* const_pointer;    
- typedef value_type& reference;    
- typedefconst value_type& const_reference;    
- typedefsize_t size_type;    
- typedefptrdiff_t difference_type;    
- 
- typedef List_iterator<T, T&, T*>             iterator; //迭代器  
- typedef List_iterator<T, const T&, const T*> const_iterator;    
- public:    
-     List() { node = get_node(); node->next = node; node->prev = node; } //构造哨兵结点  
-     ~List() { clear(); }      
- //返回类型要求是iterator，而实际返回的是结点指针，为什么可以呢？关键在于List_iterator有一个接受结点指针的构造函数  
-     iterator begin()             { return node->next; }     
-     const_iterator begin() const { return node->next; }    
-     iterator end()               { return node; }    
-     const_iterator end() const   { return node; }    
- bool empty() const { return node->next == node; }    
-     reference front() { return *begin(); }    
-     const_reference front() const { return *begin(); }    
-     reference back() { return *(--end()); }    
-     const_reference back() const { return *(--end()); }    
- void push_front(const T& x) { insert(begin(), x); }    
- void push_back(const T& x) { insert(end(), x); }    
- void pop_front() { erase(begin()); }    
- void pop_back() {  iterator tmp = end(); erase(--tmp); }    
- //插入结点   
- void insert(iterator pos, const T &x) {                
-         list_type tmp = get_node();    
-         tmp->data = x;    
-         tmp->next = pos.node;     
-         tmp->prev = pos.node->prev;    
-         (pos.node->prev)->next = tmp;    
-         pos.node->prev = tmp;    
-     }    
- //删除结点   
-     iterator erase(iterator pos) {     
-         list_type next_node = pos.node->next;    
-         list_type prev_node = pos.node->prev;    
-         prev_node->next = next_node;    
-         next_node->prev = prev_node;    
-         put_node(pos.node);    
- return next_node;    
-     }    
- //清除所有结点   
- void clear() {    
-         list_type cur = node->next;    
- while(cur != node)    
-         {    
-             list_type tmp = cur;    
-             cur = cur->next;    
-             put_node(tmp);    
-         }    
-         node->next = node;    
-         node->prev = node;    
-     }    
- private:    
-     list_type node;    
-     list_type get_node() { returnnew list_node; }      //分配空间  
- void put_node(list_type p) { delete p; p = NULL; }  //释放空间  
- };   
```cpp
//List.h
#pragma once 
#include "List_iterator.h"
//资源分配器  
class MyAlloc  
{  
};  
//链表定义  
template <class T, class Alloc = MyAlloc >  
class List {  
public:        
    typedef List_node<T> list_node; //结点类型  
    typedef list_node* list_type;   //结点指针  
  
    typedef T value_type;  
    typedef value_type* pointer;  
    typedef const value_type* const_pointer;  
    typedef value_type& reference;  
    typedef const value_type& const_reference;  
    typedef size_t size_type;  
    typedef ptrdiff_t difference_type;  
  
    typedef List_iterator<T, T&, T*>             iterator; //迭代器  
    typedef List_iterator<T, const T&, const T*> const_iterator;  
public:  
    List() { node = get_node(); node->next = node; node->prev = node; } //构造哨兵结点  
    ~List() { clear(); }    
    //返回类型要求是iterator，而实际返回的是结点指针，为什么可以呢？关键在于List_iterator有一个接受结点指针的构造函数  
    iterator begin()             { return node->next; }   
    const_iterator begin() const { return node->next; }  
    iterator end()               { return node; }  
    const_iterator end() const   { return node; }  
    bool empty() const { return node->next == node; }  
    reference front() { return *begin(); }  
    const_reference front() const { return *begin(); }  
    reference back() { return *(--end()); }  
    const_reference back() const { return *(--end()); }  
    void push_front(const T& x) { insert(begin(), x); }  
    void push_back(const T& x) { insert(end(), x); }  
    void pop_front() { erase(begin()); }  
    void pop_back() {  iterator tmp = end(); erase(--tmp); }  
    //插入结点  
    void insert(iterator pos, const T &x) {              
        list_type tmp = get_node();  
        tmp->data = x;  
        tmp->next = pos.node;   
        tmp->prev = pos.node->prev;  
        (pos.node->prev)->next = tmp;  
        pos.node->prev = tmp;  
    }  
    //删除结点  
    iterator erase(iterator pos) {   
        list_type next_node = pos.node->next;  
        list_type prev_node = pos.node->prev;  
        prev_node->next = next_node;  
        next_node->prev = prev_node;  
        put_node(pos.node);  
        return next_node;  
    }  
    //清除所有结点  
    void clear() {  
        list_type cur = node->next;  
        while(cur != node)  
        {  
            list_type tmp = cur;  
            cur = cur->next;  
            put_node(tmp);  
        }  
        node->next = node;  
        node->prev = node;  
    }  
private:  
    list_type node;  
    list_type get_node() { return new list_node; }      //分配空间  
    void put_node(list_type p) { delete p; p = NULL; }  //释放空间  
};
```
            再来给出函数对象的定义，可以看到绑定器，取反器的具体实现。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)
- //Functional.h 
- #pragma once  
- 
- //用来定义一元操作的参数类别和返回值类别   
- template <class Arg, class Result>    
- struct unary_function {    
- typedef Arg argument_type;  //内嵌型别技术  
- typedef Result result_type;    
- };    
- //用来定义二元操作的参数类别和返回值类别   
- template <class Arg1, class Arg2, class Result>    
- struct binary_function {    
- typedef Arg1 first_argument_type;    
- typedef Arg2 second_argument_type;    
- typedef Result result_type;    
- };    
- 
- //一元操作，就两个   
- template <class T>    
- struct negate : public unary_function<T, T> {    
-     T operator()(const T& x) const { return -x; }    
- };    
- template <class T>    
- struct logical_not : public unary_function<T,bool> {    
- bool operator()(const T& x) const { return !x; }    
- };    
- //加减乘除取模   
- template <class T>    
- struct plus : public binary_function<T, T, T> {    
-     T operator()(const T& x, const T& y) const { return x + y; }    
- };    
- template <class T>    
- struct minus : public binary_function<T, T, T> {    
-     T operator()(const T& x, const T& y) const { return x - y; }    
- };    
- template <class T>    
- struct multiplies : public binary_function<T, T , T> {    
-     T operator()(const T& x, const T& y) const { return x * y; }    
- };    
- template <class T>    
- struct divides : public binary_function<T ,T, T> {    
-     T operator()(const T& x, const T& y) const { return x / y; }    
- };    
- template <class T>    
- struct modulus : public binary_function<T, T, T> {    
-     T operator()(const T& x, const T& y) const { return x % y; }    
- };    
- //关系运算   
- template <class T>    
- struct equal_to : public binary_function<T, T, bool> {    
- bool operator()(const T& x, const T& y) const { return x == y; }    
- };    
- template <class T>    
- struct not_equal_to : public binary_function<T, T,bool> {    
- bool operator()(const T& x, const T& y) const { return x != y; }    
- };    
- template <class T>    
- struct greater : public binary_function<T, T, bool> {    
- bool operator()(const T& x, const T& y) const { return x > y; }    
- };    
- template <class T>    
- struct less : public binary_function<T, T, bool> {    
- bool operator()(const T& x, const T& y) const { return x < y; }    
- };    
- template <class T>    
- struct greater_equal : public binary_function<T, T, bool> {    
- bool operator()(const T& x, const T& y) const { return x >= y; }    
- };    
- template <class T>    
- struct less_equal : public binary_function<T, T, bool> {    
- bool operator()(const T& x, const T& y) const { return x <= y; }    
- };    
- //逻辑运算   
- template <class T>    
- struct logical_and : public binary_function<T, T, bool>{    
- bool operator()(const T& x, const T& y) const { return x && y; }    
- };    
- template <class T>    
- struct logical_or : public binary_function<T, T, bool>    
- {    
- bool operator()(const T& x, const T& y) const { return x || y; }    
- };    
- 
- //绑定第一个参数   
- template <class Operation>     
- class binder1st: public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {    
- public:    
-     binder1st(const Operation& x, consttypename Operation::first_argument_type& y) : op(x), value(y) {} //构造函数  
- typename Operation::result_type operator()(consttypename Operation::second_argument_type& x) const {    
- return op(value, x);  //固定第一个参数  
-     }    
- protected:    
-     Operation op;    
- typename Operation::first_argument_type value;    
- };    
- //绑定第二个参数   
- template <class Operation>     
- class binder2nd: public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {    
- public:    
-     binder2nd(const Operation& x, consttypename Operation::second_argument_type& y) : op(x), value(y) {}    
- typename Operation::result_type operator()(consttypename Operation::first_argument_type& x) const {    
- return op(x, value);  //固定第二个参数  
-     }    
- protected:    
-     Operation op;    
- typename Operation::second_argument_type value;    
- };    
- //外部接口   
- template <class Operation, class T>    
- inline binder1st<Operation> bind1st(const Operation& fn, const T& x) {    
- typedeftypename Operation::first_argument_type Arg1_type;    
- return binder1st<Operation>(fn,Arg1_type(x));    
- }    
- //外部接口   
- template <class Operation, class T>    
- inline binder2nd<Operation> bind2nd(const Operation& fn, const T& x) {    
- typedeftypename Operation::second_argument_type Arg2_type;    
- return binder2nd<Operation>(fn, Arg2_type(x));    
- }    
- 
- //一元操作求反   
- template <class Predicate>    
- class unary_negate: public unary_function<typename Predicate::argument_type, bool> {    
- protected:    
-     Predicate pred;    
- public:    
- explicit unary_negate(const Predicate& x) : pred(x) {}    
- bool operator()(consttypename Predicate::argument_type& x) const {    
- return !pred(x);    
-   }    
- };    
- //二元操作求反   
- template <class Predicate>     
- class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type,bool> {    
- protected:    
-     Predicate pred;    
- public:    
- explicit binary_negate(const Predicate& x) : pred(x) {}    
- bool operator()(consttypename Predicate::first_argument_type& x, consttypename Predicate::second_argument_type& y) const {    
- return !pred(x, y);     
-     }    
- };    
- //外部接口   
- template <class Predicate>    
- inline unary_negate<Predicate> not1(const Predicate& pred)    
- {    
- return unary_negate<Predicate>(pred);    
- }    
- //外部接口   
- template <class Predicate>    
- inline binary_negate<Predicate> not2(const Predicate& pred)    
- {    
- return binary_negate<Predicate>(pred);    
- }    
```cpp
//Functional.h
#pragma once 
//用来定义一元操作的参数类别和返回值类别  
template <class Arg, class Result>  
struct unary_function {  
    typedef Arg argument_type;  //内嵌型别技术  
    typedef Result result_type;  
};  
//用来定义二元操作的参数类别和返回值类别  
template <class Arg1, class Arg2, class Result>  
struct binary_function {  
    typedef Arg1 first_argument_type;  
    typedef Arg2 second_argument_type;  
    typedef Result result_type;  
};  
  
//一元操作，就两个  
template <class T>  
struct negate : public unary_function<T, T> {  
    T operator()(const T& x) const { return -x; }  
};  
template <class T>  
struct logical_not : public unary_function<T,bool> {  
    bool operator()(const T& x) const { return !x; }  
};  
//加减乘除取模  
template <class T>  
struct plus : public binary_function<T, T, T> {  
    T operator()(const T& x, const T& y) const { return x + y; }  
};  
template <class T>  
struct minus : public binary_function<T, T, T> {  
    T operator()(const T& x, const T& y) const { return x - y; }  
};  
template <class T>  
struct multiplies : public binary_function<T, T , T> {  
    T operator()(const T& x, const T& y) const { return x * y; }  
};  
template <class T>  
struct divides : public binary_function<T ,T, T> {  
    T operator()(const T& x, const T& y) const { return x / y; }  
};  
template <class T>  
struct modulus : public binary_function<T, T, T> {  
    T operator()(const T& x, const T& y) const { return x % y; }  
};  
//关系运算  
template <class T>  
struct equal_to : public binary_function<T, T, bool> {  
    bool operator()(const T& x, const T& y) const { return x == y; }  
};  
template <class T>  
struct not_equal_to : public binary_function<T, T,bool> {  
    bool operator()(const T& x, const T& y) const { return x != y; }  
};  
template <class T>  
struct greater : public binary_function<T, T, bool> {  
    bool operator()(const T& x, const T& y) const { return x > y; }  
};  
template <class T>  
struct less : public binary_function<T, T, bool> {  
    bool operator()(const T& x, const T& y) const { return x < y; }  
};  
template <class T>  
struct greater_equal : public binary_function<T, T, bool> {  
    bool operator()(const T& x, const T& y) const { return x >= y; }  
};  
template <class T>  
struct less_equal : public binary_function<T, T, bool> {  
    bool operator()(const T& x, const T& y) const { return x <= y; }  
};  
//逻辑运算  
template <class T>  
struct logical_and : public binary_function<T, T, bool>{  
    bool operator()(const T& x, const T& y) const { return x && y; }  
};  
template <class T>  
struct logical_or : public binary_function<T, T, bool>  
{  
  bool operator()(const T& x, const T& y) const { return x || y; }  
};  
//绑定第一个参数  
template <class Operation>   
class binder1st: public unary_function<typename Operation::second_argument_type, typename Operation::result_type> {  
public:  
    binder1st(const Operation& x, const typename Operation::first_argument_type& y) : op(x), value(y) {} //构造函数  
    typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const {  
        return op(value, x);  //固定第一个参数  
    }  
protected:  
    Operation op;  
    typename Operation::first_argument_type value;  
};  
//绑定第二个参数  
template <class Operation>   
class binder2nd: public unary_function<typename Operation::first_argument_type,typename Operation::result_type> {  
public:  
    binder2nd(const Operation& x, const typename Operation::second_argument_type& y) : op(x), value(y) {}  
    typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {  
        return op(x, value);  //固定第二个参数  
    }  
protected:  
    Operation op;  
    typename Operation::second_argument_type value;  
};  
//外部接口  
template <class Operation, class T>  
inline binder1st<Operation> bind1st(const Operation& fn, const T& x) {  
    typedef typename Operation::first_argument_type Arg1_type;  
    return binder1st<Operation>(fn,Arg1_type(x));  
}  
//外部接口  
template <class Operation, class T>  
inline binder2nd<Operation> bind2nd(const Operation& fn, const T& x) {  
    typedef typename Operation::second_argument_type Arg2_type;  
    return binder2nd<Operation>(fn, Arg2_type(x));  
}  
//一元操作求反  
template <class Predicate>  
class unary_negate: public unary_function<typename Predicate::argument_type, bool> {  
protected:  
    Predicate pred;  
public:  
    explicit unary_negate(const Predicate& x) : pred(x) {}  
    bool operator()(const typename Predicate::argument_type& x) const {  
    return !pred(x);  
  }  
};  
//二元操作求反  
template <class Predicate>   
class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type,bool> {  
protected:  
    Predicate pred;  
public:  
    explicit binary_negate(const Predicate& x) : pred(x) {}  
    bool operator()(const typename Predicate::first_argument_type& x, const typename Predicate::second_argument_type& y) const {  
        return !pred(x, y);   
    }  
};  
//外部接口  
template <class Predicate>  
inline unary_negate<Predicate> not1(const Predicate& pred)  
{  
    return unary_negate<Predicate>(pred);  
}  
//外部接口  
template <class Predicate>  
inline binary_negate<Predicate> not2(const Predicate& pred)  
{  
    return binary_negate<Predicate>(pred);  
}
```
        最后给出算法的定义。可以看到STL的精髓在于算法与容器的分开设计，通过迭代器将两者粘合在一起，实在是精妙之极。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6660943#)
- //Algorithm.h 
- #pragma once  
- 
- //交换两个迭代器所指的元素   
- template <class InputIterator1, class InputIterator2>    
- inlinevoid Iter_swap(InputIterator1 a, InputIterator2 b) {    
-    _Iter_swap(a, b, VALUE_TYPE(InputIterator1)); //VALUE_TYPE返回迭代器的值类型  
- }    
- 
- //真正的交换函数   
- template <class InputIterator1, class InputIterator2, class T>    
- inlinevoid _Iter_swap(InputIterator1 a, InputIterator2 b, T) {    
-    T tmp = *a;    
-    *a = *b;    
-    *b = tmp;    
- }  
- 
- //条件计数 
- template <class InputIterator, class Predicate, class Size>    
- void Count_if(InputIterator first, InputIterator last, Predicate pred, Size& n) {    
- for ( ; first != last; ++first)    
- if (pred(*first))    
-             ++n;    
- }    
- //条件计数，有返回值 
- template <class InputIterator, class Predicate>    
- typename Iterator_traits<InputIterator>::difference_type Count_if(InputIterator first, InputIterator last, Predicate pred) {    
- typename Iterator_traits<InputIterator>::difference_type n = 0;    
- for ( ; first != last; ++first)    
- if (pred(*first))    
-             ++n;    
- return n;    
- }  
```cpp
//Algorithm.h
#pragma once 
//交换两个迭代器所指的元素  
template <class InputIterator1, class InputIterator2>  
inline void Iter_swap(InputIterator1 a, InputIterator2 b) {  
   _Iter_swap(a, b, VALUE_TYPE(InputIterator1)); //VALUE_TYPE返回迭代器的值类型  
}  
//真正的交换函数  
template <class InputIterator1, class InputIterator2, class T>  
inline void _Iter_swap(InputIterator1 a, InputIterator2 b, T) {  
   T tmp = *a;  
   *a = *b;  
   *b = tmp;  
}
//条件计数
template <class InputIterator, class Predicate, class Size>  
void Count_if(InputIterator first, InputIterator last, Predicate pred, Size& n) {  
    for ( ; first != last; ++first)  
        if (pred(*first))  
            ++n;  
}  
//条件计数，有返回值
template <class InputIterator, class Predicate>  
typename Iterator_traits<InputIterator>::difference_type Count_if(InputIterator first, InputIterator last, Predicate pred) {  
    typename Iterator_traits<InputIterator>::difference_type n = 0;  
    for ( ; first != last; ++first)  
        if (pred(*first))  
            ++n;  
    return n;  
}
```
本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
