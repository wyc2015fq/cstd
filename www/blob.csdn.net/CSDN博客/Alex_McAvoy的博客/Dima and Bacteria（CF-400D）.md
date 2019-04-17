# Dima and Bacteria（CF-400D） - Alex_McAvoy的博客 - CSDN博客





2019年01月17日 20:09:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43








> 
# Problem Description

Dima took up the biology of bacteria, as a result of his experiments, he invented k types of bacteria. Overall, there are n bacteria at his laboratory right now, and the number of bacteria of type i equals ci. For convenience, we will assume that all the bacteria are numbered from 1 to n. The bacteria of type ci are numbered from ![](https://img-blog.csdnimg.cn/20190117104656109.jpg) to ![](https://img-blog.csdnimg.cn/20190117104705301.jpg).

With the help of special equipment Dima can move energy from some bacteria into some other one. Of course, the use of such equipment is not free. Dima knows m ways to move energy from some bacteria to another one. The way with number i can be described with integers ui, vi and xi mean that this way allows moving energy from bacteria with number ui to bacteria with number vi or vice versa for xi dollars.

Dima's Chef (Inna) calls the type-distribution correct if there is a way (may be non-direct) to move energy from any bacteria of the particular type to any other bacteria of the same type (between any two bacteria of the same type) for zero cost.

As for correct type-distribution the cost of moving the energy depends only on the types of bacteria help Inna to determine is the type-distribution correct? If it is, print the matrix d with size k × k. Cell d[i][j] of this matrix must be equal to the minimal possible cost of energy-moving from bacteria with type i to bacteria with type j.

# Input

The first line contains three integers n, m, k (1 ≤ n ≤ 105; 0 ≤ m ≤ 105; 1 ≤ k ≤ 500). The next line contains k integers c1, c2, ..., ck (1 ≤ ci ≤ n). Each of the next m lines contains three integers ui, vi, xi (1 ≤ ui, vi ≤ 105; 0 ≤ xi ≤ 104). It is guaranteed that ![](https://img-blog.csdnimg.cn/20190117104722131.jpg).

# Output

If Dima's type-distribution is correct, print string «Yes», and then k lines: in the i-th line print integers d[i][1], d[i][2], ..., d[i][k] (d[i][i] = 0). If there is no way to move energy from bacteria i to bacteria j appropriate d[i][j] must equal to -1. If the type-distribution isn't correct print «No».

# Examples

**Input**

4 4 2

1 3

2 3 0

3 4 0

2 4 1

2 1 2

**Output**

Yes

0 2

2 0

**Input**

3 1 2

2 1

1 2 0

**Output**

Yes

0 -1

-1 0

**Input**

3 2 2

2 1

1 2 0

2 3 1

**Output**

Yes

0 1

1 0

**Input**

3 0 2

1 2

**Output**

No


题意：给出 n 个点 m 条边 k 个集合以及 k 个集合的点数，满足每个集合任意两点间移动距离所需花费为 0，求任意两个集合移动所需的花费

思路：先对每个集合的点进行编号，然后用并查集判断是否满足同个集合任意两点距离为 0，如果满足，就用 Floyd 求传递闭包算出任意两个集合的点移动所需花费

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;
LL n,m,k;
LL G[1001][1001];
LL father[N];
LL Set[N],num[N];
LL Find(LL x){
    if(x!=father[x])
        father[x]=Find(father[x]);
    return father[x];
}
void Union(LL x,LL y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[y]=x;
}

bool judge()
{
    LL cnt=1;
    for(LL i=1;i<=k;i++){
        LL x=Find(cnt);
        for(LL j=1;j<=Set[i];j++)
            if(Find(cnt++)!=x)
                return false;
    }
    return true;
}

void floyd(){
    for(LL p=1;p<=k;p++)
        for(LL i=1;i<=k;i++)
            for(LL j=1;j<=k;j++)
                G[i][j]=min(G[i][j],G[i][p]+G[p][j]);
}

int main(){
    while(scanf("%lld%lld%lld",&n,&m,&k)!=EOF){
        memset(G,INF,sizeof(G));
        for(LL i=1;i<=k;i++){
            for(LL j=1;j<=k;j++){
                if(i==j){
                    G[i][j]=0;
                    G[j][i]=0;
                }
                else{
                    G[i][j]=INF;
                    G[j][i]=INF;
                }
            }
        }

        for(LL i=1;i<=n;i++)
            father[i]=i;

        LL cnt=1;
        for(LL i=1;i<=k;i++){
            scanf("%lld",&Set[i]);
            for(LL j=1;j<=Set[i];j++)//对集合进行分组
                num[cnt++]=i;
        }

        while(m--){
            LL x,y,w;
            scanf("%lld%lld%lld",&x,&y,&w);
            if(!w)
                Union(x,y);

            LL u=num[x],v=num[y];
            G[u][v]=min(G[u][v],w);
            G[v][u]=min(G[u][v],w);
        }

        if(!judge()){
            printf("No\n");
        }
        else{
            printf("Yes\n");
            floyd();
            for(LL i=1;i<=k;i++){//枚举集合
                for(LL j=1;j<=k;j++){
                    if(i==j)
                        G[i][j]=0;
                    if(G[i][j]==INF)//不能到达
                        G[i][j]=-1;
                    if(j<k)
                        printf("%lld ",G[i][j]);
                    else
                        printf("%lld",G[i][j]);
                }
                 printf("\n");
            }
        }
    }
    return 0;
}
```





