# Going Home（POJ-2195） - Alex_McAvoy的博客 - CSDN博客





2018年12月06日 21:49:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

On a grid map there are n little men and n houses. In each unit time, every little man can move one unit step, either horizontally, or vertically, to an adjacent point. For each little man, you need to pay a $1 travel fee for every step he moves, until he enters a house. The task is complicated with the restriction that each house can accommodate only one little man. 

Your task is to compute the minimum amount of money you need to pay in order to send these n little men into those n different houses. The input is a map of the scenario, a '.' means an empty space, an 'H' represents a house on that point, and am 'm' indicates there is a little man on that point. 

You can think of each point on the grid map as a quite large square, so it can hold n little men at the same time; also, it is okay if a little man steps on a grid with a house without entering that house.

# **Input**

There are one or more test cases in the input. Each case starts with a line giving two integers N and M, where N is the number of rows of the map, and M is the number of columns. The rest of the input will be N lines describing the map. You may assume both N and M are between 2 and 100, inclusive. There will be the same number of 'H's and 'm's on the map; and there will be at most 100 houses. Input will terminate with 0 0 for N and M.

# Output

For each test case, output one line with the single integer, which is the minimum amount, in dollars, you need to pay.

# Sample Input

**2 2.mH.5 5HH..m...............mm..H7 8...H.......H.......H....mmmHmmmm...H.......H.......H....0 0**

# Sample Output

**21028**


题意：给一 n*m 的地图，地图上有若干个人和房子，且人和房子数量相同，人每移动一格需要花费 1 费用，一个房子只能住一个人，现在要让所有的人都入住房子，求花费的最小费用

思路：人为左点集，房子为右点集，每个人与每个房子间都有一条边，边的权值是人到房子的距离，现要让每个人都进入一房间，且移动的费用最小，实质就是求二分图的一个完全匹配，且匹配点的边权和最小

使用 KM 算法只能求二分图的最优匹配，即边权和最大的完全匹配， 这里有一个技巧，就是将所有的边权取负，再进行 KM 算法，得到的解取负就是边权和最小的完全匹配

假设存在一个最优解 res，是所有解中花费最小的，那么 -res 自然是所有花费中最大的解，当将所有边权取负后，用 KM 算法得到的最优匹配必然是那个花费最大的解，取负后就是所需的最小边权值的解

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
                    linkY[y]=x;
                    linkX[x]=y;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++){
        if(visX[i]){
            for(int j=1;j<=n;j++){
                if(!visY[j]){
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);
                }
            }
        }
    }
    for(int i=1;i<=n;i++){
        if(visX[i])
            Lx[i]-=minn;
        if(visY[i])
            Ly[i]+=minn;
    }
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));


    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=n;j++)
            Lx[i]=max(Lx[i],G[i][j]);
    }

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
        ans+=G[linkY[i]][i];

    return ans;
}
struct Node{
    int x;
    int y;
}nodeMan[N],nodeHouse[N];
int main(){
    int r,c;
    while(scanf("%d%d",&r,&c)!=EOF&&(r+c)){

        int numMan=0,numHouse=0;
        for(int i=1;i<=r;i++){
            for(int j=1;j<=c;j++){
                char c;
                cin>>c;
                if(c=='H'){
                    numHouse++;
                    nodeHouse[numHouse].x=i;
                    nodeHouse[numHouse].y=j;
                }
                if(c=='m'){
                    numMan++;
                    nodeMan[numMan].x=i;
                    nodeMan[numMan].y=j;
                }
            }
        }

        for(int i=1;i<=numMan;i++){
            for(int j=1;j<=numHouse;j++){
                int x=abs(nodeMan[i].x-nodeHouse[j].x);
                int y=abs(nodeMan[i].y-nodeHouse[j].y);
                G[i][j]=-(x+y);
            }
        }

        n=numMan;
        int res=KM();
        printf("%d\n",-res);
    }
    return 0;
}
```






