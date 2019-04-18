# HDU 4745 Two Rabbits （区间DP） - xcw0754 - 博客园
# [HDU 4745 Two Rabbits （区间DP）](https://www.cnblogs.com/xcw0754/p/4895822.html)
题意：
　　两只兔子，在一个由n块石头围成的环上跳跃，每块石头有一个权值ai。开始时两兔站在同一石头上（也算跳1次），一只从左往右跳，一只从右往左跳，两只同时跳，而每跳一次，两只兔子所站的石头的权值都要相等，在一圈内（各自不能越过起点，也不能再次回到起点）它们（单只兔子）最多能跳多少次(1 <= n <= 1000, 1 <= ai <= 1000)。
思路：　　
　　此题要求的就是最长回文子序列（并不是子串），而最长回文子序列的算法复杂度为O(n*n)。但是由于是个环上，所以要挖掘一下环的性质。
　　假设两只兔子跳的是序列，而不是环，那么他们是这样跳的4<-3<-2<-1->2->3->4，一共跳了4次，颜色的不同代表了不同的兔子，那么现在将这个序列接起来，就是4321234的环了，在环上他们跳的次数是7次。可以发现其中一只兔子跳了1->2->3->4->4->3->2，而另一只是逆着跳而已。观察一下，其实算2->3->4就行了，下一段肯定是一个逆的过程（另一兔子所跳的）。那么就只需要算一个序列的最长回文子序列啦。注意起点必须是两只一块站着，所以这种情况下必定是个奇数。
　　如果序列并不是这样的呢？比如12213443，在序列中的某一处断开了，变成两段回文，由于是个环，所以是两段的最长回文子序列之和。枚举一下断开处就行了，注意只需要一个起点，即只要有一段是个奇数就行了。
　　注：这道题这样提交的话是WA的，但是代码是没有错的。而起点算两次的代码却可以AC。
　　WA的正确代码：
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=1005;
 4 int a[N], dp[N][N], n;
 5 
 6 int main( )
 7 {
 8     freopen("input.txt","r",stdin);
 9     while(scanf("%d",&n), n)
10     {
11         memset(dp,0,sizeof(dp));
12         for(int i=1; i<=n; i++)
13         {
14             scanf("%d", &a[i]);
15             dp[i][i]=1;
16         }
17         for(int j=2; j<=n; j++)
18         {
19             for(int i=j-1; i>0; i--)
20             {
21                 dp[i][j]=max(dp[i+1][j], dp[i][j-1]);
22                 if(a[i]==a[j])
23                     dp[i][j]=max(dp[i][j], dp[i+1][j-1]+2);
24             }
25         }
26         int ans=0;
27         for(int i=1; i<=n; i++)
28         {
29             int tmp=dp[1][i-1]+dp[i][n];
30             if( dp[1][i-1]%2 || dp[i][n]%2 )
31                 ans=max(ans, tmp);
32             else
33                 ans=max(ans, tmp-1);//两段都是偶数，必须减少1
34         }
35         cout<<ans<<endl;
36 }
37     return 0;
38 }
WA代码
```
　　AC的错误代码：
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=1005;
 4 int a[N], dp[N][N];
 5 
 6 int main( )
 7 {
 8     //freopen("input.txt","r",stdin);
 9     int n;
10     while(scanf("%d",&n), n)
11     {
12         for(int i=1; i<=n; i++)     scanf("%d",&a[i]);
13         for(int i=1; i<=n; i++)     dp[i][i]=1;
14         for(int j=2; j<=n; j++)
15         {
16             for(int i=j-1; i>0; i--)
17             {
18                 dp[i][j]=max(dp[i+1][j],dp[i][j-1]);
19                 if(a[i]==a[j])  dp[i][j]=max(dp[i][j], dp[i+1][j-1]+2);
20             }
21         }
22         int ans=0;
23         for(int i=1; i<=n; i++)
24             ans=max(ans, dp[1][i-1]+dp[i][n]);
25         cout<<ans<<endl;
26     }
27     return 0;
28 }
AC代码
```

