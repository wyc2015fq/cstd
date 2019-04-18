# HDU 2602 Bone Collector骨头收藏者（01背包） - xcw0754 - 博客园
# [HDU 2602 Bone Collector骨头收藏者（01背包）](https://www.cnblogs.com/xcw0754/p/4229980.html)
题意：收藏骨头。
思路： 常规的01背包。
```
1 #include <iostream>
 2 #define N 1005
 3 using namespace std;
 4 int volume[N];    //体积
 5 int value[N];    //价值
 6 int dp[N];    //总价值（动态更新）
 7 int max(int a,int b)
 8 {
 9     return a>b?a:b;
10 }
11 void cal(int n,int v)    //不断更新数组dp
12 {
13     int i,j;
14     for(i=1;i<=n;i++)
15         for(j=v;j-volume[i]>=0;j--)
16             dp[j]=max(dp[j],dp[j-volume[i]]+value[i]);    //不放or放    
17 }
18 void main()
19 {
20     int num;
21     scanf("%d",&num);
22     while(num--)
23     {
24         int n,v,j;
25         memset(dp,0,sizeof(dp));
26         scanf("%d%d",&n,&v);
27         for(j=1;j<=n;j++)
28             scanf("%d",&value[j]);
29         for(j=1;j<=n;j++)
30             scanf("%d",&volume[j]);
31         cal(n,v);
32         printf("%d\n",dp[v]);
33     }
34 }
AC代码
```

