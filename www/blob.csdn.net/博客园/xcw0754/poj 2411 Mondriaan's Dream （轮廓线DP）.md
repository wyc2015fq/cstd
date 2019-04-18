# poj 2411 Mondriaan's Dream （轮廓线DP） - xcw0754 - 博客园
# [poj 2411 Mondriaan's Dream （轮廓线DP）](https://www.cnblogs.com/xcw0754/p/4780083.html)
题意：有一个n*m的棋盘，要求用1*2的骨牌来覆盖满它，有多少种方案？（n<12，m<12）
思路：
　　由于n和m都比较小，可以用轮廓线，就是维护最后边所需要的几个状态，然后进行DP。这里需要维护的状态数就是min(n,m)。即大概是一行的大小。每次放的时候，只考虑（1）以当前格子为右方，进行横放；（2）以当前格子为下方进行竖放；（3）还有就是可以不放。
　　3种都是不一样的，所以前面的一种状态可能可以转为后面的几种状态，只要满足了条件。条件是，横放时，当前格子不能是最左边的；竖放时，当前格子不能是最上边的。而且要放的时候，除了当前格子，另一个格子也是需要为空才行的。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #define pii pair<int,int>
 6 #define INF 0x3f3f3f3f
 7 #define LL long long
 8 using namespace std;
 9 const int N=1e5+2;
10 LL dp[2][1<<15];
11 
12 LL cal(int n,int m)
13 {
14     if(n<m)     swap(n,m);
15     memset(dp, 0, sizeof(dp));
16     dp[0][(1<<m)-1]=1;
17     int cur=0;
18     int h=(1<<(m-1));
19     for(int i=0; i<n; i++)
20     {
21         for(int k=0; k<m; k++)
22         {
23             cur^=1;
24             memset(dp[cur], 0, sizeof(dp[cur]));
25             for(int j=0; j<(1<<m); j++)
26             {
27                 if( j&h )            dp[cur][(j^h)<<1]+=dp[cur^1][j];          //最高位为1时，可以不放
28                 if( k && !(j&1) && (h&j))  dp[cur][((j&(h-1))<<1)|3]+=dp[cur^1][j];      //放横，左边为0，上面为1
29                 if( i && !(h&j) )         dp[cur][(j<<1)|1]+=dp[cur^1][j];      //放竖，上面为0
30             }
31         }
32     }
33     return dp[cur][(1<<m)-1];
34 }
35 
36 int main()
37 {
38     freopen("input.txt", "r", stdin);
39     int n, m;
40     while(scanf("%d%d",&n,&m), n+m)    printf("%lld\n", cal(n,m));
41     return 0;
42 }
AC代码
```
　　UVA一样的题：http://acm.hust.edu.cn/vjudge/problem/viewProblem.action?id=33787
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #define pii pair<int,int>
 6 #define INF 0x3f3f3f3f
 7 #define LL long long
 8 using namespace std;
 9 const int N=12;
10 LL dp[2][1<<N];
11 
12 LL cal(int n,int m)
13 {
14     if(n<m)     swap(n,m);
15     memset(dp, 0, sizeof(dp));
16     dp[0][(1<<m)-1]=1;
17     int cur=0, h=(1<<(m-1));
18     for(int i=0; i<n; i++)
19     {
20         for(int k=0; k<m; k++)
21         {
22             cur^=1;
23             for(int j=0; j<(1<<m); j++) dp[cur][j]=0;
24             for(int j=0; j<(1<<m); j++)
25             {
26                 if( j&h )                  dp[cur][(j^h)<<1]+=dp[cur^1][j];          //最高位为1时，可以不放
27                 if( k && !(j&1) && (h&j))  dp[cur][((j&(h-1))<<1)|3]+=dp[cur^1][j];  //放横，左边为0，上面为1
28                 if( i && !(h&j) )          dp[cur][(j<<1)|1]+=dp[cur^1][j];          //放竖，上面为0
29             }
30         }
31     }
32     return dp[cur][(1<<m)-1];
33 }
34 
35 int main()
36 {
37     //freopen("input.txt", "r", stdin);
38     int n, m;
39     while(~scanf("%d%d",&n,&m))    printf("%lld\n", cal(n,m));
40     return 0;
41 }
AC代码
```

