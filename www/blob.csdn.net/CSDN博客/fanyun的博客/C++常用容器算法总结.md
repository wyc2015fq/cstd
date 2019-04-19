# C++常用容器算法总结 - fanyun的博客 - CSDN博客
2018年07月08日 22:53:46[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：184
1.找到并输出Vector中的最大值和最小值
    代码如下：
```cpp
// VectorSort.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
	vector<int> vec(12);	//12行
	int i = 0;
	for (vector<int >::iterator ite = vec.begin(); ite != vec.end(); ++ite)
	{
		*ite = i;
		i++;
	}
	auto maxMin = minmax_element(vec.begin(), vec.end());
	cout << *maxMin.first << "," << *maxMin.second << endl;
	return 0;
}
```
输出结果如下图1所示：
![](https://img-blog.csdn.net/20180708220858609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                        图1  vector求最大最小测试结果
2.vector中元素去重
     利用unique函数(#include<algorithm>)和sort函数取出vector中的重复元素。unique函数实际上是一种伪去除函数， 它可以将容器中相邻的重复元素放到容器的末尾，然后返回第一个重复元素的地址。由于重复元素相邻才能放到末尾 所以需要使用sort先进行排序
    代码如下：
```cpp
// ordertoheavy.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
	vector<int> vec;
	vector<int>::iterator iter;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	sort(vec.begin(), vec.end());
	for (vector<int>::iterator i = vec.begin(); i != vec.end(); ++i)
	{
		cout << *i << " ";
	}
	cout << endl;
	iter = unique(vec.begin(), vec.end());
	vec.erase(iter, vec.end());
	for (iter = vec.begin(); iter != vec.end(); ++iter)
		cout << *iter << " ";
	cout << endl;
	return 0;
}
```
效果如图2所示：
![](https://img-blog.csdn.net/20180708222155800?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                               图2  vector 去重测试结果
3.使用map在插入键值对时会自动排序，可以灵活实现对数据的各种要求排序。如排序并获得排序前对应的角标索引， 不排序获得排序后的名次。
   代码如下：
```cpp
// mapSort.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;
class a
{
public:
	struct strA
	{
		double d;
		int I;
	};
};
int main()
{
	vector<a::strA> a;
	a.resize(7);
	a[0].d = 2;
	a[1].d = 6;
	a[2].d = 0;
	a[3].d = -1;
	a[4].d = 18.4;
	a[5].d = 13;
	a[6].d = 11.2;
	map<double, int> temp;
	for (int i = 0; i < a.size(); ++i)
		temp.insert(make_pair(a[i].d, i));	//数据放入map中会自动按键值升序排序
	int i = a.size();
	for (map<double, int>::iterator it = temp.begin(); it != temp.end(); ++it)
	{
		a[it->second].I = i;
		i--;
	}
	for (int i = 0; i < a.size(); ++i)
		cout << a[i].d <<"的"<< "序号：" << a[i].I << endl;
	return 0;
}
```
排序结果如图3所示：
![](https://img-blog.csdn.net/20180708223138875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                图3 map排序结果
