# HDU 1325 Is It A Tree? 并查集 - 紫芝的博客 - CSDN博客





2018年04月07日 14:14:06[紫芝](https://me.csdn.net/qq_40507857)阅读数：42








# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1325)

# Is It A Tree?

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 29087    Accepted Submission(s): 6698****

**Problem Description**

A tree is a well-known data structure that is either empty (null, void, nothing) or is a set of one or more nodes connected by directed edges between nodes satisfying the following properties. 

There is exactly one node, called the root, to which no directed edges point. 


Every node except the root has exactly one edge pointing to it. 


There is a unique sequence of directed edges from the root to each node. 


For example, consider the illustrations below, in which nodes are represented by circles and edges are represented by lines with arrowheads. The first two of these are trees, but the last is not.

![](http://acm.hdu.edu.cn/data/images/1325-1.gif)![](http://acm.hdu.edu.cn/data/images/1325-2.gif)![](http://acm.hdu.edu.cn/data/images/1325-3.gif)
In this problem you will be given several descriptions of collections of nodes connected by directed edges. For each of these you are to determine if the collection satisfies the definition of a tree or not. 

**Input**

The input will consist of a sequence of descriptions (test cases) followed by a pair of negative integers. Each test case will consist of a sequence of edge descriptions followed by a pair of zeroes Each edge description will consist of a pair of integers; the first integer identifies the node from which the edge begins, and the second integer identifies the node to which the edge is directed. Node numbers will always be greater than zero. 

**Output**

For each test case display the line ``Case k is a tree." or the line ``Case k is not a tree.", where k corresponds to the test case number (they are sequentially numbered starting with 1). 

**Sample Input**

6 8 5 3 5 2 6 4 5 6 0 0 

8 1 7 3 6 2 8 9 7 5 7 4 7 8 7 6 0 0

3 8 6 8 6 4 5 3 5 6 5 2 0 0 

-1 -1

**Sample Output**

Case 1 is a tree. Case 2 is a tree. Case 3 is not a tree.

**Source**

[North Central North America 1997](http://acm.hdu.edu.cn/search.php?field=problem&key=North+Central+North+America+1997&source=1&searchmode=source)

**Recommend**

Ignatius.L   |   We have carefully selected several similar problems for you:  [1272](http://acm.hdu.edu.cn/showproblem.php?pid=1272)[1856](http://acm.hdu.edu.cn/showproblem.php?pid=1856)[1232](http://acm.hdu.edu.cn/showproblem.php?pid=1232)[1213](http://acm.hdu.edu.cn/showproblem.php?pid=1213)[1875](http://acm.hdu.edu.cn/showproblem.php?pid=1875)

### 题意：

给n对点，每两点之间有一条边，判断这些点是否可以构成一棵树

### 思路一：

1.无环

2.除了根，所有的入度为 1，根的入度为 0

3.树只有一个根，否则是森林

4.空树也是树

```cpp
#include<bits/stdc++.h >
using namespace std;
int a,b;
int f[100005];
int main()
{
	int t=1;
	while(1)
	{
		int edge=0;//树的边的个数 
		int node=0;//节点的个数 
		bool flag=0;//是否有回路 
		memset(f,0,sizeof(f));
		while(scanf("%d%d",&a,&b)&&a&&b)
		{
			if(a<0||b<0)
			return 0;
			if(f[b]-1==1)//判断是否有回路 
			flag=1;
			if(f[a]==0)
			node++;
			if(f[b]==0)
			node++;
			f[a]=1;f[b]=2;edge++;
		}
		//树的节点个数等于边的个数加一 
		if(flag==0&&node==edge+1)
		printf("Case %d is a tree.\n",t++);  
            else printf("Case %d is not a tree.\n",t++); 
	}
}
```

### 思路二：并查集



```cpp
#include <iostream>
#include<cmath>
#include<cstdio>
#include<cstring>
#define N 11000
using namespace std;
int pre[N],vis[N],flag,maxn;
void init()
{
    for(int i=0;i<N;i++)
        pre[i]=i;
    memset(vis,0,sizeof(vis));
    maxn=0;
    flag=1;
}

int find(int x)
{
    if(pre[x]==x)   return x;
    return pre[x]=find(pre[x]);
}

void unions(int u,int v)
{
    int t1=find(u);
    int t2=find(v);
    if(v!=t2||t1==t2)   flag=0;
    if(t1!=t2)  pre[t2]=t1;
}

int main()
{
    int kase=0;
    int u,v;
    init();
    while(~scanf("%d%d",&u,&v)&&u>=0&&v>=0)
    {
        if(u==0&&v==0)
        {
            int tp=0;
            for(int i=1;i<=maxn;i++)//寻找根节点
                if(vis[i]&&i==pre[i]) tp++;
            if(tp<=1&&flag)
                printf("Case %d is a tree.\n",++kase);
            else
                printf("Case %d is not a tree.\n",++kase);
            init();
            continue;
        }
        maxn=max(maxn,max(u,v));
        vis[u]=vis[v]=1;
        unions(u,v);
    }
}
```







