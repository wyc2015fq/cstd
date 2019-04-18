# UESTC 1307 WINDY数  （数位DP，基础） - xcw0754 - 博客园
# [UESTC 1307 WINDY数  （数位DP，基础）](https://www.cnblogs.com/xcw0754/p/4852649.html)
题意：
　　windy定义了一种windy数。不含前导零且相邻两个数字之差至少为2的正整数被称为windy数。windy想知道，在A和B之间，包括A和B，总共有多少个windy数？
思路：
　　就是给连续的两位数字之间一些限制而已。主要还是放在推数量的问题上。相信很容易能写出转移方程，但是本题的问题在于前导零问题，不知道你是如何统计开头为0的，比如dp[i][0]表示什么？如果就要统计区间[0,10]，输出的dp[2][0]会是8吗？（即02,03,04,05,06,07,08,09），其实你忘记了统计00和01，因为他们和前导0冲突了。所以想办法解决这个问题就行了。
　　由于本题找不到提交源头了，码出来不保证正确性。
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
13 #define LL unsigned long long
14 using namespace std;
15 const double PI  = acos(-1.0);
16 const int N=10;
17 int dp[N][N], sum[N], bit[N+5];
18 
19 void pre_cal()
20 {
21     sum[1]=10;
22     for(int i=0; i<N; i++) dp[1][i]=1;
23     for(int i=2; i<N; i++ ) //位数
24     {
25         sum[i]+=sum[i-1];
26         for(int j=1; j<N; j++)      sum[i]+=dp[i-1][j];//不含前导0
27         for(int j=0; j<N; j++)  //以j开头
28         {
29             for(int k=0; k<N; k++)  //下一个位以k开头
30                 if(abs(j-k)>=2)
31                     dp[i][j]+=dp[i-1][k];
32         }
33     }
34 }
35 
36 
37 int cal(int n)
38 {
39     memset(bit,0,sizeof(bit));
40     int len=0;
41     while(n)
42     {
43         bit[++len]=n%10;
44         n/=10;
45     }
46     int ans=sum[len-1], i=len;
47     for( ; i>0; i--)
48     {
49         for(int j=0; j<bit[i]; j++) //不能超过bit[i]
50             if( abs(bit[i+1]-j)>=2 )
51                 ans+=dp[i][j];
52         if( i<len && abs(bit[i+1]-bit[i])<2 )
53             break;      //上一位和这位已经构成非法
54     }
55     if(i==0)    ans++;
56     return ans;
57 }
58 
59 int main()
60 {
61     //freopen("input.txt","r",stdin);
62     pre_cal();
63     int L, R;
64     while(~scanf("%d%d",&L,&R))
65         printf("%d\n",cal(R)-cal(L-1));
66     return 0;
67 }
代码
```

