# BZOJ2131免费的馅饼  DP+树状数组 - 紫芝的博客 - CSDN博客





2018年08月18日 12:41:16[紫芝](https://me.csdn.net/qq_40507857)阅读数：92








## [免费的馅饼](https://www.lydsy.com/JudgeOnline/problem.php?id=2131)

## Description

![](https://www.lydsy.com/JudgeOnline/images/2131.jpg)

## Input

第一行是用空格隔开的二个正整数，分别给出了舞台的宽度W（1到10^8之间）和馅饼的个数n（1到10^5）。　　接下来n行，每一行给出了一块馅饼的信息。由三个正整数组成，分别表示了每个馅饼落到舞台上的时刻t[i]（1到10^8秒），掉到舞台上的格子的编号p[i]（1和w之间），以及分值v[i]（1到1000之间）。游戏开始时刻为0。输入文件中同一行相邻两项之间用一个空格隔开。输入数据中可能存在两个馅饼的t[i]和p[i]都一样。

## Output

一个数，表示游戏者获得的最大总得分。

## Sample Input

3 4

1 2 3

5 2 3

6 3 4

1 1 5

## Sample Output

12

【数据规模】

对于100%的数据，1<=w,t[i]<=10^8,1<=n<=100000。

题解：

用f[i][j]表示游戏者在第i时刻，第j位置上获得的最大价值，考虑将馅饼按时间从小到大排序，这样可以将二维数组降到一维，用f[i]表示到第i个馅饼，第i个必须选的最大值，然后枚举 i 到 i-1 所有的 j，如果它接到a[j]这个馅饼以后能在规定时间内跑到a[i]，就用f[j]来更新f[i]。

考虑什么样的 j 能作为决策点，对于当前点的a【i】和用作决策点的a【j】，时间限制是 a[i].t - a[j].t,它们之间的距离是abs（a[i].pos-a[j].pos）;跑过这段距离最少所需要的时间是![](https://img-blog.csdn.net/20180818123234757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。注意这里是向上取整，因为如果是5个格子的话它要用3个单位时间跑到而不是2个。

那么如果j可以作为i的决策点就可以列出一个不等式。为了消掉难搞的绝对值我们分两种情况讨论:

①当i的pos大于j的pos的时候，就是 ![](https://img-blog.csdn.net/20180818123145958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。把i的和j的分别拿到两边去。乘以2消掉上取整，然后再移项就有了一个非常好的式子：![](https://img-blog.csdn.net/20180818123328710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。

②当i的pos小于j的pos的时候，我们可以化出式子：![](https://img-blog.csdn.net/2018081812353319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。于是就可以给每一个a[i]预处理两个权值![](https://img-blog.csdn.net/20180818123641818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)，转化比较w1或w2就可以了。但是这个玩意怎么优化呢？如果直接这么看的话它的限制好像还是很多，还是没有办法搞。

当i的pos大于j的pos并且i的w1大于j的w1的时候，把w1加上两倍的pos就变成了w2，这个时候相当于是大的那一边加上较大的数字，小的那一边加上较小的数字，大小关系不变；当i的pos小于j的pos的时候证明方法是类似的。用后面的式子推出前面的那就很显然了。

那么只需要把所有馅饼按照w1为第一关键字，w2为第二关键字排序，然后用树状数组维护前缀最大值就可以了。注意作为下标的w2要离散化。

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
const int maxn=1e5+10;
int w,n;//舞台的宽度W（1到10^8之间）和馅饼的个数n（1到10^5）
int cnt;//数据量
int ans;//答案
int f[maxn];
int Hash[maxn];
int s[maxn];//树状数组

struct pies
{
    int t;//馅饼落到舞台上的时刻t[i]（1到10^8秒）
    int p;//掉到舞台上的格子的编号p[i]（1和w之间）
    int v;//以及分值v[i]（1到1000之间）
    int w1;//w1=2*t+pos
    int w2;//w2=2∗t−pos
};
pies a[maxn];
void init()
{
    cnt=0;
    ans=0;
    memset(f,0,sizeof(f));
    memset(Hash,0,sizeof(Hash));
    memset(s,0,sizeof(s));
    memset(a,0,sizeof(a));
}
bool comp(pies a,pies b)
{
    return a.w1<b.w1||a.w1==b.w1&&a.w2<=b.w2;
}
int lowbit(int x)
{
    return x&(-x);
}
int ask(int i)
{
    int Max=0;
    while(i){
        Max=max(Max,s[i]);
        i-=lowbit(i);
    }
    return Max;
}
void add(int i,int val)
{
    while(i<=cnt){
        s[i]=max(s[i],val);
        i+=lowbit(i);
    }
}
int main()
{
//freopen("test.in", "r", stdin);
//freopen("test.out", "w", stdout);
    while (scanf("%d%d",&w,&n)!=EOF){
            init();
        for(int i=1;i<=n;i++)
    {
        scanf("%d%d%d",&a[i].t,&a[i].p,&a[i].v);
        a[i].t*=2;
        a[i].w1=a[i].t-a[i].p;
        a[i].w2=a[i].t+a[i].p;
        Hash[++cnt]=a[i].w2;
    }
    sort(Hash+1,Hash+cnt+1);
    cnt=unique(Hash+1,Hash+cnt+1)-Hash-1;
    for(int i=1;i<=n;i++)
        a[i].w2=lower_bound(Hash+1,Hash+cnt+1,a[i].w2)-Hash;
    sort(a+1,a+n+1,comp);
    for(int i=1;i<=n;i++)
    {
        f[i]=ask(a[i].w2)+a[i].v;
        ans=max(ans,f[i]);
        add(a[i].w2,f[i]);
    }
    printf("%d\n",ans);
}
    return 0;
}
```





