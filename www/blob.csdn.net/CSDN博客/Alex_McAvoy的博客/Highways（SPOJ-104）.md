# Highways（SPOJ-104） - Alex_McAvoy的博客 - CSDN博客





2019年04月08日 21:20:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：30








> 
# Problem Description

In some countries building highways takes a lot of time... Maybe that's because there are many possiblities to construct a network of highways and engineers can't make up their minds which one to choose. Suppose we have a list of cities that can be connected directly. Your task is to count how many ways there are to build such a network that between every two cities there exists exactly one path. Two networks differ if there are two cities that are connected directly in the first case and aren't in the second case. At most one highway connects two cities. No highway connects a city to itself. Highways are two-way.

# **Input**

The input begins with the integer t, the number of test cases (equal to about 1000). Then t test cases follow. The first line of each test case contains two integers, the number of cities (1<=n<=12) and the number of direct connections between them. Each next line contains two integers a and b, which are numbers of cities that can be connected. Cities are numbered from 1 to n. Consecutive test cases are separated with one blank line.

# Output

The number of ways to build the network, for every test case in a separate line. Assume that when there is only one city, the answer should be 1. The answer will fit in a signed 64-bit integer.

# Sample Input

**44 53 44 22 31 21 3**

**2 12 1**

**1 0**

**3 31 22 33 1**

# Sample Output

**8113**


题意：t 组数据，n 个点 m 条边，求最小生成树个数

思路：Matrix-Tree 定理模版题

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
#define E 1e-12
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=1000000007;
const int N=10+5;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;

LL K[N][N];
LL det(int n){//求矩阵K的n-1阶顺序主子式
    LL res=1;
    for(int i=1;i<=n-1;i++){//枚举主对角线上第i个元素
        for(int j=i+1;j<=n-1;j++){//枚举剩下的行
            while(K[j][i]){//辗转相除
                int t=K[i][i]/K[j][i];
                for(int k=i;k<=n-1;k++)//转为倒三角
                    K[i][k]=K[i][k]-t*K[j][k];
                swap(K[i],K[j]);//交换i、j两行
                res=-res;//取负
            }
        }
        res=res*K[i][i];
    }
    return abs(res);
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);
        memset(K,0,sizeof(K));
        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            K[x][x]++;
            K[y][y]++;
            K[x][y]--;
            K[y][x]--;
        }
        printf("%lld\n",det(n));
    }
    return 0;
}
```





