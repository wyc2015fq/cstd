# STL priority_queue - qq_37385726的博客 - CSDN博客





2018年07月30日 11:02:34[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：38








## **STL priority_queue可以直接调函数就实现的功能是：**

**1.入队  push**

**2.出队 pop**

**3.返回队首元素 top**

**4.判空  size,empty**

**5.队列元素数  size**



## **STL priority_queue可以手动实现的功能是：**

**1.查询**

**2.删除**

**3.遍历**



```cpp
​
#include<iostream>
using namespace std;
#include<queue>

int main(void)
{
	priority_queue<int,vector<int>,less<int> > pq1;  //从大到小
	priority_queue<int,vector<int>,greater<int> > pq2;  //从小到大
	
//****************queue入队********************// 
	pq1.push(1);
	pq1.push(2);
	pq1.push(3);
	pq1.push(4);
	
	pq2.push(1);
	pq2.push(2);
	pq2.push(3);
	pq2.push(4);


//****************queue队首元素********************// 
	cout<<"队首元素"<<endl;
	cout<<"queue.top(pq1)  "<<pq1.top()<<endl; 
	cout<<"queue.top(pq2)  "<<pq2.top()<<endl; 
	cout<<endl<<endl;
	
	
	
//****************priority_queue遍历元素********************//  
	
	//priority_queue没有支持的遍历元素的方法，没有迭代器
	//如果要便利只能pop()和push()结合使用
	cout<<"遍历pq1"<<endl; 
	int sz=pq1.size();
	int t[sz];
	for(int i=0;i<sz;i++)
	{
		int temp=pq1.top();
		pq1.pop();
		t[i]=temp;
		cout<<temp<<endl;	
	} 
	for(int i=0;i<sz;i++)
	{
		pq1.push(t[i]);
	}

	cout<<endl<<endl;


//****************priority_queue出队********************//  
	cout<<"当前队首出队pq1"<<endl;
	pq1.pop();
	int siz=pq1.size();
	int tt[siz];
	for(int i=0;i<siz;i++)
	{
		int temp=pq1.top();
		pq1.pop();
		tt[i]=temp;
		cout<<temp<<endl;	
	} 
	for(int i=0;i<siz;i++)
	{
		pq1.push(tt[i]);
	}

	cout<<endl<<endl;
	
	
//****************priority_queue判空********************//

	//empty和size函数判空
	if(pq1.size()==0)
	{
		cout<<"way:priority_queue.size()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:priority_queue.size()  "<<"nonempty"<<endl;
	}
	if(pq1.empty())
	{
		cout<<"way:priority_queue.empty()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:priority_queue.empty()  "<<"nonempty"<<endl;
	}
	cout<<endl<<endl; 
	return 0;
}

​
```





