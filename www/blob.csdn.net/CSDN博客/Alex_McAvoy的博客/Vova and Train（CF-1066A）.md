# Vova and Train（CF-1066A） - Alex_McAvoy的博客 - CSDN博客





2018年10月24日 18:36:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：91
个人分类：[CodeForces																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Vova plans to go to the conference by train. Initially, the train is at the point 1 and the destination point of the path is the point L. The speed of the train is 1length unit per minute (i.e. at the first minute the train is at the point 1, at the second minute — at the point 2 and so on).

There are lanterns on the path. They are placed at the points with coordinates divisible by v (i.e. the first lantern is at the point v, the second is at the point 2 v and so on).

There is also exactly one standing train which occupies all the points from ll to rrinclusive.

Vova can see the lantern at the point p if pp is divisible by vv and there is no standing train at this position (p∉[l;r]). Thus, if the point with the lantern is one of the points covered by the standing train, Vova can't see this lantern.

Your problem is to say the number of lanterns Vova will see during the path. Vova plans to go to tt different conferences, so you should answer tt independent queries.

# Input

The first line of the input contains one integer tt (1≤t≤104) — the number of queries.

Then tt lines follow. The ii-th line contains four integers Li,vi,li,ri (1≤L,v≤109, 1≤l≤r≤L) — destination point of the i-th path, the period of the lantern appearance and the segment occupied by the standing train.

# Output

Print t lines. The i-th line should contain one integer — the answer for the i-th query.

# Examples

**Input**

4

10 2 3 7

100 51 51 51

1234 1 100 199

1000000000 1 1 1000000000

**Output**

3

0

1134

0

————————————————————————————————————————————

题意：找 1~L 之间 v 的倍数的个数，在区间 [l,r] 之间的灯笼不能算 

思路：注意最后 l/v-right/v+left/v

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
#define N 200001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
struct Node{
    int x;
    int step;
}q[N];
bool vis[N];
int dir[2]={-1,1};
int bfs(int n,int k){
    int head=1,tail=1;
    memset(vis,false,sizeof(vis));
    vis[n]=1;
    q[tail].x=n;
    q[tail].step=0;
    tail++;

    while(head<tail){
        int x=q[head].x;
        int step=q[head].step;
        if(x==k)
            return step;

        int nx;
        for(int i=0;i<2;i++){
            nx=x+dir[i];
            if(nx>=0&&nx<N&&!vis[nx]){
                vis[nx]=1;
                q[tail].x=nx;
                q[tail].step=step+1;
                tail++;
            }
        }

        nx=x*2;
        if(nx>=0&&nx<N&&!vis[nx]){
            vis[nx]=1;
            q[tail].x=nx;
            q[tail].step=step+1;
            tail++;
        }
        head++;
    }
    return -1;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int l,v,left,right;
        scanf("%d%d%d%d",&l,&v,&left,&right);
        int num=l/v-right/v+left/v;
        if(left%v==0)
            num--;
        printf("%d\n",num);
    }
    return 0;
}
```






