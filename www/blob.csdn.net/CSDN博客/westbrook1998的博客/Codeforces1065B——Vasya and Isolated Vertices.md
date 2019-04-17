# Codeforces1065B——Vasya and Isolated Vertices - westbrook1998的博客 - CSDN博客





2018年10月13日 15:38:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：81








> 
题目省略


前半个小时想了个假思路，后面找到正确思路，然而想错了int的范围，而且没考虑到for循环的i变量如果是int的话可能会直接爆，卡题到结束

题意是给n个点m条边，然后这个图保证没有自环和重边，然后求最大和最小可能的孤立点个数，最小的其实就是一条边尽量消耗两个点，就是n-2*m，最大就是找到k个点的完全图，然后m条边先减去这k阶完全图的边数k*(k-1)/2，剩下的边又另一个点连向这个k阶完全图

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
long long n,m;
const int N=1e6+10;
long long a[N];
int main(void){
    a[0]=0;
    for(long long i=1;i<N;i++){
        a[i]=i*(i-1)/2;
    }
    scanf("%I64d%I64d",&n,&m);
    long long Min=max((long long)0,n-2*m);
    long long k=lower_bound(a,a+n,m)-a;
    long long Max=n-k;
    printf("%I64d %I64d\n",Min,Max);
    return 0;
}
```



