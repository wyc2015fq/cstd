# C++实现走迷宫算法 - fanyun的博客 - CSDN博客
2018年09月28日 20:22:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：842
1、走迷宫算法要求
       你来到一个迷宫前。该迷宫由若干个房间组成，每个房间都有一个得分，第一次进入这个房间，你就可以得到这个分数。还有若干双向道路连结这些房间，你沿着这些道路从一个房间走到另外一个房间需要一些时间。游戏规定了你的起点和终点房间，你首要目标是从起点尽快到达终点，在满足首要目标的前提下，使得你的得分总和尽可能大。现在问题来了，给定房间、道路、分数、起点和终点等全部信息，你能计算在尽快离开迷宫的前提下，你的最大得分是多少么？
       输入：
      第一行4个整数n (<=500), m, start, end。n表示房间的个数，房间编号从0到(n - 1)，m表示道路数,任意两个房间之间最多只有一条道路，start和end表示起点和终点房间的编号。 
       第二行包含n个空格分隔的正整数(不超过600)，表示进入每个房间你的得分。 
       再接下来m行，每行3个空格分隔的整数x, y, z (0< z<=200)表示道路,表示从房间x到房间y(双向)的道路,注意，最多只有一条道路连结两个房间, 你需要的时间为z。 输入保证从start到end至少有一条路径。
     输出：
     一行，两个空格分隔的整数，第一个表示你最少需要的时间，第二个表示你在最少时间前提下可以获得的最大得分。
Input示例：
3 2 0 2
1 2 3
0 1 10
1 2 11
Output示例：
216
2、走迷宫算法思路
        dijkstra求单源最短路，特殊的地方在于，以往求最短路的判定条件只有长度，现在加上了分数，所以在条件判断上要考虑分数，同样的，在松弛步骤上也要对每个点的权值(从起点到当前点路程最短分数最大时的分数值)进行更新。 
```cpp
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>
#include <string.h>
#include <set>
#include <stack>
#include <stdlib.h>
#include <time.h>
using namespace std;
const int MAX = 0x1f1f1f1f;
int d[509];
int g[509][509];
int v[509];
int dd[509];
bool f[509] = {};
void dji(int n, int s, int e)
{
    for(int i=0;i<n;i++)
    {
        v[i] = g[s][i];
        dd[i] = d[i]+d[s];
    }
    dd[s] -= d[s];
    v[s] = 0;
    f[s] = 1;
    for(int k=1;k<n;++k)
    {
        int mi=-1, mmin = MAX, sc = -1;
        for(int j=0;j<n;j++)
        {
            if(!f[j])
            {
                if((v[j] == mmin && dd[j] > sc)
                   || v[j] < mmin)
                {
                    sc = dd[j];
                    mmin = v[j];
                    mi = j;
                }
            }
        }
        if(mi == -1)
            break;
        f[mi] = 1;
        for(int i=0;i<n;i++)
        {
            if(v[i] > mmin + g[mi][i])
            {
                v[i] = mmin + g[mi][i];
                dd[i] = sc + d[i];
            }
            else if(v[i] == mmin + g[mi][i])
            {
                dd[i] = max(dd[i], sc + d[i]);
            }
        }
    }
    printf("%d %d\n",v[e], dd[e]);
}
int main()
{
    int n, m, s, e;
    memset(g, 0x1f, sizeof(g));
    scanf("%d%d%d%d",&n,&m,&s,&e);
    for(int i=0;i<n;i++)
        scanf("%d",&d[i]);
    for(int i=0;i<m;i++)
    {
        int a,b,c;
        scanf("%d%d%d",&a,&b,&c);
        g[a][b] = g[b][a] = c;
    }
    dji(n,s,e);
}
```
