# hdu  4263  Red/Blue Spanning Tree（并查集） - HJ - CSDN博客
2016年08月18日 15:13:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：292
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------数据结构--------																[并查集](https://blog.csdn.net/feizaoSYUACM/article/category/6307988)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Given an undirected, unweighted, connected graph, where each edge is colored either blue or red, determine whether a spanning tree with exactly k blue edges exists.
Input 
There will be several test cases in the input. Each test case will begin with a line with three integers: 
n m k 
Where n (2≤n≤1,000) is the number of nodes in the graph, m (limited by the structure of the graph) is the number of edges in the graph, and k (0大于等于k小于n) is the number of blue edges desired in the spanning tree. 
Each of the next m lines will contain three elements, describing the edges: 
c f t 
Where c is a character, either capital ‘R’ or capital ‘B’, indicating the color of the edge, and f and t are integers (1≤f,t≤n, t≠f) indicating the nodes that edge goes from and to. The graph is guaranteed to be connected, and there is guaranteed to be at most one edge between any pair of nodes. 
The input will end with a line with three 0s.
Output 
For each test case, output single line, containing 1 if it is possible to build a spanning tree with exactly k blue edges, and 0 if it is not possible. Output no extra spaces, and do not separate answers with blank lines.
Sample Input
3 3 2 
B 1 2 
B 2 3 
R 3 1 
2 1 1 
R 1 2 
0 0 0
Sample Output
1 
0
解析：先要理解题意，用并查集算法，先算出用红边去构成生成树最多用x1条，那么蓝边最少就用n-1-x1，再算出用蓝边去构成生成树最多用x2条，那么蓝边最多就用x2，然后判断k是否介于之间即可，代码如下：
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int c1,c2,f1[1005],f2[1005];     
int find(int x,int *f)
{
    return f[x]==x?x:find(f[x],f);
} 
void Union(int a,int b,int &c,int *f)
{
    int x=find(a,f);
    int y=find(b,f);
    if(x!=y)
    {
        f[x]=y;
        c++;
    }
}
int main()
{
    int n,m,k,p,q;
    char s[10];
    while(~scanf("%d %d %d",&n,&m,&k))   //输入要用scanf,用cin可能超时 
    {
        if(n==0&&m==0&&k==0)
          break;
        c1=c2=0;
        for(int i=1;i<=n;i++)
          f1[i]=f2[i]=i;           //赋初值 
        for(int i=0;i<m;i++)
        {
            scanf("%s %d %d",&s,&p,&q);
            if(s[0]=='R')       //分别进行合并  
              Union(p,q,c1,f1);           //c1表示红边构成生成树最多用的条数 
            else
              Union(p,q,c2,f2);         //c2表示蓝边构成生成树最多用的条数 
        }
        if(n-1-c1<=k&&k<=c2)       //如果k值满足在蓝边构成生成树最少用n-1-c1条和最多用c2条的范围之内 
          printf("1\n");
        else
          printf("0\n");
    }
    return 0;
}
```
