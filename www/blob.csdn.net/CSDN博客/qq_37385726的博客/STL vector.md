# STL vector - qq_37385726的博客 - CSDN博客





2018年07月30日 12:04:43[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：25标签：[STL  																[vector](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=STL  &t=blog)
个人分类：[STL](https://blog.csdn.net/qq_37385726/article/category/7888287)








## STL vector直接调函数就能实现的操作：

**1.追加元素 push_back**

**2.插入元素 insert(vec.begin()+i , j)**

**3.删除尾元素  pop_back**

**4.删除元素 erase(vec.begin()+i)  erase(vec.begin()+i,vec.begin()+j)删除[i,j)**

**5.返回首元素  front**

**6.返回尾元素  back**

**7.迭代器  vector<int>::iterator**

**8.遍历  数组遍历，迭代器遍历**

**9.排序  #include<algorithm>  sort(vec.begin(),vec.end())    ****sort(vec.begin(),vec.end(),cmp)  //cmp排序规则**

**10.翻转  #include<algorithm>  reverse(vec.begin(),vec.end())**

**11.清空 clear**



## STL vector手动就能实现的操作：

**1.查询  **



```cpp
#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>


bool cmp(int a,int b)
{
	return a>b;
}



int main(void)
{
	vector<int> vec;

//****************vector追加元素********************// 
	cout<<"push_back函数追加元素"<<endl; 
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	cout<<endl<<endl;


//****************vector插入元素********************// 
	cout<<"insert插入元素"<<endl;
	vec.insert(vec.begin(),100);  //在vector首插入元素 
	vec.insert(vec.begin()+1,1000);  //在vector首后的第一个位置插入元素 
 	cout<<endl<<endl;
 	


//****************vector首元素********************//

	cout<<"vector首元素"<<endl;
	cout<<"vector.front  "<<vec.front()<<endl;
	cout<<endl<<endl;
	
	
//****************vector尾元素********************//
	
	cout<<"vector尾元素"<<endl;
	cout<<"vector.back  "<<vec.back()<<endl;
	cout<<endl<<endl; 
	



//****************vector遍历元素********************//

	//数组方式遍历
	cout<<"数组方式遍历"<<endl; 
	int sz=vec.size(); 
	for(int i=0;i<sz;i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl<<endl;

	//迭代器遍历
	cout<<"迭代器遍历"<<endl;
	vector<int>::iterator iter;
	for(iter=vec.begin();iter!=vec.end();iter++)
	{
		cout<<*iter<<endl;	
	} 
	cout<<endl<<endl;
	
	
//****************vector查找元素********************//
	cout<<"查找元素"<<endl;
	cout<<"在遍历的基础上添加查找的功能即可"<<endl; 
	cout<<endl<<endl;




//****************vector删除尾元素********************//
	cout<<"删除尾元素pop_back"<<endl;
	vec.pop_back(); 
	cout<<endl<<endl;




//****************vector删除元素********************//
	cout<<"删除元素"<<endl;
	cout<<"删除首元素  "<<endl;
	vec.erase(vec.begin());
	cout<<"删除首元素后1个位置到首元素后3个位置间的这三个元素  " <<endl;
	vec.erase(vec.begin()+1,vec.begin()+4);
	
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl<<endl;



//****************vector判空********************//

	//empty和size函数判空
	if(vec.size()==0)
	{
		cout<<"way:vector.size()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:vector.size()  "<<"nonempty"<<endl;
	}
	if(vec.empty())
	{
		cout<<"way:vector.empty()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:vector.empty()  "<<"nonempty"<<endl;
	}
	cout<<endl<<endl; 
	
	
	 
//****************vector排序********************//	 
	cout<<"从小到大排序"<<endl;
	sort(vec.begin(),vec.end()); // 默认从小到大
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<"从大到小排序"<<endl;
	sort(vec.begin(),vec.end(),cmp); // 添加cmp排序规则 
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl<<endl;

//****************vector元素翻转********************//
	
	cout<<"翻转"<<endl;
	reverse(vec.begin(),vec.end());
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl<<endl;

//****************vector清空********************//
	vec.clear(); 


}
```






