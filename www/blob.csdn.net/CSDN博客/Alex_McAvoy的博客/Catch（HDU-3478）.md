# Catch（HDU-3478） - Alex_McAvoy的博客 - CSDN博客





2018年11月19日 22:04:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51








> 
# Problem Description

A thief is running away!

We can consider the city where he locates as an undirected graph in which nodes stand for crosses and edges stand for streets. The crosses are labeled from 0 to N–1. 

The tricky thief starts his escaping from cross S. Each moment he moves to an adjacent cross. More exactly, assume he is at cross u at the moment t. He may appear at cross v at moment t + 1 if and only if there is a street between cross u and cross v. Notice that he may not stay at the same cross in two consecutive moment.

The cops want to know if there’s some moment at which it’s possible for the thief to appear at any cross in the city.

# **Input**

The input contains multiple test cases:

In the first line of the input there’s an integer T which is the number of test cases. Then the description of T test cases will be given. 

For any test case, the first line contains three integers N (≤ 100 000), M (≤ 500 000), and S. N is the number of crosses. M is the number of streets and S is the index of the cross where the thief starts his escaping.

For the next M lines, there will be 2 integers u and v in each line (0 ≤ u, v < N). It means there’s an undirected street between cross u and cross v.

# Output

For each test case, output one line to tell if there’s a moment that it’s possible for the thief to appear at any cross. Look at the sample output for output format.

# Sample Input

**23 3 00 10 21 22 1 00 1**

# Sample Output

**Case 1: YESCase 2: NO**


题意： 给一 n 个点 m 条边的无向图，再给一个起点 x，从起点出发，每个单位时间只能从一点走到相邻的点上，现在问有没有一个时刻，可以在图的任意结点上。

思路：首先，图要是不连通的，则不存在这个时刻，其次，若图是二分图也不存在完美时刻（图被分为两半，一半是奇数时刻到达，一半偶数时刻到达）

因此，只有在图连通，且非二分图的时候，才能有时刻的存在。通过并查集判断连通，再判断图是否二分图即可

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
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
vector<int> G[N];
int color[N];
int father[N];
int Find(int x){
    if(father[x]==-1)
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[x]=y;
}
bool bipartite(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(color[x]==color[y])
            return false;
        if(color[y]==0){
            color[y]=3-color[x];
            if(!bipartite(y))
                return false;
        }
    }
    return true;
}
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        int n,m,s;
        scanf("%d%d%d",&n,&m,&s);

        memset(color,0,sizeof(color));
        memset(father,-1,sizeof(father));
        for(int i=0;i<n;i++)
            G[i].clear();

        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
            Union(x,y);
        }

        int ssc=0;
        for(int i=0;i<n;i++)
            if(Find(i)==i)
                ssc++;

        if(ssc>1)
            printf("Case %d: NO\n",Case++);
        else{
            bool flag=true;
            color[s]=1;
            if(bipartite(s))
                flag=false;
            if(flag)
                printf("Case %d: YES\n",Case++);
            else
                printf("Case %d: NO\n",Case++);
        }
    }
    return 0;
}
```





