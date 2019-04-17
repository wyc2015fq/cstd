# Haybale Guessing （POJ-3657） - Alex_McAvoy的博客 - CSDN博客





2018年05月29日 20:25:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：76
个人分类：[POJ																[数据结构——并查集](https://blog.csdn.net/u011815404/article/category/8376617)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The cows, who always have an inferiority complex about their intelligence, have a new guessing game to sharpen their brains.

A designated 'Hay Cow' hides behind the barn and creates N (1 ≤ N ≤ 1,000,000) uniquely-sized stacks (conveniently numbered 1..N) of hay bales, each with 1..1,000,000,000 bales of hay.

The other cows then ask the Hay Cow a series of Q (1 ≤ Q ≤ 25,000) questions about the the stacks, all having the same form:

What is the smallest number of bales of any stack in the range of stack numbers Ql..Qh (1 ≤ Ql ≤ N; Ql ≤ Qh ≤ N)?The Hay Cow answers each of these queries with a single integer A whose truthfulness is not guaranteed.

Help the other cows determine if the answers given by the Hay Cow are self-consistent or if certain answers contradict others.

# **Input**

Line 1: Two space-separated integers: N and Q

Lines 2..Q+1: Each line contains three space-separated integers that represent a single query and its reply: Ql, Qh, and A

# Output

Line 1: Print the single integer 0 if there are no inconsistencies among the replies (i.e., if there exists a valid realization of the hay stacks that agrees with all Q queries). Otherwise, print the index from 1..Q of the earliest query whose answer is inconsistent with the answers to the queries before it.

# Sample Input

**20 4**

**1 10 75 19 73 12 811 15 12**

# Sample Output

**3**


———————————————————————————————————————————————————————

题意：给一段长度为n，每个位置上的数都不同的序列a[1..n]和问答(xi, yi, ri)代表RMQ(a, x, y)=r，求给出最早的有矛盾的那个问答的编号。

思路：

有矛盾的情况有两种，一是之前已更新区间最小值为x，又要更新此区间或他的子区间的最小值为更小的数；二是两段区间的最小值相同，但没有交集。

先用并查集合并再查找有矛盾的情况，由于要求最小的序号，采用二分查找，从1枚举到m。

# **Source Program**

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10000001
#define MOD 123
#define E 1e-6
using namespace std;
struct Node
{
    int x;
    int y;
    int minn;
}a[N],b[N];
int father[N];
int ans;
int n,m;
int read()
{
    int x=0,f=1;
    char ch=getchar();
    for(;!isdigit(ch);ch=getchar())
        if(ch=='-')
            f=-1;
    for(;isdigit(ch);ch=getchar())
        x=(x*2)+(x*8)+ch-'0';
    return x*f;
}
int Find(int x)
{
    if(father[x]==x)
        return x;
    else
    return father[x]=Find(father[x]);
}
bool cmp(Node a,Node b)
{
    return a.minn>b.minn;
}
bool check(int pos)
{
    for(int i=1;i<=n+1;i++)
       father[i]=i;
    for(int i=1;i<=pos;i++)
       b[i]=a[i];
    sort(b+1,b+pos+1,cmp);

    int lmin=b[1].x,lmax=b[1].x,rmin=b[1].y,rmax=b[1].y;
    for(int i=2;i<=pos;i++)
    {
        if(b[i].minn<b[i-1].minn)//情况一
        {
            if(Find(lmax)>rmin)
                return true;
            for(int j=lmin;j<=rmax;j++)
            {
                j=Find(j);
                father[j]=Find(rmax+1);
            }
            lmax=lmin=b[i].x;
            rmax=rmin=b[i].y;
        }
        else if(b[i].minn==b[i-1].minn)//情况二
        {
             lmax=max(lmax,b[i].x);
             lmin=min(lmin,b[i].x);
             rmax=max(rmax,b[i].y);
             rmin=min(rmin,b[i].y);
             if(lmax>rmin)
                return true;
        }
    }
    if(Find(lmax)>rmin)
        return true;
    else
        return false;
}
int main()
{
    n=read();
    m=read();
    for(int i=1;i<=m;i++)
    {
        a[i].x=read();
        a[i].y=read();
        a[i].minn=read();
    }

    int l=1,r=m,mid;
    while(r-l>1)
    {
        int mid=(l+r)/2;
        if(check(mid))
        {
            ans=mid;
            r=mid;
        }
        else
            l=mid;
    }
    printf("%d\n",ans);
    return 0;
}
```






