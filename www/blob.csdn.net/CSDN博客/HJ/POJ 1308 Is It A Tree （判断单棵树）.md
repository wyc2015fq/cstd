# POJ  1308 Is It A Tree?（判断单棵树） - HJ - CSDN博客
2017年01月17日 02:42:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：317
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------数据结构--------																[并查集](https://blog.csdn.net/feizaoSYUACM/article/category/6307988)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description 
A tree is a well-known data structure that is either empty (null, void, nothing) or is a set of one or more nodes connected by directed edges between nodes satisfying the following properties. 
![这里写图片描述](http://poj.org/images/1308_1.jpg)
There is exactly one node, called the root, to which no directed edges point. 
Every node except the root has exactly one edge pointing to it. 
There is a unique sequence of directed edges from the root to each node. 
For example, consider the illustrations below, in which nodes are represented by circles and edges are represented by lines with arrowheads. The first two of these are trees, but the last is not.
In this problem you will be given several descriptions of collections of nodes connected by directed edges. For each of these you are to determine if the collection satisfies the definition of a tree or not.
Input 
The input will consist of a sequence of descriptions (test cases) followed by a pair of negative integers. Each test case will consist of a sequence of edge descriptions followed by a pair of zeroes Each edge description will consist of a pair of integers; the first integer identifies the node from which the edge begins, and the second integer identifies the node to which the edge is directed. Node numbers will always be greater than zero.
Output 
For each test case display the line “Case k is a tree.” or the line “Case k is not a tree.”, where k corresponds to the test case number (they are sequentially numbered starting with 1).
Sample Input
6 8  5 3  5 2  6 4 
5 6  0 0
8 1  7 3  6 2  8 9  7 5 
7 4  7 8  7 6  0 0
3 8  6 8  6 4 
5 3  5 6  5 2  0 0 
-1 -1
Sample Output
Case 1 is a tree. 
Case 2 is a tree. 
Case 3 is not a tree.
Source 
North Central North America 1997
题目大意：给出若干组数据（a，b），表示有一条边从a指向b，每一组数据以（0,0）结束，多组数据以（-1，-1）结束。要求判断所给的数据是否满足单棵树（有且只有一棵树，所有点都在这一棵树上，且必须不能有环存在）
解法： 
注意判断两点（1）根节点有且只有一个（2）根节点之外的任何节点都只有一个入度
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int N=105;
int parent[N],vis[N];
void first_clear()     //初始化 
{
    for(int i=1;i<N;i++)
      parent[i]=i;
    memset(vis,0,sizeof(vis));
}
int find(int x)
{
    return parent[x]==x?x:find(parent[x]);
}
void Union(int x,int y)
{
    x=find(x);
    y=find(y);
    if(x==y)
      return ;
    parent[y]=x;
}
int findroot(int x)      //判断根是否唯一 
{
    int i=1;
    while(i<=x&&!vis[i])
      i++;
    int root=find(i);
    while(i<=x)
    {
        if(vis[i]&&find(i)!=root)
          return 0;
        i++;
    }
    return 1;
}
int main()
{
    int a,b,blag=0,maxn=0,tt=1;
    first_clear();
    while(scanf("%d%d",&a,&b)!=EOF)
    {
        if(a<0||b<0)
          break;
        if(a==0&&b==0)
        {
            if(!blag&&findroot(maxn))
              printf("Case %d is a tree.\n", tt++);
            else
              printf("Case %d is not a tree.\n", tt++);  
            blag=0;
            maxn=0;
            first_clear();
            continue;
        }
        if(blag)                 //如果已经不能构成一棵树的话，下面步骤可以省略 
          continue;
        maxn=a>maxn?a:maxn;
        maxn=b>maxn?b:maxn;
        vis[a]=vis[b]=1;
        if(find(a)==find(b))      //如果两者有共同祖先，并且还属于父子关系的话，就无法形成树 
          blag=1; 
        Union(a,b);
    }
    return 0;
}
```
