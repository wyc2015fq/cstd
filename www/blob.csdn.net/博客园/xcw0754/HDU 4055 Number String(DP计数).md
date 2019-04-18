# HDU 4055 Number String(DP计数) - xcw0754 - 博客园
# [HDU 4055 Number String(DP计数)](https://www.cnblogs.com/xcw0754/p/4471344.html)
题意：
　　给你一个含n个字符的字符串，字符为'D'时表示小于号，字符为“I”时表示大于号，字符为“？”时表示大小于都可以。比如排列 {3, 1, 2, 7, 4, 6, 5} 表示为字符串 DIIDID。任务是计算所有能产生给定字符串的序列数量，每个序列含n+1个数字，分别为1～n+1，即从1开始且不重复。
思路：DP计数。如下步骤
1）将规模n降低，使得对于每个i （1<=i<=n）都可以依靠i-1的结果来计算。最小规模为1个符号，决定两个数字的序列。
2）考虑对于具有i个数字的序列（值从1～i），指定第i个数字为j （1<=j<=i+1），计算所产生的序列数量。那么dp[i][j]表示着具有i个数字的序列，若最后一个数字为j时，所有可能的序列数量。那么dp[i][i+1]就统计了具有i个数字的且满足前i个要求的序列数量。
3）分两种情况：
第一种是第i个为I（小于号，升序）：dp[i][j] = dp[i-1][j-1] + dp[i-1][j-2] + .. + dp[i-1][1]。
另一种是第i个为D（大于号，降序）： dp[i][j] = dp[i-1][i] + dp[i-1][i-1] + ... + dp[i-1][j]。
　　分析第一个式子，假设前面i个数字所能产生的可能已经知道，那么直接将j加在最后面就行了。比如：序列3>2<4>1  共4个数字和3个符号，假设第4个符号为<，那么现在考虑第5个数字，要算的有dp[4][1] dp[4][2] dp[4][3] dp[4][4] dp[4][5]共5个。dp[4][1]可以想象将1接在序类后，即 3 2 4 1 1，疑问？这不是重复了吗？这样子我们可以将前4个数字中所有大于第5个数字的都加1，就变成4 3 5 2 1，这个序列就合法了吧？是的，对于所有的4个数字的合法序列都这么做。前提是对于第4个数字小于5的所有4数字序列才可以这么考虑。大于等于5的序列再来加1也必定重复。
　　分析第二个式子，同样，假设前面i个数字所能产生的可能已经知道，也是加在后面。比如：序列3>1<2 共3个数字和2个符号，假设第3个符号为>，那么现在考虑第4个数字，2大于1，同时还得考虑如上“加一”情况，所以1和2都要考虑。那么要计算的有dp[3][1] dp[3][2] 共2个，dp[3][3]对于我们所假设的情况是非法的。因为大于等于2的都要加一，所以dp[3][2]可能产生的序列之一为 { 3+1, 1,2+1, 2 } 即 {  4, 1,3 ,2 } 。合法！对于其他情况依此法计算。
下面代码实现时利用了前缀和，所以并不是单纯的逐个加，有点技巧在里面，还没领悟透其中原理。
```
1 #include <iostream>
 2 #include <cstdio>
 3 #include <cstring>
 4 #define LL long long
 5 using namespace std;
 6 const int mod=1000000007;
 7 const int N=1005;
 8 char a[N];
 9 LL sum[N][N];
10 
11 
12 int main()
13 {
14     //freopen("input.txt", "r", stdin);
15     while(cin>>a)
16     {
17         int len=strlen(a);
18         sum[0][1] = 1;        //初始化：没有符号时，假设为只有1个数字，那么就只有序列”1“而已了，结果为1种。
19         
20 
21         for(int i=1; i<=len; i++)    //对于第i个符号，就有具有i+1个数字的序列。注：i+1个数字的序列就是1～i+1共i+1个，没有大于i+1的数字。
22         {
23             for(int j=1; j<=i+1; j++)    
24             {
25             
26                 sum[i][j] = sum[i][j-1];    //继承第i+1位为j-1的所有情况。
27                 
28                 if(a[i-1]!='D')            //第i个符号是 '<' 。 若是'?'则两个if都执行
29                     sum[i][j] += sum[i-1][j-1];
30 
31                 if(a[i-1]!='I')            //第i个符号是 '>' 
32                     sum[i][j] += sum[i-1][i] - sum[i-1][j-1] + mod ;     //可能出现负的，所以要加mod
33 
34                 sum[i][j] %= mod;
35             }
36         }
37         cout<<sum[len][len+1]<<endl;
38     }
39     return 0;
40 }
AC代码
```
下面是我的代码，为了好理解不作优化，运算次数应该比上面的少。
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <stdlib.h>
 5 using namespace std;
 6 const int N=1005;
 7 const int mod=1000000007;
 8 unsigned int sum[N][N];
 9 char s[N];
10 int main()
11 {
12     //freopen("input.txt","r",stdin);
13     while(cin>>s)
14     {
15         memset(sum, 0, sizeof(sum));
16         int len=strlen(s);
17         sum[0][1]=1;
18 
19         for(int i=1; i<=len; i++)       //对于len个符号，有len+1个数字组成。
20         {
21             int siz=i+2;
22             if(s[i-1]=='I')
23             {
24                 for(int j=1; j<siz; j++)    //从小往大的方向累加。
25                 {
26                     sum[i][j]=sum[i][j-1];
27                     sum[i][j]=((long long)sum[i][j]+ sum[i-1][j-1])%mod;
28                 }
29             }
30             else if(s[i-1]=='D')
31             {
32                 for(int j=i+1; j>0; j--)        //从大往小的方向累加。
33                 {
34                     sum[i][j]=sum[i][j+1];
35                     sum[i][j]=((long long)sum[i][j]+sum[i-1][j])%mod;
36                 }
37             }
38             else        //问号，将上一个位置所有可能相加。
39             {
40                 long long tmp=0;
41                 for(int j=1; j<=i; j++)
42                     tmp=((long long)tmp+sum[i-1][j])%mod;
43                 for(int j=1; j<siz; j++)
44                     sum[i][j]=tmp;
45             }
46         }
47         long long ans=0;
48         for(int i=1; i<=len+1; i++)     //累计所有可能。
49             ans=((long long)ans+sum[len][i])%mod;
50         cout<<ans<<endl;
51     }
52     return 0;
53 }
int型代码
```
```
1 #include <iostream>
 2 #include <stdio.h>
 3 #include <string.h>
 4 #include <stdlib.h>
 5 using namespace std;
 6 const int N=1005;
 7 const int mod=1000000007;
 8 long long sum[N][N];
 9 char s[N];
10 
11 int main()
12 {
13 
14     //freopen("input.txt","r",stdin);
15     while(cin>>s)
16     {
17         memset(sum, 0, sizeof(sum));
18         int len=strlen(s);
19         sum[0][1]=1;
20 
21         for(int i=1; i<=len; i++)
22         {
23             int siz=i+2;
24             if(s[i-1]=='I')
25             {
26                 for(int j=1; j<siz; j++)
27                 {
28                     sum[i][j]=sum[i][j-1];
29                     sum[i][j]+=sum[i-1][j-1]%mod;
30                 }
31             }
32             else if(s[i-1]=='D')
33             {
34                 for(int j=i+1; j>0; j--)
35                 {
36                     sum[i][j]=sum[i][j+1];
37                     sum[i][j]+=sum[i-1][j]%mod;
38                 }
39             }
40             else
41             {
42                 long long tmp=0;
43                 for(int j=1; j<=i; j++)
44                     tmp+=sum[i-1][j]%mod;
45                 for(int j=1; j<siz; j++)
46                     sum[i][j]=tmp;
47             }
48         }
49         long long ans=0;
50         for(int i=1; i<=len+1; i++)
51             ans+=sum[len][i]%mod;
52         cout<<ans%mod<<endl;
53     }
54     return 0;
55 }
longlong型代码（好理解）
```

