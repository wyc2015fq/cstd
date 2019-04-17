# STL stack - qq_37385726的博客 - CSDN博客





2018年07月30日 12:17:37[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：16标签：[STL																[stac](https://so.csdn.net/so/search/s.do?q=stac&t=blog)](https://so.csdn.net/so/search/s.do?q=STL&t=blog)
个人分类：[STL](https://blog.csdn.net/qq_37385726/article/category/7888287)








## STL queue**可以直接调函数就实现的功能是：**

**1.入栈  push**

**2.出栈 pop**

**3.返回栈顶元素 top**

**5.判空  size,empty**

**6.栈元素数  size**



## **STL queue可以手动实现的功能是：**

**1.查询**

**2.删除**

**3.遍历**

**4.清空**





```cpp
#include<iostream>
using namespace std;
#include<stack>


int main(void)
{
	stack<int> st;

//****************stack入栈********************// 
	st.push(1);
	st.push(2);
	st.push(3);
	
//****************stack栈顶元素********************// 
	cout<<"栈顶 "<<st.top()<<endl;
	cout<<endl<<endl;

//****************stack遍历********************// 

	cout<<"和queue一样不支持遍历，不支持迭代器，不支持数组操作"<<endl;
	cout<<"只能使用push和pop函数来实现遍历，这里就不操作了"<<endl;
	cout<<endl<<endl;

//****************stack出栈********************// 
	st.pop();
	cout<<"此时栈顶  "<<st.top()<<endl;
	cout<<endl<<endl;
	
	
	
//****************stack判空********************// 


	//empty和size函数判空
	if(st.size()==0)
	{
		cout<<"way:stack.size()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:stack.size()  "<<"nonempty"<<endl;
	}
	if(st.empty())
	{
		cout<<"way:stack.empty()  "<<"empty"<<endl;
	}
	else
	{
		cout<<"way:stack.empty()  "<<"nonempty"<<endl;
	}
	cout<<endl<<endl; 
	
//****************stack清空********************// 
 
 	while(st.size())
	{
	 	st.pop();
	} 




	return 0;
}
```






