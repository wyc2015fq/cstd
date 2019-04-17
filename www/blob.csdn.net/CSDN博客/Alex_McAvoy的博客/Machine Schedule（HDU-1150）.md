# Machine Schedule（HDU-1150） - Alex_McAvoy的博客 - CSDN博客





2019年03月12日 14:52:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

As we all know, machine scheduling is a very classical problem in computer science and has been studied for a very long history. Scheduling problems differ widely in the nature of the constraints that must be satisfied and the type of schedule desired. Here we consider a 2-machine scheduling problem. 

There are two machines A and B. Machine A has n kinds of working modes, which is called mode_0, mode_1, ..., mode_n-1, likewise machine B has m kinds of working modes, mode_0, mode_1, ... , mode_m-1. At the beginning they are both work at mode_0. 

For k jobs given, each of them can be processed in either one of the two machines in particular mode. For example, job 0 can either be processed in machine A at mode_3 or in machine B at mode_4, job 1 can either be processed in machine A at mode_2 or in machine B at mode_4, and so on. Thus, for job i, the constraint can be represent as a triple (i, x, y), which means it can be processed either in machine A at mode_x, or in machine B at mode_y. 

Obviously, to accomplish all the jobs, we need to change the machine's working mode from time to time, but unfortunately, the machine's working mode can only be changed by restarting it manually. By changing the sequence of the jobs and assigning each job to a suitable machine, please write a program to minimize the times of restarting machines. 

# **Input**

The input file for this program consists of several configurations. The first line of one configuration contains three positive integers: n, m (n, m < 100) and k (k < 1000). The following k lines give the constrains of the k jobs, each line is a triple: i, x, y. 

The input will be terminated by a line containing a single zero. 

# Output

The output should be one integer per line, which means the minimal times of restarting machine.

# Sample Input

**5 5 100 1 11 1 22 1 33 1 44 2 15 2 26 2 37 2 48 3 39 4 30**

# Sample Output

**3**


题意：给出两台机器 A、B，分别有 m、n 种模式，初始都在模式 0，现给出 k 个工作及其所需要的模式，每个工作都可将 A 设为模式 i 或将 B 设为模式 j，但每一次更换模式需要重启机器，求完成所有工作的最小重启数

思路：初始模式都是 0，对于任务可以在 0 模式下完成的可以不考虑，只需考虑模式 A 的 1~n-1 与模式 B 的 1~m-1 下的任务，将 A 的模式看做左点集，将 B 的模式看做右点集，每个点代表重启一次，对于每个任务（i,j）连接左 i 右 j 即可，现在想让机器重启次数最小，就是要在左边与右边选出总数最少的点，让这些点覆盖所有边，实质就是求该二分图的最小点覆盖数

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=1E9+7;
const int N=10000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
vector<int> G[N];//存边
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(!vis[y]){//不在交替路中
            vis[y]=true;//放入交替路
            if(link[y]==-1 || dfs(link[y])){//如果是未匹配点，说明交替路是增广路
                link[y]=x;//交换路径
                return true;//返回成功
            }
        }
    }
    return false;//不存在增广路，返回失败
}
int hungarian(int x){
    int ans=0;//记录最大匹配数
    for(int i=1;i<=x;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}
int main(){
    int n,m,k;
    while(scanf("%d",&n)!=EOF&&(n)){
        scanf("%d%d",&m,&k);
        memset(link,-1,sizeof(link));
        for(int i=0;i<n;i++)
            G[i].clear();

        while(k--){
            int i,x,y;
            scanf("%d%d%d",&i,&x,&y);
            if(x==0||y==0)
                continue;
            G[x].push_back(y);
        }

        printf("%d\n",hungarian(n-1));
    }
    return 0;
}
```




