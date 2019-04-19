# C++list的使用总结及常用list操作 - fanyun的博客 - CSDN博客
2017年02月26日 21:12:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：44302
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一、List定义：**
List是stl实现的双向链表，与向量(vectors)相比, 它允许快速的插入和删除，但是随机访问却比较慢。使用时需要添加头文件
#include <list>
**二、List定义和初始化：**
    list<int>lst1;          //创建空list
    list<int> lst2(5);       //创建含有5个元素的list
    list<int>lst3(3,2);  //创建含有3个元素的list
    list<int>lst4(lst2);    //使用lst2初始化lst4
    list<int>lst5(lst2.begin(),lst2.end());  //同lst4
**三、List常用操作函数：**
Lst1.assign() 给list赋值 
Lst1.back() 返回最后一个元素 
Lst1.begin() 返回指向第一个元素的迭代器 
Lst1.clear() 删除所有元素 
Lst1.empty() 如果list是空的则返回true 
Lst1.end() 返回末尾的迭代器 
Lst1.erase() 删除一个元素 
Lst1.front() 返回第一个元素 
Lst1.get_allocator() 返回list的配置器 
Lst1.insert() 插入一个元素到list中 
Lst1.max_size() 返回list能容纳的最大元素数量 
Lst1.merge() 合并两个list 
Lst1.pop_back() 删除最后一个元素 
Lst1.pop_front() 删除第一个元素 
Lst1.push_back() 在list的末尾添加一个元素 
Lst1.push_front() 在list的头部添加一个元素 
Lst1.rbegin() 返回指向第一个元素的逆向迭代器 
Lst1.remove() 从list删除元素 
Lst1.remove_if() 按指定条件删除元素 
Lst1.rend() 指向list末尾的逆向迭代器 
Lst1.resize() 改变list的大小 
Lst1.reverse() 把list的元素倒转 
Lst1.size() 返回list中的元素个数 
Lst1.sort() 给list排序 
Lst1.splice() 合并两个list 
Lst1.swap() 交换两个list 
Lst1.unique() 删除list中重复的元素
**四、List使用示例：**
示例1：遍历List
    //迭代器法
```cpp
for(list<int>::const_iteratoriter = lst1.begin();iter != lst1.end();iter++)
    {
      cout<<*iter;
    }
    cout<<endl;
```
示例2：
```cpp
#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>
#include <windows.h>
using namespace std;
 
typedef list<int> LISTINT;
typedef list<int> LISTCHAR;
 
void main()
{
    //用LISTINT创建一个list对象
    LISTINT listOne;
    //声明i为迭代器
    LISTINT::iterator i;
 
    listOne.push_front(3);
    listOne.push_front(2);
    listOne.push_front(1);
 
    listOne.push_back(4);
    listOne.push_back(5);
    listOne.push_back(6);
 
    cout << "listOne.begin()--- listOne.end():" << endl;
    for (i = listOne.begin(); i != listOne.end(); ++i)
        cout << *i << " ";
    cout << endl;
 
    LISTINT::reverse_iterator ir;
    cout << "listOne.rbegin()---listOne.rend():" << endl;
    for (ir = listOne.rbegin(); ir != listOne.rend(); ir++) {
        cout << *ir << " ";
    }
    cout << endl;
 
    int result = accumulate(listOne.begin(), listOne.end(), 0);
    cout << "Sum=" << result << endl;
    cout << "------------------" << endl;
 
    //用LISTCHAR创建一个list对象
    LISTCHAR listTwo;
    //声明i为迭代器
    LISTCHAR::iterator j;
 
    listTwo.push_front('C');
    listTwo.push_front('B');
    listTwo.push_front('A');
 
    listTwo.push_back('D');
    listTwo.push_back('E');
    listTwo.push_back('F');
 
    cout << "listTwo.begin()---listTwo.end():" << endl;
    for (j = listTwo.begin(); j != listTwo.end(); ++j)
        cout << char(*j) << " ";
    cout << endl;
 
    j = max_element(listTwo.begin(), listTwo.end());
    cout << "The maximum element in listTwo is: " << char(*j) << endl;
    Sleep(10000);
}
```
![](https://img-blog.csdn.net/20170224212056307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```cpp
#include <iostream> 
#include <list> 
#include <windows.h>
using namespace std;
typedef list<int> INTLIST;
//从前向后显示list队列的全部元素 
void put_list(INTLIST list, char *name)
{
	INTLIST::iterator plist;
	cout << "The contents of " << name << " : ";
	for (plist = list.begin(); plist != list.end(); plist++)
		cout << *plist << " ";
	cout << endl;
}
//测试list容器的功能 
void main(void)
{
	//list1对象初始为空 
	INTLIST list1;
	INTLIST list2(5, 1);
	INTLIST list3(list2.begin(), --list2.end());
	//声明一个名为i的双向迭代器 
	INTLIST::iterator i;
	put_list(list1, "list1");
	put_list(list2, "list2");
	put_list(list3, "list3");
	list1.push_back(7);
	list1.push_back(8);
	cout << "list1.push_back(7) and list1.push_back(8):" << endl;
	put_list(list1, "list1");
	list1.push_front(6);
	list1.push_front(5);
	cout << "list1.push_front(6) and list1.push_front(5):" << endl;
	put_list(list1, "list1");
	list1.insert(++list1.begin(), 3, 9);
	cout << "list1.insert(list1.begin()+1,3,9):" << endl;
	put_list(list1, "list1");
	//测试引用类函数 
	cout << "list1.front()=" << list1.front() << endl;
	cout << "list1.back()=" << list1.back() << endl;
	list1.pop_front();
	list1.pop_back();
	cout << "list1.pop_front() and list1.pop_back():" << endl;
	put_list(list1, "list1");
	list1.erase(++list1.begin());
	cout << "list1.erase(++list1.begin()):" << endl;
	put_list(list1, "list1");
	list2.assign(8, 1);
	cout << "list2.assign(8,1):" << endl;
	put_list(list2, "list2");
	cout << "list1.max_size(): " << list1.max_size() << endl;
	cout << "list1.size(): " << list1.size() << endl;
	cout << "list1.empty(): " << list1.empty() << endl;
	put_list(list1, "list1");
	put_list(list3, "list3");
	cout << "list1>list3: " << (list1 > list3) << endl;
	cout << "list1<list3: " << (list1 < list3) << endl;
	list1.sort();
	put_list(list1, "list1");
	list1.splice(++list1.begin(), list3);
	put_list(list1, "list1");
	put_list(list3, "list3");
	Sleep(10000);
}
```
![](https://img-blog.csdn.net/20170224215445484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
