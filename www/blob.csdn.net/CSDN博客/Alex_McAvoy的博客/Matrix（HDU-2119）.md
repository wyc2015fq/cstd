# Matrix（HDU-2119） - Alex_McAvoy的博客 - CSDN博客





2019年03月13日 09:25:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：15
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Give you a matrix(only contains 0 or 1),every time you can select a row or a column and delete all the '1' in this row or this column . 

Your task is to give out the minimum times of deleting all the '1' in the matrix.

# **Input**

There are several test cases. 

The first line contains two integers n,m(1<=n,m<=100), n is the number of rows of the given matrix and m is the number of columns of the given matrix. 

The next n lines describe the matrix:each line contains m integer, which may be either ‘1’ or ‘0’. 

n=0 indicate the end of input. 

# Output

For each of the test cases, in the order given in the input, print one line containing the minimum times of deleting all the '1' in the matrix. 

# Sample Input

**3 3 0 0 01 0 10 1 00**

# Sample Output

**2**


题意：给出一个 n*m 的 01 矩阵，每次只能删除一行或者一列的 1，问最少多少次能删除完矩阵中的 1

思路：

将行号作为左点集，列号作为右点集，若 (i,j) 为 1，则建一条左 i 右 j 的无向边，那么问题就转化为了求二分图的最小覆盖点集，使得每条边都至少被一个点覆盖，由于**最小覆盖点集=最大匹配数**，因此建好图后跑匈牙利即可

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
const int N=1000+5;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int n,m;
bool vis[N];//vis[i]表示是否在交替路中
int link[N];//存储连接点
int G[N][N];//存边
bool dfs(int x){
    for(int y=1;y<=m;y++){//对x的每个邻接点
        if(G[x][y]==1&&!vis[y]){//不在交替路中
            vis[y]=true;//放入交替路
            if(link[y]==-1 || dfs(link[y])){//如果是未匹配点，说明交替路是增广路
                link[y]=x;//交换路径
                return true;//返回成功
            }
        }
    }
    return false;//不存在增广路，返回失败
}
int hungarian(){
    int ans=0;//记录最大匹配数
    memset(link,-1,sizeof(link));
    for(int i=1;i<=n;i++){//从左侧开始每个结点找一次增广路
        memset(vis,false,sizeof(vis));
        if(dfs(i))//找到一条增广路，形成一个新匹配
            ans++;
    }
    return ans;
}

int main(){

    while(scanf("%d",&n)!=EOF&&(n)){
        memset(G,0,sizeof(G));
        scanf("%d",&m);

        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                scanf("%d",&G[i][j]);

        printf("%d\n",hungarian());
    }
    return 0;
}
```






