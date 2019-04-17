# Mining Station on the Sea（HDU-2448） - Alex_McAvoy的博客 - CSDN博客





2018年12月08日 19:23:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

The ocean is a treasure house of resources and the development of human society comes to depend more and more on it. In order to develop and utilize marine resources, it is necessary to build mining stations on the sea. However, due to seabed mineral resources, the radio signal in the sea is often so weak that not all the mining stations can carry out direct communication. However communication is indispensable, every two mining stations must be able to communicate with each other (either directly or through other one or more mining stations). To meet the need of transporting the exploited resources up to the land to get put into use, there build n ports correspondently along the coast and every port can communicate with one or more mining stations directly.

Due to the fact that some mining stations can not communicate with each other directly, for the safety of the navigation for ships, ships are only allowed to sail between mining stations which can communicate with each other directly. 

The mining is arduous and people do this job need proper rest (that is, to allow the ship to return to the port). But what a coincidence! This time, n vessels for mining take their turns to take a rest at the same time. They are scattered in different stations and now they have to go back to the port, in addition, a port can only accommodate one vessel. Now all the vessels will start to return, how to choose their navigation routes to make the total sum of their sailing routes minimal. 

Notice that once the ship entered the port, it will not come out!

# **Input**

There are several test cases. Every test case begins with four integers in one line, n (1 = <n <= 100), m (n <= m <= 200), k and p. n indicates n vessels and n ports, m indicates m mining stations, k indicates k edges, each edge corresponding to the link between a mining station and another one, p indicates p edges, each edge indicating the link between a port and a mining station. The following line is n integers, each one indicating one station that one vessel belongs to. Then there follows k lines, each line including 3 integers a, b and c, indicating the fact that there exists direct communication between mining stations a and b and the distance between them is c. Finally, there follows another p lines, each line including 3 integers d, e and f, indicating the fact that there exists direct communication between port d and mining station e and the distance between them is f. In addition, mining stations are represented by numbers from 1 to m, and ports 1 to n. Input is terminated by end of file.

# Output

Each test case outputs the minimal total sum of their sailing routes.

# Sample Input

**3 5 5 61 2 41 3 31 4 41 5 52 5 32 4 31 1 51 5 32 5 32 4 63 1 43 2 2**

# Sample Output

**13**


题意：有一 n 个港口 m 个油田的无向图，并给出了图中所有的边与权值，现在给出 n 个船所在的油田编号，要让这 n 条船每一条头回到一个港口中，且每个港口只能容纳一条船，求这 n 条船行走的距离总和的最小值

思路：每条船到任意港口均有一个最短距离，首先要求出最短距离，然后建立一个二分图，左点集为 n 个港口，右点集为 n 条船，若第 j 条船到第 i 个港口的距离为 x，则连一条左 i 右 j 的边，由于要求的是最小值，因此需要对所有边取负再使用 KM 算法，最后得出的最优匹配取负，即为答案 

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=n;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=n;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=n;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));
    memset(Lx,0,sizeof(Lx));
    memset(Ly,0,sizeof(Ly));

    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            Lx[i]=max(Lx[i],G[i][j]);

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=n;i++)
        if(linkY[i]!=-1)
            ans+=G[linkY[i]][i];

    return ans;
}
int id[N];
int dis[N][N];
int main(){
    int m,k,p;
    while(scanf("%d%d%d%d",&n,&m,&k,&p)!=EOF&&(n+m+k+p)){
        for(int i=1;i<=n;i++)
            scanf("%d",&id[i]);

        memset(dis,INF,sizeof(dis));
        for(int i=1;i<=n+m;i++)
            for(int j=1;j<=n+m;j++)
                if(i==j)
                    dis[i][j]=0;

        while(k--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            dis[x+n][y+n]=w;
            dis[y+n][x+n]=w;
        }

        while(p--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            dis[x][y+n]=w;
        }

        for(int k=1;k<=n+m;k++)
            for(int i=1;i<=n+m;i++)
                for(int j=1;j<=n+m;j++)
                    if(dis[i][k]<INF && dis[k][j]<INF)
                        dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);

        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                G[i][j]=-dis[i][id[j]+n];

        printf("%d\n",-KM());
    }
    return 0;
}
```






