# HDU1599 find the mincost route  Floyd算法求最小环 - 紫芝的博客 - CSDN博客





2018年10月09日 15:57:09[紫芝](https://me.csdn.net/qq_40507857)阅读数：30
个人分类：[C/C++基础知识](https://blog.csdn.net/qq_40507857/article/category/7565821)









## find the mincost route

杭州有N个景区，景区之间有一些双向的路来连接，现在8600想找一条旅游路线，这个路线从A点出发并且最后回到A点，假设经过的路线为V1,V2,....VK,V1,那么必须满足K>2,就是说至除了出发点以外至少要经过2个其他不同的景区，而且不能重复经过同一个景区。现在8600需要你帮他找一条这样的路线，并且花费越少越好。 

Input

第一行是2个整数N和M（N <= 100, M <= 1000)，代表景区的个数和道路的条数。 

接下来的M行里，每行包括3个整数a,b,c.代表a和b之间有一条通路，并且需要花费c元(c <= 100)。

Output

对于每个测试实例，如果能找到这样一条路线的话，输出花费的最小值。如果找不到的话，输出"It's impossible.".

Sample Input

3 3
1 2 1
2 3 1
1 3 1
3 3
1 2 1
1 2 3
2 3 1
Sample Output

3
It's impossible.

```cpp
#include <iostream>
#include<string.h>
#include<cstdio>
#include <stack>
#include <vector>
using namespace std;
#define INF 0x3f3f3f3f
typedef long long ll;
const int maxn=510;
ll mp[maxn][maxn];//邻接矩阵存图
ll d[maxn][maxn];//保存两点之间的最短距离
int n,m;
ll MIN=INF;
void floyd()
{
    //一个环至少要3个互不相同的点，所以要保证k大于i，i大于j
    for(int k=1;k<=n;k++){
        for(int i=1;i<k;i++)
            for(int j=i+1;j<k;j++)
           MIN=min(d[i][j]+mp[i][k]+mp[k][j],MIN);
            //得到从i出发，再回到i的最小环

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)//得到i,两点的最短路径
            d[i][j]=min(d[i][k]+d[k][j],d[i][j]);
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
            if(i==j)
            mp[i][j]=d[i][j]=0;
        else mp[i][j]=d[j][i]=INF;

        MIN=INF;
        for(int i=0;i<m;i++)
        {
            ll u,v,w;
            scanf("%lld%lld%lld",&u,&v,&w);
            mp[u][v]=mp[v][u]=min(mp[u][v],w);
            d[u][v]=d[v][u]=mp[u][v];
        }
    floyd();
    if(MIN==INF)
        printf("It's impossible.\n");
    else
         printf("%lld\n",MIN);
    }
}
```

### [无穷大 0x7ffffff 0x3f3f3f3f](https://blog.csdn.net/qq_40507857/article/details/80039732)

```cpp
#include <iostream>
#include<string.h>
#include<cstdio>
#include <stack>
#include <vector>
using namespace std;
#define INF 0x7ffffff
const int maxn=510;
int mp[maxn][maxn];
int d[maxn][maxn];
int n,m;
int MIN=INF;
void floyd()
{
    for(int k=1;k<=n;k++){
        for(int i=1;i<k;i++)
            for(int j=i+1;j<k;j++)
           MIN=min(d[i][j]+mp[i][k]+mp[k][j],MIN);

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
            d[i][j]=min(d[i][k]+d[k][j],d[i][j]);
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
            if(i==j)
            mp[i][j]=d[i][j]=0;
        else mp[i][j]=d[j][i]=INF;

        MIN=INF;
        for(int i=0;i<m;i++)
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            mp[u][v]=mp[v][u]=min(mp[u][v],w);
            d[u][v]=d[v][u]=mp[u][v];
        }
    floyd();
    if(MIN==INF)
        printf("It's impossible.\n");
    else
         printf("%d\n",MIN);
    }
}
```





