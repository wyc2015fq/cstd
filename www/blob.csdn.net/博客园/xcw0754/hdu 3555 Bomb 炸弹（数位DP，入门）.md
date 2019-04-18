# hdu 3555 Bomb 炸弹（数位DP，入门） - xcw0754 - 博客园
# [hdu 3555 Bomb 炸弹（数位DP，入门）](https://www.cnblogs.com/xcw0754/p/4524521.html)
题意：
　　给一个数字n，求从1~n中有多少个数是含有49的，比如49，149，1490等都是含49的。
思路：
　　2^64也顶多是十进制的20多位，那么按十进制位来分析更简单。如果能计算k位十进制数中分别有多少个含49的，那么计算就简单了。
　　首先要求关于十进制位的一些信息，比如：i位的十进制数包含49有多少个，不包含49的多少个（除掉最高位是9的数量），不包含49但是最高位是9的有多少个（因为可能和更高一位组合成49）。注意精度，注意爆longlong。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=22;
 5 LL dp[N][4];
 6 int s[25];
 7 
 8 void init()
 9 {
10     dp[0][0]=1;
11     for(int i=1; i<N; i++)
12     {
13         dp[i][0]=dp[i-1][0]*10-dp[i-1][1];  //dp[i][0]代表长度为 i 并且不含有49的数字的个数；
14         dp[i][1]=dp[i-1][0];                //dp[i][1]代表长度为 i 并且不含有49，但是最高位是9的数字的个数；
15         dp[i][2]=dp[i-1][1]+dp[i-1][2]*10;　//dp[i][2]代表长度为 i 并且含有49的数字的个数。
16     }
17 }
18 
19 int main()
20 {
21     init();
22     freopen("input.txt", "r", stdin);
23     LL t, n;
24     cin>>t;
25     while(t--)
26     {
27         memset(s,0,sizeof(s));
28         scanf("%lld",&n);
29         int cnt=0;
30         n++;    //个位上必须大出1，方便计算出现紧挨着的49的情况。
31         while(n)
32         {
33             s[++cnt]=n%10;  //逐位提取出来
34             n/=10;
35         }
36         LL ans=0;
37         int last=0, flag=0;
38         for(int i=cnt; i>0; i--)    //具体就是要分析什么情况下会出现49，而且不能计算重复。
39         {
40             ans+=s[i]*dp[i-1][2];
41 
42             if(flag)    ans+=dp[i-1][0]*s[i];   //之前出现过紧挨着的49，那么第i位所可能出现的0~s[i]-1都与dp[i-1][0]个构成符合条件的数。
43 
44             if(!flag&&s[i]>4)    ans+=dp[i-1][1];   //s[i]大于4的情况，如果flag为true，那么都会在之前的紧挨49之后的那一步被统计掉。
45 
46             if(last==4&&s[i]==9)    flag=true;  //一旦flag开启，一直开启。目的是为了计算上限。
47 
48             last=s[i];
49         }
50         cout<<ans<<endl;
51     }
52     return 0;
53 }
AC代码
```
通用的解法。
```
1 //#include <bits/stdc++.h>
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
16 const int N=70;
17 
18 ULL dp[N][10];
19 int bit[N];
20 
21 void pre_cal()
22 {
23     dp[0][0]=1;
24     for(int i=1; i<=19; i++ )   //计算不含49的
25     {
26         dp[i][0]=10*dp[i-1][0]-dp[i-1][9];
27         dp[i][9]=dp[i-1][0];    //以9开头
28     }
29 }
30 
31 
32 ULL cal(ULL n)  //计算区间[0~n]吉利的数量
33 {
34     int len=0, i;
35     ULL big=n;
36     while(n)
37     {
38         bit[++len]=n%10;
39         n/=10;
40     }
41     bit[len+1]=0;
42     ULL ans=0;
43     for(i=len ;i>0; i--)
44     {
45         ans+=dp[i-1][0]*bit[i];
46         if(bit[i]>4)    ans-=dp[i-1][9];
47         if(bit[i+1]==4&&bit[i]==9)  break;
48     }
49     if(i==0)    ans++;  //n本身是否吉利？
50     return big-ans+1;
51 }
52 
53 int main()
54 {
55     //freopen("input.txt","r",stdin);
56     pre_cal();
57     int t;cin>>t;
58     ULL s;
59     while(t--)
60     {
61         scanf("%lld",&s);
62         printf("%llu\n",cal(s));
63     }
64     return 0;
65 }
AC代码
```

