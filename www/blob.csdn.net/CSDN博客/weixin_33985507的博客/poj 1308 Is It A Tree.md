# poj 1308 Is It A Tree? - weixin_33985507的博客 - CSDN博客
2013年06月07日 14:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
树
题意：给一个有向图，判断是否为有根数树
这题应该来讲是个水题但是OJ将其归为星题，主要是容易WA，理清逻辑就可以了
注意:
有根树的判定，1.连通，2边数+1 = 点数 3.无环    由两个可以推到另一个
做这题紧紧抓住树的定义即可，另外处理一下下面的一些特殊情况
1.题意要求是树，不能是森林
2.空树也是树，题目一开始说了
3.数据中可能有自环，即1 1这种，这样就不是一棵树了
3.有重边，1 2 ， 1 2，同样不是树
4.有环，1 2 ， 2 1，同样不是树
5.横叉边  2 3 ， 4 3  ， 同样不是树
网上都说并查集，其实不用那么麻烦，就模拟一下即可
```
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
#define N 10010
#define M 1010
#define INF 0x3f3f3f3f
bool istree;
int Min,Max,node,edge,Count;
int inde[N];
int num[N],e[N][M];
bool used[N],vis[N];
inline int max(int a ,int b)
{
    return a > b ? a : b;
}
inline int min(int a , int b)
{
    return a < b ? a : b;
}
void dfs(int u)
{
    vis[u] = true;
    Count++;
    for(int i=0; i<num[u]; i++)
    {
        int v = e[u][i];
        if(vis[v]) //有环或横叉边
        { istree = false; return ;}
        dfs(v);
        if(!istree) return ;
    }
}
void solve()
{
    int cc = 0;
    if(edge + 1 != node) 
    { 
        istree = false; 
        return; 
    }
    istree = true;
    memset(vis,false,sizeof(vis));
    for(int i=Min; i<=Max; i++)
        if(used[i] && inde[i] == 0)
        {
            cc++; //一个连通分量
            dfs(i);
            if(!istree) break;
        }
    if(cc != 1) istree = false; //连通分量数不为1
}
int main()
{
    int cas = 0;
    int u,v;
    while(cin >> u >> v)
    {
        if(u == -1 && v == -1) break;
        if(u == 0 && v == 0) //空树
        {
            printf("Case %d is a tree.\n",++cas);
            continue;
        }
        memset(inde,0,sizeof(inde));
        memset(num,0,sizeof(num));
        memset(used,false,sizeof(used));
        Min = min(u,v);
        Max = max(u,v);
        node = 2; edge = 1;
        used[u] = used[v] = true;
        e[u][num[u]++] = v;
        inde[v]++;
        while(cin >> u >> v)
        {
            if(u == 0 && v == 0) break;
            inde[v]++; edge++;
            e[u][num[u]++] = v;
            Min = min(Min,u); Min = min(Min,v);
            Max = max(Max,u); Max = max(Max,v);
            if(!used[u]) node++ , used[u] = true;
            if(!used[v]) node++ , used[v] = true;
        }
        solve();
        if(istree)    printf("Case %d is a tree.\n",++cas);
        else        printf("Case %d is not a tree.\n",++cas);
    }
    return 0;
}
```
