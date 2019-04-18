# CF 55D  Beautiful numbers （数位DP） - xcw0754 - 博客园
# [CF 55D  Beautiful numbers （数位DP）](https://www.cnblogs.com/xcw0754/p/4854539.html)
题意：
　　如果一个正整数能被其所有位上的数字整除，则称其为Beautiful number，问区间[L,R]共有多少个Beautiful number？（1<=L<=R<=9*1018）
思路：
　　数字很大，不能暴力。但是想要知道一个数是否为Beautiful number时，至少得等到它的所有位都出现吧？不然如何确定其实可以被整除的呢？
　　分析一下，类似2232和3232等这样的数字，这两个只是出现了2和3而已，他们的lcm都是6，所以有可以压缩统计的地方就是lcm，开一维来存储。接下来考虑前缀部分有没有什么可以压缩的地方，由于1~9的lcm最大是2520，那可以将前缀先模2520，最后再模那个数位的真正lcm就行了（2520必定是所有1~9中的任一组合的lcm的倍数，所以先取余是不会影响结果的），那么再开一维。所以状态为dp[位数][数位lcm][余2520的结果]，就可以将所有数字给归类到这3维里面了。数位lcm可以优化，打表发现仅有47位可能的lcm而已，所以不必开2520的大小。
```
1 #include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x7f3f3f3f
12 #define LL long long
13 #define ULL unsigned long long
14 using namespace std;
15 const double PI  = acos(-1.0);
16 const int N=20;
17 const int mod=2520;
18 int p[49]={0,1,2,3,4,5,6,7,8,9,10,12,14,15,18,20,21,24,28,
19 30,35,36,40,42,45,56,60,63,70,72,84,90,105,120,126,140,168,
20 180,210,252,280,315,360,420,504,630,840,1260,2520};
21 
22 int _lcm(int m,int n){return (m*n)/__gcd(m, n);}
23 LL f[N][49][mod+2], bit[N], has[mod+2];
24 
25 LL dfs(int i,int lcm,int left, bool e)
26 {
27     if(i==0) return lcm&&left%p[lcm]==0 ;
28     if(!e && ~f[i][lcm][left])    return f[i][lcm][left];
29 
30     LL ans=0;
31     int u= e? bit[i]: 9;
32     for(int d=0; d<=u; d++)
33     {
34         int t= lcm? has[_lcm(p[lcm], max(d,1))]: max(d,0);
35         ans+=dfs(i-1, t, (left*10+d)%mod, e&&d==u);
36     }
37     return e==true? ans: f[i][lcm][left]=ans;
38 }
39 
40 LL cal(LL n)
41 {
42     int len=0;
43     while(n)    //拆数
44     {
45         bit[++len]=n%10;
46         n/=10;
47     }
48     return dfs(len,0,0,true);
49 }
50 
51 int main()
52 {
53     //freopen("input.txt","r",stdin);
54     memset(f,-1,sizeof(f));
55     for(int i=0; i<49; i++)   has[p[i]]=i;
56 
57     LL L, R, t;cin>>t;
58     while( t-- )
59     {
60         cin>>L>>R;
61         cout<<cal(R)-cal(L-1)<<endl;
62     }
63     return 0;
64 }
AC代码
```

