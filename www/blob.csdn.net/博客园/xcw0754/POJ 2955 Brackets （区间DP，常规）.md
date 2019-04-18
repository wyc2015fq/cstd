# POJ 2955  Brackets  （区间DP，常规） - xcw0754 - 博客园
# [POJ 2955  Brackets  （区间DP，常规）](https://www.cnblogs.com/xcw0754/p/4841323.html)
题意：
　　给出一个字符串，其中仅仅含 “ ( ) [ ] ” 这4钟符号，问最长的合法符号序列有多长？（必须合法的配对，不能混搭）
思路：
　　区间DP的常规问题吧，还是枚举区间[i->j]再枚举其中第k个与第i个来配对，如果配对了就+2这样子。
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
11 #define INF 0x3f3f3f3f
12 #define LL  long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=110;
16 char s[N];
17 int dp[N][N];
18 
19 bool ispair(char c1,char c2)
20 {
21     if( c1=='('&&c2==')' )   return true;
22     if( c1=='['&&c2==']' )   return true;
23     return false;
24 }
25 
26 int cal(int n)
27 {
28     memset(dp, 0, sizeof(dp));
29     for(int j=2; j<=n; j++ )
30     {
31         for(int i=j-1; i>0; i--)
32         {
33             dp[i][j]=dp[i+1][j];
34             for(int k=i+1; k<=j; k++)   //枚举与s[i]配对的另一半括号
35             {
36                 if( ispair(s[i],s[k]) )
37                     dp[i][j]=max( dp[i][j], dp[i+1][k-1]+dp[k+1][j]+1  );
38             }
39         }
40     }
41     return dp[1][n]<<1;
42 }
43 
44 int main()
45 {
46     //freopen("input.txt", "r", stdin);
47     while(1)
48     {
49         gets(s+1);
50         if(s[1]=='e')   break;
51         printf("%d\n", cal(strlen(s+1)));
52 
53     }
54     return 0;
55 }
AC代码
```

