# Chocolate（HDU-2282） - Alex_McAvoy的博客 - CSDN博客





2018年12月07日 16:44:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：145
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Lethe loves eating chocolates very much. In Lethe's birthday, her good friend echo brings N boxes to her, and makes the boxes on the circle. Furthermore, echo tells Lethe that there are many chocolates in the boxes, but the total number of chocolates doesn't exceed N. Also, echo wants Lethe to displace the chocolates in such a way that in each box remains no more than one chocolate. In one move she can shift one chocolate from current box to its neighboring box. (Each box has two neighboring boxes). Can you tell Lethe the minimum number of move to achieve this goal?

# **Input**

There are multi-cases (The total number of cases won't exceed 20). First line is an integer N(1<=N<=500), the total number of boxes. Then N lines follow, each line includes a number, indicates the number of chocolates in the box.

# Output

Output the minimum number of move.

# Sample Input

**101330020000**

# Sample Output

**9**


题意：给出 n 个盒子，盒子摆成环，每个盒子中有一定数量的巧克力，且所有盒子内巧克力的总数小于 n，每次移动可以将一块巧克力从第 i 格移动到第 i-1 或 i+1 格，问最少移动几次能将使得每个盒子的巧克力数小于等于 1

思路：类似 [Going Home（POJ-2195）](https://blog.csdn.net/u011815404/article/details/84865221)要求边权值和最小的完全匹配，不过图是一个环，因此存储距离时要注意取最小值的求法

左点集为要移动的巧克力，右点集为空盒子，若一个巧克力 i 能被移动到 j，那么就在左 i 右 j 连一条权值为两者最小距离的边，由于所有盒子连接成一个环，因此要注意最小距离的求法，从第 i 个盒子移动到第 j 个盒子的最小步数是 min(n-abs(i-j),abs(i-j))，由于要求图的最小权值的完全匹配，因此使用 KM 算法时注意将边的权值存为负数，最后得到的答案取负就是最优匹配

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
int n,m;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=m;y++){
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
            for(int j=1;j<=m;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=m;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));
    memset(Lx,0,sizeof(Lx));
    memset(Ly,0,sizeof(Ly));

    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
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
    for(int i=1;i<=m;i++)
        if(linkY[i]!=-1)
            ans+=G[linkY[i]][i];

    return ans;
}
int chocolate[N],box[N];
int numChocolate,numBox;
int main(){
    while(scanf("%d",&n)!=EOF&&(n)){
        numChocolate=numBox=0;
        memset(chocolate,0,sizeof(chocolate));
        memset(box,0,sizeof(box));

        for(int i=1;i<=n;i++){
            int x;
            scanf("%d",&x);
            if(x>1){
                while(x>1){
                    numChocolate++;
                    chocolate[numChocolate]=i;
                    x--;
                }
            }
            else if(x==0){
                numBox++;
                box[numBox]=i;
            }
        }

        for(int i=1;i<=numChocolate;i++){
            for(int j=1;j<=numBox;j++){
                int x=abs(chocolate[i]-box[j]);
                int y=n-abs(chocolate[i]-box[j]);
                G[i][j]=-min(x,y);
            }
        }

        n=numChocolate;
        m=numBox;

        printf("%d\n",-KM());
    }
    return 0;
}
```






