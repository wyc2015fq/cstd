# Sonya and Ice Cream（CF-1004E） - Alex_McAvoy的博客 - CSDN博客





2018年08月28日 16:47:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：71
个人分类：[CodeForces																[数据结构——树与二叉树](https://blog.csdn.net/u011815404/article/category/7813122)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Sonya likes ice cream very much. She eats it even during programming competitions. That is why the girl decided that she wants to open her own ice cream shops.

Sonya lives in a city with n junctions and n−1 streets between them. All streets are two-way and connect two junctions. It is possible to travel from any junction to any other using one or more streets. City Hall allows opening shops only on junctions. The girl cannot open shops in the middle of streets.

Sonya has exactly k friends whom she can trust. If she opens a shop, one of her friends has to work there and not to allow anybody to eat an ice cream not paying for it. Since Sonya does not want to skip an important competition, she will not work in shops personally.

Sonya wants all her ice cream shops to form a simple path of the length r (1≤r≤k), i.e. to be located in different junctions f1,f2,…,fr and there is street between fi and fi+1 for each ii from 1 to r−1.

The girl takes care of potential buyers, so she also wants to minimize the maximum distance between the junctions to the nearest ice cream shop. The distance between two junctions aa and bb is equal to the sum of all the street lengths that you need to pass to get from the junction aa to the junction b. So Sonya wants to minimize

maxamin1≤i≤rda,fi


where a takes a value of all possible n junctions, fi — the junction where the ii-th Sonya's shop is located, and dx,y — the distance between the junctions x and y.

Sonya is not sure that she can find the optimal shops locations, that is why she is asking you to help her to open not more than k shops that will form a simple path and the maximum distance between any junction and the nearest shop would be minimal.

# Input

The first line contains two integers n and k (1≤k≤n≤105) — the number of junctions and friends respectively.

Each of the next n−1 lines contains three integers ui, vi, and di (1≤ui,vi≤n, vi≠ui, 1≤d≤104) — junctions that are connected by a street and the length of this street. It is guaranteed that each pair of junctions is connected by at most one street. It is guaranteed that you can get from any junctions to any other.

# Output

Print one number — the minimal possible maximum distance that you need to pass to get from any junction to the nearest ice cream shop. Sonya's shops must form a simple path and the number of shops must be at most k.

# Examples

**Input**

6 2

1 2 3

2 3 4

4 5 2

4 6 3

2 4 6

**Output**

4

**Input**

10 3

1 2 5

5 7 2

3 2 6

10 6 3

3 8 1

6 4 2

4 1 6

6 9 4

5 2 5

**Output**

7

# Note

In the first example, you can choose the path 2-4, so the answer will be 4.

![](https://img-blog.csdn.net/20180828164509166?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

In the second example, you can choose the path 4-1-2, so the answer will be 7.

![](https://img-blog.csdn.net/20180828164517226?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

————————————————————————————————————————————

题意：在树上找 k 个点，使树上每个点距能够找到的点距离最短

思路来源：[点击这里](https://blog.csdn.net/weixin_39399748/article/details/80996373)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<string.h>
#include<cmath>
#include<vector>
using namespace std;
 
int n,k,e,head[100010],r[100005],f1[100005],f2[100005],len;
vector<int> vec;
 
struct Edge{
    int to,cost,next;
}edge[200010];
 
void addedge(int a,int b, int c)
{
    edge[e].to=b;
    edge[e].cost=c;
    edge[e].next=head[a];
    head[a]=e++;
}
 
void dfs1(int u, int fa)
{
    r[u]=u;
    for (int i=head[u]; i!=-1; i=edge[i].next)
    {
        int v=edge[i].to;
        if (v==fa) continue;
        dfs1(v,u);
        if (f1[u]<f1[v]+edge[i].cost)
        {
            f1[u]=f1[v]+edge[i].cost; r[u]=r[v];
        }
    }
}
 
void dfs2(int u, int fa)
{
    r[u]=u;
    for (int i=head[u]; i!=-1; i=edge[i].next)
    {
        int v=edge[i].to;
        if (v==fa) continue;
        //cout<<"v="<<v<<endl;
        dfs2(v,u);
        if (f1[u]<=f1[v]+edge[i].cost)
        {
            f2[u]=f1[u];
            f1[u]=f1[v]+edge[i].cost;
            r[u]=v;
        }else if (f2[u]<f1[v]+edge[i].cost)
        {
            f2[u]=f1[v]+edge[i].cost;
        }
        //cout<<"u="<<u<<" f1="<<f1[u]<<" f2="<<f2[u]<<endl;
    }
}
 
bool fun(int d)
{
    //cout<<"d="<<d<<endl;
    bool tag=false; int num=0;
    for (int i=0; i<vec.size(); i++)
    {
        //cout<<"i="<<vec[i]<<" f1="<<f1[vec[i]]<<" len="<<len<<endl;
        if (!tag)
        {
            if (len-f1[vec[i]]<=d) continue;
            tag=true; num=2;
            if (k==1)
            {
                if (f1[vec[i-1]]>d || f2[vec[i-1]]>d) return false;
                return true;
            }else
            {
                if (f2[vec[i]]>d || f2[vec[i-1]]>d) return false;
                //cout<<"yes"<<endl;
                if (k==2 && f1[vec[i]]>d) return false;
                if (k==2) return true;
            }
        }else
        {
            num++;
            if (f2[vec[i]]>d) return false;
            if (num==k)
            {
                if (f1[vec[i]]>d) return false;
                return true;
            }
        }
    }
    return true;
}
 
long long solve()
{
    long long l=0; long long r=1e9;
    long long ans=-1;
    while (l<=r)
    {
        long long mid=(l+r)/2;
        if (fun(mid))
        {
            r=mid-1; ans=mid;
        }else
        {
            l=mid+1;
        }
    }
    return ans;
}
 
int main()
{
    scanf("%d%d",&n,&k);
    memset(head,-1,sizeof(head));
    e=0;
    for (int i=1; i<n; i++)
    {
        int a,b,c; scanf("%d%d%d",&a,&b,&c);
        addedge(a,b,c); addedge(b,a,c);
    }
    memset(f1,0,sizeof(f1));
    memset(f2,0,sizeof(f2));
    dfs1(1,0);
    int tmp=r[1];
    //cout<<"tmp="<<tmp<<endl;
    memset(f1,0,sizeof(f1));
    dfs2(tmp,0);
    len=f1[tmp];
    vec.push_back(tmp);
    while (tmp!=r[tmp])
    {
        tmp=r[tmp];
        vec.push_back(tmp);
    }
    if (k>vec.size())
    {
        int ans=0;
        for (int i=0; i<vec.size(); i++)
        {
            ans=max(ans,f2[vec[i]]);
        }
        cout<<ans<<endl;
        return 0;
    }
    cout<<solve();
    return 0;
}
```






