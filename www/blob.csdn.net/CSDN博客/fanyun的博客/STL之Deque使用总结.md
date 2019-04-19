# STL之Deque使用总结 - fanyun的博客 - CSDN博客
2017年05月31日 21:21:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：359标签：[STL](https://so.csdn.net/so/search/s.do?q=STL&t=blog)
个人分类：[C++																[数据结构](https://blog.csdn.net/fanyun_01/article/category/7129881)](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
          Deque是STL的标准容器之一，使用和Vector类似，在许多操作的地方可以直接替代。支持随即存取，也就是[]操作符；支持两端操作，push(pop)-back(front)，在两端操作上与list效率差不多
          因此在实际使用时，如何选择这三个容器中哪一个，应根据你的需要而定，一般应遵循下面的原则： 
  (1).如果你需要高效的随即存取，而不在乎插入和删除的效率，使用vector 
  (2).如果你需要大量的插入和删除，而不关心随即存取，则应使用list 
  (3).如果你需要随即存取，而且关心两端数据的插入和删除，则应使用deque。
**1. Deque的初始化及销毁**
(1).创建一个没有任何元素的deque对象
deque<int> d1; 
(2). 创建一个具有10个元素的deque对象，每个元素值为默认deque<int> d2(10); 
(3). 创建一个具有10个元素的deque对象,每个元素的初始值为5.5
deque<double> d3(10, 5.5); 
(4). 通过拷贝一个deque对象的元素值, 创建一个新的deque对象deque<double> d4(d3); 
int iArray[] = {11, 13, 19,23, 27};
(5). 将迭代器区间[first, last)所指的元素拷贝到一个新创建的deque对象中
deque<int> d5(iArray,iArray+5);
(6). Deque的销毁
~Deque();
**2. Deque使用函数总结：**
(1).c.assign(beg,end)
将[beg; end)区间中的数据赋值给c。
(2).c.assign(n,elem)
将n个elem的拷贝赋值给c。
(3).c.at(idx)
传回索引idx所指的数据，如果idx越界，抛出out_of_range。
(4).c.back()
传回最后一个数据，不检查这个数据是否存在。
(5).c.begin()
传回迭代器重的可一个数据。
(6).c.clear()
移除容器中所有数据。
(7).deque<Elem> c
创建一个空的deque。
(8).deque<Elem> c1(c2)
复制一个deque。
(9).Deque<Elem> c(n)
创建一个deque，含有n个数据，数据均已缺省构造产生。
(10).Deque<Elem> c(n,elem)
创建一个含有n个elem拷贝的deque。
(11).Deque<Elem>c(beg,end)
创建一个以[beg;end)区间的deque。
(12).c.~deque<Elem>()
销毁所有数据，释放内存。
(13).c.empty()
判断容器是否为空。
(14).c.end()
指向迭代器中的最后一个数据地址。
(15).c.erase(pos)
删除pos位置的数据，传回下一个数据的位置。
(16).c.erase(beg,end)
删除[beg,end)区间的数据，传回下一个数据的位置。
(17).c.front()
传回地一个数据。
(18).get_allocator
使用构造函数返回一个拷贝。
(19).c.insert(pos,elem)
在pos位置插入一个elem拷贝，传回新数据位置。
(20).c.insert(pos,n,elem)
在pos位置插入n个elem数据。无返回值
(21).c.insert(pos,beg,end)
在pos位置插入在[beg,end)区间的数据。无返回值。
(22).c.max_size()
返回容器中最大数据的数量。
(23).c.pop_back()
删除最后一个数据。
(24).c.pop_front()
删除头部数据。
(25).c.push_back(elem)
在尾部加入一个数据。
(26).c.push_front(elem)
在头部插入一个数据。
(27).c.rbegin()
传回一个逆向队列的第一个数据。
(28).c.rend()
传回一个逆向队列的最后一个数据的下一个位置。
(29).c.resize(num)
重新指定队列的长度。
(30).c.size()
返回容器中实际数据的个数。
(31).C1.swap(c2)
将c1和c2元素互换。
(32).Swap(c1,c2)
同上操作。
```cpp
#include <iostream>
#include <deque>
using namespace std;
void printDeque(deque<int> d)
{
//使用下标
for (unsigned int i = 0; i < d.size(); i++)
{
 cout<<"d["<<i<<"] = "<<d[i]<<", ";
}
//使用迭代器
deque<int>::iterator iter = d.begin();
for (;iter != d.end(); iter ++)
{
 cout<<"d["<<iter-d.begin()<<"] = "<<(*iter)<<", ";
}
//使用迭代器指针
deque<int>::iterator *pIter = new deque<int>::iterator;
if ( NULL == pIter )
{
   return ;
}
for (*pIter = d.begin(); *pIter != d.end(); (*pIter)++)
{
   cout<<"d["<<*pIter - d.begin() <<"]="<<**pIter<<", ";
}
if (NULL != pIter)
{
   delete pIter;
   pIter = NULL;
}
cout<<endl;
}
void main()
{
//创建deque
deque<int> d1; 
deque<int> d2(10);
deque<double> d3(10, 5.5); 
deque<double> d4(d3); 
int iArray[] = {11, 13, 19, 23, 27};
deque<int> d5(iArray, iArray+5);
//初始化赋值：同vector一样,使用尾部插入函数push_back()
for (int i = 1; i < 6 ; i++)
   d1.push_back(i*10);
//遍历元素: 1-下标方式 2-迭代器方式 反向遍
cout<<"printDeque(d1) : "<<endl;
printDeque(d1);
//元素插入：尾部插入用push_back()，头部插入用push_front()，其它位置插入用insert(&pos, elem)
cout<<"d1.push_front(100): "<<endl;
d1.push_front(100);
printDeque(d1);
cout<<"d1.insert(d1.begin()+3, 200): "<<endl; //支持随机存取(即[]操作符)，所以begin()可以+3
d1.insert(d1.begin()+2,200);
printDeque(d1);
//元素删除 尾部删除用pop_back();头部删除用pop_front(); 
//任意迭代位置或迭代区间上的元素删除用erase(&pos)/erase(&first, &last)；删除所有元素用clear();
cout<<"d1.pop_front(): "<<endl;
d1.pop_front();
printDeque(d1);
cout<<"d1.erase(d1.begin()+1): "<<endl;
d1.erase(d1.begin()+1); //删除第2个元素d1[1]
printDeque(d1);
cout<<"d1.erase(d1.begin(), d1.begin() + 2) = "<<endl;
d1.erase(d1.begin(), d1.begin() + 2);
printDeque(d1);
cout<<"d1.clear() :"<<endl;
d1.clear();
printDeque(d1);
//其它常用
cout<<"其它常用用法: "<<endl;
int flag = 0;
while(flag < 2)
{
   if (0 == flag )
   {
    for (int i = 1; i < 6 ; i++) //恢复
     d1.push_back(i*10);
   }
   else
   {
    d1.clear();
    cout<<"after d1.clear() , d1.front(), d1.back() is abnormal! other info.:"<<endl;
   }
   cout<<"d1.empty() = "<<d1.empty()<<endl;
   cout<<"d1.size() = "<<d1.size()<<endl;
   cout<<"d1.max_size() = "<<hex<<d1.max_size()<<endl;
   if (!d1.empty())
   {
    cout<<"d1.front() = "<<d1.front()<<endl;
    cout<<"d1.back() = "<<d1.back()<<endl;
   }
  
   flag++;
  
}
//交换
cout<<"d1.swap(d5)= "<<endl;
d1.swap(d5);
cout<<"d1 = ";
printDeque(d1);
cout<<"d5 = ";
printDeque(d5);
//printDeque(d)
}
```
