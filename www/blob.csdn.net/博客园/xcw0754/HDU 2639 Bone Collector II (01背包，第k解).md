# HDU 2639 Bone Collector II (01背包，第k解) - xcw0754 - 博客园
# [HDU 2639 Bone Collector II (01背包，第k解)](https://www.cnblogs.com/xcw0754/p/4480730.html)
题意：
　　数据是常规的01背包，但是求的不是最大容量限制下的最佳解，而是第k佳解。
思路：
　　有两种解法：
1）网上普遍用的O(V*K*N)。
2）先用常规01背包的方法求出背包容量限制下能装的最大价值m，再以m为背包容量再进行一次01背包，dp[j]表示当物品的组合价值为j时，它们的体积之和的最小量。那么就求出了所有可能的价值，从1～m都有，但是其中一些是求不出来的，也就是骨头的价值不能组合成这个数字，那么就得过滤掉。
```
1 #include <iostream>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <limits.h>
 5 using namespace std;
 6 int N, V, K, dp[1000000], w[105], v[105];
 7 
 8 void cal()
 9 {
10     for(int i=0; i<N; i++)
11     {
12         for(int j=V; j>=v[i]; j--)
13             dp[j] = max( dp[j], dp[j-v[i]]+w[i] );
14     }
15     if(K==1)
16     {
17         cout<<dp[V]<<endl;
18         return ;
19     }
20     
21     int m=dp[V];
22     int flag=(1<<30)+(1<<29);
23     dp[0]=0;
24     for(int i=1; i<=m; i++)
25         dp[i]=flag;
26     
27     for(int i=0; i<N; i++)
28     {
29         for(int j=m; j>=w[i]; j--)
30         {
31             if(dp[j-w[i]]<flag)
32                 dp[j]=min(dp[j], dp[j-w[i]]+v[i]);
33             
34         }
35     }
36 
37     int cnt=0;
38     for(int i=m; i>0; i--)
39     {
40         if(dp[i]!=flag && dp[i]<=V )
41         {
42             cnt++;
43             if(cnt==K)
44             {
45                 cout<<i<<endl;
46                 return ;
47             }
48         }
49     }
50     cout<<"0"<<endl;
51 }
52 
53 int main()
54 {
55     //freopen("input.txt","r",stdin);
56     int t;
57     cin>>t;
58     while(t--)
59     {
60         memset(dp,0,sizeof(dp));
61         cin>>N>>V>>K;
62         for(int i=0; i<N; i++)
63             cin>>w[i];
64         for(int i=0; i<N; i++)
65             cin>>v[i];
66         cal();
67 
68     }
69     return 0;
70 }
AC代码（第2种解法）
```

