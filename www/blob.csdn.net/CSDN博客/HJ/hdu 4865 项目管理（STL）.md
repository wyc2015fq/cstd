# hdu  4865  项目管理（STL） - HJ - CSDN博客
2017年03月31日 22:23:22[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：403
Problem Description 
我们建造了一个大项目！这个项目有n个节点，用很多边连接起来，并且这个项目是连通的！ 
两个节点间可能有多条边，不过一条边的两端必然是不同的节点。 
每个节点都有一个能量值。
现在我们要编写一个项目管理软件，这个软件呢有两个操作： 
1.给某个项目的能量值加上一个特定值。 
2.询问跟一个项目相邻的项目的能量值之和。（如果有多条边就算多次，比如a和b有2条边，那么询问a的时候b的权值算2次）。
Input 
第一行一个整数T(1 <= T <= 3),表示测试数据的个数。 
然后对于每个测试数据，第一行有两个整数n(1 <= n <= 100000)和m(1 <= m <= n + 10)，分别表示点数和边数。
然后m行，每行两个数a和b，表示a和b之间有一条边。 
然后一个整数Q。
然后Q行，每行第一个数cmd表示操作类型。如果cmd为0，那么接下来两个数u v表示给项目u的能量值加上v(0 <= v <= 100)。 
如果cmd为1，那么接下来一个数u表示询问u相邻的项目的能量值之和。
所有点从1到n标号。
Output 
对每个询问，输出一行表示答案。
Sample Input
1 
3 2 
1 2 
1 3 
6 
0 1 15 
0 3 4 
1 1 
1 3 
0 2 33 
1 2
Sample Output
4 
15 
15
用vector处理一下。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#include<vector>
#define N 100010 
using namespace std;
vector<int>G[N];
vector<vector<int> >c;
int a[N];
int main()
{
    int t,n,m,k,u,v,ans,p;
    scanf("%d",&t);
    while(t--)
    {
        memset(a,0,sizeof(a));
        scanf("%d%d",&n,&m);
        c.clear();
        c.resize(n*2);
        for(int i=0;i<m;i++)
        {
            scanf("%d%d",&u,&v);
            c[u].push_back(v);     //表示v在u的邻接点集合中 
            c[v].push_back(u);    //并且u也在v的邻接点集合中 
        }
        scanf("%d",&k);
        while(k--)
        {
            scanf("%d%d",&p,&u);
            if(p==0)
            {
                scanf("%d",&v);
                a[u]+=v;       //存储节点u的权值 
            }
            else
            {
                long long sum=0;
                for(int i=0;i<c[u].size();i++)   //寻找u所有的邻接点 
                  sum+=a[c[u][i]];
                printf("%lld\n",sum);
            }
        }
    }
    return 0;
}
```
