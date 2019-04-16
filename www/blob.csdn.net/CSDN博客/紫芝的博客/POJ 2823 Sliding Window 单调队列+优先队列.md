# POJ 2823 Sliding Window 单调队列+优先队列 - 紫芝的博客 - CSDN博客





2018年04月19日 21:33:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：41








# **Sliding Window**
|**Time Limit:** 12000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 69712||**Accepted:** 19787|
|**Case Time Limit:** 5000MS| | |

**Description**

An array of size *n* ≤ 106 is given to you. There is a sliding window of size *k* which is moving from the very left of the array to the very right. You can only see the *k* numbers in the window. Each time the sliding window moves rightwards by one position. Following is an example: 

The array is [1 3 -1 -3 5 3 6 7], and *k* is 3.
|Window position|Minimum value|Maximum value|
|----|----|----|
|[1  3  -1] -3  5  3  6  7|-1|3|
|1 [3  -1  -3] 5  3  6  7|-3|3|
|1  3 [-1  -3  5] 3  6  7|-3|5|
|1  3  -1 [-3  5  3] 6  7|-3|5|
|1  3  -1  -3 [5  3  6] 7|3|6|
|1  3  -1  -3  5 [3  6  7]|3|7|

Your task is to determine the maximum and minimum values in the sliding window at each position. 

**Input**

The input consists of two lines. The first line contains two integers *n* and *k* which are the lengths of the array and the sliding window. There are *n* integers in the second line. 

**Output**

There are two lines in the output. The first line gives the minimum values in the window at each position, from left to right, respectively. The second line gives the maximum values. 

**Sample Input**

```
8 3
1 3 -1 -3 5 3 6 7
```

**Sample Output**

```
-1 -3 -3 -3 3 3
3 3 5 5 6 7
```

**Source**

[POJ Monthly--2006.04.28](http://poj.org/searchproblem?field=source&key=POJ+Monthly--2006.04.28), Ikki

# 用deque实现单调队列

1.求区间最大值时，建立从队头到队尾的单调递减的队列

2.求区间最小值时，建立从队头到队尾的单调递增的队列

若队列元素个数超过区间大小，则从队头删除元素



```cpp
#include<bits/stdc++.h>
using namespace std;
#define N 1000005
int a[N]; 
//int maxq[N],minq[N];
int n,k;
int main()
{
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=1;i<=n;i++)
	cin>>a[i];
	
	//int f=0,r=1;
	deque<int> dq;
	
		dq.clear();
		for(int i=1;i<=n;i++)
		{
			while(!dq.empty()&&(a[dq.back()]>a[i]||i-dq.back()+1>k))
			dq.pop_back();
			
			while(!dq.empty()&&i-dq.front()+1>k)
			dq.pop_front();
			
			dq.push_back(i);
			if(i>=k)	{
			cout<<a[dq.front()];
			if(i!=n)	cout<<" ";
			}
		}
		cout<<endl;
		
		dq.clear();
	for(int i=1;i<=n;i++)
	{
			while(!dq.empty()&&(a[dq.back()]<a[i]||i-dq.back()+1>k))
			dq.pop_back();
		
			while(!dq.empty()&&i-dq.front()+1>k)
			dq.pop_front();
			
			dq.push_back(i);
			if(i>=k)	{
			cout<<a[dq.front()];
			if(i!=n)	cout<<" ";
			}
		}
		cout<<endl;
	return 0;
}
```

# STL优先队列：

自定义比较运算符com_max，com_min；



```cpp
//poj2823 优先队列 
#include<iostream>
#include<queue>
using namespace std;
const int N=100005;
int a[N],out_min[N],out_max[N];
int cnt1=0,cnt2=0;
int n,k;

struct cmp_max{
	bool operator()(const int a1,const int a2)
	{
		return a[a1]>a[a2];
	}
};
struct cmp_min{
	bool operator()(const int a1,const int a2)
	{
		return a[a1]<a[a2];
	}
};
priority_queue<int,vector<int>,cmp_max> q1;
priority_queue<int,vector<int>,cmp_min> q2;

int main()
{
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=1;i<=n;i++)
	cin>>a[i];
	for(int i=1;i<=k;i++)
	{
		q1.push(i);
		q2.push(i);
	}
	out_max[cnt1++]=a[q1.top()];
	out_min[cnt2++]=a[q2.top()];
	for(int i=k+1;i<=n;i++)//向后移动 
	{
		q1.push(i);
		q2.push(i);
		while(i-q1.top()>=k)//如果队首元素过期，则删除队首元素
		q1.pop();
		out_max[cnt1++]=a[q1.top()] ;
		
		while(i-q2.top()>=k)
		q2.pop();
		out_min[cnt2++]=a[q2.top()];
		
	}
	for(int i=0;i<=(n-k);++i)
	printf("%d%c",out_max[i],(i<n-k)?' ':'\n');
	for(int i=0;i<=(n-k);++i)
	printf("%d%c",out_min[i],(i<n-k)?' ':'\n');
	return 0;
}
```









