# Special Fish（HDU-3395） - Alex_McAvoy的博客 - CSDN博客





2018年12月06日 22:32:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

There is a kind of special fish in the East Lake where is closed to campus of Wuhan University. It’s hard to say which gender of those fish are, because every fish believes itself as a male, and it may attack one of some other fish who is believed to be female by it.

A fish will spawn after it has been attacked. Each fish can attack one other fish and can only be attacked once. No matter a fish is attacked or not, it can still try to attack another fish which is believed to be female by it.

There is a value we assigned to each fish and the spawns that two fish spawned also have a value which can be calculated by XOR operator through the value of its parents.

We want to know the maximum possibility of the sum of the spawns.

# **Input**

The input consists of multiply test cases. The first line of each test case contains an integer n (0 < n <= 100), which is the number of the fish. The next line consists of n integers, indicating the value (0 < value <= 100) of each fish. The next n lines, each line contains n integers, represent a 01 matrix. The i-th fish believes the j-th fish is female if and only if the value in row i and column j if 1.

The last test case is followed by a zero, which means the end of the input.

# Output

Output the value for each test in a single line.

# Sample Input

**31 2 30111011100**

# Sample Output

**6**


题意：给出一个 n 条鱼，每条鱼有一个价值 v，再给出一个 n*n 的布尔矩阵，（i,j）若为 1 则表示第 i 条鱼可能会与第 j 条鱼交配并产卵，且产卵的数量 = vi XOR vj，一条鱼只能交配 1 次、被交配一次，求这 n 条鱼产卵的最大数量

思路：每次产卵都是由一条鱼去找另一条鱼交配构成的，且任意两次交配活动，交配、被交配的鱼必然不同，左点集为交配的鱼，右点集为被交配的鱼，若左 i 会与右 j 交配，则建立一条边，边的权值为他们的产卵数量，若任两条鱼不会产卵，则建立一条 0 边，所以问题实质就是要找出一个最优匹配

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
int val[N];
int main(){
    while(scanf("%d",&n)!=EOF&&(n)){
        for(int i=1;i<=n;i++)
            scanf("%d",&val[i]);

        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                char c;
                cin>>c;
                if(c=='1')
                    G[i][j]=val[i]^val[j];
                else
                    G[i][j]=0;
            }
        }
        printf("%d\n",KM());
    }
    return 0;
}
```






