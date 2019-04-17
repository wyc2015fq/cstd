# ACM Huffman - qq_37385726的博客 - CSDN博客





2018年05月23日 21:32:47[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：41
个人分类：[ACM](https://blog.csdn.net/qq_37385726/article/category/7683299)









![](https://img-blog.csdn.net/20180523212844723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





**·文章长度：不需要记录每一个叶子节点的高度，即不需要通过height1*wieght1+....这种方式来求**

**·可以通过不断相加，eg 1+2+3+3+4+6即1+2+（1+2）+3+4+（1+2+3）**

**·使用优先队列来实现：priority_queue<int, vector<int>,greator<int> >**

**    其中**greator<int>是从小到大；less<int> 是从大到小****



```cpp
#include<iostream>
using namespace std;
#include<queue>


int main(void)
{
	int n;
	while(~scanf("%d",&n))
	{
		priority_queue<int,vector<int>,greater<int>  > pq;
		int temp;
		int total=0;
		for(int i=0;i<n;i++)
		{
			scanf("%d",&temp);
			pq.push(temp);
		}
		if(pq.size()==1)
		{
			total=temp;
		}
		while(pq.size()!=1)
		{
			int temp1=pq.top();
			pq.pop();
			int temp2=pq.top();
			pq.pop();
			total+=temp1+temp2;
			pq.push(temp1+temp2);
		}
		printf("%d\n",total);
	}
	return 0;
}
```







