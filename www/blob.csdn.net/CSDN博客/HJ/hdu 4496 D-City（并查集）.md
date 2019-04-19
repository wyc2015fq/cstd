# hdu  4496  D-City（并查集） - HJ - CSDN博客
2016年08月12日 21:06:30[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：305
Problem Description 
Luxer is a really bad guy. He destroys everything he met.  
One day Luxer went to D-city. D-city has N D-points and M D-lines. Each D-line connects exactly two D-points. Luxer will destroy all the D-lines. The mayor of D-city wants to know how many connected blocks of D-city left after Luxer destroying the first K D-lines in the input.  
Two points are in the same connected blocks if and only if they connect to each other directly or indirectly.
Input 
First line of the input contains two integers N and M.  
Then following M lines each containing 2 space-separated integers u and v, which denotes an D-line.  
Constraints:  
0 < N <= 10000  
0 < M <= 100000  
0 <= u, v < N. 
Output 
Output M lines, the ith line is the answer after deleting the first i edges in the input.
Sample Input 
5 10  
0 1  
1 2  
1 3  
1 4  
0 2  
2 3  
0 4  
0 3  
3 4  
2 4
Sample Output 
1  
1  
1  
2  
2  
2  
2  
3  
4  
5 
Hint
The graph given in sample input is a complete graph, that each pair of vertex has an edge connecting them, so there’s only 1 connected block at first.  
The first 3 lines of output are 1s  because  after  deleting  the  first  3  edges  of  the  graph,  all  vertexes  still  connected together.  
But after deleting the first 4 edges of the graph, vertex 1 will be disconnected with other vertex, and it became an independent connected block.  
Continue deleting edges the disconnected blocks increased and finally it will became the number of vertex, so the last output should always be N. 
解析：题目大意是，给定一个无向图，删除前k条边(1<=k<=m)，求联通分量的个数，用并查集合，反过来减去边的个数，然后计算独立集的个数，代码如下：
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int parent[100005],res[100005]; 
int find(int x)            //查找祖先结点 
{
    return parent[x]==x?x:(parent[x]=find(parent[x]));
}
struct node
{
    int x,y;
}s[100005];
int main()
{
    int n,m;
    while(~scanf("%d %d",&n,&m))
    {
        for(int i=0;i<n;i++)
          parent[i]=i;
        for(int i=0;i<m;i++)
          scanf("%d %d",&s[i].x,&s[i].y);
        res[m-1]=n;                     //假设所有的n个点都联通起来，每个点都有n-3条边 
        for(int i=m-1;i>0;i--)     //倒着去减边的个数 
        {
            int a=find(s[i].x);
            int b=find(s[i].y);
            if(a!=b)                   //合并a,b 
            { 
                parent[a]=b;
                res[i-1]=res[i]-1;        //如果祖先结点不一样，则需要减一赋值 
            }
            else
              res[i-1]=res[i];         //否则直接赋值 
        }
        for(int i=0;i<m;i++)
          printf("%d\n",res[i]);
    }
    return 0;
}
```
