# C++动态规划配对算法 - fanyun的博客 - CSDN博客
2018年07月29日 21:33:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：226
1、动态规划配对算法    
       空间里有n个点P0,P1,…,Pn-1，你的任务是把它们配成n/2对（n是偶数），使得每个点恰好在一个点对中。所有点对中两点的距离之和应尽量小。
2、思路      
      因为是对集合进行配对，自然需要记录当前集合的状态，老方法，二进制。 dp(s) = min(dist(i, j) + dp(s-i-j)) i是s集合中最小的元素的下标(最小最大都可以，只是作为检索起点)，j是s集合中其他元素的下标 s从1到1左移n再-1进行迭代，因为s-i-j必然小于s，所以直接迭代就可以，连递归都省去了。因为没有在oj上找到类似的，就百度上找到别人做的一组数据，直接copy了，暂做测试。如有问题，请大家帮忙支出。
```cpp
测试数据：
Input： 
20 
1 2 3 
1 1 1 
5 6 2 
4 7 8 
2 3 1 
1 4 7 
2 5 8 
3 6 9 
1 2 5 
2 3 6 
4 5 2 
7 8 5 
4 5 1 
-1 2 3 
-1 -9 -7 
0 0 0 
100 0 0 
9 5 1 
7 5 3 
5 5 5 
Output： 
119.076
```
```cpp
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <map>
using namespace std;
#define LL long long
const int N = 29;
int x[N], y[N], z[N];
double dist[N][N];
double dp[(1<<N)];
void init_dist(int n)
{
    for(int i=0; i<n; i++)
        for(int j=i+1; j<n; j++)
            dist[i][j] = sqrt((x[i]-x[j])*(x[i]-x[j])
                +(y[i]-y[j])*(y[i]-y[j])
                +(z[i]-z[j])*(z[i]-z[j]));
}
double solve(int n)
{
    for(int s=1; s<(1<<n); s++)
    {
        dp[s] = 1E9;
        int pos = 0;
        while(pos<n && !(s&(1<<pos))) ++pos;
        for(int i=pos+1; i<n; i++)
            if(s&(1<<i))
                dp[s] = min(dp[s], dist[pos][i]+dp[s^(1<<pos)^(1<<i)]);
    }
    return dp[(1<<n)-1];
}
int main()
{
    int n;
    cin>>n;
    for(int i=0; i<n; i++)
        cin>>x[i]>>y[i]>>z[i];
    init_dist(n);
    dp[0] = 0;
    cout<<solve(n)<<endl;
    return 0;
}
```
