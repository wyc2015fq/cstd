# Applese 走迷宫（2019牛客寒假算法基础集训营 Day4-C） - Alex_McAvoy的博客 - CSDN博客





2019年02月02日 15:57:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[牛客																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/8639591)








> 
# 【题目描述】

精通程序设计的 Applese 双写了一个游戏。

在这个游戏中，它被困在了一个 n×m 的迷宫中，它想要逃出这个迷宫。

在迷宫中，有一些方格是水池，只有当 Applese 处于水属性的时候才可以通过；有一些方格是岩浆，只有当 Applese 是火属性的时候可以通过；有一些方格是墙壁，无论如何都无法通过；另一些格子是空地（包括起点和终点），可以自由通过。

在一些空地上有神秘道具可以让 Applese 转换自己的属性（从水属性变为火属性或从火属性变为水属性，需要一个单位的时间）。

已知 Applese 在一个单位的时间内可以朝四个方向行走一格，且开始处于水属性，位于空地的道具拾取后只能在该处立即使用（或者不使用），且可以多次使用。求它走出迷宫需要的最少时间。

# 【输入描述】

第一行两个正整数 n, m 表示迷宫的大小。

接下来 n 行，每行长度为 m 的字符串。描述地图。

其中 'S' 表示起点，'T' 表示终点，'.' 表示空地，'w'表示岩浆，'~'表示水池，'@' 表示道具，'#'表示障碍。

保证地图中的起点和终点只有一个，道具都位于空地。

1≤n,m≤100

# 【输出描述】

输出一个整数，表示 Applese 走出迷宫的最短时间。特别地，如果 Applese 走不出迷宫，输出 "-1"。

# 【样例】

示例1

输入

5 5

.w@..

.S#..

~w#..

.w..~

@w.~T

输出

18


思路：用 bfs 进行搜索求最短路，由于涉及到水火属性的问题，因此状态需要到达三维，即 dis[i][j][0/1] 表示在 (i,j) 处属性为水/火的最短路，然后按题意进行模拟，分类讨论转移

# 【源代码】

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 101
#define LL long long
using namespace std;
char G[N][N];
bool vis[N][N][2];
int dx[]={-1,0,1,0};
int dy[]={0,-1,0,1};
struct Node{
    int x,y;//坐标
    int step;//步数
    bool sta;//水火的状态,0表示水,1表示火
    Node(){}
    Node(int x,int y,int step,bool sta):x(x),y(y),step(step),sta(sta){}
}S,T;
bool judge(int x,int y,bool sta){//检测当前属性能否通过
    if(G[x][y]=='w'&&!sta)
        return false;
    else if(G[x][y]=='~'&&sta)
        return false;
    return true;
}
int bfs(int n,int m){
    memset(vis,0,sizeof(vis));
    vis[S.x][S.y][0]=1;

    queue<Node> Q;
    Q.push(S);
    while(!Q.empty()){

        Node temp=Q.front();
        Q.pop();

        int x=temp.x;
        int y=temp.y;
        int step=temp.step;
        bool sta=temp.sta;

        if(x==T.x&&y==T.y)//到达终点
            return step;

        if(G[x][y]=='@'&&vis[x][y][!sta]==0){//属性变更
            Q.push( Node(x,y,step+1,!sta) );
            vis[x][y][!sta]=1;
        }

        for(int i=0;i<4;i++){
            int nx=x+dx[i];
            int ny=y+dy[i];

            if(nx>=0&&ny>=0&&nx<n&&ny<m&&G[x][y]!='#'){
                if(judge(nx,ny,sta)){//检查当前属性能否通过当前格
                    Node next=Node(nx,ny,step+1,sta);
                    if(vis[nx][ny][sta]==0){
                        Q.push(next);
                        vis[nx][ny][sta]=1;
                    }
                }

            }
        }
    }
    return -1;
}

int main(){

    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++)
        scanf("%s", G[i]);

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(G[i][j]=='S'){
                S.x=i;
                S.y=j;
                S.step=0;
                S.sta=0;//初始处于水属性
            }
            if(G[i][j]=='T'){
                T.x=i;
                T.y=j;
            }
        }
    }

    printf("%d\n",bfs(n,m));

    return 0;
}
```






