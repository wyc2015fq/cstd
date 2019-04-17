# Applese 的毒气炸弹（2019牛客寒假算法基础集训营 Day4-G） - Alex_McAvoy的博客 - CSDN博客





2019年02月03日 14:34:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：90








> 
# 【题目描述】

众所周知，Applese 是个很强的选手，它的化学一定很好。

今天他又AK了一套题觉得很无聊，于是想做个毒气炸弹玩。

毒气炸弹需要 k 种不同类型元素构成，Applese一共有 n 瓶含有这些元素的试剂。 

已知元素混合遵循 m 条规律，每一条规律都可以用 "x y c" 描述。

表示将第 x 瓶试剂混入第 y 瓶试剂或者把第 y 瓶试剂混入第 x 瓶试剂，需要消耗 c 的脑力。

特别地，除了这 m 条规律外，Applese 可以将任意两瓶相同元素的试剂混合，且不需要消耗脑力。

Applese 想要配出毒气炸弹，就需要使 S 中含有 1∼k 这 k 种元素。它想知道自己最少花费多少脑力可以把毒气炸弹做出来。

# 【输入描述】

第一行为三个整数 n, m, k 表示 Applese 拥有的试剂的数量，混合规律的数量和所需的元素种类数。

第二行为 n 个整数 a1,a2,…,an，分别表示每一瓶试剂的元素类型。

接下来m行，每行三个整数 x, y, c，含义如题目描述中所述。不保证 x, y的试剂种类不同。

1≤n,k,m≤10^5

1≤x,y≤n,x≠y

1≤c≤10^9

# 【输出描述】

输出一个正整数表示最小的耗费脑力。特别地，如果无法合成出毒气炸弹，输出 "-1"。

# 【样例】

示例1

输入

6 8 2

1 1 1 2 2 2

1 2 1

2 3 2

1 3 3

3 4 6

4 5 1

4 6 3

5 6 2

1 6 2

输出

2


思路：

题目比较绕，读懂题后其实并不难，看成一张图后，其实就是 n 个点 m 条边然后在 k 的限制内求最小生成树

# 【源代码】

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
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
const int MOD=1e9+7;
using namespace std;
struct Edge{
    int x,y;
    int cost;
    Edge(){}
    Edge(int x,int y,int cost):x(x),y(y),cost(cost){}
    bool operator < (const Edge &rhs)const{
        return rhs.cost>cost;
    }
}edge[N];
int n,m,k;
int a[N];
int father[N],num[N];
int Find(int x){
    if(father[x]==x)
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x==y)
        return;
        
    if(num[x]==num[y]){
        father[y]=x;
        num[x]++;
    }
    else if(num[x]<num[y])
        father[x]=y;
    else
        father[y]=x;

}
LL kruskal(){
    sort(edge,edge+m);

    LL res=0;
    int cnt=0;
    for(int i=0;i<m;i++){
        int x=edge[i].x;
        int y=edge[i].y;
        int cost=edge[i].cost;
        if(Find(x)!=Find(y)){
            Union(x,y);
            res+=(LL)cost;

            if(++cnt==k-1)
                break;
        }
    }
    return (cnt==k-1?res:-1);
}
int main(){
    cin>>n>>m>>k;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    for(int i=0;i<m;i++){
        int x,y,c;
        cin>>x>>y>>c;
        edge[i]=Edge(a[x],a[y],c);
    }

    memset(num,0,sizeof(num));
    for(int i=0;i<=n;i++)
        father[i]=i;

    cout<<kruskal()<<endl;
    return 0;
}
```





