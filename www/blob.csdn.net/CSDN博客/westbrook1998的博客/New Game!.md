# New Game! - westbrook1998的博客 - CSDN博客





2018年10月01日 18:04:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：91








> 
链接：[https://www.nowcoder.com/acm/contest/201/L](https://www.nowcoder.com/acm/contest/201/L)

来源：牛客网

题目描述

Eagle Jump公司正在开发一款新的游戏。Hifumi Takimoto作为其中的员工，获得了提前试玩的机会。现在她正在试图通过一个迷宫。

这个迷宫有一些特点。为了方便描述，我们对这个迷宫建立平面直角坐标系。迷宫中有两条平行直线 L1:Ax+By+C1=0, L2:Ax+By+C2=0，还有 n 个圆 。角色在直线上、圆上、园内行走不消耗体力。在其他位置上由S点走到T点消耗的体力为S和T的欧几里得距离。

Hifumi Takimoto想从 L1 出发，走到 L2 。请计算最少需要多少体力。

输入描述:

第一行五个正整数 n,A,B,C1,C2 (1≤ n ≤ 1000, -10000 ≤ A,B,C1,C2 ≤ 10000)，其中 A,B 不同时为 0。

接下来 n 行每行三个整数 x,y,r(-10000 ≤ x,y ≤ 10000, 1≤ r ≤ 10000) 表示一个圆心为 (x,y)，半径为 r 的圆。

输出描述:

仅一行一个实数表示答案。与正确结果的绝对误差或者相对误差不超过 10-4 即算正确。

示例1

输入

复制

2 0 1 0 -4

0 1 1

1 3 1

输出

复制

0.236068
就把直线和圆都看成结点，然后计算他们之间的距离，然后Dijk求一下最短路

代码：

```
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
int n,a,b,c1,c2;
const int N=1005;
int x[N],y[N],r[N];
double dis1[N];
double dis2[N];
//L1 0 L2 n+1
double dis[N][N];
bool vis[N];
double low[N];
const double INF=0x3f3f3f3f;
double Dijkstra(){
    int s=0;
    int t=n+1;
    for(int i=0;i<=n+1;i++){
        low[i]=INF;
        vis[i]=false;
    }
    low[s]=0;
    for(int i=0;i<=n+1;i++){
        int k=-1;
        double Min=INF;
        for(int j=0;j<=n+1;j++){
            if(!vis[j] && low[j]<Min){
                Min=low[j];
                k=j;
            }
        }
        if(k==-1){
            break;
        }
        vis[k]=true;
        for(int j=0;j<=n+1;j++){
            if(!vis[j] && low[k]+dis[k][j]<low[j]){
                low[j]=low[k]+dis[k][j];
            }
        }
    }
    return low[t];
}
int main(void){
    scanf("%d%d%d%d%d",&n,&a,&b,&c1,&c2);
    for(int i=1;i<=n;i++){
        scanf("%d%d%d",&x[i],&y[i],&r[i]);
        double t=abs(a*x[i]+b*y[i]+c1)*1.0/sqrt(a*a+b*b)-r[i];
        if(t<0){
            dis1[i]=0;
            dis[0][i]=dis[i][0]=0;
        }
        else{
            dis1[i]=t;
            dis[0][i]=dis[i][0]=t;
        }
        t=abs(a*x[i]+b*y[i]+c2)*1.0/sqrt(a*a+b*b)-r[i];
        if(t<0){
            dis2[i]=0;
            dis[n+1][i]=dis[i][n+1]=0;
        }
        else{
            dis2[i]=t;
            dis[n+1][i]=dis[i][n+1]=t;
        }
    }
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            double t=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]))-r[i]-r[j];
            if(t<0){
                dis[i][j]=dis[j][i]=0;
            }
            else{
                dis[i][j]=dis[j][i]=t;
            }
        }
    }
    dis[0][n+1]=dis[n+1][0]=abs(c1-c2)*1.0/sqrt(a*a+b*b);
    // for(int i=0;i<=n+1;i++){
    //     for(int j=0;j<=n+1;j++){
    //         printf("%lf ",dis[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("%.6lf\n",Dijkstra());
    return 0;
}
```





