# Exploration（POJ-3618） - Alex_McAvoy的博客 - CSDN博客





2018年06月02日 10:34:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68
个人分类：[POJ																[基础算法——排序](https://blog.csdn.net/u011815404/article/category/7439201)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Bessie is traveling on a road teeming with interesting landmarks. The road is labeled just like a number line, and Bessie starts at the "origin" (x = 0). A total of N (1 ≤ N ≤ 50,000) landmarks are located at points x1, x2, ..., xN (-100,000 ≤ xi ≤ 100,000). Bessie wants to visit as many landmarks as possible before sundown, which occurs in T (1 ≤ T ≤ 1,000,000,000) minutes. She travels 1 distance unit in 1 minute.

Bessie will visit the landmarks in a particular order. Since the landmarks closer to the origin are more important to Farmer John, she always heads for the unvisited landmark closest to the origin. No two landmarks will be the same distance away from the origin.

Help Bessie determine the maximum number of landmarks she can visit before the day ends.

# **Input**

Line 1: Two space-separated integers: T and N

Lines 2..N+1: Line i+1 contains a single integer that is the location of the ith landmark: xi

# Output

Line 1: The maximum number of landmarks Bessie can visit.

# Sample Input

**25 5**

**10-38-71**

# Sample Output

**4**

———————————————————————————————————————————————————————

题意：一条数轴上，给n个点，从原点出发，优先达到距离原点最近的点，求t秒内到达的点的最大数量。

思路：点的坐标有正有负，升序排序时按绝对值大小排序，然后计算t秒内到达的最大数量即可。

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
#define N 100001
#define MOD 2520
#define E 1e-12
using namespace std;
int a[N];
bool cmp(int x,int y)
{
    return abs(x)<abs(y);
}
int main()
{
    int t,n;
    scanf("%d%d",&t,&n);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);

    sort(a+1,a+1+n,cmp);

    int cnt=0;
    int use=0;//已用时间
    int k=1;
    if(a[1]==0)
    {
        cnt=1;
        k++;
    }

    for(int i=k;i<=n;i++)
    {
        int time=abs(a[i]-a[i-1]);
        if(time<=t-use)//若时间足够，则前往下一点
        {
            cnt++;
            use+=time;//累计已用时间
        }
        else
            break;
    }
    printf("%d\n",cnt);

    return 0;
}
```






