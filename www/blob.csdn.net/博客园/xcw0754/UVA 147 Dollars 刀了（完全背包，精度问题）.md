# UVA 147 Dollars 刀了（完全背包，精度问题） - xcw0754 - 博客园
# [UVA 147 Dollars 刀了（完全背包，精度问题）](https://www.cnblogs.com/xcw0754/p/4489920.html)
题意:一样是求钱的转换方案数，但是这次单位下降到分，但给的是元为单位的，所以是浮点的，但是固定有两位小数。
思路：数据都放大100倍来计算，去除精度问题，转成整型时要注意精度。即使给的是0.02，乘以100后的结果不一定是2，而是2左右，所以再加上一个很小的数再转即可，比如0.0001；
```
1 #include <iostream>
 2 #include <cstdio>
 3 #define LL long long
 4 using namespace std;
 5 const int  N=30002;
 6 int coin[]={5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
 7 LL dp[N+10]={1};
 8 void cal()
 9 {
10     for(int i=0; i<11; i++)
11         for(int j=0; j+coin[i]<=N; j++ )
12             dp[j+coin[i]]+=dp[j];
13 }
14 int main() 
15 {
16     //freopen("input.txt", "r", stdin);
17     cal();
18     double n;
19     while(scanf("%lf",&n),n!=0.00)
20         printf("%6.2lf%17lld\n", n, dp[int(n*100+0.001)] );//重点在精度
21     return 0;
22 }
AC代码
```

