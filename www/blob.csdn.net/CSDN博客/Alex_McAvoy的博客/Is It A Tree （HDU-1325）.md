# Is It A Tree?（HDU-1325） - Alex_McAvoy的博客 - CSDN博客





2018年05月23日 19:00:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43








> 
# Problem Description

A tree is a well-known data structure that is either empty (null, void, nothing) or is a set of one or more nodes connected by directed edges between nodes satisfying the following properties. 

There is exactly one node, called the root, to which no directed edges point. <br><br>Every node except the root has exactly one edge pointing to it. 

There is a unique sequence of directed edges from the root to each node. <br><br>For example, consider the illustrations below, in which nodes are represented by circles and edges are represented by lines with arrowheads. The first two of these are trees, but the last is not.

![](https://img-blog.csdn.net/20180523190228766)![](https://img-blog.csdn.net/20180523190221927)![](https://img-blog.csdn.net/20180523190213338)



In this problem you will be given several descriptions of collections of nodes connected by directed edges. For each of these you are to determine if the collection satisfies the definition of a tree or not.

# **Input**

The input will consist of a sequence of descriptions (test cases) followed by a pair of negative integers. Each test case will consist of a sequence of edge descriptions followed by a pair of zeroes Each edge description will consist of a pair of integers; the first integer identifies the node from which the edge begins, and the second integer identifies the node to which the edge is directed. Node numbers will always be greater than zero. 

# Output

For each test case display the line ``Case k is a tree." or the line ``Case k is not a tree.", where k corresponds to the test case number (they are sequentially numbered starting with 1). 

# Sample Input

**6 8 5 3 5 2 6 4**

**5 6 0 08 1 7 3 6 2 8 9 7 57 4 7 8 7 6 0 03 8 6 8 6 45 3 5 6 5 2 0 0-1 -1**

# Sample Output

**Case 1 is a tree.Case 2 is a tree.Case 3 is not a tree.**


———————————————————————————————————————————————————————

题意：给出每对点的链接情况，问你最后构成的是不是一棵树

思路

并查集无疑，由于判断最后生成的是否是树，仅需满足点数=边数+1即可，此外除了根结点的入度是0，其他所有结点的入度为1，再注意以下几点即可：

    1.森林不是树

    2.构成环路不是树

    3.空树是一棵树 

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#define INF 0x3f3f3f3f
#define N 1000001
#define MOD 2520
#define E 1e-12
using namespace std;
int father[N];
int main()
{
    int i=1;
    while(true)
    {
        int point=0,edge=0;
        bool flag=false;
        memset(father,0,sizeof(father));

        int x,y;
        while(scanf("%d%d",&x,&y)!=EOF&&x&&y)
        {
            if(x<0||y<0)//均为-1时，终止程序
                return 0;

            if(x==y)
                flag=true;

            if(father[y]-1==1)//判断是否有回路
                flag=true;

            if(father[x]==0)//统计点的个数
                point++;
            if(father[y]==0)
                point++;
            father[x]=1;
            father[y]=2;
            edge++;//统计边的条数
        }
        if(flag==false&&point==edge+1)//如果点数=边数+1，则生成一棵树
            printf("Case %d is a tree.\n",i++);
        else
            printf("Case %d is not a tree.\n",i++);
    }
    return 0;
}
```





