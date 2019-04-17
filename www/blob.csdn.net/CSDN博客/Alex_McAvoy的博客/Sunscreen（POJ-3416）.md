# Sunscreen（POJ-3416） - Alex_McAvoy的博客 - CSDN博客





2018年06月06日 17:44:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47
个人分类：[POJ																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

To avoid unsightly burns while tanning, each of the C (1 ≤ C ≤ 2500) cows must cover her hide with sunscreen when they’re at the beach. Cow i has a minimum and maximum SPF rating (1 ≤ minSPFi ≤ 1,000; minSPFi ≤ maxSPFi ≤ 1,000) that will work. If the SPF rating is too low, the cow suffers sunburn; if the SPF rating is too high, the cow doesn’t tan at all……..

The cows have a picnic basket with L (1 ≤ L ≤ 2500) bottles of sunscreen lotion, each bottle i with an SPF rating SPFi (1 ≤ SPFi ≤ 1,000). Lotion bottle i can cover coveri cows with lotion. A cow may lotion from only one bottle.

What is the maximum number of cows that can protect themselves while tanning given the available lotions?

# **Input**

Line 1: Two space-separated integers: C and L

Lines 2..C+1: Line i describes cow i’s lotion requires with two integers: minSPFi and maxSPFi

Lines C+2..C+L+1: Line i+C+1 describes a sunscreen lotion bottle i with space-separated integers: SPFi and coveri

# Output

A single line with an integer that is the maximum number of cows that can be protected while tanning

# Sample Input

**3 2 **

**3 10 2 5 1 5 6 2 4 1 **

# Sample Output

**2**

———————————————————————————————————————————————————————

题意：c头牛晒太阳，每头牛有一个防晒范围，太低会晒伤，太高不能晒的充分，已知有L种防晒霜以及他们的数量与防晒范围，现给牛涂防晒霜，问有多少头牛能充分晒到太阳。 

思路

一开始只想到了用贪心算法，将牛按照防晒范围的最小值升序排序，对每一种防晒霜，找出上限大于他且下限小于他的牛跟他匹配，枚举所有防晒霜，将所有符合最小值小于该防晒霜的奶牛的最大值存储，然后记录答案。

**错误代码**

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
#define N 2501
#define MOD 1001
#define E 1e-12
using namespace std;
struct Node{
    int minn;
    int maxx;
}spf[N];
int num[1001];//存储每一种防晒霜的防晒范围
bool vis[1001];//判断防晒霜是否被用过
bool cmp(Node a,Node b)
{
    if(a.maxx==b.maxx)
        return a.minn>b.minn;
    return a.maxx<b.maxx;
}
int main()
{
    int c,l;
    scanf("%d%d\n",&c,&l);
    for(int i=1;i<=c;i++)//奶牛的防晒范围
        scanf("%d%d",&spf[i].minn,&spf[i].maxx);
    for(int i=1;i<=l;i++)//桶排存储每种防晒霜的防晒值
    {
        int x,y;
        scanf("%d%d",&x,&y);
        num[x]+=y;
    }

    int cnt=0;
    sort(spf+1,spf+c+1,cmp);
    for(int i=1;i<=1000;i++)//枚举每一种防晒霜
        for(int j=1;j<=num[i];j++)//枚举每一种防晒霜的防晒值
            for(int k=1;k<=c;k++)//枚举每一头牛
                if(spf[k].minn<=i&&i<=spf[k].maxx&&vis[k]==0)//如果当前的防晒霜未被用过且符合牛的防晒范围
                {
                    cnt++;
                    vis[k]=1;
                    break;
                }

    printf("%d\n",cnt);
    return 0;
}
```



WA多次，查看大量题解后发现需要用优先队列，将奶牛按照防晒范围的最小值升序排序，将防晒霜也按照防晒值升序排序，从最小的防晒霜枚举，将最小值小于等于该防晒霜的奶牛的最大值放入优先队列之中，然后优先队列最小值先出，就可将这些最大值中的最小的取出来，更新答案。



# Source Program

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
#define N 2501
#define MOD 1001
#define E 1e-12
using namespace std;
struct Cow
{
    int maxx;
    int minn;
}cow[N],cc;
struct Sunscreen
{
    int sum;
    int ans;
}sunscreen[N],ss;
priority_queue<int, vector<int>, greater<int> > q;
bool cmp1(Cow x,Cow y)
{
    return x.minn<y.minn;
}
bool cmp2(Sunscreen x,Sunscreen y)
{
    return x.ans<y.ans;
}
int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)//牛的防晒范围
        scanf("%d%d",&cow[i].minn,&cow[i].maxx);
    for(int i=1;i<=m;i++)//防晒霜的防晒值
        scanf("%d%d",&sunscreen[i].ans,&sunscreen[i].sum);

    sort(cow+1,cow+1+n,cmp1);//按牛的防晒下限升序排序
    sort(sunscreen+1,sunscreen+1+m,cmp2);//按防晒霜的防晒值升序排序

    int ans=0;
    int j=1;
    for(int i=1;i<=m;i++)
    {
        while(j<=n&&cow[j].minn<=sunscreen[i].ans)//从最小的防晒霜枚举，将最小值小于等于该防晒霜的奶牛的最大值放入优先队列
        {
            q.push(cow[j].maxx);
            j++;
        }
        while(sunscreen[i].sum!=0&&!q.empty())//优先队列最小值先出，可将所有最大值中的最小值取出，更新答案
        {
            int k=q.top();
            q.pop();
            if(k>=sunscreen[i].ans)
            {
                ans++;
                sunscreen[i].sum--;
            }
        }
    }
    printf("%d\n",ans);
    return 0;
}
```






