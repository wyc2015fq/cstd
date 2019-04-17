# pairs（HDU-5178） - Alex_McAvoy的博客 - CSDN博客





2019年02月17日 21:12:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[HDU																[基础算法——尺取法](https://blog.csdn.net/u011815404/article/category/8676200)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

John has nn points on the X axis, and their coordinates are  (x[i],0),(i=0,1,2,…,n−1) . He wants to know how many pairs<a,b><a,b> that ![|x[b]-x[a]|\leqslant k,a< b](https://private.codecogs.com/gif.latex?%7Cx%5Bb%5D-x%5Ba%5D%7C%5Cleqslant%20k%2Ca%3C%20b)

# Input

The first line contains a single integer T (about 5), indicating the number of cases. 

Each test case begins with two integers n,k(1≤n≤100000,1≤k≤109). 

Next n lines contain an integer x[i](−109≤x[i]≤109), means the X coordinates.

# Output

For each case, output an integer means how many pairs<a,b> that ![|x[b]-x[a]|\leqslant k](https://private.codecogs.com/gif.latex?%7Cx%5Bb%5D-x%5Ba%5D%7C%5Cleqslant%20k)

# **Sample Input**

**25 5-10001001011025 300-1000100101102**

# Sample Output

**310**


题意：给出 n 个数，问有多少对 <a,b> 满足 ![|x[b]-x[a]|\leqslant k,a< b](https://private.codecogs.com/gif.latex?%7Cx%5Bb%5D-x%5Ba%5D%7C%5Cleqslant%20k%2Ca%3C%20b)

思路：尺取法

将 ![|x[b]-x[a]|\leqslant k](https://private.codecogs.com/gif.latex?%7Cx%5Bb%5D-x%5Ba%5D%7C%5Cleqslant%20k) 去掉绝对值，得：![x[a]-k \leq x[b] \leq x[a]-k](https://private.codecogs.com/gif.latex?x%5Ba%5D-k%20%5Cleq%20x%5Bb%5D%20%5Cleq%20x%5Ba%5D-k)

对 n 个数排序后使用，对相邻的数尺取即可

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
#define N 100001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
LL a[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        LL k;
        scanf("%d%lld",&n,&k);
        for(int i=1;i<=n;i++)
            scanf("%lld",&a[i]);

        sort(a+1,a+1+n);

        LL res=0;
        int l=1,r=1;
        while(l<=n){
            while(r+1<=n&&a[r+1]-a[l]<=k)
                r++;
            res+=(r-l);
            l++;
        }
        printf("%lld\n",res);
    }

    return 0;
}
```






