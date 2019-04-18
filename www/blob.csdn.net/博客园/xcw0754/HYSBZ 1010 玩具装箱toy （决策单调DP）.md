# HYSBZ  1010  玩具装箱toy （决策单调DP） - xcw0754 - 博客园
# [HYSBZ  1010  玩具装箱toy （决策单调DP）](https://www.cnblogs.com/xcw0754/p/4866121.html)
题意：
　　有n个玩具，要将它们分为若干组，玩具长度C可能不同。给出n个玩具的摆放顺序，连续的任意多个玩具都可以成为一组。区间[i,j]成为一组的费用是cost=(j-i+Sigma(Ck)-L)2且i<=k<=j。给定n和L和每个玩具的长度，问分组后费用总和是多少? (n<=5*104)。
思路：
　　转移方程：dp[i]=min( dp[j]+(sum[i]-sum[j]+i-j+1-L)2  )。sum[i]表示前i件玩具长度的总和，0<j<i，（i-j+1）表示与i同组的玩具个数。
　　根据方程是可以推出这题是满足决策单调性的。以下是抄来的证明，稍微修改：
　　令f[i]=sum[i]+i, c=1+L，则dp[i]=min( dp[j]+(f[i]-f[j]-c)2  )
　　1.证明决策单调性
　　假设在状态i处的k决策优于j决策，且j<k，那么 dp[k]+(f[i]-f[k]-c)2<=dp[j]+(f[i]-dp[j]-c)2。
　　而对于i后面的某个状态t，设f[t]=f[i]+v，先不管v是多少。
　　要证明：dp[k]+(f[t]-f[k]-c)2<=dp[j]+(f[t]-f[j]-c)2
　　只要证(将f[t]=f[i]+v代入)：dp[k]+(f[i]+v-f[k]-c)2<=dp[j]+(f[i]+v-f[j]-c)2
　　只要证dp[k]+(f[i]-f[k]-c)2+2v*(f[i]-f[k]-c)+v2  <=  dp[j]+(f[i]-f[j]-c)2+2v*(f[i]-f[j]-c)+v2。
　　由于假设，所以只要证： 2v*(f[i]-f[k]-c)<=2v*(f[i]-f[j]-c)。
　　即证：f[k]>=f[j]（显然）
　　证明完毕
　　思路很明确，一直卡在二分上面，噗。
　　用一个队列来维护这些区间段，由于区间段必定是连在一起的，所以只需要记录左端点L以及更新这个区间的决策k。如果队列为空，则后面全部由i来更新得到，若非空，那么判断队尾的L，是否由i来更新会更优，若是，则pop掉队尾，继续同样的动作，直到队列为空或者i作为决策不如队尾的L更好，那么i可以更新的就是[L,n]之中的尾部区间[r,n]，而r可以用二分查找的方式。细节上很容易写挫，比如i决策可能完全都可以用武之地，不用二分去找了，否则会错；二分时必定要保证r由i来更新更佳，且有可能会出现等于的情况。复杂度O(nlogn)，斜率优化等再写。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <set>
 7 #include <deque>
 8 #include <map>
 9 #include <algorithm>
10 #include <vector>
11 #include <iostream>
12 #define pii pair<int,int>
13 #define back que[rear-1]
14 #define INF 0x7f7f7f7f
15 #define LL long long
16 #define ULL unsigned long long
17 using namespace std;
18 const double PI  = acos(-1.0);
19 const int N=50100;
20 
21 LL len[N], dp[N], L;
22 int q[N], d[N], n, l, r;   //区间以及决策
23 LL cost(int j,int i)        //用j来更新i的费用
24 {
25     return  dp[j]+(len[i]-len[j]-L)*(len[i]-len[j]-L);
26 }
27 
28 int find(int i,int k,int st)
29 {
30     int ll=st, rr=n;
31     while(ll<rr)
32     {
33         int mid=rr-(rr-ll+1)/2;
34         if( cost(i,mid)<cost(k,mid)) rr=mid;
35         else    ll=mid+1;
36     }
37     return rr;
38 }
39 LL cal()
40 {
41     l=r=1;
42     d[1]=0;q[1]=1;  //初始时，0可以更新[1,n]
43     for(int i=1; i<=n; i++)
44     {
45         dp[i]=cost(d[l], q[l]++);   //q[l]永远等于i
46         if( l<r && q[l]==q[l+1] )  l++;
47 
48         while( l<=r && cost(i,q[r])<cost(d[r],q[r]) )   r--;
49         if(l>r) //只能用i来更新
50         {
51             q[++r]=i+1;
52             d[r]=i;
53         }
54         else if( cost(i,n)<cost(d[r],n))
55         {
56             int tmp=find(i, d[r], q[r]);
57             q[++r]=tmp;
58             d[r]=i;
59         }
60     }
61     return dp[n];
62 }
63 
64 int main()
65 {
66     //freopen("input.txt","r",stdin);
67     while(~scanf("%d%lld",&n,&L))
68     {
69         L++;len[0]=0;
70         for(int i=1; i<=n; i++)
71         {
72             scanf("%lld",&len[i]);
73             len[i]+=len[i-1];
74         }
75         for(int i=1; i<=n; i++) len[i]+=i;
76         printf("%lld\n", cal() );
77     }
78     return 0;
79 }
AC代码
```

