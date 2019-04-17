# Expedition（POJ-2431） - Alex_McAvoy的博客 - CSDN博客





2018年07月24日 23:45:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45








# Problem Description

A group of cows grabbed a truck and ventured on an expedition deep into the jungle. Being rather poor drivers, the cows unfortunately managed to run over a rock and puncture the truck's fuel tank. The truck now leaks one unit of fuel every unit of distance it travels. 

To repair the truck, the cows need to drive to the nearest town (no more than 1,000,000 units distant) down a long, winding road. On this road, between the town and the current location of the truck, there are N (1 <= N <= 10,000) fuel stops where the cows can stop to acquire additional fuel (1..100 units at each stop). 

The jungle is a dangerous place for humans and is especially dangerous for cows. Therefore, the cows want to make the minimum possible number of stops for fuel on the way to the town. Fortunately, the capacity of the fuel tank on their truck is so large that there is effectively no limit to the amount of fuel it can hold. The truck is currently L units away from the town and has P units of fuel (1 <= P <= 1,000,000). 

Determine the minimum number of stops needed to reach the town, or if the cows cannot reach the town at all. 

# **Input**

* Line 1: A single integer, N 

* Lines 2..N+1: Each line contains two space-separated integers describing a fuel stop: The first integer is the distance from the town to the stop; the second is the amount of fuel available at that stop. 

* Line N+2: Two space-separated integers, L and P

# Output

* Line 1: A single integer giving the minimum number of fuel stops necessary to reach the town. If it is not possible to reach the town, output -1.

# Sample Input

**44 45 211 515 1025 10**

# Sample Output

**2**

————————————————————————————————————————————————————

题意：汽车要行走 L 的路程，每走 1 距离消耗 1 单位的汽油，目前已有汽油为 P ，一路上有 n 个加油站，可以获得相应的汽油，求最少要加多少次油可以到达终点。

思路：按照贪心的思想，将所有加油站进行排序，由于不知道什么时候加最合适，所以将那些途经的加油站全部放到优先级队列中，当需要加油时再加，相当于经过一个加油站时将油加上，这样每次加的都是油量最大的，因此最后的加油次数就是最少的。

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
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
struct Node{
    int pos;
    int gasoline;
}station[N];
int cmp(Node a,Node b)
{
    if(a.pos==b.pos)
        return a.gasoline>b.gasoline;
    return a.pos<b.pos;
}
int main()
{
    int n,l,p;
    scanf("%d",&n);
    for(int i=0;i<n;i++)
        scanf("%d%d",&station[i].pos,&station[i].gasoline);
    scanf("%d%d",&l,&p);

    for(int i=0;i<n;i++)//将加油站位置转换为距终点的距离
        station[i].pos=l-station[i].pos;

    /*将终点视为一个加油站*/
    station[n].pos=l;
    station[n].gasoline=0;

    sort(station,station+n+1,cmp);//将加油站按位置升序排序

    priority_queue<int> q;
    bool flag=true;
    int sum=0,pos=0,res_gasoline=p;
    for(int i=0;i<n+1;i++)
    {
        int dis=station[i].pos-pos;//要走的距离
        while(res_gasoline<dis)
        {
            if(q.empty())//如果未到加油站就没油了
            {
                flag=false;
                break;
            }

            res_gasoline+=q.top();
            q.pop();
            sum++;
        }
        if(!flag)
            break;
        res_gasoline-=dis;
        pos=station[i].pos;
        q.push(station[i].gasoline);
    }
    if(flag)
        printf("%d\n",sum);
    else
        printf("-1\n");

    return 0;
}
```





