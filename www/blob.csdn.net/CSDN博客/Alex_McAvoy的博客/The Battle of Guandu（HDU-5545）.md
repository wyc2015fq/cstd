# The Battle of Guandu（HDU-5545） - Alex_McAvoy的博客 - CSDN博客





2019年01月20日 18:20:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[HDU																[图论——Dijkstra](https://blog.csdn.net/u011815404/article/category/8312429)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

In the year of 200, two generals whose names are Cao Cao and Shao Yuan are fighting in Guandu. The battle of Guandu was a great battle and the two armies were fighting at M different battlefields whose numbers were 1 to M. There were also N villages nearby numbered from 1 to N. Cao Cao could train some warriors from those villages to strengthen his military. For village i, Cao Cao could only call for some number of warriors join the battlefield xi. However, Shao Yuan's power was extremely strong at that time. So in order to protect themselves, village i would also send equal number of warriors to battlefield yi and join the Yuan Shao's Army. If Cao Cao had called for one warrior from village i, he would have to pay ci units of money for the village. There was no need for Cao Cao to pay for the warriors who would join Shao Yuan's army. At the beginning, there were no warriors of both sides in every battlefield. 

As one of greatest strategist at that time, Cao Cao was considering how to beat Shao Yuan. As we can image, the battlefields would have different level of importance wiwi. Some of the battlefields with wi=2 were very important, so Cao Cao had to guarantee that in these battlefields, the number of his warriors was greater than Shao Yuan's. And some of the battlefields with wi=1 were not as important as before, so Cao Cao had to make sure that the number of his warriors was greater or equal to Shao Yuan's. The other battlefields with wi=0 had no importance, so there were no restriction about the number of warriors in those battlefields. Now, given such conditions, could you help Cao Cao find the least number of money he had to pay to win the battlefield?

# Input

The first line of the input gives the number of test cases, T(1≤T≤30). T test cases follow. 

Each test case begins with two integers N and M(1≤N,M≤105) in one line. 

The second line contains N integers separated by blanks. The ithith integer xi(1≤xi≤M) means Cao Cao could call for warriors from village ii to battlefield xi. 

The third line also contains N integers separated by blanks. The ithith integer yi(1≤yi≤M) means if Cao Cao called some number of warriors from village i, there would be the same number of warriors join Shao Yuan's army and fight in battlefield yi. 

The next line contains N integers separated by blanks. The ithith integer ci(0≤ci≤105) means the number of money Cao Cao had to pay for each warrior from this village. 

The last line contains M integers separated by blanks. The ith number wi(wi∈0,1,2) means the importance level of ith battlefield.

# Output

For each test case, output one line containing Case #x: y, where x is the test case number (starting from 1) and y is the least amount of money that Cao Cao had to pay for all the warriors to win the battle. If he couldn't win, y=−1.

# **Sample Input**

**22 32 31 11 10 1 21 11112**

# Sample Output

**Case #1: 1Case #2: -1**


题意：n 个村庄 m 个战场，对于村庄 i，曹操可支付 c[i]*num 元，使这个村庄派 num 个士兵去战场 x[i] 为曹操作战，同时这个村庄也会派 num 个士兵去战场 y[i] 为袁绍作战，对于每个战场，有三种重要度 0、1、2，重要度为 0 表示该战场对双方士兵数没有要求，重要度为 1 表示该战场曹操的士兵数必须不小于袁绍的士兵数，重要度为 2 表示该战场曹操士兵数必须大于袁绍士兵数，求曹操若想赢得战争最少支付的钱数，若不可能赢则输出 -1

思路：从贪心的思路来说，最省钱的策略是不考虑重要度为 0 的战场，对重要度为 1 的战场使曹操的士兵数等于袁绍的士兵数，对重要度为 2 的战场使曹操的士兵数等于袁绍的士兵数 +1

考虑从曹操的战场向袁绍的战场连边，对于重要度为 2 的战场，给该战场 A 曹操的士兵数量 +1，则对应的战场 B 袁绍的士兵数+1，为满足要求，再像战场 B 的曹操的士兵数+1，则 B 又经过某边转移到战场 C，以此类推，直到转移某个重要度为 0 的战场，从而完成对战场 A 的分配

以如上的思路去枚举重要度为 2 的战场，进行转移，寻找重要度为 0 的战场的最短路径，记录答案，然后累加答案即可

可以看出问题的实质是一个多源多汇最短路问题，其中源点是重要度为 2 的战场，汇点是重要度为 0 的战场，此外，由于数据量过大，因此可以将边反向，将重要度为 0 的点作为起点进行初始化，求到重要度为 2 的点的最短路，最后累加答案即可

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
#define N 50001
#define LL long long
using namespace std;
struct Edge{
    int from,to;
    int dis;
    Edge(int from,int to,int dis):from(from),to(to),dis(dis){}
};
struct Node {
    int dis;
    int x;
    Node(int dis,int x):dis(dis),x(x) {}
    bool operator < (const Node &rhs) const {
        return dis>rhs.dis;
    }
};
int a[N],b[N],c[N],w[N];
bool vis[N];
LL dis[N];
vector<Edge> G[N];
priority_queue<Node> Q;
LL dijkstra(int n){
    memset(vis,false,sizeof(vis));

    for(int i=1;i<=n;i++){//初始化
        if(w[i]==0){//重要度为0
            Q.push(Node(0,i));
            dis[i]=0;
        }
        else
            dis[i]=INF;
    }

    while(!Q.empty()) {
        Node x=Q.top();Q.pop();

        int u=x.x;
        if(vis[u])
            continue;
        vis[u]=true;
        for(int i=0;i<G[u].size();i++) {
            Edge &e=G[u][i];
            if(dis[e.to]>dis[e.from]+e.dis) {
                dis[e.to]=dis[e.from]+e.dis;
                Q.push(Node(dis[e.to],e.to));
            }
        }
    }


    LL res=0;
    for(int i=1;i<=n;i++){
        if(w[i]==2){
            if(dis[i]==INF)
                return -1;
            res+=dis[i];
        }
    }

    return res;
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);

        for(LL i=1;i<=m;i++)
            G[i].clear();

        for(LL i=1;i<=n;i++)
            scanf("%d",&a[i]);
        for(LL i=1;i<=n;i++)
            scanf("%d",&b[i]);
        for(LL i=1;i<=n;i++)
            scanf("%d",&c[i]);
        for(LL i=1;i<=m;i++)
            scanf("%d",&w[i]);

        for(LL i=1;i<=n;i++)//反向建边
            G[b[i]].push_back(Edge(b[i],a[i],c[i]));

        LL res=dijkstra(m);

        printf("Case #%d: %lld\n",Case++,res);
    }
    return 0;
}
```






