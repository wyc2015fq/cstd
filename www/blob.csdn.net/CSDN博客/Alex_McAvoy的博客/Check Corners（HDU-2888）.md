# Check Corners（HDU-2888） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:50:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30
个人分类：[HDU																[数据结构——RMQ](https://blog.csdn.net/u011815404/article/category/8671578)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Paul draw a big m*n matrix A last month, whose entries Ai,j are all integer numbers ( 1 <= i <= m, 1 <= j <= n ). Now he selects some sub-matrices, hoping to find the maximum number. Then he finds that there may be more than one maximum number, he also wants to know the number of them. But soon he find that it is too complex, so he changes his mind, he just want to know whether there is a maximum at the four corners of the sub-matrix, he calls this “Check corners”. It’s a boring job when selecting too many sub-matrices, so he asks you for help. (For the “Check corners” part: If the sub-matrix has only one row or column just check the two endpoints. If the sub-matrix has only one entry just output “yes”.)

# Input

There are multiple test cases. 

For each test case, the first line contains two integers m, n (1 <= m, n <= 300), which is the size of the row and column of the matrix, respectively. The next m lines with n integers each gives the elements of the matrix which fit in non-negative 32-bit integer. 

The next line contains a single integer Q (1 <= Q <= 1,000,000), the number of queries. The next Q lines give one query on each line, with four integers r1, c1, r2, c2 (1 <= r1 <= r2 <= m, 1 <= c1 <= c2 <= n), which are the indices of the upper-left corner and lower-right corner of the sub-matrix in question. 

# Output

For each test case, print Q lines with two numbers on each line, the required maximum integer and the result of the “Check corners” using “yes” or “no”. Separate the two parts with a single space.

# **Sample Input**

**4 44 4 10 72 13 9 115 7 8 2013 20 8 241 1 4 41 1 3 31 3 3 41 1 1 1**

# Sample Output

**20 no13 no20 yes4 yes**


题意：给出一个 n*m 的数字矩阵，然后给定 q 个询问，每次询问给出 r1、c1、r2、c2 四个数，要求输出以 r1、c1 为左上角 r2、c2 为右下角的子矩形的最大值，并且判断最大值是否是在这个子矩阵的角上

思路：二维 RMQ 模版题

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
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 301
#define LL long long
const int MOD=10;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int G[N][N];
int dpMax[N][N][9][9];
void initRMQ(int n,int m){

    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            dpMax[i][j][0][0]=G[i][j];

    for(int ii=0;(1<<ii)<=n;ii++){
        for(int jj=0;(1<<jj)<=m;jj++){
            if(ii+jj){
                for(int i=1;i+(1<<ii)-1<=n;i++){
                    for(int j=1;j+(1<<jj)-1<=m;j++){
                        if(ii)
                            dpMax[i][j][ii][jj] = max(dpMax[i][j][ii-1][jj] , dpMax[i+(1<<(ii-1))][j][ii-1][jj]);
                        else
                            dpMax[i][j][ii][jj] = max(dpMax[i][j][ii][jj-1] , dpMax[i][j+(1<<(jj-1))][ii][jj-1]);
                    }
                }
            }
        }
    }
}
int getMax(int x1,int y1,int x2,int y2){
    int k1=0,k2=0;
    while((1<<(k1+1))<=x2-x1+1)
        k1++;
    while((1<<(k2+1))<=y2-y1+1)
        k2++;

    x2=x2-(1<<k1)+1;
    y2=y2-(1<<k2)+1;
    int temp1=max(dpMax[x1][y1][k1][k2],dpMax[x1][y2][k1][k2]);
    int temp2=max(dpMax[x2][y1][k1][k2],dpMax[x2][y2][k1][k2]);

    return max(temp1,temp2);
}

int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                scanf("%d",&G[i][j]);
        initRMQ(n,m);

        int q;
        scanf("%d",&q);
        while(q--){
            int r1,c1,r2,c2;
            scanf("%d%d%d%d",&r1,&c1,&r2,&c2);
            int maxx=getMax(r1,c1,r2,c2);
            printf("%d ",maxx);

            if(G[r1][c1]==maxx || G[r1][c2]==maxx || G[r2][c1]==maxx || G[r2][c2]==maxx)
                printf("yes\n");
            else
                printf("no\n");
        }
    }
    return 0;
}
```






