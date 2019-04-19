# STL之List使用总结 - fanyun的博客 - CSDN博客
2017年05月31日 21:20:51[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：354
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
         List是我们常用的STL标准容器之一，它是一种序列式容器，在数据结构上和双向链表结构类似，List中的数据元素是通过链表指针串连逻辑意义上的线性表，因此，链表的特点是支持元素在任意位置的插入删除，且效率很高。List实际上就是我们数据结构中的双向链表，它的内存是不连续的，通过指针来进行数据访问，这个使得它效率低下，因此它没有提供[]操作符的重载。但是由于链表的特点，他可以很好的效率支持任意地方的删除和插入。
**1. List的初始化及销毁**
(1). 默认构造函数,长度为0的列表
 list<int> lis1;
 (2).带有单个整形参数的构造函数,长度为50的列表
 list<int> lis2(50);
 list<int> lis3(50,1);//长度为50，初始值为1的列表
 (3).复制构造函数,构造一个新的列表lis4，作为已存在的列表lis3的完全复制
 list<int> lis4(lis3);
(4).带两个常量参数的构造函数，产生初始值为一个区间的向量。区间由一个半开区间[first,last) 来指定
 list<int> lis5(lis3.begin(),lis3.end());
由上可以看出：Lsit和Vector的初始化及销毁是一样的
**2. List常用函数：**
（1）Begin（）和end()
通过调用list容器的成员函数begin()得到一个指向容器起始位置的iterator，可以调用list容器的 end() 函数来得到list末端下一位置
 (2). push_back() 和push_front()
使用list的成员函数push_back和push_front插入一个元素到list中。其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入
(3). empty()
判断list()是否为空。
(4).resize()
如果调用resize(n)将list的长度改为只容纳n个元素，超出的元素将被删除，如果需要扩展那么调用默认构造函数T()将元素加到list末端。如果调用resize(n,val)，则扩展元素要调用构造函数T(val)函数进行元素构造，其余部分相同。
(5).clear()
清空list中的所有元素。
(6).front()和back()
通过front()可以获得list容器中的头部元素，通过back()可以获得list容器的最后一个元素。但是有一点要注意，就是list中元素是空的时候，这时候调用front()和back()会发生什么呢？实际上会发生不能正常读取数据的情况，但是这并不报错，那我们编程序时就要注意了，个人觉得在使用之前最好先调用empty()函数判断list是否为空。
(7). pop_back和pop_front()
通过删除最后一个元素，通过pop_front()删除第一个元素；序列必须不为空，如果当list为空的时候调用pop_back()和pop_front()会使程序崩掉。
(8).assign()
具体和vector中的操作类似，也是有两种情况，第一种是：l1.assign(n,val)将 l1中元素变为n个T(val）。第二种情况是：l1.assign(l2.begin(),l2.end())将l2中的从l2.begin()到l2.end()之间的数值赋值给l1。
(9).swap()
交换两个链表(两个重载)，一个是l1.swap(l2); 另外一个是swap(l1,l2)，都可能完成连个链表的交换。
(10). reverse()
通过reverse()完成list的逆置。
(11). Merge()
合并两个链表并使之默认升序(也可改)，l1.merge(l2，greater<int>());调用结束后l2变为空，l1中元素包含原来l1 和 l2中的元素，并且排好序，升序。其实默认是升序，greater<int>()可以省略，另外greater<int>()是可以变的，也可以不按升序排列。
(12). Insert()
在指定位置插入一个或多个元素(三个重载)：
l1.insert(l1.begin(),10); 在l1的开始位置插入10。
l1.insert(l1.begin(),2,20); 在l1的开始位置插入2个20。
l1.insert(l1.begin(),l2.begin(),l2.end());在l1的开始位置插入l2的从开始到结束的所有位置的元素。
(13).erase()
删除一个元素或一个区域的元素(两个重载)
l1.erase(l1.begin()); 将l1的第一个元素删除。
l1.erase(l1.begin(),l1.end());将l1的从begin()到end()之间的元素删除。
```cpp
#include <iostream>
#include <list>
using namespace std;
int main()
{
    list<int> l1;
    list<int> l2(2,0);
    list<int>::iterator iter;
    l1.push_back(1);
    l1.push_back(2);
    l2.push_back(3);
    l1.merge(l2,greater<int>());//合并后升序排列，实际上默认就是升序
    for(iter = l1.begin() ; iter != l1.end() ; iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl<<endl;
    if(l2.empty())
    {
        cout<<"l2 变为空 ！！";
    }
    cout<<endl<<endl;
    return 0;
}
```
