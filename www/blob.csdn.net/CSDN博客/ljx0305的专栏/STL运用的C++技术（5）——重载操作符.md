# STL运用的C++技术（5）——重载操作符 - ljx0305的专栏 - CSDN博客
2013年01月30日 15:22:50[ljx0305](https://me.csdn.net/ljx0305)阅读数：418
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
STL是C++标准库的重要组成部分之一，它不仅是一个可复用的组件库，更是一个包含算法与数据结构的软件框架，同时也是C++泛型编程的很好例子。STL中运用了许多C++的高级技术。本文介绍重载操作符。主要参考了《C++
 Primer》和《STL源码剖析》。
       重载操作符是具有特殊名称的函数：保留了operator后接需定义的操作符符号。这是《C++ Primer》中的定义。在STL中重载操作符主要用在两个地方，一个是迭代器中，另一个是算法中。本文介绍在迭代器中的应用，下篇介绍在算法中的应用。在本系列博文中，多次提到迭代器，不愧是STL的关键所在。迭代器就像是各种容器对象的智能指针，这仅仅是我的理解。指针的各种行为中常见的有解引用操作符（*）、箭头操作符（->）、自增、自减等。对于容器来说，必须重载这些操作符，以适应自身的指针行为。重载这些操作符，迭代器责无旁贷。看一段STL的源码，做了些修改，更清楚一些。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)
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
- typedef bidirectional_iterator_tag iterator_category;  
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
	typedef bidirectional_iterator_tag iterator_category;
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
      上面这段代码展现了这些操作符是如何被重载。其实这是个双向链表的迭代器定义，有自增和自减。再进一步，那么链表如何使用上述定义的迭代器呢？下面给出链表的定义，只取STL链表的部分功能，同时给出了测试用例。已在VS2008下测试通过。
**[cpp]**[view plain](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6657417#)
- #include <iostream> 
- usingnamespace std;  
- 
- //结点定义，双向链表，把上面的代码拷贝下来即可 
- //链表的迭代器，把上面的代码拷贝下来即可 
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
- //测试用例 
- int main()  
- {  
-     List<int> l;  
-     l.push_back(1);  
-     l.push_back(2);  
-     cout<<l.front()<<' '<<l.back()<<endl; //1 2
-     l.push_front(3);  
-     l.push_front(4);  
-     cout<<l.front()<<' '<<l.back()<<endl; //4 2
-     l.pop_back();  
-     l.pop_front();  
-     cout<<l.front()<<' '<<l.back()<<endl; //3 1  
-     cout<<l.empty()<<endl;  //0
-     l.clear();  
-     cout<<l.empty()<<endl;  //1
- return 0;  
- }  
```cpp
#include <iostream>
using namespace std;
//结点定义，双向链表，把上面的代码拷贝下来即可
//链表的迭代器，把上面的代码拷贝下来即可
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
//测试用例
int main()
{
	List<int> l;
	l.push_back(1);
	l.push_back(2);
	cout<<l.front()<<' '<<l.back()<<endl; //1 2
	l.push_front(3);
	l.push_front(4);
	cout<<l.front()<<' '<<l.back()<<endl; //4 2
	l.pop_back();
	l.pop_front();
	cout<<l.front()<<' '<<l.back()<<endl; //3 1	 
	cout<<l.empty()<<endl;  //0
	l.clear();
	cout<<l.empty()<<endl;  //1
	return 0;
}
```
       上面这两段程序已经给出了链表的部分功能，同时看到了迭代器的运用以及重载操作符的实现。搞清楚了以上代码，再去看STL的源代码，可能会轻松一点吧，大同小异，核心的结构基本上都是这样的。
本人享有博客文章的版权，转载请标明出处[http://blog.csdn.net/wuzhekai1985](http://blog.csdn.net/wuzhekai1985)
