# STL queue - qq_37385726的博客 - CSDN博客





2018年07月30日 10:45:20[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：95








##  STL queue**可以直接调函数就实现的功能是：**

**1.入队  push**

**2.出队 pop**

**3.返回队首元素 front**

**4.返回队尾元素 back**

**5.判空  size,empty**

**6.队列元素数  size**



## **STL queue可以手动实现的功能是：**

**1.查询**

**2.删除**

**3.遍历**



```cpp
#include<iostream>
using namespace std;
#include<queue>

int main(void)
{
	queue<int> q;
	
//****************queue入队********************// 
	
	//push函数
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	
	
//****************queue队首元素********************// 
	cout<<"队首元素"<<endl;
	cout<<"queue.front  "<<q.front()<<endl; 
	cout<<endl<<endl;
	
//****************queue队尾元素********************// 
	cout<<"队尾元素"<<endl;
	cout<<"queue.back  "<<q.back()<<endl; 
	cout<<endl<<endl;	
	

//****************queue遍历元素********************//  
	
	//queue没有支持的遍历元素的方法，没有迭代器
	//如果要便利只能pop()和push()结合使用
	cout<<"遍历"<<endl; 
	int sz=q.size();
	for(int i=0;i<sz;i++)
	{
		int temp=q.front();
		q.pop();
		q.push(temp);
		cout<<temp<<endl;	
	} 

	cout<<endl<<endl;



//****************queue出队********************//  
	cout<<"当前队首出队"<<endl;
	q.pop();
	sz=q.size();
	for(int i=0;i<sz;i++)
	{
		int temp=q.front();
		q.pop();
		q.push(temp);
		cout<<temp<<endl;	
	} 

	cout<<endl<<endl;


//****************queue查询********************//  	

	//queue没有find函数,也不支持数组查询操作 
	cout<<"查询元素3"<<endl;
	sz=q.size(); 
	int flag=1;
	for(int i=0;i<sz;i++)
	{
		int temp=q.front();
		q.pop();
		q.push(temp);
		if(temp==3&&flag)
		{
			flag=0;
		} 	
	} 
	if(!flag)
	{
		cout<<"find it"<<endl;
	}
	else
	{
		cout<<"no"<<endl;
	}
	cout<<endl<<endl;
	
	
	
//****************queue判空********************//

	//empty和size函数判空
	if(q.size()==0)
	{
		cout<<"way:queue.size()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:queue.size()  "<<"nonempty"<<endl;
	}
	if(q.empty())
	{
		cout<<"way:queue.empty()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:queue.empty()  "<<"nonempty"<<endl;
	}
	cout<<endl<<endl; 

	return 0;	
}
```





