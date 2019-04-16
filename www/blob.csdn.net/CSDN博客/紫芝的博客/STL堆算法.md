# STL堆算法 - 紫芝的博客 - CSDN博客





2018年04月25日 22:59:09[紫芝](https://me.csdn.net/qq_40507857)阅读数：31
个人分类：[STL](https://blog.csdn.net/qq_40507857/article/category/7595795)









# STL堆算法


## 1.make_heap()
把指定范围内的元素生成一个堆


## 2.pop_back()
①并不是真正的把最大的元素从堆中弹出，而是重新排序堆
②把first和last-1交换，然后重新生成一个堆
③可以使用容器的back来访问被弹出的元素，或使用pop_back进行删除


## 3.push_back()
①假设first到last-1是一个有效堆，被加入堆的元素存放在last-1的位置，重新生成堆
②在使用该函数前，必须先把元素插入容器末尾 


## 4.sort_heap()

对指定范围内的元素重新排序，假设该序列是一个有序堆 




```cpp
//#include<bits/stdc++.h>
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	int myints[]={10,20,30,5,15};
	vector<int>::iterator it;
	vector<int> v(myints,myints+5);
	
	make_heap(v.begin(),v.end());
	cout<<"initinal max heap:"<<v.front()<<endl;
	
	pop_heap(v.begin(),v.end());
	v.pop_back();
	cout<<"max heap after pop:"<<v.front()<<endl;
	
	v.push_back(99);
	push_heap(v.begin(),v.end());
	cout<<"max heap after push:"<<v.front()<<endl;
	
	sort_heap(v.begin(),v.end());
	cout<<"final sorted range:";
	for(int i=0;i<v.size();i++)
	cout<<" "<<v[i];
	cout<<endl;
	return 0; 
 }
```




