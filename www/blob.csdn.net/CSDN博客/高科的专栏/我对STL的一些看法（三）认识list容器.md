
# 我对STL的一些看法（三）认识list容器 - 高科的专栏 - CSDN博客

2013年12月09日 16:03:48[高科](https://me.csdn.net/pbymw8iwm)阅读数：6006个人分类：[C/C++																](https://blog.csdn.net/pbymw8iwm/article/category/910215)[STL																](https://blog.csdn.net/pbymw8iwm/article/category/1793843)[
							](https://blog.csdn.net/pbymw8iwm/article/category/910215)



## C++ List（双向链表）
是一个线性链表结构，它的数据由若干个节点构成，每一个节点都包括一个信息块（即实际存储的数据）、一个前驱指针和一个后驱指针。它无需分配指定的内存大小且可以任意伸缩，这是因为它存储在非连续的内存空间中，并且由指针将有序的元素链接起来。由于其结构的原因，list 随机检索的性能非常的不好，因为它不像vector 那样直接找到元素的地址，而是要从头一个一个的顺序查找，这样目标元素越靠后，它的检索时间就越长。检索时间与目标元素的位置成正比。虽然随机检索的速度不够快，但是它可以迅速地在任何节点进行插入和删除操作。因为list
 的每个节点保存着它在链表中的位置，插入或删除一个元素仅对最多三个元素有所影响，不像vector 会对操作点之后的所有元素的存储地址都有所影响，这一点是vector 不可比拟的。

### list 的特点：
(1) 不使用连续的内存空间这样可以随意地进行动态操作；
(2) 可以在内部任何位置快速地插入或删除，当然也可以在两端进行push和pop 。
(3) 不能进行内部的随机访问，即不支持[ ] 操作符和vector.at() ；
Lists将元素按顺序储存在链表中，与向量(vectors)相比，它允许快速的插入和删除，但是随机访问却比较慢.
4.clear() 删除所有元素
5.empty() 如果list是空的则返回true
6.end() 返回末尾的迭代器
7.erase() 删除一个元素
语法：
iterator erase( iterator loc );//删除loc处的元素
iterator erase( iterator start, iterator end ); //删除start和end之间的元素
8.front() 返回第一个元素的引用
9.get_allocator() 返回list的配置器
10.insert() 插入一个元素到list中
语法：
iterator insert( iterator loc, const TYPE &val );
//在指定位置loc前插入值为val的元素,返回指向这个元素的迭代器,
void insert( iterator loc, size_type num, const TYPE &val );
//定位置loc前插入num个值为val的元素
void insert( iterator loc, input_iterator start, input_iterator end );
//在指定位置loc前插入区间[start, end)的所有元素
11.max_size() 返回list能容纳的最大元素数量
12.merge() 合并两个list
语法: 6
void merge( list &lst);//把自己和lst链表连接在一起
void merge( list &lst, Comp compfunction );
//指定compfunction，则将指定函数作为比较的依据。
13.pop_back() 删除最后一个元素
14.pop_front() 删除第一个元素
15.push_back() 在list的末尾添加一个元素
16.push_front() 在list的头部添加一个元素
17.rbegin() 返回指向第一个元素的逆向迭代器
18.remove() 从list删除元素
语法:
void remove( const TYPE &val );
//删除链表中所有值为val的元素
19.remove_if() 按指定条件删除元素
20.rend() 指向list末尾的逆向迭代器
21.resize() 改变list的大小
语法:
void resize( size_type num, TYPE val );
//把list的大小改变到num。被加入的多余的元素都被赋值为val22.
22.reverse() 把list的元素倒转
23.size() 返回list中的元素个数
24.sort() 给list排序
语法:
void sort();//为链表排序，默认是升序
void sort( Comp compfunction );//采用指定函数compfunction来判定两个元素的大小。
25.splice() 合并两个list
语法:
void splice( iterator pos, list &lst );//把lst连接到pos的位置
void splice( iterator pos, list &lst, iterator del );//插入lst中del所指元素到现链表的pos上
void splice( iterator pos, list &lst, iterator start, iterator end );//用start和end指定范围。
26.swap() 交换两个list
语法：
void swap( list &lst );// 交换lst和现链表中的元素
27.unique() 删除list中重复的元素
语法:
void unique();//删除链表中所有重复的元素
void unique( BinPred pr );// 指定pr，则使用pr来判定是否删除。
当然这个里边有很多函数都有需要注意的地方，比如：\#include<iostream>
\#include <list>
using namespace std;
int  main()
{
list<int> a(10,89);
list<int> b (1,34);
b.merge(a);
cout<<"a size:"<<a.size()<<endl<<"b size:"<<b.size()<<endl;
return 0;
}
你认为结果会是多少呢？

