# Starship Troopers（HDU-1011） - Alex_McAvoy的博客 - CSDN博客





2018年10月09日 20:03:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50








# Problem Description

You, the leader of Starship Troopers, are sent to destroy a base of the bugs. The base is built underground. It is actually a huge cavern, which consists of many rooms connected with tunnels. Each room is occupied by some

 bugs, and their brains hide in some of the rooms. Scientists have just developed a new weapon and want to experiment it on some brains. Your task is to destroy the whole base, and capture as many brains as possible.

To kill all the bugs is always easier than to capture their brains. A map is drawn for you, with all the rooms marked by the amount of bugs inside, and the possibility of containing a brain. The cavern's structure is like a tree in such a way that there is

 one unique path leading to each room from the entrance. To finish the battle as soon as possible, you do not want to wait for the troopers to clear a room before advancing to the next one, instead you have to leave some troopers at each room passed to fight

 all the bugs inside. The troopers never re-enter a room where they have visited before.

A starship trooper can fight against 20 bugs. Since you do not have enough troopers, you can only take some of the rooms and let the nerve gas do the rest of the job. At the mean time, you should maximize the possibility of capturing a brain. To simplify the

 problem, just maximize the sum of all the possibilities of containing brains for the taken rooms. Making such a plan is a difficult job. You need the help of a computer.

# **Input**

The input contains several test cases. The first line of each test case contains two integers N (0 < N <= 100) and M (0 <= M <= 100), which are the number of rooms in the cavern and the number of starship troopers you have,

 respectively. The following N lines give the description of the rooms. Each line contains two non-negative integers -- the amount of bugs inside and the possibility of containing a brain, respectively. The next N - 1 lines give the description of tunnels.

 Each tunnel is described by two integers, which are the indices of the two rooms it connects. Rooms are numbered from 1 and room 1 is the entrance to the cavern.

The last test case is followed by two -1's.

# Output

For each test case, print on a single line the maximum sum of all the possibilities of containing brains for the taken rooms.

# Sample Input

5 10

50 10

40 10

40 20

65 30

70 30

1 2

1 3

2 4

2 5

1 1

20 7

-1 -1

# Sample Output

50

7

————————————————————————————————————————————————————

题意：给出每个房间的联通图，要想到下一个房间必须先攻破上一个房间，给出每个房间拥有的BUG数量和可得到的能量数量，要求每个人最多消灭20个BUG，如果一个房间内不足20个BUG也必须安排一个士兵

思路：树形DP的分组背包问题

将每个节点的子节点看成一个背包，背包容量是这个节点的子孙数量，选几个节点就是选容量，要注意的是想选父节点必须先选子节点以及特判为0点情况

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
#define N 101
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
struct Edge{
    int to;
    int next;
}edge[N*2];
int n,m;
int bug[N],power[N];
int head[N],vis[N];
int dp[N][N];
int cnt;
void addEdge(int x,int y){
    edge[cnt].to=y;
    edge[cnt].next=head[x];
    head[x]=cnt;
    cnt++;
}
void treeDP(int x){
    vis[x]=1;
    int cost=(bug[x]+19)/20;//不足20的部分
    for(int i=cost;i<=m;i++)
        dp[x][i]=power[x];
    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(!vis[y]){
            treeDP(y);
            for(int j=m;j>=cost;j--)
                for(int k=1;j+k<=m;k++)
                    if(dp[y][k])
                        dp[x][j+k]=max(dp[x][j+k],dp[x][j]+dp[y][k]);
        }
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF&&n+m>0){
        cnt=0;
        memset(vis,0,sizeof(vis));
        memset(dp,0,sizeof(dp));
        memset(head,-1,sizeof(head));
        for(int i=1;i<=n;i++)
            scanf("%d%d",&bug[i],&power[i]);
        for(int i=1;i<n;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,y);
            addEdge(y,x);
        }

        if(m==0){
            printf("0\n");
            continue;
        }

        treeDP(1);
        printf("%d\n",dp[1][m]);
    }

	return 0;
}
```





