# Assignment（HDU-2853） - Alex_McAvoy的博客 - CSDN博客





2018年12月10日 13:09:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Last year a terrible earthquake attacked Sichuan province. About 300,000 PLA soldiers attended the rescue, also ALPCs. Our mission is to solve difficulty problems to optimization the assignment of troops. The assignment is measure by efficiency, which is an integer, and the larger the better.

We have N companies of troops and M missions, M>=N. One company can get only one mission. One mission can be assigned to only one company. If company i takes mission j, we can get efficiency Eij. 

We have a assignment plan already, and now we want to change some companies’ missions to make the total efficiency larger. And also we want to change as less companies as possible.

# **Input**

For each test case, the first line contains two numbers N and M. N lines follow. Each contains M integers, representing Eij. The next line contains N integers. The first one represents the mission number that company 1 takes, and so on.

1<=N<=M<=50, 1<Eij<=10000.

Your program should process to the end of file.

# Output

For each the case print two integers X and Y. X represents the number of companies whose mission had been changed. Y represents the maximum total efficiency can be increased after changing.

# Sample Input

**3 32 1 33 2 41 26 22 1 32 31 2 31 2 31 2**

# Sample Output

**2 261 2**


题意： 给定一左点集有 n 个点，右点集有 m 个点的二分图，每条边有一个权值，输入给出一组匹配方案，现要求满足给定关系的中的最大匹配权值在原方案中增长了多少，并求出在原方案上最少改变了多少条边才得到这个新的匹配

思路：题目要求在原方案上改变最少的边得到最优匹配，实质就是优先用原匹配的边去构建最优匹配，套用模版即可

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

    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=m;j++)
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
    for(int i=1;i<=m;i++)
        if(linkY[i]!=-1)
            ans+=G[linkY[i]][i];

    return ans;
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                scanf("%d",&G[i][j]);
                G[i][j]=G[i][j]*(n+1);
            }
        }

        int oldVal=0;
        for(int i=1;i<=n;i++){
            int j;
            scanf("%d",&j);
            oldVal+=(G[i][j]/(n+1));
            G[i][j]++;
        }

        int ans=KM();
        printf("%d %d\n",n-ans%(n+1),ans/(n+1)-oldVal);
    }
    return 0;
}
```






