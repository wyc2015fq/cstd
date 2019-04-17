# River Hopscotch（POJ-3258） - Alex_McAvoy的博客 - CSDN博客





2018年07月24日 16:11:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[POJ																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Every year the cows hold an event featuring a peculiar version of hopscotch that involves carefully jumping from rock to rock in a river. The excitement takes place on a long, straight river with a rock at the start and another rock at the end, L units away from the start (1 ≤ L ≤ 1,000,000,000). Along the river between the starting and ending rocks, N (0 ≤ N ≤ 50,000) more rocks appear, each at an integral distance Di from the start (0 < Di <L).

To play the game, each cow in turn starts at the starting rock and tries to reach the finish at the ending rock, jumping only from rock to rock. Of course, less agile cows never make it to the final rock, ending up instead in the river.

Farmer John is proud of his cows and watches this event each year. But as time goes by, he tires of watching the timid cows of the other farmers limp across the short distances between rocks placed too closely together. He plans to remove several rocks in order to increase the shortest distance a cow will have to jump to reach the end. He knows he cannot remove the starting and ending rocks, but he calculates that he has enough resources to remove up to M rocks (0 ≤ M ≤ N).

FJ wants to know exactly how much he can increase the shortest distance *before* he starts removing the rocks. Help Farmer John determine the greatest possible shortest distance a cow has to jump after removing the optimal set of M rocks.

# **Input**

Line 1: Three space-separated integers: L, N, and M 

Lines 2..N+1: Each line contains a single integer indicating how far some rock is away from the starting rock. No two rocks share the same position.

# Output

Line 1: A single integer that is the maximum of the shortest distance a cow has to jump after removing M rocks

# Sample Input

**25 5 2214112117**

# Sample Output

**4**

————————————————————————————————————————————————————

题意：有一条宽度为 L 的河，河的两端各有一块石头，河中有 n 块石头，每块石头到河起点一端的石头都有唯一的距离，现在要移除 m 块石头，求移除 m 块石头后，剩下的石头的间距的最小值最大。

思路：没想出来该怎么二分，看了题解才明白，要二分最小值，再比较最小值是 mid 时，来改动 left 还是 right

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
#include<set>
#include<map>
#include<stack>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int dis[N];
int l,n,m;
bool judge(int mid)
{
    int cnt=0,num=0;
    for(int i=1;i<=n+1;)
    {
        if(dis[i]-dis[num]>=mid)
        {
            i++;
            num=i-1;
        }
        else
        {
            cnt++;
            i++;
        }
    }
    if(cnt<=m)
        return true;
    else
        return false;
}
int main()
{
    scanf("%d%d%d",&l,&n,&m);
    for(int i=1;i<=n;i++)//所有石头到河的起点的距离
        scanf("%d",&dis[i]);

    dis[0]=0;//河的起点到起点的距离
    dis[n+1]=l;//河的终点到起点的距离

    sort(dis,dis+n+1);//将所有石头排序

    int left=0,right=l;
    while(right-left>1)
    {
        int mid=(left+right)/2;
        if(judge(mid))
            left=mid;
        else
            right=mid;
    }
    printf("%d\n",left);
    return 0;
}
```






