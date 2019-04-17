# Factors and Multiples（LightOJ-1149） - Alex_McAvoy的博客 - CSDN博客





2019年01月23日 20:48:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
个人分类：[LightOJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

You will be given two sets of integers. Let's call them set A and set B. Set A contains n elements and set B contains m elements. You have to remove k1 elements from set A and k2 elements from set B so that of the remaining values no integer in set B is a multiple of any integer in set A. k1 should be in the range [0, n] and k2 in the range [0, m].

You have to find the value of (k1 + k2) such that (k1 + k2) is as low as possible. P is a multiple of Q if there is some integer K such that P = K * Q.

![](https://img-blog.csdnimg.cn/20190130122819823.jpg)

Suppose set A is {2, 3, 4, 5} and set B is {6, 7, 8, 9}. By removing 2 and 3 from A and 8 from B, we get the sets {4, 5} and {6, 7, 9}. Here none of the integers 6, 7 or 9 is a multiple of 4 or 5.

So for this case the answer is 3 (two from set A and one from set B).

# Input

Input starts with an integer T (≤ 50), denoting the number of test cases.

The first line of each case starts with an integer n followed by n positive integers. The second line starts with m followed by m positive integers. Both n and m will be in the range [1, 100]. Each element of the two sets will fit in a 32 bit signed integer.

# Output

For each case of input, print the case number and the result.

# Sample Input

**24 2 3 4 54 6 7 8 93 100 200 3001 150**

# Sample Output

**Case 1: 3Case 2: 0**


题意：t 组数据，每组给出一 n 个数组成的序列 A 与 m 个数组成的序列 B，现要使得 A 中的数与 B 中的数都没有整除关系，问最少去掉 A、B 中的多少个数，

思路：二分图最大匹配问题

由于要求 A 与 B 中的数无整除关系，因此可对 B 中的 m 个数向 A 中的 n 个数可以整除的建一条边，而后匈牙利算法求最大匹配即可

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
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 101
#define LL long long
using namespace std;
int n,m;
int a[N],b[N];
int G[N][N];
int link[N];
bool vis[N];

bool dfs(int x){
    for(int i=1;i<=n;i++){
        if(!vis[i]&&G[x][i]){
            vis[i]=true;
            if(link[i]==-1 || dfs(link[i])){
                link[i]=x;
                return true;
            }
        }
    }
    return false;
}
int hungarian(){
    int ans=0;
    memset(link,-1,sizeof(link));
    for(int i=1;i<=m;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}

int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        memset(G,0,sizeof(G));

        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);
        scanf("%d",&m);
        for(int i=1;i<=m;i++)
            scanf("%d",&b[i]);

        for(int i=1;i<=m;i++)
            for(int j=1;j<=n;j++)
                if(b[i]%a[j]==0)
                    G[i][j]=1;

        printf("Case %d: %d\n",Case++,hungarian());
    }
	return 0;
}
```






