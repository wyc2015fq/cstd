# 牛客网 New Game! 建图+最短路 - 紫芝的博客 - CSDN博客





2018年10月01日 16:05:30[紫芝](https://me.csdn.net/qq_40507857)阅读数：130








链接：[https://www.nowcoder.com/acm/contest/201/L](https://www.nowcoder.com/acm/contest/201/L)

来源：牛客网


时间限制：C/C++ 1秒，其他语言2秒

空间限制：C/C++ 1048576K，其他语言2097152K

Special Judge, 64bit IO Format: %lld

## 题目描述

Eagle Jump公司正在开发一款新的游戏。Hifumi Takimoto作为其中的员工，获得了提前试玩的机会。现在她正在试图通过一个迷宫。

这个迷宫有一些特点。为了方便描述，我们对这个迷宫建立平面直角坐标系。迷宫中有两条平行直线 L1:Ax+By+C1=0, L2:Ax+By+C2=0，还有 n 个圆 ![](https://uploadfiles.nowcoder.com/files/20180930/305537_1538301791490_equation?tex=C_i%3A(x-x_i)%5E2%2B(y-y_i)%5E2%3D%7Br_i%7D%5E2)。角色在直线上、圆上、园内行走不消耗体力。在其他位置上由S点走到T点消耗的体力为S和T的欧几里得距离。

Hifumi Takimoto想从 L1 出发，走到 L2 。请计算最少需要多少体力。

## 输入描述:

```
第一行五个正整数 n,A,B,C1,C2 (1≤ n ≤ 1000, -10000 ≤ A,B,C1,C2 ≤ 10000)，其中 A,B 不同时为 0。
接下来 n 行每行三个整数 x,y,r(-10000 ≤ x,y ≤ 10000, 1≤ r ≤ 10000) 表示一个圆心为 (x,y)，半径为 r 的圆。
```

## 输出描述:
`仅一行一个实数表示答案。与正确结果的绝对误差或者相对误差不超过 10-4 即算正确。`


示例1

## 输入

复制

```
2 0 1 0 -4
0 1 1
1 3 1
```

## 输出

复制
`0.236068`
### 分析：

L1 到L2 之间连边权值  ![](https://img-blog.csdn.net/20180804212056206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3ZvY2Fsb2lkMDE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

直线L与圆i 之间的权值 max( 0,dis(Oi,L)-Ri )

圆 i 与圆 j 之间的权值 max( 0,dis(Oi,Oj) -Ri-Rj )

求直线L1**(0点)**与直线L2**(n+1点)**之间的最短路

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3+10;
const double eps=1e-4;
const double inf = 1e9+7;
int n;
int a,b,c1,c2;
bool vis[maxn];
double mp[maxn][maxn];
double dis[maxn];


struct circle
{
    int x,y;
    int r;
};
circle cir[maxn];
double dist(int i,int j)
{
    return sqrt((cir[i].x-cir[j].x)*(cir[i].x-cir[j].x)+(cir[i].y-cir[j].y)*(cir[i].y-cir[j].y));
}

double distLine(int c,int i)
{
    return fabs(a*cir[i].x + b*cir[i].y + c )/sqrt(a*a+b*b);
}

void spfa(int from,int to)
{
    memset(vis,0,sizeof vis);
    //memset(dis.inf,sizeof dis);
    for(int i=0;i<maxn;i++)
        dis[i]=inf;

    queue<int> q;
    q.push(from);
    vis[from]=true;
    dis[from]=0;
    while(!q.empty()){
        int cur=q.front();
        q.pop();
        vis[cur]=false;
        for(int i=0;i<=n+1;i++)
        {
            if(dis[i]>dis[cur]+mp[cur][i])
            {
                dis[i]=dis[cur]+mp[cur][i];
                if(!vis[i]){
                    vis[i]=true;
                    q.push(i);
                }
            }
        }
    }
}
int main()
{
    while(scanf("%d %d %d %d %d",&n,&a,&b,&c1,&c2)!=EOF){
        //printf("%d %d %d %d %d\n",n,a,b,c1,c2);
        for(int i=1;i<=n;i++){
            //printf("%d\n",i);
            scanf("%d%d%d",&cir[i].x,&cir[i].y,&cir[i].r);
        }

        double d=fabs(c1-c2)/sqrt(a*a+b*b);
        mp[0][n+1]=mp[n+1][0]=d;

        for(int i=1;i<=n;i++){
            d=distLine(c1,i)-cir[i].r;
            if(d<=eps)
                mp[0][i]=mp[i][0]=0;
            else
                mp[0][i]=mp[i][0]=d;

            d=distLine(c2,i)-cir[i].r;
            if(d<=eps)
                mp[n+1][i]=mp[i][n+1]=0;
            else
                mp[n+1][i]=mp[i][n+1]=d;

            for(int j=i+1;j<=n;j++){
                d=dist(i,j)-cir[i].r-cir[j].r;
                if(d<=eps)
                    mp[i][j]=mp[j][i]=0;
                else
                    mp[i][j]=mp[j][i]=d;
            }
        }
        spfa(0,n+1);
        printf("%.6lf\n",dis[n+1]);
    }
}
```





