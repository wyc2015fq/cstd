# Mouse Hunt（CF-1027D） - Alex_McAvoy的博客 - CSDN博客





2018年08月24日 13:53:30[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：114








> 
# Problem Description

Medicine faculty of Berland State University has just finished their admission campaign. As usual, about 80% of applicants are girls and majority of them are going to live in the university dormitory for the next 4 (hopefully) years.

The dormitory consists of nn rooms and a single mouse! Girls decided to set mouse traps in some rooms to get rid of the horrible monster. Setting a trap in room number i costs cici burles. Rooms are numbered from 1 to n.

Mouse doesn't sit in place all the time, it constantly runs. If it is in room i in second tt then it will run to room ai in second t+1 without visiting any other rooms inbetween (i=ai means that mouse won't leave room ii). It's second 0 in the start. If the mouse is in some room with a mouse trap in it, then the mouse get caught into this trap.

That would have been so easy if the girls actually knew where the mouse at. Unfortunately, that's not the case, mouse can be in any room from 1 to n at second 0.

What it the minimal total amount of burles girls can spend to set the traps in order to guarantee that the mouse will eventually be caught no matter the room it started from?

# Input

The first line contains as single integers n (1≤n≤2⋅105) — the number of rooms in the dormitory.

The second line contains nn integers c1,c2,…,cn (1≤ci≤104) — cici is the cost of setting the trap in room number i.

The third line contains nn integers a1,a2,…,an (1≤ai≤n) — aiai is the room the mouse will run to the next second after being in room i.

# Output

Print a single integer — the minimal total amount of burles girls can spend to set the traps in order to guarantee that the mouse will eventually be caught no matter the room it started from.

# Examples

**Input**

5

1 2 3 2 10

1 3 4 3 3

**Output**

3

**Input**

4

1 10 2 10

2 4 2 2

**Output**

10

**Input**

7

1 1 1 1 1 1 1

2 2 2 3 6 7 6

**Output**

2


————————————————————————————————————————————

题意： 给出 n 个位置捕鼠器的价值，然后给了 n 个数字，代表点 i 能够走到这个点，求使老鼠全被抓到放捕鼠器的最小价值。

思路：判断有环、无环的情况，有环的话一定是在环上因此遇到环进去找一圈即可，无环的话就是放在最后的点处。

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
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int n;
int a[N],father[N];
int in[N],out[N],look[N],ss[N];
LL ans;
void judge(int now,int start,int flag,int minn)
{
    look[now]=1;
    if(flag&&now==start)///回到起点
    {
        ans+=minn;
        return;
    }
    judge(father[now],start,1,min(minn,a[now]));
}
void dfs(int now,int id)
{
    if(ss[now]!=0&&ss[now]!=id&&look[now])//如果当前点被访问过，且这个点所属始点不是当前始点，则说明之前的链被访问过，此时链后一定有捕鼠器
        return;

    ss[now]=id;
    if(look[now])//如果该点被访问过，说明存在环
    {
        judge(now,now,0,INF);
        return;
    }
    look[now]=1;

    if(father[now]==now)
    {
        ans+=a[now];
        return;
    }
    dfs(father[now],id);
}
int main()
{
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);

    for(int i=1;i<=n;i++)
    {
        scanf("%d",&father[i]);
        if(father[i]==i)
            continue;
        in[father[i]]++;
    }

    for(int i=1;i<=n;i++)
        if(in[i]==0)
            dfs(i,i);

    for(int i=1;i<=n;i++)//判断是否有环
        if(look[i]==0)
            judge(i,i,0,INF);

    printf("%lld\n",ans);

    return 0;
}
```





