# 路痴的单身小菡 BFS求最短路径+DFS求路径数 - 紫芝的博客 - CSDN博客





2018年07月18日 17:29:16[紫芝](https://me.csdn.net/qq_40507857)阅读数：1822








##                                                            路痴的单身小菡

## 题目描述

小菡很聪明，所以他打ACM非常给力，经常偷偷学习到深夜。

他是如此的努力学习，以至于他根本就没有时间完整的逛过学校。

有一天，他听说科大湖的黑天鹅非常好看，由于没有女朋友，他便独自一个人去了。

然而他还在专心致志的观赏黑天鹅，丝毫没有意识到集训还有 k 分钟就要开始了，不幸的是刚好小菡是一个路痴。

你觉得他在 k 分钟内可以赶到创客参加集训吗？

如果可以，他最少要花多少时间才可以回到创客空间参加集训呢？这样子的路径有多少条？

## 输入

测试数据第一组为T（1 <= T <= 100），表示测试样例组数。

对于每组测试样例：

第一行输入为三个正整数 n m k（1 <= n，m <= 1000，0 <= k <= 10000），n，m表示地图的长和宽，k表示最多允许花费在路上的时间(在路上花费的时间刚好为k也合法)。

接下来n行输入地图，其中包含有符号‘*’、‘#’、‘L’、'C'。

    *：表示可以允许走动的空间。

    #：表示障碍物，无法走动的空间。

    L：表示科大湖，即小菡的起点，保证有且仅有一个。

    C：表示创客空间，即小菡的终点，保证有且仅有一个。

小菡走动的计时是从一个空间到另一个空间为一分钟。开始时，小菡已经站在了起点上。当移动时间等于k时刚好到达终点依然视为合法。

(注意：小菡只能往上下左右四个方向走动。)



## 输出

输出形式为“Case #x: ”(不包含引号)，x表示对应第x个样例。

如果小菡在 k 分钟内无法回到创客，则输出-1。

否则，输出小菡回到创客花费的最短时间，和满足该最短时间的路径条数。

## 样例输入

```
4
2 2 3
L*
*C
4 4 10
L***
****
****
***C
4 4 8
L*##
#***
###*
C***
4 4 10
L*##
#***
###*
C***
```

## 样例输出

```
Case #1: 2 2
Case #2: 6 20
Case #3: -1
Case #4: 9 1
```

```cpp
#include<cstdio>
#include<iostream>
#include<queue>
using namespace std;
const int maxn = 1005;
char mp[maxn][maxn];
int dis[maxn][maxn];
//初始化距离矩阵，0代表起点，-1代表墙，n代表从起点到这一点的距离(n=1,2,3...n)
int t,n,m,k,cnt;
struct node{
  int x,y,n;
  node(int a,int b,int c){x = a,y = b,n = c;}
};
//
void find(int x,int y){
  if(x >= 0 && x < n && y >= 0 && y < m){
    if(mp[x][y] == 'L'){
      cnt++;
      return;
    }
    if(dis[x][y] == -1) return;
    if(dis[x + 1][y] + 1 == dis[x][y])
      find(x + 1,y);
    if(dis[x - 1][y] + 1 == dis[x][y])
      find(x - 1,y);
    if(dis[x][y + 1] + 1 == dis[x][y])
      find(x,y + 1);
    if(dis[x][y - 1] + 1 == dis[x][y])
      find(x,y - 1);
  }
}
void bfs(int x,int y){
  queue<node> q;
  node p(x,y,0);
  q.push(p);
  while(!q.empty()){
    p = q.front();q.pop();
    if(mp[p.x][p.y] != '#' && p.x >= 0 && p.x < n && p.y >= 0 && p.y < m && dis[p.x][p.y] == -1){
      if(mp[p.x][p.y] == 'C'){
        if(dis[p.x][p.y] != -1 && p.n > dis[p.x][p.y]) return;
        dis[p.x][p.y] = p.n;
      }else{
        dis[p.x][p.y] = p.n;
        {node temp(p.x + 1,p.y,p.n + 1);q.push(temp);}
        {node temp(p.x - 1,p.y,p.n + 1);q.push(temp);}
        {node temp(p.x,p.y + 1,p.n + 1);q.push(temp);}
        {node temp(p.x,p.y - 1,p.n + 1);q.push(temp);}
      }
    }
  }

}
int main()
{
  scanf("%d",&t);
  for(int Case = 1;Case <= t; Case++){
    scanf("%d %d %d",&n,&m,&k);
    for(int i = 0;i < n; i++){
      scanf("%s",mp[i]);
      fill(dis[i],dis[i] + maxn, -1);
    }
    int sx,sy,ex,ey;
    for(int i = 0;i < m; i++){
      for(int j = 0;j < m; j++){
        if(mp[i][j] == 'L'){
          sx = i;
          sy = j;
        }
        if(mp[i][j] == 'C'){
          ex = i;
          ey = j;
        }
      }
    }
    bfs(sx,sy);
    if(dis[ex][ey] > k || dis[ex][ey] == -1) printf("Case #%d: -1\n",Case);
    else{
      cnt = 0;
      //输出距离矩阵
      /*
      for(int i = 0;i < n ;i++){
        for(int j = 0;j < m; j++){
          printf("%d ",dis[i][j]);
        }
        printf("\n");
      }
      */
      find(ex,ey);//从终点向起点搜索
      printf("Case #%d: %d %d\n",Case,dis[ex][ey],cnt);
    }
  }
  return 0;
}
```





