# HDU 5327 Olympiad （水题） - xcw0754 - 博客园
# [HDU 5327 Olympiad （水题）](https://www.cnblogs.com/xcw0754/p/4691461.html)
题意：beautiful数字定义为该数字中的十进制形式每一位都不同，给一个区间[L,R]，求该区间中有多少个beautiful数字。
思路：数字不大，直接暴力预处理，再统计区间[1,i]有多少个，用cnt[R]-cnt[L-1]即可。
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL long long
 5 using namespace std;
 6 const int N=100005;
 7 int has[12];
 8 int cnt[N];
 9 
10 void init()
11 {
12     for(int i=1; i<N; i++)
13     {
14         memset(has,0,sizeof(has));
15         int big=i;
16         while(big)//逐个拆出来
17         {
18             has[big%10]++;
19             big/=10;
20         }
21         int j=0;
22         for(; j<12; j++)
23             if(has[j]>1)    break;
24         if(j==12)   cnt[i]=1;
25     }
26     for(int i=1; i<N; i++)//统计i之前有多少个beautiful
27         cnt[i]+=cnt[i-1];
28 }
29 
30 int main()
31 {
32     //freopen("input.txt", "r", stdin);
33     init();
34     int t, a, b;
35     cin>>t;
36     while(t--)
37     {
38         scanf("%d%d",&a,&b);
39         printf("%d\n",cnt[b]-cnt[a-1]);
40     }
41     return 0;
42 }
AC代码
```

